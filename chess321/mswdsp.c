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

#define NOSOUND

#define NOWH

#define NOCOMM

#define NOKANJI



#include <windows.h>

#include <string.h>

#include <stdio.h>



#include "gnuchess.h"

#include "chess.h"

#include "defs.h"

#include "stats.h"



extern short boarddraw[64];

extern short colordraw[64];



extern char mvstr[4][6];

extern long evrate;

static char* ColorStr[2] = {"White", "Black"};



void

ShowPlayers (void)

{

  /* display in the status line what color the computer is playing */

  SetWindowText ( hComputerColor, (computer == black) ? "Computer is black" :

                                                        "Computer is white" );

}



void

ShowDepth (char ch)

{

   char tmp[30];

   if ( hStats ) {

      wsprintf ( tmp, "%d%c", Sdepth, ch);

      SetDlgItemText ( hStats, DEPTHTEXT, tmp);

   }

}



void

ShowScore (short score)

{

   char tmp[30];

   if ( hStats) {

      wsprintf ( tmp, "%d",score);

      SetDlgItemText ( hStats, SCORETEXT, tmp);

   }

}



void

ShowMessage (HWND hWnd, char *s)

{

  MessageBox ( hWnd, s, "Chess", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);

}



void SMessageBox ( HWND hWnd, int str_num, int str1_num )

{

   char str[80], str1[20];

   LoadString ( GetWindowWord(hWnd,GWW_HINSTANCE), str_num, str, sizeof ( str ) );

   LoadString ( GetWindowWord(hWnd,GWW_HINSTANCE), str1_num, str1, sizeof ( str1 ) );

   MessageBox ( hWnd, str, str1, MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL );

}



void

ClearMessage (void)

{

}



void

ShowCurrentMove (short int pnt, short int f, short int t)

{

   char tmp[30];



   if ( hStats) {

      algbr (f, t, false);

      wsprintf ( tmp, "(%2d) %4s",pnt, (char far *)mvstr[0]);

      SetDlgItemText ( hStats, POSITIONTEXT, tmp);

   }

}



void

ShowSidetoMove (void)

{

   char tmp[30];

   wsprintf ( tmp, "It is %s's turn",(char far*)ColorStr[player]);

   SetWindowText ( hWhosTurn, (LPSTR) tmp);



}



void

ShowPrompt (void)

{

}



void

ShowNodeCnt (long int NodeCnt, long int evrate)

{

   char tmp[40];



   if ( hStats ) {

      wsprintf ( tmp,"%-8ld", NodeCnt);

      SetDlgItemText ( hStats, NODETEXT, tmp);

      wsprintf ( tmp,"%-5ld", evrate);

      SetDlgItemText ( hStats, NODESECTEXT, tmp);

   }

}  



void

ShowResults (short int score, short unsigned int *bstline, char ch)

{

  unsigned char d, ply;

  char str[300];

  int s;



  if (flag.post)

    {

      ShowDepth (ch);

      ShowScore (score);

      d = 7; s = 0;

      for (ply = 1; bstline[ply] > 0; ply++)

        {

         algbr ((short) bstline[ply] >> 8, (short) bstline[ply] & 0xFF, false);

         if ( ply==5 || ply==9 || ply==13 || ply==17)

            s += wsprintf ( str+s,"\n");

         s += wsprintf ( str+s,"%-5s ", (char far *) mvstr[0]);

        }

      SetDlgItemText ( hStats, BSTLINETEXT, (LPSTR) str);

    }

}



void

SearchStartStuff (short int side)

{

}



void

OutputMove (HWND hWnd)

{

  char tmp[30];



  UpdateDisplay (hWnd, root->f, root->t, 0, (short) root->flags);

  wsprintf ( tmp, "My move is %s",(char far *) mvstr[0]);

  SetWindowText ( hComputerMove, tmp);



  if (root->flags & draw)

    SMessageBox ( hWnd, IDS_DRAWGAME,IDS_CHESS);

  else if (root->score == -9999)

    SMessageBox ( hWnd, IDS_YOUWIN, IDS_CHESS);

  else if (root->score == 9998)

    SMessageBox ( hWnd, IDS_COMPUTERWIN,IDS_CHESS);

  else if (root->score < -9000)

    SMessageBox ( hWnd, IDS_MATESOON,IDS_CHESS);

  else if (root->score > 9000)

    SMessageBox ( hWnd, IDS_COMPMATE,IDS_CHESS);

  if (flag.post)

    {

      ShowNodeCnt (NodeCnt, evrate);

/*

      for (i = 1999; i >= 0 && Tree[i].f == 0 && Tree[i].t == 0; i--);

      printz ("Max Tree= %5d", i);

*/

    }

}



void

UpdateClocks (void)

{

  short m, s;

  char tmp[20];



  m = (short) (et / 60);

  s = (short) (et - 60 * (long) m);

  if (TCflag)

    {

      m = (short) ((TimeControl.clock[player] - et) / 60);

      s = (short) (TimeControl.clock[player] - et - 60 * (long) m);

    }

  if (m < 0) m = 0;

  if (s < 0) s = 0;



  wsprintf ( tmp, "%0d:%02d",m,s);

  if ( player == white ) {

      SetWindowText (hClockHuman, tmp);

  } else {

      SetWindowText (hClockComputer, tmp);

  }



  if (flag.post)

    ShowNodeCnt (NodeCnt, evrate);

}



void

ShowPostnValue (short int sq)

{

}



void

ShowPostnValues (void)

{

}



void DrawPiece ( HWND hWnd, short int f )

{

   POINT aptl[4];

   HRGN hRgn;

   int x,y;



   if ( flag.reverse ) {

     x = 7-(f%8);

     y = 7-(f/8);

   } else {

      x = f%8;

      y = f/8;

   }



   QuerySqCoords ( x,y, aptl+0);

   hRgn = CreatePolygonRgn( aptl, 4, WINDING);

   InvalidateRgn ( hWnd, hRgn, FALSE );

   DeleteObject ( hRgn);

}



void

UpdateDisplay (HWND hWnd, short int f, short int t, short int redraw, short int isspec)

{

  short sq;

  

  for (sq=0; sq<64; sq++) {

         boarddraw[sq] = board[sq];

         colordraw[sq] = color[sq];

  }



  if (redraw){

      InvalidateRect ( hWnd, NULL, TRUE);

      ShowPlayers ();

      UpdateWindow ( hWnd );

  } else {

      DrawPiece (hWnd, f);

      DrawPiece (hWnd, t);

      if (isspec & cstlmask)

        if (t > f)

          {

            DrawPiece (hWnd, f + 3);

            DrawPiece (hWnd, t - 1);

          }

        else

          {

            DrawPiece (hWnd, f - 4);

            DrawPiece (hWnd, t + 1);

          }

      else if (isspec & epmask)

        {

          DrawPiece (hWnd, t - 8);

          DrawPiece (hWnd, t + 8);

        }

      UpdateWindow (hWnd);

    }

}



void

GiveHint (HWND hWnd)

{

  char s[40];

  algbr ((short) (hint >> 8), (short) (hint & 0xFF), false);

  strcpy (s, "try ");

  strcat (s, mvstr[0]);

  ShowMessage (hWnd, s);

}

