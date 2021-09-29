#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    char string[1001] = { '\0' };

    srand(time(0));

    for (int i = 0; i < 1000; i++)
        string[i] = rand() % 95 + ' ';

    for (int n = 0; n < 1000000; n++)
        printf("%s", string);
}
