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



extern char szAppName[];



long FAR PASCAL ChessWndProc(HWND hWnd, unsigned message,

                             WORD wParam, LONG lParam);



extern DWORD clrBackGround;

extern DWORD clrBlackSquare;

extern DWORD clrWhiteSquare;

extern DWORD clrBlackPiece;

extern DWORD clrWhitePiece;



BOOL FAR ChessInit(HANDLE hInstance)

{

    HANDLE hMemory;			       /* handle to allocated memory */

    PWNDCLASS pWndClass;		       /* structure pointer	     */

    BOOL bSuccess;			       /* RegisterClass() result     */

 

    hMemory = LocalAlloc(LHND, sizeof(WNDCLASS));

    pWndClass = (PWNDCLASS) LocalLock(hMemory);



    pWndClass->style = NULL;

    pWndClass->lpfnWndProc = ChessWndProc;

    pWndClass->hInstance = hInstance;

    pWndClass->hIcon = LoadIcon(hInstance, szAppName);

    pWndClass->hCursor = LoadCursor(NULL, IDC_ARROW);

    pWndClass->hbrBackground = GetStockObject(WHITE_BRUSH);

    pWndClass->lpszMenuName = (LPSTR) szAppName;

    pWndClass->lpszClassName = (LPSTR) szAppName;



    bSuccess = RegisterClass(pWndClass);



    LocalUnlock(hMemory);			    /* Unlocks the memory    */

    LocalFree(hMemory);				    /* Returns it to Windows */



    return (bSuccess);		 /* Returns result of registering the window */

}



