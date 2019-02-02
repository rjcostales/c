/*

  C source for GNU CHESS



  Revision: 1990-12-27



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

#include "defs.h"

#include "chess.h"



extern char mvstr[4][6];



BOOL FAR PASCAL ReviewDlgProc ( HWND hDlg, unsigned message,

                               WORD wParam, LONG lParam);





int ReviewDialog ( HWND hWnd, HANDLE hInst)

{

   FARPROC lpProcReview;

   int status;



   lpProcReview = MakeProcInstance(ReviewDlgProc, hInst);

	status = DialogBoxParam (hInst, MAKEINTRESOURCE(REVIEW),   hWnd,  lpProcReview, 0);

   FreeProcInstance(lpProcReview);

   return status;

}



BOOL FAR PASCAL ReviewDlgProc ( HWND hDlg, unsigned message,

                               WORD wParam, LONG lParam)

{



   int i,f,t;

   char tmp[50];



   switch (message) {

	   case WM_INITDIALOG:		   /* message: initialize dialog box */



        for (i = 1; i <= GameCnt; i++) {

            f = GameList[i].gmove >> 8;

            t = (GameList[i].gmove & 0xFF);

            algbr (f, t, false);

            wsprintf (tmp, "%4d-%c\t%5s\t%-5d\t%-2d\t%-5d",

               (i+1)/2, (i%2 ? 'w' : 'b'),

               (char far *)mvstr[0],

               GameList[i].score, GameList[i].depth,

               GameList[i].time);

            SendDlgItemMessage (hDlg, 100, LB_ADDSTRING, 0, (LONG) (LPSTR) tmp);

         }

         SendDlgItemMessage (hDlg, 100, WM_SETREDRAW, TRUE, 0);

         return (TRUE);



      case WM_SYSCOMMAND:

         if ( (wParam&0xfff0) == SC_CLOSE ) {

   		      EndDialog(hDlg, NULL);

	   	      return TRUE;

         }

         break;





	   case WM_COMMAND:		      /* message: received a command */

         switch (wParam) {

         

            case IDOK:

		         EndDialog(hDlg, 1);

		         return TRUE;

               break;



         }

	      break;

    }



    return (FALSE);			      /* Didn't process a message    */

}

