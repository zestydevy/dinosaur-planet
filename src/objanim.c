#include "common.h"
#include "macros.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/linked_list.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/objtype.h"

static const char str_80099800[] = "Error in loading Model Inst in ObjAnimSetMove ";

/* -------- .data start 800916B0 -------- */
u8 D_800916B0[33] = {
    0x00,
    0x01,
    0x02,
    0x03,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0x00,
    0x00,
    0x00,
    0x07,
    0x05,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0x05,
    0x00,
    0x04,
    0x06,
    0x00,
    0x07,
    0x00,
    0x00,
    0x00,
    0x00,
    0x08
};
/* -------- .data start 800916E0 -------- */

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
            func_80019118(model->modAnim[modAnimIndex], modAnimIndex, (u8*)animState->anims[(u16)animState->unk62[0]]->boneRemaps, model);
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
s32 func_80024108(Object* arg0, f32 arg1, f32 arg2, UnkFunc_80024108Struct* arg3) {
    AnimState* temp_t0;
    Animation* var_t5_2;
    ModelInstance* temp_t2;
    s16* new_var;
    f32 temp_ft5;
    f32 var_fa0;
    s32 temp_a1;
    f32 sp78;
    f32 sp74;
    f32 sp70;
    f32 var_fs0;
    f32 var_fs1;
    f32 var_fv0;
    f32 var_fv1;
    f32 sp5C;
    s32 temp_ft0;
    s32 temp_ft1;
    s32 temp_ft3;
    s32 temp_s0;
    s32 temp_t7;
    s32 var_t1;
    s32 var_t3;
    s32 var_t5;
    s32 var_v1;
    s16* var_v0;
    s16* var_s0;

    var_t3 = 0;
    temp_t2 = arg0->modelInsts[arg0->modelInstIdx];
    if (temp_t2->model->animCount <= 0) {
        return var_t3;
    }
    temp_t0 = temp_t2->animState0;
    if (temp_t0 == NULL) {
        return var_t3;
    }
    temp_t0->unkC[0] = temp_t0->totalAnimationFrames[0] * arg1;
    if (temp_t0->unk58[0] != 0) {
        if (temp_t0->unk62[1] & 8) {
            temp_t0->unkC[1] = temp_t0->unkC[0];
        }
        var_fv0 = temp_t0->curAnimationFrame[1];
        sp5C = temp_t0->totalAnimationFrames[1];
        temp_t0->curAnimationFrame[1] += temp_t0->unkC[1] * arg2;
        if (temp_t0->unk60[1] != 0) {
            if (temp_t0->curAnimationFrame[1] < 0.0f) {
                temp_t0->curAnimationFrame[1] += sp5C;
            }
            if (sp5C <= temp_t0->curAnimationFrame[1]) {
                temp_t0->curAnimationFrame[1] -= sp5C;
            }
        } else {
            if (temp_t0->curAnimationFrame[1] < 0.0f) {
                temp_t0->curAnimationFrame[1] = 0.0f;
            }
            if (sp5C < temp_t0->curAnimationFrame[1]) {
                temp_t0->curAnimationFrame[1] = sp5C;
            }
        }
        if (!(temp_t0->unk62[1] & 2)) {
            temp_t0->unk58[0] -= temp_t0->unk5C[1] * arg2;
        }
        if (temp_t0->unk58[0] < 0) {
            temp_t0->unk58[0] = 0;
        }
        if (temp_t0->unk58[0] == 0) {
            temp_t0->unk5C[0] = 0;
        }
    }
    sp78 = arg0->animProgress;
    arg1 *= arg2;
    arg0->animProgress += arg1;
    if (arg0->animProgress >= 1.0f) {
        if (temp_t0->unk60[0] != 0) {
            while (arg0->animProgress >= 1.0f) {
                arg0->animProgress -= 1.0f;
            }
        } else {
            arg0->animProgress = 1.0f;
        }
        var_t3 = 1;
    } else if (arg0->animProgress < 0.0f) {
        var_t3 = 1;
        if (temp_t0->unk60[0] != 0) {
            while (arg0->animProgress < 0.0f) {
                arg0->animProgress += 1.0f;
            }
        } else {
            arg0->animProgress = 0.0f;
            // FAKE
            temp_ft3 = (s32) sp70;
        }
    }
    if (arg3 == NULL) {
        return var_t3;
    }

    arg3->unk12 = 0;
    arg3->unk0[2] = 0.0f;
    arg3->unk0[1] = 0.0f;
    arg3->unk0[0] = 0.0f;
    if (arg0->curEvent != NULL) {
        arg3->unk1B = 0;
        temp_a1 = arg0->curEvent->size >> 1;
        if (temp_a1 != 0) {
            temp_ft1 = sp78 * 512.0f;
            temp_ft0 = arg0->animProgress * 512.0f;
            var_t1 = 0;
            if (temp_ft0 < temp_ft1) {
                var_t1 |= 1;
            }
            if (arg1 < 0.0f) {
                var_t1 |= 2;
            }
            for (var_t5 = 0; var_t5 < temp_a1 && arg3->unk1B < 8; var_t5++) {
                var_v1 = *((s16*)arg0->curEvent->data + var_t5);
                temp_s0 = var_v1 & 0x1FF;
                var_v1 = (var_v1 >> 9) & 0x7F;
                if (var_v1 != 0x7F) {
                    if ((var_t1 == 0) && (temp_s0 >= temp_ft1) && (temp_s0 < temp_ft0)) {
                        arg3->unk13[arg3->unk1B] = var_v1;
                        arg3->unk1B += 1;
                    }
                    if ((var_t1 == 1) && ((temp_s0 >= temp_ft1) || (temp_s0 < temp_ft0))) {
                        arg3->unk13[arg3->unk1B] = var_v1;
                        arg3->unk1B += 1;
                    }
                    if ((var_t1 == 3) && (temp_ft0 < temp_s0) && (temp_ft1 >= temp_s0)) {
                        arg3->unk13[arg3->unk1B] = var_v1;
                        arg3->unk1B += 1;
                    }
                    if ((var_t1 == 2) && ((temp_ft0 < temp_s0) || (temp_ft1 >= temp_s0))) {
                        arg3->unk13[arg3->unk1B] = var_v1;
                        arg3->unk1B += 1;
                    }
                }
            }
        }
    }
    if (temp_t2->model->unk71 & 0x40) {
        var_t5_2 = &temp_t0->anims[temp_t0->animIndexes[0]]->anim;
    } else {
        var_t5_2 = temp_t2->model->anims[temp_t0->animIndexes[0]];
    }
    if (var_t5_2->offset_rootMotion != 0) {
        arg3->unk12 = 1;
        var_s0 = NULL;
        var_v0 = (u8*)var_t5_2 + var_t5_2->offset_rootMotion;
        sp5C = ((f32*)var_v0)[0] * arg0->srt.scale;
        temp_a1 = var_v0[2] - 1;
        var_v0 += 3;
        var_v1 = 0;
        var_fa0 = temp_a1 * sp78;
        sp74 = var_fa0 - (s32) var_fa0;
        sp70 = arg0->animProgress * temp_a1;
        temp_ft3 = (s32) sp70;
        sp70 -= (f32) temp_ft3;
        if (temp_t0->unk58[1] != 0) {
            var_fs0 = temp_t0->unk58[1] / 1023.0f;
            var_fs1 = 1.0f - var_fs0;
            if (temp_t2->model->unk71 & 0x40) {
                var_t5_2 = &temp_t0->anims2[temp_t0->unk48[0]]->anim;
            } else {
                var_t5_2 = temp_t2->model->anims[temp_t0->unk48[0]];
            }
            var_s0 = (u8*)var_t5_2 + var_t5_2->offset_rootMotion + 6;
        } else {
            var_fs0 = 0.0f;
            var_fs1 = 1.0f;
        }
        for (; var_v1 != 6; var_v1++) {
            if (var_v0[0] != 0) {
                var_v0++;
                var_fa0 = temp_a1 * sp78;
                if (var_s0 != NULL) {
                    var_s0 += 1;
                }
                var_fv0 = var_v0[(s32)var_fa0] * var_fs1;
                if (var_s0 != NULL) {
                    var_fv0 += var_s0[(s32)var_fa0] * var_fs0;
                }
                var_fv1 = var_v0[(s32)var_fa0 + 1] * var_fs1;
                if (var_s0 != NULL) {
                    var_fv1 += var_s0[(s32)var_fa0 + 1] * var_fs0;
                }

                // FAKE?
                new_var = var_v0;
                temp_ft5 = ((var_fv1 - var_fv0) * sp74) + var_fv0;
                temp_ft0 = temp_ft3;
                var_fv0 = new_var[temp_ft0] * var_fs1;
                if (var_s0 != NULL) {
                    var_fv0 += var_s0[temp_ft0] * var_fs0;
                }
                var_fv1 = new_var[temp_ft0 + 1] * var_fs1;
                if (var_s0 != NULL) {
                    var_fv1 += var_s0[temp_ft0 + 1] * var_fs0;
                }
                var_fa0 = ((var_fv1 - var_fv0) * sp70) + var_fv0;
                if (arg1 > 0.0f) {
                    if (arg0->animProgress < sp78) {
                        var_fa0 += (new_var[temp_a1] * var_fs1);
                        if (var_s0 != NULL) {
                            var_fa0 += var_s0[temp_a1] * var_fs0;
                        }
                    }
                } else {
                    if (sp78 < arg0->animProgress) {
                        var_fa0 -= (new_var[temp_a1] * var_fs1);
                        if (var_s0 != NULL) {
                            var_fa0 += var_s0[temp_a1] * var_fs0;
                        }
                    }
                }
                if (var_v1 < 3) {
                    arg3->unk0[var_v1] = (var_fa0 - temp_ft5) * sp5C;
                } else {
                    arg3->unkC[var_v1 - 3] = (var_fa0 - temp_ft5);
                }
                var_v0 += temp_a1 + 1;
                if (var_s0 != NULL) {
                    var_s0 += temp_a1 + 1;
                }
            } else {
                var_v0 += 1;
                if (var_s0 != NULL) {
                    var_s0 += 1;
                }
                if (var_v1 < 3) {
                    arg3->unk0[var_v1] = 0.0f;
                } else {
                    arg3->unkC[var_v1 - 3] = 0;
                }
            }
        }
    } else {
        arg3->unk12 = 0;
    }
    return var_t3;
}

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
            func_80019118(model->modAnim[modanimIndex], modanimIndex, (u8*)animState->anims[(u16)animState->animIndexes[0]]->boneRemaps, model);
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

