/* BPSK BINARY SYMMETRIC CHANNEL SIMULATOR                          */
/* Copyright (c) 1999, Spectrum Applications, Derwood, MD, USA      */
/* All rights reserved                                              */
/* Version 2.0 Last Modified 1999.02.17                             */

//#include <alloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#include " vdsim.h "

float   gngauss(float mean, float sigma);

float gngauss(float mean, float sigma)
{

  float   u, r;                 /* uniform and Rayleigh random variables */

  /* generate a uniformly distributed random number u between 0 and 1 - 1E-6 */
  u = (float) rand() / RAND_MAX;
  if (u == 1.0)
    u = 0.999999999;

  /* generate a Rayleigh-distributed random number r using u */
  r = sigma * sqrt(2.0 * log(1.0 / (1.0 - u)));

  /* generate another uniformly-distributed random number u as before */
  u = (float) rand() / RAND_MAX;
  if (u == 1.0)
    u = 0.999999999;

  /* generate and return a Gaussian-distributed random number using r and u */
  return ((float) (mean + r * cos(2 * 3.141529654 * u)));
}

int main()
{
  int     i;

  srand(time(0));

  for (i = 0; i < 1000; i++)
    printf("%2.4f\r", gngauss(0.0, 1.0));
}
