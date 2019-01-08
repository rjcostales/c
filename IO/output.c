#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "io.h"

int main(int argc, char *argv[])
{
    char string[MAX] = { '\0' };

    srand(time(0));

    for (int i = 0; i < MAX - 1; i++) {
        string[i] = rand() % 95 + ' ';
    }
    printf("%s\n", string);
}
