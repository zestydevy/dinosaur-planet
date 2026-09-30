#ifndef _DLLS_404_H
#define _DLLS_404_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 modelIdx;
/*1A*/ s16 yaw;
/*1C*/ s16 pitch;
/*1E*/ s16 roll;
/*20*/ Vec3s16 velocity;
/*26*/ Vec3s16 acceleration;
/*2C*/ s16 yawSpeed;
/*2E*/ s16 pitchSpeed;
/*30*/ s16 rollSpeed;
/*32*/ s16 yawAcceleration;
/*34*/ s16 pitchAcceleration;
/*36*/ s16 rollAcceleration;
/*38*/ u16 lifetimeMax;
/*3A*/ u16 floorOffset;
/*3C*/ s16 unk3C;
/*3E*/ s16 gamebitFinished;
/*40*/ s16 gamebitExplode;
} CFExplodePieces_Setup;

typedef enum {
    CFExplodePieces_STATE_0_Stopped,
    CFExplodePieces_STATE_1_Moving,
    CFExplodePieces_STATE_2_Finished
} CFExplodePieces_States;

typedef enum {
    CFExplodePieces_FLAGS_4_Touching_Ground = 4
} CFExplodePieces_Flags;

DLL_INTERFACE(DLL_404_CFExplodePieces) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ CFExplodePieces_States (*GetState)(Object* self);
};

#define dll_CFExplodePieces(obj) (((DLL_404_CFExplodePieces*)obj->dll)->vtbl)

#endif // _DLLS_404_H

