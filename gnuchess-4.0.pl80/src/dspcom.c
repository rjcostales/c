/*
 * dspcom.c - C source for GNU CHESS
 *
 * Copyright (c) 1985-1996 Stuart Cracraft, John Stanback,
 *                         Daryl Baker, Conor McCarthy,
 *                         Mike McGann, Chua Kong Sian
 * Copyright (c) 1985-1996 Free Software Foundation
 *
 * This file is part of GNU CHESS.
 *
 * GNU Chess is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * GNU Chess is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Chess; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef WIN32
#define SHORT WIN_SHORT
#include <windows.h>
#undef SHORT
#endif
#include "gnuchess.h"
#include "ataks.h"
#include "ttable.h" /* calls ZeroTTable(), probably belongs in main.c*/
#ifdef HAVE_GETTIMEOFDAY
#include <sys/time.h>
#endif
CHAR mvstr[5][7];
#if defined(Think_C) && defined(Window_Events)
#include "doevents.h"
#endif
extern SHORT Mwpawn[64], Mbpawn[64], Mknight[2][64], Mbishop[2][64];
extern CHAR *version, *patchlevel;
extern void LOpeningBook();
#ifdef ECO
extern void EOpeningBook();
SHORT ecomove;
#endif
CHAR *InPtr;
#if !defined(AMIGADOS) && !defined(Think_C) && !defined(WIN32)
#include <unistd.h>
#endif

#if defined(sun) && defined(sparc) && defined(svr4)
#include <sys/filio.h>
#endif
#include <ctype.h>
#include <signal.h>
#if defined(MSDOS)
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#else
#ifdef Think_C
#include "doevents.h"
#else
#ifdef WIN32
#include <sys/types.h>
#else /* Other */
#include <sys/param.h>
#include <sys/types.h>
#include <sys/file.h>
#ifndef AMIGADOS
#include <sys/ioctl.h>
#endif
#endif
#endif /* Think_C */
#endif /* MSDOS */

void
algbr (SHORT f, SHORT t, SHORT flag)


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
      mvstr[4][0] = mvstr[0][4] = mvstr[3][0] = '\0';
      if (((mvstr[1][0] = pxx[board[f]]) == CP[2][1]) || (flag & promote))
	{
	  if (mvstr[0][0] == mvstr[0][2])	/* pawn did not eat */
	    {
	      mvstr[2][0] = mvstr[1][0] = mvstr[0][2];	/* to column */
	      mvstr[2][1] = mvstr[1][1] = mvstr[0][3];	/* to row */
	      m3p = 2;
	    }
	  else
	    /* pawn ate */
	    {
	      mvstr[2][0] = mvstr[1][0] = mvstr[0][0];	/* column */
	      mvstr[2][1] = mvstr[1][1] = mvstr[0][2];	/* to column */
	      mvstr[2][2] = mvstr[0][3];
	      m3p = 3;		/* to row */
	    }
	  if (flag & promote)
	    {
	      mvstr[0][4] = mvstr[1][2] = mvstr[2][m3p] = qxx[flag & pmask];
	      mvstr[0][5] = mvstr[1][3] = mvstr[2][m3p + 1] = mvstr[3][0] = '\0';
#ifdef CHESSTOOL 
	      mvstr[3][0] = mvstr[0][0];	/* Allow e7e8 for chesstool */
	      mvstr[3][1] = mvstr[0][1];
	      mvstr[3][2] = mvstr[0][2];
	      mvstr[3][3] = mvstr[0][3];
	      mvstr[3][4] = '\0';
#endif
	    } else mvstr[2][m3p] = mvstr[1][2] = '\0';
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
	  mvstr[4][0] = mvstr[1][0]; strcpy(&mvstr[4][1],mvstr[0]);
	  if (flag & cstlmask)
	    {
	      if (t > f)
		{
		  strcpy (mvstr[1], mvstr[0]);
		  strcpy (mvstr[0], CP[5]);
		  strcpy (mvstr[2], CP[7]);
		}
	      else
		{
		  strcpy (mvstr[1], mvstr[0]);
		  strcpy (mvstr[0], CP[6]);
		  strcpy (mvstr[2], CP[8]);
		}
	    }
	}
    }
  else
    mvstr[0][0] = mvstr[1][0] = mvstr[2][0] = mvstr[3][0] = mvstr[4][0] = '\0';
}


int
VerifyMove (CHAR *s, SHORT iop, UTSHORT *mv)

/*
 * Compare the string 's' to the list of legal moves available for the
 * opponent. If a match is found, make the move on the board.
 */

