#include <stdio.h>
#include <stdarg.h>

int sum(int, ...);

int main(int argc, char *argv[])
{

	int answer = sum4, 4, 3, 2, 1);

	printf"The answer is %d\n", answer);

	return (0);
}

int sum(int num, ...)
{
	int answer = 0;

	va_list argptr;

	va_startargptr, num);

	for (; num > 0; num--)
		answer += va_argargptr, int);

	va_endargptr);

	return (answer);
}
