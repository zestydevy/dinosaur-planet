#include "dlls/engine/53_movelib.h"
#include "sys/curves.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/objects.h"
#include "sys/objlib.h"
#include "sys/objexpr.h"
#include "sys/objtype.h"
#include "sys/rand.h"
#include "sys/segment_53F00.h"
#include "dll.h"
#include "macros.h"

/*0x0*/ static s16 data_0[] = {
    0x0023, 0x0023, 0x0023, 0x0023, 
    0x0023, 0x0023, 0x0023, 0x0023, 
    0x0023, 0x0023, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000
};

static void movelib_func_106C(Object* actor, Object* animObj, AnimObj_Data* animObjData);
static void movelib_func_16B0(Object* arg0, UnkCurvesStruct* arg1, s32 arg2, s32 arg3, f32 arg4);
static s32 movelib_func_173C(Object* arg0, UnkCurvesStruct* arg1, f32 arg2);
static s32 movelib_func_17F4(Object* arg0, DLL53Func17F4Arg1* arg1, DLL53Func17F4Arg2* arg2, f32* arg3, f32 arg4);
static f32 movelib_func_1A1C(DLL53Func17F4Arg2* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, s32 arg4);
void movelib_func_1E70(MoveLibData* arg0, s16* arg1, s16* arg2, s32 arg3);

// offset: 0x0 | ctor
void movelib_ctor(void *dll) {
    STUBBED_PRINTF("\n MOVELIB LOADED \n");
}

// offset: 0xC | dtor
void movelib_dtor(void *dll) {
    STUBBED_PRINTF("\n MOVELIB FREEDED \n");
}

// offset: 0x18 | func: 0
static s32 movelib_func_18(Object* arg0, Object* arg1, s32* arg2, MoveLibData* arg3, f32* arg4, s16* arg5, Vec3f* arg6) {
    s16 sp3E;
    s16 sp3C;
    f32 sp38;
    s32* sp34;
    HeadAnimation* var_a3;

    sp34 = objExpr_func_800349B0();
    if (arg1->objhitInfo != NULL) {
        if (arg1->objhitInfo->unk5A & 2) {
            sp38 = arg1->objhitInfo->unk56 * 4.0f;
        } else if (arg1->objhitInfo->unk5A & 1) {
            sp38 = arg1->objhitInfo->unk52;
        } else {
            sp38 = 30.0f;
        }
    } else {
        sp38 = 30.0f;
    }
    sp3E = objAngleToObjectXZ(arg0, arg1, NULL);
    sp3C = objExpr_func_800334A4(arg0, arg1, &arg3->headRefPoint, 
                         (arg3->unk4A9 & 8) ? NULL : arg3->unk1C, 
                         arg3->unk454, sp38, 8, arg3->unk4A4);
    if (!(arg3->unk4A9 & 8)) {
        arg3->unk490 = !objExpr_func_800333C8(arg0, sp34, arg3->jointCount, arg3->unk1C);
    }
    arg3->unk490 = 0;
    if ((arg3->unk4A9 & 2) && (sp3C != 0)) {
        *arg2 = 0;
        return 0;
    }
    if (arg3->unk490 == 0) {
        if ((-arg3->unk4A6 < sp3E) && (sp3E < arg3->unk4A6)) {
            *arg4 = 0.005f;
            *arg2 = 0;
            STUBBED_PRINTF(" In turn Range ");
            return !sp3C;
        }
    }
    if ((*arg2 == 0) && (sp3C != 0)) {
        *arg2 = 1;
        *arg4 = 0.005f;
    } else if (*arg2 != 0) {
        if (sp3E > 0 && arg0->curModAnimId != arg5[1]) {
            objAnimSet(arg0, arg5[1], 0.0f, 0);
            objAnim_func_80024D74(arg0, 30);
        }
        if (sp3E < 0 && arg0->curModAnimId != arg5[0]) {
            objAnimSet(arg0, arg5[0], 0.0f, 0);
            objAnim_func_80024D74(arg0, 30);
        }
        if (sp3C == 0) {
            sp3E = sp3E > 0 ? (sp3E / 20) : (sp3E / 20);
        } else {
            sp3E = sp3E > 0 ? ((sp3E - 0x500) / 20) : ((sp3E + 0x500) / 20);
        }
        arg0->srt.yaw += sp3E;
        *arg4 = (f32) (sp3E >= 0 ? sp3E : -sp3E) / 20922.25f;
    }
    return 1;
}

