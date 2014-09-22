#include <stdio.h>
#include <stdlib.h>
#include "jalali.h"

int main(int argc, char** argv)
{
    struct jyinfo a;

    if (argc != 2) {
    printf("wrong arguments given\n");
    printf("usage: jyinfo YEAR\n");
    exit(1);
    }

    a.y = atoi(argv[1]);
    jalali_get_jyear_info(&a);
    fprintf(stdout, "Year %d: lf = %d, apl = %d, pl = %d, rl = %d, p = %d, r = %d.\n", a.y, a.lf,
        a.apl, a.pl, a.rl, a.p, a.r);

    exit(0);
}
