#include <stdio.h>
#include "strfunc.h"

int main(int argc, char *argv[])
{
    char r[12] = "  right  ";

    printf("%s<\n", r);
    printf("%s<\n", rtrim(r));

    char s[12] = "  left  ";
    printf("%s<\n", s);
    printf("%s<\n", ltrim(s));

    char t[12] = "  middle  ";
    printf("%s<\n", t);
    printf("%s<\n", trim(t));

    return 0;
}
