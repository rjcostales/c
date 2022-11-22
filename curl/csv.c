#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main()
{
	char str[] = "1.2, 3.4, 5.6, 7.8, 9.0";

	// Returns first token
	char* token = strtok(str, ",");

	// Keep printing tokens while one of the
	// delimiters present in str[].
	while (token != NULL) {
		printf("%0.1f\n", atof(token));
		token = strtok(NULL, ",");
	}

	return 0;
}
