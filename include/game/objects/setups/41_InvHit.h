#ifndef _SYS_GAME_OBJECTS_SETUPS_INVHIT_H
#define _SYS_GAME_OBJECTS_SETUPS_INVHIT_H

#include "game/objects/object.h"

// ctrl: 41 | size: 0x20
typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 pad1B[0x20 - 0x1B];    
} InvHit_Setup;

#endif