s32 func_80025140(Object* arg0, f32 arg1, f32 arg2, UnkFunc_80024108Struct* arg3) {
    AnimState* temp_a1;
    ModelInstance* temp_v0;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 temp_t1;
    s32 temp_ft1;
    s32 temp_ft3;
    s32 temp_t4;
    s32 temp_t7;
    s32 var_t0;
    s32 var_t3;
    s32 var_v1;

    var_v1 = 0;
    temp_v0 = arg0->modelInsts[arg0->modelInstIdx];
    if (temp_v0->model->animCount <= 0) {
        return var_v1;
    }
    temp_a1 = temp_v0->animState1;
    temp_a1->unkC[0] = temp_a1->totalAnimationFrames[0] * arg1;
    if (temp_a1->unk58[0] != 0) {
        if (temp_a1->unk62[1] & 8) {
            temp_a1->unkC[1] = temp_a1->unkC[0];
        }
        temp_fv0 = temp_a1->curAnimationFrame[1];
        temp_fv1 = temp_a1->totalAnimationFrames[1];
        temp_a1->curAnimationFrame[1] += temp_a1->unkC[1] * arg2;
        if (temp_a1->unk60[1] != 0) {
            if (temp_a1->curAnimationFrame[1] < 0.0f) {
                temp_a1->curAnimationFrame[1] += temp_fv1;
            }
            if (temp_fv1 <= temp_a1->curAnimationFrame[1]) {
                temp_a1->curAnimationFrame[1] -= temp_fv1;
            }
        } else {
            if (temp_a1->curAnimationFrame[1] < 0.0f) {
                temp_a1->curAnimationFrame[1] = 0.0f;
            }
            if (temp_fv1 < temp_a1->curAnimationFrame[1]) {
                temp_a1->curAnimationFrame[1] = temp_fv1;
            }
        }
        if (!(temp_a1->unk62[1] & 2)) {
            temp_a1->unk58[0] -= temp_a1->unk5C[1] * arg2;
        }
        if (temp_a1->unk58[0] < 0) {
            temp_a1->unk58[0] = 0;
        }
        if (temp_a1->unk58[0] == 0) {
            temp_a1->unk5C[0] = 0;
        }
    }
    arg1 *= arg2;
    temp_fv0 = arg0->animProgressLayered;
    arg0->animProgressLayered += arg1;
    if (arg0->animProgressLayered >= 1.0f) {
        if (temp_a1->unk60[0] != 0) {
            while (arg0->animProgressLayered >= 1.0f) {
                arg0->animProgressLayered -= 1.0f;
            }
        } else {
            arg0->animProgressLayered = 1.0f;
        }
        var_v1 = 1;
    } else if (arg0->animProgressLayered < 0.0f) {
        var_v1 = 1;
        if (temp_a1->unk60[0] != 0) {
            while (arg0->animProgressLayered < 0.0f) {
                arg0->animProgressLayered += 1.0f;
            }
        } else {
            arg0->animProgressLayered = 0.0f;
        }
    }
    if (arg3 == NULL) {
        return var_v1;
    }
    arg3->unk12 = 0;
    if (arg0->curEvent != NULL) {
        arg3->unk1B = 0;
        temp_t7 = arg0->curEvent->size >> 1;
        if (temp_t7 != 0) {
            temp_ft3 = temp_fv0 * 512.0f;
            temp_ft1 = arg0->animProgressLayered * 512.0f;
            var_t0 = 0;
            if (temp_ft1 < temp_ft3) {
                var_t0 |= 1;
            }
            if (arg1 < 0.0f) {
                var_t0 |= 2;
            }
            for (var_t3 = 0; var_t3 < temp_t7 && arg3->unk1B < 8; var_t3++) {
                temp_t1 = *(((s16*)arg0->curEvent->data) + var_t3);
                temp_t4 = temp_t1 & 0x1FF;
                temp_t1 = (temp_t1 >> 9) & 0x7F;
                if (temp_t1 != 0x7F) {
                    if ((var_t0 == 0) && (temp_t4 >= temp_ft3) && (temp_t4 < temp_ft1)) {
                        arg3->unk13[arg3->unk1B] = temp_t1;
                        arg3->unk1B++;
                    }
                    if ((var_t0 == 1) && ((temp_t4 >= temp_ft3) || (temp_t4 < temp_ft1))) {
                        arg3->unk13[arg3->unk1B] = temp_t1;
                        arg3->unk1B++;
                    }
                    if ((var_t0 == 3) && (temp_ft1 < temp_t4) && (temp_ft3 >= temp_t4)) {
                        arg3->unk13[arg3->unk1B] = temp_t1;
                        arg3->unk1B++;
                    }
                    if ((var_t0 == 2) && ((temp_ft1 < temp_t4) || (temp_ft3 >= temp_t4))) {
                        arg3->unk13[arg3->unk1B] = temp_t1;
                        arg3->unk1B++;
                    }
                }
            }
        }
    }
    return var_v1;
}

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

u8 func_80025CD4(s32 arg0) {
    if (arg0 >= (s32)ARRAYCOUNT(D_800916B0))
        return 0;
    return D_800916B0[arg0];
}