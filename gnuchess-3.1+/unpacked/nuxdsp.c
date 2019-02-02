/* nuxdsp.c - (new)  ALPHA interface for CHESS

  Revision: 1991-04-15

  Copyright (C) 1986, 1987, 1988, 1989, 1990 Free Software Foundation, Inc.
  Copyright (c) 1988, 1989, 1990  John Stanback

  Modified extensively Nov 1989 Christopher North-Keys
    40x24 two-colour display
	option for shading black squares
	expanded game save, list, and restore features using $HOME
	option to disable display of coordinates
	optional auto-updating of positional information
	optional starring of black side
	mass toggle for reverse-video functions

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

#define ESC 0x1B
#define scanz scanf
#define printz printf
#define refresh() fflush(stdout)

int mycntl1, mycntl2;
static void param (short n);
#else
#include <sys/param.h>
#include <sys/types.h>
#include <sys/file.h>
#include <curses.h>

#define scanz fflush(stdout),scanw
#define printz printw

/* <stdlib.h> */
extern void *malloc (size_t);
extern void exit (int);
/* <string.h> */
extern char *strcat (char *, const char *);
extern int strcmp (const char *, const char *);
extern char *strcpy (char *, const char *);
/* <time.h> */
extern long int time (long int *);
#endif /* MSDOS */

#include "gnuchess.h"

#define pxx " PNBRQK"
#define qxx " pnbrqk"
#define rxx "12345678"
#define cxx "abcdefgh"
#define TAB (43)
/* coordinates within a square for the following are ([1,5],[1,3]) */
#define SQW (5)
#define SQH (3)
#define VIR_C(s)  ((flag.reverse) ? 7-column(s) : column(s))
#define VIR_R(s)  ((flag.reverse) ? 7-row(s) : row(s))
#define VSQ_X(x)  ((flag.reverse) ? SQW + 1 - (x) : (x))
#define VSQ_Y(y)  ((flag.reverse) ? SQH + 1 - (y) : (y))
#define Vblack(s) (!((VIR_C(s) + VIR_R(s)) % 2))
/* Squares swapped */
#define Vcoord(s,x,y) \
	((SQW)*(VIR_C(s)))+(x),((SQH)*(7-VIR_R(s))+(y))
/* Squares and internal locations swapped */
#define VcoordI(s,x,y) \
	((SQW)*(VIR_C(s)))+(VSQ_X(x)),((SQH)*(7-VIR_R(s))+(VSQ_Y(y)))
/* Squares and internal rows swapped */
#define VcoordR(s,x,y) \
	((SQW)*(VIR_C(s)))+(x),((SQH)*(7-VIR_R(s))+(VSQ_Y(y)))
char Analysis[128] = "";
short int MV[maxdepth];
int MSCORE;
char *DRAW;
char mvstr[4][6];
static char *ColorStr[2] =
{"White", "Black"};
static long evrate;
short PositionFlag = 0;
short coords = 1;
#if defined(MSDOS) && !defined(SEVENBIT)
short rv = 0;
static void ONormal (void);
static void OReverse (void);
#else
short stars = 0;
short rv = 1;
short shade = 0;
#endif /* MSDOS && !SEVENBIT */

extern char *getenv (const char *);
void TerminateSearch (int), Die (int);

void
Initialize (void)
{
  signal (SIGINT, Die);
#ifndef MSDOS
  signal (SIGQUIT, Die);
  initscr ();
  crmode ();
#else
  mycntl1 = mycntl2 = 0;
#endif /* MSDOS */
}

void
ExitChess (void)
{
  ListGame ();
  gotoXY (1, 24);
#ifndef MSDOS
  nocrmode ();
  endwin ();
#endif /* MSDOS */
  exit (0);
}

void
Die (int Sig)
{
  char s[80];

  signal (SIGINT, SIG_IGN);
#ifdef MSDOS
  Sig++;			       /* shut up the compiler */
#else
  signal (SIGQUIT, SIG_IGN);
#endif /* MSDOS */
  ShowMessage ("Abort? ");
  scanz ("%s", s);
  if (strcmp (s, "yes") == 0)
    ExitChess ();
  signal (SIGINT, Die);
#ifndef MSDOS
  signal (SIGQUIT, Die);
#endif /* MSDOS */
}

