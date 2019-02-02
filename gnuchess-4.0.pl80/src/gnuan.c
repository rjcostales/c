/*
 * gnuan.c - Analysis interface for gnuchess.
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

/*
 * This is a front end for a chess analysis program.  It takes a file of
 * moves in algebraic notation and creates a file containing each move that
 * was made, the move it would have made, the score it would give itself
 * after making the move it recommended, and the depth it searched for the
 * move.
 * 
 * This is a modification of nondsp.c.  I have cut out code that was not needed
 * for this application such as the help and edit functions.  Most of the
 * modifications were done in InputCommand.
 */

/*
 * This file will generate two different analysis programs.  One is meant to
 * be run as a background process inwhich the in and out files are
 * predefined. The other is meant to be run in the foreground where it will
 * prompt you for the name of the file with the game in it, the maximum depth
 * the search should be conducted to and the time limit per move.  It would
 * be nice to be able to give these parameters on the command line, but that
 * would mean changing main which is in the gnuchess.c file.
 * 
 * For each move it will analyse the move until either it has run out of time or
 * it has reached the maximum depth.
 * 
 * To build the version for background processing define BACKGROUND_ANALYSIS
 * either at the top of this file, or in compilation.  The files and depth
 * used are defined below.  They are MAX_DEPTH, MAX_TIME, OUT_FILE, IN_FILE
 * and PROG_FILE.  The PROG_FILE is a file that will be updated as each move
 * is analysed so you can check its progress.  This is only updated when
 * running the BACKGROUND_ANALYSIS version.  In the version where the
 * filename and depth are entered at runtime, the output goes to stdout.
 */

#undef BACKGROUND_ANALYSIS

#define MAX_DEPTH  MAXDEPTH
#define MAX_MINUTE 0
#define MAX_SECOND 20
#define OUT_FILE   "gnuan.out"
#define IN_FILE    "gnuan.in"
#define PROG_FILE  "gnuan.prog"


#include <ctype.h>
#include <signal.h>
#include <fcntl.h>
#ifdef MSDOS
#include <dos.h>
/*#include <conio.h>*/
#include <stdlib.h>
#include <string.h>
/*#include <io.h>*/
#else
#include <sys/param.h>
#include <sys/types.h>
#include <sys/file.h>
#ifndef AMIGADOS
#include <sys/ioctl.h>
#endif
void TerminateSearch (int), Die (int);

#define O_BINARY 0

#endif /* MSDOS */

#include "gnuchess.h"
#include "ttable.h"
#include "ataks.h"
#undef rxx
#undef cxx
#undef scanz
#undef printz

#define rxx "12345678"
#define cxx "abcdefgh"
#define scanz scanf
#define printz printf
extern SHORT board[64], color[64], Mvboard[64];
extern CHAR *ColorStr[2];
SHORT debuglevel = 0;
int mycnt1, mycnt2;
char mvstr[5][7];

static FILE *fpin;
static FILE *fpout;
int samedepth = false;
CHAR *DRAW;
UTSHORT MV[MAXDEPTH+1];
enum
  {
      XBOARD, GNUCHESS, PGN, OTHER
  }
InFileType;
CHAR InBuf[256];

#ifdef BACKGROUND_ANALYSIS
static FILE *fpprog;

#endif
static CHAR white_actual_move[20];
static CHAR black_actual_move[20];
static CHAR suggest_move[20];
static CHAR suggest_move1[20];
static CHAR suggest_move2[20];
static CHAR suggest_move3[20];
    int total_moves[2];
    int same_moves[2];
static int sdw, sdb;
static int print_score;
int tmpscore;
static int current_depth;
static int current_score;
static int enable_update_display = 0;
static CHAR GNUANmove[400][16];	/* moves for this game strings */
static SHORT GNUANsd[400];	/* search depth for each move */
static SHORT GNUANnext;		/*                               */
static SHORT GNUANhint;		/*                               */
static SHORT GNUANdepth;	/*Search depth override         */
static SHORT GNUANcurrmove;	/* current move within base  */
static SHORT GNUANbasemove;	/* base for curr move set    */
static SHORT GNUANmovecnt;	/* movecnt within base frame        */
static SHORT GNUANnextstop = 0;	/*loop control proceed to this move */
SHORT GNUANtop;			/* # movees in adjlist   */
SHORT GNUANtopLim = 0;			/* # movees in adjlist   */
static unsigned short GNUANadjlist[20];		/*moves to not use       */
static CHAR GNUANinbuf[256];	/* input buffer     */
struct
  {
      SHORT board[64];
      SHORT color[64];
      SHORT mvboard[64];
      SHORT currmove;
      SHORT movecnt;
      SHORT basemove;
      SHORT moveno;
  }
GNUANsave[20];
SHORT GNUANsavedepth;
static enum INenum
  {
      file, keyboard
  }
GNUANinput;
int move_number = 1;

