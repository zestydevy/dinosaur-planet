#ifndef _DLLS_214_H
#define _DLLS_214_H

#include "PR/ultratypes.h"
#include "dlls/engine/3_animation.h"
#include "game/objects/object.h"
#include "dll_def.h"

typedef struct {
    ObjSetup base;
    /** isObjSeq2CurveIndex (1 bit) | sequenceID (11 bits) | actorIndex (4 bits) */
    s16 sequenceIdBitfield;
    s16 unk1A;
    s16 unk1C;
    s8 unk1E;
    s8 unk1F;
    s8 unk20;
    u8 pad21;
    s8 unk22;
    s8 unk23;
    u8 unk24;
} AnimObjSetup;

typedef void (*AnimObj_DataF4Callback)(Object *arg0, s32 arg1, struct AnimObj_Data* arg2);
typedef s32 (*AnimObj_DataF8Callback)(Object *arg0, Object *override, s32 arg2);

typedef struct AnimObj_Data {
/*000*/ Object* actor;
/*004*/ s8 unk4[0x24 - 4];
/*024*/ f32 unk24; //some speed
/*028*/ s32 unk28;
/*02C*/ void *unk2C;
/*030*/ u32 unk30;
/*034*/ u32 unk34[4];
/*044*/ s8 unk44[0x4C - 0x44];
/*04C*/ Vec3f unk4C; //position diff between parent "override" animObj and child "actor" object?
/*058*/ f32 unk58;
/*05C*/ s16 yawDiff;
/*05E*/ s16 pitchDiff;
/*060*/ s16 rollDiff;
/*062*/ s8 unk62;
/*063*/ s8 unk63;
/*064*/ s16 animCurvesCurrentFrameA;
/*066*/ s16 animCurvesCurrentFrameB;
/*068*/ s16 animCurvesDuration;
/*06A*/ s16 unk6A;
/*06C*/ s16 unk6C;
/*06E*/ s16 animCurvesEventCount;
/*070*/ s16 animCurvesKeyframeCount;
/*072*/ s8 unk72[0x76 - 0x72];
/*076*/ s16 unk76;
/*078*/ s8 unk78[0x7A - 0x78];
/*07A*/ s16 unk7A;
/*07C*/ s16 unk7C;
/*07E*/ s8 unk7E[0x80 - 0x7E];
/*08B*/ s32 unk80;
/*084*/ s8 unk84[0x87 - 0x84];
/*087*/ s8 unk87;
/*088*/ s8 unk88[0x8B - 0x88];
/*08B*/ u8 unk8B;
/*08C*/ s8 unk8C;
/*08D*/ u8 unk8D;
/*08E*/ u8 unk8E[0x98 - 0x8E];
/*098*/ u8 unk98;
/*099*/ u8 unk99;
/*09A*/ u8 unk9A;
/*09B*/ u8 unk9B;
/*09C*/ s8 unk9C[0x9D - 0x9C];
/*09D*/ u8 unk9D;
/*09E*/ s8 unk9E[0xA0 - 0x9E];
/*0A0*/ AnimCurvesEvent* animCurvesEvents;
/*0A4*/ AnimCurvesKeyframe* animCurvesKeyframes;
/*0A8*/ s16 channelFirstKeyIndex[ANIMCURVES_KEYFRAME_CHANNELS];
/*0CE*/ s16 channelTotalKeys[ANIMCURVES_KEYFRAME_CHANNELS];
/*0F4*/ AnimObj_DataF4Callback unkF4; //end-of-sequence callback function
/*0F8*/ AnimObj_DataF8Callback unkF8;
/*0FC*/ s8 unkFC[0x11C - 0xFC];
/*11C*/ Object* unk11C;
/*120*/ s8 unk120[0x144 - 0x120];
} AnimObj_Data;

#endif
