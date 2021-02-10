/* C Program To Sort page in ascending order using Bubble Sort. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "fileutils.h"

void merge(str array[], str left[], str right[], int size_l, int size_r)
{
	int i = 0, j = 0, k = 0;

	while (i < size_l && j < size_r) {
		if (strcmp(left[i], right[j]) < 0)
			array[k++] = left[i++];
		else
			array[k++] = right[j++];
	}

	while (i < size_l)
		array[k++] = left[i++];

	while (j < size_r)
		array[k++] = right[j++];
}

void merge_sort(str array[], int size)
{
	if (size > 1) {

		int i = size / 2;
		int j = size - i;

		str left[i];
		str right[j];

		memcpy(left, array, i * sizeof(str));
		memcpy(right, array + i, j * sizeof(str));

		// for (int n = 0; n < i; n++) left[n] = array[n];
		// str *tmp= &array[i];
		// for (int n = 0; n < j; n++) right[n] = tmp[n];

		merge_sort(left, i);
		merge_sort(right, j);

		merge(array, left, right, i, j);
	}
}

int main(int argc, char *argv[])
{
	str page[MAXSIZE];
	int size = read(page);
	fprintf(stderr, "%s %d records\n", argv[0], size);

	clock_t start, end;
	start = clock();

	merge_sort(page, size);

	end = clock();

	setlocale(LC_NUMERIC, "");
	fprintf(stderr, "execution time: %0.6f secs.\n", ELAPSE_TIME(end - start));

#ifdef WRITE
	write(page);
#endif

	return 0;
}