#ifdef notdef
GNUANlist (SHORT mv[])
{
    SHORT B[64], C[64];
    SHORT from, to, i;
    SHORT plain = false;
    memcpy (B, board, sizeof (B));
    memcpy (C, color, sizeof (C));

    for (i = 1; mv[i] != 0; i++)
      {
	  from = mv[i] >> 8;
	  to = mv[i] & 0xff;
	  algbr (from, to, 0);
	  if(plain) fprintf (fpout, "%5s ", mvstr[0]);
	  else fprintf (fpout, "%5s ", mvstr[1]);
	  if (mv[i] == WHITECASTLE)
	    {
		/* castle short */ board[5] = board[7];
		board[7] = no_piece;
		color[5] = color[7];
		color[7] = neutral;
	    }
	  else if (mv[i] == LONGWHITECASTLE)
	    {
		/* castle short */ board[3] = board[0];
		board[0] = no_piece;
		color[3] = color[0];
		color[0] = neutral;
	    }
	  else if (mv[i] == BLACKCASTLE)
	    {
		/* castle short */ board[60] = board[63];
		board[63] = no_piece;
		color[60] = color[63];
		color[63] = neutral;
	    }
	  else if (mv[i] == LONGBLACKCASTLE)
	    {
		/* castle long */ board[59] = board[56];
		board[56] = no_piece;
		color[59] = color[56];
		color[56] = neutral;
	    }
/* could it be en-passant or promote */
	  else if (board[from] == pawn && color[from] == black)
	    {
		if (row (to) == 0)
		  {		/* promote */
    			 plain = true;
		  }
		else if ((row (from) == 3 && row(to) == 2) && 
			((column(to) == (column(from) -1)) || 
			 (column(to) == column(from) +1))
			&& board[to] == no_piece
			&& board[to + 8] == pawn
			&& color[to + 8] == white)
		  {		/* possible en-passant */
		     board[to+8] = no_piece;
		     color[to+8] = neutral;
		  }
	    }
	  else if (board[from] == pawn && color[from] == white)
	    {
		if (row (to) == 7)
		  {		/* promote */
    			 plain=true;
		  }
		else if ((row (from) == 4 && row(to) == 5) && 
			((column(to) == (column(from) -1)) || 
			 (column(to) == column(from) +1))
			&& board[to] == no_piece
			&& board[to - 8] == pawn
			&& color[to - 8] == black)
		  {		/* possible en-passant */
		     board[to+8] = no_piece;
		     color[to+8] = neutral;
		  }
	    }

	  board[to] = board[from];
	  color[to] = color[from];
	  board[from] = no_piece;
	  color[from] = neutral;
      }
    memcpy (board, B, sizeof (B));
    memcpy (color, C, sizeof (C));
    return(0);
}
#endif

void
GNUANadj ()
{
    int i, b;
    for (i = 0; i < GNUANtop; i++)
      {
	  for (b = TrPnt[1]; b < (unsigned) TrPnt[2]; b++)
	    {
		if (((Tree[b].f << 8) | Tree[b].t) == GNUANadjlist[i])
		  {
		      Tree[b].score = DONTUSE;
		      break;
		  }
	    }
      }
}
void
Initialize (void)
{
    mycnt1 = mycnt2 = 0;
}

void
ExitChess (void)
{
    ListGame ();
    exit (0);
}

#ifndef MSDOS			/* never called!!! */
void
Die (int sig)
{
    ExitChess ();
}

#endif /* MSDOS */

void
TerminateSearch (int sig)
{
#ifdef MSDOS
    sig++;			/* shut up the compiler */
#endif /* MSDOS */
    if (!flag.timeout)
	flag.musttimeout = true;
    flag.bothsides = false;
}

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
		if (mvstr[0][0] == mvstr[0][2])		/* pawn did not eat */
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
		      m3p = 3;	/* to row */
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
		  }
		else
		    mvstr[2][m3p] = mvstr[1][2] = '\0';
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
		mvstr[4][0] = mvstr[1][0];
		strcpy (&mvstr[4][1], mvstr[0]);
		if (flag & cstlmask)
		  {
		      if (t > f)
			{
			    strcpy (mvstr[1], mvstr[0]);
			    strcpy (mvstr[0], CP[5]);
			    strcpy (mvstr[2], CP[7]);
			    strcpy (mvstr[3], "O-O");
			}
		      else
			{
			    strcpy (mvstr[1], mvstr[0]);
			    strcpy (mvstr[0], CP[6]);
			    strcpy (mvstr[2], CP[8]);
			    strcpy (mvstr[3], "O-O-O");
			}
		  }
	    }
      }
    else
	mvstr[0][0] = mvstr[1][0] = mvstr[2][0] = mvstr[3][0] = mvstr[4][0] = '\0';
}

int
strxcmp (char *sx, char *sbase)

/*
 * Compare strings sx and sbase, but ignore any 'x' characters found in sx.
 * This kludge lets gnuan parse notation with "x" for capture.
 */
{
    for (;;)
      {
	  if (*sx == 'x')
	      sx++;
	  if (*sx == '\0')
	    {
		if (*sbase == '\0')
		    return 0;
		else
		    return -1;
	    }
	  if (*sx < *sbase)
	      return -1;
	  else if (*sx > *sbase)
	      return 1;
	  sx++;
	  sbase++;
      }
}



