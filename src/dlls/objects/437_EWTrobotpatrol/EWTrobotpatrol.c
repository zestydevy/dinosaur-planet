#include "common.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/437_EWTrobotpatrol.h"
#include "game/objects/object.h"
#include "sys/gfx/texture.h"
#include "sys/objtype.h"
#include "sys/lighting.h"

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
    Vec3f unk3C;
    Vec3f unk48;
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
    u8 unk8E;
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
    f32 unk0;
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
    Vec3f unk4C;
    Vec3f unk58;
    u8 _unk64[0x74 - 0x64];
    Object* unk74;
    EWTrobotpatrolCallback unk78;
    Object* unk7C;
    u8 _unk80[0x120 - 0x80];
    EWTrobotpatrol_Data_120 unk120;
    u8 _unk150[0x154 - 0x150];
    EWTrobotpatrol_Data_154 unk154;
    u8 unk1A4;
    EWTrobotpatrol_Data_1A8 unk1A8;
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

static void dll_437_func_1164(Object* self, EWTrobotpatrol_Data* objdata);
void dll_437_func_1414(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Data_90*);
void dll_437_func_1C74(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Data_120*);
void dll_437_func_208C(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Data_154*);
void dll_437_func_3454(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Data_1A8*);
void dll_437_func_38C4(Object*, void*, s32);
void dll_437_func_3D04(Object*, EWTrobotpatrol_Data_1D0*);

static void dll_437_func_31E4(EWTrobotpatrol_Data_154*, s32, Object*);
static void dll_437_func_39AC(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_90* arg2);
static s32 dll_437_func_15F8(Object* arg0, EWTrobotpatrol_Data_90* arg1, EWTrobotpatrol_Data* arg2, s32 arg3);

static s32 dll_437_func_1674(Object* arg0, EWTrobotpatrol_Data_90* arg1, Vec3f* arg2);
static s32 dll_437_func_1920(Object* arg0, EWTrobotpatrol_Data_90* arg1, Vec3f* arg2);

// offset: 0x0 | ctor
void dll_437_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_437_dtor(void* dll) {
    if (bss_0 != NULL) {
        texFreeTexture(bss_0);
        bss_0 = NULL;
    }
    if (bss_4 != NULL) {
        texFreeTexture(bss_4);
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
        bss_0 = texLoadTexture(0x127);
    }
    if (bss_4 == NULL) {
        bss_4 = texLoadTexture(0x16F);
    }
    objAddObjectType(self, OBJTYPE_Baddie);
    temp_s0 = self->data;
    self->animCallback = dll_437_func_3AC8;
    bzero(temp_s0, sizeof(EWTrobotpatrol_Data));
    vox_func_80008DC0(&temp_s0->unk24);
    temp_s0->unk1CC = 1;
    temp_s0->unk1C8 = -0.02f;
    dll_437_func_139C(self, temp_s0, (u8*)temp_s0 + 0x90);
    dll_437_func_1BF0(self, temp_s0, &temp_s0->unk120);
    dll_437_func_31F4(self, temp_s0, &temp_s0->unk1A8);
    dll_437_func_2060(self, temp_s0, &temp_s0->unk154);
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
        mathYprXyzMtx(&sp5C, &sp40);
        mathMtxXFMF(&sp5C, temp_s1->unk3C, 0.0f, temp_s1->unk44, &self->velocity.x, &sp58, &self->velocity.z);
        self->srt.transl.y = (mathSinfInterp(temp_s1->unk8C) * temp_s1->unk88) + temp_s1->unk84;
        sp3C = (u16) temp_s1->unk8C + (gUpdateRate << 8);
        if (sp3C >= 0x10000) {
            temp_s1->unk88 = (f32) ((f32) mathRnd(0xF, 0x23) * 0.1f);
        }
        temp_s1->unk8C = (s16) sp3C;
        self->velocity.x *= 1.0666667f;
        self->velocity.z *= 1.0666667f;
        objMove(self, self->velocity.x * gUpdateRateF, 0.0f, self->velocity.z * gUpdateRateF);
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

    objFreeObjectType(self, OBJTYPE_Baddie);
    objdata = self->data;
    vox_func_80008E08(&objdata->unk24);
    gDLL_13_Expgfx->vtbl->func5(self);
    dll_437_func_380C(self, objdata, &objdata->unk1A8, onlySelf);
}

