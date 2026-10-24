#ifndef _SYS_GAME_OBJECTS_SETUPS_SPITEM_H
#define _SYS_GAME_OBJECTS_SETUPS_SPITEM_H

#include "game/objects/object.h"

// ctrl: 124 | size: 0x1c
typedef struct {
    ObjSetup base;
    u8 modelIndex;
    u8 itemIndex;
    u8 yaw;
    u8 pitch;
} SPItem_Setup;

#endif
