#include "dlls/engine/17_partfx.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/338_LFXEmitter.h"
#include "game/objects/object_id.h"
#include "sys/camera.h"
#include "sys/dll.h"
#include "sys/gfx/projgfx.h"
#include "sys/gfx/animseq.h"
#include "sys/gfx/textable.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/objmsg.h"
#include "sys/objlib.h"
#include "sys/print.h"
#include "sys/voxmap.h"
#include "dll.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 unk18;
/*19*/ u8 unk19[0x1A - 0x19];
/*1A*/ s16 unk1A;
/*1C*/ u8 unk1C[0x1E - 0x1C];
/*1E*/ s16 unk1E;
/*20*/ s16 unk20;
} RobotAnimPatrol_Setup;

typedef struct {
/*00*/ f32 unk0;
/*04*/ s16 unk4[12];
/*1C*/ s16 unk1C[12];
/*34*/ u8 unk34[0x36 - 0x34];
/*36*/ s16 unk36;
/*38*/ s16 unk38;
/*3A*/ s16 unk3A;
} RobotAnimPatrol_Data_31C;

typedef struct {
/*000*/ Object* unk0;
/*004*/ Object* unk4;
/*008*/ Object* unk8;
/*00C*/ Object* unkC;
/*010*/ s8 unk10;
/*011*/ s8 unk11;
/*012*/ s8 unk12;
/*013*/ u8 _unk13[0x18 - 0x13];
/*018*/ Vec3f unk18;
/*024*/ f32 unk24;
/*028*/ f32 unk28;
/*02C*/ f32 unk2C;
/*030*/ f32 unk30;
/*034*/ s16 unk34;
/*036*/ s16 unk36;
/*038*/ s16 unk38;
/*03A*/ s16 unk3A;
/*03C*/ s16 unk3C;
/*03E*/ s16 unk3E;
/*040*/ s16 unk40;
/*042*/ s16 unk42;
/*044*/ u32 unk44;
/*048*/ u32 unk48;
/*04C*/ u32 unk4C;
/*050*/ u8 unk50;
/*051*/ u8 unk51;
/*052*/ u8 unk52;
/*053*/ u8 unk53;
/*054*/ Vec3f unk54;
/*060*/ Vec3f unk60;
/*06C*/ Vec3f unk6C;
/*078*/ u8 _unk78[0x84 - 0x78];
/*084*/ f32 unk84;
/*088*/ f32 unk88;
/*08C*/ f32 unk8C;
/*090*/ Vec3f unk90;
/*09C*/ s32 unk9C;
/*0A0*/ s32 unkA0;
/*0A4*/ s16 unkA4;
/*0A6*/ s16 unkA6;
/*0A8*/ u8 unkA8;
/*0A9*/ u8 unkA9;
/*0AA*/ u8 unkAA;
/*0AB*/ u8 unkAB;
/*0AC*/ f32 unkAC;
/*0B0*/ f32 unkB0;
/*0B4*/ f32 unkB4;
/*0B8*/ f32 unkB8;
/*0BC*/ DLL27_Data unkBC;
/*31C*/ RobotAnimPatrol_Data_31C unk31C;
/*358*/ u8 unk358;
/*359*/ u8 unk359;
} RobotAnimPatrol_Data;

/*0x0*/ static DLTri data_0[] = {
    {0x40, 0, 2, 1, {0}}, 
    {0x40, 2, 1, 3, {0}}
};
/*0x20*/ static Vec3f data_20[] = {
    VEC3F(0.0f, 0.0f, 0.0f)
};
/*0x2C*/ static f32 data_2C[] = {7.0f};

/*0x0*/ static Texture* bss_0; // red laser beam
/*0x4*/ static Texture* bss_4; // blue laser beam
/*0x8*/ static Texture* bss_8; // blue fireball?

