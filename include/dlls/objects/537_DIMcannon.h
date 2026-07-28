#ifndef _DLLS_537_H
#define _DLLS_537_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

/* ObjSetup for DIMCannon */
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

#endif //_DLLS_537_H
