#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

long count = 0;

long long factorial(long long n)
{
	count++;
	if (n == 1) return 1;
	else return factorial(n - 1) * n;
}

int main(int argc, char *argv[])
{
	int n = (argc == 1) ? 62 : atoi(argv[1]);
	clock_t start, end;

	start = clock();
	long long f = factorial(n);
	end = clock();

	setlocale(LC_NUMERIC, "");
	printf("execution time: %0.6f secs.\t",
			(float) (end - start) / (float) CLOCKS_PER_SEC);
	printf("%s(%'d)=%'llu in %'lu\n", *argv, n, f, count);
}
