#ifndef _SYS_GAME_OBJECTS_SETUPS_51_H
#define _SYS_GAME_OBJECTS_SETUPS_51_H

#include "game/objects/object.h"

// TODO: many more DLLs to add this to
// ctrl: 51 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
    // Union provided for code readability, all fields must be s16 to conform with the actual objcontrol definition.
    union {
        // Base definition:
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 genericA;
            /*1C*/ s16 genericB;
            /*1E*/ s16 bitA;
            /*20*/ s16 bitB;
        } generic;
        // Object-specific definitions:
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 range;
            /*1C*/ s16 unused1;
            /*1E*/ s16 unused2;
            /*20*/ s16 unused3;
        } dockpoint;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 preemptTime;
            /*1C*/ s16 unused;
            /*1E*/ s16 unlockedBit;
            /*20*/ s16 stateBit;
        } cfBigDoorSwitch;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 extraDamageStages; // aka health - 1. if zero, no model is drawn and it's assumed the block shapes are the visible surface
            /*1C*/ s16 shapeAnimatorID; // animator ID of block shapes to hide/remove collision of when blasted
            /*1E*/ s16 blastedGamebit; // bit to set when blasted
            /*20*/ s16 hitCountGamebit; // for persisting the hit count
        } blastedObj;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 opacityThreshold;
            /*1C*/ s16 unused;
            /*1E*/ s16 bitViewed;
            /*20*/ s16 bitEnabled;
        } apertureSymbol;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 offIntervalDuration;
            /*1C*/ s16 firingDuration;
            /*1E*/ s16 bitEnabled;
            /*20*/ s16 unused;
        } wcLaser;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 hitsAnimatorID;
            /*1C*/ s16 unused1;
            /*1E*/ s16 unused2;
            /*20*/ s16 unused3;
        } wcFloorTile;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 scale; //0x2000 is 1x scale
            /*1C*/ s16 roll;
            /*1E*/ s16 unused1;
            /*20*/ s16 unused2;
        } firepole;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 unused1;
            /*1C*/ s16 unused2;
            /*1E*/ s16 gamebitFinished;
            /*20*/ s16 unused3;
        } wcDial;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 unused1;
            /*1C*/ s16 unused2;
            /*1E*/ s16 gamebitVisible;
            /*20*/ s16 unused3;
        } wcMagicBridge;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 unk1A;
            /*1C*/ s16 unused;
            /*1E*/ s16 destroyedBit;
            /*20*/ s16 crashBit;
        } robotAnimPatrol;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 sightRange;
            /*1C*/ s16 unused1;
            /*1E*/ s16 unused2;
            /*20*/ s16 unused3;
        } cfPrisonGuard;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 unk1A;
            /*1C*/ s16 reverseBit;
            /*1E*/ s16 playerInsideBit;
            /*20*/ s16 unused;
        } windLift;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 preemptTime;
            /*1C*/ s16 unused;
            /*1E*/ s16 enabledGamebit;
            /*20*/ s16 stateGamebit;
        } toggleSwitch;
        struct {
            /*18*/ s8 activateRange;
            /*19*/ s8 unk19;
            /*1A*/ s16 actionIdxBase;
            /*1C*/ s16 gamebitActivate;
            /*1E*/ s16 gamebitActionIdxOffset;
            /*20*/ s16 unused2;
        } envEmitter;
    };
} Ctrl51_Setup;

#endif
