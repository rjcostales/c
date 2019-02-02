/*
 * Jesse Costales
 * Asgn 6 - Prog 14
 * this program returns the bitwise majority of three ints note that only the
 * lower 16 bits are displayed
 */

#include <stdio.h>
void    bit_print(int);
int majority(int, int, int);

int main(int argc, char* argv[])
{
    int a, b, c;
    printf("Enter a integers, representing Alice's votes\n");
    scanf("%d", &a);
    printf("Enter a integers, representing Betty's votes\n");
    scanf("%d", &b);
    printf("Enter a integers, representing Carole's votes\n");
    scanf("%d", &c);
    printf("Alice	 >");
    bit_print(a);
    printf("Betty	 >");
    bit_print(b);
    printf("Carole	 >");
    bit_print(c);
    printf("		  -------- --------\n");
    printf("Majority >");
    bit_print(majority(a, b, c));
}

int majority(int a, int b, int c)
{
    return((a & b) | (b & c) | (a & c));
}

void bit_print(int b)
{
    int i, mask = 1 << 15;
    for (i = 1; i <= 16; ++i)
    {
        putchar(((b & mask) == 0) ? '0' : '1');
        b <<= 1;
        if (i % 8 == 0 && i != 16)
            putchar(' ');
    }
    putchar('\n');
}