int
VerifyMove (CHAR * s, SHORT iop, UTSHORT * mv)

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
	  UnmakeMove (computer, &xnode, &tempb, &tempc, &tempsf, &tempst);
	  return (false);
      }
    cnt = 0;
    MoveList (computer, 2);
    pnt = TrPnt[2];
    while (pnt < TrPnt[3])
      {
	  node = &Tree[pnt++];
	  algbr (node->f, node->t, (SHORT) node->flags);
	  if (strxcmp (s, mvstr[0]) == 0 || strxcmp (s, mvstr[1]) == 0 ||
	      strxcmp (s, mvstr[2]) == 0 || strxcmp (s, mvstr[3]) == 0)
	    {
		cnt++;
		xnode = *node;
	    }
      }
    if (cnt == 1)
      {
	  MakeMove (computer, &xnode, &tempb, &tempc, &tempsf, &tempst);
	  if (SqAtakd (PieceList[computer][0], opponent))
	    {
		UnmakeMove (opponent, &xnode, &tempb, &tempc, &tempsf, &tempst);
		printz ("Illegal move\n");
		return (false);
	    }
	  else
	    {
		if (iop == 1)
		    return (true);

		/*
		 * UpdateDisplay (xnode.f, xnode.t, 0, (SHORT) xnode.flags);
		 */
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
		GameList[GameCnt].time = (SHORT) et;
		TimeControl.clock[opponent] -= et;
		--TimeControl.moves[opponent];
		*mv = (xnode.f << 8) | xnode.t;
		algbr (xnode.f, xnode.t, false);
		return (true);
	    }
      }
#ifdef CHESSTOOL
    printz ("Illegal move\n");
#else
    if (cnt > 1)
	ShowMessage ("Ambiguous Move!");
#endif
    if (cnt == 0)
      {
	  pnt = TrPnt[2];
	  while (pnt < TrPnt[3])
	    {
		node = &Tree[pnt++];
		algbr (node->f, node->t, (SHORT) node->flags);
		printf ("%s -> %s %s %s %s\n", s, mvstr[0], mvstr[1], mvstr[2], mvstr[3]);
	    }
      }
    return (false);
}

void
help (void)
{
}

void
EditBoard (void)

/*
 * Set up a board position. Pieces are entered by typing the piece followed
 * by the location. For example, Nf3 will place a knight on square f3.
 */

{
}

void
SetupBoard (void)

/*
 * Compatibility with Unix chess and the nchesstool. Set up a board position.
 * Eight lines of eight characters are used to setup the board. a8-h8 is the
 * first line. Black pieces are  represented  by  uppercase characters.
 */

{
}

void
ShowDepth (CHAR ch)
{
#ifdef MSDOS
    ch++;			/* shut up the compiler */
#endif /* MSDOS */
}

void
ShowLine (UTSHORT * bstline)
{
    register int i;

    for (i = 1; bstline[i] > 0; i++)
      {
	  if ((i > 1) && (i % 8 == 1))
	      printf ("\n                          ");
	  algbr ((SHORT) (bstline[i] >> 8), (SHORT) (bstline[i] & 0xFF), false);
	  printf ("%5s ", mvstr[0]);
      }
    printf ("\n");
}


void
ShowResults (SHORT score, UTSHORT * bstline, CHAR ch)
{
    int i;

    if (flag.post)
      {
	  ElapsedTime (2);
	  printf ("%2d%c %6d %4ld %8ld  ", Sdepth, ch, score, et / 100, NodeCnt);
	  ShowLine (bstline);
      }
    current_score = score;
    current_depth = Sdepth;
    for (i = 1; bstline[i] > 0; i++)
      {
	  MV[i] = bstline[i];
      }
    MV[i] = 0;
}

void
SearchStartStuff (SHORT side)
{
    signal (SIGINT, TerminateSearch);
#ifndef MSDOS
    signal (SIGQUIT, TerminateSearch);
#endif /* MSDOS */
    if (flag.post)
      {
	  fprintf (stderr, "\nMove# %d    Target= %ld    Clock: %ld\n",
		   move_number,
		   ResponseTime, TimeControl.clock[side]);
      }
}

void
OutputMove (SHORT score)
{
    strcpy (suggest_move, mvstr[0]);
    strcpy (suggest_move1, mvstr[1]);
    strcpy (suggest_move2, mvstr[2]);
    strcpy (suggest_move3, mvstr[3]);
    print_score = current_score;
}


#ifdef HAVE_GETTIMEOFDAY
#include <sys/time.h>
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
#endif /* FIONREAD */
#else
    if (kbhit ())
      {
	  if (!flag.timeout)
	      flag.back = true;
	  flag.bothsides = false;
      }
#endif /* MSDOS */
    gettimeofday (&tv, NULL);
    et = (tv.tv_sec * 100 + (tv.tv_usec / 10000)) - time0;
    ETnodes = NodeCnt + ZNODES;
    if (et < 0)
	et = 0;
    if (iop == 1)
      {
	  if (et > ResponseTime + ExtraTime && Sdepth > MINDEPTH)
	      flag.timeout = true;
	  ETnodes = NodeCnt + ZNODES;
	  gettimeofday (&tv, NULL);
	  time0 = tv.tv_sec * 100 + tv.tv_usec / 10000;
      }
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
#ifndef MSDOS
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
#endif /* FIONREAD */
#else
    if (kbhit ())
      {
	  if (!flag.timeout)
	      flag.back = true;
	  flag.bothsides = false;
      }
#endif /* MSDOS */
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
}

#endif

void
SetTimeControl (void)
{
    if (TCflag)
      {
	  TimeControl.moves[white] = TimeControl.moves[black] = TCmoves;
	  TimeControl.clock[white] = TimeControl.clock[black] = 6000L * TCminutes + 100L * TCseconds;
      }
    else
      {
	  TimeControl.moves[white] = TimeControl.moves[black] = 0;
	  TimeControl.clock[white] = TimeControl.clock[black] = 0;
	  MaxResponseTime = 6000L * TCminutes + 100L * TCseconds;
      }
    et = 0;
    ElapsedTime (1);
}