void
TerminateSearch (int Sig)
{
  signal (SIGINT, SIG_IGN);
#ifdef MSDOS
  Sig++;			       /* shut up the compiler */
#else
  signal (SIGQUIT, SIG_IGN);
#endif /* MSDOS */
  flag.timeout = true;
  flag.bothsides = false;
  signal (SIGINT, Die);
#ifndef MSDOS
  signal (SIGQUIT, Die);
#endif /* MSDOS */
}
void 
ShowLine (short unsigned int *bstline)
{
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
	  ShowMessage ("Illegal Move!!");
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
  if (cnt > 1)
    ShowMessage ("Ambiguous Move!");
  return (false);
}

void
help (void)
{
  ClrScreen ();
  printz ("CHESS command summary\n");
  printz ("g1f3     move from g1 to f3      quit      Exit Chess\n");
  printz ("Nf3      move knight to f3       beep      on/off\n");
  printz ("o-o      castle king side        easy      on/off\n");
  printz ("o-o-o    castle queen side       hash      on/off\n");
  printz ("bd       redraw board            reverse   board display\n");
  printz ("list     game to chess.lst       book      on/off\n");
  printz ("undo     undo last ply           remove    take back a move\n");
  printz ("edit     edit board              force     enter game moves\n");
  printz ("switch   sides with computer     both      computer match\n");
  printz ("white    computer plays white    black     computer plays black\n");
  printz ("depth    set search depth        level     select level\n");
  printz ("post     principle variation     hint      suggest a move\n");
  printz ("save     game to file            get/xget  game from file\n");
  printz ("random   randomize play          new       start new game\n");
  printz ("rv       toggle reverse video    coords    toggle coords\n");
#if !defined(MSDOS) || defined(SEVENBIT)
  printz ("shade    toggle shade black      stars     toggle stars\n");
#endif /* !MSDOS || SEVENBIT */
  printz ("p        show coordinate values\n");
  gotoXY (10, 21);
  printz ("Computer: %s", ColorStr[computer]);
  gotoXY (10, 22);
  printz ("Opponent: %s", ColorStr[opponent]);
  gotoXY (10, 23);
  printz ("Level: %ld", Level);
  gotoXY (10, 24);
  printz ("Easy mode: %s", (flag.easy) ? "ON" : "OFF");
  gotoXY (40, 21);
  printz ("Depth: %d", MaxSearchDepth);
  gotoXY (40, 22);
  printz ("Random: %s", (dither) ? "ON" : "OFF");
  gotoXY (40, 23);
  printz ("Transposition table: %s", (flag.hash) ? "ON" : "OFF");
  gotoXY (40, 24);
  printz ("Hit <RET> to return: ");
  refresh ();
  fflush (stdin);
  getchar ();
  ClrScreen ();
  UpdateDisplay (0, 0, 1, 0);
}

void
EditBoard (void)

/*
  Set up a board position. Pieces are entered by typing the piece
  followed by the location. For example, Nf3 will place a knight on
  square f3.
*/

{
  short a, r, c, sq, i;
  char s[80];

  ClrScreen ();
  UpdateDisplay (0, 0, 1, 0);
  gotoXY (TAB, 3);
  printz (".   Exit to main");
  gotoXY (TAB, 4);
  printz ("#   Clear board");
  gotoXY (TAB, 5);
  printz ("c   Change sides");
  gotoXY (TAB, 7);
  printz ("Enter piece & location: ");
  a = white;
  do
    {
      gotoXY (TAB, 6);
      printz ("Editing: %s", ColorStr[a]);
      gotoXY (TAB + 24, 7);
      ClrEoln ();
      scanz ("%s", s);
      if (s[0] == '#')
	{
	  for (sq = 0; sq < 64; sq++)
	    {
	      board[sq] = no_piece;
	      color[sq] = neutral;
	      DrawPiece (sq);
	    }
	}
      if (s[0] == 'c' || s[0] == 'C')
	a = otherside[a];
      c = s[1] - 'a';
      r = s[2] - '1';
      if ((c >= 0) && (c < 8) && (r >= 0) && (r < 8))
	{
	  sq = locn (r, c);
	  for (i = king; i > no_piece; i--)
	    if ((s[0] == pxx[i]) || (s[0] == qxx[i]))
	      break;
	  board[sq] = i;
	  color[sq] = (board[sq] == no_piece) ? neutral : a;
	  DrawPiece (sq);
	}
  } while (s[0] != '.');

  for (sq = 0; sq < 64; sq++)
    Mvboard[sq] = (board[sq] != Stboard[sq]) ? 10 : 0;
  GameCnt = 0;
  Game50 = 1;
  ZeroRPT ();
  Sdepth = 0;
  InitializeStats ();
  ClrScreen ();
  UpdateDisplay (0, 0, 1, 0);
}

