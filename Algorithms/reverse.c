#include <stdio.h>
#include <string.h>

char   *reverse(char *str)
{
    int i, j;
    char temp;

    for (i = 0, j = strlen(str) - 1; i < j; i++, j--) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }

    return str;
}

int main(int argc, char *argv[])
{

    char s1[] = "This is a test!";
    char s2[] = "Do it again!!!";

    printf("%s\n%s\n", s1, reverse(s1));
    printf("%s\n%s\n", s2, reverse(s2));

    return 0;
}