{
  static SHORT pnt, tempb, tempc, tempsf, tempst, cnt;
  static struct leaf xnode;
  struct leaf *node;
  *mv = 0;

  if (iop == 2)
    {
      UnmakeMove (opponent, &xnode, &tempb, &tempc, &tempsf, &tempst);
      return (false);
    }
  cnt = 0;
  VMoveList (opponent, 2);
  pnt = TrPnt[2];
  while (pnt < TrPnt[3])
    {
      node = &Tree[pnt++];
      algbr (node->f, node->t, (SHORT) node->flags);
      if (strcmp (s, mvstr[0]) == 0 || strcmp (s, mvstr[1]) == 0 ||
	  strcmp (s, mvstr[2]) == 0 || strcmp (s, mvstr[3]) == 0
		|| strcmp (s, mvstr[4]) == 0)
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
#if defined CHESSTOOL
	  printz (CP[78]);
#else
#ifdef NONDSP
/* Illegal move in check */
	  printz (CP[77]);
	  printz ("\n");
#ifdef DEBUG8
  if (1)
    {   
      FILE *D;
      int r, c, l;
      extern UTSHORT PrVar[];
      D = fopen ("DEBUG", "a+");
      pnt = TrPnt[2];
      fprintf (D, "resp = %ld\n", ResponseTime);
      fprintf (D, "iop = %d\n", iop);
      fprintf (D, "matches = %d\n", cnt);
      algbr (hint >> 8, hint & 0xff, (SHORT) 0);
      fprintf (D, "hint %s\n", mvstr[0]);
      fprintf (D, "inout move is %s\n", s);
      for (r = 1; PrVar[r]; r++)
        {
          algbr (PrVar[r] >> 8, PrVar[r] & 0xff, (SHORT) 0);
          fprintf (D, " %s", mvstr[0]);
        }
      fprintf (D, "\n");
      fprintf (D, "legal move are \n");
      while (pnt < TrPnt[3])
        {
          node = &Tree[pnt++];
          algbr (node->f, node->t, (SHORT) node->flags);
          fprintf (D, "%s %s %s %s %s\n", mvstr[0], mvstr[1], mvstr[2], mvstr[3],mvstr[4]);
        } 
      fprintf (D, "\n current board is\n");
      for (r = 7; r >= 0; r--)
        {
          for (c = 0; c <= 7; c++)
            {
              l = locn (r, c);
              if (color[l] == neutral)
                fprintf (D, " -");
              else if (color[l] == white)
                fprintf (D, " %c", qxx[board[l]]);
              else
                fprintf (D, " %c", pxx[board[l]]);
            }
          fprintf (D, "\n");
        }
      fprintf (D, "\n");
      fclose (D);
    }  
#endif 

#else
/* Illegal move in check */
	  ShowMessage (CP[77]);
#endif
#endif /* CHESSTOOL */
	  return (false);
	}
      else
	{
	  if (iop == 1)
	    return (true);
	  UpdateDisplay (xnode.f, xnode.t, 0, (SHORT) xnode.flags);
	  if ((board[xnode.t] == pawn)
	      || (xnode.flags & capture)
	      || (xnode.flags & cstlmask))
	    {
	      Game50 = GameCnt;
	      ZeroRPT ();
	    }
	  GameList[GameCnt].depth = GameList[GameCnt].score = 0;
	  GameList[GameCnt].nodes = 0;
	  ElapsedTime (1);
	  GameList[GameCnt].time = (SHORT) (et+50)/100;
	  if (TCflag)
	    {
	      TimeControl.clock[opponent] -= (et-TCadd);
	      timeopp[oppptr] = et;
	      --TimeControl.moves[opponent];
	    }
	  *mv = (xnode.f << 8) | xnode.t;
	  algbr (xnode.f, xnode.t, false);
	  return (true);
	}
    }
#if defined CHESSTOOL
  printz (CP[78]);
#else
#ifdef NONDSP
/* Illegal move */
  printz (CP[75], s);
#ifdef DEBUG8
  if (1)
    {
      FILE *D;
      int r, c, l;
      extern UTSHORT PrVar[];
      D = fopen ("DEBUG", "a+");
      pnt = TrPnt[2];
      fprintf (D, "resp = %ld\n", ResponseTime);
      fprintf (D, "iop = %d\n", iop);
      fprintf (D, "matches = %d\n", cnt);
      algbr (hint >> 8, hint & 0xff, (SHORT) 0);
      fprintf (D, "hint %s\n", mvstr[0]);
      fprintf (D, "inout move is %s\n", s);
      for (r = 1; PrVar[r]; r++)
	{
	  algbr (PrVar[r] >> 8, PrVar[r] & 0xff, (SHORT) 0);
	  fprintf (D, " %s", mvstr[0]);
	}
      fprintf (D, "\n");
      fprintf (D, "legal move are \n");
      while (pnt < TrPnt[3])
	{
	  node = &Tree[pnt++];
	  algbr (node->f, node->t, (SHORT) node->flags);
	  fprintf (D, "%s %s %s %s %s\n", mvstr[0], mvstr[1], mvstr[2], mvstr[3],mvstr[4]);
	}
      fprintf (D, "\n current board is\n");
      for (r = 7; r >= 0; r--)
	{
	  for (c = 0; c <= 7; c++)
	    {
	      l = locn (r, c);
	      if (color[l] == neutral)
		fprintf (D, " -");
	      else if (color[l] == white)
		fprintf (D, " %c", qxx[board[l]]);
	      else
		fprintf (D, " %c", pxx[board[l]]);
	    }
	  fprintf (D, "\n");
	}
      fprintf (D, "\n");
      fclose (D);
    }
#endif
#else
/* Illegal move */
  ShowMessage (CP[76]);
#endif
#endif /* CHESSTOOL */
#if !defined CHESSTOOL && !defined XBOARD
  if (cnt > 1)
    ShowMessage (CP[32]);
#endif /* CHESSTOOL */
  return (false);
}

int
parser (CHAR *f, int side)
{
  int c1, r1, c2, r2;

  if (f[4] == 'o')
    if (side == black)
      return 0x3C3A;
    else
      return 0x0402;
  else if (f[0] == 'o')
    if (side == black)
      return 0x3C3E;
    else
      return 0x0406;
  else
    {
      c1 = f[0] - 'a';
      r1 = f[1] - '1';
      c2 = f[2] - 'a';
      r2 = f[3] - '1';
      return (locn (r1, c1) << 8) | locn (r2, c2);
    }
  /*NOTREACHED*/
}

