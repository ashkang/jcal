#include <stdio.h>
#include <stdlib.h>
#include "jalali.h"

int main(int argc, char** argv)
{
    if (argc != 2) {
    printf("wrong arguments given\n");
    printf("usage: sec_converter SECONDS\n");
    exit(1);
    }

    struct ab_jtm a;
    int s;
    s = atoi(argv[1]);
    jalali_create_time_from_secs(s, &a);
    printf("%d secs passed, time created: %d days, %02d:%02d:%02d.\n", s, a.ab_days, a.ab_hour, a.ab_min, a.ab_sec);

    exit(0);
}
