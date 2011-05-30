#include <stdio.h>
#include <jalali/jalali.h>

int main(int argc, char** argv)
{
    if (argc != 2)
	printf("usage: test_leap YEAR.\n");
    else
	printf("year %d leap status: %d.\n", atoi(argv[1]), jalali_is_jleap(atoi(argv[1])));
    
    return 0;
}
