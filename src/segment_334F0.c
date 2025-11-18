#include "segment_334F0.h"
#include "dlls/objects/210_player.h"
#include "sys/curves.h"
#include "dll.h"
#include "sys/main.h"
#include "sys/rand.h"

#define TWENTY_DEGREES 0xE38
#define HEAD_TURN_LIMIT 0x1FFF //~45 degrees (giving a 90 degree turn range)

typedef enum {
    BLINK_Wait = 0,
    BLINK_Animate = 1,
    BLINK_Eyelid_Close_Finished = 0x80
} BlinkStates;

typedef enum {
    HEAD_TURN_Goal_Reached = 0,
    HEAD_TURN_Wait = 1,
    HEAD_TURN_Animate = 2
} HeadTurnStates;

typedef enum {
    HEAD_ANIMATION_TAG_Pupil_L = 0,
    HEAD_ANIMATION_TAG_Pupil_R = 1,
    HEAD_ANIMATION_TAG_Eyelid_L = 4,
    HEAD_ANIMATION_TAG_Eyelid_R = 5
} HeadAnimationTags;

static const char str_80099c90[] = " WARNING EXPR: This Object has no Head ";
static const char str_80099cb8[] = " WARNING EXPR: Obj Has No Joint %i ";
static const char str_80099cdc[] = " WARNING: Expr Contrl Flag does not exist \n";
extern u8 D_800B2E00;

s32 D_80091720[] = { 0x0, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10, 0x11, 0x12, 0x13 };
s32 D_80091748[] = { 0, 0 };

void func_800328F0(Object* obj, HeadAnimation* arg1, f32 arg2) {
    Object* target; // could also be SRT*

    //Handle player head aim
    if (obj->group == GROUP_UNK1) {
        arg1 = ((DLL_210_Player*)obj->dll)->vtbl->func54(obj);
        arg2 = ((DLL_210_Player*)obj->dll)->vtbl->func56(obj);
        if (((DLL_210_Player*)obj->dll)->vtbl->func66(obj, 2) != NULL) {
            arg2 = 1.0f;
        }

        target = ((DLL_210_Player*)obj->dll)->vtbl->func52(obj);
        if (target != NULL) {
            arg1->headAimX = target->srt.transl.x;
            arg1->headAimY = target->srt.transl.y;
            arg1->headAimZ = target->srt.transl.z;
            arg1->aimIsActive = 1;
        } else {
            arg1->aimIsActive = 0;
        }
        func_80033B68(obj, arg1, arg2);
        return;
    }

    //Handle non-player head aim
    func_80033B68(obj, arg1, 0.0f);
}

/** Manages character models' randomised blinks, by changing the frame of the eyelids' animated textures.
  * @bug: animation is framerate dependent
  */
void func_80032A08(Object* obj, HeadAnimation* arg1) {
    s32* eyelidR;
    s32* eyelidL;
    s32 eyelidValue;

    eyelidR = func_800348A0(obj, HEAD_ANIMATION_TAG_Eyelid_R, 0);
    eyelidL = func_800348A0(obj, HEAD_ANIMATION_TAG_Eyelid_L, 0);

    if (!eyelidR || !eyelidL) {
        return;
    }

    eyelidValue = *eyelidL;

    switch (arg1->blinkState & 0xF) {
    case BLINK_Wait:
        if (arg1->blinkDelayTimer > 0) {
            //Wait for timer to run out
            arg1->blinkDelayTimer -= gUpdateRate;
        } else if (rand_next(0, 1000) > 985) {
            //1.5% chance of going into a blink
            arg1->blinkState = BLINK_Animate;
            arg1->blinkDelayTimer = 0;
        }
        break;
    case BLINK_Animate:
        if (arg1->blinkState & BLINK_Eyelid_Close_Finished) {
            //Animate eyelid opening
            eyelidValue -= 0x100;
            if (eyelidValue < 0) {
                eyelidValue = 0;
                arg1->blinkState = BLINK_Wait;
                arg1->blinkDelayTimer = 0;
            }
        } else {
            //Animate eyelid closing
            eyelidValue += 0x100;
            if (eyelidValue > 0x200) {
                eyelidValue -= 0x200;
                if (eyelidValue < 0) {
                    eyelidValue = 0;
                    arg1->blinkState = BLINK_Wait;
                } else {
                    arg1->blinkState = (s8)(BLINK_Eyelid_Close_Finished + BLINK_Animate);
                }
                arg1->blinkDelayTimer = 0;
            }
        }
        *eyelidR = eyelidValue;
        *eyelidL = eyelidValue;
        break;
    }

    func_80034678(obj, arg1, 0);
}

