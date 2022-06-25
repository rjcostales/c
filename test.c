#include <stdio.h>

void foo(int i) {
	printf("%i\t%p\n", i, (void *) &i);
	foo(i + 1);
}

int main() {
	foo(1);
	return 0;
}