void
ShowPlayers (void)
{
  gotoXY (TAB, ((flag.reverse) ? 23 : 2));
  printz ("%s", (computer == black) ? "Computer" : "Human   ");
  gotoXY (TAB, ((flag.reverse) ? 2 : 23));
  printz ("%s", (computer == white) ? "Computer" : "Human   ");
}

void
ShowDepth (char ch)
{
  gotoXY (TAB, 4);
  printz ("Depth= %d%c ", Sdepth, ch);
  ClrEoln ();
}

void
ShowScore (short score)
{
  gotoXY (TAB, 5);
  printz ("Score= %d", score);
  ClrEoln ();
}

void
ShowMessage (char *s)
{
  gotoXY (TAB, 6);
  printz ("%s", s);
  ClrEoln ();
}

void
ClearMessage (void)
{
  gotoXY (TAB, 6);
  ClrEoln ();
}

void
ShowCurrentMove (short int pnt, short int f, short int t)
{
  algbr (f, t, false);
  gotoXY (TAB, 7);
  printz ("(%2d) %4s", pnt, mvstr[0]);
}

void
ShowHeader (void)
{
  gotoXY (TAB, 10);
#ifdef MSDOS
  printz ("GNU Chess display (MS-DOS, Mar 90)");
#else
  printz ("GNU Chess display (Nov 89)");
#endif /* MSDOS */
}

void
ShowSidetoMove (void)
{
  gotoXY (TAB, 14);
  printz ("%2d:   %s", 1 + GameCnt / 2, ColorStr[player]);
  ClrEoln ();
}

void
ShowPrompt (void)
{
  gotoXY (TAB, 19);
  printz ("Your move is? ");
  ClrEoln ();
}

void
ShowNodeCnt (long int NodeCnt, long int evrate)
{
  gotoXY (TAB, 21);
  printz ("Nodes= %8ld, Nodes/Sec= %5ld", NodeCnt, evrate);
  ClrEoln ();
}

void
ShowResults (short int score, short unsigned int *bstline, char ch)
{
  unsigned char d, ply;
  if (flag.post)
    {
      ShowDepth (ch);
      ShowScore (score);
      d = 7;
      for (ply = 1; bstline[ply] > 0; ply++)
	{
	  if (ply % 4 == 1)
	    {
	      gotoXY (TAB, ++d);
	      ClrEoln ();
	    }
	  algbr ((short) bstline[ply] >> 8, (short) bstline[ply] & 0xFF, false);
	  printz ("%5s ", mvstr[0]);
	}
      ClrEoln ();
      while (d < 13)
	{
	  gotoXY (TAB, ++d);
	  ClrEoln ();
	}
    }
}

void
SearchStartStuff (short int side)
{
  short i;

  signal (SIGINT, TerminateSearch);
#ifdef MSDOS
  side++;			       /* shut up the compiler */
#else
  signal (SIGQUIT, TerminateSearch);
#endif /* MSDOS */
  for (i = 4; i < 14; i++)
    {
      gotoXY (TAB, i);
      ClrEoln ();
    }
}

void
OutputMove (void)
{
  int i;

  UpdateDisplay (root->f, root->t, 0, (short) root->flags);
  gotoXY (TAB, 17);
  printz ("My move is: %s", mvstr[0]);
  if (flag.beep)
    putchar (7);
  ClrEoln ();

  gotoXY (TAB, 24);
  if (root->flags & draw)
    printz ("Drawn game! %s", DRAW);
  else if (root->score == -9999)
    printz ("Opponent mates!");
  else if (root->score == 9998)
    printz ("Computer mates!");
  else if (root->score < -9000)
    printz ("Opponent will soon mate!");
  else if (root->score > 9000)
    printz ("Computer will soon mate!");
  ClrEoln ();

  if (flag.post)
    {
      ShowNodeCnt (NodeCnt, evrate);
      gotoXY (TAB, 22);
      for (i = 1999; i >= 0 && Tree[i].f == 0 && Tree[i].t == 0; i--) ;
      printz ("Max Tree= %5d", i);
      ClrEoln ();
    }
}

void
ElapsedTime (short int iop)

/*
  Determine the time that has passed since the search was started. If
  the elapsed time exceeds the target (ResponseTime+ExtraTime) then set
  timeout to true which will terminate the search.
*/

{
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
      UpdateClocks ();
    }
}

