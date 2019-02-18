#include <stdio.h>
#include <string.h>

typedef struct {
	char a;
	int b;
	char s[10];
} test;

int main(int argc, char *argv[])
{
	int i = -1;
	unsigned int u = 0;
	char c = 0;
	short s = 0;
	long l = 0;
	float f = 0;
	double d = 0;

	test t;

	t.a = i;
	t.b = i;

	printf("struct %d %d %d\n",
		(int) sizeof(t.a),
		(int) sizeof(t.b),
		(int) sizeof(t));

	printf("%d %d %d %d %d %d %d\n",
		(int) sizeof(i),
		(int) sizeof(u),
		(int) sizeof(c),
		(int) sizeof(s),
		(int) sizeof(l),
		(int) sizeof(f),
		(int) sizeof(d));

	c = i;
	u = i;
	s = i;
	l = i;

	l = f;
	f = d;

	printf("%d %d %d %d %d %d %d\n",
		(int) sizeof(i),
		(int) sizeof(u),
		(int) sizeof(c),
		(int) sizeof(s),
		(int) sizeof(l),
		(int) sizeof(f),
		(int) sizeof(d));

	return 0;
}
