#include <stdlib.h>
#include <console.h>

#define MAXLINE 80
#define MAXSIZE 1200

main (int argc, char *argv[]) {
  int i, j, min, count;
  char line[MAXLINE], *temp, *page[MAXSIZE];

  argc = ccommand (&argv);

  for (i = 0; fgets (line, MAXLINE, stdin); i++) {
    page[i] = calloc (strlen (line) + 1, sizeof (char));
    strcpy (page[i], line);
  }

  count = i;

  // selection sort
  for (i = 0; i < count; i++) {
    min = i;

    for (j = i; j < count; j++) {
      if ((strcmp (page[min], page[j]) > 0))
        min = j;
    }

    temp = page[i];
    page[i] = page[min];
    page[min] = temp;
  }

  //
  for (i = 0; i < count; i++)
    fputs (page[i], stdout);
}
