#include "common.h"
#include "dlls/objects/210_player.h"
#include "sys/objtype.h"
#include "sys/objmsg.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    u8 unk19[0x1A - 0x19];
    s16 unk1A;
    u8 unk1C[0x1E - 0x1C];
    s16 unk1E;
} RobotAnimPatrol_Setup;

typedef struct {
    f32 unk0;
    s16 unk4[12];
    s16 unk1C[12];
    u8 unk34[0x36 - 0x34];
    s16 unk36;
    s16 unk38;
    s16 unk3A;
} RobotAnimPatrol_Data_31C;

typedef struct {
    Object* unk0;
    Object* unk4;
    Object* unk8;
    Object* unkC;
    s8 unk10;
    s8 unk11;
    s8 unk12;
    u8 _unk13[0x18 - 0x13];
    Vec3f unk18;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    u8 _unk30[0x34 - 0x30];
    s16 unk34;
    s16 unk36;
    s16 unk38;
    u8 _unk3A[0x3C - 0x3A];
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    u32 unk44;
    u32 unk48;
    u32 unk4C;
    u8 unk50;
    u8 unk51;
    u8 unk52;
    u8 unk53;
    u8 _unk54[0x6C - 0x54];
    f32 unk6C[3];
    u8 _unk78[0x84 - 0x78];
    f32 unk84;
    f32 unk88;
    f32 unk8C;
    u8 _unk90[0x9C - 0x90];
    s32 unk9C;
    s32 unkA0;
    s16 unkA4;
    s16 unkA6;
    u8 _unkA8;
    u8 unkA9;
    u8 unkAA;
    u8 unkAB;
    f32 unkAC;
    f32 unkB0;
    f32 unkB4;
    f32 unkB8;
    DLL27_Data unkBC;
    RobotAnimPatrol_Data_31C unk31C;
    u8 unk358;
    u8 unk359;
} RobotAnimPatrol_Data;

/*0x0*/ static u32 data_0[] = {
    0x40000201, 0x00000000, 0x00000000, 0x00000000, 
    0x40020103, 0x00000000, 0x00000000, 0x00000000
};
/*0x20*/ static Vec3f data_20[] = {
    VEC3F(0.0f, 0.0f, 0.0f)
};
/*0x2C*/ static f32 data_2C[] = {7.0f};

/*0x0*/ static Texture* bss_0; // red laser beam
/*0x4*/ static Texture* bss_4; // blue laser beam
/*0x8*/ static Texture* bss_8; // blue fireball?

int dll_381_func_BC4(Object* self, Object* animObj, AnimObj_Data* animObjData, s8);
void dll_381_func_2E9C(RobotAnimPatrol_Data_31C*);
static int dll_381_func_16FC(Object* self);
static void dll_381_func_2EAC(Object*, RobotAnimPatrol_Data_31C*, f32, f32, f32, f32);
static void dll_381_func_2890(Object*, RobotAnimPatrol_Data*, s32);
static s32 dll_381_func_3040(Object*, RobotAnimPatrol_Data_31C*);
static void dll_381_func_334C(f32 arg0, f32 arg1, f32 arg2, s16* arg3, s16* arg4);
static void dll_381_func_33E8(s16 arg0, s16 arg1, Vec3f* arg2, f32 arg3);
void dll_381_func_1F60(Object*, ModelInstance*, Gfx**, Mtx**, Vtx**, DLTri**);

// offset: 0x0 | ctor
void dll_381_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_381_dtor(void* dll) {
    if (bss_0 != NULL) {
        texFreeTexture(bss_0);
        bss_0 = NULL;
    }
    if (bss_4 != NULL) {
        texFreeTexture(bss_4);
        bss_4 = NULL;
    }
    if (bss_8 != NULL) {
        texFreeTexture(bss_8);
        bss_8 = NULL;
    }
}