// offset: 0xC9C | func: 5 | export: 5
u32 dll_437_get_model_flags(Object* self) {
    return MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0xCAC | func: 6 | export: 6
u32 dll_437_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(EWTrobotpatrol_Data);
}

// offset: 0xCC0 | func: 7
void dll_437_func_CC0(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_90* arg2, void* arg3) {
    Object* player;
    EWTrobotpatrol_Data* objdata2;
    f32 playerDist;
    f32 vec[3];
    Vec3s16 playerVoxPos;
    Vec3s16 selfVoxPos;

    objdata2 = self->data;
    player = objGetPlayer();
    vec[0] = self->srt.transl.x - player->srt.transl.x;
    vec[1] = self->srt.transl.y - player->srt.transl.y;
    vec[2] = self->srt.transl.z - player->srt.transl.z;
    playerDist = sqrtf(SQ(vec[0]) + SQ(vec[1]) + SQ(vec[2]));
    vox_func_80007EE0(&player->srt.transl, &playerVoxPos);
    playerVoxPos.s[1] += 2;
    vox_func_80007EE0(&self->srt.transl, &selfVoxPos);
    objdata->unk21 = vox_func_80008048(&playerVoxPos, &selfVoxPos, NULL, NULL, 0);
    if ((objdata->unk1F != 0) && (objdata->unk1F == 2)) {
#ifndef AVOID_UB
        PRAGMA_IGNORE_PUSH("-Wcast-function-type-mismatch")
        // @fake cast
        ((s32 (*)(Object *, Object *, s32, u32))objdata->unk78)(objdata->unk74, self, 1, 0);
        PRAGMA_IGNORE_POP()
#else
        arg1->unk78(arg1->unk74, arg0, 1, 0);
#endif
        switch (objdata->unk20) {
        case 0:
            dll_437_func_31E4(&objdata2->unk154, 2, player);
            if (dll_437_func_15F8(self, arg2, objdata, 0) != 0) {
                bcopy(&objdata->unk4C, &objdata->unk58, sizeof(objdata->unk58));
                objdata->unk20 = 1;
                objdata->unk18 = 0x12C;
            }
            break;
        case 1:
            dll_437_func_15F8(self, arg2, objdata, 1);
            if ((playerDist < 300.0f) && (objdata->unk21 != 0)) {
                objdata->unk20 = 2;
                objdata->unk18 = 0x12C;
                objdata->unk1A = 0;
            } else {
                objdata->unk18 -= gUpdateRate;
                if (objdata->unk18 < 0) {
                    objdata->unk1E = 0;
                    objdata->unk1F = 0;
                    objdata->unk20 = 0;
                }
            }
            break;
        case 2:
            dll_437_func_15F8(self, arg2, objdata, 1);
            if ((playerDist > 300.0f) || (objdata->unk21 == 0)) {
                objdata->unk20 = 1;
            }
            objdata->unk1A -= gUpdateRate;
            if (objdata->unk1A < 0) {
                objdata->unk1A = 0;
            }
            if (objdata->unk1A == 0) {
                if ((objdata->unk21 != 0) 
                        && (((DLL_210_Player*)player->dll)->vtbl->func66(player, 9) == 0) 
                        && (((DLL_210_Player*)player->dll)->vtbl->func66(player, 1) != 0)) {
                    objdata2->unk1A4 = 3;
                }
                objdata->unk1A = 0x96;
            }
            break;
        }
    } else {
        objdata2->unk154.unk44 = -0.02f;
        if (objdata->unk4 != -1) {
            dll_437_func_39AC(self, objdata, arg2);
            if (objdata->unk1D > 0) {
                objdata->unk1D = objdata->unk1D - 1;
            }
            if ((objdata->unk1D == 0) && (objdata->unk14 < 3.0f)) {
                objdata->unk78(objdata->unk74, self, 0, objdata->unk4);
                objdata->unk1D = 0x78;
            }
        }
        dll_437_func_1164(self, objdata);
        if (objdata->unk1E == 2) {
            dll_amSfx->Play(self, SOUND_112, MAX_VOLUME, NULL, NULL, 0, NULL);
            if (objdata2){} // @fake
            objdata2->unk154.unk44 = 0.02f;
            objdata2->unk1C4 = 0;
            objdata2->unk1A8.unk0 = 0.0f;
            bcopy(&self->srt.transl, &objdata->unk4C, sizeof(objdata->unk4C));
            objdata->unk1F = 2;
            objdata->unk20 = 0;
        }
    }
}

