#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/437_EWTrobotpatrol.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/camera.h"
#include "sys/dll.h"
#include "sys/gfx/textable.h"
#include "sys/gfx/texture.h"
#include "sys/gfx/projgfx.h"
#include "sys/gfx/animseq.h"
#include "sys/joypad.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/lighting.h"
#include "sys/voxmap.h"
#include "dll.h"

typedef struct {
/*0*/ f32 unk0;
/*4*/ f32 unk4;
/*8*/ u16 unk8;
/*A*/ u16 unkA;
/*C*/ u16 unkC;
/*E*/ s8 unkE;
/*F*/ s8 unkF;
} EWTrobotpatrol_Data_80;

typedef struct {
/*00*/ f32 unk0;
/*04*/ f32 unk4;
/*08*/ f32 unk8;
/*0C*/ u8 _unkC[0x18 - 0xC];
/*18*/ f32 unk18;
/*1C*/ f32 unk1C;
/*20*/ f32 unk20;
/*24*/ f32 unk24;
/*28*/ f32 unk28;
/*2C*/ f32 unk2C;
/*30*/ f32 unk30;
/*34*/ f32 unk34;
/*38*/ f32 unk38;
/*3C*/ Vec3f unk3C;
/*48*/ Vec3f unk48;
/*54*/ u8 _unk54[0x6C - 0x54];
/*6C*/ f32 unk6C;
/*70*/ f32 unk70;
/*74*/ f32 unk74;
/*78*/ f32 unk78;
/*7C*/ f32 unk7C;
/*80*/ f32 unk80;
/*84*/ f32 unk84;
/*88*/ f32 unk88;
/*8C*/ s16 unk8C;
/*8E*/ u8 unk8E;
} EWTrobotpatrol_Data_90;

// size:0x34 ?
typedef struct {
/*00*/ Vec3f unk0[4];
/*30*/ u8 unk30;
} EWTrobotpatrol_Data_120;

typedef struct {
/*00*/ Object* unk0;
/*04*/ u8 _unk4[0x8 - 0x4];
/*08*/ Vec3f unk8;
/*14*/ f32 unk14;
/*18*/ f32 unk18;
/*1C*/ f32 unk1C;
/*20*/ Vec3f unk20;
/*2C*/ Vec3f unk2C;
/*38*/ Vec3f unk38;
/*44*/ f32 unk44;
/*48*/ s16 unk48;
/*4A*/ s16 unk4A;
/*4C*/ s16 unk4C;
/*4E*/ u8 unk4E;
/*4F*/ u8 unk4F;
/*50*/ u8 unk50;
/*51*/ u8 unk51;
} EWTrobotpatrol_Data_154;

// beam?
typedef struct {
/*00*/ f32 unk0;
/*04*/ Object* unk4; // RobotBeam
/*08*/ Vec3f unk8; // beam dir
/*14*/ s16 unk14;
/*16*/ s16 unk16;
/*18*/ s16 unk18;
/*1A*/ u8 unk1A;
/*1B*/ u8 unk1B;
/*1C*/ u8 unk1C;
} EWTrobotpatrol_Data_1A8;

// size:0x3C
typedef struct {
/*00*/ f32 unk0;
/*04*/ s16 unk4;
/*06*/ s16 unk6[12];
/*1E*/ s16 unk1E[12];
/*36*/ s16 unk36;
/*38*/ s16 unk38;
/*3A*/ s16 unk3A;
} EWTrobotpatrol_Data_1D0;

typedef struct {
/*000*/ u8 unk0; // unused
/*004*/ s32 unk4;
/*008*/ s32 unk8;
/*00C*/ u8 _unkC[0x14 - 0xC];
/*014*/ f32 unk14;
/*018*/ s16 unk18;
/*01A*/ s16 unk1A;
/*01C*/ u8 _unk1C[0x1D - 0x1C];
/*01D*/ u8 unk1D;
/*01E*/ u8 unk1E;
/*01F*/ u8 unk1F;
/*020*/ u8 unk20;
/*021*/ u8 unk21;
/*024*/ Unk80008E40 unk24;
/*04C*/ Vec3f unk4C;
/*058*/ Vec3f unk58;
/*064*/ u8 _unk64[0x74 - 0x64];
/*074*/ Object* unk74;
/*078*/ EWTrobotpatrolCallback unk78;
/*07C*/ Object* unk7C;
/*080*/ EWTrobotpatrol_Data_80 unk80;
/*090*/ EWTrobotpatrol_Data_90 unk90;
/*120*/ EWTrobotpatrol_Data_120 unk120;
/*154*/ EWTrobotpatrol_Data_154 unk154;
/*1A8*/ EWTrobotpatrol_Data_1A8 unk1A8;
/*1C8*/ f32 unk1C8;
/*1CC*/ u8 unk1CC;
/*1D0*/ EWTrobotpatrol_Data_1D0 unk1D0;
} EWTrobotpatrol_Data;

/*0x0*/ static DLTri data_0[] = {
    {0x40, 0, 2, 1, {0}}, 
    {0x40, 2, 1, 3, {0}}
};

/*0x0*/ static Texture* bss_0;
/*0x4*/ static Texture* bss_4;

static void EWTrobotpatrol_func_1164(Object* self, EWTrobotpatrol_Data* objdata);
static void EWTrobotpatrol_func_139C(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_90* a2);
static void EWTrobotpatrol_func_1414(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Data_90*);
static s32 EWTrobotpatrol_func_15F8(Object* arg0, EWTrobotpatrol_Data_90* arg1, EWTrobotpatrol_Data* arg2, s32 arg3);
static s32 EWTrobotpatrol_func_1674(Object* arg0, EWTrobotpatrol_Data_90* arg1, Vec3f* arg2);
static s32 EWTrobotpatrol_func_1920(Object* arg0, EWTrobotpatrol_Data_90* arg1, Vec3f* arg2);
static void EWTrobotpatrol_func_1BF0(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_120* a2);
static void EWTrobotpatrol_func_1C74(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Data_120*);
static void EWTrobotpatrol_func_2060(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_154* a2);
static void EWTrobotpatrol_func_208C(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Data_154*);
void EWTrobotpatrol_func_231C(Object* arg0, ModelInstance* arg1, Gfx** arg2, Mtx** arg3, Vtx** arg4, DLTri** arg5);
s32 EWTrobotpatrol_func_2C38(Vec3f* a0, Vec3f* a1, Vec3f* a2, Vec3f* a3, Object* a4);
static void EWTrobotpatrol_func_2ED4(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_154* arg2);
static void EWTrobotpatrol_func_31E4(EWTrobotpatrol_Data_154*, s32, Object*);
static void EWTrobotpatrol_func_31F4(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_1A8* a2);
static void EWTrobotpatrol_func_3454(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Data_1A8*);
static void EWTrobotpatrol_func_380C(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Data_1A8*, s32);
static void EWTrobotpatrol_func_38C4(Object*, EWTrobotpatrol_Data_80*, u8);
static void EWTrobotpatrol_func_39AC(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_90* arg2);
static int EWTrobotpatrol_func_3AC8(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3);
static void EWTrobotpatrol_func_3B60(EWTrobotpatrol_Data_1D0* a0);
static void EWTrobotpatrol_func_3B70(Object*, EWTrobotpatrol_Data_1D0*, f32, f32, f32, f32);
static void EWTrobotpatrol_func_3D04(Object*, EWTrobotpatrol_Data_1D0*);
static void EWTrobotpatrol_func_4004(f32 arg0, f32 arg1, f32 arg2, s16* arg3, s16* arg4);
static void EWTrobotpatrol_func_40A0(s16 arg0, s16 arg1, Vec3f* arg2, f32 arg3);

