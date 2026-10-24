#ifndef _SYS_GAME_OBJECTS_SETUPS_EXPLODER_H
#define _SYS_GAME_OBJECTS_SETUPS_EXPLODER_H

#include "game/objects/object.h"

// ctrl: 105 | size: 0x48
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 pieceCount;
/*1A*/ s16 yaw;
/*1C*/ s16 pitch;
/*1E*/ s16 roll;
/*20*/ Vec3s16 displacementOrigin;
/*26*/ Vec3s16 _unk26;
/*2C*/ s16 explosionPower;
/*2E*/ s16 floorOffset;
/*30*/ s16 acceleration;
/*32*/ Vec3s16 _unk32;
/*38*/ u16 lifetimeMax;
/*3A*/ s16 _unk3A;
/*3C*/ s16 _unk3C;
/*3E*/ s16 gamebitFinished;
/*40*/ s16 gamebitExplode;
/*42*/ u8 _unk42[0x48 - 0x42];
} Exploder_Setup;

// size: 0x44 (note: struct too small for the ctrl #, pieces are runtime objects only)
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
} ExploderPiece_Setup;
    
#endif
