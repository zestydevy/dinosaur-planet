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

typedef enum {
    DF_Lantern_FLAG_1_Player_Collision = 1,     //Toggle objHits during setup (doesn't seem to work, but probably meant to decide whether the lantern burns you?)
    DF_Lantern_FLAG_2_Emit_Light = 2,           //Apply a lightAction
    DF_Lantern_FLAG_4_Show_PartFX_Glow = 4,     //Draw the partFX
    DF_Lantern_FLAG_8_Play_Sound = 8,           //Play a crackling sound loop when near the lantern
    DF_Lantern_FLAG_10_Always_Lit = 0x10,       //For indoor lanterns - always lit, instead of only at night-time
    DF_Lantern_FLAG_20_Use_Fade_Distance = 0x20 //Use the objSetup fadeDistance as the camera range for the partFX glow
} DF_Lantern_Flags;

// ctrl: 117 | size: 0x24
typedef struct {
    ObjSetup base;
    u8 roll;
    u8 pitch;
    u8 yaw;
    u8 useOtherLFXConfig;  //Boolean, affects which lightAction indices are used
    f32 scale;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 flags;              //See `DF_Lantern_Flags`, used so indoor lanterns are always lit (instead of just at night), or to use fadeDistance for camera range
} DF_Lantern_Setup;

#endif
