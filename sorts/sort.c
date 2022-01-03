/* C Program To Sort page in ascending order using Bubble Sort. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "fileutils.h"
#include "sortutils.h"

#define SORT SORT_NAME

int main(int argc, char *argv[])
{
	str page[MAXSIZE];
	int size = read(page);
	fprintf(stderr, "%s %d records\n", argv[0], size);

	clock_t start, end;
	start = clock();

	SORT(page, size);

	end = clock();

	setlocale(LC_NUMERIC, "");
	fprintf(stderr, "execution time: %0.6f secs.\n", ELAPSE_TIME(end - start));

#ifdef WRITE
	write(page);
#endif

	return 0;
}