void
UpdateClocks (void)
{
  short m, s;
  m = (short) (et / 60);
  s = (short) (et - 60 * (long) m);
  if (TCflag)
    {
      m = (short) ((TimeControl.clock[player] - et) / 60);
      s = (short) (TimeControl.clock[player] - et - 60 * (long) m);
    }
  if (m < 0)
    m = 0;
  if (s < 0)
    s = 0;
  if (player == white)
    gotoXY (60, (flag.reverse) ? 2 : 23);
  else
    gotoXY (60, (flag.reverse) ? 23 : 2);
  printz ("%d:%2d   ", m, s);
  if (flag.post)
    ShowNodeCnt (NodeCnt, evrate);
  refresh ();
}


void
SetTimeControl (void)
{
  if (TCflag)
    {
      TimeControl.moves[white] = TimeControl.moves[black] = TCmoves;
      TimeControl.clock[white] = TimeControl.clock[black] = 60 * (long) TCminutes;
    }
  else
    {
      TimeControl.moves[white] = TimeControl.moves[black] = 0;
      TimeControl.clock[white] = TimeControl.clock[black] = 0;
      Level = 60 * (long) TCminutes;
    }
  et = 0;
  ElapsedTime (1);
}

void
gotoXY (short int x, short int y)
{
#ifdef MSDOS
  putchar (ESC);
  putchar ('[');
  param (y);
  putchar (';');
  param (x);
  putchar ('H');
#else
  move (y - 1, x - 1);
#endif /* MSDOS */
}

void
ClrScreen (void)
{
#ifdef MSDOS
  putchar (ESC);
  putchar ('[');
  putchar ('2');
  putchar ('J');
#else
  clear ();
#endif /* MSDOS */
  refresh ();
}

void
ClrEoln (void)
{
#ifdef MSDOS
  putchar (ESC);
  putchar ('[');
  putchar ('K');
#else
  clrtoeol ();
#endif /* MSDOS */
  refresh ();
}

#ifdef MSDOS
void
param (short n)
{
  if (n >= 10)
    {
      register short d, q;
      q = n / 10;
      d = n % 10;
      putchar (q + '0');
      putchar (d + '0');
    }
  else
    putchar (n + '0');
}

#endif /* MSDOS */

void
OReverse ()
{
#ifdef MSDOS
  putchar (ESC);
  putchar ('[');
  param (7);
  putchar ('m');
#else
  standout ();
  /* attron (A_REVERSE); */
#endif /* MSDOS */
}

void
ONormal ()
{
#ifdef MSDOS
  putchar (ESC);
  putchar ('[');
  param (0);
  putchar ('m');
#else
  standend ();
  /* attroff (A_REVERSE);*/
#endif /* MSDOS */
}

void
DrawPiece (short int sq)
{
  gotoXY (VcoordR (sq, 2, 2));

  switch (color[sq])
    {
    case black:
      if (rv)
	OReverse ();
#if defined(MSDOS) && !defined(SEVENBIT)
      printz (" %c ", pxx[board[sq]]);
#else
      printz ((stars ? "*%c*" : " %c "), pxx[board[sq]]);
#endif /* MSDOS && !SEVENBIT */
      ONormal ();
      break;
    case neutral:
#if defined(MSDOS) && !defined(SEVENBIT)
      if (rv)
	printz (Vblack (sq) ? "\262\262\262" : "\260\260\260");
      else
	printz (Vblack (sq) ? "\260\260\260" : "\262\262\262");
#else
      if (shade)
	printz (Vblack (sq) ? "///" : "   ");
      else
	{
	  if (Vblack (sq))
	    OReverse ();
	  printz ("   ");
	  ONormal ();
	}
#endif /* MSDOS && !SEVENBIT */
      break;
    case white:
#if defined(MSDOS) && !defined(SEVENBIT)
      if (!rv)
	OReverse ();
      printz (" %c ", pxx[board[sq]]);
      ONormal ();
#else
      printz (" %c ", pxx[board[sq]]);
#endif /* MSDOS && !SEVENBIT */
      break;
    default:
      ShowMessage ("DrawPiece:  color[sq] err");
      break;
    }
}

