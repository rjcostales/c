/*
 * nuxdsp.c - (new)  ALPHA interface for CHESS
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
#include <ctype.h>
#include <signal.h>
#ifdef MSDOS
#include <dos.h>
/*#include <conio.h>*/
#include <stdlib.h>
#include <string.h>

#define ESC 0x1B
#define refresh() fflush(stdout)


#else
#include <sys/param.h>
#include <sys/types.h>
#include <sys/file.h>
#include <curses.h>

#endif /* MSDOS */

#include "gnuchess.h"
static void param (SHORT n);
int mycnt1, mycnt2;
extern tshort sscore[];

extern SHORT pscore[2];

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
CHAR Analysis[128] = "";
UTSHORT MV[MAXDEPTH+1];
int MSCORE;
CHAR *DRAW;
extern SHORT Mwpawn[64], Mbpawn[64], Mknight[2][64], Mbishop[2][64];
SHORT PositionFlag = 0;

#if defined(MSDOS) && !defined(SEVENBIT)
static void ONormal (void);
static void OReverse (void);

#endif /* MSDOS && !SEVENBIT */

/* extern CHAR *getenv (const CHAR *);*/
void TerminateSearch (int), Die (int);

void
Initialize (void)
{
  signal (SIGINT, Die);
#ifndef MSDOS
  signal (SIGQUIT, Die);
  initscr ();
  crmode ();
  echo ();
#else
  mycnt1 = mycnt2 = 0;
#endif /* MSDOS */
}

void
ExitChess (void)
{
  ListGame ();
  gotoXY (1, 24);
#ifndef MSDOS
  refresh();
  nocrmode ();
  endwin ();
#endif /* MSDOS */
  exit (0);
}

void
Die (int Sig)
{
  CHAR s[80];

  signal (SIGINT, SIG_IGN);
#ifdef MSDOS
  Sig++;			/* shut up the compiler */
#else
  signal (SIGQUIT, SIG_IGN);
#endif /* MSDOS */
  ShowMessage (CP[31]);		/*Abort?*/
  scanz ("%s", s);
  if (strcmp (s, CP[210]) == 0)	/*yes*/
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
  Sig++;			/* shut up the compiler */
#else
  signal (SIGQUIT, SIG_IGN);
#endif /* MSDOS */
  if (!flag.timeout)
    flag.musttimeout = true;
  flag.bothsides = false;
  signal (SIGINT, Die);
#ifndef MSDOS
  signal (SIGQUIT, Die);
#endif /* MSDOS */
}
void
ShowLine (UTSHORT *bstline)
{
}

