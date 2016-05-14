#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <limits.h>
#include <float.h>

int main(int argc, char *argv[])
{
    int i;
    char c;
    short s;

    printf("size of int:\t%i\n", sizeof(int));
    printf("size of char:\t%i\n", sizeof(char));
    printf("size of short:\t%i\n", sizeof(short));
    printf("size of long:\t%i\n", sizeof(long));

    printf("size of float:\t%i\n", sizeof(float));
    printf("size of double:\t%i\n", sizeof(double));

    printf("size of uint32_t:\t%i\n", sizeof(uint32_t));
    printf("size of uint64_t:\t%i\n", sizeof(uint64_t));

    printf("long :\t%i\n", LONG_MAX);
    printf("long :\t%u\n", ULONG_MAX);

    printf("float :\t%f\t%f\n", FLT_MIN, FLT_MAX);
    printf("double :\t%f\t%f\n", DBL_MIN, DBL_MAX);
}
