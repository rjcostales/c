#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	float array3d[2][3][5];
	float *ip;

	ip = &array3d[0][0][0];
	for (int n = 0; n < 30; n++)
		*ip++ = (n * 1.0) + 10.0;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 5; k++) {
				printf("%d %d %d %3.1f\t%p\n", i, j, k,
				array3d[i][j][k], &array3d[i][j][k]);
			}
		}
	}

	printf("%ld\n", sizeof(array3d));

	// int a[];
	int *a = calloc(10, sizeof(int));
	for ( int i = 0; i < 10; i++)
		printf("%d\n", a[i]);

}
