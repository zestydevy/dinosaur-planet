#ifndef _SYS_GAME_OBJECTS_SETUPS_SNOWHORN_H
#define _SYS_GAME_OBJECTS_SETUPS_SNOWHORN_H

#include "game/objects/object.h"

// ctrl: 40 | size: 0x20
typedef struct {
/*0x10*/ ObjSetup base;
/*0x18*/ s16 playerNearbyRange; //`SnowHorn_FLAG_80_Player_Nearby` is set when the player is inside a multiple of this range (not used for anything, though)
/*0x1A*/ s16 squirtInverval;    //For the Blue SnowHorn: the time (in seconds) between squirting water at the player
/*0x1C*/ s8 yaw;
/*0x1D*/ s8 characterIdx;       //Which SnowHorn this is (see `SnowHorn_CharacterIndices`)
} SnowHorn_Setup;

#endif