// offset: 0x3B4 | func: 1
static void movelib_func_3B4(Object* arg0, Object* arg1, MoveLibData* arg2, s16* arg3) {
    s32* temp_a1 = objExpr_func_800349B0();
    switch (arg2->unk498) {
    case 3:
        objExpr_func_80033224(arg0, temp_a1, arg2->jointCount, arg2->unk1C);
        arg2->unk490 = 0;
        arg2->unk498 = 2;
        /* fallthrough */
    case 2:
        if (movelib_func_18(arg0, arg1, &arg2->unk494, arg2, &arg2->unk0, arg3, &arg2->headRefPoint) == 0) {
            arg2->unk498 = 6;
        }
        break;
    case 6:
        arg2->unk498 = 7;
        /* fallthrough */
    case 7:
        arg2->unk0 = 0.005f;
        break;
    }
    arg2->prevLookat = arg1;
}

// offset: 0x4B8 | func: 2 | export: 0
void movelib_func_4B8(Object* obj, MoveLibData* data) {
    f32 maxLookAtSearchDist;
    f32 lookatYOffset;
    f32 temp_fv1;
    s16 sp5A;
    s32* sp54;
    Object* lookat;
    f32 sp44[3];

    maxLookAtSearchDist = 1000.0f;
    lookatYOffset = 30.0f;
    sp5A = 0;
    sp54 = objExpr_func_800349B0();
    objGetPlayer();
    if (data->unk499 == 0) {
        if ((data->unk4A9 & 1) && (data->unk498 != 8)) {
            data->unk498 = 8;
            if (!(data->unk4A9 & 8)) {
                objExpr_func_80033224(obj, sp54, data->jointCount, data->unk1C);
                data->unk490 = 0x50;
                objExpr_func_80033350(data->unk1C, data->jointCount, 0, 0);
            } else {
                objExpr_func_800332A4(obj, objExpr_func_800349B0(), data->jointCount);
            }
        } else if (!(data->unk4A9 & 1) && (data->unk498 == 8)) {
            data->unk498 = 0;
            if (!(data->unk4A9 & 8)) {
                objExpr_func_80033224(obj, sp54, data->jointCount, data->unk1C);
                data->unk490 = 0x50;
            }
        }
        if (data->unk498 > 1) {
            if ((data->unk490 != 0) && !(data->unk4A9 & 8)) {
                data->unk490 = !objExpr_func_800333C8(obj, sp54, data->jointCount, data->unk1C);
                return;
            }
            objExpr_func_800332A4(obj, objExpr_func_800349B0(), data->jointCount);
            return;
        }
        if (data->lookat == NULL) {
            lookat = objGetNearestTypeTo(OBJTYPE_LookAt, obj, &maxLookAtSearchDist);
        } else {
            lookat = data->lookat;
        }
        if (NULL != lookat) {
            if (data->unk4A9 & 0x20) {
                sp44[0] = data->headRefPoint.x - lookat->srt.transl.x;
                sp44[1] = data->headRefPoint.y - lookat->srt.transl.y;
                sp44[2] = data->headRefPoint.z - lookat->srt.transl.z;
                temp_fv1 = sqrtf(SQ(sp44[0]) + SQ(sp44[2]));
                if (temp_fv1 <= 40.0f) {
                    temp_fv1 = (temp_fv1 - 10.0f) / 30.0f;
                    temp_fv1 = temp_fv1 < 0.0f
                        ? 0.0f
                        : temp_fv1 > 1.0f
                            ? 1.0f
                            : temp_fv1;
                    temp_fv1 = 1.0f - temp_fv1;
                    data->headRefPoint.x = (data->headRefPoint.x * (1.0f - temp_fv1)) + (obj->srt.transl.x * temp_fv1);
                    data->headRefPoint.z = (data->headRefPoint.z * (1.0f - temp_fv1)) + (obj->srt.transl.z * temp_fv1);
                }
            }
            if ((data->unk4AC != -1) && (lookat == data->prevLookat)) {
                if (((data->unk4B4 -= gUpdateRate) <= 0) && ((data->unk4B4 + gUpdateRate) > 0)) {
                    objExpr_func_80033224(obj, sp54, data->jointCount, data->unk1C);
                    data->unk490 = 0x50;
                    objExpr_func_80033350(data->unk1C, data->jointCount, 0, 0);
                    data->unk498 = 0;
                    return;
                }
                if (data->unk490 != 0) {
                    data->unk490 = !objExpr_func_800333C8(obj, sp54, data->jointCount, data->unk1C);
                }
                if (data->unk4B4 < -data->unk4B0) {
                    data->unk4B4 = mathRnd(data->unk4B0, data->unk4AC);
                }
                if (data->unk4B4 < 0) {
                    return;
                }
            } else {
                data->unk4B4 = data->unk4AC;
            }
            if ((lookat != data->prevLookat) && (lookat != NULL)) {
                if (lookat->objhitInfo != NULL) {
                    if (lookat->objhitInfo->unk5A & 2) {
                        lookatYOffset = (f32) lookat->objhitInfo->unk56 * 4.0f;
                    } else if (lookat->objhitInfo->unk5A & 1) {
                        lookatYOffset = (f32) lookat->objhitInfo->unk52;
                    } else {
                        lookatYOffset = 30.0f;
                    }
                } else {
                    lookatYOffset = 30.0f;
                }
            }
            if (lookat != NULL) {
                sp5A = objAngleToObjectXZ(obj, lookat, NULL);
            }
            if (data->unk4A9 & 0x10) {
                objExpr_func_80034D94(0, 1);
                sp5A -= 0x8000;
            }
            if ((((sp5A >= 0) ? sp5A : -sp5A) >= 0x5555) || (lookat == NULL)) {
                if ((data->unk498 != 0) || ((lookat == NULL) && (data->prevLookat != NULL))) {
                    objExpr_func_80033224(obj, sp54, data->jointCount, data->unk1C);
                    data->unk490 = 0xA;
                    objExpr_func_80033350(data->unk1C, data->jointCount, 0, 0);
                    data->unk498 = 0;
                }
            } else {
                if ((lookat != data->prevLookat) || (data->unk498 == 0)) {
                    objExpr_func_80033224(obj, sp54, data->jointCount, data->unk1C);
                    data->unk490 = 1;
                }
                if (data->unk4A9 & 8) {
                    data->unk490 = 0;
                }
                objExpr_func_800334A4(obj, lookat, &data->headRefPoint, 
                              data->unk490 != 0 ? data->unk1C : NULL, 
                              data->unk454, lookatYOffset, 8, data->unk4A4);
                data->unk498 = 1;
            }
            data->prevLookat = lookat;
            data->lookat = NULL;
            if (!(data->unk4A9 & 8) && (data->unk490 != 0)) {
                data->unk490 = !objExpr_func_800333C8(obj, sp54, data->jointCount, data->unk1C);
            }
        }
    }
}

