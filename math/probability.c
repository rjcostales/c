#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int func(int n)
{
	int r = 0;

	for (int i = 0; i <  n; i++) {
		(rand() % 2) ? r++ : r--;
	}
	return r / 2;
}

void display(int array[], int i)
{
	int t = 0;

	for (i = 0; i < 256; i++) {
		int n = array[i];
		t += n;
		printf("%d\t%d\t%d\n", i, n, t);
	}
}

int main(int argc, char *argv[])
{
	// init the random number generator
	time_t t;
	srand((unsigned) time(&t));

	int array[256] = {0};
	int n;

	for (int i = 0; i < 1000000; i++) {
		n = func(1024) + 128;
		array[n]++;
	}

	display(array, 256);
}
