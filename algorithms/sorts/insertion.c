#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "sortutils.h"

int main(int argc, char *argv[])
{
	str tmp, page[MAXSIZE];
	int i, j, size = read(page);
	printf("%s %d records\n", argv[0], size);

	// insertion sort
	clock_t start, end;
	start = clock();
	for (i = 1; i < size; i++) {
		tmp = (str) page[i];
		j = i;
		while (j && (strcmp(page[j - 1], tmp) > 0)) {
			page[j] = page[j - 1];
			j--;
		}
		page[j] = tmp;
	}
	end = clock();
	setlocale(LC_NUMERIC, "");
	printf("execution time: %0.6f secs.\n",
		   (float) (end - start) / (float) CLOCKS_PER_SEC);

	// write(page);

	return 0;
}
