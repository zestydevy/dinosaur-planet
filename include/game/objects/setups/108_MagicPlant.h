#ifndef _SYS_GAME_OBJECTS_SETUPS_MAGICPLANT_H
#define _SYS_GAME_OBJECTS_SETUPS_MAGICPLANT_H

#include "game/objects/object.h"

// ctrl: 108 | size: 0x20
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u16 regrowthTime;    //The amount of time to spend regrowing (in frames)
/*1A*/ s8 unused1A;
/*1B*/ u8 dustIdx;          //The index of the type of MagicDust to create (dMagicDustObjIDs)
/*1C*/ u8 modelInstIdx;     //The model index to use for the plant (default vs. snowy)
/*1D*/ u8 yaw;
} MagicPlant_Setup;

#endif
