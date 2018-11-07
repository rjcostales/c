#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

char   *strtoupper(char *str);
char   *strtoupper(char *str)
{
  register char *s = str;

  while (*s) {
    *s = toupper(*s);
    *s++;
  }
  return str;
}

main()
{
  char    str[] = "This is a test\n";

  printf(strtoupper(str));
  printf(str);
}
