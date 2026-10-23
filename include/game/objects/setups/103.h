#ifndef _SYS_GAME_OBJECTS_SETUPS_103_H
#define _SYS_GAME_OBJECTS_SETUPS_103_H

#include "game/objects/object.h"

// ctrl: 103 | size: 0x20
typedef struct {
/*00*/ ObjSetup base;
    union {
        struct {
        /*18*/ u8 roll;
        /*19*/ u8 pitch;
        /*1A*/ u8 yaw;
        /*1B*/ u8 setting; // use varies
        /*1C*/ f32 size;
        };
        struct {
        /*18*/ u8 unk18;
        /*19*/ u8 unused1;
        /*1A*/ u8 unused2;
        /*1B*/ u8 unused3;
        /*1C*/ f32 size;
        } cfForceField;
        struct {
        /*18*/ u8 unk18;
        /*19*/ u8 unused1;
        /*1A*/ u8 unused2;
        /*1B*/ u8 unused3;
        /*1C*/ f32 unused4;
        } cfTreasRobo;
        struct {
        /*18*/ u8 hasBaby;
        /*19*/ u8 unused1;
        /*1A*/ u8 yaw;
        /*1B*/ u8 unused2;
        /*1C*/ f32 unused3;
        } cfSupTreasureCh;
        struct {
        /*18*/ u8 animatorID;
        /*19*/ u8 unk19;
        /*1A*/ u8 unk1A;
        /*1B*/ u8 unused1;
        /*1C*/ f32 unused2;
        } cfMagicWall;
        struct {
        /*18*/ u8 unused1;
        /*19*/ u8 unused2;
        /*1A*/ u8 radius;
        /*1B*/ u8 yOffset;
        /*1C*/ f32 size;
        } ccFloor;
        struct {
        /*18*/ u8 unused1;
        /*19*/ u8 unused2;
        /*1A*/ u8 yaw;
        /*1B*/ u8 modelIndex; //Which model to use: square/diamond/circle/triangle
        /*1C*/ f32 vocalsDistance; //Music track's vocals added/removed based on this radius (stored divided by 400)
        } totempole;
        struct {
        /*18*/ u8 roll;
        /*19*/ u8 pitch;
        /*1A*/ u8 yaw;
        /*1B*/ u8 carryingGold; //Bitfield flags: carrying a Shiny Nugget when bit0 is set
        /*1C*/ f32 interactRadius; //Distance at which player can interact with tumbleweed
        } tumbleweed;
        struct {
        /*18*/ u8 speedX;
        /*19*/ u8 speedY;
        /*1A*/ u8 speedZ;
        /*1B*/ u8 unused1;
        /*1C*/ f32 unused2;
        } drBullet;
        struct {
        /*18*/ u8 unused1;
        /*19*/ u8 unused2;
        /*1A*/ u8 unused3;
        /*1B*/ u8 id;
        /*1C*/ f32 unused4;
        } nwIce;
    };
} Ctrl103_Setup;

#endif