void
GetGame (void)
{
  FILE *fd;
  CHAR fname[256], *p;
  int c, i, j;
  int eps = -1;
  SHORT sq;
/* enter file name */
  ShowMessage (CP[63]);
  scanz ("%s", fname);
/* chess.000 */
  if (fname[0] == '\0')
    strcpy (fname, CP[137]);
  if ((fd = fopen (fname, "r")) != NULL)
    {
      NewGame ();
      fgets (fname, 256, fd);
      computer = opponent = white;
      InPtr = fname;
      skip ();
      if (*InPtr == 'c')
	computer = black;
      else
	opponent = black;
      skip ();
      skip ();
      skip ();
      Game50 = atoi (InPtr);
      skip();
      skip();
      eps = atoi(InPtr);
      fgets (fname, 256, fd);
      InPtr = &fname[14];
      castld[white] = ((*InPtr == CP[214][0]) ? true : false);
      skip ();
      skip ();
      castld[black] = ((*InPtr == CP[214][0]) ? true : false);
      fgets (fname, 256, fd);
      InPtr = &fname[11];
      skipb ();
      TCflag = atoi (InPtr);
      skip ();
      InPtr += 14;
      skipb ();
      OperatorTime = atoi (InPtr);
      fgets (fname, 256, fd);
      InPtr = &fname[11];
      skipb ();
      TimeControl.clock[white] = atol (InPtr);
      skip ();
      skip ();
      TimeControl.moves[white] = atoi (InPtr);
      fgets (fname, 256, fd);
      InPtr = &fname[11];
      skipb ();
      TimeControl.clock[black] = atol (InPtr);
      skip ();
      skip ();
      TimeControl.moves[black] = atoi (InPtr);
      fgets (fname, 256, fd);
      for (i = 7; i > -1; i--)
	{
	  fgets (fname, 256, fd);
	  p = &fname[2];
	  InPtr = &fname[11];
	  skipb ();
	  for (j = 0; j < 8; j++)
	    {
	      sq = i * 8 + j;
	      if (*p == '.')
		{
		  board[sq] = no_piece;
		  color[sq] = neutral;
		}
	      else
		{
		  for (c = 0; c < 8; c++)
		    {
		      if (*p == pxx[c])
			{
			  board[sq] = c;
			  color[sq] = black;
			}
		    }
		  for (c = 0; c < 8; c++)
		    {
		      if (*p == qxx[c])
			{
			  board[sq] = c;
			  color[sq] = white;
			}
		    }
		}
	      p++;
	      Mvboard[sq] = atoi (InPtr);
	      skip ();
	    }
	}
      GameCnt = 0;
      flag.regularstart = true;
      Book = BOOKFAIL;
      fgets (fname, 256, fd);
      fgets (fname, 256, fd);
      fgets (fname, 256, fd);
      while (fgets (fname, 256, fd))
	{
	  struct GameRec *g;
	  int side = black;

	  ++GameCnt;
	  InPtr = fname;
	  skipb ();
	  g = &GameList[GameCnt];
	  side = side ^ 1;
	  g->gmove = parser (InPtr, side);
	  skip ();
	  g->score = atoi (InPtr);
	  skip ();
	  g->depth = atoi (InPtr);
	  skip ();
	  g->nodes = atol (InPtr);
	  skip ();
	  g->time = atol (InPtr);
	  skip ();
	  g->flags = c = atoi (InPtr);
	  skip ();
#ifdef HAVE_STRTOUL
	  g->hashkey = strtoul (InPtr, (CHAR **) NULL, 16);
	  skip ();
	  g->hashbd = strtoul (InPtr, (CHAR **) NULL, 16);
#else
	  g->hashkey = strtol (InPtr, (CHAR **) NULL, 16);
	  skip ();
	  g->hashbd = strtol (InPtr, (CHAR **) NULL, 16);
#endif
	  skip ();
	  g->epssq = atoi(InPtr);
	  g->piece = no_piece;
	  g->color = neutral;
	  if (c & (capture | cstlmask))
	    {
	      if (c & capture)
		{
		  skip ();
		  for (c = 0; c < 8; c++)
		    if (pxx[c] == *InPtr)
		      break;
		  g->piece = c;
		}
	      skip ();
	      g->color = ((*InPtr == CP[119][0]) ? black : white);
	    }
	}
/*      if (TimeControl.clock[white] > 0)
	TCflag = true; */
      if (TCflag == false)
        MaxResponseTime = TimeControl.clock[white];
      fclose (fd);
    }
  ZeroRPT ();
  InitializeStats ();
  epsquare = eps;
  UpdateDisplay (0, 0, 1, 0);
  Sdepth = 0;
  hint = 0;
}

void
GetXGame (void)
{
  FILE *fd;
  CHAR fname[256], *p;
  int  i, j;
  SHORT emptycount;
/* Enter file name */
  ShowMessage (CP[63]);
  scanz ("%s", fname);
  if (fname[0] == '\0')
/* xboard.position.read*/
    strcpy (fname, CP[205]);
  if ((fd = fopen (fname, "r")) != NULL)
    {
      NewGame ();
      flag.regularstart = false;
      Book = false;
      fgets (fname, 256, fd);
    p = fname;

    /* Piece placement data */
    for (i = 8 - 1; i >= 0; i--) {
        j = 0;
        while (j < 8) {
            if (isdigit(*p)) {
                emptycount = *p++ - '0';
                if (j + emptycount > 8) return ;
                while (emptycount--) {board[(i)*8 + j] = no_piece;color[(i)*8 + j++] = neutral;}
            } else {
		int c;
		int sq = (i)*8 + j++;
		for (c = 0; c < 8; c++)
                    {
                      if (*p == qxx[c])
                        {
                          board[sq] = c;
                          color[sq] = black;
                        }
                    }    
                  for (c = 0; c < 8; c++)
                    {
                      if (*p == pxx[c])
                        {
                          board[sq] = c;
                          color[sq] = white;
                        }
                    }    
		    p++;
            }   
        }
        if (*p != '/' && *p != ' ') return ;
        p++;
    }   

    /* Active color */
    switch (*p) {
      case 'w':
        computer = white;
        break;
      case 'b':
        computer = black;
        break;
      default:
        return ;
    }
       
    /* !!We ignore the rest of the FEN notation */


          xwndw = BXWNDW;
	}
      fclose (fd);
  ZeroRPT ();
  InitializeStats ();
  UpdateDisplay (0, 0, 1, 0);
  Sdepth = 0;
  hint = 0;
}

void
SaveGame (void)
{
  FILE *fd;
  CHAR fname[256];
  SHORT sq, i, c, f, t;
  CHAR p;

  if (savefile[0])
    strcpy (fname, savefile);
  else
    {
/* Enter file name*/
      ShowMessage (CP[63]);
      scanz ("%s", fname);
    }

  if (fname[0] == '\0')
/* chess.000 */
    strcpy (fname, CP[137]);
  if ((fd = fopen (fname, "w")) != NULL)
    {
      CHAR *b, *w;

      b = w = CP[74];
      if (computer == black)
	b = CP[141];
      if (computer == white)
	w = CP[141];
      fprintf (fd, CP[37], b, w, Game50,epsquare);
      fprintf (fd, CP[42], castld[white] ? CP[214] : CP[215], castld[black] ? CP[214] : CP[215]);
      fprintf (fd, CP[111], TCflag, OperatorTime);
      fprintf (fd, CP[117],
	       TimeControl.clock[white], TimeControl.moves[white],
	       TimeControl.clock[black], TimeControl.moves[black]);
      for (i = 7; i > -1; i--)
	{
	  fprintf (fd, "%1d ", i + 1);
	  for (c = 0; c < 8; c++)
	    {
	      sq = i * 8 + c;
	      switch (color[sq])
		{
		case black:
		  p = pxx[board[sq]];
		  break;
		case white:
		  p = qxx[board[sq]];
		  break;
		default:
		  p = '.';
		}
	      fprintf (fd, "%c", p);
	    }
	  for (f = i * 8; f < i * 8 + 8; f++)
	    fprintf (fd, " %d", Mvboard[f]);
	  fprintf (fd, "\n");
	}
      fprintf (fd, "  %s\n", cxx);
      fprintf (fd, CP[126]);
      for (i = 1; i <= GameCnt; i++)
	{
	  struct GameRec *g = &GameList[i];

	  f = g->gmove >> 8;
	  t = (g->gmove & 0xFF);
	  algbr (f, t, g->flags);
	  fprintf (fd, "%s %5d %5d %7ld %6ld %5d  %#08lx %#08lx %d %c   %s\n",
		   mvstr[0], g->score, g->depth,
		   g->nodes, g->time, g->flags, g->hashkey, g->hashbd,g->epssq,
	   pxx[g->piece], (((int)(g->color) == 2) ? (CHAR *)"     " : ColorStr[g->color]));
	}
      fclose (fd);
/* Game saved */
      ShowMessage (CP[70]);
    }
  else
    /*ShowMessage ("Could not open file");*/
    ShowMessage (CP[48]);
}

