#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "fileutils.h"

int main(int argc, char *argv[])
{
	str tmp, page[MAXSIZE];
	int i, j, size = read(page);
	fprintf(stderr, "%s %d records\n", argv[0], size);

	// insertion sort
	clock_t start, end;
	start = clock();
	for (i = 1; i < size; i++) {
		tmp = page[i];
		j = i;
		while (j && (strcmp(page[j - 1], tmp) > 0)) {
			page[j] = page[j - 1];
			j--;
		}
		page[j] = tmp;
	}
	end = clock();

	setlocale(LC_NUMERIC, "");
	fprintf(stderr, "execution time: %0.6f secs.\n", ELAPSE_TIME(end - start));

#ifdef WRITE
	write(page);
#endif

	return 0;
}