void func_80032B44(Object *obj, s32 arg1) {
    s32 v0;
    s32 pad;
    s32 sp1C;
    s32 sp18;

    v0 = gDLL_6_AMSFX->vtbl->func_DCC(arg1, 2, &sp18, &sp1C);
    func_800339E0(obj, sp1C & 0xFF, v0, (f32) sp18);
    v0 = gDLL_6_AMSFX->vtbl->func_DCC(arg1, 1, &sp18, &sp1C);
    func_80033AA0(obj, sp1C & 0xFF, v0, (f32) sp18);
}

void func_80032C0C(Object* obj, Object* otherObj, s16* arg2, s32 arg3) {
    s32 pad;
    s32 pad2;
    s16* sp1C;

    sp1C = func_80034804(obj, 0);
    if (sp1C == NULL) {
        return;
    }

    arg2[10] = ((arctan2_f(obj->srt.transl.x - otherObj->srt.transl.x, obj->srt.transl.z - otherObj->srt.transl.z) & 0xFFFFu) & 0xFFFFu) - obj->srt.yaw;
    arg3 = (s16)(arg3 * 182.04f);
    if (arg3 < arg2[10]) {
        arg2[10] = arg3;
    }
    if (arg2[10] < -arg3) {
        arg2[10] = -arg3;
    }
    sp1C[1] = arg2[10];
}

void func_80032CF8(Object* obj, Object* otherObj, HeadAnimation* arg2, s32 arg3) {
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    s16* sp34;
    s32 temp;
    s16 sp2C[2];
    s32 i;

    sp34 = func_80034804(obj, 0);
    if (sp34 == NULL) {
        return;
    }

    if (otherObj == NULL) {
        sp34[1] = (s16) ((s16) sp34[1] >> 1);
        sp34[0] = (u16) ((s16) sp34[0] >> 1);
        return;
    }
    sp44 = obj->srt.transl.x - otherObj->srt.transl.x;
    sp40 = obj->srt.transl.z - otherObj->srt.transl.z;
    sp3C = obj->srt.transl.y - otherObj->srt.transl.y;
    sp38 = sqrtf(SQ(sp44) + SQ(sp40));
    temp = arctan2_f(sp44, sp40);
    sp2C[0] = ((s16)temp - (obj->srt.yaw & 0xFFFF));
    CIRCLE_WRAP(sp2C[0])
    sp2C[1] = (arctan2_f(sp38, sp3C) - 0x3FFF) & 0xFFFF & 0xFFFF;
    arg3 = (s16) (arg3 * 182.04f);
    for (i = 0; i < 2; i++) {
        arg2[i].headGoalAngle = sp2C[i];
        if (arg3 < arg2[i].headGoalAngle) {
            arg2[i].headGoalAngle = arg3;
        }
        if (arg2[i].headGoalAngle < -arg3) {
            arg2[i].headGoalAngle = -arg3;
        }
    }
    sp34[1] = arg2->headGoalAngle;
    sp34[0] = arg2[1].headGoalAngle;
}

s16 func_80032EBC(Object* obj, Object* otherObj, HeadAnimation* arg2, s16* arg3) {
    s16 var_a2;
    s16 var_s0;
    s16* temp_v0;
    s16 temp_ft1;
    s32 i;

    var_s0 = arctan2_f(obj->srt.transl.x - otherObj->srt.transl.x, obj->srt.transl.z - otherObj->srt.transl.z) - (obj->srt.yaw & 0xFFFF & 0xFFFF);
    for (i = 0; i < 10 && var_s0 != 0; i++) {
        temp_v0 = func_80034804(obj, D_80091720[i]);
        temp_ft1 = arg3[0] * 182.04f;
        arg3++;
        var_a2 = var_s0;
        if (temp_ft1 < var_s0) {
            var_a2 = temp_ft1;
            var_s0 -= temp_ft1;
            if (var_s0 < 0) {
                var_s0 = 0;
            }
        } else if (var_s0 < -temp_ft1) {
            var_a2 = -temp_ft1;
            var_s0 += temp_ft1;
            if (var_s0 > 0) {
                var_s0 = 0;
            }
        } else {
            var_s0 = 0;
        }
        if (arg2 != NULL) {
            arg2->headGoalAngle = var_a2;
            func_80034250(arg2, temp_v0);
            arg2++;
        } else {
            temp_v0[1] = var_a2;
        }
    }

    return var_s0;
}

