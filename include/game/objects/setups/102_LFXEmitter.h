#ifndef _SYS_GAME_OBJECTS_SETUPS_LFXEMITTER_H
#define _SYS_GAME_OBJECTS_SETUPS_LFXEMITTER_H

#include "game/objects/object.h"

// ctrl: 102 | size: 0x28
typedef struct {
/*00*/    ObjSetup base;
/*18*/    s16 rollRate;
/*1A*/    s16 pitchRate;
/*1C*/    s16 yawRate;
/*1E*/    s16 actionIndex;
/*20*/    s16 lifetime;
/*22*/    s16 enableBit;
/*24*/    u8 followCurve;
/*25*/    s8 curveSpeed;
} LFXEmitter_Setup;

#endif
