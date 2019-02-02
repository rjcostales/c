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

#include <stdio.h>

#include <stdlib.h>

#include <time.h>



#include "gnuchess.h"

#include "defs.h"



extern HWND hStats;



/*extern short distdata[64][64], taxidata[64][64];*/

extern short far *distdata, far *taxidata;



extern FILE *hashfile;

extern short stage, stage2, Developed[2];

extern short ChkFlag[maxdepth], CptrFlag[maxdepth], PawnThreat[maxdepth];

extern short Pscore[maxdepth], Tscore[maxdepth];

extern short rehash;



/* extern struct hashval hashcode[2][7][64]; */

extern struct hashval far *hashcode;

extern unsigned char far * history;



void

Initialize_dist (void)

{

  register short a, b, d, di;



  for (a = 0; a < 64; a++)

    for (b = 0; b < 64; b++)

      {

        d = abs (column (a) - column (b));

        di = abs (row (a) - row (b));

/*

        taxidata[a][b] = d + di;

        distdata[a][b] = (d > di ? d : di);

*/

        *(taxidata+a*64+b) = d + di;

        *(distdata+a*64+b) = (d > di ? d : di);



      }

}



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



extern short board[64], color[64];



/*extern unsigned char nextpos[8][64][64];*/

/*extern unsigned char nextdir[8][64][64];*/

extern unsigned char far * nextpos;

extern unsigned char far * nextdir;

/*

  ptype is used to separate white and black pawns, like this;

  ptyp = ptype[side][piece]

  piece can be used directly in nextpos/nextdir when generating moves

  for pieces that are not black pawns.

*/

static short _based(_segname("_CODE")) ptype[2][8] =

{

  no_piece, pawn, knight, bishop, rook, queen, king, no_piece,

  no_piece, bpawn, knight, bishop, rook, queen, king, no_piece};



static short _based(_segname("_CODE")) direc[8][8] =

{

  0, 0, 0, 0, 0, 0, 0, 0,

  10, 9, 11, 0, 0, 0, 0, 0,

  8, -8, 12, -12, 19, -19, 21, -21,

  9, 11, -9, -11, 0, 0, 0, 0,

  1, 10, -1, -10, 0, 0, 0, 0,

  1, 10, -1, -10, 9, 11, -9, -11,

  1, 10, -1, -10, 9, 11, -9, -11,

  -10, -9, -11, 0, 0, 0, 0, 0};



static short _based(_segname("_CODE")) max_steps[8] =

{0, 2, 1, 7, 7, 7, 1, 2};



static short _based(_segname("_CODE")) nunmap[120] =

{

  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

  -1, 0, 1, 2, 3, 4, 5, 6, 7, -1,

  -1, 8, 9, 10, 11, 12, 13, 14, 15, -1,

  -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,

  -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,

  -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,

  -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,

  -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,

  -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,

  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};





void

Initialize_moves (void)



/*

  This procedure pre-calculates all moves for every piece from every square.

  This data is stored in nextpos/nextdir and used later in the move generation

  routines.

*/



