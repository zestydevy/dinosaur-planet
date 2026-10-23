#ifndef _SYS_GAME_OBJECTS_SETUPS_114_H
#define _SYS_GAME_OBJECTS_SETUPS_114_H

#include "game/objects/object.h"

// ctrl: 114 | size: 0x28
typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    u16 particleID;
    s16 unk22;
    s16 gamebit;
} Ctrl114_Setup;

#endif
