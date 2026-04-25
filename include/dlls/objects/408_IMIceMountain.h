#ifndef _DLLS_408_H
#define _DLLS_408_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    u8 state;
    u8 flags;
    s8 warpCounter;
} IMIceMountain_Data;

typedef enum {
    STATE_0,
    STATE_Rescue_Tricky,
    STATE_Race_Start_Sequence,
    STATE_Race,
    STATE_Race_Won,
    STATE_Race_Lost,
    STATE_Intro_Sequence
} IMIceMountain_State;

typedef enum {
    IMICEMOUNTAIN_FLAG_1 = 0x1
} IMIceMountain_Flag;

typedef enum {
    IM_Act1_Rescuing_Tricky = 1,
    IM_Act2 = 2, //Rematch 1?
    IM_Act3 = 3, //Rematch 2?
    IM_Act4 = 4  //Ice Mountain finished?
} IM_Acts;

typedef enum {
    IM_ObjGroup0_AnimTricky = 0,
    IM_ObjGroup1 = 1,                           //Empty
    IM_ObjGroup2_AnimSpacecraft = 2,
    IM_ObjGroup3_Cave_Jetbikes_SharpClaw = 3,   //Player Jetbike, SharpClaw & Jetbike
    IM_ObjGroup4_Cave_Jetbikes_SharpClaw = 4,   //SharpClaw & Jetbike (closer to door)
    IM_ObjGroup5_Summit_Main = 5,
    IM_ObjGroup6_Track_Icicles = 6,             //Objects for the first stretch with icicle-covered arches
    IM_ObjGroup7_Summit_Jetbike_Sequences = 7   //Summit: SharpClaw racing sequences
} IM_ObjectGroups;

#endif // _DLLS_408_H
