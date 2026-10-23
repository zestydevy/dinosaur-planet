#ifndef _SYS_GAME_OBJECTS_SETUPS_CCGRUB_H
#define _SYS_GAME_OBJECTS_SETUPS_CCGRUB_H

#include "game/objects/object.h"

// ctrl: 121 | size: 0x1c
typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 rangeInnerSq;
    u8 rangeOuterSq;
} CCgrub_Setup;

#endif
