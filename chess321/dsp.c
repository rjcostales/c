/*

  C source for GNU CHESS



  Revision: 1990-09-30



  Modified by Daryl Baker for use in MS WINDOWS environment



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



#define NOATOM 

#define NOCLIPBOARD

#define NOCREATESTRUCT

#define NOFONT

#define NOREGION

#define NOSOUND

#define NOWH

#define NOWINOFFSETS

#define NOCOMM

#define NOKANJI



#include <windows.h>

#include <string.h>

#include <time.h>

#include <stdio.h>



#include "gnuchess.h"

#include "defs.h"

#include "chess.h"





static short _based(_segname("_CODE")) Stboard[64] =

{rook, knight, bishop, queen, king, bishop, knight, rook,

 pawn, pawn, pawn, pawn, pawn, pawn, pawn, pawn,

 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

 pawn, pawn, pawn, pawn, pawn, pawn, pawn, pawn,

 rook, knight, bishop, queen, king, bishop, knight, rook};



static short _based(_segname("_CODE")) Stcolor[64] =

{white, white, white, white, white, white, white, white,

 white, white, white, white, white, white, white, white,

 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,

 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,

 black, black, black, black, black, black, black, black,

 black, black, black, black, black, black, black, black};



int mycntl1, mycntl2;



char mvstr[4][6];

long evrate;

int PositionFlag = 0;



#define pxx " PNBRQK"

#define qxx " pnbrqk"



void TerminateSearch (int), Die (int);



void

Initialize (void)

{

  mycntl1 = mycntl2 = 0;

}



void

ExitChess (void)

{



}



void

Die (int Sig)

{

}



void

TerminateSearch (int Sig)

{

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

      mvstr[0][0] = (char) ('a'+column (f));

      mvstr[0][1] = (char) ('1'+row (f));

      mvstr[0][2] = (char) ('a'+column (t));

      mvstr[0][3] = (char) ('1'+row (t));

      mvstr[0][4] = mvstr[3][0] = '\0';

      if ((mvstr[1][0] = pxx[board[f]]) == 'P')

        {

          if (mvstr[0][0] == mvstr[0][2])       /* pawn did not eat */

            {

              mvstr[2][0] = mvstr[1][0] = mvstr[0][2];  /* to column */

              mvstr[2][1] = mvstr[1][1] = mvstr[0][3];  /* to row */

              m3p = 2;

            }

          else

            /* pawn ate */

            {

              mvstr[2][0] = mvstr[1][0] = mvstr[0][0];  /* from column */

              mvstr[2][1] = mvstr[1][1] = mvstr[0][2];  /* to column */

              mvstr[2][2] = mvstr[0][3];

              m3p = 3;          /* to row */

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

          mvstr[2][2] = mvstr[1][1] = mvstr[0][2];      /* to column */

          mvstr[2][3] = mvstr[1][2] = mvstr[0][3];      /* to row */

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

VerifyMove (HWND hWnd, char *s, short int iop, short unsigned int *mv)



/*

   Compare the string 's' to the list of legal moves available for the

   opponent. If a match is found, make the move on the board.

*/



{

  static short pnt, tempb, tempc, tempsf, tempst, cnt;

  static struct leaf xnode;

  struct leaf  far *node;



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

          SMessageBox (hWnd, IDS_ILLEGALMOVE, IDS_CHESS);

          return (false);

        }

      else

        {

          if (iop == 1)

            return (true);

          UpdateDisplay (hWnd, xnode.f, xnode.t, 0, (short) xnode.flags);

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

  if (cnt > 1) SMessageBox (hWnd, IDS_AMBIGUOUSMOVE, IDS_CHESS);

  return (false);

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

GetGame (HWND hWnd, char *fname)

{

  FILE *fd;

  int c;

  short sq;

  unsigned short m;



  struct GameRec tmp_rec;



  if ((fd = fopen (fname, "r")) == NULL)

    {

      SMessageBox (hWnd, IDS_LOADFAILED, IDS_CHESS);

      return;

    }



  fscanf (fd, "%hd%hd%hd", &computer, &opponent, &Game50);

  fscanf (fd, "%hd%hd", &castld[white], &castld[black]);

  fscanf (fd, "%hd%hd", &TCflag, &OperatorTime);

  fscanf (fd, "%ld%ld%hd%hd",

          &TimeControl.clock[white], &TimeControl.clock[black],

          &TimeControl.moves[white], &TimeControl.moves[black]);

  for (sq = 0; sq < 64; sq++)

    {

      fscanf (fd, "%hd%hd", &m, &Mvboard[sq]);

      board[sq] = (m >> 8);

      color[sq] = (m & 0xFF);

      if (color[sq] == 0)

        color[sq] = neutral;

      else

        --color[sq];

    }

  GameCnt = 0;

  c = '?';

  while (c != EOF)

    {

      ++GameCnt;

      c = fscanf (fd, "%hd%hd%hd%ld%hd%hd%hd", &tmp_rec.gmove,

                  &tmp_rec.score, &tmp_rec.depth,

                  &tmp_rec.nodes, &tmp_rec.time,

                  &tmp_rec.piece, &tmp_rec.color);

      GameList[GameCnt] = tmp_rec;

      if (GameList[GameCnt].color == 0)

        GameList[GameCnt].color = neutral;

      else

        --GameList[GameCnt].color;

    }



  GameCnt--;

  if (TimeControl.clock[white] > 0)

    TCflag = true;

  computer--;

  opponent--;



  fclose (fd);



  InitializeStats ();

  Sdepth = 0;

  UpdateDisplay (hWnd, 0, 0, 1, 0);

}



void

SaveGame (HWND hWnd, char *fname)

{

  FILE *fd;

  short sq, i, c;



  if (NULL == (fd = fopen (fname, "w")))

    {

      ShowMessage (hWnd, "Not saved");

      return;

    }



  fprintf (fd, "%d %d %d\n", computer + 1, opponent + 1, Game50);

  fprintf (fd, "%d %d\n", castld[white], castld[black]);

  fprintf (fd, "%d %d\n", TCflag, OperatorTime);

  fprintf (fd, "%ld %ld %d %d\n",

           TimeControl.clock[white], TimeControl.clock[black],

           TimeControl.moves[white], TimeControl.moves[black]);

  for (sq = 0; sq < 64; sq++)

    {

      if (color[sq] == neutral)

        c = 0;

      else

        c = color[sq] + 1;

      fprintf (fd, "%d %d\n", 256 * board[sq] + c, Mvboard[sq]);

    }

  for (i = 1; i <= GameCnt; i++)

    {

      if (GameList[i].color == neutral)

        c = 0;

      else

        c = GameList[i].color + 1;

      fprintf (fd, "%d %d %d %ld %d %d %d\n",

               GameList[i].gmove, GameList[i].score, GameList[i].depth,

               GameList[i].nodes, GameList[i].time,

               GameList[i].piece, c);

    }

  fclose (fd);

}



void

ListGame (HWND hWnd, char *fname)

{

  FILE *fd;

  short i, f, t;



  if ( (fd = fopen (fname, "w")) == NULL) {

      ShowMessage (hWnd, "Cannot write chess.lst");

      return;

  }



  fprintf (fd, "\n");

  fprintf (fd, "       score  depth   nodes  time         ");

  fprintf (fd, "       score  depth   nodes  time\n");

  for (i = 1; i <= GameCnt; i++)

    {

      f = GameList[i].gmove >> 8;

      t = (GameList[i].gmove & 0xFF);

      algbr (f, t, false);

      fprintf (fd, "%5s  %5d     %2d %7ld %5d", mvstr[0],

               GameList[i].score, GameList[i].depth,

               GameList[i].nodes, GameList[i].time);



      if ((i % 2) == 0)

        fprintf (fd, "\n");

      else

        fprintf (fd, "         ");



    }

  fprintf (fd, "\n\n");

  fclose (fd);

}



void

Undo (HWND hWnd)



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

      if ((color[t] == white && row (f) == 6 && row (t) == 7)

          || (color[t] == black && row (f) == 1 && row (t) == 0))

        {

          int g, from = f;

          for (g = GameCnt - 1; g > 0; g--)

            if (GameList[g].gmove & 0xFF == from)

              from = GameList[g].gmove >> 8;

          if ((color[t] == white && row (from) == 1)

              || (color[t] == black && row (from) == 6))

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

  if (TCflag)

    ++TimeControl.moves[color[f]];

  GameCnt--;

  computer = otherside[computer];

  opponent = otherside[opponent];

  flag.mate = false;

  Sdepth = 0;

  UpdateDisplay (hWnd, 0, 0, 1, 0);

  InitializeStats ();

}

