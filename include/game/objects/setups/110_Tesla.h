#ifndef _SYS_GAME_OBJECTS_SETUPS_TESLA_H
#define _SYS_GAME_OBJECTS_SETUPS_TESLA_H

#include "game/objects/object.h"

// ctrl: 110 | size: 0x24
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
    u8 damageStrength;
} Tesla_Setup;

#endif
