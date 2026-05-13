#include "common.h"
#include "dlls/engine/53.h"
#include "sys/objlib.h"
#include "sys/objexpr.h"

// TODO: move to header
typedef struct {
    u8 _unk0[0x10 - 0x0];
    Vec3f unk10;
    HeadAnimation unk1C;
    u8 _unk40[0x454 - 0x40];
    s16 unk454[25]; // at least 25
    u8 _unk486[0x490 - 0x486];
    s32 unk490;
    u8 _unk494[0x4A4 - 0x494];
    s16 unk4A4;
    s16 unk4A6;
    u8 unk4A8;
    u8 unk4A9;
    u8 _unk4AA[0x4B8 - 0x4AA];
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
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_18.s")
#else
// regalloc
s32 dll_53_func_18(Object* arg0, Object* arg1, s32* arg2, MoveLibData* arg3, f32* arg4, s16* arg5) {
    s16 sp3E;
    s16 sp3C;
    f32 sp38;
    s32* sp34;
    HeadAnimation* var_a3;
    s32 var_a3_2;

    sp34 = func_800349B0();
    if (arg1->objhitInfo != NULL) {
        if (arg1->objhitInfo->unk5A & 2) {
            sp38 = (f32) arg1->objhitInfo->unk56 * 4.0f;
        } else if (arg1->objhitInfo->unk5A & 1) {
            sp38 = (f32) arg1->objhitInfo->unk52;
        } else {
            sp38 = 30.0f;
        }
    } else {
        sp38 = 30.0f;
    }
    sp3E = func_80031DD8(arg0, arg1, NULL);
    if (arg3->unk4A9 & 8) {
        var_a3 = NULL;
    } else {
        var_a3 = &arg3->unk1C;
    }
    sp3C = func_800334A4(arg0, arg1, &arg3->unk10, var_a3, arg3->unk454, sp38, 8, (s16) (s32) arg3->unk4A4);
    if (!(arg3->unk4A9 & 8)) {
        arg3->unk490 = (s32) (func_800333C8(arg0, sp34, (s32) arg3->unk4A8, &arg3->unk1C) == 0);
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
            return sp3C == 0;
        }
    }
    if ((*arg2 == 0) && (sp3C != 0)) {
        *arg2 = 1;
        *arg4 = 0.005f;
    } else if (*arg2 != 0) {
        if (sp3E > 0) {
            if (arg0->curModAnimId != arg5[1]) {
                func_80023D30(arg0, arg5[1], 0.0f, 0U);
                func_80024D74(arg0, 0x1E);
            }
        }
        if (sp3E < 0) {
            if (arg0->curModAnimId != arg5[0]) {
                func_80023D30(arg0, arg5[0], 0.0f, 0U);
                func_80024D74(arg0, 0x1E);
            }
        }
        if (sp3C == 0) {
            if (sp3E > 0) {
                sp3E /= 20;
            } else {
                sp3E /= 20;
            }
        } else if (sp3E > 0) {
            sp3E = (sp3E - 0x500) / 20;
        } else {
            sp3E = (sp3E + 0x500) / 20;
        }
        arg0->srt.yaw += sp3E;
        if (sp3E >= 0) {
            var_a3_2 = sp3E;
        } else {
            var_a3_2 = -sp3E;
        }
        *arg4 = (f32) var_a3_2 / 20922.25f;
    }
    return 1;
}
#endif

// offset: 0x3B4 | func: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_3B4.s")

// offset: 0x4B8 | func: 2 | export: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/53_movelib/dll_53_func_4B8.s")

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

/*0x28*/ static const char str_28[] = " In turn Range ";
/*0x38*/ static const char str_38[] = "Got Curve ";
/*0x44*/ static const char str_44[] = " PathId %i Loc id %i \n";
/*0x5C*/ static const char str_5C[] = " Tangent 1 %f %f %f \n";
/*0x74*/ static const char str_74[] = " Tangent 2 %f %f %f \n";
/*0x8C*/ static const char str_8C[] = "t value %f ";
/*0x98*/ static const char str_98[] = "WARNING: Max CreatePoint is Exceeded ";
/*0xC0*/ static const char str_C0[] = "WARNING: Error Could not find createpoitn node ";
/*0xF0*/ static const char str_F0[] = " Error Could not find node ";
/*0x10C*/ static const char str_10C[] = "CURVEPOINT :Error Could not find node ";
