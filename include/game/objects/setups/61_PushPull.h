#ifndef _SYS_GAME_OBJECTS_SETUPS_PUSHPULL_H
#define _SYS_GAME_OBJECTS_SETUPS_PUSHPULL_H

#include "game/objects/object.h"

// ctrl: 61 | size: 0x24
typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    u8 _unk1C[0x24 - 0x1C];
} PushPull_Setup;

#endif
