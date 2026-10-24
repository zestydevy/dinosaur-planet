#ifndef _SYS_GAME_OBJECTS_SETUPS_DIMLAVABALLGENERATOR_H
#define _SYS_GAME_OBJECTS_SETUPS_DIMLAVABALLGENERATOR_H

#include "game/objects/object.h"

// ctrl: 73 | size: 0x28
typedef struct {
    ObjSetup base;
    s16 timerMax;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 index;
    s16 gamebit1;
    u8 unk20;
    u8 unk21;
    s16 gamebit2;
    s16 gamebit3;
} DIMLavaBallGenerator_Setup;

#endif