// offset: 0xB0 | func: 0 | export: 0
#ifndef NON_MATCHING
void dll_381_obj_Setup(Object* self, ObjSetup* setup, s32 reset);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/dll_381_obj_Setup.s")
#else
void dll_381_obj_Setup(Object* self, RobotAnimPatrol_Setup* setup, s32 reset) {
    RobotAnimPatrol_Data* temp_s0;
    ObjSetup* temp_v0;
    Object* temp_v0_2;
    ObjectShadow* temp_v0_3;
    s32 _pad[7];
    u8 sp3B;

    temp_s0 = self->data;
    sp3B = 5;
    temp_s0->unk358 = 0;
    if ((setup->unk1E != -1) && (mainGetBits(setup->unk1E) != 0)) {
        temp_s0->unk358 |= 1;
        return;
    }
    temp_s0->unk358 &= ~0x1;
    objInitMesgQueue(self, 4);
    self->animCallback = dll_381_func_BC4;
    if (bss_0 == NULL) {
        bss_0 = texLoadTexture(TEXTABLE_127);
    }
    if (bss_4 == NULL) {
        bss_4 = texLoadTexture(TEXTABLE_16B);
    }
    if (bss_8 == NULL) {
        bss_8 = texLoadTexture(TEXTABLE_16E);
    }
    if (temp_s0->unk0 == NULL) {
        temp_v0 = objAllocSetup(sizeof(ObjSetup), OBJ_RobotBeam2);
        temp_v0->x = self->srt.transl.x;
        temp_v0->y = self->srt.transl.y;
        temp_v0->z = self->srt.transl.z;
        temp_v0->objId = OBJ_RobotBeam2;
        temp_v0->loadFlags = 2;
        temp_v0->byte5 = 4;
        temp_v0->fadeDistance = 0xFF;
        temp_v0_2 = objSetupObject(temp_v0, 0, -1, -1, self->parent);
        temp_s0->unk0 = temp_v0_2;
        temp_v0_2->srt.scale = 0.35f;
    }
    camGetObjectChildPosition(temp_s0->unk0, 
        &temp_s0->unk0->globalPosition.x, 
        &temp_s0->unk0->globalPosition.y, 
        &temp_s0->unk0->globalPosition.z);
    temp_v0_3 = temp_s0->unk0->shadow;
    if (temp_v0_3 != NULL) {
        temp_v0_3->flags |= 0x50;
        temp_v0_3->r = 0xC0;
        temp_v0_3->g = 0xFF;
        temp_v0_3->b = 0xFF;
        temp_v0_3->a = 0x1E;
        temp_v0_3->scale = 3.5f;
    }
    temp_s0->unk18.x = setup->base.x;
    temp_s0->unk18.y = setup->base.y;
    temp_s0->unk18.z = setup->base.z;
    temp_s0->unk36 = setup->unk18 << 8;
    temp_s0->unk38 = setup->unk1A;
    temp_s0->unk24 = 0.2f;
    temp_s0->unk3C = 0;
    temp_s0->unk52 = 0;
    temp_s0->unk40 = mathRnd(240, 600);
    temp_s0->unk4 = 0;
    temp_s0->unk10 = 0;
    temp_s0->unk11 = 0;
    temp_s0->unk51 = 0;
    temp_s0->unk50 = 1;
    temp_s0->unkA0 = self->srt.yaw;
    temp_s0->unk9C = 0;
    temp_s0->unkA4 = 0;
    temp_s0->unk34 = 0x400;
    temp_s0->unkAA = 0;
    temp_s0->unkAB = 0;
    temp_s0->unk359 = 0;
    temp_s0->unk8 = 0;
    temp_s0->unkC = 0;
    temp_s0->unk2C = 2.0f;
    temp_s0->unkB4 = 0.0f;
    temp_s0->unkB8 = 0.0f;
    objAnimSet(self, 0, 1.0f, 0);
    gDLL_27->vtbl->init(&temp_s0->unkBC, 0, 0x40083, 1);
    gDLL_27->vtbl->setup_terrain_collider(&temp_s0->unkBC, 1, data_20, data_2C, &sp3B);
    objAddObjectType(self, OBJTYPE_24);
    objAddObjectType(self, OBJTYPE_Baddie);
    dll_381_func_2E9C(&temp_s0->unk31C);
    func_8002674C(self);
}
#endif

// offset: 0x484 | func: 1 | export: 1
void dll_381_obj_Control(Object* self) {
    RobotAnimPatrol_Data* temp_a3 = self->data;
    
    diPrintf(" ROBO not Overriden ");
    if (!(temp_a3->unk358 & 1)) {
        temp_a3->unk50 = gUpdateRate;
        if (temp_a3->unkAB) {
            dll_381_func_2890(self, temp_a3, temp_a3->unkAB);
        } else {
            if (self->unkE0 == 0) {
                self->srt.transl.x = temp_a3->unk18.x;
                self->srt.transl.y = temp_a3->unk18.y;
                self->srt.transl.z = temp_a3->unk18.z;
                self->srt.yaw = temp_a3->unk36;
                self->unkE0 = 1;
            }
            dll_381_func_3040(self, &temp_a3->unk31C);
        }
    }
}

