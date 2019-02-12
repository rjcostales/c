#include <stdio.h>
#include <string.h>

typedef struct {
    char a;
    int b;
    char s[10];
} test;

int main(int argc, char *argv[])
{
    int i = -1;
    unsigned int u = 0;
    char c = 0;
    short s = 0;
    long l = 0;
    float f = 0;
    double d = 0;

    test t;

    t.a = i;
    t.b = i;

    printf ("struct %lu %lu %lu\n", sizeof(t.a), sizeof(t.b), sizeof(t));

    printf ("%lu %lu %lu %lu %lu %lu\n",
            sizeof(i), sizeof(c), sizeof(s),
            sizeof(l), sizeof(f), sizeof(d));
    c = i;
    s = i;
    l = i;

    l = f;
    f = d;

    printf ("%lu %lu %lu %lu %lu %lu\n",
            sizeof(i), sizeof(c), sizeof(s),
            sizeof(l), sizeof(f), sizeof(d));

    return 0;
}
