/* C Program To Sort page in ascending order using Insertion Sort. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "fileutils.h"

void insertion_sort(str array[], int n)
{
	int i, j;

	for (i = 1; i < n; i++) {
		str tmp = array[i];
		j = i;
		while (j && (strcmp(array[j - 1], tmp) > 0)) {
			array[j] = array[j - 1];
			j--;
		}
		array[j] = tmp;
	}
}

int main(int argc, char *argv[])
{
	str page[MAXSIZE];
	int size = read(page);
	fprintf(stderr, "%s %d records\n", argv[0], size);

	clock_t start, end;
	start = clock();

	insertion_sort(page, size);

	end = clock();

	setlocale(LC_NUMERIC, "");
	fprintf(stderr, "execution time: %0.6f secs.\n", ELAPSE_TIME(end - start));

#ifdef WRITE
	write(page);
#endif

	return 0;
}
