#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main()
{
	unsetenv("LC_ALL");
	setlocale(LC_NUMERIC, "");

	int bignum = 1000000000;

	printf("%'u\n", bignum);
	printf("%'i\n", bignum);
	printf("%'d\n", bignum);

	bignum = 2000000000;

	printf("%'u\n", bignum);
	printf("%'i\n", bignum);
	printf("%'d\n", bignum);

	bignum = 4000000000;

	printf("%'u\n", bignum);
	printf("%'i\n", bignum);
	printf("%'d\n", bignum);

	printf("%'u\n", (int) 5000000000);
	printf("%'i\n", (int) 5000000000);
	printf("%'d\n", (int) 5000000000);

	printf("%'u\n", (int) 0xffffffffffff);
	printf("%'i\n", (int) 0xffffffffffff);
	printf("%'d\n", (int) 0xffffffffffff);
}
