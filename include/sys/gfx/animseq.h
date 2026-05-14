/** Anim sequence system
 */
#ifndef _SYS_GFX_ANIMSEQ_H
#define _SYS_GFX_ANIMSEQ_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/objanim.h"

#define ANIMCURVES_KEYFRAME_CHANNELS 19
#define MAX_DECISION 10
#define SEQSLOT_NONE (-1)
#define SEQSLOT_ANIMOBJ (-2)
// When given to start_obj_sequence, enables all actors in the sequence
#define OBJSEQ_ALL_ACTORS (-1)

enum AnimDecisionCondition {
    ANIM_DECISION_A_BUTTON = 18,
    ANIM_DECISION_B_BUTTON = 19,
    ANIM_DECISION_CUSTOM1 = 20,
    ANIM_DECISION_CUSTOM2 = 21,
    ANIM_DECISION_CUSTOM3 = 22,
    ANIM_DECISION_CUSTOM4 = 23,
    ANIM_DECISION_CUSTOM5 = 24,
    ANIM_DECISION_CUSTOM6 = 25
};

// TODO: better name
enum Field7AFlags {
    ANIM7AFLAG_OVERRIDE_POS = 0x1,
    ANIM7AFLAG_OVERRIDE_ROT = 0x2,
    ANIM7AFLAG_OVERRIDE_MODEL = 0x4,
    ANIM7AFLAG_OVERRIDE_HEAD = 0x8,
    ANIM7AFLAG_OVERRIDE_SCALE = 0x10,
    ANIM7AFLAG_OVERRIDE_OPACITY = 0x20,
    ANIM7AFLAG_OVERRIDE_EYES = 0x40,
    ANIM7AFLAG_UNK100 = 0x100,
    ANIM7AFLAG_OVERRIDE_JAW = 0x200,
    ANIM7AFLAG_UNK400 = 0x400
};

typedef struct {
/*0*/ f32 value;
/*4*/ s8 interpolation;
/*5*/ s8 channel;
/*6*/ s16 timeOffset;
} AnimCurvesKeyframe;

typedef struct {
/*0*/ s8 type;
/*1*/ u8 delay;
/*2*/ s16 params; // official name: val
} AnimCurvesEvent;

typedef struct {
    ObjSetup base;
    /** isObjSeq2CurveIndex (1 bit) | sequenceID (11 bits) | actorIndex (4 bits) */
    s16 sequenceIdBitfield;
    s16 unk1A;
    s16 target; // target (actor) object ID + 4
    s8 unk1E;
    s8 seqSlot;
    s8 unk20;
    s8 unk21;
    s8 unk22;
    s8 unk23;
    u8 unk24;
} AnimObj_Setup;

typedef void (*AnimObj_DataF4Callback)(Object *actor, Object *override, struct AnimObj_Data* arg2);
typedef s32 (*AnimObj_DecisionCallback)(Object *actor, Object *override, s32 arg2);

typedef struct {
    s32 unk0;
    s32 unk4;
    f32 unk8[8];
    f32 unk28;
} UnkAnimStruct;

typedef struct AnimObj_Data {
/*000*/ Object* actor; // the target of the anim obj (e.g. the target of an Override object)
/*004*/ f32 unk4;
/*008*/ f32 unk8;
/*00C*/ f32 unkC;
/*010*/ f32 unk10;
/*014*/ s16 unk14;
/*016*/ s16 unk16;
/*018*/ s16 unk18;
/*01A*/ s16 seqYaw; // base yaw of the whole object sequence?
/*004*/ s8 unk1C[0x20 - 0x1C];
/*020*/ f32 unk20;
/*024*/ f32 unk24; //some speed
/*028*/ s32 unk28;
/*02C*/ UnkAnimStruct *unk2C;
/*030*/ u32 unk30; //soundHandle?
/*034*/ u32 sfxHandles[4]; // sfx sound handles
/*044*/ s16 sfxTimer[4]; // timer to end sfx early. if 32000, sfx will not end early
/*04C*/ Vec3f unk4C; //position diff between parent "override" animObj and child "actor" object?
/*058*/ f32 unk58;
/*05C*/ s16 yawDiff;
/*05E*/ s16 pitchDiff;
/*060*/ s16 rollDiff;
/*062*/ s8 unk62;
/*063*/ s8 seqSlot;
/*064*/ s16 time;
/*066*/ s16 prevTime;
/*068*/ s16 duration;
/*06A*/ s16 unk6A;
/*06C*/ s16 counter;
/*06E*/ s16 animCurvesEventCount;
/*070*/ s16 animCurvesKeyframeCount;
/*072*/ s16 eventIdx;
/*074*/ s16 eventTime;
/*076*/ s16 eventGamebit; // gamebit toggleable by anim events
/*078*/ s16 modAnimIdx;
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
/*08A*/ s8 sfxNextSlot; // next free sfx handle slot
/*08B*/ u8 unk8B; // "activation" state?
/*08C*/ u8 unk8C;
/*08D*/ u8 lastMessage;
/*08E*/ u8 messages[10];
/*098*/ u8 messageCount;
/*099*/ u8 modAnimStartFrame;
/*09A*/ u8 blinkFrameR; // eyelid frame for right eye
/*09B*/ u8 blinkFrameL; // eyelid frame for left eye
/*09C*/ s8 unk9C[0x9D - 0x9C];
/*09D*/ u8 unk9D; // TODO: flags?
/*09E*/ s8 unk9E[0xA0 - 0x9E];
/*0A0*/ AnimCurvesEvent* animCurvesEvents;
/*0A4*/ AnimCurvesKeyframe* animCurvesKeyframes;
/*0A8*/ s16 channelFirstKeyIndex[ANIMCURVES_KEYFRAME_CHANNELS]; // global index of first keyframe per channel 
/*0CE*/ s16 channelTotalKeys[ANIMCURVES_KEYFRAME_CHANNELS]; // total number of keyframes per channel
/*0F4*/ AnimObj_DataF4Callback unkF4; //end-of-sequence callback function
/*0F8*/ AnimObj_DecisionCallback decisionCallback;
/*0FC*/ UnkFunc_80024108Struct unkFC;
/*118*/ s32 actorUID;
/*11C*/ Object* unk11C;
/*120*/ s16 unk120;
/*122*/ s16 unk122;
/*124*/ s16 decisionTimes[MAX_DECISION]; // timestamp of decision target
/*138*/ u8 decisionConditions[MAX_DECISION]; // condition of decision
/*142*/ u8 unk142_4: 4;
/*142*/ u8 unk142_0: 4;
/*143*/ s8 unk143[0x144 - 0x143];
} AnimObj_Data;

#endif
