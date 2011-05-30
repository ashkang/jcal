#include <stdio.h>
#include "jalali/jalali.h"

int main(int argc, char** argv)
{
    struct ab_jtm a;
    int s;
    s = atoi(argv[1]);
    jalali_create_time_from_secs(s, &a);
    printf("%d secs passed, time created: %d days, %d:%d:%d.\n", s, a.ab_days, a.ab_hour, a.ab_min, a.ab_sec);
    return 0;
}
