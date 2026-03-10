#ifndef _DLLS_290_H
#define _DLLS_290_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u16 regrowthTime;    //The amount of time to spend regrowing (in frames)
/*1A*/ s8 unused1A;
/*1B*/ u8 dustIdx;          //The index of the type of MagicDust to create (dMagicDustObjIDs)
/*1C*/ u8 modelInstIdx;     //The model index to use for the plant (default vs. snowy)
/*1D*/ u8 yaw;
} MagicPlant_Setup;

typedef struct {
/*00*/ Object *magic;       //OBJ_MagicDust created by the plant
/*04*/ f32 growProgress;    //Animation tValue while the plant's in its bud form (depends on gplay timer)
/*08*/ f32 animProgress;    //Animation tValue when grown
/*0C*/ u32 soundHandle;     //Sound handle for twinkling loop that plays when near the plant
/*10*/ s16 idleAnimDelay;   //Randomised delay for idle animation (for desyncing neighbouring plants?)
/*12*/ s8 unused12;
/*13*/ s8 state;            //State Machine value
} MagicPlant_Data;

typedef enum {
    MagicPlant_STATE_Growing = 0,           //Regrowth timer counts down, then grows & creates Magic Dust gem
    MagicPlant_STATE_Idle = 1,              //Grown, swaying gently in the wind
    MagicPlant_STATE_Wilting = 2,           //Wilting after being damaged
    MagicPlant_STATE_Bud = 3,               //Becoming a bud, and starting a gplay regrowth timer
    MagicPlant_STATE_Damaged = 4            //Damaged, shakes and drops gem
} MagicPlant_States;

typedef enum {
    MagicPlant_MODANIM_Growing = 0,         //Growing up from a bud
    MagicPlant_MODANIM_Rustled = 1,         //Unused? Sways like it's caught by a gust of wind, then back to idle
    MagicPlant_MODANIM_Wilting = 2,         //Falling to the ground
    MagicPlant_MODANIM_Damage_Shake = 3,    //Damage reaction
    MagicPlant_MODANIM_Sway_Loop = 4        //Main idle loop, gently swaying
} MagicPlant_Modanim;

#endif // _DLLS_290_H
