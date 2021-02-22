/* C Program To Sort page in ascending order using Shell Sort. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define SIZE 1000000

#define ELAPSE_TIME(X) ((float) (X) / (float) CLOCKS_PER_SEC)

static inline
void swap(int *a, int *b) { int tmp = *a; *a = *b; *b = tmp; }

static inline
void merge(int* array, int* left, int* right, int size_l, int size_r)
{
	int  a = 0, l = 0, r = 0;

	while (l < size_l && r < size_r) {
		if (left[l] < right[r])
			array[a++] = left[l++];
		else
			array[a++] = right[r++];
	}

	// copy the remainders
	while (l < size_l)
		array[a++] = left[l++];

	while (r < size_r)
		array[a++] = right[r++];
}

void merge_sort(int array[], int size)
{
	if (size <= 1) return; // nothing to sort

	// divide
	int size_l = size / 2;
	int size_r = size - size_l;

	// vla - core dumping with sizes larger than 600,000
	// int left[size_l];
	// int right[size_r];
	int* left = calloc(size_l, sizeof(int));
	int* right = calloc(size_r, sizeof(int));

	memcpy(left, array, size_l * sizeof(int));
	memcpy(right, array + size_l, size_r * sizeof(int));

	// sort left and right
	merge_sort(left, size_l);
	merge_sort(right, size_r);

	merge(array, left, right, size_l, size_r);

	// deallocate
	free(left);
	free(right);
}

void shell_sort(int array[], int n)
{
	for (int size_l = n / 2; size_l > 0; size_l /= 2) {
		for (int j = size_l; j < n; j++) {
			for (int a = j - size_l; a >= 0; a = a - size_l) {
				if (array[a+size_l] > array[a])
					break;
				 else {
					swap(&array[a], &array[a+size_l]);
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int page[SIZE];
	clock_t start, end;

	setlocale(LC_NUMERIC, "");
	int seed = time(NULL);

	printf("%s - %i\n", argv[0], SIZE);

	srand(seed);
	for (int i = 0; i < SIZE; i++ ) page[i] = rand();

	start = clock();
	merge_sort(page, SIZE);
	end = clock();
	fprintf(stdout,
			  "merge sort execution time: %0.6f secs.\n",
			  ELAPSE_TIME(end - start));

	FILE* f1 = fopen("merge.dat", "w");
	for (int i = 0; i < SIZE; i++ )
		fprintf(f1, "%i\n", page[i]);
	fclose(f1);

	srand(seed);
	for (int i = 0; i < SIZE; i++ ) page[i] = rand();

	start = clock();
	shell_sort(page, SIZE);
	end = clock();
	fprintf(stdout,
		 	  "shell sort execution time: %0.6f secs.\n",
			  ELAPSE_TIME(end - start));

	FILE* f2 = fopen("shell.dat", "w");
	for (int i = 0; i < SIZE; i++ )
		fprintf(f2, "%i\n", page[i]);
	fclose(f2);

	return 0;
}