{

  short ptyp, po, p0, d, di, s, delta;

  unsigned char far *ppos, far *pdir;

  short dest[8][8];

  short steps[8];

  short sorted[8];



  for (ptyp = 0; ptyp < 8; ptyp++)

    for (po = 0; po < 64; po++)

      for (p0 = 0; p0 < 64; p0++)

        {

          *(nextpos+ptyp*64*64+po*64+p0) = (unsigned char) po;

          *(nextdir+ptyp*64*64+po*64+p0) = (unsigned char) po;

        }

  for (ptyp = 1; ptyp < 8; ptyp++)

    for (po = 21; po < 99; po++)

      if (nunmap[po] >= 0)

        {

          ppos = nextpos+ptyp*64*64+nunmap[po]*64;

          pdir = nextdir+ptyp*64*64+nunmap[po]*64;

          /* dest is a function of direction and steps */

          for (d = 0; d < 8; d++)

            {

              dest[d][0] = nunmap[po];

              delta = direc[ptyp][d];

              if (delta != 0)

                {

                  p0 = po;

                  for (s = 0; s < max_steps[ptyp]; s++)

                    {

                      p0 = p0 + delta;

                      /*

                        break if (off board) or

                        (pawns only move two steps from home square)

                      */

                      if (nunmap[p0] < 0 || (ptyp == pawn || ptyp == bpawn)

                          && s > 0 && (d > 0 || Stboard[nunmap[po]] != pawn))

                        break;

                      else

                        dest[d][s] = nunmap[p0];

                    }

                }

              else

                s = 0;



              /*

                sort dest in number of steps order

                currently no sort is done due to compability with

                the move generation order in old gnu chess

              */

              steps[d] = s;

              for (di = d; s > 0 && di > 0; di--)

                if (steps[sorted[di - 1]] == 0) /* should be: < s */

                  sorted[di] = sorted[di - 1];

                else

                  break;

              sorted[di] = d;

            }



          /*

            update nextpos/nextdir,

            pawns have two threads (capture and no capture)

          */

          p0 = nunmap[po];

          if (ptyp == pawn || ptyp == bpawn)

            {

              for (s = 0; s < steps[0]; s++)

                {

                  ppos[p0] = (unsigned char) dest[0][s];

                  p0 = dest[0][s];

                }

              p0 = nunmap[po];

              for (d = 1; d < 3; d++)

                {

                  pdir[p0] = (unsigned char) dest[d][0];

                  p0 = dest[d][0];

                }

            }

          else

            {

              pdir[p0] = (unsigned char) dest[sorted[0]][0];

              for (d = 0; d < 8; d++)

                for (s = 0; s < steps[sorted[d]]; s++)

                  {

                    ppos[p0] = (unsigned char) dest[sorted[d]][s];

                    p0 = dest[sorted[d]][s];

                    if (d < 7)

                      pdir[p0] = (unsigned char) dest[sorted[d + 1]][0];

                    /* else is already initialized */

                  }

            }

        }

}



GLOBALHANDLE hGameList, hTree, hHistory, hTTable, hHashCode, hdistdata;

GLOBALHANDLE htaxidata, hnextdir, hnextpos;



/* hmm.... shouldn`t main be moved to the interface routines */

int

init_main (HWND hWnd)

{

  short int ahead = true, hash = true;



/* Mods for MS WINDOWS includes moving statics to dynamic allocated */



   hHistory = GlobalAlloc ( GMEM_MOVEABLE | GMEM_ZEROINIT,

                          (long) (8192 * sizeof (char) ) );



   if ( hHistory == NULL ) {

      return 1;

   }



   history = (unsigned char far *) GlobalLock (hHistory);

   

   hnextdir = GlobalAlloc ( GMEM_MOVEABLE | GMEM_ZEROINIT,

                          (long) (64*64*8 * sizeof (char) ) );



   if ( hnextdir == NULL ) {

      return 1;

   }



   nextdir = (unsigned char far *) GlobalLock (hnextdir);

   

   hnextpos = GlobalAlloc ( GMEM_MOVEABLE | GMEM_ZEROINIT,

                          (long) (64*64*8 * sizeof (char) ) );



   if ( hnextpos == NULL ) {

      return 1;

   }



   nextpos = (unsigned char far *) GlobalLock (hnextpos);

 

   hdistdata = GlobalAlloc ( GMEM_MOVEABLE | GMEM_ZEROINIT,

                          (long) (64*64 * sizeof (short) ) );



   if ( hdistdata == NULL ) {

      return 1;

   }



   distdata = (short far *) GlobalLock (hdistdata);



   htaxidata = GlobalAlloc ( GMEM_MOVEABLE | GMEM_ZEROINIT,

                          (long) (64*64 * sizeof (short) ) );



   if ( htaxidata == NULL ) {

      return 1;

   }



   taxidata = (unsigned short far *) GlobalLock (htaxidata);



   hHashCode = GlobalAlloc ( GMEM_MOVEABLE | GMEM_ZEROINIT,

                          (long) (2*7*64 * sizeof (struct hashval) ) );



   if ( hHashCode == NULL ) {

      return 1;

   }



   hashcode = (struct hashval far *) GlobalLock (hHashCode);



   hTree = GlobalAlloc ( GMEM_MOVEABLE | GMEM_ZEROINIT,

                          (long) (2000 * sizeof (struct leaf)) );



   if ( hTree == NULL ) {

      return 1;

   }



   Tree = (struct leaf far *) GlobalLock ( hTree);



   hGameList = GlobalAlloc ( GMEM_MOVEABLE | GMEM_ZEROINIT,

                          (long) (512 * sizeof (struct GameRec)) );



   if ( hGameList == NULL ) {

      return 1;

   }



   GameList = (struct GameRec far *) GlobalLock (hGameList);



   hTTable = GlobalAlloc ( GMEM_MOVEABLE | GMEM_ZEROINIT,

                          (long) (2*ttblsz * sizeof (struct hashentry)) );



   if ( hTTable == NULL ) {

      return 1;

   }



   ttable = (struct hashentry far *) GlobalLock (hTTable);



  Level = 0;

  TCflag = false;

  OperatorTime = 0;

  Initialize ();

  Initialize_dist ();

  Initialize_moves ();

  NewGame (hWnd);

  GetOpenings ( hWnd);



  flag.easy = ahead;

  flag.hash = hash;

  hashfile = NULL;

  return (0);

}





