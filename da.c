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
#include "util.h"

#include "logger.h"

/**
* Default max size of the path length.
*/
#define __MAX = 256

/* Forward declarations: */

/**
* @brief		The function to get the tips.
* @details	    The function to get the tips.
* @param[in]	argv The command line in.
* @return	   None.
*/
void print_usage(char *argv[]);

/**
* The struct of the element in elist about documents.
*/
struct f{
    unsigned long size;
    char* path;
    time_t accTime;
};


/**
* @brief		To traverse a path and write into a elist.
* @details	    To traverse a path and write into a elist recursively.
* @param[in]	list The elist we want to write into.
* @param[in]    path The path we want to traverse.
* @return       None.
*/
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

/**
* @brief		The comparator function to sort with last accessed time.
* @details	    The comparator function to sort with last accessed time.
* @param[in]	a First argument.
* @param[in]    b Second argument.
* @return       If b is after a, return -1, if b and a is equivalent, return 0, else return 1.
*/
int cmptf(const void *a, const void *b) {
    struct f* sa = (struct f*) a;
    struct f* sb = (struct f*) b;
    return sb->accTime - sa->accTime;
}

/**
* @brief		The comparator function to sort with last accessed size.
* @details	    The comparator function to sort with last accessed size.
* @param[in]	a First argument.
* @param[in]    b Second argument.
* @return       If b is after a, return -1, if b and a is equivalent, return 0, else return 1.
*/
int cmpsf(const void *a, const void *b) {
    struct f* sa = (struct f*) a;
    struct f* sb = (struct f*) b;
    return sb->size - sa->size;
}

/**
* @brief		The function to get the tips.
* @details	    The function to get the tips.
* @param[in]	argv The command line in.
* @return	   None.
*/
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
        if (options.sort_by_time) {
            elist_sort(list,cmptf);
        } else {
            elist_sort(list,cmpsf);
        }
        int widPath = 80 - 29;
        for (int i = 0; i < options.limit; i++) {
            struct f* temp = (struct f*) elist_get(list, i);
            char *p = (char*) malloc (widPath + 1);
            if (strlen(temp->path) > widPath) {
                snprintf(p, widPath, "...%s", strlen(temp->path) - widPath + 4 + temp->path);
            } else {
                for (int j = 0; j < widPath - strlen(temp->path); j++) {
                    snprintf(p + j, 1, " ");
                }
                snprintf(p + widPath - strlen(temp->path), strlen(temp->path), "%s",temp->path);
            }
            char *s = (char*) malloc (15);
            human_readable_size(s, 14, (double) temp->size, 1);
            char *at = (char*) malloc (16);
            simple_time_format(at, 15, temp->accTime);
            fprintf(stderr, "%s%s%s", p, s, at);
        }
    }
    return 0;
}