s16 func_80033044(Object* obj, Object* otherObj, Vec3f* arg2, HeadAnimation* arg3, s16* arg4) {
    f32 temp_fs0;
    f32 sp40;
    s16 var_a1;
    s16 var_s0;
    s16 var_s3;
    s16* temp_v0;
    s16 temp_ft2;
    s32 i;

    temp_fs0 = arg2->x - otherObj->srt.transl.x;
    sp40 = arg2->z - otherObj->srt.transl.z;
    var_s3 = obj->srt.yaw;
    for (i = 0; i < 10; i++) {
        if (func_80034804(obj, D_80091720[i]) != NULL) {
            var_s3 += arg3[i].headGoalAngle;
        }
    }
    var_s0 = (arctan2_f(temp_fs0, sp40) & 0xFFFF & 0xFFFF) - var_s3;
    for (i = 0; i < 10; i++) {
        temp_v0 = func_80034804(obj, D_80091720[i]);
        if (temp_v0 == NULL) {
            return var_s0;
        }

        temp_ft2 = arg4[0] * 182.04f;
        arg4 += 1;
        var_s0 += arg3[i].headGoalAngle;
        var_a1 = var_s0;
        if (temp_ft2 < var_s0) {
            var_a1 = temp_ft2;
            var_s0 -= temp_ft2;
        } else if (var_s0 < -temp_ft2) {
            var_a1 = -temp_ft2;
            var_s0 += temp_ft2;
        } else {
            var_s0 = 0;
        }
        temp_v0[1] = var_a1;
        arg3[i].headGoalAngle = var_a1;
    }

    return var_s0;
}

void func_80033224(Object* arg0, s32* arg1, s32 arg2, HeadAnimation* arg3) {
    s16* temp_v0;
    s32 i;

    for (i = 0; i < arg2; i++) {
        temp_v0 = func_80034804(arg0, arg1[i]);
        arg3->headStartAngle = temp_v0[1];
        arg3[1].headStartAngle = temp_v0[0];
        arg3 += 2;
    }
}

void func_800332A4(Object* arg0, s32* arg1, s32 arg2) {
    s16* temp_v0;
    s32 i;

    for (i = 0; i < arg2; i++) {
        temp_v0 = func_80034804(arg0, arg1[i]);
        if (temp_v0 == NULL) {
            continue;
        }

        temp_v0[1] = (temp_v0[1] * 3) >> 2;
        temp_v0[0] = (temp_v0[0] * 3) >> 2;
        temp_v0[2] = (temp_v0[2] * 3) >> 2;
    }
}

void func_80033350(HeadAnimation* arg0, s32 arg1, s16 arg2, s16 arg3) {
    s32 i;

    for (i = 0; i < arg1; i++) {
        arg0->headGoalAngle = arg2;
        arg0[1].headGoalAngle = arg3;
        arg0 += 2;
    }
}

s32 func_800333C8(Object* arg0, s32* arg1, s32 arg2, HeadAnimation* arg3) {
    s16 *temp_v0;
    s32 temp_v0_2;
    s32 var_s1;
    s32 i;

    var_s1 = 0;
    for (i = 0; i < arg2; i++) {
        temp_v0 = func_80034804(arg0, arg1[i]);
        var_s1 += func_80034250(arg3, temp_v0);
        temp_v0_2 = func_80034518(arg3 + 1, temp_v0, 10.0f, 500.0f);
        arg3 += 2;
        var_s1 += temp_v0_2;
    }

    return var_s1 == (arg2 * 2);
}

