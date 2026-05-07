#ifndef _DLLS_345_H
#define _DLLS_345_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 unk18;
/*1A*/ s8 roll;
/*1B*/ s8 pitch;
/*1C*/ s8 yaw;
/*1D*/ u8 amplitudeX;
/*1E*/ u8 amplitudeZ;
/*1F*/ u8 amplitudeY;
/*20*/ u8 distance;
/*21*/ u8 unk21;
/*22*/ u8 unk22;
/*23*/ u8 unk23;
/*24*/ u8 iterations;
} WaterFallSpray_Setup;

#endif
