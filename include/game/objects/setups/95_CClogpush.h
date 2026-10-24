#ifndef _SYS_GAME_OBJECTS_SETUPS_CCLOGPUSH_H
#define _SYS_GAME_OBJECTS_SETUPS_CCLOGPUSH_H

#include "game/objects/object.h"

// ctrl: 95 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebit1;
/*1A*/ s16 gamebit2;
/*1C*/ s16 gamebit3;
/*1E*/ s16 unk1E;
/*20*/ u8 unk20;
/*21*/ u8 objType; // 11
/*22*/ u8 objectSeqIndex;
/*23*/ u8 flags;
} CClogpush_Setup;

#endif