// offset: 0x0 | ctor
void EWTrobotpatrol_ctor(void *dll) { }

// offset: 0xC | dtor
void EWTrobotpatrol_dtor(void* dll) {
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
void EWTrobotpatrol_obj_Setup(Object* self, EWTrobotpatrol_Setup* setup, s32 reset) {
    EWTrobotpatrol_Data* objdata;

    if (bss_0 == NULL) {
        bss_0 = texLoadTexture(TEXTABLE_127); // red beam?
    }
    if (bss_4 == NULL) {
        bss_4 = texLoadTexture(TEXTABLE_16F); // noise pattern
    }
    objAddObjectType(self, OBJTYPE_Baddie);
    objdata = self->data;
    self->animCallback = EWTrobotpatrol_func_3AC8;
    bzero(objdata, sizeof(EWTrobotpatrol_Data));
    vox_func_80008DC0(&objdata->unk24);
    objdata->unk1CC = 1;
    objdata->unk1C8 = -0.02f;
    EWTrobotpatrol_func_139C(self, objdata, &objdata->unk90);
    EWTrobotpatrol_func_1BF0(self, objdata, &objdata->unk120);
    EWTrobotpatrol_func_31F4(self, objdata, &objdata->unk1A8);
    EWTrobotpatrol_func_2060(self, objdata, &objdata->unk154);
    objdata->unk4 = -1;
    objdata->unk8 = -1;
    objdata->unk0 = setup->unk1A;
    objdata->unk1E = 0;
    objdata->unk1F = 0;
    objdata->unk20 = 0;
    EWTrobotpatrol_func_3B60(&objdata->unk1D0);
}

// offset: 0x23C | func: 1 | export: 1
void EWTrobotpatrol_obj_Control(Object* self) {
    EWTrobotpatrol_Data* objdata;
    EWTrobotpatrol_Data_90* temp_s1;
    s32 _pad;
    MtxF sp5C;
    f32 sp58;
    SRT sp40;
    s32 sp3C;

    objdata = self->data;
    EWTrobotpatrol_func_3D04(self, &objdata->unk1D0);
    if (objdata->unk1D0.unk4 <= 40) {
        EWTrobotpatrol_func_38C4(self, &objdata->unk80, 2);
        temp_s1 = &objdata->unk90;
        EWTrobotpatrol_func_1414(self, objdata, temp_s1);
        sp40.yaw = self->srt.yaw;
        sp40.pitch = 0;
        sp40.roll = 0;
        sp40.transl.x = 0.0f;
        sp40.transl.y = 0.0f;
        sp40.transl.z = 0.0f;
        sp40.scale = 1.0f;
        mathYprXyzMtx(&sp5C, &sp40);
        mathMtxXFMF(&sp5C, temp_s1->unk3C.x, 0.0f, temp_s1->unk3C.z, &self->velocity.x, &sp58, &self->velocity.z);
        self->srt.transl.y = (mathSinfInterp(temp_s1->unk8C) * temp_s1->unk88) + temp_s1->unk84;
        sp3C = (u16) temp_s1->unk8C + (gUpdateRate << 8);
        if (sp3C >= 0x10000) {
            temp_s1->unk88 = (f32) ((f32) mathRnd(0xF, 0x23) * 0.1f);
        }
        temp_s1->unk8C = (s16) sp3C;
        self->velocity.x *= 1.0666667f;
        self->velocity.z *= 1.0666667f;
        objMove(self, self->velocity.x * gUpdateRateF, 0.0f, self->velocity.z * gUpdateRateF);
        EWTrobotpatrol_func_1C74(self, objdata, &objdata->unk120);
        EWTrobotpatrol_func_3454(self, objdata, &objdata->unk1A8);
        EWTrobotpatrol_func_208C(self, objdata, &objdata->unk154);
        objdata->unk1CC = 0;
    }
}

// offset: 0x4A4 | func: 2 | export: 2
void EWTrobotpatrol_obj_Update(Object* self) {
    Object* hitBy;
    EWTrobotpatrol_Data* objdata;
    s32 damageType;

    damageType = func_80025F40(self, &hitBy, NULL, NULL);
    objdata = self->data;
    if ((damageType == Damage_Type_Projectile) || (damageType == Damage_Type_Explosion)) {
        EWTrobotpatrol_func_3B70(self, &objdata->unk1D0, 
            hitBy->srt.transl.x - self->srt.transl.x, 
            hitBy->srt.transl.y - self->srt.transl.y, 
            hitBy->srt.transl.z - self->srt.transl.z, 
            15.0f);
    }
}

// offset: 0x558 | func: 3 | export: 3
#ifndef NON_MATCHING
void EWTrobotpatrol_obj_Print(Object *self, Gfx **gdl, Mtx **mtxs, Vtx **vtxs, DLTri **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/EWTrobotpatrol_obj_Print.s")
#else
// needs EWTrobotpatrol_func_231C to be static

void EWTrobotpatrol_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** pols, s8 visibility) {
    EWTrobotpatrol_Data* objdata;
    ModelInstance* temp_s2;
    Object* temp_s0;
    MtxF* temp_v0;
    s32 bone;
    s32 _pad;
    f32 temp_fv1_2;
    MtxF sp64;
    u8 sp63;
    u8 sp62;
    u8 sp61;
    u8 var_a0;
    u8 var_a1;
    u8 var_a2;
    u8 sp5D;
    u8 sp5C;
    u8 sp5B;
    ObjectShadow* sp54;
    Vec3f sp48;
    s32 _pad2;
    u8 sp43;
    u8 sp42;
    u8 sp41;
    Vec3f* temp;

    lightGetAmbient(&sp43, &sp42, &sp41);
    objdata = self->data;
    temp_s2 = self->modelInsts[self->modelInstIdx];
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    } else if (!(temp_s2->unk34 & 8)) {
        mod_func_8001943C(self, &sp64, 1.0f, 0.0f);
        mod_func_80019730(temp_s2, temp_s2->model, self, &sp64);
    }
    temp_s0 = objdata->unk1A8.unk4;
    temp = &objdata->unk1A8.unk8;
    bone = self->def->pAttachPoints[0].bones[self->modelInstIdx];
    temp_v0 = (MtxF*) &((f32*)temp_s2->matrices[temp_s2->unk34 & 1])[bone << 4];
    temp_s0->srt.transl.x = temp_v0->m[3][0] + gWorldX;
    temp_s0->srt.transl.y = temp_v0->m[3][1];
    temp_s0->srt.transl.z = temp_v0->m[3][2] + gWorldZ;
    camGetObjectChildPosition(temp_s0, &temp_s0->globalPosition.x, &temp_s0->globalPosition.y, &temp_s0->globalPosition.z);
    if (trackObjVisCheck(temp_s0) != 0) {
        sp54 = temp_s0->shadow;
        switch (objdata->unk1E) {
        case 1:
            var_a0 = 0xD0;
            var_a1 = 0;
            var_a2 = 0;
            break;
        case 2:
            var_a0 = 0xD0;
            var_a1 = 0;
            var_a2 = 0;
            break;
        case 0:
        default:
            var_a0 = 0xBA;
            var_a1 = 0xFF;
            var_a2 = 0xFF;  
            break;
        }
        sp63 = 0xBA;
        sp62 = 0xFF;
        sp61 = 0xFF;
        sp63 += (objdata->unk1A8.unk0 * (f32) (var_a0 - sp63));
        sp62 += (objdata->unk1A8.unk0 * (f32) (var_a1 - sp62));
        sp61 += (objdata->unk1A8.unk0 * (f32) (var_a2 - sp61));
        bone = self->def->pAttachPoints[3].bones[self->modelInstIdx];
        temp_v0 = (MtxF*) &((f32*)temp_s2->matrices[temp_s2->unk34 & 1])[bone << 4];
        sp48.f[0] = temp_v0->m[3][0] + gWorldX;
        sp48.f[1] = temp_v0->m[3][1];
        sp48.f[2] = temp_v0->m[3][2] + gWorldZ;
        sp48.f[0] = temp_s0->srt.transl.x - sp48.f[0];
        sp48.f[1] = temp_s0->srt.transl.y - sp48.f[1];
        sp48.f[2] = temp_s0->srt.transl.z - sp48.f[2];
        temp_fv1_2 = 1.0f / sqrtf(SQ(sp48.f[0]) + SQ(sp48.f[1]) + SQ(sp48.f[2]));
        temp->x = (f32) (sp48.f[0] * temp_fv1_2);
        temp->y = (f32) (sp48.f[1] * temp_fv1_2);
        temp->z = (f32) (sp48.f[2] * temp_fv1_2);
        sp54->dir.x = -temp->x;
        sp54->dir.y = -temp->y;
        sp54->dir.z = -temp->z;
        sp54->tr.x = temp_s0->srt.transl.x;
        sp54->tr.y = temp_s0->srt.transl.y;
        sp54->tr.z = temp_s0->srt.transl.z;
        sp54->flags |= OBJ_SHADOW_FLAG_FADE_OUT;
        sp54->r = sp63;
        sp54->g = sp62;
        sp54->b = sp61;
        temp_s0->prevLocalPosition.x = temp_s0->srt.transl.x;
        temp_s0->prevLocalPosition.y = temp_s0->srt.transl.y;
        temp_s0->prevLocalPosition.z = temp_s0->srt.transl.z;
        temp_s0->srt.yaw = objdata->unk1A8.unk16;
        temp_s0->srt.pitch = 0;
        temp_s0->srt.roll = 0;
        temp_s0->srt.scale = 0.2f;
        temp_s0->opacityWithFade = self->opacityWithFade;
        if (temp_s0->opacityWithFade > 160) {
            temp_s0->opacityWithFade = 160;
        }
        temp_s0->opacityWithFade = (u8) ((temp_s0->opacityWithFade * (temp_s0->opacity + 1)) >> 8);
        sp5D = sp43;
        sp5C = sp42;
        sp5B = sp41;
        sp43 = sp63;
        sp42 = sp62;
        sp41 = sp61;
        objprintDrawModel(temp_s0, gdl, mtxs, vtxs, pols, 1.0f);
        sp43 = sp5D;
        sp42 = sp5C;
        sp41 = sp5B;
        temp_s0->modelInsts[temp_s0->modelInstIdx]->unk34 &= ~0x8;
    }
    EWTrobotpatrol_func_231C(self, temp_s2, gdl, mtxs, vtxs, pols);
}
#endif

