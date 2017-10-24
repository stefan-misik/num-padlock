#include "main_wnd.h"
#include "resource.h"
#include "tray_icon.h"
#include "about_dialog.h"
#include "defs.h"

/******************************************************************************/
/*                               Private                                      */
/******************************************************************************/

/**
 * @brief Main window data
 */
typedef struct tagMAINWNDDATA
{
    HICON hMainIcon;        /** < Main Icon handle */    
    HMENU hTrayIconMenu;    /** < Menu for the tray icon */
} MAINWNDDATA, *LPMAINWNDDATA;

/* Tray icon notification messages  */
#define WM_TRAY_ICON WM_USER + 0

/* ID of the tray icon */
#define TRAY_ICON_ID 0

/* Get Main window data pointer */
#define GetMainWindowData(hWnd) (LPMAINWNDDATA)(GetWindowLongPtr((hWnd), \
                                                GWLP_USERDATA))

/**
 * @brief Show or hide main window
 * 
 * @param hWnd Main window handle
 * @param bShow FALSE to hide the window
 */
static VOID ShowMainWnd(
    HWND hWnd,
    BOOL bShow
)
{
    if(bShow)
    {
        SetForegroundWindow(hWnd);
        ShowWindow(hWnd, SW_SHOW);
    }
    else
    {       
        ShowWindow(hWnd, SW_HIDE);
    }
}
/**
 * @brief Destroy all objects in a Main Window data structure and free memory
 *        taken by this structure
 */
static VOID DestroyMainWndData(
    LPMAINWNDDATA lpData
)
{
    if(NULL != lpData)
    {
        if(NULL != lpData->hMainIcon)
            DestroyIcon(lpData->hMainIcon);
        if(NULL != lpData->hTrayIconMenu)
            DestroyMenu(lpData->hTrayIconMenu);

        HeapFree(g_hHeap, 0, lpData);
    }
}

/**
 * @brief Allocate and initialize main window data structure
 * 
 * @return New Main window data structure pointer
 */
static LPMAINWNDDATA CreateMainWndData(VOID)
{
    LPMAINWNDDATA lpData;

    /* Allocate data structure associated with the main window */
    lpData = HeapAlloc(g_hHeap, 0, sizeof(MAINWNDDATA));
    if(NULL == lpData)
        return NULL;

    /* Initialize main window data */
    lpData->hMainIcon = NULL;
    lpData->hTrayIconMenu = NULL;
        
    return lpData;
}

/******************************************************************************/
/*                         Windows Messages                                   */
/******************************************************************************/

/**
 * @brief Process WM_INITDIALOG message
 * 
 * @param hwndInitialControl A handle to the control to receive the default
 *        keyboard focus
 * @param lpData Additional initialization data
 * @return Set the keyboard focus
 */
static BOOL OnInitDialog(
    HWND hwnd,
    HWND hwndInitialControl,
    LPVOID lpAdditionalData
)
{
    LPMAINWNDDATA lpData = (LPMAINWNDDATA)lpAdditionalData;
    
    /* Store Pointer to the data structure with the window */
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)lpData);

    /* Set icon */
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)lpData->hMainIcon);
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)lpData->hMainIcon);

    /* Add tray icon */
    TrayIconAdd(hwnd, TRAY_ICON_ID, WM_TRAY_ICON, lpData->hMainIcon);
    
    return TRUE;
}

/**
 * @brief called when window is to be closed
 * 
 * @param hwnd Main window handle
 * 
 * @return TRUE if message is processed
 */
static INT_PTR OnClose(
    HWND hwnd
)
{
    /* Hide the window */
    /*ShowMainWnd(hwnd, FALSE);*/
    DestroyWindow(hwnd);

    return TRUE;
}

/**
 * @brief Called when window is to be destroyed
 * 
 * @param hwnd Main window handle
 * 
 * @return TRUE if message is processed
 */
static INT_PTR OnDestroy(
    HWND hwnd
)
{
    LPMAINWNDDATA lpData;

    /* Remove tray icon */
    TrayIconRemove(hwnd, TRAY_ICON_ID);

    /* Destroy data associated with the window */
    lpData = GetMainWindowData(hwnd);
    if(NULL != lpData)
        DestroyMainWndData(lpData);
    
    /* Remove pointer to non-existing data */
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)NULL);

    /* Quit the application */
    PostQuitMessage(0);

    return TRUE;
}

/**
 * @brief Command sent by a control
 * 
 * @param hwnd Main window handle
 * @param wNotifCode Control-specific notification code
 * @param wControlID Dialog Control ID
 * @param hwndC COntrol window handle
 * 
 * @return TRUE if message is processed
 */
static INT_PTR OnControlCommand(
    HWND hwnd,
    WORD wNotifCode,
    WORD wControlID,
    HWND hwndC
)
{
    switch(wControlID)
    {
    case IDOK:
        return TRUE;
    }
    return FALSE;
}

/**
 * @brief Command sent by a menu or an accelerator
 * 
 * @param hwnd Main window handle
 * @param wID Menu item or accelerator ID
 * @param bIsMenu TRUE if sent by menu item
 * 
 * @return TRUE if message is processed
 */
