#include "common.h"
#include "sys/objtype.h"
#include "sys/objlib.h"
#include "dlls/objects/278_flameblast.h"
#include "dlls/objects/332_FXEmit.h"

typedef struct {
    s16 unk0;
    u16 pad2;
    s32 unk4;
    u32 unk8_31: 1;
    u32 pad8_0: 31;
} DLL212_3FF4;

typedef struct {
    u8 pad0[0x8];
    s32 unk8;
    u8 padC[0x24-0xC];
    s16 unk24;
    u16 pad26;
    s32 unk28;
    u16 unk2C;
} Kyte_Unk3;

typedef struct {
    HeadAnimation unk0;
    u8 pad24[0x48 - 0x24];
    u8 unk48;
    u8 unk49;
} Kyte_Unk;

typedef struct {
    u32 pad0;
    u32 pad4;
    f32 unk8;
    u32 padC;
    s32* unk10;
    s32 unk14;
} Kyte_Unk2;

typedef struct {
    void *unk0; // loaded dll
    Object *unk4[3];
    Object *unk10;
    s32 unk14;
    Vec3f unk18;
    Vec3f unk24;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 pad44[0x1D8 - 0x44];
    DLL212_3FF4 unk1D8;
    u8 pad1E4[0x238 - 0x1E4];
    s32 unk238;
    u8 pad23C[0x278 - 0x23C];
} DLL212_Data;

/*0x0*/ static u32 data_0[] = {
    0x0000000d, 0x00000005, 0xffffffff, 0x0000000b, 0x0000030a, 0x3f800000, 0x3f800000, 0x3f800000, 
    0x3f800000, 0x3f800000
};
/*0x28*/ static u32 data_28[] = {
    0x00000006, 0x00000013, 0x00000012, 0x00000015, 0xffffffff, 0x00000016, 0x00000018, 0x3f800000, 
    0x3f800000, 0x3f800000, 0x3f800000
};
/*0x54*/ static u32 data_54[] = {
    0x00000000, 0x00000009, 0x00000008, 0x00000004, 0x00000002, 0xffffffff, 0x3f800000, 0x3f800000, 
    0x3f800000, 0x3f800000, 0x00000000, 0x00000009, 0x00000008, 0x00000004, 0x00000002, 0xffffffff
};
/*0x94*/ static u32 data_94[] = {
    0x3fc00000, 0x40200000, 0x41a00000, 0x3e19999a
};
/*0xA4*/ static u32 data_A4[] = {
    (u32)&data_0, 0x00000002, 0x3edc28f6, 0x3f99999a, 0x40000000, 0x3e4ccccd
};
/*0xBC*/ static u32 data_BC[] = {
    (u32)&data_28, 0x00000003, 0x3dcccccd, 0x3ecccccd, 0x3ef0a3d7, 0x3e4ccccd
};
/*0xD4*/ static u32 data_D4[] = {
    (u32)&data_54, 0x00000003, 0x00000002, 0x00000000, 0x00000001
};
/*0xE8*/ static u32 data_E8 = 0x00000004;
/*0xEC*/ static u32 data_EC[] = {
    0x00000008, 0x00000002, 0x00000004, 0x0000c254, 0x00000292, 0x0000d144, 0x004148c4, 0x00404282, 
    0x0020c444, 0x08208482, 0x00000004, 0x00000002, 0x00094248, 0x00024248, 0x00044248, 0x00814008, 
    0x00104048, 0x0000d148, 0x0000c148, 0x0000c144, 0x0400c148, 0x08208444, 0x18200482
};
/*0x148*/ static s16 data_148[] = { -1, -1, 0x08bb, -1, -1, -1 };
/*0x154*/ static u32 data_154[] = {
    0x00000000, 0x00000002
};
/*0x15C*/ static u32 data_15C[] = {
    0x00000000, 0x00000000, 0x3f800000
};
/*0x168*/ static u32 data_168 = 0xffffffff;
/*0x16C*/ static u32 data_16C[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static u8 bss_0[0x10];

static void Kyte_func_4020(DLL212_3FF4* arg0, s32 arg1);
static void Kyte_func_3F44(Object *self, DLL212_Data* objdata);
static s32 Kyte_func_3A2C(Object* self, DLL212_Data* objdata);

// offset: 0x0 | ctor
void Kyte_ctor(void* dll) { }

// offset: 0xC | dtor
void Kyte_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void Kyte_obj_Setup(Object* self, ObjSetup* setup, s32 reset);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_obj_Setup.s")

// offset: 0x120 | func: 1 | export: 1
void Kyte_obj_Control(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_obj_Control.s")

// offset: 0x6B8 | func: 2 | export: 2
void Kyte_obj_Update(Object* self) { }

// offset: 0x6C4 | func: 3 | export: 3
void Kyte_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_obj_Print.s")

// offset: 0x868 | func: 4 | export: 4
void Kyte_obj_Free(Object* self, s32 onlySelf);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_obj_Free.s")

// offset: 0x8C4 | func: 5 | export: 5
u32 Kyte_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x8D4 | func: 6 | export: 6
u32 Kyte_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DLL212_Data);
}