#ifndef NON_EQUIVALENT
s32 func_800334A4(Object* obj, Object* otherObj, Vec3f* arg2, HeadAnimation* arg3, s16* arg4, f32 arg5, s16 arg6, s16 arg7);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_800334A4.s")
#else
// https://decomp.me/scratch/hIDz6
s32 func_800334A4(Object* obj, Object* otherObj, Vec3f* arg2, HeadAnimation* arg3, s16* arg4, f32 arg5, s16 arg6, s16 arg7) {
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    s16 sp84[2];
    s16 sp80[2];
    u8 sp6B;
    f32 temp_fs0;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_lo;
    s32 temp_t5;
    s16 temp_v1_2;
    s16 var_a0;
    s16 var_t7;
    s32 var_v1;
    s16* temp_v0;
    s16* var_a1;
    s16* var_a3;
    s32 temp_ft0;
    s32 i;
    s32 j;
    s32* var_s6;
    u8 var_v1_4;

    sp6B = 0;
    temp_fs0 = arg2->x - otherObj->srt.transl.x;
    sp8C = arg2->z - otherObj->srt.transl.z;
    sp90 = (arg2->y + arg5) - otherObj->srt.transl.y;
    sp88 = sqrtf(SQ(temp_fs0) + SQ(sp8C));
    sp84[0] = ((s16)((u16)arctan2_f(temp_fs0, sp8C)) - (obj->srt.yaw & 0xFFFF));
    CIRCLE_WRAP(sp84[0])
    sp84[1] = arg7 - -(u16)arctan2_f(sp88, sp90);
    CIRCLE_WRAP(sp84[1])
    if ((D_800B2E00 >> 0x1F) != 0) {
        sp84[0] -= 0x8000;
        sp84[1] = -sp84[1];
    }

    for (i = 0; i < 10; i++) {
        temp_v0 = func_80034804(obj, D_80091720[i]);
        if (temp_v0 == NULL) {
            return sp6B;
        }
        for (j = 0; j < 2; j++) {
            if (j % 2) {
                var_a0 = (arg4[i + 15] * 182.04f);
            } else {
                var_a0 = (arg4[i] * 182.04f);
            }
            temp_v1_2 = sp84[j];
            sp80[j] = temp_v1_2;
            if (var_a0 < temp_v1_2) {
                sp80[j] = var_a0;
                sp84[j] = temp_v1_2 - var_a0;
            } else if (temp_v1_2 < -var_a0) {
                sp80[j] = -var_a0;
                sp84[j] = temp_v1_2 + var_a0;
            } else {
                sp84[j] = 0;
            }
        }
        if (arg3 != NULL) {
            arg3->headGoalAngle = sp80[0];
            func_80034250(arg3, temp_v0);
            arg3[1].headGoalAngle = sp80[1];
            func_80034518(arg3 + 1, temp_v0, 10.0f, 500.0f);
            arg3 += 2;
        } else {
            temp_a1 = (s16) ((s16) ((sp80[0] + temp_v0[1]) >> 1) - temp_v0[1]);
            temp_lo = ((s16) (-arg4[i] * 182.04f) / 10) * gUpdateRate;
            if (temp_a1 < temp_lo) {
                var_t7 = temp_lo;
            } else {
                temp_lo = ((s16) (arg4[i] * 182.04f) / 10) * gUpdateRate;
                if (temp_lo < temp_a1) {
                    temp_a1 = temp_lo;
                }
                var_t7 = temp_a1;
            }
            temp_a0 = (s16) ((s16) ((sp80[1] + temp_v0[0]) >> 1) - temp_v0[0]);
            temp_lo = (-(s16) (arg4[i + 15] * 182.04f) / 10) * gUpdateRate;
            if (temp_a0 < temp_lo) {
                var_a0 = temp_lo;
            } else {
                temp_lo = ((s16) (arg4[i + 15] * 182.04f) / 20) * gUpdateRate;
                if (temp_lo < temp_a0) {
                    temp_a0 = temp_lo;
                }
                var_a0 = temp_a0;
            }
            temp_v0[0] += var_a0;
            temp_v0[1] += var_t7;
        }
        if (i == 0) {
            sp6B = (sp80[0] - 4) < temp_v0[1] && temp_v0[1] < (sp80[0] + 4);
        }
    }

    (u8)D_800B2E00 &= 0xFF7F;
    return sp84[0];
}
#endif

void func_800339E0(Object* obj, s32 arg1, s32 arg2, f32 arg3) {
    ModelInstance* sp2C;
    ModelInstanceBlendshape* sp28;

    sp2C = obj->modelInsts[obj->modelInstIdx];
    sp28 = sp2C->blendshapes;
    if (arg1 == 0) {
        arg1 = -1;
    } else {
        arg1 += 0xE;
    }
    if (arg2 != 0) {
        sp28 = sp28;
        func_8001AF04(sp2C, (s32) sp28->id, arg1, 0.0f, 0, 1);
        sp28->unk8 = arg3;
        return;
    }
    if (sp28->unk8 != 0.0f) {
        sp28->strength = (sp28->unk8 - arg3) / sp28->unk8;
        return;
    }
    sp28->strength = 0.0f;
}

