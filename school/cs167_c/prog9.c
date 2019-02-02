/*
 * Jesse Costales
 * Asst 4 - Prog 9
 *
 * This program genrates an array of random integers.  The array is then sorted
 * and the frequency is calculated.  There are no arguments and output is
 * stdout.
 */

#include <stdio.h>
#include "prog9.h"

/* function prototypes */
void    fill(int *, int);
void    show(int *, int, char *);

int main(int argc, char* argv[])
{
    int array[SIZE];
    fill(array, SIZE);
    show(array, SIZE, "Unsorted");
    sort(array, SIZE);
    show(array, SIZE, "Sorted");
    /* the extra point function */
    show(freq(array, SIZE), RANGE, "Frequency");
}

void fill(int array[], int size)
{
    while (size)                /* you didn't say we had to start at 0 */
        array[--size] = rand(LOWER, UPPER);
}

void show(int ar[], int size, char title[])
{
    int i;
    puts(title);
    for (i = 0; i < size; i++)
    {
        printf("%d = %d\n", i + 1, ar[i]);
    }
    putchar('\n');
}