// offset: 0x8E8 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_8E8.s")

// offset: 0xA94 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_A94.s")

// offset: 0xAB4 | func: 9 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_AB4.s")

// offset: 0xAC4 | func: 10 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_AC4.s")

// offset: 0xADC | func: 11 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_ADC.s")

// offset: 0xAEC | func: 12 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_AEC.s")

// offset: 0xB04 | func: 13 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_B04.s")

// offset: 0xB24 | func: 14 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_B24.s")

// offset: 0xB34 | func: 15 | export: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_B34.s")

// offset: 0xB94 | func: 16 | export: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_B94.s")

// offset: 0xC04 | func: 17 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_C04.s")

// offset: 0xC18 | func: 18 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_C18.s")

// offset: 0xC2C | func: 19 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_C2C.s")

// offset: 0xC48 | func: 20 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_C48.s")

// offset: 0xC5C | func: 21 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_C5C.s")

// offset: 0xC74 | func: 22 | export: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_C74.s")

// offset: 0xC8C | func: 23 | export: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_C8C.s")

// offset: 0xC9C | func: 24 | export: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_C9C.s")

// offset: 0xCAC | func: 25 | export: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_CAC.s")

// offset: 0xCCC | func: 26 | export: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_CCC.s")

// offset: 0xCDC | func: 27 | export: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_CDC.s")

// offset: 0xCE8 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_CE8.s")

// offset: 0xF08 | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_F08.s")

// offset: 0x1134 | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_1134.s")

// offset: 0x1404 | func: 31
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_1404.s")

// offset: 0x1730 | func: 32
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_1730.s")

// offset: 0x1864 | func: 33
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_1864.s")

// offset: 0x18F8 | func: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_18F8.s")

// offset: 0x1D2C | func: 35
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_1D2C.s")

// offset: 0x1F14 | func: 36
static Object* Kyte_func_1F14(Object* self, s32 arg1) {
    Object* temp_a0;
    Object* var_s4;
    s32 sp4C;
    Object** var_s0;
    f32 temp_fv0;
    f32 var_fs0;
    s32 i;

    var_s4 = NULL;
    var_fs0 = M_INFINITY_F;
    var_s0 = objGetAllOfType(OBJTYPE_KyteTarget, &sp4C);
    for (i = 0; i < sp4C; i++) {
        temp_a0 = var_s0[i];
        if ((((DLL_Unknown*)temp_a0->dll)->vtbl)->func[10].withOneArgS32(temp_a0) & arg1) {
            temp_fv0 = vec3DistanceSquared(&self->globalPosition, &var_s0[i]->globalPosition);
            if (temp_fv0 < var_fs0) {
                var_s4 = var_s0[i];
                var_fs0 = temp_fv0;
            }
        }
    }
    return var_s4;
}

// offset: 0x200C | func: 37
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_200C.s")

// offset: 0x20A4 | func: 38
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/Kyte_func_20A4.s")

