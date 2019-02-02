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

#define NOREGION

#define NOSOUND

#define NOWH

#define NOWINOFFSETS

#define NOCOMM

#define NOKANJI



#include <windows.h>

#include "gnuchess.h"

#include "chess.h"



extern int coords;



void Init_Menus (HWND hWnd, WORD wParam, LONG lParam )

{

   WORD Status;

        if ( lParam == MENU_ID_FILE )  {

            EnableMenuItem ( wParam, MSG_CHESS_SAVE,

                             (GameCnt>0 ? MF_ENABLED : MF_GRAYED) );

            EnableMenuItem ( wParam, MSG_CHESS_LIST,

                             (GameCnt>0 ? MF_ENABLED : MF_GRAYED) );



        } else if ( lParam == MENU_ID_EDIT ) {       /*Edit Menu */

            EnableMenuItem ( wParam, MSG_CHESS_REVIEW,

                             (GameCnt>0 ? MF_ENABLED : MF_GRAYED) );

            EnableMenuItem ( wParam, MSG_CHESS_UNDO,

                             (GameCnt>0 ? MF_ENABLED : MF_GRAYED) );

            EnableMenuItem ( wParam, MSG_CHESS_REMOVE,

                             (GameCnt>1 ? MF_ENABLED : MF_GRAYED) );

            if ( flag.force == false)  Status = MF_UNCHECKED; else Status = MF_CHECKED;

            CheckMenuItem ( wParam, MSG_CHESS_FORCE, Status);



        } else if ( lParam == MENU_ID_OPTIONS ){  /* Options Menu */

            if ( flag.hash == false)  Status = MF_UNCHECKED; else Status = MF_CHECKED;

            CheckMenuItem ( wParam, MSG_CHESS_HASH, Status);



            if ( coords == false)  Status = MF_UNCHECKED; else Status = MF_CHECKED;

            CheckMenuItem ( wParam, MSG_CHESS_COORD, Status);



            if ( flag.beep == false)  Status = MF_UNCHECKED; else Status = MF_CHECKED;

            CheckMenuItem ( wParam, MSG_CHESS_BEEP, Status);



            if ( flag.bothsides == false)  Status = MF_UNCHECKED; else Status = MF_CHECKED;

            CheckMenuItem ( wParam, MSG_CHESS_BOTH, Status);



            if ( Book == NULL)  Status = MF_UNCHECKED; else Status = MF_CHECKED;

            CheckMenuItem ( wParam, MSG_CHESS_BOOK, Status);



            if ( flag.post == false)  Status = MF_UNCHECKED; else Status = MF_CHECKED;

            CheckMenuItem ( wParam, MSG_CHESS_POST, Status);

        } else if (lParam == MENU_ID_SKILL) {

           if ( dither == 0)

                  Status = MF_UNCHECKED; else Status = MF_CHECKED;

            CheckMenuItem ( wParam, MSG_CHESS_RANDOM, Status);

  

            if ( flag.easy == false)

                  Status = MF_UNCHECKED; else Status = MF_CHECKED;

            CheckMenuItem ( wParam, MSG_CHESS_EASY, Status);



         } else if ( lParam == MENU_ID_SIDE) {

            if ( flag.reverse == false)

                  Status = MF_UNCHECKED; else Status = MF_CHECKED;

            CheckMenuItem ( wParam, MSG_CHESS_REVERSE, Status);

  

            if ( computer == black ){

               CheckMenuItem ( wParam, MSG_CHESS_BLACK, MF_CHECKED);

               CheckMenuItem ( wParam, MSG_CHESS_WHITE, MF_UNCHECKED);

            } else {

               CheckMenuItem ( wParam, MSG_CHESS_WHITE, MF_CHECKED);

               CheckMenuItem ( wParam, MSG_CHESS_BLACK, MF_UNCHECKED);

            }



         }



}

