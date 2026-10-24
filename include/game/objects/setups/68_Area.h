#ifndef _SYS_GAME_OBJECTS_SETUPS_AREA_H
#define _SYS_GAME_OBJECTS_SETUPS_AREA_H

#include "game/objects/object.h"

// ctrl: 68 | size: ?
typedef struct {
    ObjSetup base;
    u16 value;      //Passed to other objects when inside the Area object's volume, via objGetAreaValueAtPoint
    u16 halfWidth;
    u16 halfHeight;
    u16 halfDepth;
    u8 yaw;
    u8 pitch;
} Area_Setup;

#endif