static const char str_1[] = "RobotAnimPatrol Beam Error!!\n";

// offset: 0x56C | func: 2 | export: 2
void dll_381_obj_Update(Object* self) {
    RobotAnimPatrol_Data* temp_s0;
    Object* sp48;
    Object* sp44;
    s32 temp_v0;

    temp_s0 = self->data;
    temp_v0 = func_80025F40(self, &sp44, NULL, NULL);
    if (!(temp_s0->unk358 & 1)) {
        if ((temp_v0 == 0xF) || (temp_v0 == 5)) {
            dll_381_func_2EAC(self, &temp_s0->unk31C, 
                sp44->srt.transl.x - self->srt.transl.x, 
                sp44->srt.transl.y - self->srt.transl.y, 
                sp44->srt.transl.z - self->srt.transl.z, 
                15.0f);
            if (temp_s0->unk4C != 0) {
                gDLL_6_AMSFX->vtbl->Stop(temp_s0->unk4C);
            }
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_B16, MAX_VOLUME, &temp_s0->unk4C, NULL, 0, NULL);
        }
        sp48 = objGetPlayer();
        if (sp48 != NULL) {
            temp_s0->unk50 = gUpdateRate;
            temp_s0->unk10 = dll_381_func_16FC(self);
            if ((temp_s0->unk10 != 0) && (temp_s0->unk11 == 0) && (((DLL_210_Player*)sp48->dll)->vtbl->func66(sp48, 1) != 0)) {
                temp_s0->unk12 = 1;
                temp_s0->unkA9 = 1;
                temp_s0->unk4 = sp48;
            }
            temp_s0->unk11 = temp_s0->unk10;
        }
    }
}

// offset: 0x740 | func: 3 | export: 3
#ifndef NON_MATCHING
void dll_381_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/dll_381_obj_Print.s")
#else
// needs dll_381_func_1F60 to be static

void dll_381_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** pols, s8 visibility) {
    RobotAnimPatrol_Data* temp_s1;
    ModelInstance* temp_s2;
    s32 _pad;
    MtxF* temp_v0;
    MtxF sp48;
    f32* temp_a1;
    Object* temp_a0;
    s32 bone;
    ObjectShadow* temp_v0_2;

    temp_s1 = self->data;
    if (!(temp_s1->unk358 & 1)) {
        temp_s2 = self->modelInsts[self->modelInstIdx];
        if (visibility != 0) {
            objprintDrawModel(self, gdl, mtxs, (Vertex** ) vtxs, (Triangle** ) pols, 1.0f);
        } else if (!(temp_s2->unk34 & 8)) {
            mod_func_8001943C(self, &sp48, 1.0f, 0.0f);
            mod_func_80019730(temp_s2, temp_s2->model, self, &sp48);
        }
        temp_a0 = temp_s1->unk0;
        if (temp_a0 != NULL) {
            if (temp_a0->stateFlags & 0x40) {
                temp_s1->unk0 = NULL;
                return;
            }
            bone = self->def->pAttachPoints[0].bones[self->modelInstIdx];
            temp_v0 = (MtxF*) &((f32*)temp_s2->matrices[temp_s2->unk34 & 1])[bone << 4];
            temp_a0->srt.transl.x = temp_v0->m[3][0] + gWorldX;
            temp_a0->srt.transl.y = temp_v0->m[3][1];
            temp_a0->srt.transl.z = temp_v0->m[3][2] + gWorldZ;
            camGetObjectChildPosition(temp_a0, &temp_a0->globalPosition.x, &temp_a0->globalPosition.y, &temp_a0->globalPosition.z);
            if (trackObjVisCheck(temp_a0) != 0) {
                temp_v0_2 = temp_a0->shadow;
                temp_a0->prevLocalPosition.x = temp_a0->srt.transl.x;
                temp_a0->prevLocalPosition.y = temp_a0->srt.transl.y;
                temp_a0->prevLocalPosition.z = temp_a0->srt.transl.z;
                temp_a0->srt.yaw = self->srt.yaw;
                temp_a0->srt.pitch = self->srt.pitch + 0x3500;
                temp_a0->srt.roll = 0;
                temp_a1 = temp_s1->unk6C;
                temp_a0->opacityWithFade = self->opacityWithFade;
                if (temp_a0->opacityWithFade > 160) {
                    temp_a0->opacityWithFade = 160;
                }
                temp_a1[0] = 0.0f;
                temp_a1[1] = 1.0f;
                temp_a1[2] = 0.0f;
                mathRotateRPY(&temp_a0->srt, temp_a1);
                temp_v0_2->dir.x = temp_a1[0];
                temp_v0_2->dir.y = temp_a1[1];
                temp_v0_2->dir.z = temp_a1[2];
                temp_s1->unk84 = temp_a1[0];
                temp_s1->unk88 = temp_a1[1];
                temp_s1->unk8C = temp_a1[2];
                temp_v0_2->tr.x = temp_a0->srt.transl.x - (temp_a1[0] * 20.0f);
                temp_v0_2->tr.y = temp_a0->srt.transl.y - (temp_a1[1] * 20.0f);
                temp_v0_2->tr.z = temp_a0->srt.transl.z - (temp_a1[2] * 20.0f);
                objprintDrawModel(temp_a0, gdl, mtxs, (Vertex** ) vtxs, (Triangle** ) pols, 1.0f);
                temp_s1->unk0->modelInsts[0]->unk34 &= ~0x8;
            }
            dll_381_func_1F60(self, temp_s2, gdl, mtxs, vtxs, pols);
        }
    }
}
#endif