void
DrawSquare (short int sq)
{
#if defined(MSDOS) && !defined(SEVENBIT)
  if (rv)
    {
      gotoXY (Vcoord (sq, 1, 1));
      printz (Vblack (sq) ? "\262\262\262\262\262" : "\260\260\260\260\260");
      gotoXY (Vcoord (sq, 1, 2));
      printz (Vblack (sq) ? "\262\262\262\262\262" : "\260\260\260\260\260");
      gotoXY (Vcoord (sq, 1, 3));
      printz (Vblack (sq) ? "\262\262\262\262\262" : "\260\260\260\260\260");
    }
  else
    {
      gotoXY (Vcoord (sq, 1, 1));
      printz (Vblack (sq) ? "\260\260\260\260\260" : "\262\262\262\262\262");
      gotoXY (Vcoord (sq, 1, 2));
      printz (Vblack (sq) ? "\260\260\260\260\260" : "\262\262\262\262\262");
      gotoXY (Vcoord (sq, 1, 3));
      printz (Vblack (sq) ? "\260\260\260\260\260" : "\262\262\262\262\262");
    }
#else
  if (shade)
    {
      gotoXY (Vcoord (sq, 1, 1));
      printz (Vblack (sq) ? "/////" : "     ");
      gotoXY (Vcoord (sq, 1, 2));
      printz (Vblack (sq) ? "/////" : "     ");
      gotoXY (Vcoord (sq, 1, 3));
      printz (Vblack (sq) ? "/////" : "     ");
    }
  else
    {
      if (Vblack (sq))
	OReverse ();
      gotoXY (Vcoord (sq, 1, 1));
      printz ("     ");
      gotoXY (Vcoord (sq, 1, 2));
      printz ("     ");
      gotoXY (Vcoord (sq, 1, 3));
      printz ("     ");
      ONormal ();
    }
#endif /* MSDOS && !SEVENBIT */
}

void
DrawCoords (void)
{
  short z;

  for (z = 0; z <= 7; z++)
    {
      short sq;

      sq = z << 3;
      gotoXY (VcoordI (sq, 1, 1));
#if !defined(MSDOS) || defined(SEVENBIT)
      if ((Vblack (sq) || shade) && rv)
#endif /* !MSDOS || SEVENBIT */
	OReverse ();
      printz ("%d", 1 + z);
      ONormal ();
    }

  for (z = 0; z <= 7; z++)
    {
      short sq;

      sq = z;
      gotoXY (VcoordI (sq, SQW, SQH));
#if !defined(MSDOS) || defined(SEVENBIT)
      if ((Vblack (sq) || shade) && rv)
#endif /* !MSDOS || SEVENBIT */
	OReverse ();
      printz ("%c", cxx[z]);
      ONormal ();
    }

#if !defined(MSDOS) || defined(SEVENBIT)
  for (z = 1; z <= (8 * SQH); z++)
    {
      gotoXY ((8 * SQW) + 1, z);
      printz ("|");
    }
#endif /* MSDOS && !SEVENBIT */
}

void
ShowPostnValue (short int sq)

/*
  must have called ExaminePosition() first
*/

{
  short score;

  gotoXY (VcoordR (sq, 2, 1));
  ScorePosition (color[sq], &score);
#if !defined(MSDOS) || defined(SEVENBIT)
  if (Vblack (sq) && !shade)
    OReverse ();
#endif /* !MSDOS || SEVENBIT */

  if (color[sq] != neutral)
    printz ("%3d", svalue[sq]);
  else
#if defined(MSDOS) && !defined(SEVENBIT)
    {
      if (rv)
	printz (Vblack (sq) ? "\262\262\262" : "\260\260\260");
      else
	printz (Vblack (sq) ? "\260\260\260" : "\262\262\262");
    }
#else
    printz (shade && Vblack (sq) ? "///" : "   ");
#endif /* MSDOS && !SEVENBIT */
  ONormal ();
}

void
ShowPostnValues (void)
{
  short sq, score;

  ExaminePosition ();
  for (sq = 0; sq < 64; sq++)
    ShowPostnValue (sq);
  ScorePosition (opponent, &score);
  ShowScore (score);
}

