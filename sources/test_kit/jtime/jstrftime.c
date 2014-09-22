#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jalali.h"
#include "jtime.h"

int main(int argc, char** argv) {
    if (argc != 2) {
    printf("wrong arguments given\n");
    printf("usage jstrftime FORMAT\n");
    exit(1);
    }

    const char* fmt = argv[1];
    char buf[32];
    time_t t;
    size_t s;
    time(&t);
    struct jtm j;
    jlocaltime_r(&t, &j);
    jalali_show_time(&j);
    s = jstrftime(buf, 32, fmt, &j);
    printf("%s:%d\n", buf, (int)s);

    exit(0);
}
