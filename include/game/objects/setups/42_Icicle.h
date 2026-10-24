#ifndef _SYS_GAME_OBJECTS_SETUPS_ICICLE_H
#define _SYS_GAME_OBJECTS_SETUPS_ICICLE_H

#include "game/objects/object.h"

// ctrl: 42 | size: 0x20
typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 fallPlayerRange; //The player must be inside this lateral distance from the icicle, for it to fall
    u8 scale;
    s16 gamebitEnabled; //The icicle's state machine won't run until this gamebit is set (when specified)
    s16 fallDelayTimer; //The icicle will start falling after this delay, after the fall conditions are activated
} Icicle_Setup;

#endif
