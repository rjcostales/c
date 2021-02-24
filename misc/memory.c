#include <stdlib.h>
#include <stdio.h>

void check(int depth) {
	char c[4];
	char *ptr = malloc(4);
	printf("stack at %p, heap at %p\n", &c[0], ptr);
	if (depth <= 0) return;
	check(depth-1);
}

int main() {
	check(10);
	return 0;
}
