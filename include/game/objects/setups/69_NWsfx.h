#ifndef _SYS_GAME_OBJECTS_SETUPS_NWSFX_H
#define _SYS_GAME_OBJECTS_SETUPS_NWSFX_H

#include "game/objects/object.h"

// ctrl: 69 | size: 0x1c
typedef struct {
    ObjSetup base;
    s16 gamebitDisable;      //Sounds stop playing when this gamebit is set
    u8 soundListIdx;         //The random sounds list will only be used when this is set to 0. Possibly intended to pick between different (unimplemented) arrays of soundIDs?
    u8 intervalInSeconds;    //The delay between sounds (actual delay will be randomised: somewhere between 1x and 2x this value)
} NWsfx_Setup;

#endif
