#ifndef _SYS_GAME_OBJECTS_SETUPS_KYTETARGET_H
#define _SYS_GAME_OBJECTS_SETUPS_KYTETARGET_H

#include "game/objects/object.h"

// ctrl: 115 | size: 0x20
typedef struct {
/*00*/ ObjSetup base;
    union {
        struct {
        /*18*/ u8 commandRange; // find/flame command range
        /*19*/ u8 redFood;
        /*1A*/ u16 flightGroup;
        /*1C*/ u8 setting; // use varies
        /*1D*/ u8 unk1D;
        /*1E*/ u8 distCheckMode; //affects how player-to-perch distance is calculated (2D X/Z vs. full 3D)
        /*1F*/ u8 yaw;
        };
        struct {
        /*18*/ u8 commandRange;
        /*19*/ u8 timer;
        /*1A*/ u16 flightGroup;
        /*1C*/ u8 talkSeq;
        /*1D*/ u8 unk1D;
        /*1E*/ u8 distCheckMode;
        /*1F*/ u8 yaw;
        } findObj;
    };
} KyteTarget_Setup;

#endif
