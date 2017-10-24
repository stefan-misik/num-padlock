#ifndef PADLOCKER_H
#define PADLOCKER_H

#include "win_common.h"

/**
 * @brief Num-padlock data 
 */
typedef struct tagPADLOCKDATA
{
    BOOL bIsEnabled;    /** < Enable num-padlock */
    UINT uPeriod;       /** < Check period length */
    BOOL bNumLockIsOn;  /** < Desired num lock status */
    HWND hwndReceiver;  /** < Window to receive WM_TIMER messages */
    UINT_PTR nIDEvent;  /** < A nonzero timer identifier (0 means no timer is
                         *    not running) */
} PADLOCKDATA, *LPPADLOCKDATA;

/**
 * @breif Initialize padlocker data structure
 * @param lpPD
 */
VOID PadlockerInit(
    LPPADLOCKDATA lpPD
);

/**
 * @brief Perform operations to preserve desired state
 * 
 * @param[in] lpPD Pointer to padlocker data
 * 
 * @return TRUE on success
 */
BOOL PadlockerProcess(
    const LPPADLOCKDATA lpPD
);

/**
 * @brief Update internal state of padlocker
 * 
 * @param[in] lpPD Pointer to padlocker data
 * @param     hwndReceiver Window to receive WM_TIMER messages
 * @param     nIDEvent A nonzero timer identifier
 * 
 * @return TRUE on success
 */
BOOL PadlockerUpdate(
    const LPPADLOCKDATA lpPD,
    HWND hwndReceiver,
    UINT_PTR nIDEvent
);

#endif /* PADLOCKER_H */

