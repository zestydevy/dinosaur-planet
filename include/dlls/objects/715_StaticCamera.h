#ifndef _DLLS_715_H
#define _DLLS_715_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    u8 cameraID; //An identifier for selecting a specific camera (Trigger Objects' CameraAction params need to match this)
    u8 unk19;
    u8 fov;
    u8 flags;   //1: aim yaw at player, 2: aim pitch at player, 4: use player roll 
    s16 yaw;
    s16 pitch;
    s16 roll;
} StaticCamera_Setup;

#endif
