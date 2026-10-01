#ifndef _DLLS_421_H
#define _DLLS_421_H

#include "dll.h"

typedef struct {
    /* 0000 */ f32 unk0; // lerp t-value from 0 to 1
    /* 0004 */ f32 unk4;
    /* 0008 */ f32 unk8;
    /* 000C */ f32 unkC;
    /* 0010 */ s32 unk10;
    /* 0014 */ f32 unk14[20];
    /* 0064 */ f32 unk64;
    /* 0068 */ Vec3f unk68; //curve point (at current tValue)
    /* 0074 */ Vec3f unk74; //curve tangent (at current tValue)
    /* 0080 */ s32 unk80;
    /* 0084 */ f32* unk84;
    /* 0088 */ f32* unk88;
    /* 008C */ f32* unk8C;
    /* 0090 */ s32 numControlPoints;
    /* 0094 */ SplineFunc splineFunc;
    /* 0098 */ SplineConverterFunc splineConverterFunc;
} UnkCurvesStruct_Split;

typedef struct {
    ObjSetup base;
    u8 roll;
    u8 pitch;
    u8 yaw;
} DFCradle_Setup;

typedef struct {
    UnkCurvesStruct_Split curves;
    f32 speed;          //The cradle's current absolute movement speed along its path
    f32* splineX;
    f32* splineY;
    f32* splineZ;
    f32 pulleyValLower; //An approximate rope path position value for the pulley closer to the Lower Falls (used for object station calcs)
    f32 pulleyValUpper; //An approximate rope path position value for the pulley closer to the Upper Falls (used for object station calcs)
    s8 enabled;         //Cradle has been powered (Kyte activated turbine lever)
    s8 direction;       //1 (towards Upper Falls) or -1 (towards Lower Falls)
    u8 pauseTimer;      //A short delay before the next move, after arriving at a pulley
    u8 prevCradleStation;   //The cradle's "station" value from the previous tick (see `DFCradle_getStationNumber`)
    s8 soundTimer; //Randomised interval between rope straining sounds
} DFCradle_Data;

#endif // _DLLS_421_H
