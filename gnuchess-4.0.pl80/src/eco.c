/*
 * eco.c - C source for GNU CHESS
 *
 * Copyright (c) 1985-1996 Stuart Cracraft, John Stanback,
 *                         Daryl Baker, Conor McCarthy,
 *                         Mike McGann, Chua Kong Sian
 * Copyright (c) 1985-1996 Free Software Foundation
 *
 * This file is part of GNU CHESS.
 *
 * GNU Chess is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2, or (at your option) any later
 * version.
 *
 * GNU Chess is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * GNU Chess; see the file COPYING.  If not, write to the Free Software
 * Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include "gnuchess.h"
#include "ttable.h" /* uses hashbd, hashkey */
#include "ataks.h"
#if !defined(AMIGADOS) && !defined(Think_C)
#include <unistd.h>
#endif
#ifdef Think_C
#include <unix.h>
#endif

#ifdef MSDOS
#include <io.h>
#endif
#if !defined MSDOS && !defined(Think_C)
#define O_BINARY 0
#endif
#include <fcntl.h>
unsigned long booksize = BOOKSIZE;
unsigned long int BKTBLSIZE;
unsigned long BOOKMASK;
unsigned long bookcount = 0;
unsigned bookpocket = BOOKPOCKET;
unsigned int curecoptr;

UTSHORT bookmaxply = BOOKMAXPLY;

CHAR *bookfile = NULL;
CHAR *binbookfile = BINBOOK;

int GotBook = false;
static CHAR bmvstr[5][6];
unsigned long bhashbd, bhashkey;


void
Balgbr (SHORT f, SHORT t, SHORT flag)


     /*
      * Generate move strings in different formats.
      */

{
    int m3p;
	bmvstr[0][0] = bmvstr[1][0] = bmvstr[2][0] = bmvstr[3][0] = bmvstr[4][0] = '\0';

    if (f != t)
      {
	  /* algebraic notation */
	  bmvstr[0][0] = cxx[column (f)];
	  bmvstr[0][1] = rxx[row (f)];
	  bmvstr[0][2] = cxx[column (t)];
	  bmvstr[0][3] = rxx[row (t)];
	  bmvstr[0][4] = bmvstr[3][0] = '\0';
	  if (((bmvstr[1][0] = pxx[board[f]]) == 'P') || (flag & promote))
	    {
		if (bmvstr[0][0] == bmvstr[0][2])	/* pawn did not eat */
		  {
		      bmvstr[2][0] = bmvstr[1][0] = bmvstr[0][2];	/* to column */
		      bmvstr[2][1] = bmvstr[1][1] = bmvstr[0][3];	/* to row */
		      m3p = 2;
		  }
		else
		    /* pawn ate */
		  {
		      bmvstr[2][0] = bmvstr[1][0] = bmvstr[0][0];	/* column */
		      bmvstr[2][1] = bmvstr[1][1] = bmvstr[0][2];	/* to column */
		      bmvstr[2][2] = bmvstr[0][3];
		      m3p = 3;	/* to row */
		  }
		if (flag & promote)
		  {
		      bmvstr[0][4] = bmvstr[1][2] = bmvstr[2][m3p] = qxx[flag & pmask];
		      bmvstr[0][5] = bmvstr[1][3] = bmvstr[2][m3p + 1] = bmvstr[3][0] = '\0';
		  }
		else
		    bmvstr[2][m3p] = bmvstr[1][2] = '\0';
                if(flag & epmask){
			bmvstr[3][0] = bmvstr[0][2];
			bmvstr[3][1] = bmvstr[0][3];
			bmvstr[3][2] = '\0';
		}
	    }
	  else
	      /* not a pawn */
	    {
		bmvstr[2][0] = bmvstr[1][0];
		bmvstr[2][1] = bmvstr[0][1];
		bmvstr[2][2] = bmvstr[1][1] = bmvstr[0][2];	/* to column */
		bmvstr[2][3] = bmvstr[1][2] = bmvstr[0][3];	/* to row */
		bmvstr[2][4] = bmvstr[1][3] = '\0';
		strcpy (bmvstr[3], bmvstr[2]);
		bmvstr[3][1] = bmvstr[0][0];
		bmvstr[4][0] = bmvstr[1][0]; strcpy(&bmvstr[4][1],bmvstr[0]);
		if (flag & cstlmask)
		  {
		      if (t > f)
			{
			    strcpy (bmvstr[1], bmvstr[0]);
			    strcpy (bmvstr[0], CP[5]);
			    strcpy (bmvstr[2], CP[7]);
			}
		      else
			{
			    strcpy (bmvstr[1], bmvstr[0]);
			    strcpy (bmvstr[0], CP[6]);
			    strcpy (bmvstr[2], CP[8]);
			}
		  }
	    }
      }
    else
	bmvstr[0][0] = bmvstr[1][0] = bmvstr[2][0] = bmvstr[3][0] = bmvstr[4][0] = '\0';
}


