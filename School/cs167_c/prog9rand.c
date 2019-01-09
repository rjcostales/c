/*
 * Jesse Costales
 * Asst 4 - Prog 9
 *
 * rand returns a pseudo-random number. lower is the min value desired and upper is
 * the maximum.
 */

#include <stdio.h>
#include "prog9.h"

int rand(int lower, int upper)
{
    const int a = 477;
    const int c = 3461;
    const int m = 32768;
    static int r = INITIAL_SEED;
    r = (a * r + c) % m;
    return((r / (float) m) * (upper - lower + 1) + lower);
}
