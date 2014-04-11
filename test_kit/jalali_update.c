#include <stdio.h>
#include <time.h>
#include <jalali/jalali.h>
#include <jalali/jtime.h>

int main(int argc, char** argv) {
    time_t t;
    struct jtm j;
    int mday_diff = 60;
    time(&t);
    jlocaltime_r(&t, &j);
    jalali_show_time(&j);
    printf("After %d days: \n", mday_diff);
    j.tm_mday += mday_diff;
    jalali_update(&j);
    jalali_show_time(&j);
    return 0;
}
