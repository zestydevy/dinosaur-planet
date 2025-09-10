#include "game/objects/object.h"
#include "common.h"

#include "common.h"
#include "macros.h"
#include "variables.h"
#include "sys/gfx/model.h"
#include "sys/linked_list.h"
#include "sys/objects.h"
#include "sys/objtype.h"

static const char str_80099800[] = "Error in loading Model Inst in ObjAnimSetMove ";

#pragma GLOBAL_ASM("asm/nonmatchings/objanim/func_80023D30.s")

s32 func_800240BC(Object* object, f32 progress) {
    if (progress > 0.999f){
        progress = 0.999f; 
    } else if (progress < 0.0f) {
        progress = 0.0f;
    }
    object->animProgress = progress;
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/objanim/func_80024108.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/objanim/func_8002493C.s")
#else
extern f32 D_80099838; //1023

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

/*
    a0 = krystal
    a1 = 3D8F142C (0.06986269)
    a2 = [0.04, 0.14] (seem to be animation playback speeds!)
*/
s32 func_8002493C(Object* object, f32 arg1, f32* arg2) {
    f32 speedScaledA;
    f32 progress;
    f32 remainingProgress;
    f32 temp_fa1;
    f32 animProgressPerKey;
    f32 lastKeyIndex_f;
    f32 scale;
    f32 curKeyIndex_f;
    f32 keyframeDelta_f;
    f32 var_fa0;
    f32 speedScaledB;
    f32 var_fv0;
    f32 var_fv1;
    f32* rootSpeed;
    s16 temp_a3;
    s16 rootMotion_offset2;
    s16 endValue;
    s16 temp_t6;
    s16 temp_t7;
    s16 component;
    s16* var_v1;
    s32 lastKeyIndex;
    s32 keyIndex;
    s32 var_t1;
    s32 var_t1_2;
    AnimState* animState;
    s16* key1;
    ModelInstance* modelInstance;
    Model* model;
    AnimationRootMotion* rootMotion2;
    s16* keyframes2;
    s16* key2;
    s16* keyframes1;
    Animation* anim;
    Animation* anim2;

    modelInstance = object->modelInsts[object->modelInstIdx];
    model = modelInstance->model;
    if (model->animCount <= 0)
        return 0;
    
    scale = object->srt.scale;
    animState = modelInstance->animState0;
    keyframes1 = NULL;
    var_t1 = 0;
    arg1 *= scale / object->def->scale;
    temp_a3 = animState->blendStrength_A;
    
    if (temp_a3 != 0) {
        progress = (f32) temp_a3 / D_80099838;
        remainingProgress = 1.0f - progress;
        if (model->unk_0x71 & 0x40) {
            anim = (Animation*)(&animState->anims2[animState->unk_0x48]->anim);
        } else {
            anim = model->anims[animState->unk_0x48];
        }

        //Checking the XYZ components in the root motion header, to see which axis the root motion moves along
        if (anim->offset_rootMotion != 0) {
            rootSpeed = (f32*)(&anim + anim->offset_rootMotion);
            keyframes1 = (s16*)(&rootSpeed + 6);
            if (0) { }
            speedScaledA = *rootSpeed * scale;

            //not sure whether to use struct access or something else here
            if (keyframes1[0] == 0) {
                temp_t7 = keyframes1[1];
                keyframes1 += 2;
                if (temp_t7 == 0) {
                    temp_t6 = keyframes1[1];
                    keyframes1 += 2;
                    if (temp_t6 == 0) {
                        keyframes1 = NULL;
                    }
                }
            }

            //If an XYZ component is in use, keyframes1 points to the keyframes for the root motion
            if (keyframes1 != NULL) {
                keyframes1 += 2;
            }
        }
    }
    
    if (model->unk_0x71 & 0x40) {
        anim2 = (Animation*)(&animState->anims[animState->animIndexA]->anim);
    } else {
        anim2 = model->anims[animState->animIndexA];
    }
    
    rootMotion_offset2 = anim2->offset_rootMotion;
    rootMotion2 = (AnimationRootMotion*)(&anim2 + rootMotion_offset2);
    
    if (rootMotion_offset2 == NULL) 
        return 0;    
        
    component = rootMotion2->components.X;
    speedScaledB = rootMotion2->speed * scale;
    var_v1 = (s16*)(&rootMotion2 + 6);
    lastKeyIndex = rootMotion2->totalKeyframes - 1;
    if (component != 0) {
        var_t1 = 1;
    } else {
        var_v1 += 2;
        component = *var_v1;
    }
    if ((var_t1 == 0) && (component == 0)) {
        var_v1 += 2;
        component = *var_v1;
    }
    keyframes2 = var_v1 + 2;
    
    if (component != 0) {
        endValue = keyframes2[lastKeyIndex];
        if (endValue < 0) {
            speedScaledB = -speedScaledB;
        }
        if (endValue == 0) {
            return 0;
        }
        
        lastKeyIndex_f = (f32) lastKeyIndex;
        animProgressPerKey = 1.0f / lastKeyIndex_f;
        curKeyIndex_f = object->animProgress * lastKeyIndex_f;
        keyIndex = (s32) curKeyIndex_f;
        key2 = &keyframes2[keyIndex];
        keyframeDelta_f = curKeyIndex_f - (f32) keyIndex;
        if (keyframes1 != NULL) {
            if (keyframes1[lastKeyIndex] < 0) {
                speedScaledA = -speedScaledA;
            }
            key1 = &keyframes1[keyIndex];
            var_fa0 = (key2[0] * remainingProgress * speedScaledB) + (key1[0] * progress * speedScaledA);
            var_fv0 = (key2[1] * remainingProgress * speedScaledB) + (key1[1] * progress * speedScaledA);
        } else {
            key2 = &keyframes2[keyIndex];
            var_fa0 = key2[0] * speedScaledB;
            var_fv0 = key2[1] * speedScaledB;
        }

        
        var_t1_2 = 0;
        var_fv1 = animProgressPerKey - (animProgressPerKey * keyframeDelta_f);
        temp_fa1 = ((var_fv0 - var_fa0) * keyframeDelta_f) + var_fa0 + arg1;
        do {
            if (temp_fa1 < var_fv0) {
                var_t1_2 = 1;
                var_fv1 -= ((var_fv0 - temp_fa1) * animProgressPerKey) / (var_fv0 - var_fa0);
            } else {
                keyIndex += 1;
                key2 += 1;
                if (keyIndex >= lastKeyIndex) {
                    keyIndex = 0;
                    key2 = keyframes2;
                }
                var_fa0 = var_fv0;
                if (keyframes1 != NULL) {
                    key1 = &keyframes1[keyIndex];
                    var_fv0 += (((f32) key2[1] - (f32) key2[0]) * speedScaledB * remainingProgress) + (((f32) key1[1] - (f32) key1[0]) * speedScaledA * progress);
                } else {
                    var_fv0 += ((f32) key2[1] - (f32) key2[0]) * speedScaledB;
                }
                var_fv1 += animProgressPerKey;
            }
        } while (var_t1_2 == 0);
        
        if (arg2 != NULL) {
            *arg2 = var_fv1;
        }
        
        return 1;
    }
    return 0;

}

#endif

void func_80024D74(Object* object, s32 arg1) {
    ModelInstance* model;
    AnimState* animState;

    model = object->modelInsts[object->modelInstIdx];
    if (model != NULL) {
        animState = model->animState0;
        animState->unk_0x5c[1] = 0x3FF / arg1;
    }
}

void func_80024DD0(Object* arg0, s32 animStateLayer, s16 arg2, s16 arg3) {
    AnimState *animState;
    ModelInstance* model;

    model = arg0->modelInsts[arg0->modelInstIdx];
    if (!model)
        return;
        
    animState = animStateLayer ? model->animState1 : model->animState0;
    animState->unk_0x58[arg2] = arg3;
}

s16 func_80024E2C(Object* arg0) {
  AnimState *animState;
  ModelInstance *model;
    
  model = arg0->modelInsts[arg0->modelInstIdx];
  animState = model->animState0;
  return animState->unk_0x58[0];
}

Animation* func_80019118(s16 animID, s16 modAnimID, u8* amap, Model* model);

#pragma GLOBAL_ASM("asm/nonmatchings/objanim/func_80024E50.s")
/** 
  * setting animation blending for layered/additive animation? 
  * (like the weapon stowing animation applied additively to the main body animation)
  */
// s32 func_80024E50(Object* object, s32 modAnimIndex, f32 animProgress, u8 arg3) {
//     s32 temp_t9;
//     u8 curModAnimLayered;
//     Model* model;
//     AnimState* animState;
//     Animation* anim;

//     if (animProgress > 1.0f) {
//         animProgress = 1.0f;
//     } else if (animProgress < 0.0f) {
//         animProgress = 0.0f;
//     }
//     object->animProgressLayered = animProgress;
    
//     model = object->modelInsts[object->modelInstIdx]->model;
//     if (model->animCount == 0) {
//         return 0;
//     }
    
//     animState = object->modelInsts[object->modelInstIdx]->animState1;

//     animState->unk46 = animState->unk44;
//     animState->unk_0x63 = (u8)arg3;
//     animState->unk_0x8 = animState->unk_0x4;
//     animState->unk_0x18 = animState->unk_0x14;
//     animState->unk_0x10 = animState->unk_0xc;
//     animState->animHeaderB = animState->animHeaderA;
//     animState->unk_0x61 = animState->unk_0x60;
//     animState->unk4a = animState->unk48;
//     animState->animHeaderD = animState->animHeaderC;
//     animState->unk_0x5c = (u16)animState->unk_0x5a;
//     animState->unk_0x5a = 0;
//     animState->modAnimIdBlend = -1;
    
//     curModAnimLayered = modAnimIndex != object->curModAnimIdLayered;
//     object->curModAnimIdLayered = modAnimIndex;
    
//     modAnimIndex = model->modAnimBankBases[(modAnimIndex >> 8)] + (u8)modAnimIndex;
    
//     if (modAnimIndex >= model->animCount) {
//         modAnimIndex = model->animCount - 1;
//     }
//     if (modAnimIndex < 0) {
//         modAnimIndex = 0;
//     }
    
//     if (model->unk_0x71 & 0x40) {
//         if (curModAnimLayered) {
//             animState->unk_0x62 = 1 - animState->unk_0x62;
//             animState->unk44 = animState->unk_0x62;
//             func_80019118(model->modAnim[modAnimIndex], modAnimIndex, animState->anims[(u16)animState->unk44]->boneRemaps, model);
//         }
//         anim = (Animation*)(&animState->anims[animState->unk44]->anim);
//     } else {
//         animState->unk44 = modAnimIndex;
//         anim = model->anims[(u16)modAnimIndex];
//     }

//     animState->animHeaderA = &anim->animHeader;
//     animState->unk_0x60 = anim->unk_0x1 & 0xF0;
//     animState->unk_0x14 = animState->animHeaderA->totalKeyframes;
    
//     if (animState->unk_0x60 == 0) {
//         animState->unk_0x14 = animState->unk_0x14 - 1.0f;
//     }
//     temp_t9 = anim->unk_0x1 & 0xF;
//     if (temp_t9 != 0) {
//         animState->unk_0x10 = animState->unk_0xc;
//         animState->unk_0x5e = (0x3FF / temp_t9);
//         animState->unk_0x58 = 0x3FF;
//     }
//     animState->unk_0xc = 0.0f;
//     animState->unk_0x4 = animState->unk_0x14 * animProgress;
//     return 0;
// }

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

#pragma GLOBAL_ASM("asm/nonmatchings/objanim/func_800255F8.s")
/*
    a0: 8069F158 (Model)
    a1: 806AB3A0 (AnimState)
    a2: 4 (modanim bank in upper byte, index within bank on lower byte)
    a3: 3FF (blend duration/speed, maybe?)

    something to do with blending between two animations?
*/
// void func_800255F8(Model* model, AnimState* animState, s32 modAnimIndex, s16 arg3) {
//     f32 var_fv0;
//     s16 temp_v0;
//     s8 temp_v0_2;
//     s32 temp_t1;
//     Animation* anim;

//     modAnimIndex = model->modAnimBankBases[modAnimIndex >> 8] + (u8)modAnimIndex;
    
//     if (modAnimIndex >= model->animCount) {
//         modAnimIndex = model->animCount - 1;
//     }
//     if (modAnimIndex < 0) {
//         modAnimIndex = 0;
//     }
    
//     if (model->unk_0x71 & 0x40) {
//         if (modAnimIndex != animState->modAnimIdBlend) {
//             temp_v0_2 = animState->unk_0x62;
//             animState->unk4a = 1 - temp_v0_2;
//             animState->unk48 = temp_v0_2;
//             func_80019118(model->modAnim[modAnimIndex], modAnimIndex, animState->anims2[(u16)temp_v0_2], model);

//             animState->modAnimIdBlend = modAnimIndex;
//         }
//         anim = (Animation*)(&animState->anims2[animState->unk48]->anim);
//     } else {
//         animState->unk48 = modAnimIndex;
//         anim = model->anims[(u16)modAnimIndex];
//     }
    
//     animState->animHeaderC = (AnimationHeader*) (&anim->animHeader);
//     temp_t1 = anim->unk_0x1 & 0xF0;
//     if (temp_t1 != animState->unk_0x60) {
//         animState->unk_0x5a = 0;
//         return;
//     }
    
//     var_fv0 = (u8)animState->animHeaderC->totalKeyframes; 
//     if (temp_t1 == 0) {
//         var_fv0 -= 1.0f;
//     }
//     if (var_fv0 != animState->unk_0x14) {
//         animState->unk_0x5a = 0;
//         return;
//     }
//     animState->unk_0x5a = arg3;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/objanim/func_80025780.s")

extern u8 D_800916B0[];

u8 func_80025CD4(s32 arg0) {
    if (arg0 >= 0x21)
        return 0;
    return D_800916B0[arg0];
}