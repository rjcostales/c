#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int i, j;

    srand(time(0));

    for (i = 0; i < 32; i++) {
	printf("%i\n", rand());
    }
}
