#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define LOOP 10000
#define SIZE 100

int main(int argc, char *argv[])
{
    int integers[SIZE];
    int result;
    clock_t start, stop;

    setlocale(LC_NUMERIC, "");

    printf("%s\t%'d - %'dx%'d\n", argv[0], LOOP, SIZE, SIZE);

    for (int i = 0; i < SIZE; i++)
        integers[i] = rand();
    // for (int i = 0; i < SIZE; i++)
    //     printf("%g\n", integers[i]);

    start = clock();
    for (int t = 0; t < LOOP; t++)
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                result = integers[j];
    stop = clock();
    printf("nop\t%0.6f\n",
      (float) (stop - start) / (float) CLOCKS_PER_SEC);

    start = clock();
    for (int t = 0; t < LOOP; t++)
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                result = integers[i] + integers[j];
    stop = clock();
    printf("add\t%0.6f\n",
      (float) (stop - start) / (float) CLOCKS_PER_SEC);

    start = clock();
    for (int t = 0; t < LOOP; t++)
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                result = integers[i] - integers[j];
    stop = clock();
    printf("sub\t%0.6f\n",
      (float) (stop - start) / (float) CLOCKS_PER_SEC);

    start = clock();
    for (int t = 0; t < LOOP; t++)
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                result = integers[i] * integers[j];
    stop = clock();
    printf("mul\t%0.6f\n",
      (float) (stop - start) / (float) CLOCKS_PER_SEC);

    start = clock();
    for (int t = 0; t < LOOP; t++)
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                result = integers[i] / integers[j];
    stop = clock();
    printf("div\t%0.6f\n",
      (float) (stop - start) / (float) CLOCKS_PER_SEC);

    start = clock();
    for (int t = 0; t < LOOP; t++)
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                result = integers[i] % integers[j];
    stop = clock();
    printf("mod\t%0.6f\n",
      (float) (stop - start) / (float) CLOCKS_PER_SEC);

    start = clock();
    for (int t = 0; t < LOOP; t++)
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                result = integers[i] & integers[j];
    stop = clock();
    printf("and\t%0.6f\n",
      (float) (stop - start) / (float) CLOCKS_PER_SEC);

    start = clock();
    for (int t = 0; t < LOOP; t++)
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                result = integers[i] | integers[j];
    stop = clock();
    printf("or\t%0.6f\n",
      (float) (stop - start) / (float) CLOCKS_PER_SEC);

    start = clock();
    for (int t = 0; t < LOOP; t++)
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                result = integers[i] ^ integers[j];
    stop = clock();
    printf("xor\t%0.6f\n",
      (float) (stop - start) / (float) CLOCKS_PER_SEC);
} /* main */