#ifndef QUIETBOOKGEN
void
bkdisplay (s, cnt, moveno)
     CHAR *s;
     int cnt;
     int moveno;
{
    static SHORT pnt;
#ifndef SEMIQUIETBOOKGEN
    struct leaf *node;
    int r, c, l;
#endif

    pnt = TrPnt[2];
    printf ("matches = %d\n", cnt);
    printf ("inout move is :%s:move number %d side %s\n", s, moveno / 2 + 1, !(moveno & 1) ? "white" : "black");
#ifndef SEMIQUIETBOOKGEN
    printf ("legal moves are \n");
    while (pnt < TrPnt[3])
      {
	  node = &Tree[pnt++];
	  Balgbr (node->f, node->t, (SHORT) node->flags);
	  printf ("%s %s %s %s %s\n", bmvstr[0], bmvstr[1], bmvstr[2], bmvstr[3],bmvstr[4]);
      }
    printf ("\n current board is\n");
    for (r = 7; r >= 0; r--)
      {
	  for (c = 0; c <= 7; c++)
	    {
		l = locn (r, c);
		if (color[l] == neutral)
		    printf (" -");
		else if (color[l] == white)
		    printf (" %c", qxx[board[l]]);
		else
		    printf (" %c", pxx[board[l]]);
	    }
	  printf ("\n");
      }
    printf ("\n\n");
#endif
}

#endif

int
BVerifyMove (CHAR *s, UTSHORT *mv, int moveno)

     /*
      * Compare the string 's' to the list of legal moves available for the
      * opponent. If a match is found, make the move on the board.
      */

{
    static SHORT pnt, tempb, tempc, tempsf, tempst, cnt;
    static struct leaf xnode;
    struct leaf *node;

    *mv = 0;
    cnt = 0;
    VMoveList (opponent, 2);
    pnt = TrPnt[2];
    while (pnt < TrPnt[3])
      {
	  node = &Tree[pnt++];
	  Balgbr (node->f, node->t, (SHORT) node->flags);
	  if (strcmp (s, bmvstr[0]) == 0 || strcmp (s, bmvstr[1]) == 0 ||
	      strcmp (s, bmvstr[2]) == 0 || strcmp (s, bmvstr[3]) == 0 || strcmp (s, bmvstr[4]) == 0)
	    {
		cnt++;
		xnode = *node;
	    }
      }
    if (cnt == 1)
      {
	  MakeMove (opponent, &xnode, &tempb, &tempc, &tempsf, &tempst);
	  if (SqAtakd (PieceList[opponent][0], computer))
	    {
		UnmakeMove (opponent, &xnode, &tempb, &tempc, &tempsf, &tempst);
		/* Illegal move in check */
#ifndef QUIETBOOKGEN
		printf (CP[77]);
		printf ("\n");
		bkdisplay (s, cnt, moveno);
#endif
		return (false);
	    }
	  else
	    {
		*mv = (xnode.f << 8) | xnode.t;
		Balgbr (xnode.f, xnode.t, false);
		if (board[xnode.t] == pawn){
		        if (xnode.t - xnode.f == 16)
          			epsquare = xnode.f + 8;
        		else if (xnode.f - xnode.t == 16)
          			epsquare = xnode.f - 8;}
			else epsquare = -1;

		return (true);
	    }
      }
    /* Illegal move */
#ifndef QUIETBOOKGEN
    printf (CP[75], s);
    bkdisplay (s, cnt, moveno);
#endif
    return (false);
}