void
UpdateDisplay (short int f, short int t, short int redraw, short int isspec)
{
  short sq;

  if (redraw)
    {
      ShowHeader ();
      ShowPlayers ();
      for (sq = 0; sq < 64; sq++)
	{
	  DrawSquare (sq);
	  DrawPiece (sq);
	}
      if (coords)
	DrawCoords ();
    }
  else
    {
      DrawPiece (f);
      DrawPiece (t);
      if (isspec & cstlmask)
	if (t > f)
	  {
	    DrawPiece (f + 3);
	    DrawPiece (t - 1);
	  }
	else
	  {
	    DrawPiece (f - 4);
	    DrawPiece (t + 1);
	  }
      else if (isspec & epmask)
	{
	  DrawPiece (t - 8);
	  DrawPiece (t + 8);
	}
    }
  if (PositionFlag)
    ShowPostnValues ();
  refresh ();
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

  fname[0] = '\0';
  ShowMessage ("File name: ");
  scanz ("%s", fname);
  if (!fname[0])
    strcpy (fname, "chess.000");
  ShowMessage ("Loading ");
  printz ("%s", fname);

  if ((fd = fopen (fname, "r")) == NULL)
    {
      ShowMessage ("Load failed");
      return;
    }


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
  ZeroRPT ();
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
  fname[0] = 0;
  ShowMessage ("File name: ");
  scanz ("%s", fname);
  if (!fname[0])
    strcpy (fname, "xboard.position.read");
  ShowMessage ("Loading ");
  printz ("%s", fname);
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
  char fname[256], tname[256];
  short sq, i, c, f, t;
  char p, *tmp;
  char *b, *w;

  tname[0] = 0;

  if (tmp = getenv ("HOME"))
    strcpy (fname, tmp);
  else
    fname[0] = '\0';
  strcat (fname, "/");

  ShowMessage ("File name: ");
  refresh ();
  scanz ("%s", tname);

  if (tname[0])
    strcat (fname, tname);
  else
    strcat (fname, "chess.000");

  ShowMessage ("Saving ");
  printz ("%s", fname);

  if (NULL == (fd = fopen (fname, "w")))
    {
      ShowMessage ("Not saved");
      return;
    }
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
  printz ("Game saved on file: %s", fname);
}

void
ListGame (void)
{
  FILE *fd;
  char fname[256];
  char *tmp;
  short i, f, t;

  if (tmp = getenv ("HOME"))
    strcpy (fname, tmp);
  else
    fname[0] = '\0';
  strcat (fname, "/chess.lst");

  if (fd = fopen (fname, "w"))
    ShowMessage ("Writing ~/chess.lst");
  else
    {
      ShowMessage ("Cannot write ~/chess.lst");
      return;
    }

  fprintf (fd, "gnuchess\n");
  fprintf (fd, "       score  depth   nodes  time         ");
  fprintf (fd, "       score  depth   nodes  time\n");
  for (i = 1; i <= GameCnt; i++)
    {
      f = GameList[i].gmove >> 8;
      t = (GameList[i].gmove & 0xFF);
      algbr (f, t, false);
      if ((i % 2) == 0)
	fprintf (fd, "\n");
      else
	fprintf (fd, "         ");
      fprintf (fd, "%5s  %5d     %2d %7ld %5d", mvstr[0],
	       GameList[i].score, GameList[i].depth,
	       GameList[i].nodes, GameList[i].time);
    }
  fprintf (fd, "\n\n");
  fclose (fd);
  ShowMessage ("~/chess.lst written");
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
  ShowSidetoMove ();
  UpdateDisplay (0, 0, 1, 0);
  InitializeStats ();
}

void
ChangeAlphaWindow (void)
{
  ShowMessage ("Awindow= ");
  scanz ("%hd", &Awindow);
}

void
ChangeBetaWindow (void)
{
  ShowMessage ("Bwindow= ");
  scanz ("%hd", &Bwindow);
}

void
GiveHint (void)
{
  char s[40];
  algbr ((short) (hint >> 8), (short) (hint & 0xFF), false);
  strcpy (s, "try ");
  strcat (s, mvstr[0]);
  ShowMessage (s);
}

void
ChangeHashDepth (void)
{
  ShowMessage ("hashdepth= ");
  scanz ("%hd", &HashDepth);
  ShowMessage ("MoveLimit= ");
  scanz ("%hd", &HashMoveLimit);
}

void
ChangeSearchDepth (void)
{
  ShowMessage ("depth= ");
  scanz ("%hd", &MaxSearchDepth);
}

void
SetContempt (void)
{
  ShowMessage ("contempt= ");
  scanz ("%hd", &contempt);
}


void
SetAnalysis (void)
{
  ShowMessage ("game file= ");
  scanz ("%s", &Analysis);
}

void
ChangeXwindow (void)
{
  ShowMessage ("xwndw= ");
  scanz ("%hd", &xwndw);
}

