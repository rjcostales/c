/*
  nondsp.c - UNIX & MSDOS NON-DISPLAY, AND CHESSTOOL interface for Chess

  Revision: 1991-04-15

  Copyright (C) 1986, 1987, 1988, 1989, 1990 Free Software Foundation, Inc.
  Copyright (c) 1988, 1989, 1990  John Stanback

  This file is part of CHESS.

  CHESS is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY.  No author or distributor accepts responsibility to anyone for
  the consequences of using it or for whether it serves any particular
  purpose or works at all, unless he says so in writing.  Refer to the CHESS
  General Public License for full details.

  Everyone is granted permission to copy, modify and redistribute CHESS, but
  only under the conditions described in the CHESS General Public License.
  A copy of this license is supposed to have been given to you along with
  CHESS so you can know your rights and responsibilities.  It should be in a
  file named COPYING.  Among other things, the copyright notice and this
  notice must be preserved on all copies.
*/

#include <ctype.h>
#include <signal.h>
#ifdef MSDOS
#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#else
#include <sys/param.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
void TerminateSearch (int), Die (int);
#endif /* MSDOS */

#include "gnuchess.h"

#define pxx " PNBRQK"
#define qxx " pnbrqk"
#define rxx "12345678"
#define cxx "abcdefgh"
#define printz printf
#define scanz scanf
char Analysis[128] = "";
short int MV[29];
int MSCORE;
#ifdef DEBUG
short int debuglevel = 0;
FILE *debugfd;
#endif /*DEBUG*/
#ifdef CHESSTOOL
short int chesstool = 1;
#else
short int chesstool = 0;
#endif /*CHESSTOOL*/
char mvstr[4][6];
static char *ColorStr[2] =
{"White", "Black"};
static long evrate;
static int mycnt1, mycnt2;
static int ahead;
char *DRAW;

void
Initialize (void)
{
  mycnt1 = mycnt2 = 0;
#ifdef CHESSTOOL
  setlinebuf (stdout);
  /* setvbuf(stdout,NULL,_IOLBF,BUFSIZ); */
  printf ("Chess\n");
  if (Level == 0 && !TCflag)
    Level = 15;
#endif /* CHESSTOOL */
}

void
ExitChess (void)
{
  ListGame ();
  exit (0);
}

#ifndef MSDOS				/* never called!!! */
void
Die (int sig)
{
  char s[80];
  ShowMessage ("Abort? ");
  scanz ("%s", s);
  if (strcmp (s, "yes") == 0)
    ExitChess ();
}

#endif /* MSDOS */

void
TerminateSearch (int sig)
{
#ifdef MSDOS
  sig++;				/* shut up the compiler */
#endif /* MSDOS */
  flag.timeout = true;
  flag.bothsides = false;
}

void
algbr (short int f, short int t, short int flag)


/*
   Generate move strings in different formats.
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
	  if (mvstr[0][0] == mvstr[0][2])	/* pawn did not eat */
	    {
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
	      m3p = 3;			/* to row */
	    }
	  if (flag & promote)
	    {
	      mvstr[0][4] = mvstr[1][2] = mvstr[2][m3p] = qxx[flag & pmask];
	      mvstr[1][3] = mvstr[2][m3p + 1] = mvstr[0][5] = '\0';
	    }
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
VerifyMove (char *s, short int iop, short unsigned int *mv)

/*
   Compare the string 's' to the list of legal moves available for the
   opponent. If a match is found, make the move on the board.
*/