void
help (void)
{
  ClrScreen ();
  /*printz ("CHESS command summary\n");*/
  printz (CP[40]);
  printz ("----------------------------------------------------------------\n");
  /*printz ("g1f3      move from g1 to f3      quit      Exit Chess\n");*/
  printz (CP[158]);
#ifdef DEBUG
  /*printz ("Nf3       move knight to f3       cache      turn %s\n", (flag.cache) ? "off" : "on");*/
  printz (CP[86], (!flag.nocache) ? CP[92] : CP[93]);
#endif
  /*printz ("a7a8q     promote pawn to queen\n");*/
  printz (CP[128], (flag.material) ? CP[92] : CP[93]);
  /*printz ("o-o       castle king side        easy      turn %s\n", (flag.easy) ? "off" : "on");*/
  printz (CP[173], (flag.easy) ? CP[92] : CP[93]);
  /*printz ("o-o-o     castle queen side       hash      turn %s\n", (flag.hash) ? "off" : "on");*/
  printz (CP[174], (flag.hash) ? CP[92] : CP[93]);
  /*printz ("bd        redraw board            reverse   board display\n");*/
  printz (CP[130]);
  /*printz ("list      game to chess.lst       book      turn %s used %d of %d\n", (Book) ? "off" : "on", bookcount, booksize);*/
  printz (CP[170], (Book) ? CP[92] : CP[93], bookcount, BOOKSIZE);
  /*printz ("undo      undo last ply           remove    take back a move\n");*/
  printz (CP[200]);
  /*printz ("edit      edit board              force     enter game moves\n");*/
  printz (CP[153]);
  /*printz ("switch    sides with computer     both      computer match\n");*/
  printz (CP[194]);
  /*printz ("white     computer plays white    black     computer plays black\n");*/
  printz (CP[202]);
  /*printz ("depth     set search depth        clock     set time control\n");*/
  printz (CP[149]);
  /*printz ("hint      suggest a move         post      turn %s principle variation\n", (flag.post) ? "off" : "on");*/
  printz (CP[177], (flag.post) ? CP[92] : CP[93]);
  /*printz ("save      game to file            get       game from file\n");*/
  printz (CP[188]);
  /*printz ("random    randomize play          new       start new game\n");*/
  printz (CP[181]);
  /*printz ("coords    show coords            rv        reverse video\n");*/
  printz (CP[144]);
#if !defined(MSDOS) || defined(SEVENBIT)
  printz (CP[192]);
#endif /* !MSDOS || SEVENBIT */
  gotoXY (10, 20);
  printz (CP[47], ColorStr[computer]);
  gotoXY (10, 21);
  printz (CP[97], ColorStr[opponent]);
  gotoXY (10, 22);
  printz (CP[79], MaxResponseTime/100);
  gotoXY (10, 23);
  printz (CP[59], (flag.easy) ? CP[93] : CP[92]);
  gotoXY (40, 20);
  printz (CP[52], MaxSearchDepth);
  gotoXY (40, 21);
  printz (CP[100], (dither) ? CP[93] : CP[92]);
  gotoXY (40, 22);
  printz (CP[112], (flag.hash) ? CP[93] : CP[92]);
  gotoXY (40, 23);
  printz (CP[73]);
  gotoXY (10, 24);
if(flag.gamein)
  printz (CP[230], (TCflag) ? CP[93] : CP[92],
           TimeControl.clock[white] / 100, TCadd/100, MaxSearchDepth);
else
  printz (CP[110], (TCflag) ? CP[93] : CP[92],
          TimeControl.moves[white], TimeControl.clock[white] / 100, TCadd/100, MaxSearchDepth);
  refresh ();
#ifdef BOGUS
  fflush (stdin); /*what is this supposed to do??*/
#endif /*BOGUS*/
  getchar ();
  ClrScreen ();
  UpdateDisplay (0, 0, 1, 0);
}

void
EditBoard (void)

/*
 * Set up a board position. Pieces are entered by typing the piece followed
 * by the location. For example, Nf3 will place a knight on square f3.
 */

{
  SHORT a, r, c, sq, i;
  CHAR s[80];

  flag.regularstart = true;
  Book = BOOKFAIL;
  ClrScreen ();
  UpdateDisplay (0, 0, 1, 0);
  gotoXY (TAB, 3);
  printz (CP[29]);
  gotoXY (TAB, 4);
  printz (CP[28]);
  gotoXY (TAB, 5);
  printz (CP[136]);
  gotoXY (TAB, 7);
  printz (CP[64]);
  a = white;
  do
    {
      gotoXY (TAB, 6);
      printz (CP[60], ColorStr[a]);	/*Editing %s*/
      gotoXY (TAB + 24, 7);
      ClrEoln ();
      scanz ("%s", s);
      if (s[0] == CP[28][0])	/*#*/
	{
	  for (sq = 0; sq < 64; sq++)
	    {
	      board[sq] = no_piece;
	      color[sq] = neutral;
	      DrawPiece (sq);
	    }
	}
      if (s[0] == CP[136][0])	/*c*/
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
	  color[sq] = ((board[sq] == no_piece) ? neutral : a);
	  DrawPiece (sq);
	}
  } while (s[0] != CP[29][0]);

  for (sq = 0; sq < 64; sq++)
    Mvboard[sq] = ((board[sq] != Stboard[sq]) ? 10 : 0);
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
  printz ("%s", (computer == black) ? CP[218] : CP[74]);
  gotoXY (TAB, ((flag.reverse) ? 2 : 23));
  printz ("%s", (computer == white) ? CP[218] : CP[74]);
}

void
ShowDepth (CHAR ch)
{
  gotoXY (TAB, 4);
  printz (CP[53], Sdepth, ch);	/*Depth= %d%c*/
  ClrEoln ();
}

void
ShowScore (SHORT score)
{
  gotoXY (TAB, 5);
 printz (CP[104], score);
  ClrEoln ();
}

void
ShowMessage (CHAR *s)
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
ShowCurrentMove (SHORT pnt, SHORT f, SHORT t)
{
  algbr (f, t, false);
  gotoXY (TAB, 7);
  printz ("(%2d) %4s", pnt, mvstr[0]);
}

void
ShowHeader (void)
{
  gotoXY (TAB, 0);
#ifdef MSDOS
  printz (CP[67]);
#else
  printz (CP[68]);
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
  printz (CP[121]);		/*Your movwe is?*/
  ClrEoln ();
}