// offset: 0xA74 | func: 4 | export: 4
void dll_381_obj_Free(Object* self, s32 onlySelf) {
    RobotAnimPatrol_Data* objdata;

    objdata = self->data;
    if (onlySelf == 0) {
        if (objdata->unk0 != NULL) {
            objFreeObject(objdata->unk0);
        }
        if (objdata->unk8 != NULL) {
            objFreeObject(objdata->unk8);
        }
        if (objdata->unkC != NULL) {
            objFreeObject(objdata->unkC);
        }
    }
    if (objdata->unk48 != 0) {
        gDLL_6_AMSFX->vtbl->Stop(objdata->unk48);
    }
    if (objdata->unk44 != 0) {
        gDLL_6_AMSFX->vtbl->Stop(objdata->unk44);
    }
    objFreeObjectType(self, OBJTYPE_24);
    objFreeObjectType(self, OBJTYPE_Baddie);
}

// offset: 0xBA0 | func: 5 | export: 5
u32 dll_381_obj_GetModelFlags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW;
}

// offset: 0xBB0 | func: 6 | export: 6
u32 dll_381_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(RobotAnimPatrol_Data);
}

// offset: 0xBC4 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/dll_381_func_BC4.s")

// offset: 0x16FC | func: 8
static int dll_381_func_16FC(Object* self) {
    Object* player;
    Vec3f sp68;
    Vec3f sp5C;
    Vec3f sp50;
    f32 temp_fa1;
    f32 temp_fv0_2;
    f32 var_fa0;
    RobotAnimPatrol_Data* objdata;
    SRT sp28;

    player = objGetPlayer();
    if (player == NULL) {
        return 0;
    }
    sp50.f[0] = player->srt.transl.x;
    sp50.f[1] = player->srt.transl.y + 35.0f;
    sp50.f[2] = player->srt.transl.z;
    objdata = self->data;
    sp68.f[0] = sp50.f[0] - self->srt.transl.x;
    sp68.f[1] = sp50.f[1] - self->srt.transl.y;
    sp68.f[2] = sp50.f[2] - self->srt.transl.z;
    if (sqrtf(SQ(sp68.f[0]) + SQ(sp68.f[1]) + SQ(sp68.f[2])) < 300.0f) {
        sp5C.x = sp50.f[0] - objdata->unk0->srt.transl.x;
        sp5C.y = sp50.f[1] - objdata->unk0->srt.transl.y;
        sp5C.z = sp50.f[2] - objdata->unk0->srt.transl.z;
        temp_fa1 = sqrtf(SQ(objdata->unk84) + SQ(objdata->unk8C));
        sp28.yaw = -objdata->unk0->srt.yaw;
        sp28.pitch = mathAtan2f(objdata->unk88, temp_fa1);
        sp28.roll = 0;
        mathRotateYPR(&sp28, &sp5C);
        temp_fv0_2 = sqrtf(SQ(sp5C.x) + SQ(sp5C.y));
        var_fa0 = (sp5C.y * 50.0f) / 35.0f;
        if (var_fa0 > 50.0f) {
            var_fa0 = 50.0f;
        }
        return temp_fv0_2 <= var_fa0 && sp5C.z < 0.0f;
    }
    return 0;
}

