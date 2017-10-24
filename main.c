#include "win_common.h"
#include "resource.h"
#include "defs.h"
#include "main_wnd.h"


HINSTANCE g_hInstance;
HANDLE g_hHeap;


/******************************************************************************/
INT WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow
	)
{
    MSG msg;
    INITCOMMONCONTROLSEX icc;
    
    /* Initialize common controls */
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES | ICC_TAB_CLASSES |
            ICC_LISTVIEW_CLASSES;
    if(!InitCommonControlsEx(&icc))
        return -1;

    /* Store main instance handle */
	g_hInstance = hInstance;  

    /* Get Process heap */
    g_hHeap = GetProcessHeap();

    CreateMainWindow(nCmdShow);
    
	/* Enter the message loop */
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	ExitProcess((UINT)(msg.wParam));

   return (INT)(msg.wParam);
}