// offset: 0xB84 | func: 3 | export: 1
void movelib_func_B84(MoveLibData* data, Object* lookat) {
    data->lookat = lookat;
}

// offset: 0xB90 | func: 4 | export: 5
void movelib_func_B90(MoveLibData* arg0, s32 arg1, s32 arg2) {
    arg0->unk4AC = arg1;
    arg0->unk4B0 = arg2;
    arg0->unk4B4 = arg1;
}

// offset: 0xBA4 | func: 5 | export: 2
void movelib_func_BA4(Object* obj, MoveLibData* data, s16 arg2, s16 arg3, s32 jointCount) {
    data->unk4A4 = arg2;
    data->unk4A6 = arg3;
    data->jointCount = jointCount;
    data->unk494 = 0;
    data->unk490 = 0;
    data->prevLookat = NULL;
    data->lookat = NULL;
    data->unk498 = 0;
    data->unk499 = 1;
    data->unk4.z = 0.0f;
    data->unk4.y = 0.0f;
    data->unk4.x = 0.0f;
    data->unk0 = 0/*.0f*/;
    data->unk4AC = -1;
    objExpr_func_800332A4(obj, objExpr_func_800349B0(), jointCount);
    objExpr_func_80033224(obj, objExpr_func_800349B0(), jointCount, data->unk1C);
    objExpr_func_80033350(data->unk1C, data->jointCount, 0, 0);
    movelib_func_1E70(data, data_0, data_0, 8);
}

