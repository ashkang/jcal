#include <stdio.h>
#include <time.h>
#include <jalali/jalali.h>
#include <jalali/jtime.h>

int main() {
    time_t t;
    time(&t);
    struct jtm j;
    char buf[2048];
    printf("%s", jctime(&t));
    jctime_r(&t, buf);
    printf("%s", buf);
    return 0;
}