static INT_PTR OnMenuAccCommand(
    HWND hwnd,
    WORD wID,
    BOOL bIsMenu
)
{
    switch(wID)
    {
        case IDM_EXIT:
            DestroyWindow(hwnd);
            return TRUE;

        case IDM_ABOUT:
            ShowAboutDialog(hwnd);
            return TRUE;

        case IDM_SHOWHIDE:
            ShowMainWnd(hwnd, !IsWindowVisible(hwnd));
            return TRUE;
    }
    return FALSE;
}

/**
 * @brief On system command
 * 
 * @param hwnd Main window handle
 * @param wType Type of the system command
 * @param wMouseX Mouse X position, if command chosen by a mouse
 * @param wMouseY Mouse Y position, if command chosen by a mouse
 * 
 * @return TRUE if message is processed
 */
static INT_PTR OnSysCommand(
    HWND hwnd,
    WPARAM wType,
    WORD wMouseX,
    WORD wMouseY
)
{
    switch(wType)
    {
        case SC_MINIMIZE:
            /* Hide the window instead of minimizing it */
            ShowMainWnd(hwnd, FALSE);
            return TRUE;
    }

    return FALSE;
}

/**
 * @brief On Tray icon notification
 * 
 * @param hwnd Main window handle
 * @param wID Tray Icon ID
 * @param uMsg Mouse or keyboard message associated with the event
 * @return TRUE if message is processed
 */
static INT_PTR OnTrayIconNotify(
    HWND hwnd,
    WPARAM wID,
    UINT uMsg
)
{
    LPMAINWNDDATA lpData = GetMainWindowData(hwnd);

    switch(uMsg)
    {
    case WM_LBUTTONDBLCLK:
        ShowMainWnd(hwnd, !IsWindowVisible(hwnd));
        return TRUE;

    case WM_RBUTTONUP:
        TrayIconShowPopupMenu(hwnd, lpData->hTrayIconMenu);
        return TRUE;
    }
    return FALSE;
}

/**
 * @brief Main window dialog procedure
 * 
 * @param hwnd Main Window handle
 * @param uMsg Message to process
 * @param wParam WParam
 * @param lParam LParam
 * 
 * @return Depending on message
 */
static INT_PTR CALLBACK DialogProc(
    HWND   hwnd,
    UINT   uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
        return (INT_PTR)OnInitDialog(hwnd, (HWND)wParam, (LPVOID)lParam);
            
    case WM_COMMAND:
        /* Control command */
        if(0 != lParam)
        {
            return (INT_PTR)OnControlCommand(hwnd, HIWORD(wParam),
                    LOWORD(wParam), (HWND)lParam);
        }
        /* Menu or accelerator command */
        else
        {
            return (INT_PTR)OnMenuAccCommand(hwnd, LOWORD(wParam),
                    (HIWORD(wParam) == 0));
        }
        break;

    case WM_SYSCOMMAND:
        return (INT_PTR)OnSysCommand(hwnd, wParam, GET_X_LPARAM(lParam),
            GET_Y_LPARAM(lParam));
        break;

    case WM_CLOSE:
        return (INT_PTR)OnClose(hwnd);
        break;

    case WM_DESTROY:
        return (INT_PTR)OnDestroy(hwnd);
        break;

    case WM_TRAY_ICON:
        return (INT_PTR)OnTrayIconNotify(hwnd, wParam, (UINT)lParam);
        break;
    }
    
    return FALSE;
}


/******************************************************************************/
/*                                Public                                      */
/******************************************************************************/

/******************************************************************************/
HWND g_hMainWnd = NULL;

/******************************************************************************/
BOOL CreateMainWindow(
    int nCmdShow
)
{
    LPMAINWNDDATA lpData;
    
    if(NULL != g_hMainWnd)
    {
        return FALSE;
    }
    
    /* Allocate main window data structure */
    lpData = CreateMainWndData();
    if(NULL == lpData)
    {
        return FALSE;
    }

    /* Load main icon */
    lpData->hMainIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_MAIN));
    if(NULL == lpData->hMainIcon)
    {
        DestroyMainWndData(lpData);
        return FALSE;
    }

    /* Load tray icon menu */
    lpData->hTrayIconMenu = LoadMenu(g_hInstance,
            MAKEINTRESOURCE(IDR_TRAY_MENU));
    if(NULL == lpData->hTrayIconMenu)
    {
        DestroyMainWndData(lpData);
        return FALSE;
    }

    /* Set default tray menu item */
    SetMenuDefaultItem(GetSubMenu(lpData->hTrayIconMenu, 0), IDM_SHOWHIDE,
        FALSE);
    
    /* Create main Window */
	g_hMainWnd = CreateDialogParam(
		g_hInstance,
		MAKEINTRESOURCE(IDD_MAIN_WND),
		NULL,
		DialogProc,
		(LPARAM)lpData
	);
    
    /* Check if window was created*/
    if(NULL == g_hMainWnd)
    {
        return FALSE;
    }

    /* Draw Window */
    ShowWindow(g_hMainWnd, nCmdShow);
    return TRUE;
}
