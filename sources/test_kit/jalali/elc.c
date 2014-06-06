#include <stdio.h>
#include <stdlib.h>
#include <jalali/jalali.h>

int main(int argc, char** argv)
{
    if (argc != 3) {
	printf("wrong arguments given\n");
	printf("elc: leap counter from YEAR1 to YEAR2\n");
	printf("usage: elc YEAR1 YEAR2\n");
	exit(1);
    }

    int i, s = atoi(argv[1]), e = atoi(argv[2]), c = 0;

    if (s > e) {
	/* tricky swap */
	s = s+e;
	e = s-e;
	s = s-e;
    }

    printf("leap years from %d to %d... ", s, e);

    for (i=s; i<=e; i++) {
	if (jalali_is_jleap(i))
	    c++;
    }

    printf("%d.\n", c);
    exit(0);
}

