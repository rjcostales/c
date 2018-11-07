#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <limits.h>
#include <float.h>

int
main (int argc, char *argv[]) {
  int i;
  short s;

  for (char c = ' '; c < 127; c++)
    printf ("%c = %2X\n", c, c);
}
