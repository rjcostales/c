#include <stdio.h>
#include "io.h"

// Numeric Recipies
// int a    = 1664525;
// int c    = 1013904223;

// int seed = 0;
int seed = 1234567890;
// primes
const int a    = 655541;
const int c    = 2147483629;

int rand()
{
    seed = (a * seed + c) & 0xffffffff;
    return seed;
}

int main(int argc, char *argv[])
{
    const char characters[] =
        "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz";
    char buffer[BUF] = { '\0' };

    for (int i = 0; i < LEN; i++) {
        char *ptr = &buffer[0];
        for (int n = 0; n < 200; n++) {
            int r = rand();
            *ptr++ = characters[r & 0x3f]; r >>= 6;
            *ptr++ = characters[r & 0x3f]; r >>= 6;
            *ptr++ = characters[r & 0x3f]; r >>= 6;
            *ptr++ = characters[r & 0x3f]; r >>= 6;
            *ptr++ = characters[r & 0x3f]; r >>= 6;
        }
        buffer[NDX] = '\n';
        fputs(buffer, stdout);
    }
    return 0;
}