// offset: 0xBF4 | func: 4 | export: 4
void EWTrobotpatrol_obj_Free(Object* self, s32 onlySelf) {
    EWTrobotpatrol_Data* objdata;

    objFreeObjectType(self, OBJTYPE_Baddie);
    objdata = self->data;
    vox_func_80008E08(&objdata->unk24);
    gDLL_13_Expgfx->vtbl->func5(self);
    EWTrobotpatrol_func_380C(self, objdata, &objdata->unk1A8, onlySelf);
}

// offset: 0xC9C | func: 5 | export: 5
u32 EWTrobotpatrol_obj_GetModelFlags(Object* self) {
    return MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0xCAC | func: 6 | export: 6
u32 EWTrobotpatrol_obj_GetDataSize(Object *self, u32 offsetAddr) {
    return sizeof(EWTrobotpatrol_Data);
}

// offset: 0xCC0 | func: 7
static void EWTrobotpatrol_func_CC0(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_90* arg2, EWTrobotpatrol_Data_80* arg3) {
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
#ifdef __clang__
        PRAGMA_IGNORE_PUSH("-Wcast-function-type-mismatch")
#else
        PRAGMA_IGNORE_PUSH("-Wcast-function-type")
#endif
        // @fake cast
        ((s32 (*)(Object *, Object *, s32, u32))objdata->unk78)(objdata->unk74, self, 1, 0);
        PRAGMA_IGNORE_POP()
#else
        arg1->unk78(arg1->unk74, arg0, 1, 0);
#endif
        switch (objdata->unk20) {
        case 0:
            EWTrobotpatrol_func_31E4(&objdata2->unk154, 2, player);
            if (EWTrobotpatrol_func_15F8(self, arg2, objdata, 0) != 0) {
                bcopy(&objdata->unk4C, &objdata->unk58, sizeof(objdata->unk58));
                objdata->unk20 = 1;
                objdata->unk18 = 0x12C;
            }
            break;
        case 1:
            EWTrobotpatrol_func_15F8(self, arg2, objdata, 1);
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
            EWTrobotpatrol_func_15F8(self, arg2, objdata, 1);
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
                    objdata2->unk154.unk50 = 3;
                }
                objdata->unk1A = 0x96;
            }
            break;
        }
    } else {
        objdata2->unk154.unk44 = -0.02f;
        if (objdata->unk4 != -1) {
            EWTrobotpatrol_func_39AC(self, objdata, arg2);
            if (objdata->unk1D > 0) {
                objdata->unk1D = objdata->unk1D - 1;
            }
            if ((objdata->unk1D == 0) && (objdata->unk14 < 3.0f)) {
                objdata->unk78(objdata->unk74, self, 0, objdata->unk4);
                objdata->unk1D = 0x78;
            }
        }
        EWTrobotpatrol_func_1164(self, objdata);
        if (objdata->unk1E == 2) {
            dll_amSfx->Play(self, SOUND_112, MAX_VOLUME, NULL, NULL, 0, NULL);
            if (objdata2){} // @fake
            objdata2->unk154.unk44 = 0.02f;
            objdata2->unk1A8.unk1C = 0;
            objdata2->unk1A8.unk0 = 0.0f;
            bcopy(&self->srt.transl, &objdata->unk4C, sizeof(objdata->unk4C));
            objdata->unk1F = 2;
            objdata->unk20 = 0;
        }
    }
}

