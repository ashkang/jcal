#include <stdio.h>
#include <jalali.h>

int main(int argc, char** argv)
{
    int d = atoi(argv[1]);
    int m = atoi(argv[2]);
    int y = atoi(argv[3]);

    struct jtm j;
    j.tm_mday = d;
    j.tm_mon = m;
    j.tm_year = y;

    jalali_create_days_from_date(&j);
    jalali_get_diff(&j);
}
