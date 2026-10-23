#ifndef _SYS_GAME_OBJECTS_SETUPS_TRICKYGUARD_H
#define _SYS_GAME_OBJECTS_SETUPS_TRICKYGUARD_H

#include "game/objects/object.h"

// ctrl: 113 | size: 0x1c
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 range;
/*1A*/ s16 gamebit;
} TrickyGuard_Setup;

#endif