{
  static short pnt, tempb, tempc, tempsf, tempst, cnt;
  static struct leaf xnode;
  struct leaf *node;

  *mv = 0;
  if (iop == 2)
    {
      UnmakeMove (opponent, &xnode, &tempb, &tempc, &tempsf, &tempst);
      return (false);
    }
  cnt = 0;
  MoveList (opponent, 2);
  pnt = TrPnt[2];
  while (pnt < TrPnt[3])
    {
      node = &Tree[pnt++];
      algbr (node->f, node->t, (short) node->flags);
      if (strcmp (s, mvstr[0]) == 0 || strcmp (s, mvstr[1]) == 0 ||
	  strcmp (s, mvstr[2]) == 0 || strcmp (s, mvstr[3]) == 0)
	{
	  cnt++;
	  xnode = *node;
	}
    }
  if (cnt == 1)
    {
      MakeMove (opponent, &xnode, &tempb, &tempc, &tempsf, &tempst, &INCscore);
      if (SqAtakd (PieceList[opponent][0], computer))
	{
	  UnmakeMove (opponent, &xnode, &tempb, &tempc, &tempsf, &tempst);
	  printz ("Illegal move..(you are in check)\n");
	  return (false);
	}
      else
	{
	  if (iop == 1)
	    return (true);
	  UpdateDisplay (xnode.f, xnode.t, 0, (short) xnode.flags);
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
	  GameList[GameCnt].time = (short) et;
	  TimeControl.clock[opponent] -= et;
	  --TimeControl.moves[opponent];
	  *mv = (xnode.f << 8) | xnode.t;
	  algbr (xnode.f, xnode.t, false);
	  return (true);
	}
    }
#ifdef CHESSTOOL
  printz ("Illegal move (no matching move generated)");
  printz ("%s ", s);
#ifdef DEBUG
  if (true)
    {
      short r, c, l;
      pnt = TrPnt[2];
      fprintf (debugfd, "Illegal Move\nInput move is |%s|\n", s);
      fprintf (debugfd, "Generated moves are:\n");
      while (pnt < TrPnt[3])
	{
	  node = &Tree[pnt++];
	  algbr (node->f, node->t, (short) node->flags);
	  fprintf (debugfd, "|%s|%s|%s|%s|\n", mvstr[0], mvstr[1], mvstr[2], mvstr[3]);
	}
      fprintf (debugfd, "\nCurrent board is:\n");
      for (r = 7; r >= 0; r--)
	{
	  for (c = 0; c <= 7; c++)
	    {
	      l = locn (r, c);
	      if (color[l] == neutral)
		fprintf (debugfd, " -");
	      else if (color[l] == white)
		fprintf (debugfd, " %c", qxx[board[l]]);
	      else
		fprintf (debugfd, " %c", pxx[board[l]]);
	    }
	  fprintf (debugfd, "\n");
	}
      fprintf (debugfd, "\n");
    }
#endif /* DEBUG*/
#else
  if (cnt > 1)
    ShowMessage ("Ambiguous Move!");
#endif /*CHESSTOOL*/
  return (false);
}

void
help (void)
{
  ClrScreen ();
  printz ("CHESS command summary\n");
  printz ("----------------------------------------------------------------\n");
  printz ("g1f3      move from g1 to f3      quit      Exit Chess\n");
  printz ("Nf3       move knight to f3       beep      on/off\n");
  printz ("o-o       castle king side        easy      on/off\n");
  printz ("o-o-o     castle queen side       hash      on/off\n");
  printz ("bd        redraw board            reverse   board display\n");
  printz ("list      game to chess.lst       book      on/off\n");
  printz ("undo      undo last ply           remove    take back a move\n");
  printz ("edit      edit board              force     enter game moves\n");
  printz ("switch    sides with computer     both      computer match\n");
  printz ("white     computer plays white    black     computer plays black\n");
  printz ("depth     set search depth        clock     set time control\n");
  printz ("post      principle variation     hint      suggest a move\n");
  printz ("save      game to file            get       game from file\n");
  printz ("random    randomize play          new       start new game\n");
  printz ("----------------------------------------------------------------\n");
  printz ("Computer: %-12s Opponent:            %s\n",
	  ColorStr[computer], ColorStr[opponent]);
  printz ("Depth:    %-12d Response time:       %d sec\n",
	  MaxSearchDepth, Level);
  printz ("Random:   %-12s Easy mode:           %s\n",
	  (dither) ? "ON" : "OFF", (flag.easy) ? "ON" : "OFF");
  printz ("Beep:     %-12s Transposition table: %s\n",
	  (flag.beep) ? "ON" : "OFF", (flag.hash) ? "ON" : "OFF");
  signal (SIGINT, TerminateSearch);
#ifndef MSDOS
  signal (SIGQUIT, TerminateSearch);
#endif /* MSDOS */
}

