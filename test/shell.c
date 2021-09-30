/*
 * C Program To Sort page in ascending order using Shell Sort. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 50

static inline
void swap(int *a, int *b) { int tmp = *a; *a = *b; *b = tmp; }

void shell(int array[], int n)
{
	for (int i = n / 2; i > 0; i /= 2) {
		for (int j = i; j < n; j++) {
			for (int k = j - i; k >= 0; k = k - i) {
				if (array[k + i] > array[k]) break;
				else swap(&array[k], &array[k + i]);
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int page[SIZE];

	printf("%s - %i\n", argv[0], SIZE);

	srand(time(NULL));
	for (int i = 0; i < SIZE; i++) page[i] = rand();

	shell(page, SIZE);

	for (int i = 0; i < SIZE; i++) printf("%i\n", page[i]);

	return 0;
}
