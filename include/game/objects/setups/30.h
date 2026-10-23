#ifndef _SYS_GAME_OBJECTS_SETUPS_30_H
#define _SYS_GAME_OBJECTS_SETUPS_30_H

#include "game/objects/object.h"

// ctrl: 30 | size: 0x24
typedef struct {
    ObjSetup base;
    u8 modelIndex;          //Which model to use
    u8 modAnimIndex;        //Which model animation to use
    u8 animProgress;        //Initial tValue for the animation (expressed from 0-256 instead of 0-1)
    u8 animSpeed;           //Animation playback speed (stored at 10000x actual value)
    u8 scaleMultiplier;     //Multiplies the size of the object and its shadow (64 = 1x, 128 = 2x, 32 = 0.5x, etc.)
    u8 yaw;
    s16 unused1E;
    s16 gamebitVisible;     //Toggles object visibility
} Ctrl30_Setup;

#endif
