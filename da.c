#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include "elist.h"

#include "logger.h"

#define __MAX = 256

/* Forward declarations: */
void print_usage(char *argv[]);

struct f{
    unsigned long size;
    char* path;
    time_t accTime;
};



void tDir(struct elist* list, char* path) {

    DIR *dir = NULL;
    struct dirent *currentDir = NULL;

    char p[256];

    dir = opendir(path);

    // loop
    while ( (currentDir = readdir(dir)) != NULL) {
        // remove read finish
        if ( (!strncmp(currentDir->d_name, ".", 1)) || (!strncmp(currentDir->d_name, "..", 2)) ) {
            continue;
        }
        snprintf(p, sizeof(p) - 1, "%s/%s", path, currentDir->d_name);
        struct stat info;
        stat(p, &info);
        time_t tempT = info.st_atimespec.tv_sec;
        struct f temp = {info.st_size, p, tempT};
        if (S_ISDIR(info.st_mode)) {
            tDir(list, p);
        } else {
            elist_add(list, &temp);
        }
    }
    closedir(dir);
}


void print_usage(char *argv[]) {
fprintf(stderr, "Disk Analyzer (da): analyzes disk space usage\n");
fprintf(stderr, "Usage: %s [-ahs] [-l limit] [directory]\n\n", argv[0]);

fprintf(stderr, "If no directory is specified, the current working directory is used.\n\n");

fprintf(stderr, "Options:\n"
"    * -a              Sort the files by time of last access (descending)\n"
"    * -h              Display help/usage information\n"
"    * -l limit        Limit the output to top N files (default=unlimited)\n"
"    * -s              Sort the files by size (default, ascending)\n\n"
);
}

int main(int argc, char *argv[])
{
    /* Create a struct to hold program options and initialize it by declaring an
     * 'options' variable. Defaults:
     *      - sort by size (time=false)
     *      - limit of 0 (unlimited)
     *      - directory = '.' (current directory) */
    struct da_options {
        bool sort_by_time;
        unsigned int limit;
        char *directory;
    } options
        = { false, 0, "." };

    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "ahl:s")) != -1) {
        switch (c) {
            case 'a':
                options.sort_by_time = true;
                break;
            case 'h':
                print_usage(argv);
                return 0;
            case 's':
                options.sort_by_time = false;
                break;
            case 'l': {
                /*    ^-- to declare 'endptr' here we need to enclose this case
                 *    in its own scope with curly braces { } */
                char *endptr;
                long llimit = strtol(optarg, &endptr, 10);
                if (llimit < 0 || llimit > INT_MAX || endptr == optarg) {
                    fprintf(stderr, "Invalid limit: %s\n", optarg);
                    print_usage(argv);
                    return 1;
                }
                options.limit = (int) llimit;
                break;
                }
            case '?':
                if (optopt == 'l') {
                    fprintf(stderr,
                            "Option -%c requires an argument.\n", optopt);
                } else if (isprint(optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                } else {
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n", optopt);
                }
                print_usage(argv);
                return 1;
            default:
                abort();
        }
    }

    if (optind < argc) {
        options.directory = argv[optind];
    }

    LOGP("Done parsing arguments.\n");
    LOG("Sorting by: [%s], limit: [%u]\n",
            options.sort_by_time == true ? "time" : "size",
            options.limit);
    LOG("Directory to analyze: [%s]\n", options.directory);

    /* TODO:
     *  - check to ensure the directory actually exists
     *  - create a new 'elist' data structure
     *  - traverse the directory and store entries in the list
     *  - sort the list (either by size or time)
     *  - print formatted list
     */
    DIR *dir = opendir(options.directory);
    if (dir == NULL) {
        printf("Error: No such file or path.");
        return 0;
    } else {
        struct elist* list = elist_create(10, sizeof(struct f));
        tDir(list, options.directory);
        unsigned short cols = 80;
        struct winsize win_sz;
        if (ioctl(fileno(stdout), TIOCGWINSZ, &win_sz) != -1) {
            cols = win_sz.ws_col;
        }
        LOG("Display columns: %d\n", cols);




    }



    return 0;
}
