#ifndef _SYS_GAME_OBJECTS_SETUPS_TRICKYWARP_H
#define _SYS_GAME_OBJECTS_SETUPS_TRICKYWARP_H

#include "game/objects/object.h"

// ctrl: 112 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*1A*/ s16 range;
/*1C*/ s8 unk1C[0x20 - 0x1C];
/*20*/ s16 gamebit;
} TrickyWarp_Setup;

#endif
