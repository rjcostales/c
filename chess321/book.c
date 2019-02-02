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

#define NOCOMM

#define NOKANJI

 

#include <windows.h>

#include <time.h>

#include <stdlib.h>

#include <stdio.h>



#include "gnuchess.h"

#include "defs.h"

#include "chess.h"



#define MAX_BOOK_SIZE (32*1024)



static unsigned int book_used = 0;

static char far * xBook;

GLOBALHANDLE hBook = 0;



void FreeBook (void)

{

   GlobalUnlock ( hBook );

   GlobalFree ( hBook);

   hBook = 0;

   book_used = 0;

}



static void far *Book_alloc ( unsigned int size )

{

    char far * temp;

    if ( book_used+size >= MAX_BOOK_SIZE ) return (void far *)0;

    temp = xBook+book_used;

    book_used += size;

    return temp;

}





void

GetOpenings (HWND hWnd)

     

/*

   Read in the Opening Book file and parse the algebraic notation for a move

   into an unsigned integer format indicating the from and to square. Create

   a linked list of opening lines of play, with entry->next pointing to the

   next line and entry->move pointing to a chunk of memory containing the

   moves. More Opening lines of up to 256 half moves may be added to

   gnuchess.book.

*/

#ifndef BOOK

#define BOOK "/usr/games/lib/gnuchess.book"

#endif /* BOOK */     

{

  FILE *fd;

  int c, i, j, side;



  /* char buffr[2048]; */

  struct BookEntry far  *entry;

  unsigned short mv, far  *mp, tmp[100];

  char lpFile[_MAX_FNAME+_MAX_EXT+_MAX_DRIVE+_MAX_DIR+1];

  char sFname[_MAX_FNAME], sExt[_MAX_EXT], sDrive[_MAX_DRIVE], sDir[_MAX_DIR];



  GetModuleFileName ( GetWindowWord(hWnd, GWW_HINSTANCE),

                      lpFile, sizeof(lpFile) );



  _splitpath ( lpFile, sDrive, sDir, sFname, sExt);

  _makepath  ( lpFile, sDrive, sDir, "gnuchess", "boo");



  fd = fopen (lpFile, "r");



/*  if ((fd = fopen (BOOK, "r")) == NULL)

    fd = fopen ("gnuchess.book", "r"); */

  if (fd != NULL)

    {

      hBook = GlobalAlloc ( GMEM_MOVEABLE | GMEM_ZEROINIT,

                            (long) (MAX_BOOK_SIZE * sizeof (char)) );



      if( hBook == NULL )

        {

           Book = NULL;

           SMessageBox (hWnd, IDS_OBAE, IDS_CHESS);

           return;

        }

      xBook = (unsigned char far *) GlobalLock (hBook);



      Book = NULL;

      i = 0;

      side = white;

      while ((c = parse (fd, &mv, side)) >= 0)

        if (c == 1)

          {

            tmp[++i] = mv;

            side = otherside[side];

          }

        else if (c == 0 && i > 0)

          {

            entry = (struct BookEntry far *) Book_alloc ( sizeof (struct BookEntry));

            mp = (unsigned short far *) Book_alloc ( (i + 1) * sizeof (unsigned short));

            if ( (entry == 0 ) || (mp == 0) )

              {

                Book = NULL;

                SMessageBox (hWnd, IDS_OBAE, IDS_CHESS);

                GlobalUnlock ( hBook );

                GlobalFree ( hBook);

                return;

              }

            entry->mv = mp;

            entry->next = Book;

            Book = entry;

            for (j = 1; j <= i; j++)

              *(mp++) = tmp[j];

            *mp = 0;

            i = 0;

            side = white;

          }



      fclose (fd);

    }

  else

    SMessageBox (hWnd, IDS_OBNF, IDS_CHESS);

}



void

OpeningBook (unsigned short *hint)



/*

  Go thru each of the opening lines of play and check for a match with the

  current game listing. If a match occurs, generate a random number. If this

  number is the largest generated so far then the next move in this line

  becomes the current "candidate". After all lines are checked, the

  candidate move is put at the top of the Tree[] array and will be played by

  the program. Note that the program does not handle book transpositions.

*/



{

  short j, pnt;

  unsigned short m, far *mp;

  unsigned r, r0;

  struct BookEntry far *p;



  srand ((unsigned int) time ((long *) 0));

  r0 = m = 0;

  p = Book;

  while (p != NULL)

    {

      mp = p->mv;

      for (j = 1; j <= GameCnt; j++)

        if (GameList[j].gmove != *(mp++))

          break;

      if (j > GameCnt)

        if ((r = urand ()) > r0)

          {

            r0 = r;

            m = *mp;

            *hint = *(++mp);

          }

      p = p->next;

    }



  for (pnt = TrPnt[1]; pnt < TrPnt[2]; pnt++)

    if (((Tree[pnt].f << 8) | Tree[pnt].t) == m)

      Tree[pnt].score = 0;

  pick (TrPnt[1], TrPnt[2] - 1);

  if (Tree[TrPnt[1]].score < 0) {

   FreeBook ();

   Book = NULL;

  }

}