// offset: 0xCDC | func: 6 | export: 3
void movelib_func_CDC(Object* arg0, MoveLibData* arg1, s32 arg2) {
    Vec3f sp4C;
    Vec3f sp3C;
    s16 sp38[3];

    if (arg1->unk499 != 0) {
        objExpr_func_800332A4(arg0, objExpr_func_800349B0(), arg1->jointCount);
        objGetAttachPointWorldSpace(arg0, arg2, &sp3C.x, &sp3C.y, &sp3C.z, 0);
        objGetAttachPointWorldSpace(arg0, arg2 + 1, &sp4C.f[0], &sp4C.f[1], &sp4C.f[2], 0);
        arg1->unk4.x = ((sp4C.f[0] + (3.0f * sp3C.x)) * 0.25f);
        arg1->unk4.y = sp3C.y;
        arg1->unk4.z = ((sp4C.f[2] + (3.0f * sp3C.z)) * 0.25f);
        arg1->unk4.x = (arg1->unk4.x - arg0->srt.transl.x);
        arg1->unk4.y = (arg1->unk4.y - arg0->srt.transl.y);
        arg1->unk4.z = (arg1->unk4.z - arg0->srt.transl.z);
        sp38[0] = -arg0->srt.roll;
        sp38[1] = -arg0->srt.pitch;
        sp38[2] = -arg0->srt.yaw;
        mathRotateRPY((const SRT*)sp38, arg1->unk4.f);
        arg1->unk499 = 0;
    }
    objGetAttachPointWorldSpace(arg0, arg2, &sp3C.x, &sp3C.y, &sp3C.z, 0);
    arg1->headRefPoint.x = sp3C.x;
    arg1->headRefPoint.y = sp3C.y;
    arg1->headRefPoint.z = sp3C.z;
}

// offset: 0xE9C | func: 7 | export: 4
s32 movelib_func_E9C(Object* arg0, AnimObj_Data* arg1, MoveLibData* arg2, s16 arg3, s16 arg4) {
    Object* temp_v0;
    s16 sp38[2];
    s16* temp_v0_3;

    temp_v0 = objGetPlayer();
    sp38[0] = arg3;
    sp38[1] = arg4;
    arg0->objhitInfo->unk58 |= 1;
    if (arg1->unk62 == 4) {
        arg2->unk490 = 0x50;
        arg1->unk7A &= ~0x8;
        arg1->unk7A &= ~0x2;
        arg2->unk498 = 3;
        arg1->unk62 = 5;
        arg1->unk7A &= ~0x4;
        arg1->unkF4 = movelib_func_106C;
        return 1;
    }
    if (arg1->unk62 == 5) {
        if ((arg2->unk498 >= 2) && (arg2->unk498 < 8)) {
            movelib_func_3B4(arg0, temp_v0, arg2, sp38);
            objAnimAdvance(arg0, arg2->unk0, (f32) gUpdateRate, NULL);
            if (arg2->unk498 == 7) {
                arg1->unk7A |= 8;
                temp_v0_3 = objExpr_func_80034804(arg0, 0);
                arg1->unk120 = temp_v0_3[1];
                arg1->unk122 = temp_v0_3[0];
                arg2->unk498 = 0;
                arg1->unk62 = 0;
                arg1->unk7A |= 4;
                return 0;
            }
            return 1;
        }
    }
    return 0;
}

