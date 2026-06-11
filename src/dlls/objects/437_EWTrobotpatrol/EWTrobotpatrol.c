#include "common.h"
#include "dlls/objects/210_player.h"
#include "game/objects/object.h"
#include "sys/gfx/texture.h"
#include "sys/objtype.h"
#include "sys/segment_1D900.h"

typedef struct {
    ObjSetup base;
    u8 _unk18[0x1A - 0x18];
    u8 unk1A;
} EWTrobotpatrol_Setup;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 _unkC[0x18 - 0xC];
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    u8 _unk54[0x6C - 0x54];
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    s16 unk8C;
} EWTrobotpatrol_Data_90;

// size:0x30
typedef struct {
    u8 _unk0[0x30 - 0x0];
} EWTrobotpatrol_Data_120;

typedef struct {
    Object* unk0;
    u8 _unk4[0x44 - 0x4];
    f32 unk44;
    u8 _unk48[0x4F - 0x48];
    u8 unk4F;
} EWTrobotpatrol_Data_154;

// beam?
typedef struct {
    u8 _unk0[0x4 - 0x0];
    Object* unk4; // RobotBeam
    Vec3f unk8; // beam dir
    s16 _unk14;
    s16 unk16;
    s16 unk18;
    u8 unk1A;
} EWTrobotpatrol_Data_1A8;

// size:0x3C
typedef struct {
    f32 unk0;
    s16 unk4;
    s16 unk6[12];
    s16 unk1E[12];
    s16 unk36;
    s16 unk38;
    s16 unk3A;
} EWTrobotpatrol_Data_1D0;

typedef struct {
    u8 unk0;
    s32 unk4;
    s32 unk8;
    u8 _unkC[0x14 - 0xC];
    f32 unk14;
    s16 unk18;
    s16 unk1A;
    u8 _unk1C[0x1D - 0x1C];
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    Unk80008E40 unk24;
    u8 _unk4C[0x74 - 0x4C];
    s32 unk74;
    s32 (*unk78)(s32, Object*, s32, s32);
    u8 _unk7C[0x120 - 0x7C];
    EWTrobotpatrol_Data_120 unk120;
    u8 _unk150[0x198 - 0x150];
    f32 unk198;
    u8 _unk19C[0x1A4 - 0x19C];
    u8 unk1A4;
    f32 unk1A8;
    u8 _unk1AC[0x1C4 - 0x1AC];
    u8 unk1C4;
    f32 unk1C8;
    u8 unk1CC;
    u8 _unk1CD[0x1D0 - 0x1CD];
    EWTrobotpatrol_Data_1D0 unk1D0;
} EWTrobotpatrol_Data;