// offset: 0x1164 | func: 8
static void dll_437_func_1164(Object* self, EWTrobotpatrol_Data* objdata) {
    Object* player;
    Vec3f sp70;
    Vec3f sp64;
    f32 temp_fa1;
    s32 temp_v0_3;
    EWTrobotpatrol_Data* objdata2;
    EWTrobotpatrol_Data_1A8* temp_v0_2;
    SRT sp3C;
    s32 _pad;

    player = objGetPlayer();
    objdata->unk7C = player;
    if (objdata->unk1E == 2) {
        return;
    }
    objdata2 = self->data;
    temp_v0_2 = &objdata2->unk1A8;
    sp70.f[0] = player->srt.transl.x - self->srt.transl.x;
    sp70.f[1] = player->srt.transl.y - self->srt.transl.y;
    sp70.f[2] = player->srt.transl.z - self->srt.transl.z;
    if (sqrtf(SQ(sp70.f[0]) + SQ(sp70.f[1]) + SQ(sp70.f[2])) < 150.0f) {
        if (objdata->unk21 != 0) {
            sp64.x = player->srt.transl.x - temp_v0_2->unk4->srt.transl.x;
            sp64.y = player->srt.transl.y - temp_v0_2->unk4->srt.transl.y;
            sp64.z = player->srt.transl.z - temp_v0_2->unk4->srt.transl.z;
            temp_fa1 = sqrtf(SQ(temp_v0_2->unk8.x) + SQ(temp_v0_2->unk8.z));
            sp3C.yaw = -temp_v0_2->unk4->srt.yaw;
            sp3C.pitch = -mathAtan2f(temp_v0_2->unk8.y, temp_fa1);
            sp3C.roll = 0;
            mathRotateYPR(&sp3C, &sp64);
            if (sqrtf(SQ(sp64.x) + SQ(sp64.y)) <= 40.0f) {
                objdata->unk1E = 2;
            }
        }
        if (objdata->unk1E != 2) {
            temp_v0_3 = (s32)((DLL_210_Player*)player->dll)->vtbl->func66(player, 2);
            if ((temp_v0_3 != 3) && (temp_v0_3 != 4)) {
                objdata->unk1E = 0;
                return;
            }
            objdata->unk1E = 2;
            objdata->unk7C = player;
        }
    }
}

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
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_1414.s")
#else
void dll_437_func_CC0(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_90* arg2, void* arg3);