void
ListGame (void)
{
  FILE *fd;
  SHORT i, f, t;
#ifndef MSDOS
  time_t when;
  CHAR fname[256], dbuf[256];
#else
  CHAR fname[256];
#endif

  if (listfile[0])
    strcpy (fname, listfile);
  else
    {
#ifdef MSDOS
      sprintf (fname, "chess.lst");
#else
      time (&when);
      strncpy (dbuf, ctime (&when), 20);
      dbuf[7] = '\0';
      dbuf[10] = '\0';
      dbuf[13] = '\0';
      dbuf[16] = '\0';
      dbuf[19] = '\0';
/* use format "CLp16.Jan01-020304B" when patchlevel is 16,
   date is Jan 1
   time is 02:03:04
   program played black */
      sprintf (fname, "CLp%s.%s%s-%s%s%s%c", patchlevel, dbuf + 4, dbuf + 8, dbuf + 11, dbuf + 14, dbuf + 17, ColorStr[computer][0]);
      /* replace space padding with 0 */
      for (i = 0; fname[i] != '\0'; i++)
	if (fname[i] == ' ')
	  fname[i] = '0';
#endif /* MSDOS */
    }
  fd = fopen (fname, "w");
  if (!fd)
    {
      printf (CP[219], fname);
      exit (1);
    }
  /*fprintf (fd, "gnuchess game %d\n", u);*/
  fprintf (fd, CP[161], patchlevel);
  fprintf (fd, CP[10]);
  fprintf (fd, CP[11]);
  for (i = 1; i <= GameCnt; i++)
    {
      f = GameList[i].gmove >> 8;
      t = (GameList[i].gmove & 0xFF);
      algbr (f, t, GameList[i].flags);
      if(GameList[i].flags & book)
          fprintf (fd, "%6s  %5d    Book%7ld %5ld", mvstr[0],
	       GameList[i].score, 
	       GameList[i].nodes, GameList[i].time);
      else
          fprintf (fd, "%6s  %5d     %2d %7ld %5ld", mvstr[0],
	       GameList[i].score, GameList[i].depth,
	       GameList[i].nodes, GameList[i].time);
      if ((i % 2) == 0)
	{
#ifdef DEBUG40
	if(computer == black){
	  int p;
	for(p=0;GameList[i-1].d1[p] && p < 7;p++){
		algbr((GameList[i-1].d1[p]>>8) & 0xff,GameList[i-1].d1[p] & 0xff,0);
	        fprintf(fd," %s",mvstr[0]);
	}
	fprintf(fd,"\n");
	} else {
	int p;
	for(p=0;GameList[i].d1[p] && p < 7;p++){
		algbr(GameList[i].d1[p]>>8 & 0xff,GameList[i].d1[p] & 0xff,0);
	        fprintf(fd," %s",mvstr[0]);
	}
	fprintf(fd,"\n");
	}
#else
	fprintf(fd,"\n");
#endif
    }
   }
  fprintf (fd, "\n\n");
  if (GameList[GameCnt].flags & draw)
    {
      fprintf (fd, CP[54], DRAW);
    }
  else if (GameList[GameCnt].score == -9999)
    {
      fprintf (fd, "%s\n", ColorStr[player ]);
    }
  else if (GameList[GameCnt].score == 9998)
    {
      fprintf (fd, "%s\n", ColorStr[player ^ 1]);
    }
  fclose (fd);
}

void
Undo (void)

/*
 * Undo the most recent half-move.
 */

{
  SHORT f, t;
  f = GameList[GameCnt].gmove >> 8;
  t = GameList[GameCnt].gmove & 0xFF;
  if (board[t] == king && distance (t, f) > 1)
    (void) castle (GameList[GameCnt].color, f, t, 2);
  else
    {
      /* Check for promotion: */
      if (GameList[GameCnt].flags & promote)
	{
	  board[t] = pawn;
	}
      board[f] = board[t];
      color[f] = color[t];
      board[t] = GameList[GameCnt].piece;
      color[t] = GameList[GameCnt].color;
      if (color[t] != neutral)
	Mvboard[t]--;
      Mvboard[f]--;
    }
  if (GameList[GameCnt].flags & epmask)
    EnPassant (otherside[color[f]], f, t, 2);
  else
    InitializeStats ();
  epsquare = GameList[GameCnt].epssq;
  if (TCflag && (TCmoves>1))
    ++TimeControl.moves[color[f]];
  hashkey = GameList[GameCnt].hashkey;
  hashbd = GameList[GameCnt].hashbd;
  GameCnt--;
  computer = computer ^ 1;
  opponent = opponent ^ 1;
  flag.mate = false;
  Sdepth = 0;
  player = player ^ 1;
  ShowSidetoMove ();
  UpdateDisplay (0, 0, 1, 0);
/*
  if (flag.regularstart)
*/
    Book = BOOKFAIL;
}

