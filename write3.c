#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main (int argc, char *argv[])
{
  char buffer[] =
    " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\n";

  char strings[201][5];
  char *string = &strings[0][0];
  int rand[200];
  int t;

  strings[200][0] = '\0';

  int urandom = open ("/dev/urandom", O_RDONLY);

  for (int n = 0; n < 1000000; n++)
    {
      read (urandom, rand, 800);
      for (int i = 0; i < 200; i++)
	{
	  int r = rand[i];
	  for (int j = 0; j < 5; j++)
	    {
	      strings[i][j] = buffer[r & 0x3f];
	      r >>= 6;
	    }
	}
      printf ("%s", string);
    }

  close (urandom);
}
