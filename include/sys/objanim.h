#ifndef _SYS_OBJANIM_H
#define _SYS_OBJANIM_H

#include "game/objects/object.h"
#include "sys/gfx/animation.h"
#include "sys/gfx/model.h"

typedef struct {
    /*00*/    s16 X;
    /*02*/    s16 Y;
    /*04*/    s16 Z;
} AnimationRootMotionComponents;

typedef struct {
    /*00*/    f32 speed;
    /*04*/    s16 totalKeyframes;
    /*06*/    AnimationRootMotionComponents components;
    /*0C*/    s16 keyframes; //array
} AnimationRootMotion;

// size: 0x1C
typedef struct {
  f32 unk0[3];
  s16 unkC[3];
  u8 unk12;
  s8 unk13[8];
  s8 unk1B; // current length of unk13
} UnkFunc_80024108Struct;

s32 objAnimSet(Object* object, s32 modAnimIndex, f32 animProgress, u8 arg3);
s32 objAnimSetProgress(Object* object, f32 progress);
s32 objAnimAdvance(Object*, f32, f32 updateRate, UnkFunc_80024108Struct*);

s32 objGetAnimChange(Object* obj, f32 dist, f32* change);

void objAnim_func_80024D74(Object* object, s32 arg1);
void objAnim_func_80024DD0(Object* arg0, s32 animStateLayer, s16 arg2, s16 arg3);
s16 objAnim_func_80024E2C(Object* arg0);

s32 objAnimSetLayered(Object* object, s32 modanimIndex, f32 animProgress, u8 arg3);
s32 objAnimSetLayeredProgress(Object* object, f32 progress);
s32 objAnimAdvanceLayered(Object*, f32, f32, UnkFunc_80024108Struct*);

void objAnimSetBlend(Object* object, s32 modAnimBankAndIndex, s32 arg2);
void objAnimSetBlendLayered(Object* object, s32 modAnimBankAndIndex, s32 arg2);

void objAnim_func_80025780(Object*, f32 updateRate, UnkFunc_80024108Struct*, u16);
u8 objAnim_func_80025CD4(s32 arg0);

#endif