void
RESET (void)

     /*
      * Reset the board and other variables to start a new game.
      */

{
    SHORT l;

    flag.illegal = flag.mate = flag.post = flag.quit = flag.reverse = flag.bothsides = flag.onemove = flag.force = false;
    flag.material = flag.coords = flag.hash = flag.easy = flag.rcptr = true;
    flag.stars = flag.shade = flag.back = flag.musttimeout = false;
#ifdef CHESSTOOL
	flag.beep=false;
#else
	flag.beep=true;
#endif
#ifdef CLIENT
    flag.gamein = true;
#else
    flag.gamein = false;
#endif
    GenCnt = epsquare = 0;
    GameCnt = 0;
    Developed[white] = Developed[black] = false;
    castld[white] = castld[black] = false;
    PawnThreat[0] = CptrFlag[0] = false;
    opponent = white;
    computer = black;
    for (l = 0; l < 64; l++)
      {
	  board[l] = Stboard[l];
	  color[l] = Stcolor[l];
	  Mvboard[l] = 0;
      }
    InitializeStats ();
}


int 
gnc (FILE * fd)
{
  int c;
  c = getc (fd);
  if (c == '(')
    {
      do
	{
	  c = getc (fd);
	  if (c == ')')
	    {
	      c = getc (fd);
	      break;
	    }
	  if (c == EOF)
	    break;
	}
      while (true);
    }
  else if (c == '{')
    {
      do
	{
	  c = getc (fd);
	  if (c == '}')
	    {
	      c = getc (fd);
	      break;
	    }
	  if (c == EOF)
	    break;
	}
      while (true);
    }
  return c;
}

int
Vparse (FILE * fd, UTSHORT *mv, SHORT side, CHAR *opening, int moveno)
{
  register int c, i;
  CHAR s[128];
  CHAR *p;

  while (true)
    {

      while ((c = gnc (fd)) == ' ' || c == '\n');
      if (c == '\r') continue;
      i = 0;
      if (c == '#' || c == '[' || c == '%')
	{			/* comment */
          curecoptr = ftell(fd);
	  p = opening;
	  do
	    {
	      *p++ = c;
	      c = gnc (fd);
	      if (c == '\r') continue;
	      /* goes to end of line */
	      if (c == '\n')
		{
		  /* does the comment continue */
		  if ((c = getc (fd)) == '[') continue;
		  ungetc (c, fd);
		  *p = '\0';
		  return 0;
	      } if (c == EOF) return -1;
	    }
	  while (true);
	}
      /* is it a move number or analysis ( in [ ] or in { } ) */
      /* number cannot start with a 0 because of 0-0 */
      else if (!isalpha (c) && c != '0')
	{
	  int nl = false;
	  int nonspace = false;

	  while (true)
	    {
	      c = gnc (fd);
	      if (nl)
		if (c == '#' || c == '[' || c == '%')
		  {
		    ungetc (c, fd);
		    return 0;
		  }
	      if (c == '\r')
		continue;
	      if (c == '\n')
		{
		  nl = true;
		  continue;
		}
	      else
		nl = false;
	      if (c == EOF)
		{
		  return -1;
		}
	      /* stop at first nonspace a ... is space */
	      /* must be nonspace because of 0-0 */
	      if (nonspace)
		{
		  if (c != '.' && c != ' ')
		    break;
		}
	      if (c == '.')
		{
		  nonspace = true;
		}
	      /* stop if alpha must be move */
	      else if (isalpha (c))
		break;
	    }
	}
      s[0] = (CHAR) c;

      while ((c = gnc (fd)) != '\n' && c != ' ' && c != '\t' && c != EOF)
	{
	  if (isupper (c))
	    {
	      if (c != 'O')
		{
		  ungetc (c, fd);
		  c = ' ';
		  break;
		}
	    }
	  if (c == '\r') continue;
	  if (c == '?') break;
	  if (c == '!') continue;
	  if (c == '+') continue;
	  if (c == '#') continue;
	  if (c == '%') continue;
	  if (c == '='){c = gnc(fd); c=tolower(c);}
	  if (c != 'x') s[++i] = c;
	}
      s[++i] = '\0';

      if (c == EOF) return (-1);
      if (s[0] == '!' || s[0] == ';')
	{
	  while (c != '\n' && c != EOF) c = gnc (fd);
	  if (c == EOF) return -1;
	  else return (0);
	}
      if ((strcmp (s, "o-o-o") == 0) || (strcmp (s, "OOO") == 0) || (strcmp (s, "O-O-O") == 0) || (strcmp (s, "0-0-0") == 0))
	{
	  if (side == black) strcpy (s, "e8c8");
	  else strcpy (s, "e1c1");
	}
      else if ((strcmp ("o-o", s) == 0) || (strcmp (s, "OO") == 0) || (strcmp (s, "O-O") == 0) || (strcmp (s, "0-0") == 0))
	{
	  if (side == black) strcpy (s, "e8g8");
	  else strcpy (s, "e1g1");
	}
      else if (strcmp (s, "draw") == 0) continue;
      else if (strcmp (s, "Draw") == 0) continue;
      else if (strcmp (s, "1-0") == 0) continue;
      else if (strcmp (s, "0-1") == 0) continue;
      else if (strcmp (s, "2-1/2") == 0) continue;
      if (isupper (s[i - 1]))
	s[i - 1] = tolower (s[i - 1]);


      i = BVerifyMove (s, mv, moveno);
      bhashkey = hashkey;
      bhashbd = hashbd;
      if (c == '?')
	{			/* Bad move, not for the program to play */
	  *mv |= BADMOVE;	/* Flag it ! */
	  c = getc (fd);
	}
      else if (c == '+' || c == '\r')
	c = gnc (fd);
      if (!i)
	{
	  printf ("%s \n", opening);
	  /* flush to start of next */
	  while ((c = gnc (fd)) != '[' && c != EOF && c != '#');
	  if (c == EOF)
	    return -1;
	  else
	    {
	      ungetc (c, fd);
	      return i;
	    }
	}
      return (i);
    }
}


