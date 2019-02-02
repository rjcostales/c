#include <stdio.h>
FILE *A, *B;
char a[100];
char b[100];
char *aa, *bb;
char *aaa, *bbb;
int i, fail, right, num;
int loop = 1;
main (argc, argv)
     int argc;
     char *argv[];
{
  A = fopen (argv[1], "r");
  B = fopen (argv[2], "r");
  num = 0;
  right = 0;
  while (fgets (a, 100, A) != NULL)
    {
      if (strlen (a) < 8)
	{
	  printf ("bad file %s\n", argv[1]);
	  exit ();
	}
      fail = 0;
      num++;
      fgets (b, 100, B);
      if (strlen (b) < 8)
	{
	  printf ("bad file %s\n", argv[2]);
	  exit ();
	}
      aa = a;
      bb = b;
      for (; *aa != ':'; aa++);
      for (; *bb != ':'; bb++);
      aa += 2;
      bb += 2;
      aaa = aa;
      bbb = bb;
      loop=1;
      while (loop)
	{
	  fail = 0;
	  for (i = 0; i < 4; i++)
	    {
	      if (!fail && *aa != *bb) { fail = 1; }
	      aa++;
	      bb++;
	    }
	  if (fail && *bb == ',') { aa -= 4; loop = 1; bb++; }
	  else loop = 0;
	}
      /*printf("%s %s %c\n",aaa,bbb,fail?' ':'*');*/
      if (!fail)
	right++;
    }

  printf ("%s %d RIGHT %d\n", argv[1], right, num);
}
