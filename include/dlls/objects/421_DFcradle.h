#ifndef _DLLS_421_H
#define _DLLS_421_H

#include "dll.h"

typedef struct {
    /* 0000 */ f32 unk0;
    /* 0004 */ f32 unk4; // curveProgress? (lerp t-value from 0 to 100?)
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
    f32 unk9C;          //Spline tValue?
    f32* splineX;
    f32* splineY;
    f32* splineZ;
    f32 pulleyValLower; //Related to the rotating middle pulley closer to SwapStone Circle entrance
    f32 pulleyValUpper; //Related to the rotating middle pulley closer to the shrine entrance
    s8 enabled;         //Cradle has been powered (Kyte activated turbine lever)
    s8 direction;       //1 or -1
    u8 unkB6;
    u8 prevCradleVal;
    s8 soundTimer; //Randomised interval between rope straining sounds
} DFCradle_Data;

#endif // _DLLS_421_H