void
SelectLevel (void)
{
  ClrScreen ();
  gotoXY (32, 2);
  printz ("CHESS");
  gotoXY (20, 4);
  printz (" 1.   60 moves in   5 minutes");
  gotoXY (20, 5);
  printz (" 2.   60 moves in  15 minutes");
  gotoXY (20, 6);
  printz (" 3.   60 moves in  30 minutes");
  gotoXY (20, 7);
  printz (" 4.   40 moves in  30 minutes");
  gotoXY (20, 8);
  printz (" 5.   40 moves in  60 minutes");
  gotoXY (20, 9);
  printz (" 6.   40 moves in 120 minutes");
  gotoXY (20, 10);
  printz (" 7.   40 moves in 240 minutes");
  gotoXY (20, 11);
  printz (" 8.    1 move  in  15 minutes");
  gotoXY (20, 12);
  printz (" 9.    1 move  in  60 minutes");
  gotoXY (20, 13);
  printz ("10.    1 move  in 600 minutes");

  OperatorTime = 1;
  TCmoves = 60;
  TCminutes = 5;

  gotoXY (20, 17);
  printz ("Enter Level: ");
  refresh ();
  scanz ("%ld", &Level);
  switch ((int) Level)
    {
    case 1:
      TCmoves = 60;
      TCminutes = 5;
      break;
    case 2:
      TCmoves = 60;
      TCminutes = 15;
      break;
    case 3:
      TCmoves = 60;
      TCminutes = 30;
      break;
    case 4:
      TCmoves = 40;
      TCminutes = 30;
      break;
    case 5:
      TCmoves = 40;
      TCminutes = 60;
      break;
    case 6:
      TCmoves = 40;
      TCminutes = 120;
      break;
    case 7:
      TCmoves = 40;
      TCminutes = 240;
      break;
    case 8:
      TCmoves = 1;
      TCminutes = 15;
      break;
    case 9:
      TCmoves = 1;
      TCminutes = 60;
      break;
    case 10:
      TCmoves = 1;
      TCminutes = 600;
      break;
    }

  TCflag = (TCmoves > 1);
  SetTimeControl ();
  ClrScreen ();
  UpdateDisplay (0, 0, 1, 0);
}

void
DoDebug (void)
{
  short c, p, sq, tp, tc, tsq, score;
  char s[40];

  ExaminePosition ();
  ShowMessage ("Enter piece: ");
  scanz ("%s", s);
  c = neutral;
  if (s[0] == 'w' || s[0] == 'W')
    c = white;
  if (s[0] == 'b' || s[0] == 'B')
    c = black;
  for (p = king; p > no_piece; p--)
    if ((s[1] == pxx[p]) || (s[1] == qxx[p]))
      break;
  for (sq = 0; sq < 64; sq++)
    {
      tp = board[sq];
      tc = color[sq];
      board[sq] = p;
      color[sq] = c;
      tsq = PieceList[c][1];
      PieceList[c][1] = sq;
      ShowPostnValue (sq);
      PieceList[c][1] = tsq;
      board[sq] = tp;
      color[sq] = tc;
    }
  ScorePosition (opponent, &score);
  ShowScore (score);
}

void
TestSpeed (void (*f) (short int side, short int ply))
{
  short i;
  long t1, t2;

  t1 = time (0);
  for (i = 0; i < 10000; i++)
    {
      f (opponent, 2);
    }
  t2 = time (0);
  NodeCnt = 10000L * (TrPnt[3] - TrPnt[2]);
  evrate = NodeCnt / (t2 - t1);
  ShowNodeCnt (NodeCnt, evrate);
}

char s[80];
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
  short ok, tmp;
  unsigned short mv;

  ok = flag.quit = false;
  player = opponent;
  ShowSidetoMove ();
  ft = 0;
  if (hint > 0 && !flag.easy && Book == NULL)
    {
      fflush (stdout);
      time0 = time ((long *) 0);
      algbr ((short) hint >> 8, (short) hint & 0xFF, false);
      strcpy (s, mvstr[0]);
      tmp = epsquare;
      if (VerifyMove (s, 1, &mv))
	{
	  ShowPrompt ();
	  SelectMove (computer, 2);
	  (void) VerifyMove (mvstr[0], 2, &mv);
	  if (Sdepth > 0)
	    Sdepth--;
	}
      ft = time ((long *) 0) - time0;
      epsquare = tmp;
    }
  signal (SIGINT, Die);
#ifndef MSDOS
  signal (SIGQUIT, Die);
