#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "io.h"

int main(int argc, char * argv[])
{
    char string[MAX];

    for (int n = 0; n < LEN; n++)
        fgets(string, MAX, stdin);

    return 0;
}
