/*

  C source for GNU CHESS



  Revision: 1990-09-30



  Modified by Daryl Baker for use in MS WINDOWS environment



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

#define NOSOUND

#define NOWH

#define NOCOMM

#define NOKANJI



#include <windows.h>

#include <stdio.h>



#include "defs.h"



static int deltay;

static HRGN hitrgn[8];



void InitHitTest ( void )

{

   POINT ptls[4];

   POINT toppt, botpt;

   int i;

   for (i=0; i<8; i++) {

      QuerySqOrigin ( i, 0, ptls+0 );

      QuerySqOrigin ( i, 8, ptls+1 );

      QuerySqOrigin ( i+1, 8, ptls+2 );

      QuerySqOrigin ( i+1, 0, ptls+3 );

      hitrgn[i] = CreatePolygonRgn ( ptls, 4, WINDING);

   }

      QuerySqOrigin ( 0, 0, &botpt );

      QuerySqOrigin ( 0, 8, &toppt );

      deltay = botpt.y-toppt.y;

 }



void Hittest_Destructor (VOID)

{

   int i;

   for (i=0; i<8; i++)

      DeleteObject ( hitrgn[i] );

}



static int HorzHitTest ( int x, int y)

{

   int i;



   for ( i=0; i<8; i++) {

      if ( PtInRegion ( hitrgn[i],x,y) ) return i;

   }

   return -1;

}



int HitTest ( int x, int y)

{

   int xsq, ysq;

   POINT sq00;



   xsq = HorzHitTest ( x, y );

   if (xsq==-1) return -1;



   QuerySqOrigin ( 0,0, &sq00);



   if ( y > sq00.y ) return -1;

   if ( y < (sq00.y-deltay) ) return -1;



   ysq = 7 - (y - (sq00.y-deltay) ) / (deltay/8);

   return ( ysq*8 + xsq );

}

