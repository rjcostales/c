/*
 * Jesse Costales
 * Asst 4 - Prog 9
 *
 * sort orders an array from low to high.  The size of the array is the
 * second argument.	The array, which is the first argument will be modified.
 */

#include <stdio.h>
#define COMP(a, b)    ((a) < (b) ? 1 : 0)

void sort(int array[], int size)
{
    int i, j;
    int min;
    int pos;
    int cnt;

    for (i = 0; i < size; i++)
    {
        min = array[i];
        pos = i;
        for (j = i + 1; j < size; j++)
        {
            if (COMP(array[j], min))
            {
                min = array[j];
                pos = j;
            }
        }
        array[pos] = array[i];
        array[i]   = min;
    }
}