// offset: 0x106C | func: 8
static void movelib_func_106C(Object* actor, Object* animObj, AnimObj_Data* animObjData) {
    MoveLibData* sp2C;
    s32* sp28;

    sp28 = objExpr_func_800349B0();
    sp2C = actor->data;
    gDLL_2_Camera->vtbl->set_target_object(NULL);
    sp2C->unk498 = 0;
    objExpr_func_80033224(actor, sp28, sp2C->jointCount, sp2C->unk1C);
    sp2C->unk490 = 0x50;
    objExpr_func_80033350(sp2C->unk1C, sp2C->jointCount, 0, 0);
}

// offset: 0x1130 | func: 9 | export: 10
s32 movelib_func_1130(Object* arg0, SRT* arg1, f32 arg2, s32 arg3, f32* arg4, u8* arg5) {
    Vec3f sp3C;
    f32 sp38;
    f32 sp34;
    s16 var_a0;

    if (arg1 == NULL) {
        return 0;
    }
    sp3C.f[0] = arg1->transl.x - arg0->srt.transl.x;
    sp3C.f[1] = arg1->transl.f[1] - arg0->srt.transl.f[1];
    sp3C.f[2] = arg1->transl.f[2] - arg0->srt.transl.f[2];
    sp38 = sqrtf(SQ(sp3C.f[0]) + SQ(sp3C.f[1]) + SQ(sp3C.f[2]));
    if (sp38 < (arg2 * 5.0f)) {
        arg0->srt.transl.x = arg1->transl.x;
        arg0->srt.transl.f[1] = arg1->transl.f[1];
        arg0->srt.transl.f[2] = arg1->transl.f[2];
        if ((*arg5 & 1) && (func_80058680(arg0, arg0->srt.transl.x, arg0->srt.transl.f[1], arg0->srt.transl.f[2], &sp34, 0U) == 0)) {
            arg0->srt.transl.f[1] -= sp34;
        }
        return 1;
    }
    guNormalize(&sp3C.f[0], &sp3C.f[1], &sp3C.f[2]);
    arg0->velocity.x = sp3C.f[0] * (arg2 * gUpdateRateF);
    arg0->velocity.f[1] = sp3C.f[1] * (arg2 * gUpdateRateF);
    arg0->velocity.f[2] = sp3C.f[2] * (arg2 * gUpdateRateF);
    if ((*arg5 & 1) && (func_80058680(arg0, arg0->srt.transl.x, arg0->srt.transl.f[1], arg0->srt.transl.f[2], &sp34, 0U) == 0)) {
        arg0->srt.transl.f[1] -= sp34;
    }
    if (*arg5 & 2) {
        var_a0 = arg1->yaw - (u16)arg0->srt.yaw;
        CIRCLE_WRAP(var_a0);
        arg0->srt.yaw = (s16) (s32) ((f32) arg0->srt.yaw + ((((f32) var_a0 + 0.5f) * (arg2 * gUpdateRateF)) / sp38));
    }
    objMove(arg0, arg0->velocity.x, arg0->velocity.f[1], arg0->velocity.f[2]);
    if (arg3 != -1) {
        if (arg3 != arg0->curModAnimId) {
            objAnimSet(arg0, arg3, 0.0f, 0U);
        }
        var_a0 = (u16)mathAtan2f(sp3C.f[0], sp3C.f[2]);
        var_a0 = arg0->srt.yaw - (var_a0 & 0xFFFF);
        CIRCLE_WRAP(var_a0);
        arg2 *= -mathCosfInterp(var_a0);
        objGetAnimChange(arg0, arg2, arg4);
    }
    
    return 0;
}

