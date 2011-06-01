#include <stdio.h>
#include <stdlib.h>
#include <jalali/jalali.h>

int main(int argc, char** argv)
{
    if (argc != 2) {
	printf("wrong arguments given\n");
	printf("usage: get_date DIFF_FROM_EPOCH_BY_DAYS\n");
	exit(1);
    }

    int p = atoi(argv[1]);
    struct jtm j;
    jalali_get_date(p, &j);
    jalali_create_date_from_days(&j);

    j.tm_hour = 0;
    j.tm_min = 0;
    j.tm_sec = 0;

    jalali_show_time(&j);

    exit(0);
}
