#include <stdio.h>
#include <string.h>
#include <ctype.h>

int bytes, sum;

int main(int argc, char *argv[])
{
	char buffer[16];
	int length;
	while (fgets(buffer,16, stdin)) {
		length = strlen(buffer);
		for(int i = 0; i < length; i++)
			printf("%02x %02x %c | ", 
					i, 
					buffer[i] & 0xFF, 
					isprint(buffer[i]) ? buffer[i] : '_');
		printf("\n");
	}
}
