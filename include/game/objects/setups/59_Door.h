#ifndef _SYS_GAME_OBJECTS_SETUPS_DOOR_H
#define _SYS_GAME_OBJECTS_SETUPS_DOOR_H

#include "game/objects/object.h"

// ctrl: 59 | size: 0x24
typedef struct { 
    ObjSetup base;
    s8 unk18;
    u8 _unk19;
    s16 unk1A;
    s16 unk1C;
    s16 _unk1E; 
    s16 unk20;
} Door_Setup;

#endif
