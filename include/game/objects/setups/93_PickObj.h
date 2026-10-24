#ifndef _SYS_GAME_OBJECTS_SETUPS_PICKOBJ_H
#define _SYS_GAME_OBJECTS_SETUPS_PICKOBJ_H

#include "game/objects/object.h"

// ctrl: 93 | size: 0x24
typedef struct {
    ObjSetup base;
    u8 yaw;
    u8 modelIdx;
    s16 pickupParam;
    u8 _unk1C[0x24 - 0x1C];
} PickObj_Setup;

#endif
