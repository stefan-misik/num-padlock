#ifndef MAIN_WND_H
#define MAIN_WND_H

#include "win_common.h"

/* Message sent to the main window to inform it of numlock being toggled */
#define WM_NUMLOCK_TOGGLED (WM_USER + 1)

/**
 * @brief The handle to the main window
 * 
 */
extern HWND g_hMainWnd;

/**
 * @brief Creates and accordingly shows main application window
 * 
 * @param nCmdShow How to show the main window
 * @return FALSE on failure
 */
BOOL CreateMainWindow(
    int nCmdShow
);


#endif /* MAIN_WND_H */

