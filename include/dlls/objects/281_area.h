#ifndef _DLLS_281_H
#define _DLLS_281_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    u16 value;      //Passed to other objects when inside the Area object's volume, via func_80031BBC
    u16 halfWidth;
    u16 halfHeight;
    u16 halfDepth;
    u8 yaw;
    u8 pitch;
} Area_Setup;

#endif // _DLLS_281_H