// offset: 0x14F4 | func: 10 | export: 11
s32 movelib_func_14F4(Object* arg0, UnkCurvesStruct* arg1, DLL53Func17F4Arg2* arg2, f32 arg3, u8 arg4, f32* arg5, s32* arg6) {
    f32 xDiff, zDiff;
    s32 sp3C;
    f32 sp38;

    sp3C = 0;
    sp38 = 0.0f;
    if (*arg6 & 0x10) {
        return 1;
    }
    if (*arg6 & 4) {
        if (movelib_func_17F4(arg0, NULL, arg2, &arg2->unk30, arg3) != 0) {
            STUBBED_PRINTF("Got Curve ");
            movelib_func_16B0(arg0, arg1, 2, arg4, 200.0f);
            *arg6 |= 8;
        }
    } else {
        sp3C = movelib_func_173C(arg0, arg1, arg3);
        if (sp3C != 0) {
            *arg6 |= 0x10;
        }
    }
    objGetAnimChange(arg0, arg3, arg5);
    if ((*arg6 & 1) && (func_80058680(arg0, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, &sp38, 0) == 0)) {
        arg0->srt.transl.y -= sp38;
    }
    if (*arg6 & 2) {
        xDiff = arg0->srt.transl.x - arg0->prevLocalPosition.x;
        zDiff = arg0->srt.transl.z - arg0->prevLocalPosition.z;
        arg0->srt.yaw += ((s16)(mathAtan2f(xDiff, zDiff) + 0x8000) - arg0->srt.yaw) >> 3;
    }
    return sp3C;
}

// offset: 0x16B0 | func: 11
static void movelib_func_16B0(Object* arg0, UnkCurvesStruct* arg1, s32 arg2, s32 arg3, f32 arg4) {
    s32 sp28[2];

    if (arg2 == 1) {
        sp28[0] = 0;
        sp28[1] = 0;
    } else {
        sp28[0] = 0x19;
        sp28[1] = 0x15;
    }
    gDLL_26_Curves->vtbl->func_4288(arg1, arg0, arg4, sp28, arg3);
    STUBBED_PRINTF(" PathId %i Loc id %i \n", arg3, arg1->unk9C->unk18);
}

// offset: 0x173C | func: 12
static s32 movelib_func_173C(Object* arg0, UnkCurvesStruct* arg1, f32 arg2) {
    s32 sp24;

    sp24 = 0;
    if ((curves_func_800053B0(&arg1->unk0, arg2) != 0) || (arg1->unk0.unk10 != 0)) {
        sp24 = gDLL_26_Curves->vtbl->func_4704(arg1);
    }
    arg0->srt.transl.x = arg1->unk0.unk68.x;
    arg0->srt.transl.y = arg1->unk0.unk68.y;
    arg0->srt.transl.z = arg1->unk0.unk68.z;
    return sp24;
}

// offset: 0x17F4 | func: 13
static s32 movelib_func_17F4(Object* arg0, DLL53Func17F4Arg1* arg1, DLL53Func17F4Arg2* arg2, f32* tValue, f32 arg4) {
    f32 spline[4];
    s16 sp40[3];
    s32 sp3C;

    sp3C = 0;
    if (arg1 != NULL) {
        arg2->unk18.y = 0.0f;
        arg2->unk18.z = 0.0f;
        arg2->unk24.y = 0.0f;
        arg2->unk24.z = 0.0f;
        arg2->unk18.x = -150.0f;
        arg2->unk24.x = -150.0f;
        mathRotateYPR(&arg0->srt, &arg2->unk18);
        STUBBED_PRINTF(" Tangent 1 %f %f %f \n", &arg2->unk18.x, &arg2->unk18.y, &arg2->unk18.z);
        sp40[2] = 0;
        sp40[1] = (s16) arg1->unk2D;
        sp40[0] = (s16) arg1->unk2C;
        mathRotateYPR((const SRT*)&sp40, &arg2->unk24);
        STUBBED_PRINTF(" Tangent 2 %f %f %f \n", &arg2->unk24.x, &arg2->unk24.y, &arg2->unk24.z);
        *tValue = 0.0f;
        arg2->unk34 = movelib_func_1A1C(arg2, &arg2->unk18, &arg2->unkC, &arg2->unk24, 0xA);
    } else {
        *tValue += (arg4 * (f32) gUpdateRate) / arg2->unk34;
        STUBBED_PRINTF("t value %f ", tValue);
        if (*tValue >= 1.0f) {
            sp3C = 1;
            *tValue = 1.0f;
        }
    }
    spline[0] = arg2->unk0.x;
    spline[1] = arg2->unkC.x;
    spline[2] = arg2->unk18.x;
    spline[3] = arg2->unk24.x;
    arg0->srt.transl.x = curvesHermite(spline, *tValue, NULL);
    spline[0] = arg2->unk0.y;
    spline[1] = arg2->unkC.y;
    spline[2] = arg2->unk18.y;
    spline[3] = arg2->unk24.y;
    arg0->srt.transl.y = curvesHermite(spline, *tValue, NULL);
    spline[0] = arg2->unk0.z;
    spline[1] = arg2->unkC.z;
    spline[2] = arg2->unk18.z;
    spline[3] = arg2->unk24.z;
    arg0->srt.transl.z = curvesHermite(spline, *tValue, NULL);
    return sp3C;
}