// offset: 0x1928 | func: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/dll_381_func_1928.s")

// offset: 0x1B08 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/dll_381_func_1B08.s")

// offset: 0x1BD4 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/dll_381_func_1BD4.s")

// offset: 0x1D04 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/dll_381_func_1D04.s")

// offset: 0x1F60 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/dll_381_func_1F60.s")

// offset: 0x24E8 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/dll_381_func_24E8.s")

// offset: 0x2784 | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/dll_381_func_2784.s")

// offset: 0x2890 | func: 16
static void dll_381_func_2890(Object* arg0, RobotAnimPatrol_Data* arg1, s32 arg2) {
    RobotAnimPatrol_Setup* setup;

    if (arg2 == 1) {
        arg1->unkAB = 2;
        gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_355, NULL, 0, -1, NULL);
        arg0->velocity.y = -1.0f;
        arg1->unkAC = mathRnd(0, 70) - 35.0f;
        arg1->unkB0 = mathRnd(0, 70) - 35.0f;
        arg1->unkB4 = -arg1->unkB0 * 0.2f;
        arg1->unkB8 = arg1->unkAC * 0.2f;
        arg1->unkAC += arg0->srt.transl.x;
        arg1->unkB0 += arg0->srt.transl.z;
        objFreeObjectType(arg0, OBJTYPE_24);
        return;
    }
    if (arg1->unkAB++ > 180) {
        gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_355, NULL, 0, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_352, NULL, 0, -1, NULL);
        setup = (RobotAnimPatrol_Setup*)arg0->setup;
        if (setup->unk1E != -1) {
            mainSetBits(setup->unk1E, 1);
        }
        objDisable(arg0);
        arg1->unk358 |= 1;
        gDLL_6_AMSFX->vtbl->Play(arg0, SOUND_B19, MAX_VOLUME, NULL, NULL, 0, NULL);
        return;
    }
    arg0->srt.pitch = (s16) (arg0->srt.pitch + (10.0f * arg0->velocity.y));
    arg0->srt.yaw = (s16) (arg0->srt.yaw - (30.0f * arg0->velocity.y));
    arg0->srt.roll += (arg0->velocity.y * 40.0f) >= 0.0f 
        ? (arg0->velocity.y * 40.0f) 
        : -(arg0->velocity.y * 40.0f);
    arg0->velocity.y = arg0->velocity.y - 0.07f;
    arg1->unkB4 += (arg1->unkAC - arg0->srt.transl.x) * 0.0625f;
    arg1->unkB8 += (arg1->unkB0 - arg0->srt.transl.z) * 0.0625f;
    arg0->srt.transl.y += arg0->velocity.y;
    arg0->srt.transl.x += arg1->unkB4;
    arg0->srt.transl.z += arg1->unkB8;
    arg1->unkAC += (mathRnd(0, 70) - 35.0f) * 0.1f;
    arg1->unkB0 += (mathRnd(0, 70) - 35.0f) * 0.1f;
    gDLL_27->vtbl->func_1E8(arg0, &arg1->unkBC, gUpdateRateF);
    gDLL_27->vtbl->func_5A8(arg0, &arg1->unkBC);
    gDLL_27->vtbl->func_624(arg0, &arg1->unkBC, gUpdateRateF);
    if (arg1->unkBC.unk25C != 0) {
        if (gDLL_6_AMSFX->vtbl->IsPlaying(arg1->unk48) == 0) {
            gDLL_6_AMSFX->vtbl->Play(arg0, mathRnd(0, 1) ? SOUND_B17 : SOUND_B18, MAX_VOLUME, &arg1->unk48, NULL, 0, NULL);
        }
        gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_355, NULL, 0, -1, NULL);
        arg1->unkAB += 4;
    }
    if (mathRnd(0, 4) == 0) {
        gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_354, NULL, 0, -1, NULL);
    }
    gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_356, NULL, 0, -1, NULL);
    gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_353, NULL, 0, -1, NULL);
}