void
ClrScreen (void)
{
#ifndef CHESSTOOL
    printz ("\n");
#endif
}

void
UpdateDisplay (SHORT f, SHORT t, SHORT redraw, SHORT isspec)
{
#ifndef CHESSTOOL
    SHORT r, c, l;

    if (!enable_update_display)
	return;
    if (redraw)
      {
	  fprintf (fpout, "\n");
	  for (r = 7; r >= 0; r--)
	    {
		for (c = 0; c <= 7; c++)
		  {
		      l = locn (r, c);
		      if (color[l] == neutral)
			  fprintf (fpout, " -");
		      else if (color[l] == white)
			  fprintf (fpout, " %c", qxx[board[l]]);
		      else
			  fprintf (fpout, " %c", pxx[board[l]]);
		  }
		fprintf (fpout, "\n");
	    }
	  fprintf (fpout, "\n");
      }
#endif /* CHESSTOOL */
#ifdef MSDOS
    f++;
    t++;
    isspec++;			/* shut up the compiler */
#endif /* MSDOS */
}

void
GetGame (void)
{
}

void
SaveGame (void)
{
}

void
ListGame (void)
{
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
    if (TCflag && (TCmoves > 1))
	++TimeControl.moves[color[f]];
    hashkey = GameList[GameCnt].hashkey;
    hashbd = GameList[GameCnt].hashbd;
    GameCnt--;
    flag.mate = false;
    Sdepth = 0;

    /*
     * ShowSidetoMove (); UpdateDisplay (0, 0, 1, 0);
     */
    if (flag.regularstart)
	Book = BOOKFAIL;
}

#ifdef notdef
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
	EnPassant (color[t], f, t, 2);
    if (TCflag)
	++TimeControl.moves[color[f]];
    GameCnt--;
    computer = otherside[computer];
    opponent = otherside[opponent];
    flag.mate = false;
    Sdepth = 0;
    InitializeStats ();
}

#endif
void
ShowMessage (CHAR * s)
{
    fprintf (stderr, "%s\n", s);
}

void
ShowSidetoMove (void)
{
}

void
PromptForMove (void)
{
#ifndef CHESSTOOL
    printz ("\nYour move is? ");
#endif /* CHESSTOOL */
}


void
ShowCurrentMove (SHORT pnt, SHORT f, SHORT t)
{
#ifdef MSDOS
    f++;
    t++;
    pnt++;			/* shut up the compiler */
#endif /* MSDOS */
}

void
ChangeAlphaWindow (void)
{
    printz ("window: ");
}

void
ChangeBetaWindow (void)
{
    printz ("window: ");
}

void
GiveHint (void)
{
    algbr ((SHORT) (hint >> 8), (SHORT) (hint & 0xFF), false);
    fprintf (stderr, "Hint: %s\n", mvstr[0]);
}

void
SelectLevel (CHAR * sx)
{
}

void
ChangeSearchDepth (void)
{
    printz ("depth= ");
    scanz ("%d", &MaxSearchDepth);
}

void
SetContempt (void)
{
    printz ("contempt= ");
    scanz ("%hd", &contempt);
}

void
ChangeXwindow (void)
{
    printz ("xwndw= ");
    scanz ("%hd", &xwndw);
}

void
TestSpeed (void (*f) (SHORT side, SHORT ply))
{
    SHORT i;
    long cnt, rate, t1, t2;

    t1 = time (0);
    for (i = 0; i < 10000; i++)
      {
	  f (opponent, 2);
      }
    t2 = time (0);
    cnt = 10000L * (TrPnt[3] - TrPnt[2]);
    rate = cnt / (t2 - t1);
    printz ("Nodes= %ld Nodes/sec= %ld\n", cnt, rate);
}

CHAR cbuf[256];
CHAR sdepth[128];
int sdindex;
SHORT GNCmode;
CHAR *GetNextPtr = (CHAR *) NULL;

