#ifndef _SYS_GAME_OBJECTS_SETUPS_CHECKPOINT_H
#define _SYS_GAME_OBJECTS_SETUPS_CHECKPOINT_H

#include "game/objects/object.h"

// ctrl: 5 | size: 0x40
typedef struct {
    ObjSetup base;
    u8 unk18[0x28 - 0x18];
    s8 unk28;
    u8 yaw;
    u8 scale;
    u8 unk2B[0x40 - 0x2B];
} Checkpoint_Setup;

#endif
