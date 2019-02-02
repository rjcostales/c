#include <stdio.h>
#include "gnuchess.h"
#define HASHFILE "/usr/local/lib/gnuchess.hash"
#ifdef MSDOS
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define RWA_ACC "r+b"
#define WA_ACC "w+b"
#else
#define RWA_ACC "r+"
#define WA_ACC "w+"
#include <sys/param.h>
#include <sys/types.h>
#include <sys/times.h>
#endif /* MSDOS */
FILE *hashfile;
#define rxx "12345678"
#define cxx "abcdefgh"
#define pxx "1PNBRQK"
#define qxx "1pnbrqk"
#define truescore 0x0001
#define lowerbound 0x0002
#define upperbound 0x0004
#define kingcastle 0x0008
#define queencastle 0x0010

struct fileentry
{
  unsigned char bd[32];
  unsigned char f, t, flags, depth, sh, sl;
};
long i, j;
int nr[maxdepth];
struct fileentry n;
int r, c;
char line[128];
char *l;
short int t;
int c1, c2;
char mvstr[3][6];
short int board[64];
void
algbr (short int f, short int t, short int flag)
/*
 * Generate move strings in different formats.
 */

{
  int m3p;

  if (f != t)
    {
      /* algebraic notation */
      mvstr[0][0] = cxx[column (f)];
      mvstr[0][1] = rxx[row (f)];
      mvstr[0][2] = cxx[column (t)];
      mvstr[0][3] = rxx[row (t)];
      mvstr[0][4] = mvstr[3][0] = '\0';
      if (((mvstr[1][0] = pxx[board[f]]) == 'P') || (flag & promote))
	{
	  if (mvstr[0][0] == mvstr[0][2])
	    {			       /* pawn did not eat */
	      mvstr[2][0] = mvstr[1][0] = mvstr[0][2];	/* to column */
	      mvstr[2][1] = mvstr[1][1] = mvstr[0][3];	/* to row */
	      m3p = 2;
	    }
	  else
	    /* pawn ate */
	    {
	      mvstr[2][0] = mvstr[1][0] = mvstr[0][0];	/* from column */
	      mvstr[2][1] = mvstr[1][1] = mvstr[0][2];	/* to column */
	      mvstr[2][2] = mvstr[0][3];
	      m3p = 3;		       /* to row */
	    }
	  mvstr[2][m3p] = mvstr[1][2] = '\0';
	  if (flag & promote)
	    {
	      mvstr[0][4] = mvstr[1][2] = mvstr[2][m3p] = qxx[flag & pmask];
	      mvstr[1][3] = mvstr[2][m3p + 1] = mvstr[0][5] = '\0';
	    }
	}
      else
	/* not a pawn */
	{
	  mvstr[2][0] = mvstr[1][0];
	  mvstr[2][1] = mvstr[0][1];
	  mvstr[2][2] = mvstr[1][1] = mvstr[0][2];	/* to column */
	  mvstr[2][3] = mvstr[1][2] = mvstr[0][3];	/* to row */
	  mvstr[2][4] = mvstr[1][3] = '\0';
	  strcpy (mvstr[3], mvstr[2]);
	  mvstr[3][1] = mvstr[0][0];
	  if (flag & cstlmask)
	    {
	      if (t > f)
		{
		  strcpy (mvstr[1], "o-o");
		  strcpy (mvstr[2], "O-O");
		}
	      else
		{
		  strcpy (mvstr[1], "o-o-o");
		  strcpy (mvstr[2], "O-O-O");
		}
	    }
	}
    }
  else
    mvstr[0][0] = mvstr[1][0] = mvstr[2][0] = mvstr[3][0] = '\0';
}

int
main (int argc, char **argv)
{
  int f = 0;
  char flbuf[10];
  char *fl;
  if ((hashfile = fopen (HASHFILE, RWA_ACC)) == NULL)
    exit (1);
  for (i = 0; i < maxdepth; i++)
    nr[i] = 0;
  fseek (hashfile, 0L, SEEK_END);
  i = ftell (hashfile) / sizeof (struct fileentry);
  fseek (hashfile, 0L, SEEK_SET);
  printf ("/V 11 72 mul def /L 60 def\n");
  for (j = 0; j < i; j++)
    {
      fread (&n, sizeof (struct fileentry), 1, hashfile);
      if (n.depth)
	{
	  nr[0]++;
	  if (nr[0] == 19)
	    {
	      nr[0] = 1;
	      printf ("showpage\n/V 11 72 mul def\n");
	      printf ("/L 60 def\n");
	      f = 0;
	    }
	  /* now process this entry */
	  strcpy (line, "C ('#[");
	  for (r = 0; r < 8; r++)
	    {
	      l = line + 6 + (7 - r) * 9;
	      for (c = 0; c < 4; c++)
		{
		  c1 = (n.bd[r * 4 + c] >> 4) & 0xf;
		  c2 = n.bd[r * 4 + c] & 0xf;
		  board[r * 8 + c * 2] = (int) c1 & 0x7;
		  board[r * 8 + c * 2 + 1] = (int) c2 & 0x7;
		  if (c1 & 0x8)
		    *l++ = qxx[c1 & 0x7];
		  else
		    *l++ = pxx[c1 & 0x7];
		  if (c2 & 0x8)
		    *l++ = qxx[c2 & 0x7];
		  else
		    *l++ = pxx[c2 & 0x7];
		}
	      *l++ = ';';
	    }
	  l--;
	  line[79] = '\0';
	  strcat (line, "]') show");
	  algbr (n.f, n.t, 0);
	  t = (n.sh << 8) + n.sl;
	  /* decode flags */
	  fl = flbuf;
	  if (n.flags & kingcastle)
	    *fl++ = 'k';
	  if (n.flags & queencastle)
	    *fl++ = 'q';
	  if (n.flags & truescore)
	    *fl++ = 't';
	  if (n.flags & lowerbound)
	    *fl++ = 'l';
	  if (n.flags & upperbound)
	    *fl++ = 'u';
	  *fl = '\0';
	  printf ("L V moveto\n");
	  printf ("R (%s flags %s depth %d score %d", mvstr[0], flbuf, n.depth, t);
	  printf (") show\n");
	  printf ("L  V 100 sub moveto\n");
	  printf ("%s\n", line);
	  f++;
	  if (f == 3)
	    {
	      printf ("/V V 120 sub def /L 60 def\n");
	      f = 0;
	    }
	  else
	    printf ("/L 160 L add def\n");
	}
    }
  if (nr[0])
    printf ("showpage\n");
}
