#include "segment_334F0.h"
#include "sys/curves.h"
#include "dll.h"

static const char str_80099c90[] = " WARNING EXPR: This Object has no Head ";
static const char str_80099cb8[] = " WARNING EXPR: Obj Has No Joint %i ";
static const char str_80099cdc[] = " WARNING: Expr Contrl Flag does not exist \n";
extern u8 D_800B2E00;

s32 D_80091720[] = { 0x0, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10, 0x11, 0x12, 0x13 };
s32 D_80091748[] = { 0, 0 };

void func_800328F0(Object* obj, Unk80032CF8* arg1, f32 arg2) {
    f32* temp_v0; // TODO: this is an Object* ?

    if (obj->group == GROUP_UNK1) {
        arg1 = ((DLL_210_Player*)obj->dll)->vtbl->func54(obj);
        arg2 = ((DLL_210_Player*)obj->dll)->vtbl->func56(obj);
        if (((DLL_210_Player*)obj->dll)->vtbl->func66(obj, 2) != NULL) {
            arg2 = 1.0f;
        }
        temp_v0 = ((DLL_210_Player*)obj->dll)->vtbl->func52(obj);
        if (temp_v0 != NULL) {
            arg1->unk4 = temp_v0[3];
            arg1->x = temp_v0[4];
            arg1->y = temp_v0[5];
            arg1->unk0 = 1;
        } else {
            arg1->unk0 = 0;
        }
        func_80033B68(obj, arg1, arg2);
        return;
    }
    func_80033B68(obj, arg1, 0.0f);
}

void func_80032A08(Object* obj, s8* arg1) { // TODO: arg1 = Unk80032CF8*
    s32* sp1C;
    s32 temp_v1;
    s32* temp_v0;

    sp1C = func_800348A0(obj, 5, 0);
    temp_v0 = func_800348A0(obj, 4, 0);
    if ((sp1C == NULL)  || (temp_v0 == NULL)) {
        return;
    }

    temp_v1 = *temp_v0;
    switch (arg1[0x1E] & 0xF) {
    case 0:
        if (arg1[0x1F] > 0) {
            arg1[0x1F] -= delayByte;
        } else if (rand_next(0, 0x3E8) >= 0x3DA) {
            arg1[0x1E] = 1;
            arg1[0x1F] = 0;
        }
        break;
    case 1:
        if (arg1[0x1E] & 0x80) {
            temp_v1 -= 0x100;
            if (temp_v1 < 0) {
                temp_v1 = 0;
                arg1[0x1E] = 0;
                arg1[0x1F] = 0;
            }
        } else {
            temp_v1 += 0x100;
            if (temp_v1 >= 0x201) {
                temp_v1 -= 0x200;
                if (temp_v1 < 0) {
                    temp_v1 = 0;
                    arg1[0x1E] = 0;
                } else {
                    arg1[0x1E] = -0x7F;
                }
                arg1[0x1F] = 0;
            }
        }
        *sp1C = temp_v1;
        *temp_v0 = temp_v1;
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

void func_80032CF8(Object* obj, Object* otherObj, Unk80032CF8* arg2, s32 arg3) {
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
    if (sp2C[0] >= 0x8001) {
        sp2C[0] += 0xFFFF0001;
    }
    if (sp2C[0] < -0x8000) {
        sp2C[0] += 0xFFFF;
    }
    sp2C[1] = (arctan2_f(sp38, sp3C) - 0x3FFF) & 0xFFFF & 0xFFFF;
    arg3 = (s16) (arg3 * 182.04f);
    for (i = 0; i < 2; i++) {
        arg2[i].unk14 = sp2C[i];
        if (arg3 < arg2[i].unk14) {
            arg2[i].unk14 = arg3;
        }
        if (arg2[i].unk14 < -arg3) {
            arg2[i].unk14 = -arg3;
        }
    }
    sp34[1] = arg2->unk14;
    sp34[0] = arg2[1].unk14;
}

s16 func_80032EBC(Object* obj, Object* otherObj, Unk80032CF8* arg2, s16* arg3) {
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
            arg2->unk14 = var_a2;
            func_80034250(arg2, temp_v0);
            arg2++;
        } else {
            temp_v0[1] = var_a2;
        }
    }

    return var_s0;
}

