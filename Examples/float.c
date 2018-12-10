#include <stdio.h>
#include <math.h>

int main(void)
{

  float   f;
  double  d;

  f = atan(1.0) * 4.0;
  d = atan(1.0) * 4.0;

  printf("sizeof float\t%i\n", sizeof(f));
  printf("sizeof double\t%i\n", sizeof(d));

  printf("float\t%5.30e\n", f);
  printf("double\t%5.30e\n", d);

  return 0;
}
