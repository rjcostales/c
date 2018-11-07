/* Generate a normal random variable with mean 0 and standard deviation
   of 1.  To adjust to some other distribution, multiply by the standard
   deviation and add the mean.  Box-Muller method
note: rand() is a function that returns a uniformly distributed random
number from 0 to RAND_MAX
*/

#include <stdlib.h>
#include <math.h>

double
gaussrand () {
  static double V2, fac;
  static int phase = 0;
  double S, Z, U1, U2, V1;

  if (phase)
    Z = V2 * fac;
  else {
    do {
      U1 = (double) rand () / RAND_MAX;
      U2 = (double) rand () / RAND_MAX;

      V1 = 2 * U1 - 1;
      V2 = 2 * U2 - 1;
      S = V1 * V1 + V2 * V2;
    } while (S >= 1);

    fac = sqrt (-2 * log (S) / S);
    Z = V1 * fac;
  }

  phase = 1 - phase;

  return Z;
}
