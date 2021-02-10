#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "fileutils.h"

int main(int argc, char *argv[])
{
	str tmp, page[MAXSIZE];
	int i, j, k, size = read(page);
	fprintf(stderr, "%s %d records\n", argv[0], size);

	// shell sort
	clock_t start, end;
	start = clock();

	for (i = size / 2; i > 0; i /= 2) {
		for (j = i; j < size; j++) {
			for (k = j - i; k >= 0; k = k - i) {
				if (strcmp(page[k+i], page[k]) > 0)
					break;
				 else {
					tmp = page[k];
					page[k] = page[k+i];
					page[k+i] = tmp;
				 }
			}
		}
	}

	end = clock();

	setlocale(LC_NUMERIC, "");
	fprintf(stderr, "execution time: %0.6f secs.\n", ELAPSE_TIME(end - start));

#ifdef WRITE
	write(page);
#endif

	return 0;
}