void
EditBoard (void)
/*
   Set up a board position. Pieces are entered by typing the piece followed
   by the location. For example, Nf3 will place a knight on square f3.
*/

{
  short a, r, c, sq, i;
  char s[80];

  ClrScreen ();
  UpdateDisplay (0, 0, 1, 0);
  printz (".   exit to main\n");
  printz ("#   clear board\n");
  printz ("c   change sides\n");
  printz ("enter piece & location: \n");

  a = white;
  do
    {
      scanz ("%s", s);
      if (s[0] == '#')
	for (sq = 0; sq < 64; sq++)
	  {
	    board[sq] = no_piece;
	    color[sq] = neutral;
	  }
      if (s[0] == 'c' || s[0] == 'C')
	a = otherside[a];
      c = s[1] - 'a';
      r = s[2] - '1';
      if ((c >= 0) && (c < 8) && (r >= 0) && (r < 8))
	{
	  sq = locn (r, c);
	  color[sq] = a;
	  board[sq] = no_piece;
	  for (i = no_piece; i <= king; i++)
	    if ((s[0] == pxx[i]) || (s[0] == qxx[i]))
	      {
		board[sq] = i;
		break;
	      }
	}
  } while (s[0] != '.');
  for (sq = 0; sq < 64; sq++)
    Mvboard[sq] = (board[sq] != Stboard[sq]) ? 10 : 0;
  GameCnt = 0;
  Game50 = 1;
  ZeroRPT ();
  InitializeStats ();
  ClrScreen ();
  UpdateDisplay (0, 0, 1, 0);
}

void
SetupBoard (void)

/*
   Compatibility with Unix chess and the nchesstool.
   Set up a board position. Eight lines of eight characters are used
   to setup the board. a8-h8 is the first line.
   Black pieces are  represented  by  uppercase characters.
*/

{
  short r, c, sq, i;
  char ch;
  char s[80];

  NewGame ();

  gets (s);				/* skip "setup" command */
  for (r = 7; r >= 0; r--)
    {
      gets (s);
      for (c = 0; c <= 7; c++)
	{
	  ch = s[c];
	  sq = locn (r, c);
	  color[sq] = neutral;
	  board[sq] = no_piece;
	  for (i = no_piece; i <= king; i++)
	    if (ch == pxx[i])
	      {
		color[sq] = black;
		board[sq] = i;
		break;
	      }
	    else if (ch == qxx[i])
	      {
		color[sq] = white;
		board[sq] = i;
		break;
	      }
	}
    }
  for (sq = 0; sq < 64; sq++)
    Mvboard[sq] = (board[sq] != Stboard[sq]) ? 10 : 0;
  InitializeStats ();
  ClrScreen ();
  UpdateDisplay (0, 0, 1, 0);
  printz ("Setup successful\n");
}

void
ShowDepth (char ch)
{
#ifdef MSDOS
  ch++;					/* shut up the compiler */
#endif /* MSDOS */
}

#ifdef DEBUG
void
ShowDBLine (const char *x, short int ply, short int depth,
	    short int alpha, short int beta, short int score,
	    short unsigned int *bstline)
{
  register int i;
  if (debuglevel > 2)
    {
      fprintf (debugfd, "%s ply %d depth %d alpha %d beta %d score %d",
	       x, ply, depth, alpha, beta, score);
      for (i = 1; bstline[i] != 0; i++)
	{
	  if ((i > 1) && (i % 8 == 1))
	    fprintf (debugfd, "\n                          ");
	  algbr ((short) (bstline[i] >> 8), (short) (bstline[i] & 0xFF), false);
	  fprintf (debugfd, "%5s ", mvstr[0]);
	}
      fprintf (debugfd, "\n");
    }
}

#endif /*DEBUG*/