signed char GetNextChar ()
{
    GNCmode = false;
    while (true)
      {
	  if (GetNextPtr == NULL)
	    {
		if (fgets (cbuf, sizeof (cbuf), fpin) != NULL)
		  {
		      GetNextPtr = cbuf;
		  }
		else
		    return -1;
	    }
	  else
	      GetNextPtr++;
	  if (*GetNextPtr == '\n' || *GetNextPtr == '\0')
	    {
		GetNextPtr = NULL;
		return ' ';
	    }
	  if (GNCmode)
	      if (*GetNextPtr == '}')
		{
		    GNCmode = false;
		    continue;
		}
	      else
		  continue;
	  if (*GetNextPtr == '{')
	    {
		GNCmode = true;
		continue;
	    }
	  return (*GetNextPtr);

      }
}
int lastws = true;
static signed char GNUANc;
int
GetNextMove (CHAR * buffer)
{
    CHAR *p, *q;

    sdw = sdb = 0;
    black_actual_move[0] = '\0';
    white_actual_move[0] = '\0';
    if (InFileType == PGN)
      {
	  lastws = true;
	  while (!isalpha (GNUANc))
	    {
		if (GNUANc == '0' && lastws)
		    break;
		if (GNUANc == ' ' || GNUANc == '.')
		    lastws = true;
		else
		    lastws = false;
		GNUANc = GetNextChar ();
		if (GNUANc == -1 && GetNextPtr == NULL)
		    return -1;
	    }
	  q = white_actual_move;
	  while (GNUANc != ' ' && GNUANc != -1)
	    {
		if (GNUANc != 'x' && GNUANc != '+')
		    *q++ = GNUANc;
		GNUANc = GetNextChar ();
	    }
	  *q = '\0';
	  lastws = true;
	  while (!isalpha (GNUANc))
	    {
		if (GNUANc == '0' && lastws)
		    break;
		if (GNUANc == ' ')
		    lastws = true;
		else
		    lastws = false;
		GNUANc = GetNextChar ();
		if (GetNextPtr == NULL)
		    return 1;
	    }
	  q = black_actual_move;
	  while (GNUANc != ' ' && GNUANc != -1)
	    {
		if (GNUANc != 'x' && GNUANc != '+')
		    *q++ = GNUANc;
		GNUANc = GetNextChar ();
	    }
	  *q = '\0';
	  return 2;

      }
    if (fgets (cbuf, sizeof (cbuf), fpin) != NULL)
      {
	  p = cbuf;
	  for (; *p; p++)
	    {
		if (isalpha (*p))
		  {
		      q = white_actual_move;
		      while (!isspace (*p))
			  *q++ = *p++;
		      *q = '\0';
		      if (strcmp (white_actual_move, "Book") == 0)
			  continue;
		      else
			  break;
		      break;
		  }
	    }
	  if (*p == '\0' && white_actual_move[0] == '\0')
	      return 0;
	  if (InFileType == GNUCHESS)
	    {
		if (samedepth)
		  {
		      sdindex = 0;
		      while (*p == ' ' || *p == '\n')
			  p++;
		      while (*p != ' ' && *p != '\0')
			  p++;
		      while (*p == ' ' || *p == '\n')
			  p++;
		      while (*p != ' ' && *p != '\n')
			  sdepth[sdindex++] = *p++;
		      sdepth[sdindex] = '\0';
		      if (strcmp (sdepth, "Book") == 0)
			{
			    sdw = -99;
			    for (; *p != ' '; p++);
			}
		      else
			  sdw = atoi (sdepth);
		  }
	    }
	  for (; *p; p++)
	    {
		if (isalpha (*p))
		  {
		      q = black_actual_move;
		      while (!isspace (*p))
			  *q++ = *p++;
		      *q = '\0';
		      if (strcmp (black_actual_move, "Book") == 0)
			  continue;
		      else
			  break;
		  }
	    }
	  if (*p == '\0' && black_actual_move[0] == '\0')
	      return 1;
	  if (InFileType == GNUCHESS)
	    {
		if (samedepth)
		  {
		      sdindex = 0;
		      while (*p == ' ' || *p == '\n')
			  p++;
		      while (*p != ' ' && *p != '\0')
			  p++;
		      while (*p == ' ' || *p == '\n')
			  p++;
		      while (*p != ' ' && *p != '\n')
			  sdepth[sdindex++] = *p++;
		      sdepth[sdindex] = '\0';
		      if (strcmp (sdepth, "Book") == 0)
			{
			    sdb = -99;
			    for (; *p != ' '; p++);
			}
		      else
			  sdb = atoi (sdepth);
		  }
	    }
	  return 2;
      }
    else			/* EOF */
	return -1;

}
UTSHORT mv;
UTSHORT GNUANmv;

int DoIt (SHORT computer, CHAR * smove, SHORT inbook, CHAR * s)
{
    SHORT dok, i;

    dok = VerifyMove (smove, 0, &mv);
    if (!dok)
      {
	  fprintf (fpout, "\nBad move.  %s  Board position is: \n", s);
	  UpdateDisplay (0, 0, 1, 0);
	  flag.quit = 1;
	  return false;
      }
    if (inbook)
      {
	  if (computer == black)
	      fprintf (fpout, "            %-9s   -     -    book\n", smove);
	  else
	      fprintf (fpout, "%3d   %-9s         -     -    book\n", move_number, smove);

	  fflush (fpout);
	  Sdepth = 0;
	  ft = 0;
	  same_moves[computer]++;
      }
    else
      {
	  CHAR x[8];

	  strcpy (x, smove);
	  if (strcmp (smove, suggest_move) == 0
	      || strcmp (smove, suggest_move1) == 0
	      || strcmp (smove, suggest_move2) == 0
	      || strcmp (smove, suggest_move3) == 0)
	    {
		same_moves[computer]++;
	    }
	  else
	      strcat (x, "*");
	  if (computer == white)
	    {
		if (sdw == -99)
		    fprintf (fpout, "%3d   %-9s      %5d   Book ", move_number, x, print_score);
		else
		    fprintf (fpout, "%3d   %-9s      %5d%5d   ", move_number, x, print_score, current_depth);
	    }
	  else
	    {
		if (sdb == -99)
		    fprintf (fpout, "            %-9s%5d  Book  ", x, print_score);
		else
		    fprintf (fpout, "            %-9s%5d%5d   ", x, print_score, current_depth);
	    }
	  for (i = 1; MV[i] > 0; i++)
	    {
/*		if(i == 1)fprintf (fpout, "%5s ", suggest_move1); */
/*		else*/ {algbr ((SHORT) (MV[i] >> 8), (SHORT) (MV[i] & 0xFF), false);
		fprintf (fpout, "%5s ", mvstr[0]);}
	    }
#ifdef notdef
	  GNUANlist (MV);
#endif
	  fprintf (fpout, "\n");
	  fflush (fpout);
	  Sdepth = 0;
	  ft = 0;
      }
    return true;
}

