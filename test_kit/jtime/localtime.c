#include <stdio.h>
#include <time.h>
#include <jalali/jalali.h>
#include <jalali/jtime.h>

int main() {
    time_t t;
    time(&t);
    struct jtm* p;
    struct jtm j;
    p = jlocaltime(&t);
    jalali_show_time(p);
    jlocaltime_r(&t, &j);
    jalali_show_time(&j);
    return 0;
}

