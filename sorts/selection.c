/* C Program To Sort page in ascending order using Selection Sort. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "fileutils.h"

inline void swap(str *a, str *b) { str tmp = *a; *a = *b; *b = tmp; }

void selection_sort(str array[], int n)
{
	int min;

	for (int i = 0; i < n; i++) {
		min = i;
		for (int j = i; j < n; j++) {
			if (strcmp(array[min], array[j]) > 0)
				min = j;
		}
		swap(&array[i], &array[min]);
	}
}

int main(int argc, char *argv[])
{
	str page[MAXSIZE];
	int size = read(page);
	fprintf(stderr, "%s %d records\n", argv[0], size);

	// selection sort
	clock_t start, end;
	start = clock();

	selection_sort(page, size);

	end = clock();

	setlocale(LC_NUMERIC, "");
	fprintf(stderr, "execution time: %0.6f secs.\n", ELAPSE_TIME(end - start));

#ifdef WRITE
	write(page);
#endif

	return 0;
}