SHORT
 TestSpeed (void (*f) (SHORT side, SHORT ply), unsigned j)
{
#ifdef test
  unsigned jj;
#endif
  unsigned i;
  long cnt, rate, t1, t2;
#ifdef HAVE_GETTIMEOFDAY
struct timeval tv;
#endif

#ifdef HAVE_GETTIMEOFDAY
  gettimeofday(&tv,NULL);
  t1 = (tv.tv_sec*100+(tv.tv_usec/10000));
#else
  t1 = time (0);
#endif
  for (i = 0; i < j; i++)
    {
      f (opponent, 2);
#ifdef test
	for(jj=TrPnt[2];i<TrPnt[3];jj++)if(!pick(jj,TrPnt[3]-1))break;
#endif
    }
#ifdef HAVE_GETTIMEOFDAY
  gettimeofday(&tv,NULL);
  t2 = (tv.tv_sec*100+(tv.tv_usec/10000));
#else
  t2 = time (0);
#endif
  cnt = j * (TrPnt[3] - TrPnt[2]);
  if (t2 - t1)
    et = (t2 - t1);
  else
    et = 1;
  rate = (((et) ? ((cnt*100) / et) : 0));
  /*printz ("Nodes= %ld Nodes/sec= %ld\n", cnt, rate);*/
#ifdef NONDSP
  printz (CP[91], cnt, rate);
#ifdef DEBUG9
  for (j = TrPnt[2]; j < TrPnt[3]; j++)
    {
      struct leaf *node = &Tree[j];
      algbr (node->f, node->t, node->flags);
      printf ("%s %s %s %s %d %x\n", mvstr[0], mvstr[1], mvstr[2], mvstr[3],node->score,node->flags);
    }
#endif
#else
  ShowNodeCnt (cnt);
#endif
	return(TrPnt[3] - TrPnt[2]);
}

void
 TestPSpeed (SHORT (*f) (SHORT side), unsigned j)
{
  SHORT i;
  long cnt, rate, t1, t2;
#ifdef HAVE_GETTIMEOFDAY
struct timeval tv;
#endif

#ifdef HAVE_GETTIMEOFDAY
  gettimeofday(&tv,NULL);
  t1 = (tv.tv_sec*100+(tv.tv_usec/10000));
#else
  t1 = time (0);
#endif
  for (i = 0; i < j; i++)
    {
      (void) f (opponent);
    }
#ifdef HAVE_GETTIMEOFDAY
  gettimeofday(&tv,NULL);
  t2 = (tv.tv_sec*100+(tv.tv_usec/10000));
#else
  t2 = time (0);
#endif
  cnt = j;
  if (t2 - t1)
    et = (t2 - t1);
  else
    et = 1;
  rate = (et) ? ((cnt*100) / et) : 0;
  /*printz ("Nodes= %ld Nodes/sec= %ld\n", cnt, rate);*/
#ifdef NONDSP
  printz (CP[91], cnt, rate);
#else
  ShowNodeCnt (cnt);
#endif
}


void
SetOppTime (CHAR *s)
{
  CHAR *time;
  register int error, oldet;
  int m, t, sec;
  sec = 0;
  time = &s[strlen (CP[197])];
  t = (int)strtol (time, &time, 10);
   if (*time == ':') {
       /* If the time does not contain a ":", it is in hundredths of
        a second; if it does, it is in minutes:seconds. */
       time++;
       sec = (int)strtol(time, &time, 10);
       t = t*6000 + sec*100;
   }
   /* This is a little tricky because opponent's clock is running.
      TimeControl.clock[opponent] records the time the clock showed
      when we started it running, not the time it should show now.
   */
   ElapsedTime(0);
   error = (t + et) - TimeControl.clock[opponent];
 
   /* Assume our error was in deciding when to start the opponent's
      clock, not in the time it showed at the start.
   */
   time0 += error;
   oldet = et;
   et -= error;
 
   m = (int)strtol (time, &time, 10);
   if (m) TimeControl.moves[opponent] = m;
  }

void
SetMachineTime (CHAR *s)
{
  CHAR *time;
  long tmp = 0;
  int m, t,sec;
  sec = 0;
  time = &s[strlen (CP[197])];
  t = (int)strtol (time, &time, 10);
   if (*time == ':') {
       /* If the time does not contain a ":", it is in hundredths of
        a second; if it does, it is in minutes:seconds. */
       time++;
       sec = (int)strtol(time, &time, 10);
       t = t*6000 + sec*100;
   }
    m = (int)strtol (time, &time, 10);
   if (m) TimeControl.moves[computer] = m;

   tmp = TimeControl.clock[computer];
   TimeControl.clock[computer] = t;
   timecomp[compptr] += (tmp - TimeControl.clock[computer]);
  }


void
InputCommand (void)

/*
 * Process the users command. If easy mode is OFF (the computer is thinking
 * on opponents time) and the program is out of book, then make the 'hint'
 * move on the board and call SelectMove() to find a response. The user
 * terminates the search by entering ^C (quit siqnal) before entering a
 * command. If the opponent does not make the hint move, then set Sdepth to
 * zero.
 */

