#include <stdio.h>
#include <float.h>

int main () {
   printf("The maximum value of float = %.25e\n", FLT_MAX);
   printf("The minimum value of float = %.25e\n", FLT_MIN);

   printf("The number of digits in the number = %d\n", FLT_MANT_DIG);
}
