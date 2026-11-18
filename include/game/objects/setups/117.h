#ifndef _SYS_GAME_OBJECTS_SETUPS_117_H
#define _SYS_GAME_OBJECTS_SETUPS_117_H

#include "game/objects/object.h"

// ctrl: 117 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 roll;
/*19*/ u8 pitch;
/*1A*/ u8 yaw;
/*
- TumbleweedBush: Tumbleweeds grow when player is inside this radius (stored halved)
- DF_Lantern:     Boolean, affects which lightAction indices are used
*/
/*1B*/ u8 setting;
/*1C*/ f32 scale;
/*20*/ u8 colourR; //TumbleweedBush: Colour multiplier for the tree
/*21*/ u8 colourG; //TumbleweedBush: Colour multiplier for the tree
/*22*/ u8 colourB; //TumbleweedBush: Colour multiplier for the tree
/*23*/ u8 flags;
} Ctrl117_Setup;

typedef enum {
    TumbleweedBush_FLAG_1_Carrying_Gold = 1     //A Shiny Nugget is held in the tree's branches
} TumbleweedBush_Flags;

typedef enum {
    DF_Lantern_FLAG_1_Player_Collision = 1,     //Toggle objHits during setup (doesn't seem to work, but probably meant to decide whether the lantern burns you?)
    DF_Lantern_FLAG_2_Emit_Light = 2,           //Apply a lightAction
    DF_Lantern_FLAG_4_Show_PartFX_Glow = 4,     //Draw the partFX
    DF_Lantern_FLAG_8_Play_Sound = 8,           //Play a crackling sound loop when near the lantern
    DF_Lantern_FLAG_10_Always_Lit = 0x10,       //For indoor lanterns - always lit, instead of only at night-time
    DF_Lantern_FLAG_20_Use_Fade_Distance = 0x20 //Use the objSetup fadeDistance as the camera range for the partFX glow
} DF_Lantern_Flags;

#endif
