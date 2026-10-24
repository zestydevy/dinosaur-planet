#ifndef _SYS_GAME_OBJECTS_SETUPS_THORNTAIL_H
#define _SYS_GAME_OBJECTS_SETUPS_THORNTAIL_H

#include "game/objects/object.h"

// ctrl: 72 | size: 0x1c
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 thorntail; // which Thorntail this is
} Thorntail_Setup;

#endif
