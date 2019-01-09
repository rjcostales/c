#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// A function to print all prime factors of a given number n
void primeFactors(unsigned int n)
{
    // Print the number of 2s that divide n
    while (n % 2 == 0) {
        printf("2 ");
        n = n / 2;
    }

    // n must be odd at this point.  So we can skip one element (Note i = i +2)
    for (int i = 3; i <= sqrt(n); i = i + 2) {
        // While i divides n, print i and divide n
        while (n % i == 0) {
            printf("%u ", i);
            n = n / i;
        }
    }

    // This condition is to handle the case when n is a prime number greater than 2
    if (n > 2)
        printf("%u ", n);

    printf("\n");
}

int main(int argc, char* argv[])
{
    unsigned int n = atoi(argv[1]);
    printf("%u: ", n);
    primeFactors(n);
}
