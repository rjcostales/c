/*
 * Jesse Costales
 * Asgn 6 - Prog 12
 */

#include <stdio.h>
#include <string.h>
int strend(char *, char *);

int main(int argc, char* argv[])
{
    char s[256], t[256];
    printf("Enter string and substring\n");
    scanf("%s%s", s, t);
    strend(s, t)
    ? printf("%s is at the end of %s\n", t, s)
    : printf("%s is NOT at the end of %s\n", t, s);
}

int strend(char *s, char *t)
{
    return(strcmp(s + strlen(s) - strlen(t), t) == 0);
}
