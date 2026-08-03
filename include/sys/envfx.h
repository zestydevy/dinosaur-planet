#ifndef _SYS_ENVFX_H
#define _SYS_ENVFX_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
/*00*/ u8 _unk0[0x4 - 0x0];
/*04*/ f32 unk4;
/*08*/ f32 unk8;
       /** Colour (R component) */
/*0C*/ u8 r[8];
       /** Colour (G component) */
/*14*/ u8 g[8];
       /** Colour (B component) */
/*1C*/ u8 b[8];
/*24*/ s16 unk24; //actionID (indexed from 1)?
/*26*/ u16 unk26; //precipitation-related?
/*28*/ u16 unk28; //precipitation rate
/*2A*/ u16 unk2A; // transition rate (between actions)
/*2C*/ u16 unk2C; //precipitation-related?
       // TODO: this is used differently for newday
       /** Fog Position (max) for 8 phases of day */
/*2E*/ u16 fogMax[8];
       /** Fog Position (min) for 8 phases of day */
/*3E*/ u16 fogMin[8];
/*4E*/ u16 unk4E;
/*50*/ u16 unk50;
/*52*/ u16 unk52;
/*54*/ u16 unk54;
/*56*/ u16 unk56;
/*58*/ u8 unk58;
/*59*/ u8 unk59; //Affects fog colour cycling? Can blend based on camera direction instead of time of day?
/*5A*/ u8 unk5A; //Sun texture (1: flare, 2: yellow ball sun, 3: moon, 4: hazy/obscured)
/*5B*/ u8 unk5B;
/*5C*/ u8 type; // envfx type (3 = fog, 5 = day, 6 = minic, 1-2,4 = clouds)
/*5D*/ u8 unk5D; //Hills texture?
/*5E*/ u8 unk5E;
/*5F*/ u8 unk5F;
} EnvFxAction;

/** Applies EnvFxAction */
s32 envfxAction(Object *arg0, Object *arg1, u16 actionIndex, s32 arg3);
s32 envfxRestoreAction(Object *arg0, Object *arg1, u16 arg2, s32 arg3);

#endif