int dll_381_func_BC4(Object* self, Object* animObj, AnimObj_Data* animObjData, s8);
static int dll_381_func_16FC(Object* self);
void dll_381_func_1B08(Object* self, RobotAnimPatrol_Data* objdata);
static void dll_381_func_1BD4(Object* self, RobotAnimPatrol_Data* objdata);
void dll_381_func_1D04(Object* self, RobotAnimPatrol_Data* objdata);
static void dll_381_func_1F60(Object* self, ModelInstance* modelInst, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** tris);
static s32 dll_381_func_24E8(Vec3f*, Vec3f*, Vec3f*, Object*);
void dll_381_func_2784(Object* self, AnimObj_Data* animObjData);
static void dll_381_func_2890(Object*, RobotAnimPatrol_Data*, s32);
void dll_381_func_2E9C(RobotAnimPatrol_Data_31C*);
static void dll_381_func_2EAC(Object*, RobotAnimPatrol_Data_31C*, f32, f32, f32, f32);
static s32 dll_381_func_3040(Object*, RobotAnimPatrol_Data_31C*);
static void dll_381_func_334C(f32 arg0, f32 arg1, f32 arg2, s16* arg3, s16* arg4);
static void dll_381_func_33E8(s16 arg0, s16 arg1, Vec3f* arg2, f32 arg3);
Object* dll_381_func_34D4(Object* self, s32 arg1, s32 arg2, f32 velX, f32 velY, f32 velZ);
void dll_381_func_35B0(Vec3f*, Object*, Vec3f*);

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
    RobotAnimPatrol_Data* objdata;
    ObjSetup* beamSetup;
    Object* beam;
    ObjectShadow* beamShadow;
    s32 _pad[7];
    u8 sp3B;

    objdata = self->data;
    sp3B = 5;
    objdata->unk358 = 0;
    if ((setup->unk1E != -1) && (mainGetBits(setup->unk1E) != 0)) {
        objdata->unk358 |= 1;
        return;
    }
    objdata->unk358 &= ~0x1;
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
    if (objdata->unk0 == NULL) {
        beamSetup = objAllocSetup(sizeof(ObjSetup), OBJ_RobotBeam2);
        beamSetup->x = self->srt.transl.x;
        beamSetup->y = self->srt.transl.y;
        beamSetup->z = self->srt.transl.z;
        beamSetup->objId = OBJ_RobotBeam2;
        beamSetup->loadFlags = 2;
        beamSetup->byte5 = 4;
        beamSetup->fadeDistance = 0xFF;
        beam = objSetupObject(beamSetup, 0, -1, -1, self->parent);
        objdata->unk0 = beam;
        beam->srt.scale = 0.35f;
    }
    camGetObjectChildPosition(objdata->unk0, 
        &objdata->unk0->globalPosition.x, 
        &objdata->unk0->globalPosition.y, 
        &objdata->unk0->globalPosition.z);
    beamShadow = objdata->unk0->shadow;
    if (beamShadow != NULL) {
        beamShadow->flags |= 0x50;
        beamShadow->r = 0xC0;
        beamShadow->g = 0xFF;
        beamShadow->b = 0xFF;
        beamShadow->a = 0x1E;
        beamShadow->scale = 3.5f;
    }
    objdata->unk18.x = setup->base.x;
    objdata->unk18.y = setup->base.y;
    objdata->unk18.z = setup->base.z;
    objdata->unk36 = setup->unk18 << 8;
    objdata->unk38 = setup->unk1A;
    objdata->unk24 = 0.2f;
    objdata->unk3C = 0;
    objdata->unk52 = 0;
    objdata->unk40 = mathRnd(240, 600);
    objdata->unk4 = 0;
    objdata->unk10 = 0;
    objdata->unk11 = 0;
    objdata->unk51 = 0;
    objdata->unk50 = 1;
    objdata->unkA0 = self->srt.yaw;
    objdata->unk9C = 0;
    objdata->unkA4 = 0;
    objdata->unk34 = 0x400;
    objdata->unkAA = 0;
    objdata->unkAB = 0;
    objdata->unk359 = 0;
    objdata->unk8 = 0;
    objdata->unkC = 0;
    objdata->unk2C = 2.0f;
    objdata->unkB4 = 0.0f;
    objdata->unkB8 = 0.0f;
    objAnimSet(self, 0, 1.0f, 0);
    gDLL_27->vtbl->init(&objdata->unkBC, 0, 0x40083, 1);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->unkBC, 1, data_20, data_2C, &sp3B);
    objAddObjectType(self, OBJTYPE_24);
    objAddObjectType(self, OBJTYPE_Baddie);
    dll_381_func_2E9C(&objdata->unk31C);
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
                temp_a1 = temp_s1->unk6C.f;
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
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/dll_381_func_BC4.s")
#else
int dll_381_func_BC4(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    f32 xTmp;
    f32 zTmp;
    s32 temp_s0;
    s32 unused;
    Vec3f sp74;
    Vec3f sp68;
    f32 dist;
    s32 var_s1;
    Object* player;
    s32 ret;
    RobotAnimPatrol_Data* objdata;
    RobotAnimPatrol_Setup* setup;

    objdata = self->data;
    ret = 0;
    dist = 1000.0f;
    setup = (RobotAnimPatrol_Setup*)self->setup;
    diPrintf(" ROBO overriden %x ", self);
    objdata->unk359 += gUpdateRate;
    if (objdata->unk359 > 20) {
        objdata->unk359 = 0;
        sp68.f[0] = objdata->unk6C.f[0] * 5.0f;
        sp68.f[1] = objdata->unk6C.f[1] * 5.0f;
        sp68.f[2] = objdata->unk6C.f[2] * 5.0f;
        if (objdata->unk8 == 0) {
            objdata->unk8 = dll_381_func_34D4(self, 0x1FB, 0, objdata->unk6C.f[0], objdata->unk6C.f[1], objdata->unk6C.f[2]);
        } else {
            sp74.f[0] = objdata->unk6C.f[0] * 30.0f;
            sp74.f[1] = objdata->unk6C.f[1] * 30.0f;
            sp74.f[2] = objdata->unk6C.f[2] * 30.0f;
            sp74.f[0] += self->srt.transl.x;
            sp74.f[1] += self->srt.transl.y;
            sp74.f[2] += self->srt.transl.z;
            if (objdata->unkC == 0) {
                objdata->unkC = dll_381_func_34D4(self, 0x1FB, 0, sp68.f[0], sp68.f[1], sp68.f[2]);
            } else {
                dll_381_func_35B0(&sp74, objdata->unkC, &sp68);
            }
            dll_381_func_35B0(&self->srt.transl, objdata->unk8, &sp68);
        }
    }
    if (objdata->unk358 & 1) {
        return 0;
    }
    objdata->unk50 = gUpdateRate;
    self->unkE0 = 0;
    animObjData->unk62 = 0;
    for (var_s1 = 0; var_s1 < animObjData->messageCount; var_s1++) {
        switch (animObjData->messages[var_s1]) {
        case 1:
            objFreeObjectType(self, OBJTYPE_24);
            objdata->unk358 |= 2;
            break;
        case 2:
            objAddObjectType(self, OBJTYPE_24);
            objdata->unk358 &= ~0x2;
            break;
        case 3:
            objdata->unkAB = 0xC8;
            return 4;
        case 4:
            objdata->unkA9 = 1;
            objdata->unk4 = objGetPlayer();
            objdata->unk358 |= 4;
            break;
        case 5:
            objdata->unk4 = objFindClosestObject(self, OBJ_KytesMum, &dist);
            if (objdata->unk4 != NULL) {
                objdata->unkA9 = 1;
            }
            break;
        }
    }

    if (mainGetBits(setup->unk20) != 0) {
        objdata->unkAB = 1;
        return 4;
    }
    if (objdata->unk358 & 2) {
        return 0;
    }
    if (dll_381_func_3040(self, &objdata->unk31C) != 0) {
        if (objdata->unk4C != 0) {
            gDLL_6_AMSFX->vtbl->Stop(objdata->unk4C);
            objdata->unk4C = 0;
        }
    }
    if (objdata->unk31C.unk4[0] > 40) {
        return 1;
    }
    if (objdata->unkAB != 0) {
        return 4;
    }
    if (objdata->unkAA == 0) {
        animObjData->unk7A = 3;
    }
    if (objdata->unk44 == 0) {
        gDLL_6_AMSFX->vtbl->Play(self, SOUND_130, MAX_VOLUME, &objdata->unk44, NULL, 0, NULL);
    }
    objdata->unk40 -= gUpdateRate;
    if (objdata->unk40 < 0) {
        objdata->unk40 = mathRnd(240, 600);
        gDLL_6_AMSFX->vtbl->Play(self, (mathRnd(0, 1) + SOUND_131), MAX_VOLUME, &objdata->unk48, NULL, 0, NULL);
    }
    if ((objdata->unk48 != 0) && (gDLL_6_AMSFX->vtbl->IsPlaying(objdata->unk48) == 0)) {
        gDLL_6_AMSFX->vtbl->Stop(objdata->unk48);
        objdata->unk48 = 0;
    }
    if (objdata->unk12 != 0) {
        animObjData->unk7A = 0;
        player = objGetPlayer();
        xTmp = player->srt.transl.x - self->srt.transl.x;
        zTmp = player->srt.transl.z - self->srt.transl.z;
        unused = mathAtan2f(-xTmp, -zTmp);
        unused -= (self->srt.yaw & 0xFFFF);
        CIRCLE_WRAP(unused);
        self->srt.yaw += objdata->unk9C;
        objdata->unk9C *= 0.7f;
        if (self->velocity.y < 0.01f) {
            self->velocity.y = 0.0f;
        } else {
            self->velocity.y *= 0.7f;
            self->srt.transl.y += self->velocity.y;
            objdata->unk30 = self->srt.transl.y;
        }
        if (self->velocity.y == 0.0f) {
            self->srt.transl.y = objdata->unk30 - (mathSinfInterp(objdata->unkA4) * objdata->unk2C);
            temp_s0 = (u16) objdata->unkA4 + (gUpdateRate << 8);
            if (temp_s0 >= 0x10000) {
                objdata->unk2C = mathRnd(15, 35) * 0.1f;
            }
            objdata->unkA4 = temp_s0;
        }
        self->unkDC += gUpdateRate;
        if (self->unkDC >= 0x5B) {
            objdata->unk12 = 0;
            objdata->unkA9 = 0;
            objdata->unk4 = NULL;
            animObjData->unk4C.x = self->srt.transl.x - animObj->srt.transl.x;
            animObjData->unk4C.y = self->srt.transl.y - animObj->srt.transl.y;
            animObjData->unk4C.z = self->srt.transl.z - animObj->srt.transl.z;
            animObjData->yawDiff = self->srt.yaw - (animObj->srt.yaw & 0xFFFF);
            CIRCLE_WRAP(animObjData->yawDiff);
            animObjData->pitchDiff = self->srt.pitch - (animObj->srt.pitch & 0xFFFF);
            CIRCLE_WRAP(animObjData->pitchDiff);
            animObjData->rollDiff = (animObj->srt.roll & 0xFFFF) - (self->srt.roll & 0xFFFF);
            CIRCLE_WRAP(animObjData->rollDiff);
            animObjData->unk58 = 1.0f;
        }
        if (!(objdata->unk358 & 4)) {
            ret = 1;
        }
    } else {
        if (animObjData->unk58 > 0.0f) {
            animObjData->unk7A = 0;
            animObjData->unk58 -= (0.05f * gUpdateRateF);
            if (animObjData->unk58 < 0.0f) {
                animObjData->unk58 = 0.0f;
            }
            self->srt.transl.x = animObj->srt.transl.x + (animObjData->unk58 * animObjData->unk4C.x);
            self->srt.transl.y = animObj->srt.transl.y + (animObjData->unk58 * animObjData->unk4C.y);
            self->srt.transl.z = animObj->srt.transl.z + (animObjData->unk58 * animObjData->unk4C.z);
            self->srt.yaw = animObj->srt.yaw + (animObjData->unk58 * animObjData->yawDiff);
            self->srt.pitch = animObj->srt.pitch + (animObjData->unk58 * animObjData->pitchDiff);
            self->srt.roll = animObj->srt.roll + (animObjData->unk58 * animObjData->rollDiff);
            if (!(objdata->unk358 & 4)) {
                ret = 1;
            }
        } else {
            self->unkDC = 0;
            objdata->unk9C = self->srt.yaw - (objdata->unkA0 & 0xFFFF);
            CIRCLE_WRAP(objdata->unk9C);
            self->velocity.y = self->srt.transl.y - objdata->unk28;
            objdata->unk30 = self->srt.transl.y;
            objdata->unk358 &= ~0x4;
        }
    }
    objdata->unkA0 = self->srt.yaw;
    objdata->unk28 = self->srt.transl.y;
    dll_381_func_1D04(objdata->unk0, objdata);
    dll_381_func_1B08(self, objdata);
    dll_381_func_2784(self, animObjData);
    if (objdata->unkAA != 0) {
        if (objdata->unkAA == 2) {
            self->srt.transl.y += self->velocity.y * 0.85f;
            if (self->srt.transl.y > 1970.0f) {
                self->velocity.y = 0.0f;
                objdata->unkAB = 1;
            }
        }
        animObjData->unk7A &= ~0x1;
        animObjData->unk7A &= ~0x2;
        self->srt.pitch += (200.0f * self->velocity.y);
        self->srt.yaw -= (300.0f * self->velocity.y);
        self->srt.roll += ABS(400.0f * self->velocity.y);
    }
    return ret;
}

