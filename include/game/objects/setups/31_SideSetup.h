#ifndef _SYS_GAME_OBJECTS_SETUPS_SIDESETUP_H
#define _SYS_GAME_OBJECTS_SETUPS_SIDESETUP_H

#include "game/objects/object.h"

// ctrl: 31 | size: 0x1c
typedef struct {
/*00*/	ObjSetup base;
/*18*/	union Unk18{
        u8 byte[2];
        s16 halfword;
    } unk18;
/*1A*/	u8 unk1A;
} SideSetup_Setup;

#endif
