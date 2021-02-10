/* C Program To Sort array in ascending order using Shell Sort. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "fileutils.h"

void shell_sort(str array[], int n)
{
	int i, j, k;

	for (i = n / 2; i > 0; i /= 2) {
		for (j = i; j < n; j++) {
			for (k = j - i; k >= 0; k = k - i) {
				if (strcmp(array[k+i], array[k]) > 0)
					break;
				 else {
					str tmp = array[k];
					array[k] = array[k+i];
					array[k+i] = tmp;
				 }
			}
		}
	}
}

int main(int argc, char *argv[])
{
	str page[MAXSIZE];
	int size = read(page);
	fprintf(stderr, "%s %d records\n", argv[0], size);

	shell_sort(page, size);

	clock_t start, end;
	start = clock();

	shell_sort(page, size);

	end = clock();

	setlocale(LC_NUMERIC, "");
	fprintf(stderr, "execution time: %0.6f secs.\n", ELAPSE_TIME(end - start));

#ifdef WRITE
	write(page);
#endif

	return 0;
}