static const char str_2[] = " Override Man %x ";
static const char str_3[] = " Override Trans 2 ";
#endif

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
static void dll_381_func_1928(Object* self, RobotAnimPatrol_Data* objdata) {
    f32 sp6C;
    SRT sp54;
    SRT sp3C;
    DLL_IProjgfx* temp_s0;

    sp6C = (f32) (mathRnd(0, 200) - 100) / 1000.0f;
    gDLL_6_AMSFX->vtbl->Play(self, SOUND_115_Robot_LaserFire, MAX_VOLUME, NULL, NULL, 0, NULL);
    gDLL_6_AMSFX->vtbl->SetPitch(0, 1.0f + sp6C);
    sp3C.transl.x = objdata->unk90.x;
    sp3C.transl.y = objdata->unk90.y;
    sp3C.transl.z = objdata->unk90.z;
    sp3C.yaw = 0;
    sp3C.roll = 0;
    sp3C.pitch = 0;
    sp3C.scale = 1.0f;
    sp54.transl.x = objdata->unk60.x;
    sp54.transl.y = objdata->unk60.y;
    sp54.transl.z = objdata->unk60.z;
    sp54.yaw = 0;
    sp54.roll = 0;
    sp54.pitch = 0;
    sp54.scale = 1.0f;
    temp_s0 = dllLoad(DLL_ID_193, 1);
    temp_s0->vtbl->func0(objGetPlayer(), 0, &sp54, 1, -1, 7, &sp3C);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_86, &sp54, 1, -1, NULL);
    if (temp_s0 != NULL) {
        dllFree(temp_s0);
    }
}

