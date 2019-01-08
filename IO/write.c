#include <stdio.h>
#include "io.h"

int seed = 1234567890;
int a = 1664525;
int c = 1013904223;
char characters[] =
    "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz";

int rand()
{
    seed = (a * seed + c) & 0xffffffff;
    return seed;
}

int main(int argc, char **argv)
{
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
    return seed;
}