void
ShowNodeCnt (long int NodeCnt)
{
  gotoXY (TAB, 21);
  printz (CP[90], NodeCnt, (et > 100) ? NodeCnt / (et / 100) : 0);
  ClrEoln ();
}

void
ShowResults (SHORT score, UTSHORT *bstline, CHAR ch)
{
  UCHAR d, ply;

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
	  algbr ((SHORT) bstline[ply] >> 8, (SHORT) bstline[ply] & 0xFF, false);
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
SearchStartStuff (SHORT side)
{
  SHORT i;

  signal (SIGINT, TerminateSearch);
#ifdef MSDOS
  side++;			/* shut up the compiler */
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
OutputMove (SHORT score)
{

  UpdateDisplay (root->f, root->t, 0, (SHORT) root->flags);
  gotoXY (TAB, 17);
  if(flag.illegal) {printz (CP[225]); return;}
  printz (CP[84], mvstr[0]);	/*My move is %s*/
  if (flag.beep)
    putchar (7);
  ClrEoln ();

  gotoXY (TAB, 24);
  if (root->flags & draw)
    printz (CP[56], DRAW);
  else if (root->score == -9999)
    printz (CP[95]);
  else if (root->score == 9998)
    printz (CP[44]);
#ifdef VERYBUGGY
  else if (root->score < -9000)
    printz (CP[96]);
  else if (root->score > 9000)
    printz (CP[45]);
#endif /*VERYBUGGY*/
  ClrEoln ();

  if (flag.post)
    {
      register SHORT h, l, t;

      h = TREE;
      l = 0;
      t = TREE >> 1;
      while (l != t)
	{
	  if (Tree[t].f || Tree[t].t)
	    l = t;
	  else
	    h = t;
	  t = (l + h) >> 1;
	}


      ShowNodeCnt (NodeCnt);
      gotoXY (TAB, 22);

      printz (CP[81], t);	/*Max Tree= %d*/
      ClrEoln ();
    }
}

void
UpdateClocks (void)
{
  SHORT m, s;

  m = (SHORT) (et / 6000);
  s = (SHORT) (et - 6000 * (long) m) / 100;
  if (TCflag)
    {
      m = (SHORT) ((TimeControl.clock[player] - et) / 6000);
      s = (SHORT) ((TimeControl.clock[player] - et - 6000 * (long) m) / 100);
    }
  if (m < 0)
    m = 0;
  if (s < 0)
    s = 0;
  if (player == white)
    gotoXY (60, (flag.reverse) ? 2 : 23);
  else
    gotoXY (60, (flag.reverse) ? 23 : 2);
  printz ("%d:%02d   ", m, s);
  if (flag.post)
    ShowNodeCnt (NodeCnt);
  refresh ();
}

void
gotoXY (SHORT x, SHORT y)
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
param (SHORT n)
{
  if (n >= 10)
    {
      register SHORT d, q;

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
  /* attroff (A_REVERSE); */
#endif /* MSDOS */
}

void
DrawPiece (SHORT sq)
{
  gotoXY (VcoordR (sq, 2, 2));

  switch (color[sq])
    {
    case black:
      if (flag.rv)
	OReverse ();
#if defined(MSDOS) && !defined(SEVENBIT)
      printz (" %c ", pxx[board[sq]]);
#else
      printz ((flag.stars ? "*%c*" : " %c "), pxx[board[sq]]);
#endif /* MSDOS && !SEVENBIT */
      ONormal ();
      break;
    case neutral:
#if defined(MSDOS) && !defined(SEVENBIT)
      if (flag.rv)
	printz (Vblack (sq) ? "\262\262\262" : "\260\260\260");
      else
	printz (Vblack (sq) ? "\260\260\260" : "\262\262\262");
#else
      if (flag.shade)
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
      if (!flag.rv)
	OReverse ();
      printz (" %c ", pxx[board[sq]]);
      ONormal ();
#else
      printz (" %c ", pxx[board[sq]]);
#endif /* MSDOS && !SEVENBIT */
      break;
    default:
      ShowMessage (CP[55]);	/*Draw piece color[sq] err*/
      break;
    }
}

void
DrawSquare (SHORT sq)
{
#if defined(MSDOS) && !defined(SEVENBIT)
  if (flag.rv)
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
  if (flag.shade)
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
  SHORT z;

  for (z = 0; z <= 7; z++)
    {
      SHORT sq;

      sq = z << 3;
      gotoXY (VcoordI (sq, 1, 1));
#if !defined(MSDOS) || defined(SEVENBIT)
      if ((Vblack (sq) || flag.shade) && flag.rv)
#endif /* !MSDOS || SEVENBIT */
	OReverse ();
      printz ("%d", 1 + z);
      ONormal ();
    }

  for (z = 0; z <= 7; z++)
    {
      SHORT sq;

      sq = z;
      gotoXY (VcoordI (sq, SQW, SQH));
#if !defined(MSDOS) || defined(SEVENBIT)
      if ((Vblack (sq) || flag.shade) && flag.rv)
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
ShowPostnValue (SHORT sq)

/*
 * must have called ExaminePosition() first
 */

{
  SHORT score;

  gotoXY (VcoordR (sq, 2, 1));
  score = ScorePosition (color[sq]);
#if !defined(MSDOS) || defined(SEVENBIT)
  if (Vblack (sq) && !flag.shade)
    OReverse ();
#endif /* !MSDOS || SEVENBIT */

  if (color[sq] != neutral)
    printz ("%3d", svalue[sq]);
  else
#if defined(MSDOS) && !defined(SEVENBIT)
    {
      if (flag.rv)
	printz (Vblack (sq) ? "\262\262\262" : "\260\260\260");
      else
	printz (Vblack (sq) ? "\260\260\260" : "\262\262\262");
    }
#else
    printz (flag.shade && Vblack (sq) ? "///" : "   ");
#endif /* MSDOS && !SEVENBIT */
  ONormal ();
}

void
ShowPostnValues (void)
{
  SHORT sq, score;

  ExaminePosition ();
  for (sq = 0; sq < 64; sq++)
    ShowPostnValue (sq);
  score = ScorePosition (opponent);
  gotoXY (TAB, 5);
 printz (CP[103], score, mtl[computer], pscore[computer], mtl[opponent],pscore[opponent]);
  ClrEoln ();
}

void
UpdateDisplay (SHORT f, SHORT t, SHORT redraw, SHORT isspec)
{
  SHORT sq;

  if (redraw)
    {
      ShowHeader ();
      ShowPlayers ();
      for (sq = 0; sq < 64; sq++)
	{
	  DrawSquare (sq);
	  DrawPiece (sq);
	}
      if (flag.coords)
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

extern CHAR *InPtr;
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

void
ChangeAlphaWindow (void)
{
  ShowMessage (CP[114]);
  scanz ("%hd", &WAwindow);
  ShowMessage (CP[34]);
  scanz ("%hd", &BAwindow);
}

void
ChangeBetaWindow (void)
{
  ShowMessage (CP[115]);
  scanz ("%hd", &WBwindow);
  ShowMessage (CP[35]);
  scanz ("%hd", &BBwindow);
}

void
GiveHint (void)
{
  CHAR s[40];
  if (hint)
    {
      algbr ((SHORT) (hint >> 8), (SHORT) (hint & 0xFF), false);
      strcpy (s, CP[198]);	/*try*/
      strcat (s, mvstr[0]);
      ShowMessage (s);
    }
  else
    ShowMessage (CP[223]);
}

void
ChangeHashDepth (void)
{
  ShowMessage (CP[163]);
  scanz ("%hd", &HashDepth);
  ShowMessage (CP[82]);
  scanz ("%hd", &HashMoveLimit);
}

void
ChangeSearchDepth (void)
{
  ShowMessage (CP[150]);
  scanz ("%hd", &MaxSearchDepth);
  TCflag = !(MaxSearchDepth > 0);
}

void
SetContempt (void)
{
  ShowMessage (CP[142]);
  scanz ("%hd", &contempt);
}


void
SetAnalysis (void)
{
  ShowMessage (CP[159]);
  scanz ("%s", Analysis);
}

void
ChangeXwindow (void)
{
  ShowMessage (CP[208]);
  scanz ("%hd", &xwndw);
}

void
SelectLevel (CHAR *sx)
{

  CHAR T[64], *p, *q;
  ClrScreen ();
  XC = XCmore =0;

  if ((p = strstr (sx, CP[169])) != NULL) p += strlen (CP[169]);
  else if ((p = strstr (sx, CP[217])) != NULL) p += strlen (CP[217]);
  strcat (sx, "XX");
  q = T;
  *q = '\0';
  for (; *p != 'X'; *q++ = *p++);
  *q = '\0';
  /* line empty ask for input */
  while(true){
  if (!T[0])
    {
      gotoXY(5,2);
      printz(CP[61]);
      refresh();
#ifdef MSDOS
      T[0] = '\0';
      gets (T);
#else
      getstr (T);
#endif

    }
  strcat (T, "XX");
  /* skip whitespace */
  for (p = T; *p == ' '; p++);
  /*
  Input forms are:
             M    m[:s]		- M moves in m:s time
		  m[:s]   t	- game in m:s time increment t
		  m:[s]		- game in m:s time
             M    m[:s]	  t	- for M moves m:s time increment t then reset
  */
  /* could be moves or a incremental clock */
      TCminutes = TCmoves = TCseconds =  TCadd = 0;
      TCmoves = (SHORT) strtol (p, &q, 10);
      if (*q == ':')
	{ /* its gamein form: m:s  or m:s t */
	  TCminutes = TCmoves;
	  TCmoves = 0;
	  if(*(q+1) != ' ') TCseconds = (SHORT) strtol (q + 1, &q, 10); else {q++; }
	  if(*q != 'X' && *q != '+'){
		TCadd = (SHORT) strtol (q, &q, 10) * 100;
        }
	}
      else
	{ /* m or m t or M m t*/
	  while (*q == ' ') q++;
	  if (*q == 'X' || *q == '+')
	    { /*  m only 1 number must be gamein */
	      TCminutes = TCmoves;
	      TCmoves = 0;
	    }
	  else
	    { /* M m or M m[:s] t */
	      TCminutes = (SHORT) strtol (q, &q, 10);
	      if (*q == ':')
		{ /* M m[:s] t */
	          if(*(q+1) != ' ') TCseconds = (SHORT) strtol (q + 1, &q, 10); else {q++; }
		  TCadd = (SHORT) strtol (q, &q, 10);
		}else{/* M m or M m t */
		  if(*q != 'X' && *q != '+')/* its M m t*/ TCadd = (SHORT) strtol (q, &q, 10)*100;
	    	     }
	     }
	}
	while(*q == ' ')q++;
	XCmoves[XC] = TCmoves;
	XCminutes[XC] = TCminutes;
	XCseconds[XC] = TCseconds;
	XCadd[XC] = TCadd;
	XC++;
	if(*q != '+' || XC == 3) break;
    T[0]='\0';
    }
  TimeControl.clock[white] = TimeControl.clock[black] = 0;
  SetTimeControl ();
  ClrScreen ();
  UpdateDisplay (0, 0, 1, 0);
}


void
DoDebug (void)
{
  SHORT c, p, sq, tp, tc, tsq, score;
  CHAR s[40];

  ExaminePosition ();
  ShowMessage (CP[65]);
  scanz ("%s", s);
  c = neutral;
  if (s[0] == CP[9][0] || s[0] == CP[9][1])	/* w W*/ c = white;
  if (s[0] == CP[9][2] || s[0] == CP[9][3])	/*b B*/ c = black;
  for (p = king; p > no_piece; p--)
    if ((s[1] == pxx[p]) || (s[1] == qxx[p])) break;
  if(p > no_piece)
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
  score = ScorePosition (opponent);
  gotoXY (TAB, 5);
  printz (CP[103], score, mtl[computer], pscore[computer], mtl[opponent],pscore[opponent]);
  ClrEoln ();
}
void
DoTable (SHORT table[64])
{
  SHORT  sq;
  ExaminePosition ();
  for (sq=0;sq<64;sq++){

  gotoXY (VcoordR (sq, 2, 1));
#if !defined(MSDOS) || defined(SEVENBIT)
  if (Vblack (sq) && !flag.shade)
    OReverse ();
#endif /* !MSDOS || SEVENBIT */

    printz ("%3d", table[sq]);
  ONormal ();
}
}
SHORT LdisplayLine;
void Ldisplay1(){LdisplayLine=4; ClrScreen();UpdateDisplay (0, 0, 1, 0); }
 
void Ldisplay2(){
  refresh ();
  getchar ();
  ClrScreen ();
  UpdateDisplay (0, 0, 1, 0);
}
 
void Ldisplay(char *m, char *h, SHORT count)
{
        gotoXY(50,LdisplayLine);
        LdisplayLine++;
        printz("%s\t%s\t%d\n",m,h,count);
}
Ldisplay3()
{ClrScreen();refresh();LdisplayLine=4;}
void Ldisplay4(char *line)
{
        gotoXY(10,LdisplayLine);
        LdisplayLine++;
        printz("%s",line);
        refresh();
}