// offset: 0x1B08 | func: 10
void dll_381_func_1B08(Object* self, RobotAnimPatrol_Data* objdata) {
    if (objdata->unk52 == 1) {
        dll_381_func_1BD4(self, objdata);
    }
    if (self->animProgress >= 1.0f) {
        objdata->unk52 = 1;
    } else {
        objdata->unk52 = 0;
    }
    objAnimAdvance(self, 0.02f, (f32) gUpdateRate, NULL);
}

// offset: 0x1BD4 | func: 11
static void dll_381_func_1BD4(Object* self, RobotAnimPatrol_Data* objdata) {
    s32 _pad;
    f32 temp_fv0;
    f32 sp34;
    f32 temp_fv1;
    SeqJoint* sp2C;
    Vec3f sp20;

    if ((objdata->unkA8 == 0) && (objdata->unkA9 != 0)) {
        if (objdata->unk4 != NULL) {
            sp2C = objExpr_func_80034804(self, 1);
            sp20.f[0] = 0.0f;
            sp20.f[1] = 0.0f;
            sp20.f[2] = 0.0f;
            sp20.f[0] = sp20.f[0] + objdata->unk4->srt.transl.x;
            sp20.f[1] = sp20.f[1] + objdata->unk4->srt.transl.y;
            sp20.f[2] = sp20.f[2] + objdata->unk4->srt.transl.z;
            temp_fv0 = sp20.f[0] - objdata->unk54.f[0];
            sp34 = sp20.f[1] - objdata->unk54.f[1];
            temp_fv1 = sp20.f[2] - objdata->unk54.f[2];
            objdata->unk3C = (s16) -mathAtan2f(sp34, sqrtf(SQ(temp_fv0) + SQ(temp_fv1)));
            sp2C->pitch = 0x238C - objdata->unk3C;
        }
        objdata->unkA8 = 1;
    }
}

