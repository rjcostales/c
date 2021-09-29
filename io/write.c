#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "io.h"

int main(int argc, char* argv[])
{
    char string[BUF] = { '\0' };

    srand(time(0));

    for (int i = 0; i < BUF; i++)
        string[i] = rand() % 95 + ' ';

    for (int n = 0; n < LEN; n++)
        puts(string);

    return 0;
}
