#ifndef _SYS_GAME_OBJECTS_SETUPS_WARP_H
#define _SYS_GAME_OBJECTS_SETUPS_WARP_H

#include "game/objects/object.h"

// ctrl: 39 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 unk19;
/*1A*/ s8 warpID;
/*1B*/ s8 objectSeqIndex;
/*1C*/ s8 isInboundWarp;
/*1D*/ s8 mode;
/*1E*/ s8 quarterRange;
/*20*/ s16 gamebit;
} Warp_Setup;

#endif
