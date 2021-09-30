#include <stdio.h>

void
foo(int i)
{
	printf("%i\t%p\n", i, &i);
	foo(i + 1);
}

int
main(int argc, char *argv[])
{
	foo(1);
	return 0;
}
