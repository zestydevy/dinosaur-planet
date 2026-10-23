#ifndef _SYS_GAME_OBJECTS_SETUPS_LFXEMITTER_H
#define _SYS_GAME_OBJECTS_SETUPS_LFXEMITTER_H

#include "game/objects/object.h"

// ctrl: 102 | size: 0x28
typedef struct {
/*00*/    ObjSetup base;
/*18*/    s16 unk18;
/*1A*/    s16 unk1A;
/*1C*/    s16 unk1C;
/*1E*/    s16 unk1E;
/*20*/    s16 unk20;
/*22*/    s16 unk22;
/*24*/    s8 unk24;
/*25*/    s8 unk25;
/*26*/    s16 unk26;
} LFXEmitter_Setup;

#endif
