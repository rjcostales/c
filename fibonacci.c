#include <stdio.h>
#include <stdlib.h>

long fibonacci(int n)
{
    if (n == 0)
	return 0;
    if (n == 1)
	return 1;
    return fibonacci(n - 2) + fibonacci(n - 1);
}

int main(int argc, char *argv[])
{
    int n = 47;
    if (argc == 2)
	n = atoi(argv[1]);
    if (n < 0 || n > 47)
	n = 0;

    long f = fibonacci(n);
    printf("fib(%i) = %lu\n", n, f);
}
