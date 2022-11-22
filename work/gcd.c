#include <stdio.h>

long gcd(long a, long b)
{
	if (b != 0)
		return gcd(b, a % b);
	else
		return a;
}

int main(int argc, char *argv[])
{
	long n1, n2;

	printf("Enter 2 integers separated by a comma: ");
	scanf("%ld,%ld", &n1, &n2);

	int min = (n1 < n2) ? n1 : n2;
	int max = (n1 >= n2) ? n1 : n2;
	int loops = 10;
	for (int i = min; i; i = max % min) {
		max = min;
		min = max % i;
		printf("%d %d\n", max, min);
		if (loops--) break;
	}
	return 0;
}
