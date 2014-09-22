#include <stdio.h>
#include <time.h>
#include "jalali.h"
#include "jtime.h"

int main() {
    time_t t;
    time(&t);
    struct jtm j;
    jlocaltime_r(&t, &j);
    jalali_show_time(&j);
    printf("%d <-> %d\n", (int) jmktime(&j), (int) t);
    return 0;
}
