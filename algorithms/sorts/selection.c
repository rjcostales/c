#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "fileutils.h"

int main(int argc, char *argv[])
{
	str tmp, page[MAXSIZE];
	int min, size = read(page);
	fprintf(stderr, "%s %d records\n", argv[0], size);

	// selection sort
	clock_t start, end;
	start = clock();

	for (int i = 0; i < size; i++) {
		min = i;
		for (int j = i; j < size; j++) {
			if (strcmp(page[min], page[j]) > 0)
				min = j;
		}
		tmp = page[i];
		page[i] = page[min];
		page[min] = tmp;
	}

	end = clock();

	setlocale(LC_NUMERIC, "");
	fprintf(stderr, "execution time: %0.6f secs.\n", ELAPSE_TIME(end - start));

#ifdef WRITE
	write(page);
#endif

	return 0;
}
