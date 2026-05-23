#ifndef _DLLS_505_H
#define _DLLS_505_H

#include "PR/ultratypes.h"
#include "dlls/engine/27.h"
#include "game/objects/object.h"
#include "types.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 unk18;                       //unused
/*1A*/ s16 angularRange;                //maximum angular distance from desired travel angle
/*1C*/ s16 windAngle;                   //desired direction the spore should travel in
/*20*/ s32 unused20;					//unused
} SHSpore_Setup;

typedef struct {
/*000*/ DLL27_Data terrainCollider;
/*260*/ f32 lifetime;
/*264*/ f32 lateralSpeed;
/*268*/ f32 lateralAcceleration;
/*26C*/ f32 lateralSpeedGoal;
/*270*/ f32 angularJitterTimer;         //waiting time between slightly randomising flight angle
/*274*/ f32 velocityX;
/*278*/ f32 velocityZ;
/*27C*/ f32 coefficientX;               //multiplier for lateral acceleration
/*280*/ f32 coefficientZ;               //multiplier for lateral acceleration
/*284*/ f32 lateralDecelerationTimer;   //waiting time until lateral acceleration dwindles
/*288*/ f32 lateralAccelerationGoal;
/*28C*/ f32 angleChangeTimer;           //waiting time between large randomisation of flight angle
/*290*/ f32 deletionTimer;      
/*294*/ s16 angle;                      //lateral direction of travel
/*296*/ s16 angleJitter;                //lateral direction of travel (plus randomised angular noise)
/*298*/ s16 angleGoal;
} SHSpore_Data;

#endif // _DLLS_505_H
