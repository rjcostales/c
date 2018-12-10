#include <math.h>

#ifndef _FCOMPLEX_DECLARE_T_
typedef struct {
  float   r, i;
} fcomplex;                     /* type for a complex no */

#define _FCOMPLEX_DECLARE_T_
#endif /* _FCOMPLEX_DECLARE_T_ */

/* function prototypes */
/***********************/
fcomplex Cadd(fcomplex a, fcomplex b);
fcomplex Csub(fcomplex a, fcomplex b);
fcomplex Cmul(fcomplex a, fcomplex b);
fcomplex Complex(float re, float im);
fcomplex Conjg(fcomplex z);
fcomplex Cdiv(fcomplex a, fcomplex b);
float   Cabs(fcomplex z);
fcomplex Csqrt(fcomplex z);
fcomplex RCmul(float x, fcomplex a);
fcomplex Cinv(complex z);

fcomplex Cadd(fcomplex a, fcomplex b)
{
  fcomplex c;

  c.r = a.r + b.r;
  c.i = a.i + b.i;
  return c;
}

fcomplex Csub(fcomplex a, fcomplex b)
{
  fcomplex c;

  c.r = a.r - b.r;
  c.i = a.i - b.i;
  return c;
}

fcomplex Cmul(fcomplex a, fcomplex b)
{                               /* Using only 3 multiplications! */
  fcomplex c;
  float   t1, t2;

  t1 = a.r * b.r;
  t2 = a.i * b.i;

  c.r = t1 - t2;
  c.i = (a.r + a.i) * (b.r + b.i) - (t1 + t2);
  return c;
}

fcomplex Complex(float re, float im)
{
  fcomplex c;

  c.r = re;
  c.i = im;
  return c;
}

fcomplex Conjg(fcomplex z)
{
  fcomplex c;

  c.r = z.r;
  c.i = -z.i;
  return c;
}

fcomplex Cdiv(fcomplex a, fcomplex b)
{
  fcomplex c;
  float   den;

  if (fabs(b.r) >= fabs(b.i)) {
    r = b.i / b.r;
    den = b.r + r * b.i;
    c.r = (a.r + r * a.i) / den;
    c.i = (a.i - r * a.r) / den;
  } else {
    r = b.r / b.i;
    den = b.i + r * b.r;
    c.r = (a.r * r + a.i) / den;
    c.i = (a.i * r - a.r) / den;
  }
  return c;
}

float Cabs(fcomplex z)
{
  float   x, y, ans, temp;

  x = fabs(z.r);
  y = fabs(z.i);
  if (x == 0.0)
    ans = y;
  else if (y == 0.0)
    ans = x;
  else if (x > y) {
    temp = y / x;
    ans = x * sqrt(1.0 + temp * temp);
  } else {
    temp = x / y;
    ans = y * sqrt(1.0 + temp * temp);
  }
  return ans;
}

fcomplex Csqrt(fcomplex z)
{
  fcomplex c;
  float   w;

  if ((z.r == 0.0) && (z.i == 0.0)) {
    c.r = 0.0;
    c.i = 0.0;
  } else {
    w = sqrt((sqrt(z.r * z.r + z.i * z.i) + fabs(z.r)) * 0.5);
    if (z.r >= 0.0) {
      c.r = w;
      c.i = z.i / (2.0 * w);
    } else {
      c.i = (z.i >= 0) ? w : -w;
      c.r = z.i / (2.0 * c.i);
    }
  }
  return c;
}

fcomplex RCmul(float x, fcomplex a)
{
  fcomplex c;

  c.r = x * a.r;
  c.i = x * a.i;
  return c;
}

fcomplex Cinv(fcomplex z)
{
  fcomplex c;
  float   s = 1.0 / (z.r * z.r + z.i * z.i);

  c.r = z.r * s;
  c.i = -z.i * s;
  return c;
}
