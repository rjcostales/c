#include <stdio.h>
#include <stdlib.h>

int count = 0;

int fibonacci(int n, int a, int b)
{
    count++;

    if (n == 0)
	return a;
    if (n == 1)
	return b;
    return fibonacci(n - 1, b, a + b);
}

int main(int argc, char *argv[])
{
    int n = 47;
    if (argc == 2)
	n = atoi(argv[1]);
    if (n < 0 || n > 47)
	n = 0;

    int f = fibonacci(n, 0, 1);
    printf("fib(%1) = %u - %u\n", n, f, count);
}
