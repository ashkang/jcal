#include <stdio.h>
#include <time.h>
#include <jalali/jalali.h>
#include <jalali/jtime.h>

int main() {
    time_t t;
    time(&t);
    struct jtm j;
    jlocaltime_r(&t, &j);
    jalali_show_time(&j);
    printf("%d <-> %d\n", (int) jmktime(&j), (int) t);  
    return 0;
}