// offset: 0x27D8 | func: 39
s32 Kyte_func_27D8(Object* arg0, Vec3f* arg1, f32 arg2, Kyte_Unk2* arg3, s32* arg4, f32* arg5, Kyte_Unk3* arg6) {
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    f32 var_fa1;
    s16 sp32;
    s16 temp_v1;
    s32 var_a1;
    f32 sp28;
    f32 var_fv1;
    s32 sp20;

    sp20 = -1;
    sp40 = arg1->x - arg0->srt.transl.x;
    sp38 = arg1->z - arg0->srt.transl.z;
    sp3C = arg1->y - arg0->srt.transl.y;
    sp32 = arg0->srt.yaw;
    sp44 = sqrtf(SQ(sp40) + SQ(sp38));
    arg0->srt.yaw = mathAtan2f(sp40, sp38) + 0x8000;
    arg0->srt.pitch = mathAtan2f(sp3C, sp44);
    temp_v1 = arg0->srt.yaw - (sp32 & 0xFFFF);
    CIRCLE_WRAP(temp_v1);
    arg0->srt.roll = (arg0->srt.roll + (s32) (temp_v1 * 16.0f)) / 2;
    if (arg0->srt.roll > 0x3000) {
        arg0->srt.roll = 0x3000;
    }
    if (arg0->srt.roll < -0x3000) {
        arg0->srt.roll = -0x3000;
    }
    if (arg2 == 0.0f) {
        arg2 = 0.0001f;
    }
    var_fv1 = (sp3C * 0.01f) + (0.0005f * sp44) + 0.03f;
    if (var_fv1 < 0.0f) {
        var_fv1 = 0/*.0f*/;
    }
    if (var_fv1 > 0.5f) {
        var_fv1 = 0.5f;
    }
    if (*arg4 != -1) {
        var_a1 = *arg4;
        sp28 = sp28;
        var_fv1 = (arg0->animProgress * 0.01f) + (var_fv1 * (1.0f - arg0->animProgress));
    } else {
        var_fa1 = 0;
        var_a1 = arg3->unk10[(s32)var_fa1];
        if ((s32)var_fa1 < (arg3->unk14 - 1)) {
            sp20 = arg3->unk10[(s32) (var_fa1 + 1.0f)];
        }
        sp28 = ((sp3C / arg2) + 1.0f) * 0.5f;
        if (sp28 < 0/*.0f*/) {
            sp28 = 0/*.0f*/;
        }
        if (sp28 > 1.0f) {
            sp28 = 1.0f;
        }
        sp28 *= 1023.0f;
    }
    *arg5 = var_fv1;
    if (var_a1 != arg0->curModAnimId) {
        sp28 = sp28;
        objAnimSet(arg0, var_a1, 0/*.0f*/, 0U);
    }
    if (sp20 != -1) {
        objAnimSetBlend(arg0, sp20, sp28);
    }
    arg6->unk2C &= ~8;
    return 0;
}

// offset: 0x2B58 | func: 40
s32 Kyte_func_2B58(Object* arg0, Vec3f* arg1, f32 arg2, Kyte_Unk2* arg3, s32* arg4, f32* arg5, Kyte_Unk3* arg6) {
    f32 var_ft4;
    f32 sp30;
    f32 sp2C;
    f32 sp28;
    f32 var_fv1;
    f32 var_fv0;
    s16 temp_v1;
    s32 var_a1;

    var_ft4 = 0;
    if (arg0->srt.roll != 0) {
        arg0->srt.roll >>= 1;
    }
    if (arg0->srt.pitch != 0) {
        arg0->srt.pitch >>= 1;
    }
    sp30 = arg0->srt.transl.x - arg1->x;
    sp28 = arg0->srt.transl.z - arg1->z;
    sp2C = arg0->srt.transl.y - arg1->y;
    if (!(arg6->unk2C & 4)) {
        var_ft4 = sqrtf(SQ(sp30) + SQ(sp28));
        temp_v1 = mathAtan2f(sp30, sp28) - (arg0->srt.yaw & 0xFFFF);
        CIRCLE_WRAP(temp_v1);
        arg0->srt.yaw += (temp_v1 >> 2);
    }
    // FAKE
    if (sp30 != 0.0f);
    var_fv1 = (sp2C * 0.0005f) + (0.005f * var_ft4) + 0.01f;
    if (var_fv1 < 0.0f){
        var_fv1 = 0/*.0f*/;
    }
    if (var_fv1 > 0.5f) {
        var_fv1 = 0.5f;
    }
    if (*arg4 != -1) {
        var_a1 = *arg4;
        var_fv1 = (arg0->animProgress * 0.01f) + (var_fv1 * (1.0f - arg0->animProgress));
    } else {
        if (var_ft4 > 0/*.0f*/) {
            var_fv0 = 1/*.0f*/;
        } else {
            var_fv0 = 0/*.0f*/;
        }
        var_a1 = arg3->unk10[(s32) var_fv0];
    }
    if (var_a1 != arg0->curModAnimId) {
        objAnimSet(arg0, var_a1, 0/*.0f*/, 0U);
    }
    *arg5 = var_fv1;
    arg6->unk2C &= ~8;
    return 0;
}

