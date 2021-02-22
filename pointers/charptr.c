#include <stdio.h>

int main(int argc, char *argv[])
{
	// strings and character pointers
	char first[] = "Raphael";
	char last[] = "Costales";

	char *cptr = first;
	printf("%s\n", cptr);

	cptr = &last[0];
	printf("%s\n", cptr);

	for (cptr = &first[0]; *cptr; cptr++)
		printf("%c|", *cptr);

}