void dll_437_func_1414(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_90* arg2) {
    f32 temp_fa1;

    arg2->unk74 = gUpdateRateF / 60.0f;
    arg2->unk24 = arg2->unk28 = arg2->unk2C = 0.0f;
    temp_fa1 = arg2->unk70 * 0.65f;
    arg2->unk30 = SQ(arg2->unk3C.x);
    arg2->unk34 = SQ(arg2->unk3C.y);
    arg2->unk38 = SQ(arg2->unk3C.z);
    arg2->unk30 *= temp_fa1;
    arg2->unk34 *= temp_fa1;
    arg2->unk38 *= temp_fa1;
    if (arg2->unk3C.x > 0.0f) {
        arg2->unk30 = -arg2->unk30;
    }
    if (arg2->unk3C.y > 0.0f) {
        arg2->unk34 = -arg2->unk34;
    }
    if (arg2->unk3C.z > 0.0f) {
        arg2->unk38 = -arg2->unk38;
    }
    arg2->unk30 = 0.0f;
    arg2->unk34 = 0.0f;
    arg2->unk38 = 0.0f;
    arg2->unk18 = arg2->unk24;
    arg2->unk1C = arg2->unk28;
    arg2->unk20 = arg2->unk2C;
    dll_437_func_CC0(arg0, arg1, arg2, &arg1->_unk80);
    arg2->unk18 += arg2->unk0;
    arg2->unk1C += arg2->unk4;
    arg2->unk20 += arg2->unk8;
    arg2->unk48.x = arg2->unk18 * arg2->unk7C;
    arg2->unk48.y = arg2->unk1C * arg2->unk7C;
    arg2->unk48.z = arg2->unk20 * arg2->unk7C;
    arg2->unk48.x *= arg2->unk74;
    arg2->unk48.y *= arg2->unk74;
    arg2->unk48.z *= arg2->unk74;
    arg2->unk3C.x = arg2->unk48.x + arg2->unk3C.x;
    arg2->unk3C.y = arg2->unk48.y + arg2->unk3C.y;
    arg2->unk3C.z = arg2->unk48.z + arg2->unk3C.z;
}
#endif

// offset: 0x15F8 | func: 11
static s32 dll_437_func_15F8(Object* arg0, EWTrobotpatrol_Data_90* arg1, EWTrobotpatrol_Data* arg2, s32 arg3) {
    switch (arg3) {
    case 0:
        return dll_437_func_1674(arg0, arg1, &arg2->unk58);
    case 1:
        return dll_437_func_1920(arg0, arg1, &arg2->unk58);
    default:
        return 0; 
    }
}

// offset: 0x1674 | func: 12
static s32 dll_437_func_1674(Object* arg0, EWTrobotpatrol_Data_90* arg1, Vec3f* arg2) {
    Vec3f sp34;
    f32 temp_fa0;
    f32 temp_fv0_3;
    f32 var_fv1;
    s32 temp_v0;

    if ((arg1->unk3C.x > -0.01f) && (arg1->unk3C.x < 0.01f)) {
        arg1->unk3C.x = 0.0f;
    }
    if ((arg1->unk3C.z > -0.01f) && (arg1->unk3C.z < 0.01f)) {
        arg1->unk3C.z = 0.0f;
    }
    temp_fv0_3 = sqrtf(SQ(arg1->unk3C.f[0]) + SQ(arg1->unk3C.f[1]) + SQ(arg1->unk3C.f[2]));
    if (temp_fv0_3 < 0.1f) {
        bzero(&arg1->unk3C, sizeof(arg1->unk3C));
        bzero(&arg1->unk48, sizeof(arg1->unk48));
        arg1->unk0 = 0.0f;
        arg1->unk8 = 0.0f;
        return 1;
    }
    temp_fa0 = temp_fv0_3 / arg1->unk74;
    sp34.x = arg2->x - arg0->srt.transl.x;
    sp34.y = arg2->y - arg0->srt.transl.y;
    sp34.z = arg2->z - arg0->srt.transl.z;
    if (((sp34.f[0] * arg1->unk3C.f[0]) + (sp34.f[1] * arg1->unk3C.f[1]) + (sp34.f[2] * arg1->unk3C.f[2])) >= 0.0f) {
        var_fv1 = -arg1->unk6C * temp_fa0;
    } else {
        var_fv1 = arg1->unk6C * temp_fa0;
    }
    if (var_fv1 > 30.0f) {
        var_fv1 = 30.0f;
    }
    if (var_fv1 < -30.0f) {
        var_fv1 = -30.0f;
    }
    if ((var_fv1 > -0.1f) && (var_fv1 < 0.1f)) {
        var_fv1 = 0.0f;
    }
    temp_v0 = mathAtan2f(sp34.x, sp34.z);
    arg1->unk0 = mathSinfInterp(temp_v0) * var_fv1;
    arg1->unk8 = mathCosfInterp(temp_v0) * var_fv1;
    return 0;
}

