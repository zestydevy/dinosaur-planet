#ifndef _SYS_GAME_OBJECTS_SETUPS_WGBOUNCYVINE_H
#define _SYS_GAME_OBJECTS_SETUPS_WGBOUNCYVINE_H

#include "game/objects/object.h"

// ctrl: 128 | size: 0x20
typedef struct {
    ObjSetup base;
    s8 yaw;
    s16 gamebitA;           //GamebitID for defeating a nearby Pollen Cannon
    s16 gamebitB;           //GamebitID for defeating a nearby Pollen Cannon
    s16 gamebitHurtPlayer;  //Vine bounces and hurts the player when this is set (by TriggerPlane)
} WGBouncyVine_Setup;

#endif
