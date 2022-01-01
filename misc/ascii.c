#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	for (int i = ' '; i < 127; i++)
		printf("%c", i);
	printf("\n");
}