{
  int eof = 0;
#ifdef QUIETBACKGROUND
  SHORT have_shown_prompt = false;
#endif
  SHORT ok, tmp;
  UTSHORT mv;
  CHAR s[80], sx[80];
  CHAR *p, *q;

#if defined CHESSTOOL
  SHORT normal = false;
#endif

  ok = flag.quit = false;
  player = opponent;
/* if transposition table is filling start it over */
 ZeroTTable(1); /* clear ageing table entries */
/* if we have a move for our opponent, and we are thinking on his time, and not in force mode */
  if (hint > 0 && !flag.easy && !flag.force)
/* if the hint is a promotion don't do anything, we don't know what to promote to. */
    if ((board[hint >> 8] != pawn) || ((row (hint & 0x3f) != 0) && (row (hint & 0x3f) != 7)))
      {
	ft = time0;
	fflush (stdout);
/* create the hint move as string so we can do it */
	algbr ((SHORT) hint >> 8, (SHORT) hint & 0x3f, false);
	strcpy (s, mvstr[0]);
	tmp = epsquare;
#ifdef DEBUG12
#include "debug12.h"
#endif
#if !defined CHESSTOOL
	if (flag.post) GiveHint ();
#endif
/* do the hint move */
	if (VerifyMove (s, 1, &mv))
	  {
#ifdef QUIETBACKGROUND
#ifdef NONDSP
	    PromptForMove ();
#else
	    ShowSidetoMove ();
	    ShowPrompt ();
#endif
	    have_shown_prompt = true;
#endif /* QUIETBACKGROUND */
/* would love to put null move in here */
/* after we make the hint move make a 2 ply search with both plys our moves */
/* think on opponents time */
	    SelectMove (computer, 2);
/* undo the hint and carry on */
	    VerifyMove (s, 2, &mv);
	  }
	epsquare = tmp;
	time0 = ft;
      }
  while (!(ok || flag.quit))
    {
#if defined CHESSTOOL
      normal = false;
#endif
      player = opponent;
#ifdef QUIETBACKGROUND
      if (!have_shown_prompt)
	{
#endif /* QUIETBACKGROUND */
#ifdef NONDSP
	  PromptForMove ();
#else
	  ShowSidetoMove ();
	  ShowPrompt ();
#endif
#ifdef QUIETBACKGROUND
	}
      have_shown_prompt = false;
#endif /* QUIETBACKGROUND */
#ifdef NONDSP
      s[0] = sx[0] = '\0';
      while (!sx[0])
	(void) gets (sx);
#else
      fflush (stdout);
#ifdef MSDOS
      s[0] = '\0';
      eof = ( gets (sx) == NULL );
#else
      eof = ( getstr (sx) == ERR );
#endif
#endif
      sscanf (sx, "%s", s);
      if (eof)
	ExitChess ();
      if (s[0] == '\0')
	continue;
      if(s[0] == '!') printf("%s\n",sx);
      else if (strcmp (s, CP[131]) == 0)	/*bd*/
	{
#if defined CHESSTOOL || defined XBOARD
	  chesstool = 0;
#endif /* CHESSTOOL */
	  ClrScreen ();
	  UpdateDisplay (0, 0, 1, 0);
#if defined CHESSTOOL || defined XBOARD
	  chesstool = 1;
#endif /* CHESSTOOL */
	}
      else if (strcmp (s, CP[129]) == 0) /* noop */ ;	/*alg*/
      else if ((strcmp (s, CP[180]) == 0) || (strcmp (s, CP[216]) == 0))	/* quit exit*/
	flag.quit = true;
      else if (strcmp (s, CP[178]) == 0)	/*post*/
	{ flag.post = true; }
      else if (strcmp (s, CP[19]) == 0)	/*nopost*/
	{ flag.post = false; }
      else if ((strcmp (s, CP[191]) == 0) || (strcmp (s, CP[154]) == 0))	/*set edit*/
	EditBoard ();
#ifdef NONDSP
      else if (strcmp (s, CP[190]) == 0)	/*setup*/
	SetupBoard ();
#endif
      else if (strcmp (s, CP[156]) == 0)	/*first*/
	{
#if defined CHESSTOOL
	  computer = white;
	  opponent = black;
	  flag.force = false;
	  Sdepth = 0;
#endif /* CHESSTOOL */
	  ok = true;
	}
      else if (strcmp (s, CP[162]) == 0)	/*go*/
	{
	  ok = true;
	  flag.force = false;
	  if (computer == white)
	    {
	      computer = black;
	      opponent = white;
	    }
	  else
	    {
	      computer = white;
	      opponent = black;
	    }
	}
      else if (strcmp (s, CP[166]) == 0)	/*help*/
	help ();
      else if (strcmp (s, CP[221]) == 0)	/*material*/
	flag.material = !flag.material;
      else if (strcmp (s, CP[157]) == 0)	/*force*/
	{flag.force = !flag.force; flag.bothsides = false;}
      else if (strcmp (s, CP[134]) == 0)	/*book*/
	Book = Book ? 0 : BOOKFAIL;
      else if (strcmp (s, CP[172]) == 0)	/*new*/
	{
	  NewGame ();
#ifdef XBOARD
	  /* Make this work the old way for compatibility */
	  flag.beep = true;
	  dither = 0;
	  flag.easy = true;
#endif XBOARD
	  UpdateDisplay (0, 0, 1, 0);
	}
      else if (strcmp (s, CP[171]) == 0)	/*list*/
	ListGame ();
      else if (strcmp (s, CP[169]) == 0 || strcmp (s, CP[217]) == 0)	/*level clock*/
	SelectLevel (sx);
      else if (strcmp (s, CP[165]) == 0)	/*hash*/
	flag.hash = !flag.hash;
#ifdef DEBUG
      else if (strcmp (s, CP[18]) == 0)	/*cache*/
	flag.nocache = !flag.nocache;
      else if (strcmp (s, CP[20]) == 0)	/*tt*/
	flag.nott = !flag.nott;
      else if (strcmp (s, CP[21]) == 0)	/*ft*/
	flag.noft = !flag.noft;
#endif
      else if (strcmp (s, CP[227]) == 0)	/*gamein*/
	flag.gamein = !flag.gamein;
      else if (strcmp (s, CP[226]) == 0)	/*beep*/
	flag.beep = !flag.beep;
      else if (strcmp (s, CP[197]) == 0)	/*time*/
	{ SetMachineTime (sx); }
      else if (strcmp (s, CP[228]) == 0)	/*time*/
	{ SetOppTime (sx); }
      else if (strcmp (s, CP[33]) == 0)	/*Awindow*/
	ChangeAlphaWindow ();
      else if (strcmp (s, CP[39]) == 0)	/*Bwindow*/
	ChangeBetaWindow ();
      else if (strcmp (s, CP[183]) == 0)	/*rcptr*/
	flag.rcptr = !flag.rcptr;
      else if (strcmp (s, CP[168]) == 0)	/*hint*/
	GiveHint ();
      else if (strcmp (s, CP[22]) == 0)	/*book hint*/
        LOpeningBook(opponent);
#ifdef ECO
      else if (strcmp (s, "eco") == 0){	/*book hint*/
	ecomove = GameCnt+1;
	GameList[ecomove].hashbd = hashbd;
	GameList[ecomove].hashkey = hashkey;
        EOpeningBook(opponent);}
#endif
      else if (strcmp (s, CP[135]) == 0)	/*both*/
	{
	  flag.bothsides = !flag.bothsides;
          flag.force = false;
	  Sdepth = 0;
	  ElapsedTime (1);
	  SelectMove (opponent, 1);
	  ok = true;
	}
      else if (strcmp (s, CP[185]) == 0)	/*reverse*/
	{
	  flag.reverse = !flag.reverse;
	  ClrScreen ();
	  UpdateDisplay (0, 0, 1, 0);
	}
      else if (strcmp (s, CP[195]) == 0)	/*switch*/
	{
	  computer = computer ^ 1;
	  opponent = opponent ^ 1;
	  xwndw = (computer == white) ? WXWNDW : BXWNDW;
	  flag.force = false;
	  Sdepth = 0;
	  ok = true;
	}
      else if (strcmp (s, CP[203]) == 0)	/*white*/
	{
	  computer = black;
	  opponent = white;
	  xwndw = WXWNDW;
	  flag.force = false;
	  Sdepth = 0;

	  /*
           * ok = true; don't automatically start with white command
           */
	}
      else if (strcmp (s, CP[133]) == 0)	/*black*/
	{
	  computer = white;
	  opponent = black;
	  xwndw = BXWNDW;
	  flag.force = false;
	  Sdepth = 0;

	  /*
           * ok = true; don't automatically start with black command
           */
	}
      else if (strcmp (s, CP[201]) == 0 && GameCnt > 0)	/*undo*/
	{
	  Undo ();
	}
      else if (strcmp (s, CP[184]) == 0 && GameCnt > 1)	/*remove*/
	{
	  Undo ();
	  Undo ();
	}
      else if (strcmp (s, CP[160]) == 0)	/*get*/
	GetGame ();
      else if (strcmp (s, CP[207]) == 0)	/*xget*/
	GetXGame ();
      else if (strcmp (s, CP[189]) == 0)	/*save*/
	SaveGame ();
      else if (strcmp (s, CP[151]) == 0)	/*depth*/
	ChangeSearchDepth ();
#ifdef DEBUG
      else if (strcmp (s, CP[147]) == 0)	/*debuglevel*/
	ChangeDbLev ();
#endif /* DEBUG */
      else if (strcmp (s, CP[164]) == 0)	/*hashdepth*/
	ChangeHashDepth ();
      else if (strcmp (s, CP[182]) == 0)	/*random*/
	dither = (dither)?0:DITHER;
      else if (strcmp (s, "threat") == 0)       /*threat*/
        flag.threat = !flag.threat; /* tom@izf.tno.nl */
      else if (strcmp (s, "null") == 0)         /*null*/
        flag.nonull = !flag.nonull; /* tom@izf.tno.nl */
      else if (strcmp (s, "pvs") == 0)     /*PVS*/
        flag.pvs = !flag.pvs; /* tom@izf.tno.nl */
      else if (strcmp (s, "noscore") == 0)     /*noscore*/
        flag.noscore = !flag.noscore; /* tom@izf.tno.nl */
      else if (strcmp (s, "eval") == 0)     /*new eval*/
        flag.neweval = !flag.neweval; /* tom@izf.tno.nl */
      else if (strcmp (s, "deepnull") == 0)     /*deepnull*/
        flag.deepnull = !flag.deepnull; /* tom@izf.tno.nl */
      else if (strcmp (s, "verydeep") == 0)     /*verydeep*/
        flag.verydeep = !flag.verydeep; /* tom@izf.tno.nl */
      else if (strcmp (s, CP[229]) == 0)	/*hard*/
	flag.easy = false;
      else if (strcmp (s, CP[152]) == 0)	/*easy*/
	flag.easy = !flag.easy;
      else if (strcmp (s, CP[143]) == 0)	/*contempt*/
	SetContempt ();
      else if (strcmp (s, CP[209]) == 0)	/*xwndw*/
	ChangeXwindow ();
      else if (strcmp (s, CP[186]) == 0)	/*rv*/
	{
	  flag.rv = !flag.rv;
	  UpdateDisplay (0, 0, 1, 0);
	}
      else if (strcmp (s, CP[145]) == 0)	/*coords*/
	{
	  flag.coords = !flag.coords;
	  UpdateDisplay (0, 0, 1, 0);
	}
      else if (strcmp (s, CP[193]) == 0)	/*stars*/
	{
	  flag.stars = !flag.stars;
	  UpdateDisplay (0, 0, 1, 0);
	}
#ifdef DEBUG
      else if (strcmp (s, CP[196]) == 0)	/*test*/
	{
	  SHORT cnt;
	  ShowMessage (CP[108]);/*test movelist*/
	  cnt = TestSpeed (MoveList, 20000);
printf("moves = %d\n",cnt);
	  ShowMessage (CP[107]);/*test capturelist*/
	  cnt = TestSpeed (CaptureList, 30000);
printf("moves = %d\n",cnt);
	  ShowMessage (CP[85]);/*test score position*/
	  TestPSpeed (ScorePosition, 15000);
	}
      else
      if (strcmp (s, CP[179]) == 0)	/*p*/
	ShowPostnValues ();
      else if (strcmp (s, CP[148]) == 0)	/*debug*/
	DoDebug ();
	else if (strcmp (s, "Mwpawn") == 0)        /*debug*/
        DoTable (Mwpawn);
	else if (strcmp (s, "Mbpawn") == 0)        /*debug*/
        DoTable (Mbpawn);
	else if (strcmp (s, "Mwknight") == 0)        /*debug*/
        DoTable (Mknight[white]);
	else if (strcmp (s, "Mbknight") == 0)        /*debug*/
        DoTable (Mknight[black]);
	else if (strcmp (s, "Mwbishop") == 0)        /*debug*/
        DoTable (Mbishop[white]);
	else if (strcmp (s, "Mbbishop") == 0)        /*debug*/
        DoTable (Mbishop[black]);
#endif
      else
	{
/* It must be a move */
/* preprocess move;
	remove x or +
	change promotion from h1=Q to h1q
*/
	p=s; q=sx;
	while (*p != '\0'){
	if(*p == 'x' || *p == '+') p++;
	else if(*p == '='){ p++; *q = tolower(*p); p++; q++;}
		else {*q++ = *p++; }
	}
	*q = '\0';

#if defined CHESSTOOL
	  normal = (ok = VerifyMove (sx, 0, &mv));
#else
	  ok = VerifyMove (sx, 0, &mv);
#endif
	}
    }

  /* guess is correct ?*/
  Sdepth = (hint == mv) ? Sdepth-1 : 0;
  if (Tree[0].score > 9000) Sdepth = 0;

  if (flag.force)
    {
      computer = opponent;
      opponent = computer ^ 1;
    }
#if defined CHESSTOOL || defined XBOARD
#if defined CHESSTOOL
  if (normal)
    if (computer == black)
      printz ("%d. %s\n", ++mycnt2, s);
    else
      printz ("%d. ... %s\n", ++mycnt2, s);
#else
  printz ("%d. %s\n", ++mycnt2, s);
#endif
#ifdef notdef /* optional pass best line to frontend with move */
  if (flag.post)
    {
      register int i;

      printz (" %6d ", MSCORE);
      for (i = 1; MV[i] > 0; i++)
	{
	  algbr ((SHORT) (MV[i] >> 8), (SHORT) (MV[i] & 0xFF), false);
	  printz ("%5s ", mvstr[0]);
	}
    }
  printz ("\n");
#endif
#endif /* CHESSTOOL */
#if !defined CHESSTOOL
  signal (SIGINT, TerminateSearch);
#endif
#if !defined(MSDOS) && !defined(WIN32)
  signal (SIGQUIT, TerminateSearch);
#endif /* MSDOS */
}

