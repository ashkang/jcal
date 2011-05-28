#include <stdio.h>
#include <jalali.h>

int main(int argc, char** argv)
{
    int i, s = atoi(argv[1]), e = atoi(argv[2]), c = 0;
    printf("leap years from %d to %d... ", s, e);

    for (i=s; i<=e; i++) {
	if (jalali_is_jleap(i))
	    c++;
    }

    printf("%d.\n", c);
}

