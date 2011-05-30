#include <stdio.h>
#include <time.h>
#include <jalali/jalali.h>
#include <jalali/jtime.h>

const char* fmt = "%e | %F | %c | %s | u.";
int main() {
    char buf[32];
    time_t t;
    size_t s;
    time(&t);
    struct jtm j;
    jlocaltime_r(&t, &j);
    jalali_show_time(&j);
    s = jstrftime(buf, 32, fmt, &j);
    printf("%s:%d", buf, (int)s);
    return 0;
}

