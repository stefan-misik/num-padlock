#ifndef PADLOCKER_H
#define PADLOCKER_H

#include "win_common.h"

/**
 * @brief Num-padlock data 
 */
typedef struct tagPADLOCKDATA
{    
    HHOOK hKeybHook;    /** < Keyboard hook */
} PADLOCKDATA, *LPPADLOCKDATA;

/**
 * @breif Initialize padlocker data structure
 * 
 * @param[out] lpPD Padlocker data structure to initialize
 */
VOID PadlockerInit(
    LPPADLOCKDATA lpPD
);

/**
 * @brief Enable or disable the padlocker
 * 
 * @param[in,out] lpPD Padlocker data structure
 * @param bIsEnabled TRUE to enable the padlocker
 * 
 * @return TRUE on success
 */
BOOL PadlockerEnable(
    LPPADLOCKDATA lpPD,
    BOOL bEnable
);


#endif /* PADLOCKER_H */

