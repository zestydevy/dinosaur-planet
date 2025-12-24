#include "game/objects/object.h"
#include "common.h"

#include "common.h"
#include "macros.h"
#include "variables.h"
#include "sys/gfx/model.h"
#include "sys/linked_list.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/objtype.h"

static const char str_80099800[] = "Error in loading Model Inst in ObjAnimSetMove ";

Animation* func_80019118(s16 animID, s16 modAnimID, u8* amap, Model* model);

//sets new modAnimIndex on model
s32 func_80023D30(Object* object, s32 modAnimIndex, f32 animProgress, u8 arg3) {
    s32 temp_t0;
    Model* model;
    u8 changed;
    AnimState* animState;
    ModelInstance* modelInstance;
    Animation* anim;
    s32 pad;

    if (animProgress > 1.0f) {
        animProgress = 1.0f;
    } else if (animProgress < 0.0f) {
        animProgress = 0.0f;
    }
    object->animProgress = animProgress;

    modelInstance = object->modelInsts[object->modelInstIdx];
    if (!modelInstance)
        return 0;

    model = modelInstance->model;
    if (model->animCount == 0)
        return 0;

    animState = modelInstance->animState0;
    animState->unk62[1] = arg3;
    animState->animIndexes[1] = animState->animIndexes[0];
    animState->curAnimationFrame[1] = animState->curAnimationFrame[0];
    animState->totalAnimationFrames[1] = animState->totalAnimationFrames[0];
    animState->unkC[1] = animState->unkC[0];
    animState->unk34[1] = animState->unk34[0];
    animState->unk60[1] = animState->unk60[0];
    animState->unk48[1] = animState->unk48[0];
    animState->unk3C[1] = animState->unk3C[0];
    animState->unk5C[0] = animState->unk58[1];
    animState->unk58[1] = 0;
    animState->modAnimIdBlend = -1;
    
    if (object->objhitInfo && object->objhitInfo->unk8) {
        func_80026AB8(object, modelInstance, object->id, object->objhitInfo, modAnimIndex, 0);
    }
    
    if (object->curEvent) {
        obj_load_event(object, object->id, object->curEvent, modAnimIndex, /*dontQueueLoad=*/FALSE);
    }
    
    changed = modAnimIndex != object->curModAnimId;
    object->curModAnimId = modAnimIndex;
    modAnimIndex = (model->modAnimBankBases[(modAnimIndex >> 8)]) + (u8)modAnimIndex;
    
    if (modAnimIndex >= model->animCount) {
        modAnimIndex = model->animCount - 1;
    }
    if (modAnimIndex < 0) {
        modAnimIndex = 0;
    }
    
    if (model->unk71 & 0x40) {
        if (changed) {
            animState->unk62[0] = (1 - animState->unk62[0]);
            animState->animIndexes[0] = animState->unk62[0] & 0xFFFF;
            func_80019118(model->modAnim[modAnimIndex], modAnimIndex, animState->anims[(u16)animState->unk62[0]]->boneRemaps, model);
        }
        anim = &animState->anims[animState->animIndexes[0]]->anim;
    } else {
        animState->animIndexes[0] = modAnimIndex;
        anim = model->anims[(u16)modAnimIndex];
    }
    
    animState->unk34[0] = &anim->animHeader;
    animState->unk60[0] = anim->unk1 & 0xF0;
    animState->totalAnimationFrames[0] = animState->unk34[0]->totalKeyframes;
    
    if (animState->unk60[0] == 0) {
        animState->totalAnimationFrames[0] -= 1.0f;
    }
    
    temp_t0 = anim->unk1 & 0xF;
    if (temp_t0 && !(arg3 & 0x10)) {
        animState->unkC[1] = animState->unkC[0];
        animState->unk5C[1] = 0x3FF / temp_t0;
        animState->unk58[0] = 0x3FF;
    } else {
        animState->unk58[0] = 0;
    }
    
    animState->unkC[0] = 0.0f;
    animState->curAnimationFrame[0] = animState->totalAnimationFrames[0] * animProgress;
    
    if (object->linkedObject && object->linkedObject->group == GROUP_UNK48) {
        ((ObjectAnim_Data_2*)object->linkedObject->data)->unk84 &= ~1;
    }
    
    return 0;
}