// offset: 0x2DA4 | func: 41
s32 Kyte_func_2DA4(Object* arg0, Vec3f* arg1, f32 arg2, Kyte_Unk2* arg3, s32* arg4, f32* arg5, Kyte_Unk3* arg6) {
    f32 temp_fv1;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    s32 temp_v0_4;
    s32 var_a1;
    s32 var_v1;
    f32 var_fa0;

    sp38 = 0.0f;
    sp40 = arg0->srt.transl.x - arg1->x;
    sp3C = arg0->srt.transl.z - arg1->z;
    if (arg0->srt.roll != 0) {
        arg0->srt.roll >>= 1;
    }
    if (arg0->srt.pitch != 0) {
        arg0->srt.pitch >>= 1;
    }
    if (!(arg6->unk2C & 4)) {
        arg0->srt.yaw = mathAtan2f(sp40, sp3C);
    }
    if (*arg4 != -1) {
        var_a1 = *arg4;
        sp38 = 0.02f;
        arg6->unk2C |= 8;
    } else {
        if (arg6->unk2C & 2) {
            arg6->unk2C |= 8;
            temp_v0_4 = arg0->srt.yaw - arg6->unk24;
            if (temp_v0_4 >= 0) {
                var_v1 = temp_v0_4;
            } else {
                var_v1 = -temp_v0_4;
            }
            sp38 = var_v1 * 0.00005f;
            temp_v0_4 = arg0->srt.yaw - arg6->unk24;
            if (temp_v0_4 >= 0) {
                var_v1 = temp_v0_4;
            } else {
                var_v1 = -temp_v0_4;
            }
            if (var_v1 > 0x100) {
                var_a1 = arg3->unk10[1];
            } else {
                var_a1 = arg3->unk10[0];
            }
            sp38 += 0.005f;
        } else {
            arg6->unk2C |= 8;
            temp_fv1 = sqrtf((sp40 * sp40) + (sp3C * sp3C)) * gUpdateRateInverseF;
            var_fa0 = (((temp_fv1 / arg3->unk8) * (f32) (arg3->unk14 - 1)) + 1.0f);
            var_a1 = arg3->unk10[(s32)var_fa0];
            objGetAnimChange(arg0, temp_fv1, &sp38);
        }
    }
    if (var_a1 != arg0->curModAnimId) {
        objAnimSet(arg0, var_a1, 0.0f, 0U);
    }
    *arg5 = sp38;
    return 0;
}

// offset: 0x300C | func: 42
void Kyte_func_300C(Object* arg0) {
    ObjectShadow* sp2C;
    f32 sp28;

    sp2C = arg0->shadow;
    sp28 = 0.0f;
    sp2C->tr.x = arg0->srt.transl.x;
    sp2C->tr.z = arg0->srt.transl.z;
    if (trackGetHeightFloor(arg0, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, &sp28, 0U) != 0) {
        sp2C->tr.y = arg0->srt.transl.y - sp28;
    }
}

// offset: 0x3098 | func: 43
s32 Kyte_func_3098(Object* arg0, Kyte_Unk* arg1) {
    Object* sp2C;
    s32 var_v0;

    if (!arg1->unk48) {
        objExpr_func_80032CF8(arg0, NULL, &arg1->unk0, 0x78);
        return 0;
    }
    if (!arg1->unk49) {
        sp2C = objGetPlayer();
    } else {
        sp2C = objGetNearestTypeTo(arg1->unk49, arg0, NULL);
    }
    if (arg1->unk48 & 2) {
        arg0->srt.yaw += (objAngleToObjectXZ(arg0, sp2C, NULL) >> 3) & 0xFFFF & 0xFFFF;
    }
    objExpr_func_80032CF8(arg0, sp2C, arg1, 0x78);
    return 1;
}