void func_80033AA0(Object* obj, s32 arg1, s32 arg2, f32 arg3) {
    ModelInstance* sp24;
    ModelInstanceBlendshape* sp20;
    s32 a2;

    sp24 = obj->modelInsts[obj->modelInstIdx];
    sp20 = sp24->blendshapes + 2;
    if ((arg1 >= 0) && (arg1 < 0x10)) {
        if (arg2 != 0) {
            // @fake?
            if (arg1) {}
            func_8001AF04(sp24, sp20->id, arg1 - 1, 0.0f, 2, 1);
            sp20->unk8 = arg3;
            return;
        }
        if (sp20->unk8 != 0.0f) {
            sp20->strength = (sp20->unk8 - arg3) / sp20->unk8;
            return;
        }
        sp20->strength = 0.0f;
    }
}

/** Handles aiming characters' heads towards a point */
void func_80033B68(Object* obj, HeadAnimation* arg1, f32 arg2) {
    s16* neckJoint;
    s32 var_v0;

    neckJoint = func_80034804(obj, 0);
    if (neckJoint == NULL) {
        return;
    }

    if (neckJoint[0] != 0) {
        neckJoint[0] = (neckJoint[0] * 3) / 4;
    }

    if (arg2 < 0.0f) {
        arg2 = -arg2;
    }
    if (arg2 <= 0.1f) {
        func_80033C54(obj, arg1, arg2, neckJoint);
    } else {
        func_80033FD8(obj, arg1, arg2, neckJoint);
    }
    
    var_v0 = arg2 > 0.1f ? 1 : 0;
    arg1->headTurnState = (var_v0 << 8) | (arg1->headTurnState & 0xFF);
}

/** Manages character models' randomised head turn animation (by rotating jointIDs specially marked in OBJECTS.bin) */
void func_80033C54(Object* obj, HeadAnimation* arg1, f32 arg2, s16* neckJoint) {
    s32 temp_v0;
    s32 pad2;
    s32 pad;
    s32 sp28;
    s32 currentAngle;

    temp_v0 = (u8)(arg1->headTurnState >> 8);
    sp28 = arg2 > 0.1f ? 1 : 0;
    if (temp_v0 != sp28) {
        arg1->headTurnState = sp28 << 8;
    }

    switch (arg1->headTurnState & 0xFF) {
    case HEAD_TURN_Goal_Reached:
        if (arg1->aimIsActive != 0) {
            arg1->headTurnState = (sp28 << 8) | 3;
            arg1->headStartAngle = neckJoint[1];
            arg1->headAimUnk = 1.0f;
        } else {
            arg1->headTurnState = (sp28 << 8) | 1;
            arg1->headTurnDelay = rand_next(100, 400);
            arg1->headGoalAngle = neckJoint[1];
        }
        break;
    case HEAD_TURN_Wait:
        arg1->headTurnDelay -= gUpdateRate;
        //Set up next head turn
        if (arg1->headTurnDelay < 0) {
            currentAngle = arg1->headGoalAngle;
            arg1->headGoalAngle = rand_next(0, HEAD_TURN_LIMIT);
            if (currentAngle > 0) {
                if ((currentAngle - arg1->headGoalAngle) < TWENTY_DEGREES) {
                    arg1->headGoalAngle += TWENTY_DEGREES;
                }
                //Left turn limit
                if (arg1->headGoalAngle > HEAD_TURN_LIMIT) {
                    arg1->headGoalAngle = HEAD_TURN_LIMIT;
                }
                arg1->headGoalAngle = -arg1->headGoalAngle;
            } else {
                if ((arg1->headGoalAngle - currentAngle) < TWENTY_DEGREES) {
                    arg1->headGoalAngle += TWENTY_DEGREES;
                }
                if (arg1->headGoalAngle > HEAD_TURN_LIMIT) {
                    arg1->headGoalAngle = HEAD_TURN_LIMIT;
                }
            }
            arg1->headTurnState = (sp28 << 8) | 2;
            arg1->headTurnDelay = 0;
            arg1->headStartAngle = neckJoint[1];
        }
        break;
    case HEAD_TURN_Animate:
        if (arg1->aimIsActive || func_80034250(arg1, neckJoint)) {
            arg1->headTurnState = sp28 << 8;
        }
        break;
    case 3:
        if (arg1->aimIsActive == 0) {
            arg1->headTurnState = sp28 << 8;
        } else {
            arg1->headGoalAngle = arctan2_f(obj->srt.transl.x - arg1->headAimX, obj->srt.transl.z - arg1->headAimZ) - (obj->srt.yaw & 0xFFFF);
            CIRCLE_WRAP(arg1->headGoalAngle)
            if ((arg1->headGoalAngle > HEAD_TURN_LIMIT) || (arg1->headGoalAngle < -HEAD_TURN_LIMIT)) {
                arg1->headTurnState = sp28 << 8;
            } else {
                if (arg1->headAimUnk > 0.0f) {
                    neckJoint[1] = arg1->headGoalAngle + ((arg1->headStartAngle - arg1->headGoalAngle) * arg1->headAimUnk);
                    arg1->headAimUnk -= 0.01f * gUpdateRateF;
                    if (arg1->headAimUnk < 0.0f) {
                        arg1->headAimUnk = 0.0f;
                    }
                } else {
                    neckJoint[1] = arg1->headGoalAngle;
                }
            }
        }
        break;
    }

    //Clamp rotation
    if (neckJoint[1] < -HEAD_TURN_LIMIT) {
        neckJoint[1] = -HEAD_TURN_LIMIT;
    } else if (neckJoint[1] > HEAD_TURN_LIMIT) {
        neckJoint[1] = HEAD_TURN_LIMIT;
    }
}

