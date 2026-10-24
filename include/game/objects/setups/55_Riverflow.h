#ifndef _SYS_GAME_OBJECTS_SETUPS_RIVERFLOW_H
#define _SYS_GAME_OBJECTS_SETUPS_RIVERFLOW_H

#include "game/objects/object.h"

// ctrl: 55 | size: 0x20
typedef struct {
    ObjSetup base;
    u8 yaw;
    u8 range;           //determines push falloff range and push strength (also used as scale, divided by 512)
    u8 flags;           //filters what is affected by the push effect. 0 = everything, bit 1 = logs, bit 2 = player
    u8 pad1B;
    s16 toggleGamebit;  //optional: gamebitID switches off the riverflow when set
} Riverflow_Setup;

#endif
