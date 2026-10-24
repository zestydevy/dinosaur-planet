#ifndef _SYS_GAME_OBJECTS_SETUPS_DIMCANNON_H
#define _SYS_GAME_OBJECTS_SETUPS_DIMCANNON_H

#include "game/objects/object.h"

// ctrl: 125 | size: 0x2c
typedef struct {
/*00*/  ObjSetup base;
/*18*/  s16 gamebitSiloCoverOpen;
/*1A*/  s16 gamebitCannonClawDead;
/*1C*/  s16 gamebitCannonClawAboard;
/*1E*/  s16 gamebitCannonClawTruce;
/*20*/  s16 gamebitSiloEnter;
/*22*/  s16 gamebitSiloExit;
/*24*/  s16 unk24;
/*26*/  s16 hostileRange;
/*28*/  s8 yaw;
/*29*/  u8 cooldownMin;
/*2A*/  u8 cooldownMax;
/*2B*/  u8 rangeSiloRetreat;
} DIMCannon_Setup;

#endif
