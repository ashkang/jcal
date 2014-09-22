#include <stdio.h>
#include <stdlib.h>
#include "jalali.h"

int main(int argc, char** argv)
{
    if (argc != 4) {
    printf("wrong arguments given\n");
    printf("usage: get_diff DAY MONTH YEAR\n");
    exit(1);
    }

    int d = atoi(argv[1]);
    int m = atoi(argv[2]);
    int y = atoi(argv[3]);

    struct jtm j;
    j.tm_mday = d;
    j.tm_mon = m - 1;
    j.tm_year = y;
    j.tm_hour = 0;
    j.tm_min = 0;
    j.tm_sec = 0;

    jalali_create_days_from_date(&j);
    jalali_update(&j);
    jalali_show_time(&j);
    printf("diff = %d\n", jalali_get_diff(&j));

    exit(0);
}
