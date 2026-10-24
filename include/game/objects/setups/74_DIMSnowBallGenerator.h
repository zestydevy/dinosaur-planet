#ifndef _SYS_GAME_OBJECTS_SETUPS_DIMSNOWBALLGENERATOR_H
#define _SYS_GAME_OBJECTS_SETUPS_DIMSNOWBALLGENERATOR_H

#include "game/objects/object.h"

// ctrl: 74 | size: 0x20
typedef struct {
    ObjSetup base;
    s16 snowballInterval;   //How many frames to wait between creating snowballs
    u8 unk1A;
    u8 unk1B;
    u8 yaw;
} DIMSnowBallGenerator_Setup;

#endif
