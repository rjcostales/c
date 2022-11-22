#include <stdio.h>
#include <string.h>

int
main(int argc, char *argv[])
{
	char  input[255];
	char* token;

	// scanf("%s", &input);
	strcpy(input, "this,is,a,test");

	token = strtok(input, ",");
	while (token != NULL) {
		printf("%s\n", token);
		token = strtok(NULL, ",");
	}
}
