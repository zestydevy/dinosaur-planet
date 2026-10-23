#ifndef _SYS_GAME_OBJECTS_SETUPS_FIRECRYSTAL_H
#define _SYS_GAME_OBJECTS_SETUPS_FIRECRYSTAL_H

#include "game/objects/object.h"

// ctrl: 111 | size: 0x1c
typedef struct {
    ObjSetup base;
    s16 gamebitCollected;
} FireCrystal_Setup;

#endif
