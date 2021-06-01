#include "util.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <tgmath.h>
#include <malloc/_malloc.h>

void human_readable_size(char *buf, size_t buf_sz, double size, unsigned int decimals)
{
    int k = 0;
    double temp = size;
    while (temp > 1) {
        temp = temp / 1024;
        k++;
    }
    double res = size;
    if (k != 0) {
        k--;
        res = size / pow(1024, (double)k);
    }
    char *a = "111";
    if (k == 0) {
        a = "Byt";
    } else if ( k == 1 ) {
        a = "KiB";
    } else if ( k == 2 ) {
        a = "MiB";
    } else if ( k == 3 ) {
        a = "GiB";
    } else if ( k == 4 ) {
        a = "TiB";
    } else if ( k == 5 ) {
        a = "PiB";
    } else if ( k == 6 ) {
        a = "EiB";
    } else {
        a = "ZiB";
    }

    if (res >= 100 && res < 1000) {
        snprintf(buf, buf_sz, "     %.1lf %s", res, a);
        return;
    } else if (res >= 1000) {
        snprintf(buf, buf_sz, "    %.1lf %s", res, a);
        return;
    } else {
        snprintf(buf, buf_sz, "      %.1lf %s", res, a);
        return;
    }
}

size_t simple_time_format(char *buf, size_t buf_sz, time_t time)
{
    struct tm *tmtime = localtime(&time);
    strftime(buf, buf_sz, "    %b %d %Y", tmtime);
    return strlen(buf);
}