// offset: 0x1A1C | func: 14
static f32 movelib_func_1A1C(DLL53Func17F4Arg2* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, s32 arg4) {
    f32 var_fs2;
    f32 var_fs5;
    s32 var_s1;
    f32 tValue;
    f32 spA4;
    f32 spA0;
    f32 temp_fs1;
    f32 temp_fs2;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 temp_fv0_3;
    f32 spline[4];
    f32 temp_fv1;
    
    var_fs2 = arg0->unk0.x;
    spA4 = arg0->unk0.y;
    spA0 = arg0->unk0.z;
    var_fs5 = 0.0f;
    for (var_s1 = 1; var_s1 < arg4 + 1; var_s1++) {
        tValue = (f32) var_s1 / (f32) arg4;
        
        spline[0] = arg0->unk0.x;
        spline[1] = arg2->x;
        spline[2] = arg1->x;
        spline[3] = arg3->x;
        temp_fv0 = curvesHermite(spline, tValue, NULL);
        temp_fs1 = temp_fv0 - var_fs2;
        
        spline[0] = arg0->unk0.y;
        spline[1] = arg2->y;
        spline[2] = arg1->y;
        spline[3] = arg3->y;
        temp_fv0_2 = curvesHermite(spline, tValue, NULL);
        temp_fs2 = temp_fv0_2 - spA4;
        
        spline[0] = arg0->unk0.z;
        spline[1] = arg2->z;
        spline[2] = arg1->z;
        spline[3] = arg3->z;
        temp_fv0_3 = curvesHermite(spline, tValue, NULL);
        temp_fv1 = temp_fv0_3 - spA0;
        
        var_fs5 += sqrtf(SQ(temp_fs1) + SQ(temp_fs2) + SQ(temp_fv1));
        var_fs2 = temp_fv0;
        spA4 = temp_fv0_2;
        spA0 = temp_fv0_3;
    }
    return var_fs5;
}

// offset: 0x1C0C | func: 15 | export: 7
s32 movelib_func_1C0C(s32 arg0, SRT* arg1) {
    CurveSetup* temp_v0_2;
    s32 temp_v0;

    if (arg0 >= 28) {
        STUBBED_PRINTF("WARNING: Max CreatePoint is Exceeded ");
        return 0;
    }
    temp_v0 = gDLL_26_Curves->vtbl->func_218C(arg0);
    if (temp_v0 >= 0) {
        temp_v0_2 = gDLL_26_Curves->vtbl->func_39C(temp_v0);
        arg1->transl.x = temp_v0_2->pos.x;
        arg1->transl.y = temp_v0_2->pos.y;
        arg1->transl.z = temp_v0_2->pos.z;
        arg1->yaw = temp_v0_2->unk2C << 8;
        return 1;
    }

    STUBBED_PRINTF("WARNING: Error Could not find createpoitn node ");
    return 0;
}

