# Output binary name
bin=da

# Set the following to '0' to disable log messages:
LOGGER ?= 1

# Compiler/linker flags
CFLAGS += -g -Wall -fPIC -DLOGGER=$(LOGGER)
LDLIBS +=
LDFLAGS +=

all: $(bin) libelist.so

$(bin): da.o elist.o util.o
	$(CC) $(CFLAGS) $(LDLIBS) $(LDFLAGS) $^ -o $@

libelist.so: elist.o
	$(CC) $(CFLAGS) $(LDLIBS) $(LDFLAGS) $^ -shared -o $@

docs: Doxyfile
	doxygen

clean:
	rm -f $(bin) da.o elist.o util.o libelist.so
	rm -rf docs

# Individual dependencies --
da.o: da.c logger.h util.h elist.h
elist.o: elist.c elist.h logger.h
util.o: util.c util.h logger.h


# Tests --
test: $(bin) libelist.so ./tests/run_tests
	@DEBUG="$(debug)" ./tests/run_tests $(run)

testupdate: testclean test

./tests/run_tests:
	rm -rf tests
	git clone https://github.com/usf-cs521-sp21/P1-Tests.git tests

testclean:
	rm -rf tests