// offset: 0x1164 | func: 8
static void EWTrobotpatrol_func_1164(Object* self, EWTrobotpatrol_Data* objdata) {
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
static void EWTrobotpatrol_func_139C(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_90* a2) {
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
static void EWTrobotpatrol_func_1414(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_90* arg2) {
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
    EWTrobotpatrol_func_CC0(self, objdata, arg2, &objdata->unk80);
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

// offset: 0x15F8 | func: 11
static s32 EWTrobotpatrol_func_15F8(Object* self, EWTrobotpatrol_Data_90* arg1, EWTrobotpatrol_Data* objdata, s32 arg3) {
    switch (arg3) {
    case 0:
        return EWTrobotpatrol_func_1674(self, arg1, &objdata->unk58);
    case 1:
        return EWTrobotpatrol_func_1920(self, arg1, &objdata->unk58);
    default:
        return 0; 
    }
}

// offset: 0x1674 | func: 12
static s32 EWTrobotpatrol_func_1674(Object* arg0, EWTrobotpatrol_Data_90* arg1, Vec3f* arg2) {
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
static s32 EWTrobotpatrol_func_1920(Object* arg0, EWTrobotpatrol_Data_90* arg1, Vec3f* arg2) {
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
static void EWTrobotpatrol_func_1BF0(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_120* a2) {
    Vec3f sp20[] = {
        VEC3F(9.5f, 4.0f, 0.0f), 
        VEC3F(-9.5f, 4.0f, 0.0f), 
        VEC3F(0.0f, 4.0f, 9.5f), 
        VEC3F(0.0f, 4.0f, -9.5f)
    };

    bcopy(&sp20, a2->unk0, sizeof(sp20));
}

// offset: 0x1C74 | func: 15
static void EWTrobotpatrol_func_1C74(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_120* arg2) {
    s32 i;
    f32 temp_fs0;
    f32 temp_fv1;
    s32 temp_s3;
    Vec3f* temp_s0;
    f32 sp8C[4];
    s32 var_v1;
    Vec3f sp7C;
    SRT sp64;
    TextureAnimator* temp_v0;

    arg2->unk30 = 0;
    if (arg1->unk90.unk0 < 0.0f) {
        arg2->unk30 |= 1;
    } else if (arg1->unk90.unk0 > 0.0f) {
        arg2->unk30 |= 2;
    }
    if (arg1->unk90.unk8 < 0.0f) {
        arg2->unk30 |= 4;
    } else if (arg1->unk90.unk8 > 0.0f) {
        arg2->unk30 |= 8;
    }
    if (arg2->unk30 & 1) {
        sp8C[0] = 3.0f;
    } else {
        sp8C[0] = 0.0f;
    }
    if (arg2->unk30 & 2) {
        sp8C[1] = 3.0f;
    } else {
        sp8C[1] = 0.0f;
    }
    if (arg2->unk30 & 4) {
        sp8C[2] = 3.0f;
    } else {
        sp8C[2] = 0.0f;
    }
    if (arg2->unk30 & 8) {
        sp8C[3] = 3.0f;
    } else {
        sp8C[3] = 0.0f;
    }
    if (arg0->modelInstIdx < 2) {
        for (i = 0; i < 4; i++) {
            if (arg2->unk30 & (1 << i)) {
                temp_s0 = &arg2->unk0[i];
                temp_s3 = mathAtan2f(temp_s0->x, temp_s0->z);
                sp64.transl.x = temp_s0->x;
                sp64.transl.y = temp_s0->y;
                sp64.transl.z = temp_s0->z;
                temp_fs0 = temp_s0->y;
                temp_s0->y = 0.0f;
                temp_fv1 = 1.0f / sqrtf(SQ(sp64.transl.f[0]) + SQ(sp64.transl.f[1]) + SQ(sp64.transl.f[2]));
                sp8C[i] *= 0.01f;
                sp7C.x = temp_s0->x * temp_fv1;
                sp7C.y = temp_s0->y * temp_fv1;
                sp7C.z = temp_s0->z * temp_fv1;
                sp7C.x = sp8C[i] * sp7C.x;
                sp7C.y = sp8C[i] * sp7C.y;
                sp7C.z = sp8C[i] * sp7C.z;
                sp64.yaw = temp_s3 + 0x1000;
                sp64.pitch = 0;
                sp64.roll = 0;
                sp64.scale = 255.0f;
                gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_6B, &sp64, 
                    PARTFXFLAG_4000000 | PARTFXFLAG_2000000 | PARTFXFLAG_10000 | PARTFXFLAG_2, 
                    -1, &sp7C);
                sp64.yaw = temp_s3 - 0x1000;
                gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_6B, &sp64, 
                    PARTFXFLAG_4000000 | PARTFXFLAG_2000000 | PARTFXFLAG_10000 | PARTFXFLAG_2, 
                    -1, &sp7C);
                temp_s0->y = temp_fs0;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        temp_v0 = objExprGetTexAnimator(arg0, i, 0);
        temp_v0->multiplyR = 255;
        temp_v0->multiplyG = 93;
        temp_v0->multiplyB = 0;
        if (arg2->unk30 & (1 << i)) {
            var_v1 = temp_v0->frame + (gUpdateRate * 4);
            if (var_v1 > 0x100) {
                var_v1 = 0x100;
            }
            temp_v0->frame = var_v1;
        } else {
            var_v1 = temp_v0->frame - (gUpdateRate * 4);
            if (var_v1 < 0) {
                var_v1 = 0;
            }
            temp_v0->frame = var_v1;
        }
    }
}

// offset: 0x2060 | func: 16
static void EWTrobotpatrol_func_2060(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_154* a2) {
    a2->unk44 = -0.02f;
    a2->unk4F = 0;
}

// offset: 0x208C | func: 17
static void EWTrobotpatrol_func_208C(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_154* arg2) {
    if (arg2->unk4E != 0) {
        EWTrobotpatrol_func_2ED4(arg0, arg1, arg2);
    }
    arg2->unk4E = arg0->animProgress >= 1.0f;
    arg2->unk51 = gUpdateRate;
    objAnimAdvance(arg0, arg2->unk44, (f32) gUpdateRate, NULL);
}

// offset: 0x2158 | func: 18
void EWTrobotpatrol_func_2158(Object* self, EWTrobotpatrol_Data_154* arg1) {
    s32 angle;
    SRT sp54;
    SRT sp3C;
    DLL_IProjgfx* laserProj;

    angle = arg1->unk4C - (arg1->unk48 & 0xFFFF);
    CIRCLE_WRAP(angle);
    if ((angle >= -0x1000) && (angle <= 0x1000)) {
        dll_amSfx->Play(self, SOUND_115_ScorpionRobot_LaserFire, MAX_VOLUME, NULL, NULL, 0, NULL);
        sp3C.transl.x = arg1->unk20.x;
        sp3C.transl.y = arg1->unk20.y;
        sp3C.transl.z = arg1->unk20.z;
        sp3C.scale = 1.0f;
        sp3C.yaw = 0;
        sp3C.roll = 0;
        sp3C.pitch = 0;
        sp54.transl.x = arg1->unk8.x;
        sp54.transl.y = arg1->unk8.y;
        sp54.transl.z = arg1->unk8.z;
        sp54.scale = 1.0f;
        sp54.yaw = 0;
        sp54.roll = 0;
        sp54.pitch = 0;
        laserProj = dllLoad(DLL_ID_193, 1);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_86, &sp54, PARTFXFLAG_1, -1, NULL);
        laserProj->vtbl->func0(objGetPlayer(), 0, &sp54, 1, -1, 7, &sp3C);
        if (laserProj != NULL) {
            dllFree(laserProj);
        }
    }
}

// offset: 0x231C | func: 19
#ifndef NON_MATCHING
/*0x50*/ static s16 data_50[] = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/437_EWTrobotpatrol/EWTrobotpatrol_func_231C.s")
#else
// https://decomp.me/scratch/TejxV

void EWTrobotpatrol_func_231C(Object* arg0, ModelInstance* arg1, Gfx** arg2, Mtx** arg3, Vtx** arg4, DLTri** arg5) {
    static s16 data_50 = 0;
    Vec3f spE4;
    SRT spCC;
    Object* spC8;
    MtxF* temp_a0;
    f32 spC0;
    Vtx* temp_s2;
    f32 temp_fv0;
    f32 var_fv1_2;
    f32 ft2;
    EWTrobotpatrol_Data_154* temp_s0;
    s32 bone;
    EWTrobotpatrol_Data* spA4;
    f32 var_fa0;
    f32 ft3;
    f32 temp;
    f32 temp2;

    data_50 += 3;
    if (data_50 >= 0x1F) {
        data_50 = 0;
    }
    spA4 = arg0->data;
    temp_s0 = &spA4->unk154;
    if (spA4->unk154.unk4E == 0) {
        return;
    }
    temp_s2 = *arg4;
    spC8 = objGetPlayer();
    bone = arg0->def->pAttachPoints[2].bones[arg0->modelInstIdx];
    temp_a0 = (MtxF*) &((f32*)arg1->matrices[arg1->unk34 & 1])[bone << 4];
    temp_s0->unk14 = temp_a0->m[3][0] + gWorldX;
    temp_s0->unk18 = temp_a0->m[3][1];
    temp_s0->unk1C = temp_a0->m[3][2] + gWorldZ;
    bone = arg0->def->pAttachPoints[1].bones[arg0->modelInstIdx];
    temp_a0 = (MtxF*) &((f32*)arg1->matrices[arg1->unk34 & 1])[bone << 4];
    temp_s0->unk8.x = temp_a0->m[3][0] + gWorldX;
    temp_s0->unk8.y = temp_a0->m[3][1];
    temp_s0->unk8.z = temp_a0->m[3][2] + gWorldZ;
    temp_s0->unk38.x = temp_s0->unk8.x - temp_s0->unk14;
    temp_s0->unk38.y = temp_s0->unk8.y - temp_s0->unk18;
    temp_s0->unk38.z = temp_s0->unk8.z - temp_s0->unk1C;
    temp_fv0 = sqrtf(SQ(temp_s0->unk38.f[0]) + SQ(temp_s0->unk38.f[1]) + SQ(temp_s0->unk38.f[2]));
    if (temp_fv0 != 0.0f) {
        spC0 = 1.0f / temp_fv0;
    } else {
        spC0 = 0.0f;
    }
    temp_s0->unk38.x *= spC0;
    temp_s0->unk38.y *= spC0;
    temp_s0->unk38.z *= spC0;
    spE4.x = temp_s0->unk38.x * 300.0f;
    spE4.y = temp_s0->unk38.y * 300.0f;
    spE4.z = temp_s0->unk38.z * 300.0f;
    spE4.x += temp_s0->unk8.x;
    spE4.y += temp_s0->unk8.y;
    spE4.z += temp_s0->unk8.z;
    EWTrobotpatrol_func_2C38(&temp_s0->unk8, &spE4, &temp_s0->unk20, &temp_s0->unk2C, spC8);
    gDPLoadTextureBlockS((*arg2)++,
        /*timg*/bss_4 + 1,
        /*fmt*/G_IM_FMT_IA,
        /*siz*/G_IM_SIZ_8b,
        /*width*/bss_4->width,
        /*height*/bss_4->height,
        /*pal*/0,
        /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
        /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
        /*masks*/5,
        /*maskt*/5,
        /*shifts*/G_TX_NOLOD,
        /*shiftt*/G_TX_NOLOD            
    );
    gDPTileSync((*arg2)++);
    gDPLoadMultiBlockS((*arg2)++,
        /*timg*/bss_0 + 1,
        /*tmem*/bss_4->sizeBytes >> 3,
        /*rtile*/1,
        /*fmt*/G_IM_FMT_RGBA,
        /*siz*/G_IM_SIZ_32b,
        /*width*/bss_0->width,
        /*height*/bss_0->height,
        /*pal*/0,
        /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
        /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
        /*masks*/5,
        /*maskt*/5,
        /*shifts*/G_TX_NOLOD,
        /*shiftt*/G_TX_NOLOD    
    );
    dlSetPrimColor(arg2, 0xFF, 0xFF, 0xFF, 0x55);
    gDPSetCombineMode(*arg2, G_CC_DINO_PRIM_RGB_INTERFERENCE_A, G_CC_DINO_MODULATERGB_PRIMA2);
    dlApplyCombine(arg2);
    gDPSetOtherMode(*arg2, 
        G_AD_PATTERN | G_CD_NOISE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_ZB_CLD_SURF2);
    dlApplyOtherMode(arg2);
    spC0 = sqrtf(SQ(temp_s0->unk38.x) + SQ(temp_s0->unk38.z));
    spCC.yaw = mathAtan2f(temp_s0->unk38.x, temp_s0->unk38.z);
    spCC.pitch = -mathAtan2f(temp_s0->unk38.y, spC0);
    spCC.roll = 0;
    spCC.transl.x = temp_s0->unk8.x;
    spCC.transl.y = temp_s0->unk8.y;
    spCC.transl.z = temp_s0->unk8.z;
    spCC.scale = 0.1f;
    camSetupObjectSRTMatrix(arg2, arg3, &spCC, 1.0f, 0.0f, NULL);
    bcopy(data_0, *arg5, sizeof(data_0));
    gSPVertex((*arg2)++, OS_PHYSICAL_TO_K0(*arg4), 4, 0);
    dlTriangles(arg2, *arg5, 2);
    spC0 = sqrtf(SQ(temp_s0->unk20.x - temp_s0->unk8.x) + SQ(temp_s0->unk20.z - temp_s0->unk8.z));
    temp_fv0 = mathCosfInterp(spCC.pitch);
    ft2 = 0.0f;
    ft3 = 0.0f;
    temp = 0.0f;
    temp2 = 0.0f;
    if (temp_fv0 != 0.0f) {
        var_fv1_2 = spC0 / temp_fv0;
    } else {
        var_fv1_2 = 0.0f;
    }
    var_fv1_2 *= 10.0f;
    temp_s2->v.ob[0] = (s32)temp/*f6*/;
    temp_s2->v.ob[1] = (s32)ft2/*f8*/ + 14;
    temp_s2->v.ob[2] = (s32)ft3/*f10*/;
    temp_s2->v.cn[0] = 0xFF;
    temp_s2->v.cn[1] = 0;
    temp_s2->v.cn[2] = 0;
    temp_s2->v.cn[3] = 0xCD;
    temp_s2++;

    temp_s2->v.ob[0] = (s32)ft3/*f10*/;
    temp_s2->v.ob[2] = (s32)ft3/*f10*/;
    temp_s2->v.ob[1] = (s32)temp2/*f10*/ - 14;
    temp_s2->v.cn[0] = 0xFF;
    temp_s2->v.cn[1] = 0;
    temp_s2->v.cn[2] = 0;
    temp_s2->v.cn[3] = 0xCD;
    temp_s2++;

    temp_s2->v.ob[0] = (s32)ft3/*f18*/;
    temp_s2->v.ob[1] = (s32)ft2/*f8*/ + 14;
    temp_s2->v.ob[2] = var_fv1_2;
    temp_s2->v.cn[3] = 0x69;
    temp_s2->v.cn[0] = 0xFF;
    temp_s2->v.cn[1] = 0;
    temp_s2->v.cn[2] = 0;
    temp_s2++;

    temp_s2->v.ob[0] = (s32)ft3/*f18*/;
    temp_s2->v.ob[1] = (s32)temp2/*f10*/ - 14;
    temp_s2->v.ob[2] = var_fv1_2;
    temp_s2->v.cn[3] = 0x69;
    temp_s2->v.cn[0] = 0xFF;
    temp_s2->v.cn[1] = 0;
    temp_s2->v.cn[2] = 0;
    temp_s2++;
    spCC.roll = 0x4000;
    camSetupObjectSRTMatrix(arg2, arg3, &spCC, 1.0f, 0.0f, NULL);
    gSPVertex((*arg2)++, OS_PHYSICAL_TO_K0(*arg4), 4, 0);
    dlTriangles(arg2, *arg5, 2);
    *arg4 = temp_s2;
    *arg5 += 2;
    if (temp_s0->unk50 > 0) {
        EWTrobotpatrol_func_2158(arg0, temp_s0);
        temp_s0->unk50 = 0;
    }
    texRenderReset();
    lightAmbientDL(arg2);
}
#endif

// offset: 0x2C38 | func: 20
s32 EWTrobotpatrol_func_2C38(Vec3f* a0, Vec3f* a1, Vec3f* a2, Vec3f* a3, Object* a4) {
    Vec3s16 sp90;
    Vec3s16 sp88;
    Vec3s16 sp80;
    f32 var_fv1;
    s32 _pad;
    s32 _pad2;
    Vec3f sp68;
    Vec3f sp5C;
    Vec3f sp50;
    Vec3f sp44;
    s8 _pad_sp43;
    s8 sp42;
    s32 _pad3;
    f32 sp38;
    s32 _pad4;

    sp44.f[0] = a1->f[0] - a0->f[0];
    sp44.f[1] = a1->f[1] - a0->f[1];
    sp44.f[2] = a1->f[2] - a0->f[2];
    var_fv1 = sqrtf(SQ(sp44.f[0]) + SQ(sp44.f[1]) + SQ(sp44.f[2]));
    if (var_fv1 != 0.0f) {
        var_fv1 = 1.0f / var_fv1;
    }
    sp44.f[0] *= var_fv1;
    sp44.f[1] *= var_fv1;
    sp44.f[2] *= var_fv1;
    if (a3){} // @fake
    vox_func_80007EE0(a0, &sp90);
    vox_func_80007EE0(a1, &sp88);
    sp5C.f[0] = a1->f[0];
    sp5C.f[1] = a1->f[1];
    sp5C.f[2] = a1->f[2];
    sp38 = 1.0f;
    if (a4 != NULL) {
        sp42 = func_8002AD3C(a4, a0, a1, &sp5C, &sp38);
    } else {
        sp42 = 0;
    }
    a2->f[0] = a1->f[0];
    a2->f[1] = a1->f[1];
    a2->f[2] = a1->f[2];
    a3->f[0] = a1->f[0];
    a3->f[1] = a1->f[1];
    a3->f[2] = a1->f[2];
    if (vox_func_80008048(&sp90, &sp88, &sp80, NULL, 0) == 0) {
        vox_func_80007E2C(&sp68, &sp80);
        sp44.f[0] *= 20.0f;
        sp44.f[1] *= 20.0f;
        sp44.f[2] *= 20.0f;
    }
    // FAKE?
    sp44.f[0] *= 1.0f;
    sp44.f[1] *= 1.0f;
    sp44.f[2] *= 1.0f;
    // @bug: sp50 is uninitialized!!
    a3->f[1] = sp50.f[1];
    a3->f[2] = sp50.f[2];
    if (sp42 != 0) {
        if (sp38 < 1.0f) {
            a2->f[0] = sp5C.f[0];
            a2->f[1] = sp5C.f[1];
            a2->f[2] = sp5C.f[2];
            return 1;
        } else {
            a2->f[0] = sp50.f[0];
            a2->f[1] = sp50.f[1];
            a2->f[2] = sp50.f[2];
            return 2;
        }
    }
    return 0;
}

// offset: 0x2ED4 | func: 21
static void EWTrobotpatrol_func_2ED4(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_154* arg2) {
    s32 sp54;
    s32 var_a0;
    s32 var_a1;
    s32 sp48;
    s32 _pad;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    SeqJoint* sp34;
    SeqJoint* sp30;
    Vec3f sp24;
    s32 var_a0_2;
    s32 var_a1_2;

    sp34 = objExpr_func_80034804(arg0, 0);
    sp30 = objExpr_func_80034804(arg0, 1);
    if (arg2->unk0 != NULL) {
        sp24.f[0] = arg2->unk0->velocity.f[0] * 25.0f;
        sp24.f[1] = arg2->unk0->velocity.f[1] * 25.0f;
        sp24.f[2] = arg2->unk0->velocity.f[2] * 25.0f;
        sp24.f[0] += arg2->unk0->srt.transl.f[0];
        sp24.f[1] += arg2->unk0->srt.transl.f[1];
        sp24.f[2] += arg2->unk0->srt.transl.f[2];
        sp40 = sp24.f[0] - arg2->unk14;
        sp3C = sp24.f[1] - arg2->unk18;
        sp38 = sp24.f[2] - arg2->unk1C;
        var_a1 = -mathAtan2f(sp3C, sqrtf(SQ(sp40) + SQ(sp38)));
    } else {
        var_a1 = arg2->unk4A;
    }
    sp54 = arg2->unk4C - (arg2->unk48 & 0xFFFF);
    CIRCLE_WRAP(sp54);
    sp48 = var_a1 - (arg2->unk4A & 0xFFFF);
    CIRCLE_WRAP(sp48);
    switch (arg2->unk4F) {
    case 2:
        arg2->unk4C = mathAtan2f(-sp40, -sp38);
        var_a0_2 = 0x800;
        var_a1_2 = 0x800;
        break;
    case 1:
        if ((sp54 >= -0xFF) && (sp54 < 0x100)) {
            arg2->unk4C = mathRnd(0, 0xFFFF);
        }
        var_a0_2 = 0x200;
        var_a1_2 = 0x200;
        break;
    case 0:
    default:
        var_a0_2 = 0;
        var_a1_2 = 0;
        break;
    }
    sp54 *= gUpdateRate;
    sp54 >>= 3;
    if (sp54 > var_a0_2) {
        sp54 = var_a0_2;
    }
    if (sp54 < -var_a0_2) {
        sp54 = -var_a0_2;
    }
    arg2->unk48 += sp54;
    sp48 *= gUpdateRate;
    sp48 >>= 2;
    if (sp48 > var_a1_2) {
        sp48 = var_a1_2;
    }
    if (sp48 < -var_a1_2) {
        sp48 = -var_a1_2;
    }
    arg2->unk4A += sp48;
    sp34->yaw = arg2->unk48;
    sp30->pitch = 0x238C - arg2->unk4A;
}

// offset: 0x31E4 | func: 22
static void EWTrobotpatrol_func_31E4(EWTrobotpatrol_Data_154* arg0, s32 arg1, Object* arg2) {
    arg0->unk4F = arg1;
    arg0->unk0 = arg2;
}

// offset: 0x31F4 | func: 23
static void EWTrobotpatrol_func_31F4(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Data_1A8* a2) {
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
static void EWTrobotpatrol_func_3454(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_1A8* arg2) {
    Object* temp_s1;
    s32 _pad;
    Vec3f sp6C;
    Vec3f sp60;
    Vec3s16 sp58;
    Vec3s16 sp50;
    Vec3s16 sp48;
    ObjectShadow* sp44;
    f32 temp_fv1;
    s32 var_v0;

    temp_s1 = arg2->unk4;
    sp44 = temp_s1->shadow;
    sp60.f[0] = -sp44->dir.x;
    sp60.f[1] = -sp44->dir.f[1];
    sp60.f[2] = -sp44->dir.f[2];
    sp6C.f[0] = sp60.f[0] * 200.0f;
    sp6C.f[1] = sp60.f[1] * 200.0f;
    sp6C.f[2] = sp60.f[2] * 200.0f;
    sp6C.f[0] += temp_s1->srt.transl.f[0];
    sp6C.f[1] += temp_s1->srt.transl.f[1];
    sp6C.f[2] += temp_s1->srt.transl.f[2];
    vox_func_80007EE0(&temp_s1->srt.transl, &sp58);
    vox_func_80007EE0(&sp6C, &sp50);
    if (vox_func_80008048(&sp58, &sp50, &sp48, NULL, 0) == 0) {
        vox_func_80007E2C(&sp6C, &sp48);
        sp6C.f[0] -= temp_s1->srt.transl.f[0];
        sp6C.f[1] -= temp_s1->srt.transl.f[1];
        sp6C.f[2] -= temp_s1->srt.transl.f[2];
        temp_fv1 = sqrtf(SQ(sp6C.f[0]) + SQ(sp6C.f[1]) + SQ(sp6C.f[2])) - 10.0f;
        sp6C.f[0] = sp60.f[0] * temp_fv1;
        sp6C.f[1] = sp60.f[1] * temp_fv1;
        sp6C.f[2] = sp60.f[2] * temp_fv1;
        sp44->tr.x = sp6C.f[0] + temp_s1->srt.transl.x;
        sp44->tr.y = sp6C.f[1] + temp_s1->srt.transl.y;
        sp44->tr.z = sp6C.f[2] + temp_s1->srt.transl.z;
    } else {
        sp44->tr.x = temp_s1->srt.transl.x;
        sp44->tr.y = temp_s1->srt.transl.y;
        sp44->tr.z = temp_s1->srt.transl.z;
    }
    var_v0 = arg2->unk18 - (arg2->unk16 & 0xFFFF);
    CIRCLE_WRAP(var_v0);
    var_v0 *= gUpdateRate;
    var_v0 >>= 4;
    if (var_v0 > 0x800) {
        var_v0 = 0x800;
    }
    if (var_v0 < -0x800) {
        var_v0 = -0x800;
    }
    arg2->unk16 = arg2->unk16 + var_v0;
    objExpr_func_80034804(arg0, 2)->yaw = arg2->unk16;
    arg2->unk14 += gUpdateRate * 4;
    if (arg2->unk14 >= 0x400) {
        arg2->unk14 = 0;
    }
    if (arg2->unk1C == 0) {
        arg2->unk0 += 0.02f * (f32) gUpdateRate;
    } else {
        arg2->unk0 -= 0.02f * (f32) gUpdateRate;
    }
    if (arg2->unk0 < 0.0f) {
        arg2->unk0 = 0.0f;
        arg2->unk1C ^= 1;
    } else if (arg2->unk0 > 1.0f) {
        arg2->unk0 = 1.0f;
        arg2->unk1C ^= 1;
    }
    objExprGetTexAnimator(temp_s1, 0, 0)->positionV = arg2->unk14;
}

// offset: 0x380C | func: 25
static void EWTrobotpatrol_func_380C(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_1A8* arg2, s32 arg3) {
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
static void EWTrobotpatrol_func_38C4(Object* arg0, EWTrobotpatrol_Data_80* arg1, u8 arg2) {
    arg1->unkE = joyGetStickX(arg2);
    arg1->unkF = -joyGetStickY(arg2);
    arg1->unkC = joyGetButtons(arg2);
    arg1->unk8 = joyGetPressed(arg2);
    arg1->unkA = joyGetReleased(arg2);
    arg1->unk0 = (f32) arg1->unkE;
    arg1->unk4 = (f32) arg1->unkF;
}

// offset: 0x39AC | func: 27
static void EWTrobotpatrol_func_39AC(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Data_90* arg2) {
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
        EWTrobotpatrol_func_15F8(arg0, arg2, arg1, 1);
        temp_fv0 = sp2C->pos.x - arg0->srt.transl.x;
        temp_fv1 = sp2C->pos.z - arg0->srt.transl.z;
        sp28->unk1A8.unk18 = mathAtan2f(-temp_fv0, -temp_fv1);
        temp_fv0 = arg0->srt.transl.x - sp2C->pos.x;
        temp_fv1 = arg0->srt.transl.z - sp2C->pos.z;
        arg1->unk14 = sqrtf(SQ(temp_fv0) + SQ(temp_fv1));
    }
}

// offset: 0x3AC8 | func: 28
static int EWTrobotpatrol_func_3AC8(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3) {
    animObjData->unk62 = 0;
    return 0;
}

// offset: 0x3AE4 | func: 29 | export: 7
void EWTrobotpatrol_Func_3AE4(Object* self, Object* base, EWTrobotpatrolCallback callback) {
    EWTrobotpatrol_Data* objdata = self->data;
    objdata->unk74 = base;
    objdata->unk78 = callback;
}

// offset: 0x3AF8 | func: 30 | export: 8
void EWTrobotpatrol_Func_3AF8(Object* arg0, s32 arg1) {
    EWTrobotpatrol_Data* objdata = arg0->data;
    objdata->unk8 = objdata->unk4;
    objdata->unk4 = arg1;
    objdata->unk1D = 0;
    objdata->unk14 = 1000.0f;
    bzero(&objdata->unk90.unk3C, sizeof(objdata->unk90.unk3C));
}

// offset: 0x3B60 | func: 31
static void EWTrobotpatrol_func_3B60(EWTrobotpatrol_Data_1D0* a0) {
    a0->unk4 = 0;
    a0->unk38 = 0;
}

// offset: 0x3B70 | func: 32
static void EWTrobotpatrol_func_3B70(Object* arg0, EWTrobotpatrol_Data_1D0* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    s32 var_v1;
    SRT sp2C;

    if ((arg2 != 0.0f) || (arg3 != 0.0f) || (arg4 != 0.0f)) {
        guNormalize(&arg2, &arg3, &arg4);
    }
    arg1->unk36 = mathRnd(8, 12);
    EWTrobotpatrol_func_4004(arg2, arg3, arg4, &arg1->unk6[0], &arg1->unk1E[0]);
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
    gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_35B, &sp2C, PARTFXFLAG_NONE, -1, NULL);
}

// offset: 0x3D04 | func: 33
static void EWTrobotpatrol_func_3D04(Object* arg0, EWTrobotpatrol_Data_1D0* arg1) {
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
                EWTrobotpatrol_func_40A0(arg1->unk6[var_s1], arg1->unk1E[var_s1], &sp80.transl, arg1->unk0);
                gDLL_17_partfx->vtbl->spawn(arg0, (var_s1 % 2) + PARTICLE_35C, &sp80, PARTFXFLAG_NONE, -1, NULL);
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
static void EWTrobotpatrol_func_4004(f32 arg0, f32 arg1, f32 arg2, s16* arg3, s16* arg4) {
    f32 sp24 = sqrtf(SQ(arg0) + SQ(arg2));
    *arg3 = mathAtan2f(arg2, arg0);
    *arg4 = mathAtan2f(sp24, arg1);
}

// offset: 0x40A0 | func: 35
static void EWTrobotpatrol_func_40A0(s16 arg0, s16 arg1, Vec3f* arg2, f32 arg3) {
    arg2->x = (mathSinfInterp(arg1) * mathCosfInterp(arg0) * arg3);
    arg2->y = (mathCosfInterp(arg1) * arg3) + 3.0f;
    arg2->z = (mathSinfInterp(arg1) * mathSinfInterp(arg0) * arg3);
}
