#ifndef _SYS_GAME_OBJECTS_SETUPS_NWMULTISEQ_H
#define _SYS_GAME_OBJECTS_SETUPS_NWMULTISEQ_H

#include "game/objects/object.h"

// ctrl: 92 | size: 0x48
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 playedBits[8];
/*28*/ s16 playBits[8];
/*38*/ u8 rotation;
/*39*/ u8 flags;
/*3A*/ u8 replaySeqIndex;
/*3B*/ u8 replayActorMask;
/*3C*/ s16 preemptTime;
/*3E*/ u8 _unk3E[0x40 - 0x3E];
/*40*/ s8 seqIndices[8];
} NWMultiSeq_Setup;

#endif
