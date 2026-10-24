#ifndef _SYS_GAME_OBJECTS_SETUPS_CANNON_H
#define _SYS_GAME_OBJECTS_SETUPS_CANNON_H

#include "game/objects/object.h"

// ctrl: 37 | size: ?
typedef struct {
    ObjSetup base;
    u8 yaw;
    u8 unk19;       //Unknown purpose. Maybe meant as a lifetime for the cannon or its cannonballs?
} Cannon_Setup;

#endif
