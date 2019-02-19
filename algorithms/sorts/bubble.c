/* C Program To Sort page in ascending order using bubble sort. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "sortutils.h"

int main(int argc, char *argv[])
{
	str tmp, page[MAXSIZE];
	int size = read(page);
	printf("%s %d records\n", argv[0], size);
	clock_t start, end;
	start = clock();
	for (int i = 0; i < size - 1; ++i) {
		for (int j = 0; j < size - i - 1; ++j) {
			if (strcmp(page[j], page[j + 1]) > 0) {
				tmp = page[j];
				page[j] = page[j + 1];
				page[j + 1] = tmp;
			}
		}
	}
	end = clock();
	setlocale(LC_NUMERIC, "");
	printf("execution time: %0.6f secs.\n",
		   (float) (end - start) / (float) CLOCKS_PER_SEC);

	// write(page);

	return 0;
}
