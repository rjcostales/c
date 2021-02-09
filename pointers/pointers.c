#include <stdio.h>

int main(int argc, char *argv[])
{
	int a = 10;
	int b = 20;

	int *p;
	int *q;

	p = &a;
	q = &b;

	printf("Value of a is: %d\n", a);
	printf("Value of b is: %d\n", b);

	printf("Address of variable a is: %p\n", &a);
	printf("Address of variable b is: %p\n", &b);

	printf("Value of *p is: %d\n", *p);
	printf("Value of *q is: %d\n", *q);

	/* Note I have used %p for p's value as it represents an address */
	printf("Value of pointer p is: %p\n", p);
	printf("Value of pointer q is: %p\n", q);

	*p = 30;
	*q = 40;

	printf("Value of *p is: %d\n", *p);
	printf("Value of *q is: %d\n", *q);

	printf("Value of a is: %d\n", a);
	printf("Value of b is: %d\n", b);

}