// offset: 0x1920 | func: 13
static s32 dll_437_func_1920(Object* arg0, EWTrobotpatrol_Data_90* arg1, Vec3f* arg2) {
    Vec3f sp54;
    f32 temp3;
    f32 temp;
    f32 temp2;
    f32 sp44;
    f32 sp48;
    s32 _pad[2];
    f32 var_fs0;
    s32 sp30;
    s32 sp2C;

    var_fs0 = 0.0f;
    sp54.x = arg2->x - arg0->srt.transl.x;
    sp54.y = arg2->y - arg0->srt.transl.y;
    sp54.z = arg2->z - arg0->srt.transl.z;
    sp54.x /= 64;
    sp54.y = 0.0f;
    sp54.z /= 64;
    temp2 = sqrtf(SQ(sp54.f[0]) + SQ(sp54.f[1]) + SQ(sp54.f[2]));
    if (((sp54.f[0] * arg1->unk3C.f[0]) + (sp54.f[1] * arg1->unk3C.f[1]) + (sp54.f[2] * arg1->unk3C.f[2])) >= 0.0f) {
        sp44 = -temp2;
    } else {
        sp44 = temp2;
    }
    sp48 = sqrtf(SQ(arg1->unk3C.f[0]) + SQ(arg1->unk3C.f[1]) + SQ(arg1->unk3C.f[2]));
    temp3 = SQ(sp48);
    temp = (arg1->unk7C * 30.0f);
    if (temp2 > 0.05f) {
        sp2C = 0;
        if (temp2 < (temp3 / (2.0f * temp))) {
            arg1->unk8E = 0;
            var_fs0 = arg1->unk6C * (temp3 / (2.0f * sp44));
        } else {
            if (sp48 < 0.6f) {
                arg1->unk8E = 1;
            } else if (sp48 >= 1.15f) {
                arg1->unk8E = 0;
            }
            if (arg1->unk8E != 0) {
                var_fs0 = temp2 * 0.5f;
                var_fs0 *= 30.0f;
            }
        }
    } else {
        sp2C = 1;
    }
    if (var_fs0 > 30.0f) {
        var_fs0 = 30.0f;
    }
    if (var_fs0 < -30.0f) {
        var_fs0 = -30.0f;
    }
    if ((var_fs0 > -0.1f) && (var_fs0 < 0.1f)) {
        var_fs0 = 0.0f;
    }
    sp30 = mathAtan2f(sp54.x, sp54.z);
    arg1->unk0 = mathSinfInterp(sp30) * var_fs0;
    arg1->unk8 = mathCosfInterp(sp30) * var_fs0;
    return sp2C;
}

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
static void dll_437_func_31E4(EWTrobotpatrol_Data_154* arg0, s32 arg1, Object* arg2) {
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

    beamSetup = objAllocSetup(sizeof(ObjSetup), OBJ_RobotBeam);
    beamSetup->loadFlags = 8;
    beamSetup->byte5 = 4;
    beamSetup->byte6 = 0x78;
    beamSetup->fadeDistance = 0x78;
    beamSetup->quarterSize = 0x18;
    beamSetup->objId = OBJ_RobotBeam;
    beam = objSetupObject(beamSetup, OBJINIT_STANDALONE | OBJINIT_FLAG4, -1, -1, self->parent);
    beam->srt.transl.x = self->srt.transl.x;
    beam->srt.transl.y = self->srt.transl.y;
    beam->srt.transl.z = self->srt.transl.z;
    camGetObjectChildPosition(beam, 
        &beam->globalPosition.x, 
        &beam->globalPosition.y, 
        &beam->globalPosition.z);
    sp64.yaw = self->def->pAttachPoints->rot.x;
    sp64.pitch = self->def->pAttachPoints->rot.y;
    sp64.roll = self->def->pAttachPoints->rot.z;
    a2->unk8.x = 0.0f;
    a2->unk8.y = 1.0f;
    a2->unk8.z = 0.0f;
    mathRotateRPY(&sp64, a2->unk8.f);
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
    laction.unk12 = 0x15;
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

    laction.unk12 = 2;
    laction.unke = 0;
    laction.unk1b = 0;
    laction.unk0 = 0;
    laction.unk10 = arg2->unk1A;
    gDLL_11_Newlfx->vtbl->func0(arg0, arg0, &laction, 0, 0, 0);
    if (arg3 == 0) {
        if (arg2->unk4 != NULL) {
            objFreeObject(arg2->unk4);
            arg2->unk4 = NULL;
        }
    }
}