s32 func_800240BC(Object* object, f32 progress) {
    if (progress > 0.999f){
        progress = 0.999f; 
    } else if (progress < 0.0f) {
        progress = 0.0f;
    }
    object->animProgress = progress;
    return 0;
}

//advances current animation playback
#pragma GLOBAL_ASM("asm/nonmatchings/objanim/func_80024108.s")

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

// extern f32 gAnimBlendDivisor; //1023

//https://decomp.me/scratch/zIGpo
#pragma GLOBAL_ASM("asm/nonmatchings/objanim/func_8002493C.s")

void func_80024D74(Object* object, s32 arg1) {
    ModelInstance* model;
    AnimState* animState;

    model = object->modelInsts[object->modelInstIdx];
    if (model != NULL) {
        animState = model->animState0;
        animState->unk5C[1] = 0x3FF / arg1;
    }
}

void func_80024DD0(Object* arg0, s32 animStateLayer, s16 arg2, s16 arg3) {
    AnimState *animState;
    ModelInstance* model;

    model = arg0->modelInsts[arg0->modelInstIdx];
    if (!model)
        return;
        
    animState = animStateLayer ? model->animState1 : model->animState0;
    animState->unk58[arg2] = arg3;
}

s16 func_80024E2C(Object* arg0) {
  AnimState *animState;
  ModelInstance *model;
    
  model = arg0->modelInsts[arg0->modelInstIdx];
  animState = model->animState0;
  return animState->unk58[0];
}

s32 func_80024E50(Object* object, s32 modanimIndex, f32 animProgress, u8 arg3) {
    s32 temp_t9;
    u8 curModanim_layered;
    Model* model;
    AnimState* animState;
    Animation* anim;

    if (animProgress > 1.0f) {
        animProgress = 1.0f;
    } else if (animProgress < 0.0f) {
        animProgress = 0.0f;
    }
    object->animProgressLayered = animProgress;
    
    model = object->modelInsts[object->modelInstIdx]->model;
    if (model->animCount == 0) {
        return 0;
    }
    
    animState = object->modelInsts[object->modelInstIdx]->animState1;

    animState->animIndexes[1] = animState->animIndexes[0];
    animState->unk62[1] = (u8)arg3;
    animState->curAnimationFrame[1] = animState->curAnimationFrame[0];
    animState->totalAnimationFrames[1] = animState->totalAnimationFrames[0];
    animState->unkC[1] = animState->unkC[0];
    animState->unk34[1] = animState->unk34[0];
    animState->unk60[1] = animState->unk60[0];
    animState->unk48[1] = animState->unk48[0];
    animState->unk3C[1] = animState->unk3C[0];
    animState->unk5C[0] = animState->unk58[1];
    animState->unk58[1] = 0;
    animState->modAnimIdBlend = -1;
    
    curModanim_layered = modanimIndex != object->curModAnimIdLayered;
    object->curModAnimIdLayered = modanimIndex;
    
    modanimIndex = (model->modAnimBankBases[(modanimIndex >> 8)]) + (u8)modanimIndex;
    
    if (modanimIndex >= model->animCount) {
        modanimIndex = model->animCount - 1;
    }
    if (modanimIndex < 0) {
        modanimIndex = 0;
    }
    
    if (model->unk71 & 0x40) {
        if (curModanim_layered) {
            animState->unk62[0] = 1 - animState->unk62[0];
            animState->animIndexes[0] = animState->unk62[0];
            func_80019118(model->modAnim[modanimIndex], modanimIndex, animState->anims[(u16)animState->animIndexes[0]]->boneRemaps, model);
        }
        anim = (Animation*)(&animState->anims[animState->animIndexes[0]]->anim);
    } else {
        animState->animIndexes[0] = modanimIndex;
        anim = model->anims[(u16)modanimIndex];
    }

    animState->unk34[0] = &anim->animHeader;
    animState->unk60[0] = anim->unk1 & 0xF0;
    animState->totalAnimationFrames[0] = animState->unk34[0]->totalKeyframes;
    
    if (animState->unk60[0] == 0) {
        animState->totalAnimationFrames[0] -= 1.0f;
    }
    temp_t9 = anim->unk1 & 0xF;
    if (temp_t9 != 0) {
        animState->unkC[1] = animState->unkC[0];
        animState->unk5C[1] = (0x3FF / temp_t9);
        animState->unk58[0] = 0x3FF;
    }
    animState->unkC[0] = 0.0f;
    animState->curAnimationFrame[0] = animState->totalAnimationFrames[0] * animProgress;
    return 0;
}

