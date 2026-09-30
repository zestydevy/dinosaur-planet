#ifndef _DLLS_429_H
#define _DLLS_429_H

#include "dll.h"

/* 
    NOTE: these structs are used in two DLLs:

    - DLL 429 (main SeqDoor) [DFSH_Door1Spe]
    - DLL 430 (child door piece) [DFSH_Door2Spe, DFSH_Door3Spe, DFSH_Door4Spe]

    Some of the SeqDoor-like fields are only used by the main door DLL (DFSH_Door1Spe).
*/

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebitOpened;       //[Main Door Only] Opens the door
/*1A*/ s16 gamebitDoorState;    //[Main Door Only] Restores the door's state during setup
/*1C*/ s16 seqPreemptTime;      //[Main Door Only] ObjSeq time to jump to when restoring state
/*1E*/ s8 seqIndex;             //[Main Door Only] ObjSeq to play
/*1F*/ u8 yaw;
/*20*/ u8 enabledActors;        //[Main Door Only] ObjSeq actor mask
/*21*/ u8 scale;
/*22*/ s16 gamebitLit;          //A point on the door's Krazoa symbol lights up when this gamebit is set (through pressing ancient switches around Discovery Falls)
} DFSH_DoorSpecial_Setup;

typedef struct {
    u16 phase;      //Phase angle for the Krazoa symbol's glow effect
    u8 state;       //[Main Door Only] SeqDoor state (`DFSH_DoorSpecial_States`)
    u8 glowState;   //Glow effect state (`DFSH_DoorSpecial_GlowStates`)
    u8 runControl;  //[Main Door Only] Boolean: starts off TRUE, set to FALSE at the end of obj_Control (so control only runs once)
} DFSH_DoorSpecial_Data;

typedef enum {
    DFSH_Door1Special_STATE_0_Closed,
    DFSH_Door1Special_STATE_1_Open,
    DFSH_Door1Special_STATE_2_Opening,
    DFSH_Door1Special_STATE_3_Closing
} DFSH_DoorSpecial_States;

typedef enum {
    DFSH_DoorSpecial_GLOW_0_Unlit,
    DFSH_DoorSpecial_GLOW_1_Fade_In,
    DFSH_DoorSpecial_GLOW_2_Pulse
} DFSH_DoorSpecial_GlowStates;

#endif // _DLLS_429_H
