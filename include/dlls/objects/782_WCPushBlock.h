#ifndef _DLLS_782_H
#define _DLLS_782_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "types.h"

typedef enum {
    WCPushBlock_MODELIDX_Moon,
    WCPushBlock_MODELIDX_Sun
} WCPushBlock_ModelIndices;

typedef enum {
    WCPushBlock_STATE_0_Initialising,
    WCPushBlock_STATE_1_Pushable,
    WCPushBlock_STATE_2_Moving,
    WCPushBlock_STATE_3_Pushed_to_Bounds,
    WCPushBlock_STATE_4_In_Correct_Spot,
    WCPushBlock_STATE_5_Reappearing,
    WCPushBlock_STATE_6_Puzzle_Complete
} WCPushBlock_States;

#endif // _DLLS_782_H