// offset: 0x38C4 | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/dll_437_func_38C4.s")

// offset: 0x39AC | func: 27
static void dll_437_func_39AC(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_90* arg2) {
    f32 temp_fv0;
    f32 temp_fv1;
    CurveSetup* sp2C;
    EWTrobotpatrol_Data* sp28;

    sp28 = arg0->data;
    sp2C = gDLL_26_Curves->vtbl->func_39C(arg1->unk4);
    if (sp2C != NULL) {
        arg1->unk58.x = sp2C->pos.x;
        arg1->unk58.y = arg0->srt.transl.y;
        arg1->unk58.z = sp2C->pos.z;
        dll_437_func_15F8(arg0, arg2, arg1, 1);
        temp_fv0 = sp2C->pos.x - arg0->srt.transl.x;
        temp_fv1 = sp2C->pos.z - arg0->srt.transl.z;
        sp28->unk1A8.unk18 = mathAtan2f(-temp_fv0, -temp_fv1);
        temp_fv0 = arg0->srt.transl.x - sp2C->pos.x;
        temp_fv1 = arg0->srt.transl.z - sp2C->pos.z;
        arg1->unk14 = sqrtf(SQ(temp_fv0) + SQ(temp_fv1));
    }
}

// offset: 0x3AC8 | func: 28
int dll_437_func_3AC8(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3) {
    animObjData->unk62 = 0;
    return 0;
}

// offset: 0x3AE4 | func: 29 | export: 7
void dll_437_func_3AE4(Object* self, Object* base, EWTrobotpatrolCallback callback) {
    EWTrobotpatrol_Data* objdata = self->data;
    objdata->unk74 = base;
    objdata->unk78 = callback;
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
    arg1->unk36 = mathRnd(8, 12);
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
                temp_fs0 = mathSinfInterp(temp_s2);
                temp_s3 = (s16) (s32) (((f32) mathRnd(0, 0x600) + (temp_fs0 * 1792.0f)) - 768.0f);
                temp_fs0 = mathCosfInterp(temp_s2);
                temp_ft0 = mathRnd(0, 0x600);
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
    *arg3 = mathAtan2f(arg2, arg0);
    *arg4 = mathAtan2f(sp24, arg1);
}

// offset: 0x40A0 | func: 35
static void dll_437_func_40A0(s16 arg0, s16 arg1, Vec3f* arg2, f32 arg3) {
    arg2->x = (mathSinfInterp(arg1) * mathCosfInterp(arg0) * arg3);
    arg2->y = (mathCosfInterp(arg1) * arg3) + 3.0f;
    arg2->z = (mathSinfInterp(arg1) * mathSinfInterp(arg0) * arg3);
}
