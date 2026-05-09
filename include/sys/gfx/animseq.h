/** Anim sequence system
 */
#ifndef _SYS_GFX_ANIMSEQ_H
#define _SYS_GFX_ANIMSEQ_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/objanim.h"

#define ANIMCURVES_KEYFRAME_CHANNELS 19

typedef struct {
/*0*/ f32 value;
/*4*/ s8 interpolation;
/*5*/ s8 channel;
/*6*/ s16 timeOffset;
} AnimCurvesKeyframe;

typedef struct {
/*0*/ s8 type;
/*1*/ u8 delay;
/*2*/ s16 params;
} AnimCurvesEvent;

typedef struct {
    ObjSetup base;
    /** isObjSeq2CurveIndex (1 bit) | sequenceID (11 bits) | actorIndex (4 bits) */
    s16 sequenceIdBitfield;
    s16 unk1A;
    s16 unk1C;
    s8 unk1E;
    s8 unk1F;
    s8 unk20;
    s8 unk21;
    s8 unk22;
    s8 unk23;
    u8 unk24;
} AnimObj_Setup;

typedef void (*AnimObj_DataF4Callback)(Object *actor, Object *override, struct AnimObj_Data* arg2);
typedef s32 (*AnimObj_DataF8Callback)(Object *actor, Object *override, s32 arg2);

typedef struct {
    s32 unk0;
    s32 unk4;
    f32 unk8[8];
    f32 unk28;
} UnkAnimStruct;

typedef struct AnimObj_Data {
/*000*/ Object* actor;
/*004*/ f32 unk4;
/*008*/ f32 unk8;
/*00C*/ f32 unkC;
/*010*/ f32 unk10;
/*014*/ s16 unk14;
/*016*/ s16 unk16;
/*018*/ s16 unk18;
/*01A*/ s16 unk1A; //angle
/*004*/ s8 unk1C[0x20 - 0x1C];
/*020*/ f32 unk20;
/*024*/ f32 unk24; //some speed
/*028*/ s32 unk28;
/*02C*/ UnkAnimStruct *unk2C;
/*030*/ u32 unk30; //soundHandle?
/*034*/ u32 unk34[4];   //soundHandles?
/*044*/ s16 unk44[4];   //soundHandle related - playback frequency?
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
/*072*/ s16 unk72;
/*074*/ s16 unk74;
/*076*/ s16 unk76;
/*078*/ s16 unk78;
/*07A*/ s16 unk7A;
/*07C*/ s16 unk7C;
/*07E*/ s8 unk7E[0x80 - 0x7E];
/*08B*/ s32 unk80;
/*084*/ s8 unk84; // unk type
/*085*/ s8 unk85;
/*086*/ s8 unk86;
/*087*/ s8 unk87;
/*088*/ s8 unk88; // unk type
/*088*/ s8 unk89;
/*08A*/ s8 unk8A;                   //soundHandle-related
/*08B*/ u8 unk8B;
/*08C*/ u8 unk8C;
/*08D*/ u8 unk8D;
/*08E*/ u8 unk8E[0x98 - 0x8E];      //sequence subcommand IDs?
/*098*/ u8 unk98;                   //sequence subcommand count?
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
/*0FC*/ UnkFunc_80024108Struct unkFC;
/*118*/ s32 unk118;
/*11C*/ Object* unk11C;
/*120*/ s16 unk120;
/*122*/ s16 unk122;
/*124*/ s16 unk124[10];
/*138*/ u8 unk138[10];
/*142*/ u8 unk142_4: 4;
/*142*/ u8 unk142_0: 4;
/*143*/ s8 unk143[0x144 - 0x143];
} AnimObj_Data;

#endif
