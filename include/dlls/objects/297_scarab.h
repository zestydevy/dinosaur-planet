#ifndef _DLLS_297_H
#define _DLLS_297_H

#include "PR/ultratypes.h"
#include "dlls/engine/27.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unused19;
    s16 lifetime;
    u8 _unk1C[8];
} Scarab_Setup;

typedef struct {
/*00*/ u32 soundHandle;         //Manages crawling sound loop
/*04*/ f32 speedX;              //Scurry speed
/*08*/ f32 speedZ;              //Scurry speed
/*0C*/ f32 goldClimbTimer;      //Amount of time Gold Scarab has been climbing wall
/*10*/ f32 initialY;            //Y value from objSetup - used as fallback height
/*14*/ s16 destructDelayTimer;  //Used to delay deletion when collected
/*16*/ s16 unused16;
/*18*/ s16 lifetime;            //Decrements while scurrying, Scarab disappears when 0
/*1A*/ s16 rollSpeed;           //Rate of change of roll
/*1C*/ s16 scurryInitialYaw;    //Reference yaw
/*1E*/ s16 stunTimer;           //Scarab can't continue scurrying while active. Rainbow Scarabs can only be collected while stunned!
/*20*/ s16 goldClimbDuration;   //Amount of time Gold Scarab should climb wall before stopping (randomised)
/*22*/ s16 collectSoundID;      //Sound to play when collected
/*24*/ s16 collectFXScale;      //Size of effect created when collected
/*26*/ u8 state;                //State machine value
/*27*/ s8 unused27;
/*27*/ s8 unused28;
/*29*/ u8 scarabTypeIndex;      //0) Green, 1) Red, 2) Gold, 3) Rainbow
/*2A*/ u8 flags;                //Receive messages from other objects
} Scarab_Data;

typedef enum {
    Scarab_STATE_Tumbling_Through_Air = 0,
    Scarab_STATE_Scurrying = 1,
    Scarab_STATE_Climb_Wall_Gold = 2 //Special Gold Scarab behaviour: climb vertically, can't be collected until shot down
} Scarab_States;

typedef enum {
    Scarab_FLAG_Wait_for_Message = 1
} Scarab_Flags;

typedef enum {
    Scarab_TYPE_Green = 0,
    Scarab_TYPE_Red = 1,
    Scarab_TYPE_Gold = 2,
    Scarab_TYPE_Rainbow = 3
} Scarab_Types;

#endif //_DLLS_297_H
