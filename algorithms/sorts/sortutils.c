#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sortutils.h"

typedef char *str;

int read(str strings[]) {
	int size;
	char line[MAXLINE];

	for (size = 0; fgets(line, MAXLINE, stdin); size++) {
		strings[size] = calloc(strlen(line) + 1, sizeof(char));
		strcpy(strings[size], line);
	}
	return size;
}

void write(str strings[]) {
	for (int i = 0; strings[i] != NULL; i++)
		fputs(strings[i], stdout);
}
