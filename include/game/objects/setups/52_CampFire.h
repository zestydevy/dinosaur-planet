#ifndef _SYS_GAME_OBJECTS_SETUPS_CAMPFIRE_H
#define _SYS_GAME_OBJECTS_SETUPS_CAMPFIRE_H

#include "game/objects/object.h"

// ctrl: 52 | size: 0x20
typedef struct {
    ObjSetup base;
    s16 gamebitID;          //Checked during setup, but does nothing (maybe for tracking lighting fire/putting it out?)
    u8 scale;               //Scale multiplier (100 = 1.0)
    u8 unk1B;               //Unknown - stored in setup but otherwise unused
    u8 alwaysIlluminates;   //When nonzero, fire emits light even when the player isn't nearby
    u8 modGfxArg;           //Boolean affecting the fire mesh modGfx calls, unknown purpose
    u16 lfxIdx;             //Param for the light emitter
} CampFire_Setup;

#endif