#ifdef HAVE_GETTIMEOFDAY
void
ElapsedTime (SHORT iop)


/*
 * Determine the time that has passed since the search was started. If the
 * elapsed time exceeds the target (ResponseTime+ExtraTime) then set timeout
 * to true which will terminate the search. iop = 0 calculate et bump ETnodes
 * iop = 1 calculate et set timeout if time exceeded, calculate et
 */

{
  struct timeval tv;
#ifndef MSDOS
  int nchar;
  extern int errno;
  int i;
#ifdef FIONREAD
  if (i = ioctl ((int) 0, FIONREAD, &nchar))
    {
      perror ("FIONREAD");
      fprintf (stderr,
        "You probably have a non-ANSI <ioctl.h>; see README. %d %d %lx\n",
	i, errno, FIONREAD);
      exit (1);
    }

  if (nchar)
    {
      if (!flag.timeout)
	flag.back = true;
      flag.bothsides = false;
    }
#endif /*FIONREAD*/
#else /* MSDOS */
  if (kbhit ())
    {
      if (!flag.timeout)
	flag.back = true;
      flag.bothsides = false;
    }
#endif /* MSDOS */
  gettimeofday(&tv,NULL);
  et = (tv.tv_sec*100+(tv.tv_usec/10000)) - time0;
  ETnodes = NodeCnt + ZNODES;
  if (et < 0)
    et = 0;
  if (iop == 1)
    {
      if (et > ResponseTime + ExtraTime && Sdepth > MINDEPTH)
	flag.timeout = true;
      ETnodes = NodeCnt + ZNODES;
      gettimeofday(&tv,NULL);
      time0 = tv.tv_sec*100+tv.tv_usec/10000;
    }
#if !defined NONDSP
#ifdef QUIETBACKGROUND
  if (!background)
#endif /* QUIETBACKGROUND */
    UpdateClocks ();
#endif
#if defined(Think_C) && defined(Window_Events)
        check_events();
#endif

}
#else
void
ElapsedTime (SHORT iop)


