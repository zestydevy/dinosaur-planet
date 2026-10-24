#ifndef _SYS_GAME_OBJECTS_SETUPS_KILLERMUSHROOM_H
#define _SYS_GAME_OBJECTS_SETUPS_KILLERMUSHROOM_H

#include "game/objects/object.h"

// ctrl: 44 | size: 0x24
typedef struct {
    ObjSetup base;
    u16 attackCooldown;     //How long (in frames) until the mushroom leaves its attack outro state
    u16 regrowWaitDuration; //How long (in frames) until the mushroom starts regrowing after being collected/killed
    s16 gamebitAttacked;    //Optional gamebitID to set when the mushroom is damaged
    u8 aggroRadius;         //While in idle state, attacks the player with spores if they enter this radius
    u8 modelIdx;            //Mushroom design (only has 1 model, though)
    u8 _pad20[0x24 - 0x20];
} KillerMushroom_Setup;

#endif
