#include <stdio.h>
#include <time.h>
#include <jalali.h>
#include <jtime.h>

int main() {
    time_t t;
    time(&t);
    struct jtm j;
    char buf[2048];
    printf("%s", jctime(&t));
    jctime_r(&t, buf);
    printf("%s", buf);
}