s32 func_800250F4(Object* object, f32 progress) {
    if (progress > 0.999f){
        progress = 0.999f;
    } else if (progress < 0.0f){
        progress = 0.0f;
    }
    object->animProgressLayered = progress;
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/objanim/func_80025140.s")

void func_800255F8(Model* model, AnimState* animState, s32 bank_and_index, s16 arg3);

void func_80025540(Object* object, s32 modAnimBankAndIndex, s32 arg2) {
    ModelInstance* modelInstance;
    Model* model;

    modelInstance = object->modelInsts[object->modelInstIdx];
    model = modelInstance->model;
    if (model->animCount) {
        func_800255F8(model, modelInstance->animState0, modAnimBankAndIndex, arg2);
    }
}

void func_8002559C(Object* object, s32 modAnimBankAndIndex, s32 arg2) {
    ModelInstance* modelInstance;
    Model* model;

    modelInstance = object->modelInsts[object->modelInstIdx];
    model = modelInstance->model;
    if (model->animCount) {
        func_800255F8(model, modelInstance->animState1, modAnimBankAndIndex, arg2); //"probably ObjSetBlendMove"
    }
}

Animation* func_80019118(s16 animID, s16 modAnimID, u8* amap, Model* model);

void func_800255F8(Model* model, AnimState* animState, s32 modanimIndex, s16 arg3) {
    f32 var_fv0;
    s16 temp_v0;
    s8 temp_v0_2;
    s32 temp_t1;
    Animation* anim;

    modanimIndex = model->modAnimBankBases[modanimIndex >> 8] + (modanimIndex & 0xFF);
    
    if (modanimIndex >= model->animCount) {
        modanimIndex = model->animCount - 1;
    }
    if (modanimIndex < 0) {
        modanimIndex = 0;
    }
    
    if (model->unk71 & 0x40) {
        if (modanimIndex != animState->modAnimIdBlend) {
            temp_v0_2 = animState->unk62[0];
            animState->unk48[1] = 1 - temp_v0_2;
            animState->unk48[0] = temp_v0_2;
            func_80019118(model->modAnim[modanimIndex], modanimIndex, (u8*)animState->anims2[(u16)temp_v0_2], model);

            animState->modAnimIdBlend = modanimIndex;
        }
        anim = (Animation*)(&animState->anims2[animState->unk48[0]]->anim);
    } else {
        animState->unk48[0] = modanimIndex;
        anim = model->anims[(u16)modanimIndex];
    }
    
    animState->unk3C[0] = (AnimationHeader*) (&anim->animHeader);
    temp_t1 = anim->unk1 & 0xF0;
    if (temp_t1 != animState->unk60[0]) {
        animState->unk58[1] = 0;
        return;
    }
    
    var_fv0 = (u8)animState->unk3C[0]->totalKeyframes; 
    if (temp_t1 == 0) {
        var_fv0 -= 1.0f;
    }
    if (var_fv0 != animState->totalAnimationFrames[0]) {
        animState->unk58[1] = 0;
        return;
    }
    animState->unk58[1] = arg3;
}

#pragma GLOBAL_ASM("asm/nonmatchings/objanim/func_80025780.s")

extern u8 D_800916B0[];

u8 func_80025CD4(s32 arg0) {
    if (arg0 >= 0x21)
        return 0;
    return D_800916B0[arg0];
}