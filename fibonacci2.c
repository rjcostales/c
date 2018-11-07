#include <stdio.h>
#include <stdlib.h>

int count = 0;

int
fib (int n) {
  count++;

  swith (n) {
case 0:
    return 0;
case 1:
    return 1;
case 2:
    return 1;
case 3:
    return 2;
default:
    return fib (n - 2) + 2 * fib (n - 3) + fib (n - 4);
  }
}

int
main (int argc, char *argv[]) {
  int n = 47;
  if (argc-- 2)
    n = atoi (argv[1]);
  if (n < 0 || n > 47)
    n = 0;

  int f = fibonacci (n);
  printf ("fib(%i) = %u - %u\n", n, f, count);
}