s16 func_80033044(Object* obj, Object* otherObj, Vec3f* arg2, Unk80032CF8* arg3, s16* arg4) {
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
            var_s3 += arg3[i].unk14;
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
        var_s0 += arg3[i].unk14;
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
        arg3[i].unk14 = var_a1;
    }

    return var_s0;
}

void func_80033224(Object* arg0, s32* arg1, s32 arg2, Unk80032CF8* arg3) {
    s16* temp_v0;
    s32 i;

    for (i = 0; i < arg2; i++) {
        temp_v0 = func_80034804(arg0, arg1[i]);
        arg3->unk16 = temp_v0[1];
        arg3[1].unk16 = temp_v0[0];
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

void func_80033350(Unk80032CF8* arg0, s32 arg1, s16 arg2, s16 arg3) {
    s32 i;

    for (i = 0; i < arg1; i++) {
        arg0->unk14 = arg2;
        arg0[1].unk14 = arg3;
        arg0 += 2;
    }
}

s32 func_800333C8(Object* arg0, s32* arg1, s32 arg2, Unk80032CF8* arg3) {
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
s32 func_800334A4(Object* obj, Object* otherObj, Vec3f* arg2, Unk80032CF8* arg3, s16* arg4, f32 arg5, s16 arg6, s16 arg7);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_800334A4.s")
#else
// https://decomp.me/scratch/hIDz6
s32 func_800334A4(Object* obj, Object* otherObj, Vec3f* arg2, Unk80032CF8* arg3, s16* arg4, f32 arg5, s16 arg6, s16 arg7) {
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
    temp_t5 = arctan2_f(temp_fs0, sp8C) - (obj->srt.yaw & 0xFFFF);
    sp84[0] = temp_t5;
    if (temp_t5 >= 0x8001) {
        sp84[0] = temp_t5 + 0xFFFF0001;
    }
    if (sp84[0] < -0x8000) {
        sp84[0] += 0xFFFF;
    }
    i = 0;
    var_v1 = (s16)(arg7 - -arctan2_f(sp88, sp90));
    if (var_v1 >= 0x8001) {
        var_v1 = (s16)(var_v1 + 0xFFFF0001);
    }
    if (var_v1 < -0x8000) {
        var_v1 = (s16)(var_v1 + 0xFFFF);
    }
    sp84[1] = var_v1;
    if ((D_800B2E00 >> 0x1F) != 0) {
        sp84[0] -= 0x8000;
        sp84[1] = -var_v1;
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
            arg3->unk14 = sp80[0];
            func_80034250(arg3, temp_v0);
            arg3[1].unk14 = sp80[1];
            func_80034518(arg3 + 1, temp_v0, 10.0f, 500.0f);
            arg3 += 2;
        } else {
            temp_a1 = (s16) ((s16) ((sp80[0] + temp_v0[1]) >> 1) - temp_v0[1]);
            temp_lo = ((s16) (-arg4[i] * 182.04f) / 10) * delayByte;
            if (temp_a1 < temp_lo) {
                var_t7 = temp_lo;
            } else {
                temp_lo = ((s16) (arg4[i] * 182.04f) / 10) * delayByte;
                if (temp_lo < temp_a1) {
                    temp_a1 = temp_lo;
                }
                var_t7 = temp_a1;
            }
            temp_a0 = (s16) ((s16) ((sp80[1] + temp_v0[0]) >> 1) - temp_v0[0]);
            temp_lo = (-(s16) (arg4[i + 15] * 182.04f) / 10) * delayByte;
            if (temp_a0 < temp_lo) {
                var_a0 = temp_lo;
            } else {
                temp_lo = ((s16) (arg4[i + 15] * 182.04f) / 20) * delayByte;
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
        sp28->unk0x8 = arg3;
        return;
    }
    if (sp28->unk0x8 != 0.0f) {
        sp28->strength = (sp28->unk0x8 - arg3) / sp28->unk0x8;
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
            sp20->unk0x8 = arg3;
            return;
        }
        if (sp20->unk0x8 != 0.0f) {
            sp20->strength = (sp20->unk0x8 - arg3) / sp20->unk0x8;
            return;
        }
        sp20->strength = 0.0f;
    }
}

void func_80033B68(Object* obj, Unk80032CF8* arg1, f32 arg2) {
    s16* temp_v0;
    s32 var_v0;

    temp_v0 = func_80034804(obj, 0);
    if (temp_v0 == NULL) {
        return;
    }

    if (temp_v0[0] != 0) {
        temp_v0[0] = (temp_v0[0] * 3) / 4;
    }
    if (arg2 < 0.0f) {
        arg2 = -arg2;
    }
    if (arg2 <= 0.1f) {
        func_80033C54(obj, arg1, arg2, temp_v0);
    } else {
        func_80033FD8(obj, arg1, arg2, temp_v0);
    }
    var_v0 = arg2 > 0.1f ? 1 : 0;
    arg1->unk1A = (var_v0 << 8) | (arg1->unk1A & 0xFF);
}

void func_80033C54(Object* obj, Unk80032CF8* arg1, f32 arg2, s16* arg3) {
    s32 temp_v0;
    s32 pad2;
    s32 pad;
    s32 sp28;
    s32 sp24;

    temp_v0 = (u8)(arg1->unk1A >> 8);
    sp28 = arg2 > 0.1f ? 1 : 0;
    if (temp_v0 != sp28) {
        arg1->unk1A = sp28 << 8;
    }
    switch (arg1->unk1A & 0xFF) {
    case 0:
        if (arg1->unk0 != 0) {
            arg1->unk1A = (sp28 << 8) | 3;
            arg1->unk16 = arg3[1];
            arg1->z = 1.0f;
        } else {
            arg1->unk1A = (sp28 << 8) | 1;
            arg1->unk1C = rand_next(100, 400);
            arg1->unk14 = arg3[1];
        }
        break;
    case 1:
        arg1->unk1C -= delayByte;
        if (arg1->unk1C < 0) {
            sp24 = arg1->unk14;
            arg1->unk14 = rand_next(0, 0x1FFF);
            if (sp24 > 0) {
                if ((sp24 - arg1->unk14) < 0xE38) {
                    arg1->unk14 += 0xE38;
                }
                if (arg1->unk14 >= 0x2000) {
                    arg1->unk14 = 0x1FFF;
                }
                arg1->unk14 = -arg1->unk14;
            } else {
                if ((arg1->unk14 - sp24) < 0xE38) {
                    arg1->unk14 += 0xE38;
                }
                if (arg1->unk14 >= 0x2000) {
                    arg1->unk14 = 0x1FFF;
                }
            }
            arg1->unk1A = (sp28 << 8) | 2;
            arg1->unk1C = 0;
            arg1->unk16 = arg3[1];
        }
        break;
    case 2:
        if (arg1->unk0 || func_80034250(arg1, arg3)) {
            arg1->unk1A = sp28 << 8;
        }
        break;
    case 3:
        if (arg1->unk0 == 0) {
            arg1->unk1A = sp28 << 8;
        } else {
            arg1->unk14 = arctan2_f(obj->srt.transl.x - arg1->unk4, obj->srt.transl.z - arg1->y) - (obj->srt.yaw & 0xFFFF);
            if (arg1->unk14 >= 0x8001) {
                arg1->unk14 += 0xFFFF0001;
            }
            if (arg1->unk14 < -0x8000) {
                arg1->unk14 += 0xFFFF;
            }
            if ((arg1->unk14 >= 0x2000) || (arg1->unk14 < -0x1FFF)) {
                arg1->unk1A = sp28 << 8;
            } else {
                if (arg1->z > 0.0f) {
                    arg3[1] = arg1->unk14 + ((arg1->unk16 - arg1->unk14) * arg1->z);
                    arg1->z -= 0.01f * delayFloat;
                    if (arg1->z < 0.0f) {
                        arg1->z = 0.0f;
                    }
                } else {
                    arg3[1] = arg1->unk14;
                }
            }
        }
        break;
    }
    if (arg3[1] < -0x1FFF) {
        arg3[1] = -0x1FFF;
    } else if (arg3[1] >= 0x2000) {
        arg3[1] = 0x1FFF;
    }
}

void func_80033FD8(Object* obj, Unk80032CF8* arg1, f32 arg2, s16* arg3) {
    s32 v0;
    s32 sp20;

    v0 = (u8)(arg1->unk1A >> 8);
    sp20 = arg2 > 0.1f ? 1 : 0;
    if (v0 != sp20) {
        arg1->unk1A = (sp20 << 8) | 4;
        arg1->unk16 = arg3[1];
        arg1->unk14 = 0;
        arg1->unk1C = 0;
    }
    switch (arg1->unk1A & 0xFF) {
    case 0:
        arg1->unk1A = sp20 << 8;
        arg1->unk1C = rand_next(0x32, 0xC8);
        return;
    case 1:
        arg1->unk1C -= delayByte;
        if (arg1->unk1C < 0 && !(rand_next(0, 0x64) < 0x5B)) {
            arg1->unk1A = (sp20 << 8) | 5;
            if (arg1->unk0 != 0) {
                if (rand_next(0, 0x64) > 0) {
                    arg1->unk14 = 0x1FFF;
                    if (rand_next(0, 1) == 0) {
                        arg1->unk14 = -arg1->unk14;
                        return;
                    }
                }
            } else {
                arg1->unk14 = 0x1FFF;
                if (rand_next(0, 1) == 0) {
                    arg1->unk14 = -arg1->unk14;
                    return;
                }
            }
        }
        break;
    case 5:
        if (arg1->unk1C > 0) {
            arg1->unk1C -= delayByte;
            return;
        }
        if (func_80034250(arg1, arg3) != 0) {
            arg1->unk1A = (sp20 << 8) | 6;
            arg1->unk14 = -arg1->unk14;
            arg1->unk1C = rand_next(0x14, 0x64);
            return;
        }
        break;
    case 6:
        if (arg1->unk1C > 0) {
            arg1->unk1C -= delayByte;
            return;
        }
        if (func_80034250(arg1, arg3) != 0) {
            arg1->unk1A = (sp20 << 8) | 4;
            arg1->unk14 = 0;
            arg1->unk1C = rand_next(0x14, 0x64);
            return;
        }
        break;
    case 4:
        if (arg1->unk1C > 0) {
            arg1->unk1C -= delayByte;
            return;
        }
        if (func_80034250(arg1, arg3) != 0) {
            arg1->unk1A = sp20 << 8;
            arg3[1] = 0;
        }
        break;
    }
}

s32 func_80034250(Unk80032CF8* arg0, s16* arg1) {
    Vec4f sp38;
    f32 sp34;
    f32 var_fa0;

    sp38.x = 10.0f;
    sp38.y = 10.0f;
    sp38.z = 500.0f;
    sp38.w = -500.0f;
    if (arg0->unk14 != arg0->unk16) {
        sp34 = ((f32) arg1[1] - arg0->unk16) / ((f32) arg0->unk14 - arg0->unk16);
    } else {
        return 1;
    }

    if (sp34 > 1.0f) {
        sp34 = 1.0f;
    } else if (sp34 < 0.0f) {
        sp34 = 0.0f;
    }
    var_fa0 = func_80004C5C(&sp38, sp34, NULL);
    if (arg0->unk14 < arg0->unk16) {
        var_fa0 = -var_fa0;
    }
    arg1[1] += var_fa0 * delayFloat;
    if (sp34 == 1.0f || !(arg1[1] < 0x1FFF) || arg1[1] < -0x1FFE) {
        arg1[1] = arg0->unk14;
        return 1;
    }

    return 0;
}

s32 func_800343B8(Unk80032CF8* arg0, s16* arg1, f32 arg2, f32 arg3) {
    Vec4f sp38;
    f32 sp34;
    f32 var_fa0;

    sp38.x = arg2;
    sp38.y = arg2;
    sp38.z = arg3;
    sp38.w = -arg3;
    if (arg0->unk14 != arg0->unk16) {
        sp34 = ((f32) arg1[1] - arg0->unk16) / ((f32) arg0->unk14 - arg0->unk16);
    } else {
        return 1;
    }

    if (sp34 > 1.0f) {
        sp34 = 1.0f;
    } else if (sp34 < 0.0f) {
        sp34 = 0.0f;
    }
    var_fa0 = func_80004C5C(&sp38, sp34, NULL);
    if (arg0->unk14 < arg0->unk16) {
        var_fa0 = -var_fa0;
    }
    arg1[1] += var_fa0 * delayFloat;
    if (sp34 == 1.0f || !(arg1[1] < 0x1FFF) || arg1[1] < -0x1FFE) {
        arg1[1] = (s16) arg0->unk14;
        return 1;
    }

    return 0;
}

s32 func_80034518(Unk80032CF8* arg0, s16* arg1, f32 arg2, f32 arg3) {
    Vec4f sp38;
    f32 sp34;
    f32 var_fa0;

    sp38.x = arg2;
    sp38.y = arg2;
    sp38.z = arg3;
    sp38.w = -arg3;
    if (arg0->unk14 != arg0->unk16) {
        sp34 = ((f32) arg1[0] - arg0->unk16) / ((f32) arg0->unk14 - arg0->unk16);
    } else {
        return 1;
    }

    if (sp34 > 1.0f) {
        sp34 = 1.0f;
    } else if (sp34 < 0.0f) {
        sp34 = 0.0f;
    }
    var_fa0 = func_80004C5C((Vec4f* ) &sp38, sp34, NULL);
    if (arg0->unk14 < arg0->unk16) {
        var_fa0 = -var_fa0;
    }
    arg1[0] += var_fa0 * delayFloat;
    if (sp34 == 1.0f || !(arg1[0] < 0x1FFF) || arg1[0] < -0x1FFE) {
        arg1[0] = arg0->unk14;
        return 1;
    }
    return 0;
}

void func_80034678(Object* arg0, s8* arg1, f32 arg2) {
    s16* sp1C;
    s16* sp18;
    s32 var_v0;

    sp1C = func_800348A0(arg0, 1, 0);
    sp18 = func_800348A0(arg0, 0, 0);
    if ((sp1C != NULL) && (sp18 != NULL)) {
        var_v0 = 0;
        if (arg1[0x20] == 0) {
            var_v0 = 1;
        }
        if ((arg1[0x20] > 0) && (sp1C[4] >= arg1[0x22])) {
            var_v0 = 1;
        }
        if ((arg1[0x20] < 0) && (arg1[0x22] >= sp1C[4])) {
            var_v0 = 1;
        }
        if (var_v0 != 0) {
            arg1[0x22] = rand_next(-0x64, 0xA);
            if (arg1[0x22] < sp1C[4]) {
                arg1[0x20] = -5;
            } else {
                arg1[0x20] = 5;
            }
            arg1[0x21] = rand_next(0x1E, 0x64);
        }
        if (arg1[0x21] > 0) {
            arg1[0x21] -= delayByte;
            return;
        }
        sp1C[4] += arg1[0x20] * delayByte;
        sp1C[5] = 0;
        sp18[4] = -0x64 - sp1C[4];
        sp18[5] = 0;
    }
}

/** object_find_seq_bone_data_by_bone_id? */
s16* func_80034804(Object* obj, s32 sequenceBoneID) {
    ObjDef* romdef;
    u8* seqBones;
    s32 index;
    s32 listPosition;
    u32 jointID;
    u16* sequenceBoneData;

    romdef = obj->def;
    sequenceBoneData = NULL;
    if (romdef) {
        listPosition = 0;
        for (index = 0; index < romdef->numSequenceBones; index++){
            jointID = romdef->pSequenceBones[(listPosition + 1) + obj->modelInstIdx];
            if (jointID != 0xFF && sequenceBoneID == romdef->pSequenceBones[listPosition])
                sequenceBoneData = obj->ptr0x6c[index];

            listPosition += 1 + romdef->numModels;
        }
    }
    return sequenceBoneData;
}

void* func_800348A0(Object* arg0, s32 texType, s32 arg2) {
    u8 *texData;
    s32 i;
    s32 temp;
    s32 temp2;
    u8* var_v1;
    s32 frameCount;

    var_v1 = NULL;
    if (arg0->def != NULL) {
        texData = arg0->def->pTextures;
        frameCount = arg0->def->numAnimatedFrames;
        for (i = 0; i < frameCount; i++) {
            temp = i << 1;
            if (texType == *(temp + texData)) {
                temp2 = i << 4;
                var_v1 = &((u8*)arg0->ptr0x70)[temp2];
            }
        }
    }

    return var_v1;
}

s32* func_800349B0(void) {
    return D_80091720;
}

void func_800349C0(Object* arg0, Unk80032CF8* arg1, s32 soundID, s16 arg3, s32 arg4, u8 arg5) {
    u32 sp34;

    sp34 = arg1->x;
    if (arg5 != 0 || sp34 == 0 || !gDLL_6_AMSFX->vtbl->func_B48(sp34)) {
        if (sp34 != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(sp34);
        }
        gDLL_6_AMSFX->vtbl->play_sound(arg0, soundID, MAX_VOLUME, &sp34, 0, 0, 0);
        arg1->y = arg4;
        arg1->unk14 = -arg3;
        arg1->x = sp34;
        arg1->unk0 = 1;
    }
}

void func_80034B54(Object* arg0, Unk80032CF8* arg1, s16* soundIDs, u8 arg3) {
    func_800349C0(arg0, arg1, soundIDs[0], soundIDs[1], -1, arg3);
}

void func_80034B94(Object* arg0, Unk80032CF8* arg1, s32 soundID) {
    func_800349C0(arg0, arg1, soundID , 0x500, -1, 0U);
}

void func_80034BC0(Object* obj, Unk80032CF8* arg1) {
    s16* sp2C;
    u32 temp_s1;
    s32 sp24;

    temp_s1 = (u32) arg1->x;
    sp24 = (s32) arg1->y;
    sp2C = func_80034804(obj, 1);
    if (arg1->unk0 != 0) {
        arg1->unk0 = 0;
    } else {
        if ((sp24 != -1) && (temp_s1 != 0)) {
            sp24 = sp24 - delayByte;
            if (sp24 < 0) {
                arg1->unk4 = 0.0f;
                sp24 = sp24;
                gDLL_6_AMSFX->vtbl->func_A1C(temp_s1);
                arg1->unk14 = 0;
                arg1->x = 0.0f;
                arg1->y = -1.0f;
            }
            arg1->y = (f32) sp24;
        }
        if (temp_s1 != 0 && !gDLL_6_AMSFX->vtbl->func_B48(temp_s1)) {
            arg1->unk4 = 0.0f;
            gDLL_6_AMSFX->vtbl->func_A1C(temp_s1);
            arg1->unk14 = 0;
            arg1->x = 0.0f;
            arg1->y = -1.0f;
        }
    }

    sp2C[0] = (sp2C[0] + arg1->unk14) >> 1;
}

void func_80034D94(u8 arg0, u8 arg1) {
    if (!arg0) {
        D_800B2E00 = ((arg1 & 0xFF & 0xFF) << 7) | (D_800B2E00 & 0xFF7F);
    }
}
