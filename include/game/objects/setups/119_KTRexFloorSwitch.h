#ifndef _SYS_GAME_OBJECTS_SETUPS_KTREXFLOORSWITCH_H
#define _SYS_GAME_OBJECTS_SETUPS_KTREXFLOORSWITCH_H

#include "game/objects/object.h"

// ctrl: 119 | size: 0x24
typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    s16 unk1A;
    s16 unk1C;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
} KT_RexFloorSwitch_Setup;

#endif