void func_80033FD8(Object* obj, HeadAnimation* arg1, f32 arg2, s16* arg3) {
    s32 v0;
    s32 sp20;

    v0 = (u8)(arg1->headTurnState >> 8);
    sp20 = arg2 > 0.1f ? 1 : 0;
    if (v0 != sp20) {
        arg1->headTurnState = (sp20 << 8) | 4;
        arg1->headStartAngle = arg3[1];
        arg1->headGoalAngle = 0;
        arg1->headTurnDelay = 0;
    }
    switch (arg1->headTurnState & 0xFF) {
    case 0:
        arg1->headTurnState = sp20 << 8;
        arg1->headTurnDelay = rand_next(0x32, 0xC8);
        return;
    case 1:
        arg1->headTurnDelay -= gUpdateRate;
        if (arg1->headTurnDelay < 0 && !(rand_next(0, 100) < 91)) {
            arg1->headTurnState = (sp20 << 8) | 5;
            if (arg1->aimIsActive != 0) {
                if (rand_next(0, 0x64) > 0) {
                    arg1->headGoalAngle = HEAD_TURN_LIMIT;
                    if (rand_next(0, 1) == 0) {
                        arg1->headGoalAngle = -arg1->headGoalAngle;
                        return;
                    }
                }
            } else {
                arg1->headGoalAngle = HEAD_TURN_LIMIT;
                if (rand_next(0, 1) == 0) {
                    arg1->headGoalAngle = -arg1->headGoalAngle;
                    return;
                }
            }
        }
        break;
    case 5:
        if (arg1->headTurnDelay > 0) {
            arg1->headTurnDelay -= gUpdateRate;
            return;
        }
        if (func_80034250(arg1, arg3) != 0) {
            arg1->headTurnState = (sp20 << 8) | 6;
            arg1->headGoalAngle = -arg1->headGoalAngle;
            arg1->headTurnDelay = rand_next(20, 100);
            return;
        }
        break;
    case 6:
        if (arg1->headTurnDelay > 0) {
            arg1->headTurnDelay -= gUpdateRate;
            return;
        }
        if (func_80034250(arg1, arg3) != 0) {
            arg1->headTurnState = (sp20 << 8) | 4;
            arg1->headGoalAngle = 0;
            arg1->headTurnDelay = rand_next(20, 100);
            return;
        }
        break;
    case 4:
        if (arg1->headTurnDelay > 0) {
            arg1->headTurnDelay -= gUpdateRate;
            return;
        }
        if (func_80034250(arg1, arg3) != 0) {
            arg1->headTurnState = sp20 << 8;
            arg3[1] = 0;
        }
        break;
    }
}

/** Smoothly interpolates neck joint from headStartAngle to headGoalAngle
  * Returns 1 when finished, or 0 while interpolating
  */
s32 func_80034250(HeadAnimation* arg0, s16* neckJoint) {
    Vec4f sp38;
    f32 tValue;
    f32 rotateSpeed;

    sp38.x = 10.0f;
    sp38.y = 10.0f;
    sp38.z = 500.0f;
    sp38.w = -500.0f;

    //Calculate head turn animation's tValue (from 0.0f to 1.0f)
    if (arg0->headGoalAngle != arg0->headStartAngle) {
        tValue = ((f32) neckJoint[1] - arg0->headStartAngle) / ((f32) arg0->headGoalAngle - arg0->headStartAngle);
    } else {
        return 1;
    }

    //Clamp tValue
    if (tValue > 1.0f) {
        tValue = 1.0f;
    } else if (tValue < 0.0f) {
        tValue = 0.0f;
    }
    
    //Get eased rotation speed (ease-in-out)
    rotateSpeed = func_80004C5C(&sp38, tValue, NULL);
    if (arg0->headGoalAngle < arg0->headStartAngle) {
        rotateSpeed = -rotateSpeed;
    }

    //Advance rotation
    neckJoint[1] += rotateSpeed * gUpdateRateF;
    if (tValue == 1.0f || !(neckJoint[1] < 0x1FFF) || neckJoint[1] <= -0x1FFF) {
        neckJoint[1] = arg0->headGoalAngle;
        return 1;
    }

    return 0;
}

