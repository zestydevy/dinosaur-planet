#ifndef _DLLS_390_H
#define _DLLS_390_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    u8 pieceCount;
    s16 yaw;
    s16 pitch;
    s16 roll;
    Vec3s16 displacementOrigin;
    Vec3s16 _unk26;
    s16 explosionPower;
    s16 floorOffset;
    s16 acceleration;
    Vec3s16 _unk32;
    u16 lifetimeMax;
    s16 _unk3A;
    s16 _unk3C;
    s16 gamebitFinished;
    s16 gamebitExplode;
} CFExplodeWall_Setup;

typedef enum {
    CFExplodeWall_STATE_0_Waiting,
    CFExplodeWall_STATE_1_Exploding,
    CFExplodeWall_STATE_2_Finished
} CFExplodeWall_States;

typedef enum {
    CFExplodePiece_FLAG_Translating_X = 1,
    CFExplodePiece_FLAG_Translating_Z = 2,
    CFExplodePiece_FLAG_Rotating_Yaw = 4,
    CFExplodePiece_FLAG_Rotating_Pitch = 8,
    CFExplodePiece_FLAG_Rotating_Roll = 0x10
} CFExplodePiece_Flags;

#endif // _DLLS_390_H

