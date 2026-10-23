#ifndef _SYS_GAME_OBJECTS_SETUPS_IMMULTISEQ_H
#define _SYS_GAME_OBJECTS_SETUPS_IMMULTISEQ_H

#include "game/objects/object.h"

#define IMMULTISEQ_GAMEBIT_COUNT 4

// ctrl: 91 | size: 0x34
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebits1[IMMULTISEQ_GAMEBIT_COUNT];
/*20*/ s16 gamebits2[IMMULTISEQ_GAMEBIT_COUNT];
/*28*/ u8 yaw;
/*29*/ u8 unk29;
/*2A*/ u8 modelInstIdx;
/*2B*/ u8 unk2B;
/*2C*/ s8 objectSeqIndex[IMMULTISEQ_GAMEBIT_COUNT];
/*30*/ u8 values;
} IMMultiSeq_Setup;

#endif
