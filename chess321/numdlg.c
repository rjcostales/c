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

#include <string.h>

#include "chess.h"



#define IDD_INT 0x10

#define IDD_CHAR 0x11



static int   NumberDlgInt;

static char  NumberDlgChar[80];



int FAR DoGetNumberDlg (HANDLE hInst, HWND hWnd, char * szPrompt, int def);



BOOL FAR PASCAL NumberDlgDlgProc (HWND hDlg, unsigned iMessage,

                                  WORD wParam, LONG lParam)

{

   int temp, Ier;

   switch ( iMessage ){

      case WM_INITDIALOG:

         SetDlgItemText (hDlg, IDD_CHAR, (LPSTR) NumberDlgChar);

         SetDlgItemInt ( hDlg, IDD_INT, NumberDlgInt, TRUE);

         return TRUE;



      case WM_COMMAND:

         switch (wParam) {

            case IDOK:

               temp = GetDlgItemInt (hDlg, IDD_INT, &Ier, TRUE);

               if ( Ier != 0 ) {

                  NumberDlgInt = temp;

                  EndDialog ( hDlg, TRUE);

               }

               break;



            case IDCANCEL:

               EndDialog ( hDlg, TRUE);

               break;



            default: return FALSE;



         }



      default: return FALSE;

   }

   return TRUE;

}



int FAR DoGetNumberDlg (HANDLE hInst, HWND hWnd, char * szPrompt, int def)

{

   FARPROC lpfnNumberDlgProc;



   strcpy ( NumberDlgChar, szPrompt);

   NumberDlgInt = def;



   lpfnNumberDlgProc = MakeProcInstance ( NumberDlgDlgProc, hInst);

   DialogBox ( hInst, MAKEINTRESOURCE(NUMBERDLG), hWnd, lpfnNumberDlgProc);

   FreeProcInstance ( lpfnNumberDlgProc);



   return NumberDlgInt;

}