void
GNUANvarit ()
{
    memcpy (&GNUANsave[GNUANsavedepth].board, board, sizeof (board));
    memcpy (&GNUANsave[GNUANsavedepth].color, color, sizeof (color));
    memcpy (&GNUANsave[GNUANsavedepth].mvboard, Mvboard, sizeof (Mvboard));
    GNUANsave[GNUANsavedepth].currmove = GNUANcurrmove;
    GNUANsave[GNUANsavedepth].basemove = GNUANbasemove;
    GNUANsave[GNUANsavedepth].movecnt = GNUANmovecnt;
    GNUANcurrmove = GNUANmovecnt - 1;
    GNUANbasemove = GNUANmovecnt;
    GNUANmovecnt = 0;
    GNUANsave[GNUANsavedepth].moveno = move_number;
    GNUANsavedepth++;
}
void
GNUANravit ()
{
    GNUANsavedepth--;
    memcpy (board, &GNUANsave[GNUANsavedepth].board, sizeof (board));
    memcpy (color, &GNUANsave[GNUANsavedepth].color, sizeof (color));
    memcpy (Mvboard, &GNUANsave[GNUANsavedepth].mvboard, sizeof (Mvboard));
    GNUANcurrmove = GNUANsave[GNUANsavedepth].currmove;
    GNUANbasemove = GNUANsave[GNUANsavedepth].basemove;
    GNUANmovecnt = GNUANsave[GNUANsavedepth].movecnt;
    move_number = GNUANsave[GNUANsavedepth].moveno;
}

int Undid = false;

void
InputCommand (void)

/*
 * Open the file of moves to analyse.  Go through the file move by move and
 * do the following for each move.  See what gnuchess would have done in that
 * position.  Record the move it would have made along with the score it
 * would have given and the search depth.  Take back its move. Force the move
 * that was actually made.  Keep track of statistics such as how many moves
 * agreed.
 */

