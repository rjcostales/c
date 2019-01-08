#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int random = open("/dev/random", O_RDONLY);

    int randint[25];
    for (int i = 0; i < 25; i++)
        randint[i] = -1;

    read(random, randint, 99);

    for (int i = 0; i < 25; i++)
        printf("%2d %08X\n", i + 1, randint[i]);


    close(random);
}
