#ifndef _SYS_GAME_OBJECTS_SETUPS_SETUPPOINT_H
#define _SYS_GAME_OBJECTS_SETUPS_SETUPPOINT_H

#include "game/objects/object.h"

// ctrl: 4 | size: 0x20
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 mapAct;
/*19*/ u8 setupID; //assigned to Object->unkE0
/*1A*/ u8 yaw;
/*1C*/ u32 objGroupStatusBits;
} SetupPoint_Setup;

#endif