// offset: 0x1CC8 | func: 16 | export: 9
s32 movelib_func_1CC8(s32 arg0, SRT* arg1) {
    Object* temp_v0_3;
    CurveSetup* sp30;
    f32 sp2C;
    s32 temp_v0;
    f32 xDist, zDist;

    sp2C = 1000.0f;
    temp_v0 = gDLL_26_Curves->vtbl->func_218C(arg0);
    if (temp_v0 >= 0) {
        sp30 = gDLL_26_Curves->vtbl->func_39C(temp_v0);
        arg1->transl.x = sp30->pos.x;
        arg1->transl.y = sp30->pos.y;
        arg1->transl.z = sp30->pos.z;
        temp_v0_3 = objGetNearestType(OBJTYPE_LookAt, &arg1->transl, &sp2C);
        if (temp_v0_3 != NULL) {
            xDist = temp_v0_3->srt.transl.x - arg1->transl.x;
            zDist = temp_v0_3->srt.transl.z - arg1->transl.z;
            arg1->yaw = (s16) (mathAtan2(xDist, zDist) + 0x8000);
        } else {
            arg1->yaw = (s16) (sp30->unk2C << 8);
        }
        return 1;
    }
    
    STUBBED_PRINTF(" Error Could not find node ");
    return 0;
}

// offset: 0x1DEC | func: 17 | export: 8
f32 movelib_func_1DEC(Object* arg0, s32 arg1) {
    s32 temp_v0 = gDLL_26_Curves->vtbl->func_218C(arg1);
    if (temp_v0 >= 0) {
        return gDLL_26_Curves->vtbl->func_174C(arg0, temp_v0);
    }

    STUBBED_PRINTF("CURVEPOINT :Error Could not find node ");
    return -1.0f;
}

// offset: 0x1E70 | func: 18 | export: 6
void movelib_func_1E70(MoveLibData* arg0, s16* arg1, s16* arg2, s32 arg3) {
    if (arg1 == NULL) {
        arg1 = data_0;
    }
    if (arg2 == NULL) {
        arg2 = data_0;
    }
    bcopy(arg1, &arg0->unk454, arg0->jointCount * sizeof(s16));
    bcopy(arg2, &arg0->unk454[15], arg0->jointCount * sizeof(s16));
}

// offset: 0x1F1C | func: 19 | export: 12
s32 movelib_get_vtx_world_pos(Object* obj, s32 vtx, Vec3f* outPos) {
    ModelInstance* modelInst;
    Model* model;
    Vtx* verts;
    s32 faceIdx;
    MtxF* mtx;
    ModelFacebatch* face;
    s8 *joint;

    modelInst = obj->modelInsts[obj->modelInstIdx];
    model = modelInst->model;
    if ((obj->objhitInfo != NULL) && ((obj->objhitInfo->unk9F != 0) || (obj->objhitInfo->unk9E != 0))) {
        return 0;
    }
    if (vtx >= model->vertexCount) {
        return 0;
    }
    // @bug: should be: !(temp_t1->unk34 & 8)
    if (!modelInst->unk34 & 8) {
        return 0;
    }
    verts = modelInst->vertices[(modelInst->unk34 >> 1) & 1];
    faceIdx = 0;
    while (faceIdx < model->unk70 && vtx >= model->faces[faceIdx + 1].baseVertexID) {
        faceIdx += 1;
    }
    if (faceIdx == model->unk70) {
        return 0;
    }
    face = &model->faces[faceIdx];
    joint = &face->jointID_A;
    outPos->x = (f32) verts[vtx].v.ob[0];
    outPos->y = (f32) verts[vtx].v.ob[1];
    outPos->z = (f32) verts[vtx].v.ob[2];
    mathMtxXFMF((MtxF*)(((f32*)modelInst->matrices[modelInst->unk34 & 1]) + (*joint << 4)),
        outPos->x, 
        outPos->y, 
        outPos->z, 
        &outPos->x, 
        &outPos->y, 
        &outPos->z);
    outPos->x += gWorldX;
    outPos->z += gWorldZ;
    return model->vertexCount;
}