// offset: 0x1D04 | func: 12
void dll_381_func_1D04(Object* self, RobotAnimPatrol_Data* objdata) {
    ObjectShadow* shadow;
    Vec3s16 sp64;
    Vec3s16 sp5C;
    Vec3s16 sp54;
    Vec3f sp48;
    Vec3f sp3C;
    f32 temp_fv1;

    shadow = self->shadow;
    camGetObjectChildPosition(self, &self->globalPosition.x, &self->globalPosition.y, &self->globalPosition.z);
    sp3C.f[0] = -shadow->dir.x;
    sp3C.f[1] = -shadow->dir.y;
    sp3C.f[2] = -shadow->dir.z;
    sp48.f[0] = sp3C.f[0] * 200.0f;
    sp48.f[1] = sp3C.f[1] * 200.0f;
    sp48.f[2] = sp3C.f[2] * 200.0f;
    sp48.f[0] += self->srt.transl.x;
    sp48.f[1] += self->srt.transl.y;
    sp48.f[2] += self->srt.transl.z;
    vox_func_80007EE0(&self->srt.transl, &sp64);
    vox_func_80007EE0(&sp48, &sp5C);
    if (vox_func_80008048(&sp64, &sp5C, &sp54, NULL, 0) == 0) {
        vox_func_80007E2C(&sp48, &sp54);
        sp48.f[0] -= self->srt.transl.x;
        sp48.f[1] -= self->srt.transl.y;
        sp48.f[2] -= self->srt.transl.z;
        temp_fv1 = sqrtf(SQ(sp48.f[0]) + SQ(sp48.f[1]) + SQ(sp48.f[2])) - 10.0f;
        sp48.f[0] = sp3C.f[0] * temp_fv1;
        sp48.f[1] = sp3C.f[1] * temp_fv1;
        sp48.f[2] = sp3C.f[2] * temp_fv1;
        shadow->tr.x = sp48.f[0] + self->srt.transl.x;
        shadow->tr.y = sp48.f[1] + self->srt.transl.y;
        shadow->tr.z = sp48.f[2] + self->srt.transl.z;
    } else {
        shadow->tr.x = self->srt.transl.x;
        shadow->tr.y = self->srt.transl.y;
        shadow->tr.z = self->srt.transl.z;
    }
    objdata->unk3A += (gUpdateRate * 4);
    if (objdata->unk3A >= 0x400) {
        objdata->unk3A = 0;
    }
    objExprGetTexAnimator(self, 0, 0)->positionV = objdata->unk3A;
}

