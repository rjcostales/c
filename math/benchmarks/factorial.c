#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

long count = 0;

double factorial(double n)
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
	double f = factorial(n);
	end = clock();

	setlocale(LC_NUMERIC, "");
	printf("execution time: %0.6f secs.\t",
			(float) (end - start) / (float) CLOCKS_PER_SEC);
	printf("%s(%'d)=%'0.0f in %'ld\n", *argv, n, f, count);
}