// offset: 0x319C | func: 44
s32 Kyte_func_319C(Object* arg0, Kyte_Unk3* arg1, Kyte_Unk* arg2, u32 arg3, DLL212_Data* arg4) {
    s32 sp4C;
    s32 var_v0;
    s32 var_v1;
    s32 sp34[4] = { 0, 48, 4, 50 };
    Object* sp30;
    SidekickStats* sp2C;

    arg4->unk38 = arg4->unk34;
    sp4C = data_EC[arg3];
    if (arg3 > ARRAYCOUNT(data_EC)) {
        return 0;
    }
    if (sp4C & 2) {
        arg1->unk8 = 0;
    }
    if (sp4C & 4) {
        arg1->unk8 = 1;
    }
    if (sp4C & 8) {
        arg1->unk8 = 2;
    }
    if (sp4C & 0x40) {
        arg2->unk48 |= 2;
        arg1->unk2C |= 4;
    }
    if (sp4C & 0x80) {
        arg2->unk48 |= 1;
    }
    if (sp4C & 0xF00) {
        var_v0 = (sp4C & 0xF00) >> 8;
        var_v1 = 0;
        while (!(var_v0 & 1)) {
            var_v0 >>= 1;
            var_v1++;
        }
        arg2->unk49 = sp34[var_v1];
    }
    if (sp4C & 0x4000) {
        arg1->unk2C |= 0x10;
    }
    if (arg4->unk3C == -1) {
        if (sp4C & 0x8000) {
            arg4->unk3C = 0x1F4;
        }
        if (sp4C & 0x10000) {
            arg4->unk3C = 0xC8;
        }
        if (sp4C & 0x10000000) {
            arg4->unk3C = 0x514;
        }
    }
    if (sp4C & 0x400000) {
        sp30 = Kyte_func_1F14(arg0, 8);
        if (sp30 != NULL) {
            sp30 = sp30;
            sp2C = gDLL_29_Gplay->vtbl->get_sidekick_stats();
            sp2C->redFood += (((DLL_Unknown*)sp30->dll)->vtbl)->func[7].withTwoArgsS32(sp30, 2);
        } else {
            sp4C &= ~0x400000;
        }
    }
    if (sp4C & 0x20000) {
        sp30 = Kyte_func_1F14(arg0, 0x10);
        if (sp30 != NULL) {
            (((DLL_Unknown*)sp30->dll)->vtbl)->func[7].withTwoArgsS32(sp30, 6);
            arg1->unk2C |= 0x20;
            arg1->unk28 = 0x100;
        } else {
            sp4C &= ~0x20000;
        }
    }
    if (sp4C & 0x40000) {
        sp30 = Kyte_func_1F14(arg0, 0x20);
        if (sp30 != NULL) {
            (((DLL_Unknown*)sp30->dll)->vtbl)->func[7].withTwoArgsS32(sp30, 7);
            arg1->unk2C |= 0x20;
            arg1->unk28 = 0x100;
        } else {
            sp4C &= ~0x40000;
        }
    }
    if (sp4C & 0x80000) {
        arg1->unk2C |= 0x20;
        arg1->unk28 = 0x100;
        func_80026128(arg0, 0x1A, 1, -1);
        arg0->animProgress = 0.0f;
    }
    if (sp4C & 0x800000) {
        arg1->unk2C |= 0x80;
        sp30 = Kyte_func_1F14(arg0, 4);
        if (sp30 != NULL) {
            arg4->unk3C = (((DLL_Unknown*)sp30->dll)->vtbl)->func[9].withThreeArgsS32(sp30, 2, 0);
        } else {
            arg4->unk3C = 500;
        }
    }
    if (sp4C & 0x04000000) {
        arg1->unk2C |= 0x100;
    }
    arg4->unk34 = sp4C;
    return 1;
}

