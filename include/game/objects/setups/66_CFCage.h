#ifndef _SYS_GAME_OBJECTS_SETUPS_CFCAGE_H
#define _SYS_GAME_OBJECTS_SETUPS_CFCAGE_H

#include "game/objects/object.h"

// ctrl: 66 | size: 0x1c
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebit;
/*1A*/ u8 yaw8;
} CFCage_Setup;

#endif