/*
 * Determine the time that has passed since the search was started. If the
 * elapsed time exceeds the target (ResponseTime+ExtraTime) then set timeout
 * to true which will terminate the search. iop = 0 calculate et bump ETnodes
 * iop = 1 calculate et set timeout if time exceeded, calculate et
 */

{
#ifdef WIN32
#ifdef XBOARD
  /* This code works only with a pipe (it doesn't have to be named),
     so we use it only in gnuchessx. */
  DWORD nchar;
  if (PeekNamedPipe(GetStdHandle(STD_INPUT_HANDLE),
                  NULL, 0, NULL, &nchar, NULL)) {
    if (nchar) {
      if (!flag.timeout)
      flag.back = true;
      flag.bothsides = false;
    }
  }
#endif /* XBOARD */
#else /*!WIN32*/
#if !defined(MSDOS)
  int nchar;
  extern int errno;
  int i;
#ifdef FIONREAD
  if (i = ioctl ((int) 0, FIONREAD, &nchar))
    {
      perror ("FIONREAD");
      fprintf (stderr,
        "You probably have a non-ANSI <ioctl.h>; see README. %d %d %x\n",
	i, errno, FIONREAD);
      exit (1);
    }

  if (nchar)
    {
      if (!flag.timeout)
	flag.back = true;
      flag.bothsides = false;
    }
#endif /*FIONREAD*/
#else /* MSDOS */
  if (kbhit ())
    {
      if (!flag.timeout)
	flag.back = true;
      flag.bothsides = false;
    }
#endif /* MSDOS */
#endif /* !WIN32 */
  et = (time ((time_t *) 0) * 100) - time0;
  ETnodes = NodeCnt + ZNODES;
  if (et < 0)
    et = 0;
  if (iop == 1)
    {
      if (et > ResponseTime + ExtraTime && Sdepth > MINDEPTH)
	flag.timeout = true;
      ETnodes = NodeCnt + ZNODES;
      time0 = time ((time_t *) 0) * 100;
    }
#if !defined NONDSP
#ifdef QUIETBACKGROUND
  if (!background)
#endif /* QUIETBACKGROUND */
    UpdateClocks ();
#endif
#if defined(Think_C) && defined(Window_Events)
        check_events();
#endif
}
#endif


static SHORT Nmoves[] = NMOVEINIT;
void
SetTimeControl (void)
{
     flag.gamein = TCflag = false;
      TimeControl.moves[white] = TimeControl.moves[black] = 0;
if (XC){
                  if (XCmore < XC)
                    {
                      TCmoves = XCmoves[XCmore];
                      TCminutes = XCminutes[XCmore];
                      TCseconds = XCseconds[XCmore];
		      TCadd = XCadd[XCmore];
                      XCmore++;
                    }
      if((TCminutes+TCseconds) == 0){TCflag = false;} else {
      TCflag = true;
      if(TCmoves == 0)
	{int i; i = (TCminutes*60+TCseconds + 40 * (TCadd/100))/60; 
		if(i > NMOVELIMIT) i = NMOVELIMIT; 
		TCmoves = Nmoves[i]; flag.gamein = true;}
	else flag.gamein = false;
      TimeControl.moves[white] = TimeControl.moves[black] = TCmoves;
      TimeControl.clock[white] += 6000L * TCminutes + TCseconds * 100;
      TimeControl.clock[black] += 6000L * TCminutes + TCseconds * 100;
    }
  }
  flag.onemove = (TCmoves == 1);
  et = 0;
  ElapsedTime (1);
}