// offset: 0x35C0 | func: 45
s32 Kyte_func_35C0(Object* arg0, Kyte_Unk3* arg1, Kyte_Unk* arg2, DLL212_Data* arg3) {
    Object* sp3C;
    u8 sp3B;
    Vec3f sp2C;
    s32 sp28;
    DLL212_Data* sp24;

    sp28 = 0;
    sp24 = arg0->data;
    arg0->unkAF |= 8;
    sp3B = mainGetBits(BIT_Kyte_Flight_Talk_Sequence);
    if (arg3->unk3C != -1) {
        arg3->unk3C -= gUpdateRate;
        if (arg3->unk3C < 0) {
            if (!(arg1->unk2C & 2) && (arg3->unk40 != -1U)) {
                Kyte_func_4020(&sp24->unk1D8, sp24->unk238);
                mainSetBits(BIT_Kyte_Flight_Curve, arg3->unk40);
                arg3->unk40 = -1U;
            }
            arg3->unk3C = -1;
            arg1->unk2C = 0U;
            arg2->unk48 = 0;
            sp28 = 1;
        }
    }
    if (sp3B != 0xFF) {
        if ((arg3->unk34 & 0x4000) && (arg1->unk2C & 2)) {
            if (arg3->unk34 & 0x1000) {
                arg0->unkAF &= ~8;
                if ((arg0->unkAF & 1) && sp3B) {
                    joyDisableButtons(0, A_BUTTON);
                    gDLL_3_Animation->vtbl->start_obj_sequence(sp3B, arg0, -1);
                    mainSetBits(BIT_Kyte_Flight_Talk_Sequence, 0xFFU);
                }
            }
            if ((arg3->unk34 & 0x2000) && sp3B) {
                gDLL_3_Animation->vtbl->start_obj_sequence(sp3B, arg0, -1);
                arg3->unk34 &= ~0x2000;
                mainSetBits(BIT_Kyte_Flight_Talk_Sequence, 0xFFU);
            }
        }
    }
    if (arg3->unk14 == 1) {
        arg3->unk30 -= gUpdateRate;
        if (!(arg3->unk34 & 0x10)) {
            arg3->unk30 = -1;
        }
        if (arg3->unk30 < 0) {
            Kyte_func_3F44(arg0, arg3);
            sp3C = Kyte_func_1F14(arg0, 2);
            if (sp3C != NULL) {
                (((DLL_Unknown*)sp3C->dll)->vtbl)->func[7].withTwoArgs(sp3C, 1);
            }
            arg3->unk34 &= ~0x10;
        }
    } else if ((arg3->unk34 & 0x10) && ((arg3->unk34 & 4) || ((arg3->unk34 & 0x12) && (arg3->unk38 & 0x10)))) {
        sp3C = Kyte_func_1F14(arg0, 2);
        if (sp3C != NULL) {
            if (Kyte_func_3A2C(arg0, arg3) != 0) {
                (((DLL_Unknown*)sp3C->dll)->vtbl)->func[7].withTwoArgs(sp3C, 0);
            }
        }
    }
    if (arg3->unk34 & 0x400000) {
        sp3C = objGetNearestTypeTo(OBJTYPE_FireFly, arg0, 0);
        sp2C.f[0] = sp3C->srt.transl.x - arg3->unk18.x;
        sp2C.f[1] = sp3C->srt.transl.y - arg3->unk18.y;
        sp2C.f[2] = sp3C->srt.transl.z - arg3->unk18.z;
        if ((sp3C != NULL) && (sqrtf(SQ(sp2C.f[0]) + SQ(sp2C.f[1]) + SQ(sp2C.f[2])) < 2.0f)) {
            sp3C = Kyte_func_1F14(arg0, 8);
            if (sp3C != NULL) {
                (((DLL_Unknown*)sp3C->dll)->vtbl)->func[7].withTwoArgs(sp3C, 3);
            }
        }
    }
    return sp28;
}

