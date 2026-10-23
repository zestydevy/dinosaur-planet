#ifndef _SYS_GAME_OBJECTS_SETUPS_PROJECTILE_H
#define _SYS_GAME_OBJECTS_SETUPS_PROJECTILE_H

#include "game/objects/object.h"

// ctrl: 38 | size: 0x24
typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} Projectile_Setup;

#endif
