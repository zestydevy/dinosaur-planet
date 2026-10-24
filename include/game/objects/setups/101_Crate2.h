#ifndef _SYS_GAME_OBJECTS_SETUPS_CRATE2_H
#define _SYS_GAME_OBJECTS_SETUPS_CRATE2_H

#include "game/objects/object.h"

// ctrl: 101 | size: 0x28
typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    s16 unk1C;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 _unk23[0x28 - 0x23];
} Crate2_Setup;

#endif
