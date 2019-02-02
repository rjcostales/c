#include <stdio.h>
char s[100];
FILE *I;
char *p, *q, *r;
int first = 1;
int sco = 0;
int cnt = 0;
int ovs =0;
int ovc = 0;
char last[24];
main (argc, argv)
     int argc;
     char *argv[];
{
  I = fopen (argv[1], "r");
	  printf ("  test         right   wrong  total    percent\n");
  while (fgets (s, 100, I) != NULL)
    {
/* p -> name */
      p = s+8;
      for (; *p != '.'; p++);
/* q -> right */
      for (q = p; *q != ' ';q++);
      q++;
/* r -> cnt */
      for (r = q; *r != ' '; r++);
	r++;
      for (; *r != ' '; r++);
      r++;
      *p = '\0';
      if (first)
	{
	  strcpy (last, s+8);
	  first = 0;
	}
      if (strcmp (s+8, last) == 0)
	{
	  sco += atoi (q);
	  cnt += atoi (r);
	}
      else
	{
	  printf ("%9s\t%d\t%d\t%d\t%d\n", last, sco,cnt - sco, cnt, (int)(((float)sco/(float)cnt)*100.+.5));
	  strcpy (last, s+8);
	  ovs += sco; ovc += cnt;
	  cnt = atoi (r);
	  sco = atoi (q);
	}
    }
	  printf ("%9s\t%d\t%d\t%d\t%d\n", last, sco,cnt - sco, cnt, (int)(((float)sco/(float)cnt)*100.+.5));
	  ovs += sco; ovc += cnt;
	  printf ("\ntotal    \t%d\t%d\t%d\t%d\n", ovs, ovc - ovs,ovc,(int)(((float)ovs/(float)ovc)*100.+.5));
}
