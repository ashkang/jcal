#include <stdio.h>
#include <jalali/jalali.h>

int main(int argc, char** argv)
{
    int p = atoi(argv[1]);
    struct jtm j;
    jalali_get_date(p, &j);
    jalali_create_date_from_days(&j);
    jalali_show_time(&j);
}
