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

#include <time.h>



#include "gnuchess.h"

#include "chess.h"

#include "defs.h"



BOOL FAR PASCAL TestDlgProc ( HWND hDlg, unsigned message,

                               WORD wParam, LONG lParam);





int TestDialog ( HWND hWnd, HANDLE hInst)

{

   FARPROC lpProcTest;

   int status;



   lpProcTest = MakeProcInstance(TestDlgProc, hInst);

	status = DialogBox (hInst, MAKEINTRESOURCE(TEST),   hWnd,  lpProcTest);

   FreeProcInstance(lpProcTest);

   return status;

}



void

TestSpeed(HWND hWnd, int cnt, void (*f) (short int side, short int ply))

{

  short i;

  long t1, t2, evrate;

  char tmp[40];



  t1 = time (0);

  for (i = 0; i < 10000; i++)

    {

      f (opponent, 2);

    }

  t2 = time (0);

  NodeCnt = 10000L * (TrPnt[3] - TrPnt[2]);

  evrate = NodeCnt / (t2 - t1);



  wsprintf ( tmp, "Nodes= %8ld, Nodes/Sec= %5ld", NodeCnt, evrate);

  SetDlgItemText (hWnd, cnt, tmp);

}



BOOL FAR PASCAL TestDlgProc ( HWND hDlg, unsigned message,

                               WORD wParam, LONG lParam)

{



   HCURSOR hCursor;



   switch (message) {

	   case WM_INITDIALOG:		   /* message: initialize dialog box */



         SetDlgItemText ( hDlg, 100, " ");

         SetDlgItemText ( hDlg, 101, " ");

         PostMessage ( hDlg, (WM_USER+1), 0, 0);

         return (TRUE);



      case (WM_USER+1):

          hCursor = SetCursor ( LoadCursor(NULL, IDC_WAIT) );

          ShowCursor (TRUE);

          TestSpeed (hDlg, 100, MoveList);

          TestSpeed (hDlg, 101, CaptureList);

          ShowCursor (FALSE);

          SetCursor ( hCursor);

         break;



      case WM_SYSCOMMAND:

         if ( (wParam&0xfff0) == SC_CLOSE ) {

   		      EndDialog(hDlg, NULL);

	   	      return TRUE;

         }

         break;

    }



    return (FALSE);			      /* Didn't process a message    */

}

