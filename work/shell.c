/*
 * C Program To Sort page in ascending order using Shell Sort.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 64

static inline void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void shell(int array[], int n)
{
	int count = 0;
	for (int i = n / 2; i > 0; i /= 2) {
		for (int j = i; j < n; j++) {
			for (int k = j - i; k >= 0; k = k - i) {

				// printf("i = %d,\tj = %d,\tk = %d\n", i, j, k);

				if (array[k + i] > array[k]) break;
				else {
					swap(&array[k], &array[k + i]);
					for (int a = 0; a < SIZE; a++) printf("%i|", array[a]);
					printf("\t%i\t%i,%i\n", count++, k + i, k);
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int page[SIZE];

	printf("%s - %i\n", argv[0], SIZE);

	srand(time(NULL));
	for (int i = 0; i < SIZE; i++)
		page[i] = rand() % 100;

	shell(page, SIZE);

	// for (int i = 0; i < SIZE; i++) printf("%i|", page[i]);
	// printf("\n");

	return 0;
}
