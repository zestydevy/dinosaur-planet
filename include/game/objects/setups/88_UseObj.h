#ifndef _SYS_GAME_OBJECTS_SETUPS_USEOBJ_H
#define _SYS_GAME_OBJECTS_SETUPS_USEOBJ_H

#include "game/objects/object.h"

// ctrl: 88 | size: 0x28
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 pitch;
/*1A*/ u8 roll;
/*1B*/ u8 flags;
/*1C*/ s16 gamebitUsed; // gamebit that says whether this object was already used
/*1E*/ s16 gamebitRequiredItem; // inventory item required to use this object
/*20*/ s8 objectSeqIndex;
/*21*/ s8 modelno;
/*22*/ s16 gamebitEnabled; // gamebit that says whether this object is enabled (and can be used)
/*24*/ s16 replayStartTime; // if not zero, the sequence will be replayed if the object was already used
} UseObj_Setup;

#endif