void
ShowLine (short unsigned int *bstline)
{
  register int i;
  for (i = 1; bstline[i] > 0; i++)
    {
      if ((i > 1) && (i % 8 == 1))
	fprintf (stderr, "\n                          ");
      algbr ((short) (bstline[i] >> 8), (short) (bstline[i] & 0xFF), false);
      fprintf (stderr, "%5s ", mvstr[0]);
    }
  fprintf (stderr, "\n");
}

void
ShowResults (short int score, short unsigned int *bstline, char ch)
{
  register int i;
#ifndef CHESSTOOL
  if (flag.post)
    {
      fprintf (stderr, "%2d%c %6d %4ld %8ld  ", Sdepth, ch, score, et, NodeCnt);
      ShowLine (bstline);
    }
#else
  MSCORE = score;
  for (i = 1; bstline[i] > 0; i++)
    {
      MV[i] = bstline[i];
    } MV[i] = 0;
#endif /* CHESSTOOL */
}

void
SearchStartStuff (short int side)
{
  signal (SIGINT, TerminateSearch);
#ifndef MSDOS
  signal (SIGQUIT, TerminateSearch);
#endif /* MSDOS */
#ifndef CHESSTOOL
  if (flag.post)
    {
      fprintf (stderr, "\nMove# %d    Target= %ld    Clock: %ld\n",
	       TCmoves - TimeControl.moves[side] + 1,
	       ResponseTime, TimeControl.clock[side]);
    }
#endif /* CHESSTOOL */
}

void
OutputMove (void)
{
  printz ("%d. ... %s\n", ++mycnt1, mvstr[0]);
#ifdef DEBUG
  fprintf (debugfd,"%d. ... %s\n", ++mycnt1, mvstr[0]);
#endif /*DEBUG*/
  if (root->flags & draw)
    {
      printz ("Draw %s\n", DRAW);
      ExitChess ();
    }
  if (root->score == -9999)
    {
      printz ("%s\n", ColorStr[opponent]);
      ExitChess ();
    }
  if (root->score == 9998)
    {
      printz ("%s\n", ColorStr[computer]);
      ExitChess ();
    }
#ifndef CHESSTOOL
  if (flag.post)
    {
      fprintf (stderr, "Nodes= %ld  Eval= %ld  Rate= %ld  ",
	       NodeCnt, EvalNodes, evrate);
      fprintf (stderr, "Hin/Hout/Coll/Fin/Fout = %ld/%ld/%ld/%ld/%ld\n",
	       HashAdd, HashCnt, HashCol, FHashAdd, FHashCnt);
    }
#endif /* CHESSTOOL */
  UpdateDisplay (root->f, root->t, 0, root->flags);
  fprintf (stderr, "My move is: %s\n", mvstr[0]);
#ifdef DEBUG
  fprintf (debugfd, "My move is: %s\n", mvstr[0]);
#endif /* DEBUG */
  if (flag.beep)
    printz ("%c", 7);

  if (root->flags & draw)
    fprintf (stderr, "Drawn game!\n");
  else if (root->score == -9999)
    fprintf (stderr, "opponent mates!\n");
  else if (root->score == 9998)
    fprintf (stderr, "computer mates!\n");
  else if (root->score < -9000)
    fprintf (stderr, "opponent will soon mate!\n");
  else if (root->score > 9000)
    fprintf (stderr, "computer will soon mate!\n");
}

void
ElapsedTime (short int iop)


/*
   Determine the time that has passed since the search was started. If the
   elapsed time exceeds the target (ResponseTime+ExtraTime) then set timeout
   to true which will terminate the search.
*/

