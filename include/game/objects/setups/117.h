#ifndef _SYS_GAME_OBJECTS_SETUPS_117_H
#define _SYS_GAME_OBJECTS_SETUPS_117_H

#include "game/objects/object.h"

// TODO: this is more than tumbleweed bushes

// ctrl: 117 | size: 0x24
typedef struct {
    ObjSetup base;
    u8 roll;
    u8 pitch;
    u8 yaw;
    u8 growWeedsRadius;             //Tumbleweeds grow on tree when player inside this radius (stored halved)
    f32 scale;
    u8 multiplyColourR;             //Colour multiplier for the tree
    u8 multiplyColourG;             //Colour multiplier for the tree
    u8 multiplyColourB;             //Colour multiplier for the tree
    u8 carryingGold;                //Bitfield flags: carrying a Shiny Nugget when bit0 is set
} Ctrl117_Setup;

#endif
