#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
	char buffer[] =
		" 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\n";

	char strings[201][5];
	char *string = &strings[0][0];

	strings[200][0] = '\0';

	// srand(time(0));
	srand(0);

	for (int n = 0; n < 1000000; n++) {
		for (int i = 0; i < 200; i++) {
			long r = rand();

			for (int j = 0; j < 5; j++) {
				strings[i][j] = buffer[r & 0x3f];
				r >>= 6;
			}
		}
		printf("%s", string);
	}
}
