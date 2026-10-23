#ifndef _SYS_GAME_OBJECTS_SETUPS_SIDELOAD_H
#define _SYS_GAME_OBJECTS_SETUPS_SIDELOAD_H

#include "game/objects/object.h"

// ctrl: 32 | size: 0x1c
typedef struct {
    ObjSetup base;
    s16 gamebitUnlocked;    //GamebitID determining whether the sidekick is unlocked (i.e. Kyte rescued)
    u8 sidekickIndex;       //Whether this should load Tricky/Kyte (see `SideLoad_Indices`)
} SideLoad_Setup;

#endif
