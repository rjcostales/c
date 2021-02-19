#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define MAXLINE 1024
#define MAXSIZE 65536

#define ELAPSE_TIME(X) ((float) (X) / (float) CLOCKS_PER_SEC)

typedef char *str;

int read(str strings[])
{
	int i;
	char line[MAXLINE];

	clock_t start, end;
	start = clock();

	for (i = 0; fgets(line, MAXLINE, stdin); i++) {
#ifdef ZERO
		strings[i] = calloc(strlen(line) + 1, sizeof(char));
#else
		strings[i] = malloc(strlen(line) + 1);
#endif
		strcpy(strings[i], line);
	}
	
	end = clock();

	setlocale(LC_NUMERIC, "");
	printf("execution time: %0.6f secs.\n", ELAPSE_TIME(end - start));

	return i;
}

int main(int argc, char *argv[])
{
	str page[MAXSIZE];
	int size = read(page);
	printf("%d\n", size);
	return 0;
}
