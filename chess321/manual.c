/*

  C source for GNU CHESS



  Revision: 1991-01-18



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

#include "chess.h"





static char  ManualDlgChar[8];



int FAR DoManualMoveDlg (HANDLE hInst, HWND hWnd, char * szPrompt);



BOOL FAR PASCAL ManualMoveDlgProc (HWND hDlg, unsigned iMessage,

                                  WORD wParam, LONG lParam)

{

   switch ( iMessage ){

      case WM_INITDIALOG:

         SendMessage ( GetDlgItem(hDlg, 100), EM_LIMITTEXT, sizeof(ManualDlgChar)-1,0);

         SetDlgItemText (hDlg, 100, (LPSTR) ManualDlgChar);

         return TRUE;



      case WM_SYSCOMMAND:

         if ( (wParam&0xfff0) == SC_CLOSE ) {

   		      EndDialog(hDlg, FALSE);

	   	      return TRUE;

         }

         return FALSE;

         break;



      case WM_COMMAND:

         switch (wParam) {

            case IDOK:

               GetDlgItemText ( hDlg, 100, ManualDlgChar, sizeof(ManualDlgChar)-1); 

               EndDialog ( hDlg, TRUE);

               break;



            case IDCANCEL:

               EndDialog ( hDlg, FALSE);

               break;



            default: return FALSE;



         }



      default: return FALSE;

   }

   return TRUE;

}



int FAR DoManualMoveDlg (HANDLE hInst, HWND hWnd, char * szPrompt)

{

   FARPROC lpfnManualDlgProc;

   int stat;



   lstrcpy ( ManualDlgChar, "");



   lpfnManualDlgProc = MakeProcInstance ( ManualMoveDlgProc, hInst);

   stat = DialogBox ( hInst, MAKEINTRESOURCE(MANUALDLG), hWnd, lpfnManualDlgProc);

   FreeProcInstance ( lpfnManualDlgProc);



   lstrcpy ( szPrompt, ManualDlgChar);

   return stat;

}





