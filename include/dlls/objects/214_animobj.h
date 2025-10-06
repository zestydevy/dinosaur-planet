#ifndef _DLLS_214_H
#define _DLLS_214_H

#include "PR/ultratypes.h"
#include "dlls/engine/3_animation.h"
#include "game/objects/object.h"
#include "dll_def.h"

typedef struct {
    ObjCreateInfo base;
    /** isObjSeq2CurveIndex (1 bit) | sequenceID (11 bits) | actorIndex (4 bits) */
    s16 sequenceIdBitfield;
    s16 unk1A;
    s16 unk1C;
    s8 unk1E;
    s8 unk1F;
    s16 unk20;
    s8 unk22;
    s8 unk23;
    u8 unk24;
} AnimObjCreateInfo;

typedef void (*AnimObjStateF4Callback)(Object *arg0, s32 arg1, s32 arg2);
typedef s32 (*AnimObjStateF8Callback)(Object *arg0, s32 arg1, s32 arg2);

typedef struct {
/*00*/ s8 unk0[0x24 - 0];
/*24*/ f32 unk24;
/*28*/ s32 unk28;
/*2C*/ s32 unk2C;
/*30*/ u32 unk30;
/*34*/ u32 unk34[4];
/*44*/ s8 unk44[0x62 - 0x44];
/*62*/ s8 unk62;
/*63*/ s8 unk63;
/*64*/ s16 animCurvesCurrentFrameA;
/*66*/ s16 animCurvesCurrentFrameB;
/*68*/ s16 animCurvesDuration;
/*6A*/ s16 unk6A;
/*6C*/ s16 unk6C;
/*6E*/ s16 animCurvesEventCount;
/*70*/ s16 animCurvesKeyframeCount;
/*72*/ s8 unk72[0x76 - 0x72];
/*76*/ s16 unk76;
/*78*/ s8 unk78[0x7A - 0x78];
/*7A*/ s16 unk7A;
/*7C*/ s8 unk7C[0x80 - 0x7C];
/*8B*/ s32 unk80;
/*7C*/ s8 unk84[0x8B - 0x84];
/*8B*/ s8 unk8B;
/*8C*/ s8 unk8C[0x8E - 0x8C];
/*8E*/ u8 unk8E[1];
/*8F*/ s8 unk8F[0x98 - 0x8F];
/*98*/ u8 unk98;
/*99*/ u8 unk99;
/*9A*/ u8 unk9A;
/*9B*/ u8 unk9B;
/*9C*/ s8 unk9C[0x9D - 0x9C];
/*9D*/ u8 unk9D;
/*9E*/ s8 unk9E[0xA0 - 0x9E];
/*A0*/ AnimCurvesEvent* animCurvesEvents;
/*A4*/ AnimCurvesKeyframe* animCurvesKeyframes;
/*A8*/ s16 channelFirstKeyIndex[ANIMCURVES_KEYFRAME_CHANNELS];
/*CE*/ s16 channelTotalKeys[ANIMCURVES_KEYFRAME_CHANNELS];
/*F4*/ AnimObjStateF4Callback unkF4;
/*F8*/ AnimObjStateF8Callback unkF8;
} AnimObjState;

#endif
