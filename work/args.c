#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	printf("%i, %s\n", argc, argv[0]);

	for (int i = 1; i < argc; i++)
		printf("%s\n", argv[i]);

	return 0;
}