// offset: 0x1F60 | func: 13
static void dll_381_func_1F60(Object* self, ModelInstance* modelInst, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** tris) {
    Vec3f sp9C;
    SRT srt;
    MtxF* boneMtx;
    f32 magnitude;
    Vtx* vtx;
    RobotAnimPatrol_Data* objdata;
    s32 bone;
    f32 laserY1;
    f32 laserZ1;
    f32 laserY2;
    f32 laserX1;
    f32 laserZ2;
    f32 laserX2;

    objdata = self->data;
    if ((objdata->unk52 == 1) || (objdata->unk52 == 4)) {
        vtx = *vtxs;
        bone = self->def->pAttachPoints[2].bones[self->modelInstIdx];
        boneMtx = (MtxF*) &((f32*)modelInst->matrices[modelInst->unk34 & 1])[bone << 4];
        objdata->unk54.x = boneMtx->m[3][0] + gWorldX;
        objdata->unk54.y = boneMtx->m[3][1];
        objdata->unk54.z = boneMtx->m[3][2] + gWorldZ;
        bone = self->def->pAttachPoints[1].bones[self->modelInstIdx];
        boneMtx = (MtxF*) &((f32*)modelInst->matrices[modelInst->unk34 & 1])[bone << 4];
        objdata->unk60.x = boneMtx->m[3][0] + gWorldX;
        objdata->unk60.y = boneMtx->m[3][1];
        objdata->unk60.z = boneMtx->m[3][2] + gWorldZ;
        objdata->unk6C.x = objdata->unk60.x - objdata->unk54.x;
        objdata->unk6C.y = objdata->unk60.y - objdata->unk54.y;
        objdata->unk6C.z = objdata->unk60.z - objdata->unk54.z;
        magnitude = 1.0f / sqrtf(SQ(objdata->unk6C.f[0]) + SQ(objdata->unk6C.f[1]) + SQ(objdata->unk6C.f[2]));
        objdata->unk6C.x *= magnitude;
        objdata->unk6C.y *= magnitude;
        objdata->unk6C.z *= magnitude;
        sp9C.f[0] = objdata->unk6C.x * 200.0f;
        sp9C.f[1] = objdata->unk6C.y * 200.0f;
        sp9C.f[2] = objdata->unk6C.z * 200.0f;
        sp9C.f[0] += objdata->unk60.x;
        sp9C.f[1] += objdata->unk60.y;
        sp9C.f[2] += objdata->unk60.z;
        dll_381_func_24E8(&objdata->unk60, &sp9C, &objdata->unk90, objdata->unk4);
        texDPTextures(gdl, bss_0, NULL, RENDER_UNK10 | RENDER_Z_COMPARE, 0, FALSE, TRUE);
        dlSetPrimColor(gdl, 255, 255, 255, 255);
        magnitude = sqrtf(SQ(objdata->unk6C.x) + SQ(objdata->unk6C.z));
        srt.yaw = mathAtan2f(objdata->unk6C.x, objdata->unk6C.z);
        srt.pitch = -mathAtan2f(objdata->unk6C.y, magnitude);
        srt.roll = 0;
        srt.transl.x = objdata->unk60.x;
        srt.transl.y = objdata->unk60.y;
        srt.transl.z = objdata->unk60.z;
        srt.scale = 0.1f;
        camSetupObjectSRTMatrix(gdl, mtxs, &srt, 1.0f, 0.0f, NULL);
        bcopy(data_0, *tris, sizeof(data_0));
        gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(*vtxs), 4, 0);
        dlTriangles(gdl, *tris, 2);

        laserZ2 = objdata->unk90.x - objdata->unk60.x;
        laserX2 = objdata->unk90.z - objdata->unk60.z;
        magnitude = sqrtf(SQ(laserZ2) + SQ(laserX2));
        laserX1 = 0.0f;
        laserY1 = 0.0f;
        laserZ1 = 0.0f;

        laserX2 = 0.0f;
        laserY2 = 0.0f;
        laserZ2 = magnitude / mathCosfInterp(srt.pitch);
        laserZ2 += 4.0f;
        
        laserX2 *= 10.0f;
        laserZ2 *= 10.0f;
        
        // @bug: The laser pointer tris set up here don't show up because the tex coords are uninitialized
        vtx->v.ob[0] = (s32)laserX1;\
        vtx->v.ob[1] = (s32)laserY1 + 7;\
        vtx->v.ob[2] = (s32)laserZ1;
        vtx->v.cn[0] = 255;\
        vtx->v.cn[1] = 0;\
        vtx->v.cn[2] = 0;\
        vtx->v.cn[3] = 40;
        vtx++;

        vtx->v.ob[0] = (s32)laserX1;\
        vtx->v.ob[1] = (s32)laserY1 - 7;\
        vtx->v.ob[2] = (s32)laserZ1;
        vtx->v.cn[0] = 255;\
        vtx->v.cn[1] = 0;\
        vtx->v.cn[2] = 0;\
        vtx->v.cn[3] = 40;
        vtx++;

        vtx->v.ob[0] = (s32)laserX2;\
        vtx->v.ob[1] = (s32)laserY2 + 7;\
        vtx->v.ob[2] = (s32)laserZ2;
        vtx->v.cn[0] = 255;\
        vtx->v.cn[1] = 0;\
        vtx->v.cn[2] = 0;\
        vtx->v.cn[3] = 40;
        vtx++;

        vtx->v.ob[0] = (s32)laserX2;\
        vtx->v.ob[1] = (s32)laserY2 - 7;\
        vtx->v.ob[2] = (s32)laserZ2;
        vtx->v.cn[0] = 255;\
        vtx->v.cn[1] = 0;\
        vtx->v.cn[2] = 0;\
        vtx->v.cn[3] = 40;
        vtx++;
        
        srt.roll = 0x4000;
        camSetupObjectSRTMatrix(gdl, mtxs, &srt, 1.0f, 0.0f, NULL);
        gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(*vtxs), 4, 0);
        dlTriangles(gdl, *tris, 2);
        *vtxs = vtx;
        *tris += 2;
        if ((objdata->unkA8 != 0) && (objdata->unkA9 != 0)) {
            dll_381_func_1928(self, objdata);
            objdata->unkA9 = 0;
        }
        objdata->unkA8 = 0;
    }
}

