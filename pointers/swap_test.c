#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* str;

void swap(str* a, str* b)
{
	str tmp = *a;
	*a = *b;
	*b = tmp;
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
	strcpy(array[0], a);
	array[1] = calloc(strlen(b) + 1, sizeof(char));
	strcpy(array[1], b);
	array[2] = calloc(strlen(c) + 1, sizeof(char));
	strcpy(array[2], c);
	array[3] = calloc(strlen(d) + 1, sizeof(char));
	strcpy(array[3], d);
	array[4] = calloc(strlen(e) + 1, sizeof(char));
	strcpy(array[4], e);

	for(int i = 0; i < 5; i++)
		printf("%p %s\n", array[i], array[i]);

	swap(&array[0], &array[4]);

	for(int i = 0; i < 5; i++)
		printf("%p %s\n", array[i], array[i]);


}