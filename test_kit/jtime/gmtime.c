#include <stdio.h>
#include <time.h>
#include <jalali.h>
#include <jtime.h>

int main() {
    time_t t;
    time(&t);
    struct jtm j;
    struct jtm* p;
    p = jgmtime(&t);
    jgmtime_r(&t, &j);
    jalali_show_time(p);
    jalali_show_time(&j);
}

