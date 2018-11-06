#include <stdio.h>
#include <stdlib.h>

int count = 0;

int fibonacci(int n)
{
    count++;

    switch (n) {
    case 0:
	return 0;
    case 1:
	return 1;
    case 2:
	return 1;
    case 3:
	return 2;
    default:
	return fibonacci(n - 3) * 3 + fibonacci(n - 4) * 2;
    }
}

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);

    if (n < 0 || n > 47)
	n = 0;

    int f = fibonacci(n);
    printf("%u\t%u\n", f, count);
}