// offset: 0x24E8 | func: 14
static s32 dll_381_func_24E8(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Object* arg3) {
    Vec3s16 sp90;
    Vec3s16 sp88;
    Vec3s16 sp80;
    Vec3f sp74;
    Vec3f sp68;
    Vec3f sp5C;
    Vec3f sp50;
    Vec3f sp44;
    s8 unused2;
    s8 sp42;
    f32 one;
    f32 sp38;
    f32 var_fv1;

    sp44.f[0] = arg1->f[0] - arg0->f[0];
    sp44.f[1] = arg1->f[1] - arg0->f[1];
    sp44.f[2] = arg1->f[2] - arg0->f[2];
    var_fv1 = sqrtf(SQ(sp44.f[0]) + SQ(sp44.f[1]) + SQ(sp44.f[2]));
    if (var_fv1 != 0.0f) {
        var_fv1 = 1.0f / var_fv1;
    }
    sp44.f[0] *= var_fv1;
    sp44.f[1] *= var_fv1;
    sp44.f[2] *= var_fv1;
    vox_func_80007EE0(arg0, &sp90);
    vox_func_80007EE0(arg1, &sp88);
    sp5C.f[0] = arg1->f[0];
    sp5C.f[1] = arg1->f[1];
    sp5C.f[2] = arg1->f[2];
    sp38 = 1.0f;
    one = 1.0f;
    if (arg3 != NULL) {
        sp42 = func_8002AD3C(arg3, arg0, arg1, &sp5C, &sp38);
    } else {
        sp42 = 0;
    }
    unused2 = 0;
    if (vox_func_80008048(&sp90, &sp88, &sp80, NULL, 0) == 0) {
        vox_func_80007E2C(&sp68, &sp80);
        sp44.f[0] *= 20.0f;
        sp44.f[1] *= 20.0f;
        sp44.f[2] *= 20.0f;
        sp74.f[0] = sp68.f[0] - sp44.f[0];
        sp74.f[1] = sp68.f[1] - sp44.f[1];
        sp74.f[2] = sp68.f[2] - sp44.f[2];
        sp68.f[0] = sp68.f[0] + sp44.f[0];
        sp68.f[1] = sp68.f[1] + sp44.f[1];
        sp68.f[2] = sp68.f[2] + sp44.f[2];
    }
    if ((unused2 != 0) || (sp42 != 0)) {
        if (sp38 < 1.0f) {
            arg2->f[0] = sp5C.f[0];
            arg2->f[1] = sp5C.f[1];
            arg2->f[2] = sp5C.f[2];
            return 1;
        } else {
            arg2->f[0] = sp50.f[0];
            arg2->f[1] = sp50.f[1];
            arg2->f[2] = sp50.f[2];
            return 2;
        }
    }
    arg2->f[0] = arg1->f[0];
    arg2->f[1] = arg1->f[1];
    arg2->f[2] = arg1->f[2];
    return 0;
}

