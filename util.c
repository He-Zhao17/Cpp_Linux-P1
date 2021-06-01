#include "util.h"

#include <stdio.h>
#include <time.h>
#include <string.h>

void human_readable_size(char *buf, size_t buf_sz, double size, unsigned int decimals)
{


}

size_t simple_time_format(char *buf, size_t buf_sz, time_t time)
{
    struct tm *tmtime = localtime(time);
    strftime(buf, buf_sz, "    %b %d %Y", tmtime);
    return strlen(buf);
}
