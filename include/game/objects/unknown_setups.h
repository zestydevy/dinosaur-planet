#ifndef _SYS_GAME_OBJECTS_UNKNOWN_SETUPS_H
#define _SYS_GAME_OBJECTS_UNKNOWN_SETUPS_H

#include "game/objects/object.h"

// Includes DFriverflow
typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
} ObjType22Setup;

// Includes DFdockpoint
typedef struct {
    ObjSetup base;
    u8 _unk18[2];
    s16 unk1A;
} ObjType23Setup;

#endif
