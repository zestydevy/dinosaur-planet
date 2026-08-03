#ifndef _SYS_ENVFX_H
#define _SYS_ENVFX_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    u8  _unk0[0x4 - 0x0];
    f32 unk4;
    f32 unk8;

    /* Fog Colour (R component) */
    u8 unkC[8];
    /* Fog Colour (G component) */
    u8 unk14[8];
    /* Fog Colour (B component) */
    u8 unk1C[8];

    s16 unk24; //actionID (indexed from 1)?
    u16 unk26; //precipitation-related?
    u16 unk28; //precipitation rate
    u16 unk2A; //precipitation-related?
    u16 unk2C; //precipitation-related?
    //Fog min/max distances/opacities at 8 different phases of day?
    u16 unk2E[8];
    u16 unk3E[8];
    u16 unk4E;
    u16 unk50;
    u16 unk52;
    u16 unk54;
    u16 unk56;
    u8 unk58;
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
