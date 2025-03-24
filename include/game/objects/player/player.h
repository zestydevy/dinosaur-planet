#ifndef _GAME_OBJECTS_PLAYER_H
#define _GAME_OBJECTS_PLAYER_H

#include "PR/ultratypes.h"
#include "dlls/engine/29_gplay.h"
#include "game/objects/object.h"

/** Player system
 */

extern GplayStruct7 *gGplayState;

extern s32 PlayerPosBuffer_index;
extern struct Vec3_Int PlayerPosBuffer[60]; //seems to buffer player coords with "timestamp"

#endif
