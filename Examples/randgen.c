#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    unsigned int rand[20];
    int urandom = open("/dev/urandom", O_RDONLY);

    read(urandom, rand, 50);

    for (int i = 0; i < 20; i++)
        printf("%u\n", rand[i]);

    close(urandom);
}
