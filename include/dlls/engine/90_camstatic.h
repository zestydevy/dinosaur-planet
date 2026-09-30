#ifndef _DLLS_90_H
#define _DLLS_90_H

#include "PR/ultratypes.h"

typedef struct {
    s32 cameraID;   //The cameraID tag to search for in StaticCamera objects' setup structs
    u8 previousCameraEasesIn; //Don't use the StaticCamera's own easing function for the initial ease in
} CamStatic_Params;

typedef enum {
    CamStatic_FLAG_Aim_Yaw_at_Player = 1,   //Yaw aims at player, otherwise StaticCamera's own fixed yaw is used
    CamStatic_FLAG_Aim_Pitch_at_Player = 2, //Pitch aims at player, otherwise StaticCamera's own fixed pitch is used
    CamStatic_FLAG_Use_Player_Roll = 4      //Player's roll value is used, otherwise StaticCamera's own fixed roll is used
} CamStatic_Flags;

//For use in Trigger Object CameraAction commands
#define CamStatic_PreviousCameraEasesIn 0x80

#endif
