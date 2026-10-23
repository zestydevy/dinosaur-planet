#ifndef _SYS_GAME_OBJECTS_SETUPS_EWTROBOTPATROLB_H
#define _SYS_GAME_OBJECTS_SETUPS_EWTROBOTPATROLB_H

#include "game/objects/object.h"

// ctrl: 100 | size: 0x1c
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 unk18; // unused due to, presumably, missing code. likely the number of robos to spawn 
/*19*/ u8 numNodes; // set but unused, the dll discovers this count on its own
/*1A*/ u8 roboFadeDistance;
/*1B*/ u8 maxSearchTime; // maximum search time (after aggro) divided by 64
} EWTrobotpatrolB_Setup;

#endif
