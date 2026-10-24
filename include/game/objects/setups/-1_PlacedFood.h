#ifndef _SYS_GAME_OBJECTS_SETUPS_PLACEDFOOD_H
#define _SYS_GAME_OBJECTS_SETUPS_PLACEDFOOD_H

#include "game/objects/object.h"

/** 
 * ObjSetup struct for food Objects placed in the world
 * 
 * (e.g. OBJ_foodbagRedApple, OBJ_foodbagNewFish, OBJ_foodbagBlueMush)
 *
 * ctrl: -1 | size: 0x20
 */
typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s8 yaw;
    s16 unk1C;
    s16 unk1E;
} PlacedFood_Setup;

#endif
