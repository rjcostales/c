#include <stdio.h>
#include <stdlib.h>

int main()
{
	int i,j;
	int* ptr = NULL;

	printf("%p\n", ptr);
	
	ptr = &i;
	printf("%p\n", ptr);
	ptr = &j;
	printf("%p\n", ptr);

	printf("%ld %ld\n", sizeof(int), sizeof(*ptr));

	ptr = malloc(sizeof(int));
	printf("%p - %i\n", ptr, *ptr);

	*ptr = 67890;
	printf("%p - %i\n", ptr, *ptr);

	free(ptr);
	printf("%p - %i\n", ptr, *ptr);

	ptr = calloc(1, sizeof(int));
	printf("%p - %i\n", ptr, *ptr);

	*ptr = 12345;
	printf("%p - %i\n", ptr, *ptr);

	free(ptr);
	printf("%p - %i\n", ptr, *ptr);

	return 0;
}