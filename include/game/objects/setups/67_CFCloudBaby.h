#ifndef _SYS_GAME_OBJECTS_SETUPS_CFCLOUDBABY_H
#define _SYS_GAME_OBJECTS_SETUPS_CFCLOUDBABY_H

#include "game/objects/object.h"

// ctrl: 67 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
       // Considers things (the player, beans) to be close below this distance.
/*18*/ s16 closeMaxDist;
       // Can be rescued by the player below this distance.
/*1A*/ s16 rescueMaxDist;
/*1C*/ u8 initialState;
/*1D*/ u8 yaw8;
       // Set when initially rescued.
/*1E*/ s16 rescuedGamebit;
       // Swaps to this state after the player gets close for the first time.
/*20*/ u8 initialState2;
       // Set when the rescued timer completes and is considered 
       // to be in the throne room now.
/*22*/ s16 reachedPerchGamebit;
} CFCloudBaby_Setup;

#endif