{
    SHORT ok, inbook;
    CHAR s[80];
    int search_depth;

#if !defined BACKGROUND_ANALYSIS
    int max_minutes;
    int max_seconds;
    CHAR inbuf[256];
#else
    CHAR outfilename[255];
    CHAR progfilename[255];
#endif
    CHAR infilename[255];
    long start_time, end_time;
    double elapsed_time;
    float move_percent[2];
    CHAR GNUANinbuf1[128];
    CHAR *p, *q;
    int mr;

    /* Initialize necessary variables. */

    flag.quit = false;
    flag.beep = false;
    flag.post = true;
    enable_update_display = 1;
    ft = 0;
    Book = false;
    total_moves[0] = total_moves[1] = 0;
    same_moves[0] = same_moves[0] = 0;

#ifdef BACKGROUND_ANALYSIS

    /*
     * Set the in files to standard ones for analysis if background
     * processing selected.
     */

    strcpy (infilename, IN_FILE);
    strcpy (outfilename, OUT_FILE);
    strcpy (progfilename, PROG_FILE);
    fpout = fopen (outfilename, "w");
    if (fpout == NULL)
      {
	  fprintf (fpout, "This file does not exist : %s\n", outfilename);
	  flag.quit = 1;
	  return;
      }
    MaxSearchDepth = search_depth = MAX_DEPTH;
    TCminutes = MAX_MINUTE;
    TCseconds = MAX_SECOND;

#else

    /* Request information on where the file is and the depth an time. */

    fpout = stderr;
    fprintf (fpout, "Input the file with the algebraic moves of the game.\n");
    fflush (fpout);
    gets (infilename);
    fprintf (fpout, "\n");
    do
      {
	  fprintf (fpout, "Input the search depth you want to use.  (1 to 29)(- for depth from file)\n");
	  gets (inbuf);
	  search_depth = atoi (inbuf);
	  if (search_depth < 0)
	    {
		samedepth = true;
		search_depth = -search_depth;
	    }
      }
    while (search_depth < 1 || search_depth > 29);
    MaxSearchDepth = search_depth;

    fprintf (fpout, "\n");
    max_minutes = max_seconds = 0;
    do
      {
	  fprintf (fpout, "Input the maximum number of minutes per move you want to use.\n");
	  gets (inbuf);
	  max_minutes = atoi (inbuf);
	  if (max_minutes < 0)
	    {
		max_seconds = -max_minutes;
		max_minutes = 0;
	    }
      }
    while ((max_minutes + max_seconds) < 1);
    fprintf (fpout, "Post the search results(y/n).\n");
    gets (inbuf);
    if (inbuf[0] == 'y')
	flag.post = true;
    else
	flag.post = false;
    printf ("\nYou will search to %d half moves\n", search_depth);
    printf ("\nWith no search taking more than %d:%02d minutes\n", max_minutes, max_seconds);
    TCminutes = max_minutes;
    TCseconds = max_seconds;

    fprintf (fpout, "\n\n");

#endif

    fpin = fopen (infilename, "r");
    if (fpin == NULL)
      {
	  fprintf (fpout, "This file does not exist : %s\n", infilename);
	  flag.quit = 1;
	  return;
      }
    /* lets find out about this file */
    fgets (InBuf, 256, fpin);
    InFileType = OTHER;
    if (!strncmp (InBuf, "xboard", 6) || !strncmp (InBuf, "# xboard", 8))
      {
	  InFileType = XBOARD;
	  fgets (InBuf, 256, fpin);
	  fgets (InBuf, 256, fpin);
      }
    else if (!strncmp (InBuf, "gnuchess", 6))
      {
	  InFileType = GNUCHESS;
	  fgets (InBuf, 256, fpin);
      }
    else if (InBuf[0] == '[')
      {
	  InFileType = PGN;
	  while (InBuf[0] == '[')
	    {
		fprintf (fpout, "%s", InBuf);
		fgets (InBuf, 256, fpin);
	    }
	  fprintf (fpout, "\n");
      }
    else
	rewind (fpin);
    TCmoves = 1;
    TCflag = (TCmoves > 1);
    /* flag.onemove = true; */
    OperatorTime = 0;
    SetTimeControl ();


    fprintf (fpout, "Move White Black      Score Depth     Best Line\n");
    fprintf (fpout, "------------------------------------------------------------------\n");

#ifdef BACKGROUND_ANALYSIS

    /*
     * Update progress in the progress file if this is running in the
     * background.
     */

    fpprog = fopen (progfilename, "w");
    fprintf (fpprog, "Done with move #%d.\n", move_number - 1);
    fclose (fpprog);

#endif
    GNUANmovecnt = 0;
    GNUANbasemove = 0;
    while ((mr = GetNextMove (s)) > 0)
      {
	  strcpy (GNUANmove[GNUANmovecnt], white_actual_move);
	  GNUANsd[GNUANmovecnt] = (sdw == -99) ? search_depth : (sdw) ? sdw : search_depth;
	  GNUANmovecnt++;
  	  if (mr == 1) break;
	  strcpy (GNUANmove[GNUANmovecnt], black_actual_move);
	  GNUANsd[GNUANmovecnt] = (sdb == -99) ? search_depth : (sdb) ? sdb : search_depth;
	  GNUANmovecnt++;
	  if (!strcmp (white_actual_move, "Black") || !strcmp (white_actual_move, "White") || !strcmp (white_actual_move, "draw"))
	      break;
      }
    printf ("Got %d moves\n", GNUANmovecnt);

    /* all the moves are stored now start */
    opponent = black;
    computer = white;
    GNUANinput = file;
    GNUANnext = false;
    GNUANnextstop = GNUANsavedepth = GNUANcurrmove = 0;
    time (&start_time);
    Undid = false;
    while (GNUANcurrmove < (GNUANbasemove + GNUANmovecnt))
      {

#ifdef IGNUAN
/************************************************************************/
	  /* gnuan interactive                                */
/************************************************************************/

	  /*
	   * Commands: var - initiate a variation - save current another - find
	   * next best move for this position top - always look at the top n
	   * moves comment - copy to output rav - return from variation go get
	   * next move from input
	   */
/************************************************************************/
#endif
	  if (GNUANdepth)
	      MaxSearchDepth = GNUANdepth;
	  else
	    {
		if (GNUANinput == file)
		    MaxSearchDepth = GNUANsd[GNUANcurrmove];
		else
		    MaxSearchDepth = 4;
	    }
	  if (GNUANnext)
	    {
		GNUANnext = false;
		inbook = false;
	    }
	  else
	    {
		VMoveList (computer, 1);
		inbook = GOpeningBook (&GNUANhint, computer, GNUANmove[GNUANcurrmove]);
	    }

	  if (!inbook)
	    {
		flag.force = 0;
		SelectMove (computer, 1);
		Undo ();
		SetTimeControl ();
		flag.force = 1;
	    }
	  if (GNUANinput == file)
	      ok = DoIt (computer, GNUANmove[GNUANcurrmove], inbook, s);
	  else
	      ok = DoIt (computer, GNUANinbuf, inbook, s);
	  if (!ok)
	    {
		if (GNUANinput == keyboard)
		    GNUANmovecnt--;
		GNUANcurrmove--;
	    }
#ifdef BACKGROUND_ANALYSIS

	  /*
	   * Update progress in the progress file if this is running in the
	   * background.
	   */

	  fpprog = fopen (progfilename, "w");
	  fprintf (fpprog, "Done with move #%d.\n", move_number - 1);
	  fclose (fpprog);
#else

	  /*
	   * fprintf(stderr, "Done with move #%d.\n", move_number-1);
	   */

#endif
#ifdef IGNUAN
	  /* we have done the move now any commands */
	  if(GNUANtopLim){
		if(GNUANtop < GNUANtopLim){
			 GNUANadjlist[GNUANtop++] = GNUANmv;
                          GNUANnext = true;
                          Undo ();
                          Undid = true;
			}
		else GNUANtop = 0;
		}
	  if (GNUANcurrmove >= GNUANnextstop) GNUANnextstop = 0;
	  if (GNUANnextstop == 0 && (GNUANtopLim == 0 || GNUANtop == 0))
	      while (true)
		{
		    printf (">");
		    gets (GNUANinbuf1);
		    p = GNUANinbuf1;
		    q = GNUANinbuf;
		    while (true)
		      {
			  while (true)
			    {
				if (*p == ' ')
				  {
				      p++;
				      continue;
				  }
				else
				    break;
			    }
			  while (true)
			    {
				if (*p != ' ')
				  {
				      if (*p == '\0')
					{
					    break;
					}
				      *q++ = *p++;
				      continue;
				  }
				else
				    break;
			    }
			  if (*p == '\0')
			      break;
			  else
			      *q++ = ' ';
		      }
		    *q = '\0';
		    if (strcmp ("var", GNUANinbuf) == 0)
		      {
			  GNUANvarit ();
			  GNUANinput = keyboard;
		      }
		    else if (strcmp ("rav", GNUANinbuf) == 0)
		      {
			  if (GNUANsavedepth == 0)
			    {
				printf ("Error not in Var\n");
				continue;
			    }
			  GNUANravit ();
			  InitializeStats ();
			  GNUANinput = (GNUANsavedepth ? keyboard : file);
		      }
		    else if (strcmp ("next", GNUANinbuf) == 0 || strcmp ("n", GNUANinbuf) == 0)
		      {
			  GNUANadjlist[GNUANtop++] = GNUANmv;
			  GNUANnext = true;
			  Undo ();
			  Undid = true;
			  break;
		      }
		    else if ((strncmp ("depth", GNUANinbuf, 5) == 0))
		      {
			  int i = strtol (&GNUANinbuf[5], NULL, 10);
			  if (i > 0)
			      GNUANdepth = i;
			  else
			    {
				printf ("Illegal depth %d\n", i);
				continue;
			    }
		      }
		    else if (strcmp ("wn", GNUANinbuf) == 0)
		      {
			  GNUANtop = 0;
		      }
		    else if (strncmp ("d", GNUANinbuf, 2) == 0)
		      {
			  int i = strtol (&GNUANinbuf[2], NULL, 10);
			  if (i > 0)
			      GNUANdepth = i;
			  else
			    {
				printf ("Illegal depth %d\n", i);
				continue;
			    }
		      }
		    else if (strncmp ("top", GNUANinbuf, 3) == 0)
		      {
			  int i = strtol (&GNUANinbuf[4], NULL, 10);
			  if (i > 1 && i < 6)
			      GNUANtopLim = i-1;
			  else
			      printf ("Illegal range for top %d\n", i);
		      }
		    else if (strncmp ("t", GNUANinbuf, 2) == 0)
		      {
			  int i = strtol (&GNUANinbuf[2], NULL, 10);
			  if (i > 0 && i < 99)
			      GNUANtop = i;
			  else
			      printf ("Illegal range for top %d\n", i);
		      }
		    else if (strcmp ("bd", GNUANinbuf) == 0)
		      {
			  UpdateDisplay (0, 0, 1, 0);
		      }
		    else if (strcmp ("quit", GNUANinbuf) == 0)
		      {
			  exit (1);
		      }
		    else if (strcmp ("go", GNUANinbuf) == 0 || strcmp ("g", GNUANinbuf) == 0)
		      {
			  if (!GNUANsavedepth)
			      break;
			  else
			      printf ("In variation need move or rav\n");
		      }
		    else if (isdigit (GNUANinbuf[0]))
		      {
			  /* means go to move nn */
			  GNUANnextstop = 2 * (strtol (GNUANinbuf, &p, 10) - 1) - 1;
			  if (GNUANnextstop <= GNUANcurrmove)
			    {
				printf ("already past that move\n");
				GNUANnextstop = 0;
				continue;
			    }
			  break;
		      }
		    else
		      {		/* none of above must be a move if in a var */
			  if (!GNUANsavedepth)
			    {
				printf ("Illegal input:must be in var to specify move\n");
				continue;
			    }
			  if (strlen (GNUANinbuf) < 2)
			    {
				printf ("Illegal input:%s\n", GNUANinbuf);
				continue;
			    }
			  else
			    {
				strcpy (GNUANmove[GNUANbasemove + GNUANmovecnt], GNUANinbuf);
				GNUANmovecnt++;
				GNUANinput = keyboard;
				break;
			    }
		      }

		}
#endif
	  if (!Undid)
	    {
		computer ^= 1;
		opponent ^= 1;
		GNUANcurrmove++;
	        if (computer == white) move_number++;
	    }
	  Undid = false;
      }

    move_percent[white] = ((float) same_moves[white]) * 200 / (float) (GNUANmovecnt + (GNUANmovecnt % 2 ? 1 : 0));
    move_percent[black] = ((float) same_moves[black]) * 200 / (float) (GNUANmovecnt + (GNUANmovecnt % 2 ? 1 : 0));
    fprintf (fpout, "\n           White's percentage was %5.2f%%.     Black's percentage was %5.2f%%.\n", move_percent[white], move_percent[black]);
    time (&end_time);
    elapsed_time = difftime(start_time,end_time);
    fprintf (fpout, "\n           Elapsed time was %ld seconds.\n", elapsed_time);
    fclose (fpin);
    fclose (fpout);
    exit (0);
}

void
Ldisplay1 ()
{
};
void
Ldisplay2 ()
{
};
void
Ldisplay ()
{
};
void
Ldisplay3 ()
{
};
void
Ldisplay4 ()
{
};
int ecomove;
