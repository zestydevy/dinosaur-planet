#ifndef _SYS_GAME_OBJECTS_SETUPS_LEVELNAME_H
#define _SYS_GAME_OBJECTS_SETUPS_LEVELNAME_H

#include "game/objects/object.h"

// ctrl: 71 | size: 0x24
typedef struct {
    ObjSetup base;
    s16 gamebitShown;
    s16 unk1A;
    s16 unk1C;
    s16 textID;
    u8 activationRadius;
} LevelName_Setup;

#endif
