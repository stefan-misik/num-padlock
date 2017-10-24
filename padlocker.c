#include "padlocker.h"

/******************************************************************************/
/*                                Public                                      */
/******************************************************************************/

/******************************************************************************/
VOID PadlockerInit(
    LPPADLOCKDATA lpPD
)
{
    lpPD->bIsEnabled = FALSE;
    lpPD->uPeriod = 500; /* ms */
    lpPD->bNumLockIsOn = TRUE;
    lpPD->hwndReceiver = NULL;
    lpPD->nIDEvent = 0;
}

/******************************************************************************/
BOOL PadlockerProcess(
    const LPPADLOCKDATA lpPD
)
{
    /* Check if numlock status */
    if ((lpPD->bNumLockIsOn ? 0 : 1) == (1 & GetKeyState(VK_NUMLOCK)))
    {
        INPUT in[2];
        
        /* Prepare input performing pressing and releasing the numlock  key */
        in[0].type = INPUT_KEYBOARD;
        in[0].ki.wVk = VK_NUMLOCK;
        in[0].ki.wScan = 0;
        in[0].ki.dwFlags = 0;
        in[0].ki.time = 0;
        in[0].ki.dwExtraInfo = (ULONG_PTR)NULL;
        in[1] = in[0];
        in[1].ki.dwFlags |= KEYEVENTF_KEYUP;
        
        /* Simulate key press */
        SendInput(2, in, sizeof(INPUT));
    }
    
    return TRUE;
}

/******************************************************************************/
BOOL PadlockerUpdate(
    const LPPADLOCKDATA lpPD,
    HWND hwndReceiver,
    UINT_PTR nIDEvent
)
{
    if(lpPD->bIsEnabled)
    {
        SetTimer(hwndReceiver, nIDEvent, lpPD->uPeriod, NULL);

        /* Store timer info */
        lpPD->hwndReceiver = hwndReceiver;
        lpPD->nIDEvent = nIDEvent;
    }
    else if (0 != lpPD->nIDEvent)
    {
        KillTimer(lpPD->hwndReceiver, lpPD->nIDEvent);

        /* Set timer ID to zero to signify the timer is not running */
        lpPD->nIDEvent = 0;
    }

    return TRUE;
}