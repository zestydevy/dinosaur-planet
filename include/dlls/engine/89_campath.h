#ifndef _DLLS_89_H
#define _DLLS_89_H

#include "PR/ultratypes.h"

typedef struct {
    s32 pathID;
    u8 previousCameraEasesIn;
} CamPath_Params;

typedef enum {
    CamPath_FLAG_Aim_Yaw_at_Player = 1,   //Yaw aims at player, otherwise spline interpolated yaw is used
    CamPath_FLAG_Aim_Pitch_at_Player = 2, //Pitch aims at player, in addition to using spline interpolated pitch
    CamPath_FLAG_Use_Player_Roll = 4      //Player's roll value is used, otherwise spline interpolated roll is used
} CamPath_Flags;

#endif
