#ifndef _DLLS_357_H
#define _DLLS_357_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
/*00*/    ObjSetup base;
/*18*/    s16 xMin;            //Random position variance for explosion particles
/*1A*/    s16 yMin;            //Random position variance for explosion particles
/*1C*/    s16 zMin;            //Random position variance for explosion particles
/*1E*/    s16 xMax;            //Random position variance for explosion particles
/*20*/    s16 yMax;            //Random position variance for explosion particles
/*22*/    s16 zMax;            //Random position variance for explosion particles
/*24*/    s16 particleID;      //Type of particle to create for the explosion
/*26*/    s16 unused26;
/*28*/    s16 param0Max;       //Random variance for particle param 0
/*2A*/    s16 param1Max;       //Random variance for particle param 1
/*2C*/    u8 particleCount;    //Number of particles to create
/*2D*/    u8 pad2D;
/*2E*/    s16 param0Min;       //Random variance for particle param 0
/*30*/    s16 param1Min;       //Random variance for particle param 1
/*32*/    s16 gamebitExploded; //gamebit set when explosion has played out
/*34*/    s16 gamebitExplodeTrigger;  //gamebit checked to trigger the explosion
} ExplodeAnimator_Setup;

typedef struct {
    s8 unused0;
    s8 unused1;
    u8 flags;
    s8 unused3;
} ExplodeAnimator_Data;

typedef enum {
    ExplodeAnimator_FLAG_Waiting_to_Explode = 0,
    ExplodeAnimator_FLAG_Finished = 1
} ExplodeAnimator_Flags;

#endif // _DLLS_357_H