/*===================================== GDX =======================================*/

struct gdxadmin
{
    unsigned long bookcount;
    unsigned long booksize;
    unsigned long maxoffset;
} ADMIN, B;

struct gdxdata
{
    unsigned long hashbd;
    unsigned long hashkey;
    unsigned int ecoptr;
    utshort cntr;
} DATA;

#ifdef LONG64
#define lts(x) (unsigned long)(((x)&(~1))|side)
#else
#define lts(x) (unsigned long)(((x)&(~1))|side)
#endif
unsigned long currentoffset;
int gfd;

void
GetOpenings (void)

     /*
      * If a text file of opening chess plays (the Opening Book) is available: 
      * Read in the Opening Book file and parse the algebraic notation for a move
      * into an unsigned integer format indicating the from and to square. Create
      * or update a binary hash file with the recomended move/moves for each 
      * position.  
      * The binary hash file is opened with readonly access durring the game.
      */
{
    register SHORT i;
    CHAR opening[256];
    CHAR msg[256];
    UTSHORT xside, doit, side;
    SHORT c;
    UTSHORT mv;
    unsigned long games = 0;
    FILE *fd;

    printf("SIZES %d %d\n",sizeof(struct gdxadmin),sizeof(struct gdxdata));
    printf("%s\n",bookfile);
    if ((fd = fopen (bookfile, "r")) == NULL)
	fd = fopen (PGNECO, "r");
    if (fd != NULL)
      {
	  /* yes add to book */
	  /* open book as writer */
	  gfd = open (binbookfile, O_RDONLY | O_BINARY);
	  if (gfd >= 0)
	    {
		if (sizeof(struct gdxadmin) == read (gfd, &ADMIN, sizeof (struct gdxadmin)))
		  {
		      B.bookcount = ADMIN.bookcount;
		      B.booksize = ADMIN.booksize;
		      B.maxoffset = ADMIN.maxoffset;
		      if (B.booksize && !(B.maxoffset == ((unsigned long)(B.booksize - 1) * sizeof (struct gdxdata) + sizeof (struct gdxadmin))))
			{
			    printf ("bad format %s\n", binbookfile);
			    exit (1);
			}
		  }
		else
		  {
		      printf ("bad format %s\n", binbookfile);
		      exit (1);
		  }
		close (gfd);
                gfd = open (binbookfile, O_RDWR | O_BINARY);

	    }
	  else
	    {
#ifdef Think_C
                gfd = open (binbookfile, O_RDWR | O_CREAT | O_BINARY);
#else
                gfd = open (binbookfile, O_RDWR | O_CREAT | O_BINARY, 0644);
#endif
		ADMIN.bookcount = B.bookcount = 0;
		ADMIN.booksize = B.booksize = booksize;
                B.maxoffset = ADMIN.maxoffset = (unsigned long) (booksize - 1) * sizeof (struct gdxdata) + sizeof (struct gdxadmin);
		DATA.hashbd = 0;
		DATA.hashkey = 0;
		DATA.cntr = 0;
		DATA.ecoptr = 0;

	    }
	  if (gfd >= 0)
	    {


		/* setvbuf(fd,buffr,_IOFBF,2048); */
		side = white;
		xside = black;
		InitializeStats();
		i = 0;

		while ((c = Vparse (fd, &mv, side, opening, i)) >= 0)
		  {
		      if (c == 1)
                        {

			    /*
                             * if not first move of an opening and first
                             * time we have seen it save next move as
                             * hint
                             */
			    i++;
					doit = true;

					      DATA.hashbd = bhashbd;
					      DATA.hashkey = (unsigned long)(lts(bhashkey));
					      DATA.ecoptr = curecoptr;
		write (gfd, &DATA, sizeof (struct gdxdata));
#ifdef notdef
fprintf(xfd,"%8x %8x %d %x %s\n",DATA.hashbd,DATA.hashkey,curecoptr,mv,opening);
#endif

			    computer = opponent;
			    opponent = computer ^ 1;

			    xside = side;
			    side = side ^ 1;
			}
		      else if (i > 0)
			{
			    /* reset for next opening */
			    games++;
			    RESET ();
			    i = 0;
			    side = white;
			    xside = black;

			}
		  }
		fclose (fd);
		close (gfd);
		exit(0);
	    }
      }
    if (binbookfile != NULL)
      {
	  /* open book as reader */
	  gfd = open (binbookfile, O_RDONLY | O_BINARY);
	  if (gfd >= 0)
	    {
		read (gfd, &ADMIN, sizeof (struct gdxadmin));
		B.bookcount = ADMIN.bookcount;
		B.booksize = ADMIN.booksize;
		B.maxoffset = ADMIN.maxoffset;
                if (B.booksize && !(B.maxoffset == ((unsigned long) (B.booksize - 1) * sizeof (struct gdxdata) + sizeof (struct gdxadmin))))
		  {
		      printf ("bad format %s\n", binbookfile);
		      exit (1);
		  }

	    }
	  else
	    {
		B.bookcount = 0;
		B.booksize = booksize;

	    }

#if !defined CHESSTOOL && !defined XBOARD
	  sprintf (msg, CP[213], B.bookcount, B.booksize);
	  ShowMessage (msg);
#endif
      }
    /* set every thing back to start game */
    Book = BOOKFAIL;
    RESET ();
    /* now get ready to play */
    if (!B.bookcount)
      {
#if !defined CHESSTOOL && !defined XBOARD
	  ShowMessage (CP[212]);
#endif
	  Book = 0;
      }
}



int
OpeningBook (SHORT *hint, SHORT side)

     /*
      * Go thru each of the opening lines of play and check for a match with the
      * current game listing. If a match occurs, generate a random number. If this
      * number is the largest generated so far then the next move in this line
      * becomes the current "candidate". After all lines are checked, the
      * candidate move is put at the top of the Tree[] array and will be played by
      * the program. Note that the program does not handle book transpositions.
      */
{
}
void LOpeningBook (SHORT side) { }
void EOpeningBook (SHORT side) { }