// offset: 0x2E9C | func: 17
void dll_381_func_2E9C(RobotAnimPatrol_Data_31C* arg0) {
    arg0->unk4[0] = 0;
    arg0->unk38 = 0;
}

// offset: 0x2EAC | func: 18
static void dll_381_func_2EAC(Object* arg0, RobotAnimPatrol_Data_31C* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    s32 i;
    SRT sp2C;

    if ((arg2 != 0.0f) || (arg3 != 0.0f) || (arg4 != 0.0f)) {
        guNormalize(&arg2, &arg3, &arg4);
    }
    arg1->unk36 = mathRnd(8, 12);
    dll_381_func_334C(arg2, arg3, arg4, &arg1->unk4[1], &arg1->unk1C[1]);
    for (i = 1; i < arg1->unk36; i++) {
        arg1->unk4[i + 1] = arg1->unk4[1];
        arg1->unk1C[i + 1] = arg1->unk1C[1];
    }
    arg1->unk4[0] = 0x190;
    arg1->unk38 += 0x500;
    arg1->unk3A = 0xFF;
    arg1->unk0 = arg5;
    sp2C.transl.x = arg2 * arg5;
    sp2C.transl.y = arg3 * arg5;
    sp2C.transl.z = arg4 * arg5;
    gDLL_17_partfx->vtbl->spawn(arg0, 0x35B, &sp2C, 0, -1, NULL);
}

// offset: 0x3040 | func: 19
static s32 dll_381_func_3040(Object* arg0, RobotAnimPatrol_Data_31C* arg1) {
    SRT sp80;
    f32 temp_fs0;
    s16 temp_s2;
    s16 temp_s3;
    s32 temp_ft0;
    s32 itr;
    s32 i;

    if (arg1->unk4[0] == 0) {
        return 1;
    }
    if (arg1->unk4[0] > 0xFF) {
        sp80.roll = 0xFF;
    } else {
        sp80.roll = arg1->unk4[0];
    }
    if (arg1->unk3A > 0) {
        sp80.yaw = arg1->unk3A;
    } else {
        sp80.yaw = 0;
    }
    arg1->unk3A -= gUpdateRate;
    for (itr = 0; itr < (gUpdateRate / 2); itr++) { // @bug: this won't run at 60 Hz
        for (i = 0; i < arg1->unk36; i++) {
            dll_381_func_33E8(arg1->unk4[i + 1], arg1->unk1C[i + 1], &sp80.transl, arg1->unk0);
            gDLL_17_partfx->vtbl->spawn(arg0, (i % 2) + PARTICLE_35C, &sp80, 0, -1, NULL);
            temp_s2 = ((0xFFFF / arg1->unk36) * i) + arg1->unk38;
            temp_fs0 = mathSinfInterp(temp_s2);
            temp_ft0 = mathRnd(0, 0x600);
            temp_s3 = (s16) (((f32) temp_ft0 + (temp_fs0 * 1792.0f)) - 768.0f);
            temp_fs0 = mathCosfInterp(temp_s2);
            temp_ft0 = mathRnd(0, 0x600);
            arg1->unk4[i + 1] += temp_s3;
            temp_s3 = (s16) (((f32) temp_ft0 + (temp_fs0 * 1792.0f)) - 768.0f);
            arg1->unk1C[i + 1] += temp_s3;
        }
    }
    arg1->unk4[0] -= gUpdateRate;
    if (arg1->unk4[0] < 0) {
        arg1->unk4[0] = 0;
    }
    return 0;
}

// offset: 0x334C | func: 20
static void dll_381_func_334C(f32 arg0, f32 arg1, f32 arg2, s16* arg3, s16* arg4) {
    f32 sp24;

    sp24 = sqrtf(SQ(arg0) + SQ(arg2));
    *arg3 = mathAtan2f(arg2, arg0);
    *arg4 = mathAtan2f(sp24, arg1);
}

// offset: 0x33E8 | func: 21
static void dll_381_func_33E8(s16 arg0, s16 arg1, Vec3f* arg2, f32 arg3) {
    arg2->x = mathSinfInterp(arg1) * mathCosfInterp(arg0) * arg3;
    arg2->y = mathCosfInterp(arg1) * arg3 + 3.0f;
    arg2->z = mathSinfInterp(arg1) * mathSinfInterp(arg0) * arg3;
}

// offset: 0x34D4 | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/dll_381_func_34D4.s")

// offset: 0x35B0 | func: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/dll_381_func_35B0.s")
