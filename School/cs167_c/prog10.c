/*
 * Jesse Costales
 * Asgn 5 - Program 10 Palindromes
 * 11/10/92
 *
 * This program uses redirection for input and ouput. The file /usr/dict/words
 * was used for the input and outputted to a file called palindromes.  Two
 * modification are included in my function ispalindrome.  1) single letters
 * are not considered palindromes, 2) case is ignored when checking letter.
 */

#include <stdio.h>
#include <string.h>
#define MAX 64
#define TRUE    1
#define FLASE   0

int main(int argc, char* argv[])
{
    char s[MAX];
    while ((gets(s)) != NULL)
    {
        if ((strlen(s) != 1) && (ispalindrome(s)))
            puts(s);
    }
    return TRUE;
}

int ispalindrome(char s[])      /* returns TRUE if s is a palindrome */
{
    int i = 0, j;
    j = strlen(s);
    while (i < j)
        if (tolower(s[i++]) != tolower(s[--j]))
            return FLASE;
    return TRUE;
}
