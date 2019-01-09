#include <stdio.h>

#define MAXLINE 128
#define MAXSIZE 1200

main(int argc, char *argv[])
{
    int i, r, min, size;
    char line[MAXLINE], *remove, *page[MAXSIZE];

    for (size = 0; fgets(line, MAXLINE, stdin); size++) {
        page[size] = (char *) malloc(strlen(line));
        strcpy(page[size], line);
    }

    // insertion sort
    for (r = 1; r < size; r++) {

        remove = page[r];
        i = r;

        while (i && (strcmp(page[i - 1], remove) > 0)) {
            page[i] = page[i - 1];
            i--;
        }

        page[i] = remove;
    }

    for (i = 0; i < size; i++)
        fputs(page[i], stdout);
}
