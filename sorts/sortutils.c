#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sortutils.h"

typedef char *str;

int read(str strings[])
{
	int i;
	char line[MAXLINE];

	for (i = 0; fgets(line, MAXLINE, stdin); i++) {
		strings[i] = calloc(strlen(line) + 1, sizeof(char));
		strcpy(strings[i], line);
	}
	return i;
}

void write(str strings[])
{
	for (int i = 0; strings[i] != NULL; i++)
		fputs(strings[i], stdout);
}
