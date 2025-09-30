#ifndef _SYS_OBJANIM_H
#define _SYS_OBJANIM_H

#include "common.h"
#include "sys/map.h"
#include "sys/gfx/animation.h"
#include "sys/gfx/model.h"

extern u8 D_800916B0[];
extern f32 gAnimBlendDivisor; //1023

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
  s16 unkc[3];
  u8 unk12;
  s8 unk13[8];
  s8 unk1B; // current length of unk13
} UnkFunc_80024108Struct;

s32 func_80023D30(Object* object, s32 modAnimIndex, f32 animProgress, u8 arg3);
s32 func_800240BC(Object* object, f32 progress);
// s32 func_80024108(Object*, f32, f32, UnkFunc_80024108Struct*);
s32 func_8002493C(void*, f32, void*);
void func_80024D74(Object* object, s32 arg1);
void func_80024DD0(Object* arg0, s32 animStateLayer, s16 arg2, s16 arg3);
s16 func_80024E2C(Object* arg0);
s32 func_80024E50(Object* object, s32 modanimIndex, f32 animProgress, u8 arg3);
s32 func_800250F4(Object* object, f32 progress);
void func_80025540(Object* object, s32 modAnimBankAndIndex, s32 arg2);
void func_8002559C(Object* object, s32 modAnimBankAndIndex, s32 arg2);
void func_800255F8(Model* model, AnimState* animState, s32 modanimIndex, s16 arg3);
void func_80025780(Object*, f32, UnkFunc_80024108Struct*, void*);
u8 func_80025CD4(s32 arg0);

#endif
