#ifndef _SYS_GAME_OBJECTS_SETUPS_ROBOTPATROL_H
#define _SYS_GAME_OBJECTS_SETUPS_ROBOTPATROL_H

#include "game/objects/object.h"

// ctrl: 58 | size: 0x1C
typedef struct {
    ObjSetup base;
    u8 _unk18[0x1A - 0x18];
    u8 unk1A;
} RobotPatrol_Setup;

#endif