{
  if (ahead)
    {
#ifndef MSDOS
      long nchar;
      ioctl (0, FIONREAD, &nchar);
      if (nchar)
#else
      if (kbhit ())
#endif /* MSDOS */
	{
	  flag.timeout = true;
	  flag.bothsides = false;
	}
    }
  et = time ((long *) 0) - time0;
  if (et < 0)
    et = 0;
  ETnodes += 50;
  if (et > et0 || iop == 1)
    {
      if (et > ResponseTime + ExtraTime && Sdepth > 1)
	flag.timeout = true;
      et0 = et;
      if (iop == 1)
	{
	  time0 = time ((long *) 0);
	  et0 = 0;
	}
      if (et > 0)
	/* evrate used to be Nodes / cputime I dont` know why */
	evrate = NodeCnt / (et + ft);
      else
	evrate = 0;
      ETnodes = NodeCnt + 50;
    }
}

void
SetTimeControl (void)
{
  if (TCflag)
    {
      TimeControl.moves[white] = TimeControl.moves[black] = TCmoves;
      TimeControl.clock[white] = TimeControl.clock[black] = 60L * TCminutes;
    }
  else
    {
      TimeControl.moves[white] = TimeControl.moves[black] = 0;
      TimeControl.clock[white] = TimeControl.clock[black] = 0;
      Level = 60L * TCminutes;
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
UpdateDisplay (short int f, short int t, short int redraw, short int isspec)
{

  short r, c, l;
  if (redraw && !chesstool)
    {
      printz ("\n");
      for (r = 7; r >= 0; r--)
	{
	  for (c = 0; c <= 7; c++)
	    {
	      l = (flag.reverse) ? locn (7 - r, 7 - c) : locn (r, c);
	      if (color[l] == neutral)
		printz (" -");
	      else if (color[l] == white)
		printz (" %c", qxx[board[l]]);
	      else
		printz (" %c", pxx[board[l]]);
	    }
	  printz ("\n");
	}
      printz ("\n");
    }
}

char *InPtr;
void
skip ()
{
  while (*InPtr != ' ')
    InPtr++;
  while (*InPtr == ' ')
    InPtr++;
}
void
skipb ()
{
  while (*InPtr == ' ')
    InPtr++;
}
int
parser (char *f, int side)
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
  return (0);
}

void
GetGame (void)
{
  FILE *fd;
  char fname[256], *p;
  int c, i, j;
  short sq;
  unsigned short m;
  printz ("Enter file name: ");
  scanz ("%s", fname);
  if (fname[0] == '\0')
    strcpy (fname, "chess.000");
  if ((fd = fopen (fname, "r")) != NULL)
    {
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
      fgets (fname, 256, fd);
      InPtr = &fname[14];
      castld[white] = (*InPtr == 't') ? true : false;
      skip ();
      skip ();
      castld[black] = (*InPtr == 't') ? true : false;
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
      TimeControl.clock[white] = atoi (InPtr);
      skip ();
      skip ();
      TimeControl.moves[white] = atoi (InPtr);
      fgets (fname, 256, fd);
      InPtr = &fname[11];
      skipb ();
      TimeControl.clock[black] = atoi (InPtr);
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
      fgets (fname, 256, fd);
      fgets (fname, 256, fd);
      fgets (fname, 256, fd);
      while (fgets (fname, 256, fd))
	{
	  struct GameRec *g;
	  int side = computer;
	  side = otherside[side];
	  ++GameCnt;
	  InPtr = fname;
	  skipb ();
	  g = &GameList[GameCnt];
	  g->gmove = parser (InPtr, side);
	  skip ();
	  g->score = atoi (InPtr);
	  skip ();
	  g->depth = atoi (InPtr);
	  skip ();
	  g->nodes = atoi (InPtr);
	  skip ();
	  g->time = atoi (InPtr);
	  skip ();
	  g->flags = c = atoi (InPtr);
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
	      g->color = ((*InPtr == 'B') ? black : white);
	    }
	}
      /* GameCnt--;*/
      if (TimeControl.clock[white] > 0)
	TCflag = true;
      fclose (fd);
    }
  InitializeStats ();
  UpdateDisplay (0, 0, 1, 0);
  Sdepth = 0;
}