void FreeGlobals (void)

{

   if ( hHistory ) {

      GlobalUnlock (hHistory);

      GlobalFree   (hHistory);

   }



   if ( hnextdir ) {

      GlobalUnlock (hnextdir);

      GlobalFree   (hnextdir);

   }



   if ( hGameList ) {

      GlobalUnlock (hGameList);

      GlobalFree   (hGameList);

   }



   if ( hTTable ) {

      GlobalUnlock (hTTable);

      GlobalFree   (hTTable);

   }



   if ( hTree ) {

      GlobalUnlock (hTree);

      GlobalFree   (hTree);

   }



   if ( hHashCode ) {

      GlobalUnlock (hHashCode);

      GlobalFree   (hHashCode);

   }



   if ( htaxidata ) {

      GlobalUnlock (htaxidata);

      GlobalFree   (htaxidata);

   }



   if ( hdistdata ) {

      GlobalUnlock (hdistdata);

      GlobalFree   (hdistdata);

   }



   if ( hnextpos ) {

      GlobalUnlock (hnextpos);

      GlobalFree   (hnextpos);

   }



}





void

NewGame (HWND hWnd)



/*

  Reset the board and other variables to start a new game.

*/



{

  short l, c, p;



  stage = stage2 = -1;          /* the game is not yet started */



  if ( flag.post ) {

      SendMessage ( hStats, WM_SYSCOMMAND, SC_CLOSE, 0 );

      flag.post = false;

  }



  flag.mate = flag.quit = flag.reverse = flag.bothsides = false;

  flag.force = false;

  flag.hash = flag.easy = flag.beep = flag.rcptr = true;

  NodeCnt = et0 = epsquare = 0;

  dither = 0;

  Awindow = 90;

  Bwindow = 90;

  xwndw = 90;

  MaxSearchDepth = 29;

  contempt = 0;

  GameCnt = 0;

  Game50 = 1;

  hint = 0x0C14;

  ZeroRPT ();

  Developed[white] = Developed[black] = false;

  castld[white] = castld[black] = false;

  PawnThreat[0] = CptrFlag[0] = false;

  Pscore[0] = 12000;

  Tscore[0] = 12000;

  opponent = white;

  computer = black;

  for (l = 0; l < 2000; l++)

/*    Tree[l].f = Tree[l].t = 0;*/

    (Tree+l)->f = (Tree+l)->t = 0;

#if ttblsz

  rehash = 6;

  ZeroTTable ();

  srand ((unsigned int) 1);

  for (c = white; c <= black; c++)

    for (p = pawn; p <= king; p++)

      for (l = 0; l < 64; l++)

        {

          (hashcode+c*7*64+p*64+l)->key = (((unsigned long) urand ()));

          (hashcode+c*7*64+p*64+l)->key += (((unsigned long) urand ()) << 16);

          (hashcode+c*7*64+p*64+l)->bd = (((unsigned long) urand ()));

          (hashcode+c*7*64+p*64+l)->bd += (((unsigned long) urand ()) << 16);

        }

#endif /* ttblsz */

  for (l = 0; l < 64; l++)

    {

      board[l] = Stboard[l];

      color[l] = Stcolor[l];

      Mvboard[l] = 0;

    }

  if (TCflag)

    SetTimeControl ();

  else if (Level == 0) {

    OperatorTime = 1;

    TCmoves = 60;

    TCminutes = 5;

    TCflag = (TCmoves > 1);

    SetTimeControl ();

  }

  InitializeStats ();

  time0 = time ((long *) 0);

  ElapsedTime (1);

  UpdateDisplay (hWnd, 0, 0, 1, 0);

}



