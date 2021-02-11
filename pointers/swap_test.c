#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(str* a, str* b)
{
	str *tmp = a;
	a = b;
	b = *tmp;
}

int main(int argc, char *argv[])
{
	str array[5];

	str a = "Raphael";
	str b = "Jesus";
	str c = "Sanchez";
	str d = "Costales";
	str e = "";

	array[0] = calloc(strlen(a) + 1, sizeof(char));
	strcpy(strings[0], a);
	array[1] = calloc(strlen(b) + 1, sizeof(char));
	strcpy(strings[1], b);
	array[2] = calloc(strlen(c) + 1, sizeof(char));
	strcpy(strings[2], c);
	array[3] = calloc(strlen(d) + 1, sizeof(char));
	strcpy(strings[3], d);
	array[4] = calloc(strlen(e) + 1, sizeof(char));
	strcpy(strings[4], e);

	for(int i; i < 5; i++)
		printf("%s\n", array[i]);
}