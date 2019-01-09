#include <stdio.h>
#include <string.h>

void
pstring (char a[]) {
    printf ("%s\n", a);
}

void
parray (char *a[]) {
    printf ("%s\n", a[0]);
}

void
pdarray (char a[10][10]) {
    printf ("%s\n", a[0]);
}

int
main () {
    char string[] = "jesse costales";
    char *array[10] = { "this", "is", "a", "test" };
    char darray[10][10] = { "this", "is", "a", "test" };

    pstring (string);
    parray (array);
    pdarray (darray);
}
