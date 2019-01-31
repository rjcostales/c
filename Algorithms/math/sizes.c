#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <limits.h>
#include <float.h>

int main(int argc, char *argv[])
{
    int i;
    short s;

    printf("size of int\t:%lu\n", sizeof(int));
    printf("size of char\t:%lu\n", sizeof(char));
    printf("size of short\t:%lu\n", sizeof(short));
    printf("size of long\t:%lu\n", sizeof(long));

    printf("size of float\t:%lu\n", sizeof(float));
    printf("size of double\t:%lu\n", sizeof(double));

    printf("size of uint32_t\t:%lu\n", sizeof(uint32_t));
    printf("size of uint64_t\t:%lu\n", sizeof(uint64_t));

    printf("long max\t:%li\n", LONG_MAX);
    printf("long min\t:%li\n", LONG_MIN);
    printf("long unsiged\t:%lu\n", ULONG_MAX);

    printf("long decimal\t:%ld\n", ULONG_MAX);
    printf("long hexadecimal:0x%lx\n", ULONG_MAX);
    printf("long octal\t:o%lo\n", ULONG_MAX);

    printf("float\t:%f\t%f\n", FLT_MIN, FLT_MAX);
    printf("double\t:%f\t%f\n", DBL_MIN, DBL_MAX);

    printf("float\t:%le\n", 1.0 / FLT_MAX);
    printf("double\t:%le\n", 1.0 / DBL_MAX);
}
