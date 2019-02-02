/*

  C source for GNU CHESS



  Revision: 1990-09-30



  Modified by Daryl Baker for use in MS WINDOWS environment



  Based on Ideas and code segments of Charles Petzold from artices in

  MicroSoft Systems Journal.



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



#include "chess.h"

#include "defs.h"



extern struct PIECEBITMAP pieces[6];



#define PIECE_XAXIS 32

#define PIECE_YAXIS 32



static void _near QuerySqCenter ( short x, short y, POINT *pptl);

static short _near ConvertCoordToIndex ( short x, short y);

static void _near PieceOriginFromCenter ( POINT *pptl);

static void _near QuerySqPieceOrigin ( short x, short y, POINT *pptl);

static void _near DrawOnePiece ( HDC hDC, short x, short y, struct PIECEBITMAP *piece, DWORD color);

static void _near ShowPiece ( HDC hDC, POINT *pptl, struct PIECEBITMAP *Piece_bitmap,

                  DWORD Color );



static void _near QuerySqCenter ( short x, short y, POINT *pptl)

{

   POINT aptl[4];



   QuerySqCoords ( x, y, aptl );



   pptl->x = (aptl[0].x + aptl[1].x + aptl[2].x + aptl[3].x)/4;

   pptl->y = (aptl[0].y + aptl[2].y)/2;

}





static void _near PieceOriginFromCenter ( POINT *pptl)

{

   pptl->x -= PIECE_XAXIS / 2;

   pptl->y -= PIECE_YAXIS / 2;

}



static void _near QuerySqPieceOrigin ( short x, short y, POINT *pptl)

{

      QuerySqCenter ( x, y, pptl);

      PieceOriginFromCenter (pptl);

}





/*

   Draw a piece in the specificed point



   Piece_bitmap is a structure with the handles for the mask,

   outline and piece.



*/



static void _near ShowPiece ( HDC hDC, POINT *pptl, struct PIECEBITMAP *Piece_bitmap,

                  DWORD Color )

{

   HDC hMemDC;

   HBRUSH hBrush, hOldBrush;

   HPEN hOldPen;





   hOldBrush = SelectObject ( hDC, GetStockObject (BLACK_BRUSH) );

   hOldPen = SelectObject ( hDC, GetStockObject ( BLACK_PEN) );

   

   hMemDC = CreateCompatibleDC ( hDC);



   /* Write the mask to clear the space */

   SelectObject ( hMemDC, Piece_bitmap->mask);

   BitBlt ( hDC, pptl->x, pptl->y, PIECE_XAXIS, PIECE_YAXIS, hMemDC, 0, 0,SRCAND);



   /* Write out the piece with an OR */



   hBrush = CreateSolidBrush ( Color );

   SelectObject ( hDC, hBrush );



   SelectObject ( hMemDC, Piece_bitmap->piece);

   BitBlt ( hDC, pptl->x, pptl->y, PIECE_XAXIS, PIECE_YAXIS, hMemDC, 0, 0,0xB80746L);



   /* The draw the outline */



   SelectObject ( hDC, GetStockObject ( BLACK_BRUSH ) );

   SelectObject ( hMemDC, Piece_bitmap->outline);

   BitBlt ( hDC, pptl->x, pptl->y, PIECE_XAXIS, PIECE_YAXIS, hMemDC, 0, 0, 0xB80746L);





   SelectObject ( hDC, hOldBrush );

   SelectObject ( hDC, hOldPen );

   DeleteObject ( hBrush);



   if ( DeleteDC(hMemDC)==0) MessageBeep(0);

}



static short _near ConvertCoordToIndex ( short x, short y)

{

   return (y*8 + x );

}



static void _near DrawOnePiece ( HDC hDC, short x, short y, struct PIECEBITMAP *piece, DWORD color)

{

   POINT origin;



   QuerySqPieceOrigin ( x, y, &origin );

   ShowPiece ( hDC, &origin, piece, color);



}







void DrawAllPieces ( HDC hDC,  int reverse, short *pbrd, short *color,

                     DWORD clrblack, DWORD clrwhite )

{

   short x,y;

   short i;



   for ( y=0; y<8; y++) {

      for (x=0; x<8; x++) {

         i = ConvertCoordToIndex (x, y);

         if ( *(color+i) != NETURAL ) {

            if (reverse == 0)

               DrawOnePiece ( hDC, x, y, pieces+*(pbrd+i), (*(color+i)==BLACK) ? clrblack : clrwhite );

            else

               DrawOnePiece ( hDC, 7-x, 7-y, pieces+*(pbrd+i), (*(color+i)==BLACK) ? clrblack : clrwhite );

         }

      }

   }

}