void
GetXGame (void)
{
  FILE *fd;
  char fname[256], *p;
  int c, i, j;
  short sq;
  unsigned short m;
  strcpy (fname, "xboard.position.read");
  if ((fd = fopen (fname, "r")) != NULL)
    {
      NewGame ();
      fgets (fname, 256, fd);
      fname[6] = '\0';
      if (strcmp (fname, "xboard"))
	return;
      fgets (fname, 256, fd);
      fgets (fname, 256, fd);
      for (i = 7; i > -1; i--)
	{
	  fgets (fname, 256, fd);
	  p = fname;
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
		}
	      p += 2;
	    }
	}
      fclose (fd);
    }
  ZeroRPT ();
  InitializeStats ();
  UpdateDisplay (0, 0, 1, 0);
  Sdepth = 0;
}

void
SaveGame (void)
{
  FILE *fd;
  char fname[256];
  short sq, i, c, f, t;
  char p;

  if (savefile[0])
    strcpy (fname, savefile);
  else
    {
      printz ("Enter file name: ");
      scanz ("%s", fname);
    }

  if (fname[0] == '\0')
    strcpy (fname, "chess.000");
  if ((fd = fopen (fname, "w")) != NULL)
    {
      char *b, *w;
      b = w = "Human";
      if (computer == black)
	b = "computer";
      if (computer == white)
	w = "computer";
      fprintf (fd, "Black %s White %s %d\n", b, w, Game50);
      fprintf (fd, "Castled White %s Black %s\n", castld[white] ? "true" : "false", castld[black] ? "true" : "false");
      fprintf (fd, "TimeControl %d Operator Time %d\n", TCflag, OperatorTime);
      fprintf (fd, "White Clock %ld Moves %ld\nBlack Clock %d Moves %d\n\n",
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
      fprintf (fd, "\nmove  score depth  nodes  time flags capture color\n");
      for (i = 1; i <= GameCnt; i++)
	{
	  struct GameRec *g = &GameList[i];
	  f = g->gmove >> 8;
	  t = (g->gmove & 0xFF);
	  algbr (f, t, g->flags);
	  fprintf (fd, "%s %5d %5d %7ld %5d %5d   %c   %s\n",
		   mvstr[0], g->score, g->depth,
		   g->nodes, g->time, g->flags,
	       pxx[g->piece], g->color == 2 ? "     " : ColorStr[g->color]);
	}
      fclose (fd);
      printz ("Game saved on file: %s\n", fname);
    }
  else
    printz ("Could not open file: %s", fname);
}

void
ListGame (void)
{
  FILE *fd;
  short i, f, t;
  char fname[256];
  if (listfile[0])
    strcpy (fname, listfile);
  else
    strcpy (fname, "chess.lst");
  fd = fopen (fname, "w");
  fprintf (fd, "gnuchess\n");
  fprintf (fd, "       score  depth   nodes  time         ");
  fprintf (fd, "       score  depth   nodes  time\n");
  for (i = 1; i <= GameCnt; i++)
    {
      f = GameList[i].gmove >> 8;
      t = (GameList[i].gmove & 0xFF);
      algbr (f, t, false);
      if ((i % 2) == 0)
	fprintf (fd, "         ");
      else
	fprintf (fd, "\n");
      fprintf (fd, "%5s  %5d     %2d %7ld %5d", mvstr[0],
	       GameList[i].score, GameList[i].depth,
	       GameList[i].nodes, GameList[i].time);
    }
  fprintf (fd, "\n\n");
  fclose (fd);
}

void
Undo (void)
/*
   Undo the most recent half-move.
*/

{
  short f, t;
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
  player = otherside[player];
  ShowSidetoMove();
  UpdateDisplay (0, 0, 1, 0);
  InitializeStats ();
}

void
ShowMessage (char *s)
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
ShowCurrentMove (short int pnt, short int f, short int t)
{
#ifdef MSDOS
  f++;
  t++;
  pnt++;				/* shut up the compiler */
#endif /* MSDOS */
}

void
ChangeAlphaWindow (void)
{
  printz ("window: ");
  scanz ("%hd", &Awindow);
}

void
ChangeBetaWindow (void)
{
  printz ("window: ");
  scanz ("%hd", &Bwindow);
}

void
SetAnalysis (void)
{
  printz ("game file: ");
  scanz ("%s", &Analysis);
}

void
GiveHint (void)
{
  algbr ((short) (hint >> 8), (short) (hint & 0xFF), false);
  fprintf (stderr, "Hint: %s\n", mvstr[0]);
}

void
SelectLevel (void)
{
  printz ("Enter #moves #minutes: ");
  scanz ("%hd %hd", &TCmoves, &TCminutes);
  printz ("Operator time= ");
  scanz ("%hd", &OperatorTime);
  TCflag = (TCmoves > 1);
  SetTimeControl ();
}

#ifdef DEBUG
void
ChangeDbLev (void)
{
  printz ("debuglevel= ");
  scanz ("%hd", &debuglevel);
}

#endif /*DEBUG*/

void
ChangeSearchDepth (void)
{
  printz ("depth= ");
  scanz ("%hd", &MaxSearchDepth);
}

ChangeHashDepth (void)
{
  printz ("hashdepth= ");
  scanz ("%hd", &HashDepth);
  printz ("MoveLimit= ");
  scanz ("%hd", &HashMoveLimit);
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
TestSpeed (void (*f) (short int side, short int ply))
{
  short i;
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


void
InputCommand (void)
/*
   Process the users command. If easy mode is OFF (the computer is thinking
   on opponents time) and the program is out of book, then make the 'hint'
   move on the board and call SelectMove() to find a response. The user
   terminates the search by entering ^C (quit siqnal) before entering a
   command. If the opponent does not make the hint move, then set Sdepth to
   zero.
*/

{
  int i;
  short ok, tmp;
  unsigned short mv;
  char s[80];

#ifdef DEBUG
  debugfd = fopen ("/tmp/DEBUG", "w");
#endif /*DEBUG*/
  ok = flag.quit = false;
  player = opponent;
  ft = 0;
  if (hint > 0 && !flag.easy && Book == NULL)
    {
      fflush (stdout);
      time0 = time ((long *) 0);
      algbr ((short) hint >> 8, (short) hint & 0xFF, false);
      strcpy (s, mvstr[0]);
      tmp = epsquare;
      if (flag.post)
	GiveHint ();
      if (VerifyMove (s, 1, &mv))
	{
	  ahead = 1;
	  SelectMove (computer, 2);
	  VerifyMove (mvstr[0], 2, &mv);
	  if (Sdepth > 0)
	    Sdepth--;
	}
      ft = time ((long *) 0) - time0;
      epsquare = tmp;
    }
  ahead = 0;
  while (!(ok || flag.quit))
    {
      PromptForMove ();
      i = scanz ("%s", s);
      if (i == EOF || s[0] == 0)
	ExitChess ();
      player = opponent;
      if (strcmp (s, "bd") == 0)
	{
#ifdef CHESSTOOL
	  chesstool = 0;
#endif /*CHESSTOOL*/
	  ClrScreen ();
	  UpdateDisplay (0, 0, 1, 0);
#ifdef CHESSTOOL
	  chesstool = 1;
#endif /*CHESSTOOL*/
	}
      else if (strcmp (s, "alg") == 0) /* noop */ ;
      else if ((strcmp (s, "quit") == 0) || (strcmp (s, "exit") == 0))
	flag.quit = true;
      else if (strcmp (s, "post") == 0)
	flag.post = !flag.post;
      else if ((strcmp (s, "set") == 0) || (strcmp (s, "edit") == 0))
	EditBoard ();
      else if (strcmp (s, "setup") == 0)
	SetupBoard ();
      else if (strcmp (s, "first") == 0)
	ok = true;
      else if (strcmp (s, "go") == 0)
	ok = true;
      else if (strcmp (s, "help") == 0)
	help ();
      else if (strcmp (s, "force") == 0)
	flag.force = !flag.force;
      else if (strcmp (s, "book") == 0)
	Book = NULL;
      else if (strcmp (s, "new") == 0)
	NewGame ();
      else if (strcmp (s, "list") == 0)
	ListGame ();
      else if (strcmp (s, "level") == 0 || strcmp (s, "clock") == 0)
	SelectLevel ();
      else if (strcmp (s, "hash") == 0)
	flag.hash = !flag.hash;
      else if (strcmp (s, "beep") == 0)
	flag.beep = !flag.beep;
      else if (strcmp (s, "Awindow") == 0)
	ChangeAlphaWindow ();
      else if (strcmp (s, "Bwindow") == 0)
	ChangeBetaWindow ();
      else if (strcmp (s, "rcptr") == 0)
	flag.rcptr = !flag.rcptr;
      else if (strcmp (s, "hint") == 0)
	GiveHint ();
      else if (strcmp (s, "both") == 0)
	{
	  flag.bothsides = !flag.bothsides;
	  Sdepth = 0;
	  ElapsedTime (1);
	  SelectMove (opponent, 1);
	  ok = true;
	}
      else if (strcmp (s, "reverse") == 0)
	{
	  flag.reverse = !flag.reverse;
	  ClrScreen ();
	  UpdateDisplay (0, 0, 1, 0);
	}
      else if (strcmp (s, "switch") == 0)
	{
	  computer = otherside[computer];
	  opponent = otherside[opponent];
	  flag.force = false;
	  Sdepth = 0;
	  ok = true;
	}
      else if (strcmp (s, "white") == 0)
	{
	  computer = white;
	  opponent = black;
	  flag.force = false;
	  Sdepth = 0;
	  /* ok = true;/* don't automatically start with white command */
	}
      else if (strcmp (s, "black") == 0)
	{
	  computer = black;
	  opponent = white;
	  flag.force = false;
	  Sdepth = 0;
	  /* ok = true;/* don't automatically start with black command */
	}
      else if (strcmp (s, "undo") == 0 && GameCnt > 0)
	Undo ();
      else if (strcmp (s, "remove") == 0 && GameCnt > 1)
	{
	  Undo ();
	  Undo ();
	}
      else if (strcmp (s, "get") == 0)
	GetGame ();
      else if (strcmp (s, "xget") == 0)
	GetXGame ();
      else if (strcmp (s, "save") == 0)
	SaveGame ();
      else if (strcmp (s, "depth") == 0)
	ChangeSearchDepth ();
#ifdef DEBUG
      else if (strcmp (s, "debuglevel") == 0)
	ChangeDbLev ();
#endif /*DEBUG*/
      else if (strcmp (s, "hashdepth") == 0)
	ChangeHashDepth ();
      else if (strcmp (s, "random") == 0)
	dither = 6;
      else if (strcmp (s, "easy") == 0)
	flag.easy = !flag.easy;
      else if (strcmp (s, "contempt") == 0)
	SetContempt ();
      else if (strcmp (s, "xwndw") == 0)
	ChangeXwindow ();
      else if (strcmp (s, "test") == 0)
	{
	  ShowMessage ("Testing MoveList Speed");
	  TestSpeed (MoveList);
	  ShowMessage ("Testing CaptureList Speed");
	  TestSpeed (CaptureList);
	}
      else
	{
	  ok = VerifyMove (s, 0, &mv);
	  if (ok && mv != hint)
	    {
	      Sdepth = 0;
	      ft = 0;
	    }
	}
    }

  ElapsedTime (1);
  if (flag.force)
    {
      computer = opponent;
      opponent = otherside[computer];
    }
#ifdef CHESSTOOL
  printf ("%d. %s", ++mycnt2, s);
  if (flag.post)
    {
      register int i;
      printf (" %6d ", MSCORE);
      for (i = 1; MV[i] > 0; i++)
	{
	  algbr ((short) (MV[i] >> 8), (short) (MV[i] & 0xFF), false);
	  printf ("%5s ", mvstr[0]);
	}
    }
  printf ("\n");
#endif /* CHESSTOOL */
  signal (SIGINT, TerminateSearch);
#ifndef MSDOS
  signal (SIGQUIT, TerminateSearch);
#endif /* MSDOS */
}