s32 func_800343B8(HeadAnimation* arg0, s16* arg1, f32 arg2, f32 arg3) {
    Vec4f sp38;
    f32 sp34;
    f32 var_fa0;

    sp38.x = arg2;
    sp38.y = arg2;
    sp38.z = arg3;
    sp38.w = -arg3;
    if (arg0->headGoalAngle != arg0->headStartAngle) {
        sp34 = ((f32) arg1[1] - arg0->headStartAngle) / ((f32) arg0->headGoalAngle - arg0->headStartAngle);
    } else {
        return 1;
    }

    if (sp34 > 1.0f) {
        sp34 = 1.0f;
    } else if (sp34 < 0.0f) {
        sp34 = 0.0f;
    }
    var_fa0 = func_80004C5C(&sp38, sp34, NULL);
    if (arg0->headGoalAngle < arg0->headStartAngle) {
        var_fa0 = -var_fa0;
    }
    arg1[1] += var_fa0 * gUpdateRateF;
    if (sp34 == 1.0f || !(arg1[1] < 0x1FFF) || arg1[1] < -0x1FFE) {
        arg1[1] = (s16) arg0->headGoalAngle;
        return 1;
    }

    return 0;
}

s32 func_80034518(HeadAnimation* arg0, s16* arg1, f32 arg2, f32 arg3) {
    Vec4f sp38;
    f32 sp34;
    f32 var_fa0;

    sp38.x = arg2;
    sp38.y = arg2;
    sp38.z = arg3;
    sp38.w = -arg3;
    if (arg0->headGoalAngle != arg0->headStartAngle) {
        sp34 = ((f32) arg1[0] - arg0->headStartAngle) / ((f32) arg0->headGoalAngle - arg0->headStartAngle);
    } else {
        return 1;
    }

    if (sp34 > 1.0f) {
        sp34 = 1.0f;
    } else if (sp34 < 0.0f) {
        sp34 = 0.0f;
    }
    var_fa0 = func_80004C5C((Vec4f* ) &sp38, sp34, NULL);
    if (arg0->headGoalAngle < arg0->headStartAngle) {
        var_fa0 = -var_fa0;
    }
    arg1[0] += var_fa0 * gUpdateRateF;
    if (sp34 == 1.0f || !(arg1[0] < 0x1FFF) || arg1[0] < -0x1FFE) {
        arg1[0] = arg0->headGoalAngle;
        return 1;
    }
    return 0;
}

/** Manages character models' randomised eye dart animations (by adjusting the pupil textures' UVs) */
void func_80034678(Object* arg0, HeadAnimation* arg1, f32 arg2) {
    s16* pupilL;
    s16* pupilR;
    s32 animationFinished;

    pupilL = func_800348A0(arg0, HEAD_ANIMATION_TAG_Pupil_R, 0);
    pupilR = func_800348A0(arg0, HEAD_ANIMATION_TAG_Pupil_L, 0);

    if ((pupilL == NULL) || (pupilR == NULL)) {
        return;
    }

    //Check if the current eye dart is already finished
    animationFinished = FALSE;
    if (arg1->pupilSpeed == 0) {
        animationFinished = TRUE;
    }
    if ((arg1->pupilSpeed > 0) && (pupilL[4] >= arg1->pupilGoal)) {
        animationFinished = TRUE;
    }
    if ((arg1->pupilSpeed < 0) && (arg1->pupilGoal >= pupilL[4])) {
        animationFinished = TRUE;
    }

    //Set up the next eye dart animation
    if (animationFinished) {
        arg1->pupilGoal = rand_next(-100, 10);
        if (arg1->pupilGoal < pupilL[4]) {
            arg1->pupilSpeed = -5;
        } else {
            arg1->pupilSpeed = 5;
        }
        arg1->pupilDelayTimer = rand_next(30, 100);
    }

    //Wait a random amount of time between darts
    if (arg1->pupilDelayTimer > 0) {
        arg1->pupilDelayTimer -= gUpdateRate;
        return;
    }

    //Animate eye dart
    pupilL[4] += arg1->pupilSpeed * gUpdateRate;
    pupilL[5] = 0;
    pupilR[4] = -100 - pupilL[4];
    pupilR[5] = 0;
}

