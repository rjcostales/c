#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define SIZE 50000

inline void swap(float *a, float *b) { float tmp = *a; *a = *b; *b = tmp; }

void bubble(float array[], int n)
{
	for (int i = 0; i < n - 1; ++i) {
		for (int j = 0; j < n - i - 1; ++j) {
			if (array[j] > array[j + 1]) {
				swap(&array[j], &array[j + 1]);
			}
		}
	}
}

void insertion(float array[], int n)
{
	int i, j;

	for (i = 1; i < n; i++) {
		float tmp = array[i];
		j = i;
		while (j && (array[j - 1] > tmp)) {
			array[j] = array[j - 1];
			j--;
		}
		array[j] = tmp;
	}
}

void selection(float array[], int n)
{
	int min;

	for (int i = 0; i < n; i++) {
		min = i;
		for (int j = i; j < n; j++) {
			if (array[min] > array[j])
				min = j;
		}
		swap(&array[i], &array[min]);
	}
}

void shell(float array[], int n)
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
	float array[SIZE];

	int size = SIZE;
	int half = RAND_MAX / 2;

	srand(time(NULL));

	for (int i = 0; i < size; i++) array[i] = ((float) rand() - half) / half;

	selection(array, size);

	for (int i = 0; i < size; i++) printf("%i\t%0.4f\n", i, array[i]);

	return 0;
}
