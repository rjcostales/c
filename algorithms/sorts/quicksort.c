#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "fileutils.h"

static int compare(const void *a, const void *b)
{
	/* The pointers point to offsets into "array",
		so we need to dereference them to get at the strings. */

	return strcmp(*(const char **) a, *(const char **) b);
}

int main(int argc, char *argv[])
{
	str page[MAXSIZE];
	int size = read(page);

	fprintf(stderr, "%s %d records\n", argv[0], size);

	clock_t start, end;
	start = clock();

	qsort(page, size, sizeof(const char *), compare);

	end = clock();

	setlocale(LC_NUMERIC, "");
	fprintf(stderr, "execution time: %0.6f secs.\n", ELAPSE_TIME(end - start));

#ifdef WRITE
	write(page);
#endif

	return 0;
}
