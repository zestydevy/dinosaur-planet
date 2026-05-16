#include "common.h"
#include "dlls/engine/53.h"
#include "sys/objlib.h"
#include "sys/objexpr.h"
#include "sys/objtype.h"

// TODO: move to header
typedef struct {
    f32 unk0;
    u8 _unk4[0x10 - 0x4];
    Vec3f unk10;
    HeadAnimation unk1C;
    u8 _unk40[0x454 - 0x40];
    s16 unk454[25]; // at least 25
    u8 _unk486[0x490 - 0x486];
    s32 unk490;
    s32 unk494;
    u8 unk498;
    u8 unk499;
    u8 _unk49A[0x49C - 0x49A];
    Object* unk49C;
    Object* unk4A0;
    s16 unk4A4;
    s16 unk4A6;
    u8 unk4A8;
    u8 unk4A9;
    u8 _unk4AA[0x4AC - 0x4AA];
    s32 unk4AC;
    s32 unk4B0;
    s32 unk4B4;
} MoveLibData;

/*0x0*/ static u32 data_0[] = {
    0x00230023, 0x00230023, 0x00230023, 0x00230023, 0x00230023, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_53_ctor(void *dll) {
    STUBBED_PRINTF("\n MOVELIB LOADED \n");
}

// offset: 0xC | dtor
void dll_53_dtor(void *dll) {
    STUBBED_PRINTF("\n MOVELIB FREEDED \n");
}

// offset: 0x18 | func: 0
static s32 dll_53_func_18(Object* arg0, Object* arg1, s32* arg2, MoveLibData* arg3, f32* arg4, s16* arg5, Vec3f* arg6) {
    s16 sp3E;
    s16 sp3C;
    f32 sp38;
    s32* sp34;
    HeadAnimation* var_a3;

    sp34 = func_800349B0();
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
    sp3E = func_80031DD8(arg0, arg1, NULL);
    sp3C = func_800334A4(arg0, arg1, &arg3->unk10, 
                         (arg3->unk4A9 & 8) ? NULL : &arg3->unk1C, 
                         arg3->unk454, sp38, 8, arg3->unk4A4);
    if (!(arg3->unk4A9 & 8)) {
        arg3->unk490 = !func_800333C8(arg0, sp34, arg3->unk4A8, &arg3->unk1C);
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
            func_80023D30(arg0, arg5[1], 0.0f, 0);
            func_80024D74(arg0, 0x1E);
        }
        if (sp3E < 0 && arg0->curModAnimId != arg5[0]) {
            func_80023D30(arg0, arg5[0], 0.0f, 0);
            func_80024D74(arg0, 0x1E);
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
void dll_53_func_3B4(Object* arg0, Object* arg1, MoveLibData* arg2, s16* arg3) {
    s32* temp_a1 = func_800349B0();
    switch (arg2->unk498) {
    case 3:
        func_80033224(arg0, temp_a1, arg2->unk4A8, &arg2->unk1C);
        arg2->unk490 = 0;
        arg2->unk498 = 2;
        /* fallthrough */
    case 2:
        if (dll_53_func_18(arg0, arg1, &arg2->unk494, arg2, &arg2->unk0, arg3, &arg2->unk10) == 0) {
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
    arg2->unk49C = arg1;
}

// offset: 0x4B8 | func: 2 | export: 0
void dll_53_func_4B8(Object* arg0, MoveLibData* arg1) {
    f32 sp64;
    f32 sp60;
    f32 temp_fv1;
    s16 sp5A;
    s32* sp54;
    Object* sp50;
    f32 sp44[3];

    sp64 = 1000.0f;
    sp60 = 30.0f;
    sp5A = 0;
    sp54 = func_800349B0();
    get_player();
    if (arg1->unk499 == 0) {
        if ((arg1->unk4A9 & 1) && (arg1->unk498 != 8)) {
            arg1->unk498 = 8;
            if (!(arg1->unk4A9 & 8)) {
                func_80033224(arg0, sp54, arg1->unk4A8, &arg1->unk1C);
                arg1->unk490 = 0x50;
                func_80033350(&arg1->unk1C, arg1->unk4A8, 0, 0);
            } else {
                func_800332A4(arg0, func_800349B0(), arg1->unk4A8);
            }
        } else if (!(arg1->unk4A9 & 1) && (arg1->unk498 == 8)) {
            arg1->unk498 = 0;
            if (!(arg1->unk4A9 & 8)) {
                func_80033224(arg0, sp54, arg1->unk4A8, &arg1->unk1C);
                arg1->unk490 = 0x50;
            }
        }
        if (arg1->unk498 > 1) {
            if ((arg1->unk490 != 0) && !(arg1->unk4A9 & 8)) {
                arg1->unk490 = !func_800333C8(arg0, sp54, arg1->unk4A8, &arg1->unk1C);
                return;
            }
            func_800332A4(arg0, func_800349B0(), arg1->unk4A8);
            return;
        }
        if (arg1->unk4A0 == NULL) {
            sp50 = obj_get_nearest_type_to(OBJTYPE_9, arg0, &sp64);
        } else {
            sp50 = arg1->unk4A0;
        }
        if (NULL != sp50) {
            if (arg1->unk4A9 & 0x20) {
                sp44[0] = arg1->unk10.x - sp50->srt.transl.x;
                sp44[1] = arg1->unk10.y - sp50->srt.transl.y;
                sp44[2] = arg1->unk10.z - sp50->srt.transl.z;
                temp_fv1 = sqrtf(SQ(sp44[0]) + SQ(sp44[2]));
                if (temp_fv1 <= 40.0f) {
                    temp_fv1 = (temp_fv1 - 10.0f) / 30.0f;
                    temp_fv1 = temp_fv1 < 0.0f
                        ? 0.0f
                        : temp_fv1 > 1.0f
                            ? 1.0f
                            : temp_fv1;
                    temp_fv1 = 1.0f - temp_fv1;
                    arg1->unk10.x = (arg1->unk10.x * (1.0f - temp_fv1)) + (arg0->srt.transl.x * temp_fv1);
                    arg1->unk10.z = (arg1->unk10.z * (1.0f - temp_fv1)) + (arg0->srt.transl.z * temp_fv1);
                }
            }
            if ((arg1->unk4AC != -1) && (sp50 == arg1->unk49C)) {
                if (((arg1->unk4B4 -= gUpdateRate) <= 0) && ((arg1->unk4B4 + gUpdateRate) > 0)) {
                    func_80033224(arg0, sp54, arg1->unk4A8, &arg1->unk1C);
                    arg1->unk490 = 0x50;
                    func_80033350(&arg1->unk1C, arg1->unk4A8, 0, 0);
                    arg1->unk498 = 0;
                    return;
                }
                if (arg1->unk490 != 0) {
                    arg1->unk490 = !func_800333C8(arg0, sp54, arg1->unk4A8, &arg1->unk1C);
                }
                if (arg1->unk4B4 < -arg1->unk4B0) {
                    arg1->unk4B4 = rand_next(arg1->unk4B0, arg1->unk4AC);
                }
                if (arg1->unk4B4 < 0) {
                    return;
                }
            } else {
                arg1->unk4B4 = arg1->unk4AC;
            }
            if ((sp50 != arg1->unk49C) && (sp50 != NULL)) {
                if (sp50->objhitInfo != NULL) {
                    if (sp50->objhitInfo->unk5A & 2) {
                        sp60 = (f32) sp50->objhitInfo->unk56 * 4.0f;
                    } else if (sp50->objhitInfo->unk5A & 1) {
                        sp60 = (f32) sp50->objhitInfo->unk52;
                    } else {
                        sp60 = 30.0f;
                    }
                } else {
                    sp60 = 30.0f;
                }
            }
            if (sp50 != NULL) {
                sp5A = func_80031DD8(arg0, sp50, NULL);
            }
            if (arg1->unk4A9 & 0x10) {
                func_80034D94(0, 1);
                sp5A -= 0x8000;
            }
            if ((((sp5A >= 0) ? sp5A : -sp5A) >= 0x5555) || (sp50 == NULL)) {
                if ((arg1->unk498 != 0) || ((sp50 == NULL) && (arg1->unk49C != NULL))) {
                    func_80033224(arg0, sp54, arg1->unk4A8, &arg1->unk1C);
                    arg1->unk490 = 0xA;
                    func_80033350(&arg1->unk1C, arg1->unk4A8, 0, 0);
                    arg1->unk498 = 0;
                }
            } else {
                if ((sp50 != arg1->unk49C) || (arg1->unk498 == 0)) {
                    func_80033224(arg0, sp54, arg1->unk4A8, &arg1->unk1C);
                    arg1->unk490 = 1;
                }
                if (arg1->unk4A9 & 8) {
                    arg1->unk490 = 0;
                }
                func_800334A4(arg0, sp50, &arg1->unk10, 
                              arg1->unk490 != 0 ? &arg1->unk1C : NULL, 
                              arg1->unk454, sp60, 8, arg1->unk4A4);
                arg1->unk498 = 1;
            }
            arg1->unk49C = sp50;
            arg1->unk4A0 = NULL;
            if (!(arg1->unk4A9 & 8) && (arg1->unk490 != 0)) {
                arg1->unk490 = !func_800333C8(arg0, sp54, arg1->unk4A8, &arg1->unk1C);
            }
        }
    }
}

// offset: 0xB84 | func: 3 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_B84.s")

// offset: 0xB90 | func: 4 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_B90.s")

// offset: 0xBA4 | func: 5 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_BA4.s")

// offset: 0xCDC | func: 6 | export: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_CDC.s")

// offset: 0xE9C | func: 7 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_E9C.s")

// offset: 0x106C | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_106C.s")

// offset: 0x1130 | func: 9 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_1130.s")

// offset: 0x14F4 | func: 10 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_14F4.s")

// offset: 0x16B0 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_16B0.s")

// offset: 0x173C | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_173C.s")

// offset: 0x17F4 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_17F4.s")

// offset: 0x1A1C | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_1A1C.s")

// offset: 0x1C0C | func: 15 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_1C0C.s")

// offset: 0x1CC8 | func: 16 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_1CC8.s")

// offset: 0x1DEC | func: 17 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_1DEC.s")

// offset: 0x1E70 | func: 18 | export: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_1E70.s")

// offset: 0x1F1C | func: 19 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_1F1C.s")

/*0x38*/ static const char str_38[] = "Got Curve ";
/*0x44*/ static const char str_44[] = " PathId %i Loc id %i \n";
/*0x5C*/ static const char str_5C[] = " Tangent 1 %f %f %f \n";
/*0x74*/ static const char str_74[] = " Tangent 2 %f %f %f \n";
/*0x8C*/ static const char str_8C[] = "t value %f ";
/*0x98*/ static const char str_98[] = "WARNING: Max CreatePoint is Exceeded ";
/*0xC0*/ static const char str_C0[] = "WARNING: Error Could not find createpoitn node ";
/*0xF0*/ static const char str_F0[] = " Error Could not find node ";
/*0x10C*/ static const char str_10C[] = "CURVEPOINT :Error Could not find node ";
