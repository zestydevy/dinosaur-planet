#ifndef _DLLS_345_H
#define _DLLS_345_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebit;     //Unused, but seems intended for a gamebit
/*1A*/ s8 roll;
/*1B*/ s8 pitch;
/*1C*/ s8 yaw;
/*1D*/ u8 amplitudeX;
/*1E*/ u8 amplitudeZ;
/*1F*/ u8 amplitudeY;
/*20*/ u8 distance;
/*21*/ u8 unk21;
/*22*/ u8 unk22;
/*23*/ u8 flags;
/*24*/ u8 iterations;
} WaterFallSpray_Setup;

typedef enum {
    WaterFallSpray_FLAG_Big_Mist_Cloud = 0x1,
    WaterFallSpray_FLAG_Falling_Droplets = 0x2,
    WaterFallSpray_FLAG_Small_Mist_Cloud = 0x4,
    WaterFallSpray_FLAG_Mist_Jet = 0x8
} WaterFallSpray_Flags;

#endif
