#include "padlocker.h"

/******************************************************************************/
/*                               Private                                      */
/******************************************************************************/

/**
 * @brief Low level keyboard hook routine
 * 
 * @param nCode How to process the message
 * @param wParam Keyboard message identifier
 * @param lParam Low level keyboard input level information
 * 
 * @return Result
 */
static LRESULT CALLBACK LowLevelKeyboardProc(
    int    nCode,
    WPARAM wParam,
    LPARAM lParam
)
{
    /* Is it allowed to process the message */
    if(nCode >= HC_ACTION)
    {
        /* Has numlock been pressed */
        if(VK_NUMLOCK == ((LPKBDLLHOOKSTRUCT)lParam)->vkCode)
        {
            /* Block the message */
            return 1;
        }
    }

    /* Forward the message */
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

/******************************************************************************/
/*                                Public                                      */
/******************************************************************************/

/******************************************************************************/
VOID PadlockerInit(
    LPPADLOCKDATA lpPD
)
{
    lpPD->hKeybHook = NULL;
}

/******************************************************************************/
BOOL PadlockerEnable(
    LPPADLOCKDATA lpPD,
    BOOL bEnable
)
{
    if(bEnable)
    {
        /* Install the numlock disable hook */
        lpPD->hKeybHook = SetWindowsHookEx(
            WH_KEYBOARD_LL,
            LowLevelKeyboardProc,
            NULL,
            0
        );
        
        return lpPD->hKeybHook != NULL;
    }
    else
    {
        /* If the hook exists */
        if(NULL != lpPD->hKeybHook)
        {
            /* Unhook it */
            UnhookWindowsHookEx(lpPD->hKeybHook);
            /* Remove the hook handle to indicate there is no hook */
            lpPD->hKeybHook = NULL;
        }
    }

    return TRUE;
}