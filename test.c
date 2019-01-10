#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char number[] = "0123456789";
    char upper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char lower[] = "abcdefghijklmnopqrstuvwxyz";

    char *p = &lower[5];

    *p = 'z';

    printf("%s\n", lower);
    printf("%s\n", p++);
    printf("%s\n", p++);
}
