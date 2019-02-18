#include <stdio.h>

typedef struct complex {
	short real;
	short imaginary;
} complex_t, *complex_p;

void foo(complex_t *x)
{
	x->real = 200;
}

int main(int argc, char *argv[])
{
	struct complex a;
	complex_t b;
	complex_p p;

	b.real = 100;
	a.real = b.real;

	p = &a;

	printf ("%d\n", p->real);
	foo (&a);
	printf ("%d\n", p->real);
}
