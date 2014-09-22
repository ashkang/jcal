#include <stdio.h>
#include <stdlib.h>
#include "jalali.h"

int main(int argc, char** argv)
{
    if (argc != 2) {
    printf("wrong arguments given\n");
    printf("usage: leap YEAR\n");
    exit(1);
    }

    printf("year %d leap status: %d.\n", atoi(argv[1]), jalali_is_jleap(atoi(argv[1])));

    exit(0);
}