/*0x0*/ static u32 data_0[] = {
    0x40000201, 0x00000000, 0x00000000, 0x00000000, 0x40020103, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static Texture* bss_0;
/*0x4*/ static Texture* bss_4;
/*0x8*/ static u8 _bss_8[0x8];

void dll_437_func_139C(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_90* a2);
void dll_437_func_1BF0(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_120* a2);
void dll_437_func_2060(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_154* a2);
void dll_437_func_31F4(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_1A8* a2);
static void dll_437_func_380C(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Data_1A8*, s32);
int dll_437_func_3AC8(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3);
void dll_437_func_3B60(EWTrobotpatrol_Data_1D0* a0);
static void dll_437_func_3B70(Object*, EWTrobotpatrol_Data_1D0*, f32, f32, f32, f32);
static void dll_437_func_4004(f32 arg0, f32 arg1, f32 arg2, s16* arg3, s16* arg4);
static void dll_437_func_40A0(s16 arg0, s16 arg1, Vec3f* arg2, f32 arg3);

void dll_437_func_1414(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Data_90*);
void dll_437_func_1C74(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Data_120*);
void dll_437_func_208C(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Data_154*);
void dll_437_func_3454(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Data_1A8*);
void dll_437_func_38C4(Object*, void*, s32);
void dll_437_func_3D04(Object*, EWTrobotpatrol_Data_1D0*);

void dll_437_func_31E4(EWTrobotpatrol_Data_154*, s32, Object*);
void dll_437_func_39AC(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_90* arg2);
s32 dll_437_func_15F8(Object* arg0, EWTrobotpatrol_Data_90* arg1, EWTrobotpatrol_Data* arg2, s32 arg3);

s32 dll_437_func_1674(Object* arg0, EWTrobotpatrol_Data_90* arg1, void* arg2);
s32 dll_437_func_1920(Object* arg0, EWTrobotpatrol_Data_90* arg1, void* arg2);

// offset: 0x0 | ctor
void dll_437_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_437_dtor(void* dll) {
    if (bss_0 != NULL) {
        tex_free(bss_0);
        bss_0 = NULL;
    }
    if (bss_4 != NULL) {
        tex_free(bss_4);
        bss_4 = NULL;
    }
}

// offset: 0x88 | func: 0 | export: 0
#ifndef NON_MATCHING
void dll_437_setup(Object *self, ObjSetup *setup, s32 reset);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_setup.s")
#else
void dll_437_setup(Object* self, EWTrobotpatrol_Setup* setup, s32 reset) {
    EWTrobotpatrol_Data* temp_s0;

    if (bss_0 == NULL) {
        bss_0 = tex_load_deferred(0x127);
    }
    if (bss_4 == NULL) {
        bss_4 = tex_load_deferred(0x16F);
    }
    obj_add_object_type(self, OBJTYPE_Baddie);
    temp_s0 = self->data;
    self->animCallback = dll_437_func_3AC8;
    bzero(temp_s0, sizeof(EWTrobotpatrol_Data));
    func_80008DC0(&temp_s0->unk24);
    temp_s0->unk1CC = 1;
    temp_s0->unk1C8 = -0.02f;
    dll_437_func_139C(self, temp_s0, (u8*)temp_s0 + 0x90);
    dll_437_func_1BF0(self, temp_s0, &temp_s0->unk120);
    dll_437_func_31F4(self, temp_s0, (u8*)temp_s0 + 0x1A8);
    dll_437_func_2060(self, temp_s0, (u8*)temp_s0 + 0x154);
    temp_s0->unk4 = -1;
    temp_s0->unk8 = -1;
    temp_s0->unk0 = setup->unk1A;
    temp_s0->unk1E = 0;
    temp_s0->unk1F = 0;
    temp_s0->unk20 = 0;
    dll_437_func_3B60(&temp_s0->unk1D0);
}
#endif

// offset: 0x23C | func: 1 | export: 1
#if 1
void dll_437_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_control.s")
#else
void dll_437_control(Object* self) {
    MtxF sp5C;
    f32 sp58;
    SRT sp40;
    s32 sp3C;
    EWTrobotpatrol_Data_90* temp_s1;
    EWTrobotpatrol_Data* temp_s2;

    temp_s2 = self->data;
    dll_437_func_3D04(self, &temp_s2->unk1D0);
    if (temp_s2->unk1D0.unk4 < 0x29) {
        dll_437_func_38C4(self, temp_s2 + 0x80, 2);
        temp_s1 = (u8*)temp_s2 + 0x90;
        dll_437_func_1414(self, temp_s2, temp_s1);
        sp40.yaw = self->srt.yaw;
        sp40.pitch = 0;
        sp40.roll = 0;
        sp40.transl.x = 0.0f;
        sp40.transl.y = 0.0f;
        sp40.transl.z = 0.0f;
        sp40.scale = 1.0f;
        matrix_from_srt(&sp5C, &sp40);
        vec3_transform(&sp5C, temp_s1->unk3C, 0.0f, temp_s1->unk44, &self->velocity.x, &sp58, &self->velocity.z);
        self->srt.transl.y = (fsin16_precise(temp_s1->unk8C) * temp_s1->unk88) + temp_s1->unk84;
        sp3C = (u16) temp_s1->unk8C + (gUpdateRate << 8);
        if (sp3C >= 0x10000) {
            temp_s1->unk88 = (f32) ((f32) rand_next(0xF, 0x23) * 0.1f);
        }
        temp_s1->unk8C = (s16) sp3C;
        self->velocity.x *= 1.0666667f;
        self->velocity.z *= 1.0666667f;
        obj_move(self, self->velocity.x * gUpdateRateF, 0.0f, self->velocity.z * gUpdateRateF);
        dll_437_func_1C74(self, temp_s2, (u8*)temp_s2 + 0x120);
        dll_437_func_3454(self, temp_s2, (u8*)temp_s2 + 0x1A8);
        dll_437_func_208C(self, temp_s2, (u8*)temp_s2 + 0x154);
        temp_s2->unk1CC = 0;
    }
}
#endif

// offset: 0x4A4 | func: 2 | export: 2
void dll_437_update(Object* self) {
    Object* hitBy;
    EWTrobotpatrol_Data* objdata;
    s32 damageType;

    damageType = func_80025F40(self, &hitBy, NULL, NULL);
    objdata = self->data;
    if ((damageType == Damage_Type_Projectile) || (damageType == Damage_Type_Explosion)) {
        dll_437_func_3B70(self, &objdata->unk1D0, 
            hitBy->srt.transl.x - self->srt.transl.x, 
            hitBy->srt.transl.y - self->srt.transl.y, 
            hitBy->srt.transl.z - self->srt.transl.z, 
            15.0f);
    }
}

// offset: 0x558 | func: 3 | export: 3
void dll_437_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_print.s")

// offset: 0xBF4 | func: 4 | export: 4
void dll_437_free(Object* self, s32 onlySelf) {
    EWTrobotpatrol_Data* objdata;

    obj_free_object_type(self, OBJTYPE_Baddie);
    objdata = self->data;
    func_80008E08(&objdata->unk24);
    gDLL_13_Expgfx->vtbl->func5(self);
    dll_437_func_380C(self, objdata, (u8*)objdata + 0x1A8, onlySelf);
}

// offset: 0xC9C | func: 5 | export: 5
u32 dll_437_get_model_flags(Object* self) {
    return MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0xCAC | func: 6 | export: 6
u32 dll_437_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(EWTrobotpatrol_Data);
}

void dll_437_func_1164(Object* a0, EWTrobotpatrol_Data* a1);

// offset: 0xCC0 | func: 7
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_CC0.s")
#else
void dll_437_func_CC0(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_90* arg2, void* arg3) {
    Object* sp64;
    EWTrobotpatrol_Data* sp60;
    f32 sp5C;
    f32 sp50[3];
    Vec3s16 sp48;
    Vec3s16 sp40;

    sp60 = arg0->data;
    sp64 = get_player();
    sp50[0] = arg0->srt.transl.x - sp64->srt.transl.x;
    sp50[1] = arg0->srt.transl.y - sp64->srt.transl.y;
    sp50[2] = arg0->srt.transl.z - sp64->srt.transl.z;
    sp5C = sqrtf(SQ(sp50[0]) + SQ(sp50[1]) + SQ(sp50[2]));
    func_80007EE0(&sp64->srt.transl, &sp48);
    sp48.s[1] += 2;
    func_80007EE0(&arg0->srt.transl, &sp40);
    arg1->unk21 = func_80008048(&sp48, &sp40, NULL, NULL, 0U);
    if ((arg1->unk1F != 0) && (arg1->unk1F == 2)) {
        arg1->unk78(arg1->unk74, arg0, 1, 0);
        switch (arg1->unk20) {
        case 0:
            dll_437_func_31E4((u8*)sp60 + 0x154, 2, sp64);
            if (dll_437_func_15F8(arg0, arg2, arg1, 0) != 0) {
                bcopy(arg1->_unk4C, &arg1->_unk4C[0xC], 0xC);
                arg1->unk20 = 1;
                arg1->unk18 = 0x12C;
                return;
            }
            break;
        case 1:
            dll_437_func_15F8(arg0, arg2, arg1, 1);
            if ((sp5C < 300.0f) && (arg1->unk21 != 0)) {
                arg1->unk20 = 2;
                arg1->unk18 = 0x12C;
                arg1->unk1A = 0;
                return;
            }
            arg1->unk18 = (s16) (arg1->unk18 - gUpdateRate);
            if (arg1->unk18 < 0) {
                arg1->unk1E = 0;
                arg1->unk1F = 0;
                arg1->unk20 = 0;
            }
            break;
        case 2:
            dll_437_func_15F8(arg0, arg2, arg1, 1);
            if ((sp5C > 300.0f) || (arg1->unk21 == 0)) {
                arg1->unk20 = 1;
            }
            arg1->unk1A = (s16) (arg1->unk1A - gUpdateRate);
            if (arg1->unk1A < 0) {
                arg1->unk1A = 0;
            }
            if (arg1->unk1A == 0) {
                if ((arg1->unk21 != 0) 
                        && (((DLL_210_Player*)sp64->dll)->vtbl->func66(sp64, 9) == 0) 
                        && (((DLL_210_Player*)sp64->dll)->vtbl->func66(sp64, 1) != 0)) {
                    sp60->unk1A4 = 3;
                }
                arg1->unk1A = 0x96;
                return;
            }
            break;
        }
    } else {
        sp60->unk198 = -0.02f;
        if (arg1->unk4 != -1) {
            dll_437_func_39AC(arg0, arg1, arg2);
            if (arg1->unk1D > 0) {
                arg1->unk1D = arg1->unk1D - 1;
            }
            if ((arg1->unk1D == 0) && (arg1->unk14 < 3.0f)) {
                arg1->unk78(arg1->unk74, arg0, 0, arg1->unk4);
                arg1->unk1D = 0x78;
            }
        }
        dll_437_func_1164(arg0, arg1);
        if (arg1->unk1E == 2) {
            gDLL_6_AMSFX->vtbl->play(arg0, 0x112, 0x7F, NULL, NULL, 0, NULL);
            if (sp60){} // @fake
            sp60->unk198 = 0.02f;
            sp60->unk1C4 = 0;
            sp60->unk1A8 = 0.0f;
            bcopy(&arg0->srt.transl, arg1->_unk4C, 0xC);
            arg1->unk1F = 2;
            arg1->unk20 = 0;
        }
    }
}
#endif

// offset: 0x1164 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_1164.s")

// offset: 0x139C | func: 9
void dll_437_func_139C(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_90* a2) {
    a2->unk6C = 50.0f;
    a2->unk7C = 1.0f / a2->unk6C;
    a2->unk78 = (a2->unk6C * 0.5f) * 0.5f;
    a2->unk80 = 1.0f / a2->unk78;
    a2->unk70 = 7.0f;
    a2->unk84 = self->srt.transl.y + a2->unk6C;
    a2->unk88 = 2.0f;
    a2->unk8C = 0;
}

// offset: 0x1414 | func: 10
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_1414.s")
#else
void dll_437_func_CC0(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_90* arg2, void* arg3);

void dll_437_func_1414(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_90* arg2) {
    f32 temp_fa1;
    f32 temp_ft4;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 temp_fv1;
    f32 temp_fv1_2;

    arg2->unk74 = gUpdateRateF / 60.0f;
    arg2->unk24 = arg2->unk28 = arg2->unk2C = 0.0f;
    temp_fv0 = arg2->unk44;
    temp_fv1 = arg2->unk40;
    temp_fa1 = arg2->unk70 * 0.65f;
    temp_ft4 = arg2->unk3C;
    arg2->unk38 = SQ(temp_fv0);
    arg2->unk30 = SQ(temp_ft4) * temp_fa1;
    arg2->unk34 = SQ(temp_fv1) * temp_fa1;
    arg2->unk38 *= temp_fa1;
    if (temp_ft4 > 0.0f) {
        arg2->unk30 = -arg2->unk30;
    }
    if (arg2->unk40 > 0.0f) {
        arg2->unk34 = -arg2->unk34;
    }
    if (arg2->unk44 > 0.0f) {
        arg2->unk38 = -arg2->unk38;
    }
    arg2->unk30 = 0.0f;
    arg2->unk34 = 0.0f;
    arg2->unk38 = 0.0f;
    arg2->unk18 = arg2->unk24;
    arg2->unk1C = arg2->unk28;
    arg2->unk20 = arg2->unk2C;
    dll_437_func_CC0(arg0, arg1, arg2, (u8*)arg1 + 0x80);
    temp_fv0_2 = arg2->unk7C;
    temp_fv1_2 = arg2->unk74;
    arg2->unk18 += arg2->unk0;
    arg2->unk1C += arg2->unk4;
    arg2->unk20 += arg2->unk8;
    arg2->unk4C = arg2->unk1C * temp_fv0_2;
    arg2->unk50 = arg2->unk20 * temp_fv0_2;
    arg2->unk48 = arg2->unk18 * temp_fv0_2 * temp_fv1_2;
    arg2->unk4C *= temp_fv1_2;
    arg2->unk50 *= temp_fv1_2;
    arg2->unk3C = arg2->unk48 + arg2->unk3C;
    arg2->unk40 = arg2->unk4C + arg2->unk40;
    arg2->unk44 = arg2->unk50 + arg2->unk44;
}
#endif

// offset: 0x15F8 | func: 11
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_15F8.s")
#else
s32 dll_437_func_15F8(Object* arg0, EWTrobotpatrol_Data_90* arg1, EWTrobotpatrol_Data* arg2, s32 arg3) {
    switch (arg3) {
    case 0:
        return dll_437_func_1674(arg0, arg1, (u8*)arg2 + 0x58); // TODO: 
    case 1:
        return dll_437_func_1920(arg0, arg1, (u8*)arg2 + 0x58);
    default:
        return 0; 
    }
}
#endif

// offset: 0x1674 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_1674.s")

// offset: 0x1920 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_1920.s")

// offset: 0x1BF0 | func: 14
void dll_437_func_1BF0(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_120* a2) {
    u32 sp20[] = {
        0x41180000, 0x40800000, 0x00000000, 0xc1180000, 0x40800000, 0x00000000, 0x00000000, 0x40800000, 
        0x41180000, 0x00000000, 0x40800000, 0xc1180000
    };

    bcopy(&sp20, a2, 0x30);
}

/*0x50*/ static u32 data_50[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x1C74 | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_1C74.s")

// offset: 0x2060 | func: 16
void dll_437_func_2060(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_154* a2) {
    a2->unk44 = -0.02f;
    a2->unk4F = 0;
}

// offset: 0x208C | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_208C.s")

// offset: 0x2158 | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_2158.s")

// offset: 0x231C | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_231C.s")

// offset: 0x2C38 | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_2C38.s")

// offset: 0x2ED4 | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_2ED4.s")

// offset: 0x31E4 | func: 22
void dll_437_func_31E4(EWTrobotpatrol_Data_154* arg0, s32 arg1, Object* arg2) {
    arg0->unk4F = arg1;
    arg0->unk0 = arg2;
}

// offset: 0x31F4 | func: 23
void dll_437_func_31F4(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_1A8* a2) {
    ObjSetup* beamSetup;
    SRT sp64;
    LightAction laction;
    ObjectShadow* beamShadow;
    Object* beam;

    beamSetup = obj_alloc_setup(sizeof(ObjSetup), OBJ_RobotBeam);
    beamSetup->loadFlags = 8;
    beamSetup->byte5 = 4;
    beamSetup->byte6 = 0x78;
    beamSetup->fadeDistance = 0x78;
    beamSetup->quarterSize = 0x18;
    beamSetup->objId = OBJ_RobotBeam;
    beam = obj_create(beamSetup, OBJINIT_STANDALONE | OBJINIT_FLAG4, -1, -1, self->parent);
    beam->srt.transl.x = self->srt.transl.x;
    beam->srt.transl.y = self->srt.transl.y;
    beam->srt.transl.z = self->srt.transl.z;
    get_object_child_position(beam, 
        &beam->globalPosition.x, 
        &beam->globalPosition.y, 
        &beam->globalPosition.z);
    sp64.yaw = self->def->pAttachPoints->rot.x;
    sp64.pitch = self->def->pAttachPoints->rot.y;
    sp64.roll = self->def->pAttachPoints->rot.z;
    a2->unk8.x = 0.0f;
    a2->unk8.y = 1.0f;
    a2->unk8.z = 0.0f;
    rotate_vec3(&sp64, a2->unk8.f);
    beamShadow = beam->shadow;
    if (beamShadow != NULL) {
        beamShadow->flags |= 0x170;
        beamShadow->dir.x = a2->unk8.x;
        beamShadow->dir.y = a2->unk8.y;
        beamShadow->dir.z = a2->unk8.z;
        beamShadow->r = 0xFF;
        beamShadow->g = 0xFF;
        beamShadow->b = 0xFF;
        beamShadow->a = 0x3C;
    }
    a2->unk4 = beam;
    bzero(&laction, sizeof(LightAction));
    laction.unk12.asByte = 0x15;
    laction.unk19 = 0xFF;
    laction.unk1a = 0xFF;
    laction.unk18 = 0;
    laction.unk10 = 0xFFFE;
    laction.unk1c = 1;
    laction.unk4 = 0;
    laction.unk6 = -0xA;
    laction.unk8 = 0;
    laction.unka = 0x46;
    laction.unkC = 0xA;
    laction.unk1d = 0xFF;
    laction.unk22 = 2;
    laction.unk1f = 0xF;
    laction.unk20 = 8;
    laction.unk0 = 0;
    gDLL_11_Newlfx->vtbl->func0(self, self, &laction, 0, 0, 0);
    a2->unk1A = self->unkD6;
    a2->unk18 = 0;
    a2->unk16 = 0;
}

// offset: 0x3454 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_3454.s")

// offset: 0x380C | func: 25
static void dll_437_func_380C(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_1A8* arg2, s32 arg3) {
    LightAction laction;

    laction.unk12.asByte = 2;
    laction.unke = 0;
    laction.unk1b = 0;
    laction.unk0 = 0;
    laction.unk10 = arg2->unk1A;
    gDLL_11_Newlfx->vtbl->func0(arg0, arg0, &laction, 0, 0, 0);
    if (arg3 == 0) {
        if (arg2->unk4 != NULL) {
            obj_destroy_object(arg2->unk4);
            arg2->unk4 = NULL;
        }
    }
}

// offset: 0x38C4 | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_38C4.s")

// offset: 0x39AC | func: 27
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_39AC.s")

// offset: 0x3AC8 | func: 28
int dll_437_func_3AC8(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3) {
    animObjData->unk62 = 0;
    return 0;
}

// offset: 0x3AE4 | func: 29 | export: 7
void dll_437_func_3AE4(Object* arg0, s32 arg1, s32 arg2) {
    EWTrobotpatrol_Data* objdata = arg0->data;
    objdata->unk74 = arg1;
    objdata->unk78 = arg2;
}

// offset: 0x3AF8 | func: 30 | export: 8
void dll_437_func_3AF8(Object* arg0, s32 arg1) {
    EWTrobotpatrol_Data* objdata = arg0->data;
    objdata->unk8 = objdata->unk4;
    objdata->unk4 = arg1;
    objdata->unk1D = 0;
    objdata->unk14 = 1000.0f;
    bzero((u8*)objdata + 0xCC, 0xC); // TODO: vec3?
}

// offset: 0x3B60 | func: 31
void dll_437_func_3B60(EWTrobotpatrol_Data_1D0* a0) {
    a0->unk4 = 0;
    a0->unk38 = 0;
}

// offset: 0x3B70 | func: 32
static void dll_437_func_3B70(Object* arg0, EWTrobotpatrol_Data_1D0* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    s32 var_v1;
    SRT sp2C;

    if ((arg2 != 0.0f) || (arg3 != 0.0f) || (arg4 != 0.0f)) {
        guNormalize(&arg2, &arg3, &arg4);
    }
    arg1->unk36 = rand_next(8, 12);
    dll_437_func_4004(arg2, arg3, arg4, &arg1->unk6[0], &arg1->unk1E[0]);
    var_v1 = 1;
    while (var_v1 < arg1->unk36) {
        arg1->unk6[var_v1] = arg1->unk6[0];
        arg1->unk1E[var_v1] = arg1->unk1E[0];
        var_v1 += 1;
    }
    arg1->unk4 = 0x190;
    arg1->unk38 += 0x500;
    arg1->unk3A = 0xFF;
    arg1->unk0 = arg5;
    sp2C.transl.x = arg2 * arg5;
    sp2C.transl.y = arg3 * arg5;
    sp2C.transl.z = arg4 * arg5;
    gDLL_17_partfx->vtbl->spawn(arg0, 0x35B, &sp2C, 0, -1, NULL);
}

// offset: 0x3D04 | func: 33
void dll_437_func_3D04(Object* arg0, EWTrobotpatrol_Data_1D0* arg1) {
    SRT sp80;
    f32 temp_fs0;
    s16 temp_s2;
    s16 temp_s3;
    s32 temp_ft0;
    s32 var_t0;
    s32 var_s1;

    if (arg1->unk4 != 0) {
        if (arg1->unk4 >= 0x100) {
            sp80.roll = 0xFF;
        } else {
            sp80.roll = arg1->unk4;
        }
        if (arg1->unk3A > 0) {
            sp80.yaw = arg1->unk3A;
        } else {
            sp80.yaw = 0;
        }
        arg1->unk3A -= gUpdateRate;
        for (var_t0 = 0; var_t0 < (gUpdateRate / 2); var_t0++) {
            for (var_s1 = 0; var_s1 < arg1->unk36; var_s1++) {
                dll_437_func_40A0(arg1->unk6[var_s1], arg1->unk1E[var_s1], &sp80.transl, arg1->unk0);
                gDLL_17_partfx->vtbl->spawn(arg0, (var_s1 % 2) + 0x35C, &sp80, 0, -1, NULL);
                temp_s2 = ((0xFFFF / (s16) arg1->unk36) * var_s1) + arg1->unk38;
                temp_fs0 = fsin16_precise(temp_s2);
                temp_s3 = (s16) (s32) (((f32) rand_next(0, 0x600) + (temp_fs0 * 1792.0f)) - 768.0f);
                temp_fs0 = fcos16_precise(temp_s2);
                temp_ft0 = rand_next(0, 0x600);
                arg1->unk6[var_s1] += temp_s3;
                temp_s3 = (s32) (((f32) temp_ft0 + (temp_fs0 * 1792.0f)) - 768.0f);
                arg1->unk1E[var_s1] += temp_s3;
            }
        }
        arg1->unk4 -= gUpdateRate;
        if (arg1->unk4 < 0) {
            arg1->unk4 = 0;
        }
    }
}

// offset: 0x4004 | func: 34
static void dll_437_func_4004(f32 arg0, f32 arg1, f32 arg2, s16* arg3, s16* arg4) {
    f32 sp24 = sqrtf(SQ(arg0) + SQ(arg2));
    *arg3 = arctan2_f(arg2, arg0);
    *arg4 = arctan2_f(sp24, arg1);
}

// offset: 0x40A0 | func: 35
static void dll_437_func_40A0(s16 arg0, s16 arg1, Vec3f* arg2, f32 arg3) {
    arg2->x = (fsin16_precise(arg1) * fcos16_precise(arg0) * arg3);
    arg2->y = (fcos16_precise(arg1) * arg3) + 3.0f;
    arg2->z = (fsin16_precise(arg1) * fsin16_precise(arg0) * arg3);
}