// offset: 0x3A2C | func: 46
static s32 Kyte_func_3A2C(Object* self, DLL212_Data* objdata) {
    FlameBlast_Setup* temp_v0_2;
    FXEmit_Setup* temp_v0_3;
    SidekickStats* temp_v0;
    s32 i;

    gDLL_29_Gplay->vtbl->get_sidekick_stats();
    temp_v0 = gDLL_29_Gplay->vtbl->get_sidekick_stats();
    if ((temp_v0->redFood == 0) || (objdata->unk14 == 1)) {
        return 0;
    }

    temp_v0->redFood -= 1;
    objdata->unk14 = 1;
    objdata->unk30 = 0x1F4; // maybe: BIT_Used_Tricky_Cell_Key
    objdata->unk0 = dllLoad(DLL_ID_178, 1);
    for (i = 0; i < 3; i++) {
        temp_v0_2 = objAllocSetup(sizeof(FlameBlast_Setup), OBJ_flameblast);
        temp_v0_2->base.x = self->srt.transl.x;
        temp_v0_2->base.y = self->srt.transl.y;
        temp_v0_2->base.z = self->srt.transl.z;
        temp_v0_2->base.loadFlags = 2;
        temp_v0_2->base.byte5 = 1;
        temp_v0_2->timer = i * 10;
        objdata->unk4[i] = objSetupObject(&temp_v0_2->base, 5, self->mapID, -1, self->parent);
    }

    temp_v0_3 = objAllocSetup(sizeof(FXEmit_Setup), OBJ_FXEmit);
    temp_v0_3->base.loadFlags = 2;
    temp_v0_3->base.byte5 = 1;
    temp_v0_3->base.x = self->srt.transl.x;
    temp_v0_3->base.y = self->srt.transl.y;
    temp_v0_3->base.z = self->srt.transl.z;
    temp_v0_3->toggleGamebit = -1;
    temp_v0_3->disableGamebit = -1;
    temp_v0_3->yaw = 0;
    temp_v0_3->flagConfig = 1;
    temp_v0_3->pitch = 0;
    temp_v0_3->roll = 0;
    temp_v0_3->rollSpeed = 0;
    temp_v0_3->pitchSpeed = 0;
    temp_v0_3->yawSpeed = 0;
    temp_v0_3->activationRange = 0;
    temp_v0_3->bank = 1;
    temp_v0_3->indexInBank = 0x4A;
    temp_v0_3->fxRate = -0x1E;
    objdata->unk10 = objSetupObject(&temp_v0_3->base, 5, self->mapID, -1, self->parent);
    return 1;
}

// offset: 0x3C4C | func: 47
CurveSetup* Kyte_func_3C4C(Object* self) {
/*0x1AC*/ static s32 data_1AC[] = { 0x15, 0x09, 0x08, 0x16 };
    CurveSetup* temp_v0;
    CurveSetup* var_s3;
    f32 temp_fv0;
    f32 var_fs0;
    s32 i;

    var_fs0 = 700.0f;
    var_s3 = NULL;
    for (i = 0; i < 4; i++) {
        temp_v0 = gDLL_25->vtbl->func_2CF8(self, data_1AC[i]);
        if (temp_v0 != NULL) {
            temp_fv0 = vec3Distance(&self->globalPosition, &temp_v0->pos);
            if (temp_fv0 < var_fs0) {
                var_fs0 = temp_fv0;
                var_s3 = temp_v0;
            }
        }
    }

    return var_s3;
}

// offset: 0x3D30 | func: 48
void Kyte_func_3D30(Object* self, DLL212_Data* objdata) {
    SRT sp48;
    u8 var_s0;

    if (objdata->unk14 == 0) {
        return;
    }

    sp48.transl.x = objdata->unk24.x - self->srt.transl.x;
    sp48.transl.y = objdata->unk24.y - self->srt.transl.y;
    sp48.transl.z = objdata->unk24.z - self->srt.transl.z;
    sp48.scale = 1.0f;
    sp48.yaw = self->srt.yaw;
    sp48.pitch = self->srt.pitch;
    sp48.roll = self->srt.roll;
    if (objdata->unk14 == 1) {
        objdata->unk10->srt.transl.x = objdata->unk24.x;
        objdata->unk10->srt.transl.y = objdata->unk24.y;
        objdata->unk10->srt.transl.z = objdata->unk24.z;
        objdata->unk10->srt.roll = self->srt.roll;
        objdata->unk10->srt.pitch = self->srt.pitch;
        objdata->unk10->srt.yaw = self->srt.yaw;
        gDLL_17_partfx->vtbl->spawn(self, 0x535, &sp48, 2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, 0x532, &sp48, 2, -1, NULL);
        return;
    }

    gDLL_6_AMSFX->vtbl->Play(self, SOUND_95B, MAX_VOLUME, NULL, NULL, 0, NULL);
    var_s0 = 20;
    // FAKE
    do {} while (0);
    while (var_s0--) {
        gDLL_17_partfx->vtbl->spawn(self, 0x533, &sp48, 2, -1, NULL);
    }
    objdata->unk14 = 0;
}

// offset: 0x3F44 | func: 49
static void Kyte_func_3F44(Object *self, DLL212_Data* objdata) {
    s32 i;

    if (objdata->unk14 == 1) {
        objdata->unk14 = 2;
        dllFree(objdata->unk0);
        objFreeObject(objdata->unk10);
        for (i = 0; i < 3; i++) { objFreeObject(objdata->unk4[i]); }
    }
}

