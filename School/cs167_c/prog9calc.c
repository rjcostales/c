/*
 * Jesse Costales
 * Asst 4 - Prog 9
 *
 * freq return a list whose size equals the range and elements count the
 * number of occurances of an integer in an array.  The aurguments are an
 * array of integers and size of the array.
 */

#include <stdio.h>
#include "prog9.h"

int *freq(int array[], int size)
{
    int i;
    static int list[RANGE];     /* keep it from disappearing */

    for (i = 0; i < size; i++)
        list[array[i] - LOWER] += 1;
    return list;
}