/** object_find_seq_bone_data_by_bone_id? */
s16* func_80034804(Object* obj, s32 sequenceBoneID) {
    ObjDef* romdef;
    u8* seqBones;
    s32 index;
    s32 listPosition;
    u32 jointID;
    s16* sequenceBoneData;

    romdef = obj->def;
    sequenceBoneData = NULL;
    if (romdef) {
        listPosition = 0;
        for (index = 0; index < romdef->numSequenceBones; index++){
            jointID = romdef->pSequenceBones[(listPosition + 1) + obj->modelInstIdx];
            if (jointID != 0xFF && sequenceBoneID == romdef->pSequenceBones[listPosition])
                sequenceBoneData = obj->unk6C[index];

            listPosition += 1 + romdef->numModels;
        }
    }
    return sequenceBoneData;
}

void* func_800348A0(Object* obj, s32 texType, s32 arg2) {
    u8 *texData;
    s32 i;
    s32 temp;
    s32 temp2;
    u8* var_v1;
    s32 frameCount;

    var_v1 = NULL;
    if (obj->def != NULL) {
        texData = obj->def->pTextures;
        frameCount = obj->def->numAnimatedFrames;
        for (i = 0; i < frameCount; i++) {
            temp = i << 1;
            if (texType == *(temp + texData)) {
                temp2 = i << 4;
                var_v1 = &((u8*)obj->unk70)[temp2];
            }
        }
    }

    return var_v1;
}

s32* func_800349B0(void) {
    return D_80091720;
}

void func_800349C0(Object* arg0, HeadAnimation* arg1, s32 soundID, s16 arg3, s32 arg4, u8 arg5) {
    u32 sp34;

    sp34 = arg1->headAimY;
    if (arg5 != 0 || sp34 == 0 || !gDLL_6_AMSFX->vtbl->func_B48(sp34)) {
        if (sp34 != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(sp34);
        }
        gDLL_6_AMSFX->vtbl->play_sound(arg0, soundID, MAX_VOLUME, &sp34, 0, 0, 0);
        arg1->headAimZ = arg4;
        arg1->headGoalAngle = -arg3;
        arg1->headAimY = sp34;
        arg1->aimIsActive = 1;
    }
}

void func_80034B54(Object* arg0, HeadAnimation* arg1, s16* soundIDs, u8 arg3) {
    func_800349C0(arg0, arg1, soundIDs[0], soundIDs[1], -1, arg3);
}

void func_80034B94(Object* arg0, HeadAnimation* arg1, s32 soundID) {
    func_800349C0(arg0, arg1, soundID , 0x500, -1, 0U);
}

void func_80034BC0(Object* obj, HeadAnimation* arg1) {
    s16* sp2C;
    u32 temp_s1;
    s32 sp24;

    temp_s1 = (u32) arg1->headAimY;
    sp24 = (s32) arg1->headAimZ;
    sp2C = func_80034804(obj, 1);
    if (arg1->aimIsActive != 0) {
        arg1->aimIsActive = 0;
    } else {
        if ((sp24 != -1) && (temp_s1 != 0)) {
            sp24 = sp24 - gUpdateRate;
            if (sp24 < 0) {
                arg1->headAimX = 0.0f;
                sp24 = sp24;
                gDLL_6_AMSFX->vtbl->func_A1C(temp_s1);
                arg1->headGoalAngle = 0;
                arg1->headAimY = 0.0f;
                arg1->headAimZ = -1.0f;
            }
            arg1->headAimZ = (f32) sp24;
        }
        if (temp_s1 != 0 && !gDLL_6_AMSFX->vtbl->func_B48(temp_s1)) {
            arg1->headAimX = 0.0f;
            gDLL_6_AMSFX->vtbl->func_A1C(temp_s1);
            arg1->headGoalAngle = 0;
            arg1->headAimY = 0.0f;
            arg1->headAimZ = -1.0f;
        }
    }

    sp2C[0] = (sp2C[0] + arg1->headGoalAngle) >> 1;
}

void func_80034D94(u8 arg0, u8 arg1) {
    if (!arg0) {
        D_800B2E00 = ((arg1 & 0xFF & 0xFF) << 7) | (D_800B2E00 & 0xFF7F);
    }
}
