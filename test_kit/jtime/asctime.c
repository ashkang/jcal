#include <stdio.h>
#include <time.h>
#include <jalali/jalali.h>
#include <jalali/jtime.h>

int main() {
    time_t ltime;
    time(&ltime);
    struct ab_jtm a;
    struct jtm j;
    jalali_create_time_from_secs(ltime, &a);
    jalali_get_date(a.ab_days, &j);
    j.tm_sec = a.ab_sec;
    j.tm_min = a.ab_min;
    j.tm_hour = a.ab_hour;
    jalali_create_date_from_days(&j);
    printf("%s", jasctime(&j));
    return 0;
}

    
