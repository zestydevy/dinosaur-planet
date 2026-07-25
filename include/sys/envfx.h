#ifndef _SYS_ENVFX_H
#define _SYS_ENVFX_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    u8  pad0[0xC];

    /* Fog Colour (R component) */
    u8 unkC;  //night phase 1
    u8 unkD;  //night phase 2
    u8 unkE;  //night phase 3
    u8 unkF;  //night phase 4
    u8 unk10; //day phase 1
    u8 unk11; //day phase 2
    u8 unk12; //day phase 3
    u8 unk13; //day phase 4

    /* Fog Colour (G component) */
    u8 unk14; //night phase 1
    u8 unk15; //night phase 2
    u8 unk16; //night phase 3
    u8 unk17; //night phase 4
    u8 unk18; //day phase 1
    u8 unk19; //day phase 2
    u8 unk1A; //day phase 3
    u8 unk1B; //day phase 4

    /* Fog Colour (B component) */
    u8 unk1C; //night phase 1
    u8 unk1D; //night phase 2
    u8 unk1E; //night phase 3
    u8 unk1F; //night phase 4
    u8 unk20; //day phase 1
    u8 unk21; //day phase 2
    u8 unk22; //day phase 3
    u8 unk23; //day phase 4

    u16 unk24; //actionID (indexed from 1)?
    u16 unk26; //precipitation-related?
    u16 unk28; //precipitation rate
    u16 unk2A; //precipitation-related?
    u16 unk2C; //precipitation-related?
    u8  unk2E[0x4E - 0x2E]; //Fog min/max distances/opacities at 8 different phases of day?
    u8  unk4E[0x59 - 0x4E];
    u8  unk59; //Affects fog colour cycling? Can blend based on camera direction instead of time of day?
    u8  unk5A; //Sun texture (1: flare, 2: yellow ball sun, 3: moon, 4: hazy/obscured)
    u8  unk5B;
    u8  unk5C; //Type?
    u8  unk5D; //Hills texture?
    u8  unk5E;
    u8  unk5F;
} EnvFxAction;

/** Applies EnvFxAction */
s32 envfxAction(Object *arg0, Object *arg1, u16 actionIndex, s32 arg3);
s32 envfxRestoreAction(Object *arg0, Object *arg1, u16 arg2, s32 arg3);

#endif