// offset: 0x2784 | func: 15
void dll_381_func_2784(Object* self, AnimObj_Data* animObjData) {
    u32 mesgID;
    void* mesgArg;
    RobotAnimPatrol_Data* objdata;

    mesgID = 0;
    mesgArg = NULL;
    objdata = self->data;
    while (objRecvMesg(self, &mesgID, NULL, &mesgArg) != 0) {
        switch (mesgID) {
        case 15:
            objdata->unkAA = 2;
            animObjData->unk7A &= ~1;
            break;
        case 16:
            objdata->unkAB = 1;
            break;
        }
    }
}

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
Object* dll_381_func_34D4(Object* self, s32 arg1, s32 arg2, f32 velX, f32 velY, f32 velZ) {
    LFXEmitter_Setup* emitterSetup;
    Object* emitter;

    emitterSetup = objAllocSetup(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
    emitterSetup->base.x = self->srt.transl.x;
    emitterSetup->base.y = self->srt.transl.y;
    emitterSetup->base.z = self->srt.transl.z;
    emitterSetup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
    emitterSetup->base.fadeFlags = OBJSETUP_FADE_MANUAL;
    emitterSetup->base.loadDistance = 255;
    emitterSetup->base.fadeDistance = 255;
    emitterSetup->unk20 = (s16) arg2;
    emitterSetup->unk1E = (s16) arg1;
    emitterSetup->unk22 = 0x95;
    emitter = objSetupObject(&emitterSetup->base, OBJINIT_STANDALONE, self->mapID, -1, self->parent);
    emitter->velocity.x = velX;
    emitter->velocity.y = velY;
    emitter->velocity.z = velZ;
    return emitter;
}

// offset: 0x35B0 | func: 23
void dll_381_func_35B0(Vec3f* arg0, Object* arg1, Vec3f* arg2) {
    arg1->srt.transl.x = arg0->x;
    arg1->srt.transl.y = arg0->y;
    arg1->srt.transl.z = arg0->z;
    arg1->velocity.x = arg2->x;
    arg1->velocity.y = arg2->y;
    arg1->velocity.z = arg2->z;
}