#endif /* MSDOS */
  while (!(ok || flag.quit))
    {
      ShowPrompt ();
      scanz ("%s", s);
      player = opponent;
      ok = VerifyMove (s, 0, &mv);
      if (ok && mv != hint)
	{
	  Sdepth = 0;
	  ft = 0;
	}
      if (*s == '\0')
	UpdateDisplay (0, 0, 1, 0);
      else if (strcmp (s, "bd") == 0)
	{
	  ClrScreen ();
	  UpdateDisplay (0, 0, 1, 0);
	}
      else if ((strcmp (s, "quit") == 0) || (strcmp (s, "exit") == 0))
	flag.quit = true;
      else if (strcmp (s, "post") == 0)
	flag.post = !flag.post;
      else if (strcmp (s, "edit") == 0)
	EditBoard ();
      else if (strcmp (s, "go") == 0)
	ok = true;
      else if (strcmp (s, "help") == 0)
	help ();
      else if (strcmp (s, "force") == 0)
	flag.force = !flag.force;
      else if (strcmp (s, "book") == 0)
	Book = NULL;
      else if (strcmp (s, "undo") == 0 && GameCnt > 0)
	Undo ();
      else if (strcmp (s, "new") == 0)
	NewGame ();
      else if (strcmp (s, "list") == 0)
	ListGame ();
      else if (strcmp (s, "level") == 0)
	SelectLevel ();
      else if (strcmp (s, "hash") == 0)
	flag.hash = !flag.hash;
      else if (strcmp (s, "beep") == 0)
	flag.beep = !flag.beep;
      else if (strcmp (s, "Awindow") == 0)
	ChangeAlphaWindow ();
      else if (strcmp (s, "Bwindow") == 0)
	ChangeBetaWindow ();
      else if (strcmp (s, "hint") == 0)
	GiveHint ();
      else if (strcmp (s, "both") == 0)
	{
	  flag.bothsides = !flag.bothsides;
	  Sdepth = 0;
	  SelectMove (opponent, 1);
	  ok = true;
	}
      else if (strcmp (s, "reverse") == 0)
	{
	  flag.reverse = !flag.reverse;
	  ClrScreen ();
	  UpdateDisplay (0, 0, 1, 0);
	}
#if !defined(MSDOS) || defined(SEVENBIT)
      else if (strcmp (s, "shade") == 0)
	{
	  shade = !shade;
	  ClrScreen ();
	  UpdateDisplay (0, 0, 1, 0);
	}
#endif /* MSDOS && !SEVENBIT */
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
	  /*ok = true;/* don't start automatically on white command */
	}
      else if (strcmp (s, "black") == 0)
	{
	  computer = black;
	  opponent = white;
	  flag.force = false;
	  Sdepth = 0;
	  /*ok = true;/* don't start automatically on black command */
	}
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
      else if (strcmp (s, "hashdepth") == 0)
	ChangeHashDepth ();
      else if (strcmp (s, "random") == 0)
	dither = 6;
      else if (strcmp (s, "easy") == 0)
	flag.easy = !flag.easy;
      else if (strcmp (s, "contempt") == 0)
	SetContempt ();
      else if (strcmp (s, "analysis") == 0)
	SetAnalysis ();
      else if (strcmp (s, "xwndw") == 0)
	ChangeXwindow ();
      else if (strcmp (s, "coords") == 0)
	{
	  coords = !coords;
	  UpdateDisplay (0, 0, 1, 0);
	}
#if !defined(MSDOS) || defined(SEVENBIT)
      else if (strcmp (s, "stars") == 0)
	{
	  stars = !stars;
	  UpdateDisplay (0, 0, 1, 0);
	}
#endif /* MSDOS && !SEVENBIT */
      else if (strcmp (s, "test") == 0)
	{
	  ShowMessage ("Testing MoveList Speed");
	  TestSpeed (MoveList);
	  ShowMessage ("Testing CaptureList Speed");
	  TestSpeed (CaptureList);
	}
      else if (strcmp (s, "p") == 0)
	ShowPostnValues ();
      else if (strcmp (s, "debug") == 0)
	DoDebug ();
      else if (strcmp (s, "rv") == 0)
	{
	  rv = !rv;
#if !defined(MSDOS) || defined(SEVENBIT)
	  shade = !rv;
	  stars = !rv;
#endif /* MSDOS && !SEVENBIT */
	  UpdateDisplay (0, 0, 1, 0);
	}
    }

  ClearMessage ();
  ElapsedTime (1);
  if (flag.force)
    {
      computer = opponent;
      opponent = otherside[computer];
    }
  signal (SIGINT, TerminateSearch);
#ifndef MSDOS
  signal (SIGQUIT, TerminateSearch);
#endif /* MSDOS */
}
