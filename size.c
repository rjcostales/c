#include <stdio.h>

int main(int argc, char *argv[])
{
    char c;
    short s;
    int i;
    long l;

    printf("char  %lu\n", sizeof(c));
    printf("short %lu\n", sizeof(s));
    printf("int   %lu\n", sizeof(i));
    printf("long  %lu\n", sizeof(l));
}