// offset: 0x3FF4 | func: 50
void Kyte_func_3FF4(DLL212_3FF4* arg0, s32 arg1, s16 arg2) {
    arg0->unk4 = arg1;
    arg0->unk0 = arg2;
    arg0->unk8_31 = 1;
}

// offset: 0x4020 | func: 51
static void Kyte_func_4020(DLL212_3FF4* arg0, s32 arg1) {
    if (arg1 == arg0->unk4) {
        arg0->unk8_31 = 0;
    }
}

/*0x0*/ static const char str_0[] = "kyte has enterd an invalid state\n";
/*0x24*/ static const char str_24[] = " In Seq ";
/*0x30*/ static const char str_30[] = " Enable Dis %i ";
/*0x40*/ static const char str_40[] = " cleaning Up Kyte ";
/*0x54*/ static const char str_54[] = "sideCommandEnable error, command number too high\n";
/*0x88*/ static const char str_88[] = " Removing Distract ";
/*0x9C*/ static const char str_9C[] = "kyte startup warning, flight group is zero\n";
/*0xC8*/ static const char str_C8[] = "\nInitialising On Flight Group %i \n ";
/*0xEC*/ static const char str_EC[] = "\n Starting On Flight Group %i \n ";
/*0x110*/ static const char str_110[] = " Kyte Start on anon valid node ";
/*0x130*/ static const char str_130[] = "error in setting up the curve network\n";
/*0x158*/ static const char str_158[] = "curve %d, flightGroup %d, dir %d\n";
/*0x17C*/ static const char str_17C[] = "1.) Same as last frame\n";
/*0x194*/ static const char str_194[] = "2.) psuedonode\n";
/*0x1A4*/ static const char str_1A4[] = "3.) After psuedonode\n";
/*0x1BC*/ static const char str_1BC[] = "4.) Staying on the flight group %i \n";
/*0x1E4*/ static const char str_1E4[] = "setting the flight group to %d\n";
/*0x204*/ static const char str_204[] = "5.) Used route finding\n";
/*0x21C*/ static const char str_21C[] = "6.) Next node of standard type (flightGroup %d)\n";
/*0x250*/ static const char str_250[] = "7.) Staying on flight Group (even though invalid)\n";
/*0x284*/ static const char str_284[] = "8.) Just taking any old curve\n";
/*0x2A4*/ static const char str_2A4[] = "kyte error!, unable to find next curve node from %d!\n";
/*0x2DC*/ static const char str_2DC[] = "Extract route failed.\n";
/*0x2F4*/ static const char str_2F4[] = "Route finding, start %d finish %d\n";
/*0x318*/ static const char str_318[] = "routeFindNode error: couldn't find route between %d %d, return val %d\n";
/*0x360*/ static const char str_360[] = "Node exists in curve network, that isn't Kyte AI, from curvenode %d\n";
/*0x3A8*/ static const char str_3A8[] = " Found Perch ";
/*0x3B8*/ static const char str_3B8[] = " Found Perch ";
/*0x3C8*/ static const char str_3C8[] = " Perching %f ";
/*0x3D8*/ static const char str_3D8[] = "vel %f  dir %f ";
/*0x3E8*/ static const char str_3E8[] = "\n\n CURVES MOVE | CURVES MOVE \n\n";
/*0x408*/ static const char str_408[] = " Trams ";
/*0x410*/ static const char str_410[] = "Performing Kyte action ";
/*0x428*/ static const char str_428[] = "Performing Kyte action ";
/*0x440*/ static const char str_440[] = "     PERCH ::: READ KYTTE PERCH COMMAND \n\n";
/*0x46C*/ static const char str_46C[] = " \n\n Timer Reset \n";
/*0x480*/ static const char str_480[] = " LocK Activated ";
/*0x494*/ static const char str_494[] = "clearing up\n";

// offset: 0x4040 | func: 52 | export: 26
int Kyte_func_4040(Object* self, s32* arg1) {
    DLL212_Data* objdata;

    objdata = self->data;
    if (objdata->unk1D8.unk8_31) {
        diPrintf(" COMMAND ACTIVE ");
        if (arg1 != NULL) {
            *arg1 = objdata->unk1D8.unk0;
        }
        return 1;
    }

    return 0;
}

// offset: 0x40BC | func: 53
int Kyte_func_40BC(u8 arg0) {
    return (data_148[arg0] + 1) == 0 || mainGetBits(data_148[arg0]) != 0;
}

