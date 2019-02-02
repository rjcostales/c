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

#define NOCOMM

#define NOKANJI



#include <windows.h>

#include <time.h>

#include <sys\types.h>

#include <sys\stat.h>



extern char Version[];



BOOL FAR PASCAL About(HWND hDlg, unsigned message, WORD wParam, LONG lParam)

{

   

   switch (message) {



      case WM_INITDIALOG:		   /* message: initialize dialog box */

         SetDlgItemText ( hDlg, 106, Version);

         return (TRUE);



      case WM_SYSCOMMAND:

         if ( (wParam&0xfff0) == SC_CLOSE ) {

   		      EndDialog(hDlg, NULL);

	   	      return TRUE;

         }

         break;





	   case WM_COMMAND:		      /* message: received a command */

	      if (wParam == IDOK) {

		      EndDialog(hDlg, NULL);

		      return (TRUE);

	      }

	      break;

    }



    return (FALSE);			      /* Didn't process a message    */

}

