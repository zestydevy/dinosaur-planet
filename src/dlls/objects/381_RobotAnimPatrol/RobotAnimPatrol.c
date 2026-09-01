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
/*18*/ s8 rotation;
/*19*/ u8 unk19[0x1A - 0x19];
/*1A*/ s16 unk1A;
/*1C*/ u8 unk1C[0x1E - 0x1C];
/*1E*/ s16 unk1E;
/*20*/ s16 unk20;
} RobotAnimPatrol_Setup;

typedef struct {
/*00*/ f32 unk0;
/*04*/ s16 timer;
/*06*/ s16 unk6[12];
/*1E*/ s16 unk1E[12];
/*36*/ s16 unk36;
/*38*/ s16 unk38;
/*3A*/ s16 unk3A;
} RobotAnimPatrol_StunState;

typedef struct {
/*000*/ Object* beam;
/*004*/ Object* target;
/*008*/ Object* lfxEmitter;
/*00C*/ Object* lfxEmitter2;
/*010*/ s8 canSeePlayer;
/*011*/ s8 prevCanSeePlayer;
/*012*/ s8 targetIsPlayer;
/*013*/ u8 _unk13[0x18 - 0x13];
/*018*/ Vec3f spawnPos;
/*024*/ f32 unk24;
/*028*/ f32 unk28;
/*02C*/ f32 unk2C;
/*030*/ f32 unk30;
/*034*/ s16 unk34;
/*036*/ s16 spawnYaw;
/*038*/ s16 unk38;
/*03A*/ s16 beamTexV;
/*03C*/ s16 unk3C;
/*03E*/ s16 unk3E;
/*040*/ s16 chatterSfxTimer;
/*042*/ s16 unk42;
/*044*/ u32 unk44;
/*048*/ u32 soundHandle;
/*04C*/ u32 unk4C;
/*050*/ u8 updateRate;
/*051*/ u8 unk51;
/*052*/ u8 gunDeployState;
/*053*/ u8 unk53;
/*054*/ Vec3f gunBasePos;
/*060*/ Vec3f gunBarrelPos;
/*06C*/ Vec3f gunDir;
/*078*/ u8 _unk78[0x84 - 0x78];
/*084*/ Vec3f beamDir;
/*090*/ Vec3f unk90;
/*09C*/ s32 unk9C;
/*0A0*/ s32 unkA0;
/*0A4*/ s16 unkA4;
/*0A6*/ s16 _unkA6;
/*0A8*/ u8 isGunAimed;
/*0A9*/ u8 shouldShoot;
/*0AA*/ u8 unkAA;
/*0AB*/ u8 crashTick;
/*0AC*/ f32 unkAC;
/*0B0*/ f32 unkB0;
/*0B4*/ f32 unkB4;
/*0B8*/ f32 unkB8;
/*0BC*/ DLL27_Data collider;
/*31C*/ RobotAnimPatrol_StunState stunState;
/*358*/ u8 flags;
/*359*/ u8 unk359;
} RobotAnimPatrol_Data;

enum RobotAnimPatrolFlags {
    ROBOTANIMPATROL_FLAGS_Destroyed = 1,
    ROBOTANIMPATROL_FLAGS_Unk2 = 2,
    ROBOTANIMPATROL_FLAGS_Unk4 = 4,
};

/*0x0*/ static DLTri sLaserTris[] = {
    {0x40, 0, 2, 1, {0}}, 
    {0x40, 2, 1, 3, {0}}
};
/*0x20*/ static Vec3f data_20[] = {
    VEC3F(0.0f, 0.0f, 0.0f)
};
/*0x2C*/ static f32 data_2C[] = {7.0f};

/*0x0*/ static Texture* sRedLaserBeamTexture;
/*0x4*/ static Texture* sBlueLaserBeamTexture; // unused
/*0x8*/ static Texture* bss_8; // blue fireball? unused

int RobotAnimPatrol_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8);
static int RobotAnimPatrol_checkForPlayer(Object* self);
void RobotAnimPatrol_animateGun(Object* self, RobotAnimPatrol_Data* objdata);
static void RobotAnimPatrol_aimGun(Object* self, RobotAnimPatrol_Data* objdata);
void RobotAnimPatrol_updateBeam(Object* beam, RobotAnimPatrol_Data* objdata);
static void RobotAnimPatrol_gunPrint(Object* self, ModelInstance* modelInst, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** tris);
static s32 RobotAnimPatrol_aimRaycast(Vec3f*, Vec3f*, Vec3f*, Object*);
void RobotAnimPatrol_processObjMessages(Object* self, AnimObj_Data* animObjData);
static void RobotAnimPatrol_crashControl(Object*, RobotAnimPatrol_Data*, s32);
void RobotAnimPatrol_initStunState(RobotAnimPatrol_StunState*);
static void RobotAnimPatrol_stun(Object*, RobotAnimPatrol_StunState*, f32, f32, f32, f32);
static s32 RobotAnimPatrol_updateStunState(Object*, RobotAnimPatrol_StunState*);
static void RobotAnimPatrol_func_334C(f32 arg0, f32 arg1, f32 arg2, s16* arg3, s16* arg4);
static void RobotAnimPatrol_func_33E8(s16 arg0, s16 arg1, Vec3f* arg2, f32 arg3);
Object* RobotAnimPatrol_fireLfxEmitter(Object* self, s32 arg1, s32 arg2, f32 velX, f32 velY, f32 velZ);
void RobotAnimPatrol_updateLfxEmitter(Vec3f* pos, Object* emitter, Vec3f* velocity);

// offset: 0x0 | ctor
void RobotAnimPatrol_ctor(void* dll) { }

// offset: 0xC | dtor
void RobotAnimPatrol_dtor(void* dll) {
    if (sRedLaserBeamTexture != NULL) {
        texFreeTexture(sRedLaserBeamTexture);
        sRedLaserBeamTexture = NULL;
    }
    if (sBlueLaserBeamTexture != NULL) {
        texFreeTexture(sBlueLaserBeamTexture);
        sBlueLaserBeamTexture = NULL;
    }
    if (bss_8 != NULL) {
        texFreeTexture(bss_8);
        bss_8 = NULL;
    }
}

// offset: 0xB0 | func: 0 | export: 0
#ifndef NON_MATCHING
void RobotAnimPatrol_obj_Setup(Object* self, ObjSetup* setup, s32 reset);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/RobotAnimPatrol_obj_Setup.s")
#else
void RobotAnimPatrol_obj_Setup(Object* self, RobotAnimPatrol_Setup* setup, s32 reset) {
    RobotAnimPatrol_Data* objdata = self->data;
    ObjSetup* beamSetup;
    Object* beam;
    ObjectShadow* beamShadow;
    s32 _pad[7];
    u8 sp3B;
    
    sp3B = 5;
    objdata->flags = 0;
    if ((setup->unk1E != -1) && (mainGetBits(setup->unk1E) != 0)) {
        objdata->flags |= ROBOTANIMPATROL_FLAGS_Destroyed;
        return;
    }
    objdata->flags &= ~ROBOTANIMPATROL_FLAGS_Destroyed;
    objInitMesgQueue(self, 4);
    self->animCallback = RobotAnimPatrol_animCallback;
    if (sRedLaserBeamTexture == NULL) {
        sRedLaserBeamTexture = texLoadTexture(TEXTABLE_127);
    }
    if (sBlueLaserBeamTexture == NULL) {
        sBlueLaserBeamTexture = texLoadTexture(TEXTABLE_16B);
    }
    if (bss_8 == NULL) {
        bss_8 = texLoadTexture(TEXTABLE_16E);
    }
    if (objdata->beam == NULL) {
        beamSetup = objAllocSetup(sizeof(ObjSetup), OBJ_RobotBeam2);
        beamSetup->x = self->srt.transl.x;
        beamSetup->y = self->srt.transl.y;
        beamSetup->z = self->srt.transl.z;
        beamSetup->objId = OBJ_RobotBeam2;
        beamSetup->loadFlags = OBJSETUP_LOAD_MANUAL;
        beamSetup->fadeFlags = OBJSETUP_FADE_CAMERA;
        beamSetup->fadeDistance = 255;
        beam = objSetupObject(beamSetup, 0, -1, -1, self->parent);
        objdata->beam = beam;
        beam->srt.scale = 0.35f;
    }
    camGetObjectChildPosition(objdata->beam, 
        &objdata->beam->globalPosition.x, 
        &objdata->beam->globalPosition.y, 
        &objdata->beam->globalPosition.z);
    beamShadow = objdata->beam->shadow;
    if (beamShadow != NULL) {
        beamShadow->flags |= OBJ_SHADOW_FLAG_CUSTOM_COLOR | OBJ_SHADOW_FLAG_CUSTOM_DIR;
        beamShadow->r = 192;
        beamShadow->g = 255;
        beamShadow->b = 255;
        beamShadow->a = 30;
        beamShadow->scale = 3.5f;
    }
    objdata->spawnPos.x = setup->base.x;
    objdata->spawnPos.y = setup->base.y;
    objdata->spawnPos.z = setup->base.z;
    objdata->spawnYaw = setup->rotation << 8;
    objdata->unk38 = setup->unk1A;
    objdata->unk24 = 0.2f;
    objdata->unk3C = 0;
    objdata->gunDeployState = 0;
    objdata->chatterSfxTimer = mathRnd(240, 600);
    objdata->target = NULL;
    objdata->canSeePlayer = FALSE;
    objdata->prevCanSeePlayer = FALSE;
    objdata->unk51 = 0;
    objdata->updateRate = 1;
    objdata->unkA0 = self->srt.yaw;
    objdata->unk9C = 0;
    objdata->unkA4 = 0;
    objdata->unk34 = 0x400;
    objdata->unkAA = 0;
    objdata->crashTick = 0;
    objdata->unk359 = 0;
    objdata->lfxEmitter = NULL;
    objdata->lfxEmitter2 = 0;
    objdata->unk2C = 2.0f;
    objdata->unkB4 = 0.0f;
    objdata->unkB8 = 0.0f;
    objAnimSet(self, 0, 1.0f, 0);
    gDLL_27->vtbl->init(&objdata->collider, DLL27FLAG_NONE, DLL27FLAG_40000 | DLL27FLAG_80 | DLL27FLAG_2 | DLL27FLAG_1, DLL27MODE_1);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->collider, 1, data_20, data_2C, &sp3B);
    objAddObjectType(self, OBJTYPE_24);
    objAddObjectType(self, OBJTYPE_Baddie);
    RobotAnimPatrol_initStunState(&objdata->stunState);
    func_8002674C(self);
}
#endif

// offset: 0x484 | func: 1 | export: 1
void RobotAnimPatrol_obj_Control(Object* self) {
    RobotAnimPatrol_Data* objdata = self->data;
    
    diPrintf(" ROBO not Overriden ");
    if (!(objdata->flags & 1)) {
        objdata->updateRate = gUpdateRate;
        if (objdata->crashTick) {
            RobotAnimPatrol_crashControl(self, objdata, objdata->crashTick);
        } else {
            if (self->unkE0 == 0) {
                self->srt.transl.x = objdata->spawnPos.x;
                self->srt.transl.y = objdata->spawnPos.y;
                self->srt.transl.z = objdata->spawnPos.z;
                self->srt.yaw = objdata->spawnYaw;
                self->unkE0 = 1;
            }
            RobotAnimPatrol_updateStunState(self, &objdata->stunState);
        }
    }
}

static const char str_1[] = "RobotAnimPatrol Beam Error!!\n";

// offset: 0x56C | func: 2 | export: 2
void RobotAnimPatrol_obj_Update(Object* self) {
    RobotAnimPatrol_Data* objdata;
    Object* player;
    Object* hitBy;
    s32 dmgType;

    objdata = self->data;
    dmgType = func_80025F40(self, &hitBy, NULL, NULL);
    if (!(objdata->flags & ROBOTANIMPATROL_FLAGS_Destroyed)) {
        if ((dmgType == Damage_Type_Projectile) || (dmgType == Damage_Type_Explosion)) {
            RobotAnimPatrol_stun(self, &objdata->stunState, 
                hitBy->srt.transl.x - self->srt.transl.x, 
                hitBy->srt.transl.y - self->srt.transl.y, 
                hitBy->srt.transl.z - self->srt.transl.z, 
                15.0f);
            if (objdata->unk4C != 0) {
                gDLL_6_AMSFX->vtbl->Stop(objdata->unk4C);
            }
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_B16, MAX_VOLUME, &objdata->unk4C, NULL, 0, NULL);
        }
        player = objGetPlayer();
        if (player != NULL) {
            objdata->updateRate = gUpdateRate;
            objdata->canSeePlayer = RobotAnimPatrol_checkForPlayer(self);
            if (objdata->canSeePlayer && !objdata->prevCanSeePlayer && (((DLL_210_Player*)player->dll)->vtbl->func66(player, 1) != 0)) {
                objdata->targetIsPlayer = 1;
                objdata->shouldShoot = 1;
                objdata->target = player;
            }
            objdata->prevCanSeePlayer = objdata->canSeePlayer;
        }
    }
}

// offset: 0x740 | func: 3 | export: 3
void RobotAnimPatrol_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** pols, s8 visibility) {
    RobotAnimPatrol_Data* objdata;
    ModelInstance* modelInst;
    s32 _pad;
    MtxF* attachPointMtx;
    MtxF sp48;
    f32* temp_a1;
    Object* beam;
    s32 bone;
    ObjectShadow* beamShadow;

    objdata = self->data;
    if (!(objdata->flags & ROBOTANIMPATROL_FLAGS_Destroyed)) {
        modelInst = self->modelInsts[self->modelInstIdx];
        if (visibility != 0) {
            objprintDrawModel(self, gdl, mtxs, (Vertex** ) vtxs, (Triangle** ) pols, 1.0f);
        } else if (!(modelInst->unk34 & 8)) {
            mod_func_8001943C(self, &sp48, 1.0f, 0.0f);
            mod_func_80019730(modelInst, modelInst->model, self, &sp48);
        }
        beam = objdata->beam;
        if (beam != NULL) {
            if (beam->stateFlags & 0x40) {
                objdata->beam = NULL;
                return;
            }
            // Attach beam to attach point 0
            bone = self->def->pAttachPoints[0].bones[self->modelInstIdx];
            attachPointMtx = (MtxF*) &((f32*)modelInst->matrices[modelInst->unk34 & 1])[bone << 4];
            beam->srt.transl.x = attachPointMtx->m[3][0] + gWorldX;
            beam->srt.transl.y = attachPointMtx->m[3][1];
            beam->srt.transl.z = attachPointMtx->m[3][2] + gWorldZ;
            camGetObjectChildPosition(beam, &beam->globalPosition.x, &beam->globalPosition.y, &beam->globalPosition.z);
            if (trackObjVisCheck(beam) != 0) {
                beamShadow = beam->shadow;
                beam->prevLocalPosition.x = beam->srt.transl.x;
                beam->prevLocalPosition.y = beam->srt.transl.y;
                beam->prevLocalPosition.z = beam->srt.transl.z;
                beam->srt.yaw = self->srt.yaw;
                beam->srt.pitch = self->srt.pitch + 0x3500;
                beam->srt.roll = 0;
                temp_a1 = objdata->gunDir.f;
                beam->opacityWithFade = self->opacityWithFade;
                if (beam->opacityWithFade > 160) {
                    beam->opacityWithFade = 160;
                }
                temp_a1[0] = 0.0f;
                temp_a1[1] = 1.0f;
                temp_a1[2] = 0.0f;
                mathRotateRPY(&beam->srt, temp_a1);
                beamShadow->dir.x = temp_a1[0];
                beamShadow->dir.y = temp_a1[1];
                beamShadow->dir.z = temp_a1[2];
                objdata->beamDir.x = temp_a1[0];
                objdata->beamDir.y = temp_a1[1];
                objdata->beamDir.z = temp_a1[2];
                beamShadow->tr.x = beam->srt.transl.x - (temp_a1[0] * 20.0f);
                beamShadow->tr.y = beam->srt.transl.y - (temp_a1[1] * 20.0f);
                beamShadow->tr.z = beam->srt.transl.z - (temp_a1[2] * 20.0f);
                objprintDrawModel(beam, gdl, mtxs, (Vertex** ) vtxs, (Triangle** ) pols, 1.0f);
                objdata->beam->modelInsts[0]->unk34 &= ~0x8;
            }
            RobotAnimPatrol_gunPrint(self, modelInst, gdl, mtxs, vtxs, pols);
        }
    }
}

// offset: 0xA74 | func: 4 | export: 4
void RobotAnimPatrol_obj_Free(Object* self, s32 onlySelf) {
    RobotAnimPatrol_Data* objdata;

    objdata = self->data;
    if (onlySelf == 0) {
        if (objdata->beam != NULL) {
            objFreeObject(objdata->beam);
        }
        if (objdata->lfxEmitter != NULL) {
            objFreeObject(objdata->lfxEmitter);
        }
        if (objdata->lfxEmitter2 != NULL) {
            objFreeObject(objdata->lfxEmitter2);
        }
    }
    if (objdata->soundHandle != 0) {
        gDLL_6_AMSFX->vtbl->Stop(objdata->soundHandle);
    }
    if (objdata->unk44 != 0) {
        gDLL_6_AMSFX->vtbl->Stop(objdata->unk44);
    }
    objFreeObjectType(self, OBJTYPE_24);
    objFreeObjectType(self, OBJTYPE_Baddie);
}

// offset: 0xBA0 | func: 5 | export: 5
u32 RobotAnimPatrol_obj_GetModelFlags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW;
}

// offset: 0xBB0 | func: 6 | export: 6
u32 RobotAnimPatrol_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(RobotAnimPatrol_Data);
}

// offset: 0xBC4 | func: 7
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/381_RobotAnimPatrol/RobotAnimPatrol_animCallback.s")
#else
int RobotAnimPatrol_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    f32 xTmp;
    f32 zTmp;
    s32 temp_s0;
    s32 unused;
    Vec3f lfxPos;
    Vec3f lfxVelocity;
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
        lfxVelocity.f[0] = objdata->gunDir.f[0] * 5.0f;
        lfxVelocity.f[1] = objdata->gunDir.f[1] * 5.0f;
        lfxVelocity.f[2] = objdata->gunDir.f[2] * 5.0f;
        if (objdata->lfxEmitter == NULL) {
            objdata->lfxEmitter = RobotAnimPatrol_fireLfxEmitter(self, 0x1FB, 0, objdata->gunDir.f[0], objdata->gunDir.f[1], objdata->gunDir.f[2]);
        } else {
            lfxPos.f[0] = objdata->gunDir.f[0] * 30.0f;
            lfxPos.f[1] = objdata->gunDir.f[1] * 30.0f;
            lfxPos.f[2] = objdata->gunDir.f[2] * 30.0f;
            lfxPos.f[0] += self->srt.transl.x;
            lfxPos.f[1] += self->srt.transl.y;
            lfxPos.f[2] += self->srt.transl.z;
            if (objdata->lfxEmitter2 == 0) {
                objdata->lfxEmitter2 = RobotAnimPatrol_fireLfxEmitter(self, 0x1FB, 0, lfxVelocity.f[0], lfxVelocity.f[1], lfxVelocity.f[2]);
            } else {
                RobotAnimPatrol_updateLfxEmitter(&lfxPos, objdata->lfxEmitter2, &lfxVelocity);
            }
            RobotAnimPatrol_updateLfxEmitter(&self->srt.transl, objdata->lfxEmitter, &lfxVelocity);
        }
    }
    if (objdata->flags & ROBOTANIMPATROL_FLAGS_Destroyed) {
        return 0;
    }
    objdata->updateRate = gUpdateRate;
    self->unkE0 = 0;
    animObjData->unk62 = 0;
    for (var_s1 = 0; var_s1 < animObjData->messageCount; var_s1++) {
        switch (animObjData->messages[var_s1]) {
        case 1:
            objFreeObjectType(self, OBJTYPE_24);
            objdata->flags |= ROBOTANIMPATROL_FLAGS_Unk2;
            break;
        case 2:
            objAddObjectType(self, OBJTYPE_24);
            objdata->flags &= ~ROBOTANIMPATROL_FLAGS_Unk2;
            break;
        case 3:
            objdata->crashTick = 200; // instantly explode
            return 4;
        case 4:
            objdata->shouldShoot = TRUE;
            objdata->target = objGetPlayer();
            objdata->flags |= ROBOTANIMPATROL_FLAGS_Unk4;
            break;
        case 5:
            objdata->target = objFindClosestObject(self, OBJ_KytesMum, &dist);
            if (objdata->target != NULL) {
                objdata->shouldShoot = TRUE;
            }
            break;
        }
    }

    if (mainGetBits(setup->unk20) != 0) {
        objdata->crashTick = 1;
        return 4;
    }
    if (objdata->flags & ROBOTANIMPATROL_FLAGS_Unk2) {
        return 0;
    }
    if (RobotAnimPatrol_updateStunState(self, &objdata->stunState) != 0) {
        if (objdata->unk4C != 0) {
            gDLL_6_AMSFX->vtbl->Stop(objdata->unk4C);
            objdata->unk4C = 0;
        }
    }
    if (objdata->stunState.timer > 40) {
        return 1;
    }
    if (objdata->crashTick != 0) {
        return 4;
    }
    if (objdata->unkAA == 0) {
        animObjData->unk7A = 3;
    }
    if (objdata->unk44 == 0) {
        gDLL_6_AMSFX->vtbl->Play(self, SOUND_130, MAX_VOLUME, &objdata->unk44, NULL, 0, NULL);
    }
    objdata->chatterSfxTimer -= gUpdateRate;
    if (objdata->chatterSfxTimer < 0) {
        objdata->chatterSfxTimer = mathRnd(240, 600);
        gDLL_6_AMSFX->vtbl->Play(self, (mathRnd(0, 1) + SOUND_131), MAX_VOLUME, &objdata->soundHandle, NULL, 0, NULL);
    }
    if ((objdata->soundHandle != 0) && (gDLL_6_AMSFX->vtbl->IsPlaying(objdata->soundHandle) == 0)) {
        gDLL_6_AMSFX->vtbl->Stop(objdata->soundHandle);
        objdata->soundHandle = 0;
    }
    if (objdata->targetIsPlayer) {
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
        if (self->unkDC > 90) {
            objdata->targetIsPlayer = FALSE;
            objdata->shouldShoot = FALSE;
            objdata->target = NULL;
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
        if (!(objdata->flags & ROBOTANIMPATROL_FLAGS_Unk4)) {
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
            if (!(objdata->flags & ROBOTANIMPATROL_FLAGS_Unk4)) {
                ret = 1;
            }
        } else {
            self->unkDC = 0;
            objdata->unk9C = self->srt.yaw - (objdata->unkA0 & 0xFFFF);
            CIRCLE_WRAP(objdata->unk9C);
            self->velocity.y = self->srt.transl.y - objdata->unk28;
            objdata->unk30 = self->srt.transl.y;
            objdata->flags &= ~ROBOTANIMPATROL_FLAGS_Unk4;
        }
    }
    objdata->unkA0 = self->srt.yaw;
    objdata->unk28 = self->srt.transl.y;
    RobotAnimPatrol_updateBeam(objdata->beam, objdata);
    RobotAnimPatrol_animateGun(self, objdata);
    RobotAnimPatrol_processObjMessages(self, animObjData);
    if (objdata->unkAA != 0) {
        if (objdata->unkAA == 2) {
            self->srt.transl.y += self->velocity.y * 0.85f;
            if (self->srt.transl.y > 1970.0f) {
                self->velocity.y = 0.0f;
                objdata->crashTick = 1;
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
static int RobotAnimPatrol_checkForPlayer(Object* self) {
    Object* player;
    Vec3f vec2Player;
    Vec3f sp5C;
    Vec3f playerPos;
    f32 beamXZMag;
    f32 temp_fv0_2;
    f32 var_fa0;
    RobotAnimPatrol_Data* objdata;
    SRT sp28;

    player = objGetPlayer();
    if (player == NULL) {
        return 0;
    }
    playerPos.f[0] = player->srt.transl.x;
    playerPos.f[1] = player->srt.transl.y + 35.0f;
    playerPos.f[2] = player->srt.transl.z;
    objdata = self->data;
    // Check if player is in range
    vec2Player.f[0] = playerPos.f[0] - self->srt.transl.x;
    vec2Player.f[1] = playerPos.f[1] - self->srt.transl.y;
    vec2Player.f[2] = playerPos.f[2] - self->srt.transl.z;
    if (sqrtf(SQ(vec2Player.f[0]) + SQ(vec2Player.f[1]) + SQ(vec2Player.f[2])) < 300.0f) {
        // Check if player is in beam cone
        sp5C.x = playerPos.f[0] - objdata->beam->srt.transl.x;
        sp5C.y = playerPos.f[1] - objdata->beam->srt.transl.y;
        sp5C.z = playerPos.f[2] - objdata->beam->srt.transl.z;
        beamXZMag = sqrtf(SQ(objdata->beamDir.x) + SQ(objdata->beamDir.z));
        sp28.yaw = -objdata->beam->srt.yaw;
        sp28.pitch = mathAtan2f(objdata->beamDir.y, beamXZMag);
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
static void RobotAnimPatrol_fireGun(Object* self, RobotAnimPatrol_Data* objdata) {
    f32 pitchMod;
    SRT startSRT;
    SRT endSRT;
    DLL_IProjgfx* laserProj;

    pitchMod = (f32) (mathRnd(0, 200) - 100) / 1000.0f;
    gDLL_6_AMSFX->vtbl->Play(self, SOUND_115_Robot_LaserFire, MAX_VOLUME, NULL, NULL, 0, NULL);
    gDLL_6_AMSFX->vtbl->SetPitch(0, 1.0f + pitchMod);
    endSRT.transl.x = objdata->unk90.x;
    endSRT.transl.y = objdata->unk90.y;
    endSRT.transl.z = objdata->unk90.z;
    endSRT.yaw = 0;
    endSRT.roll = 0;
    endSRT.pitch = 0;
    endSRT.scale = 1.0f;
    startSRT.transl.x = objdata->gunBarrelPos.x;
    startSRT.transl.y = objdata->gunBarrelPos.y;
    startSRT.transl.z = objdata->gunBarrelPos.z;
    startSRT.yaw = 0;
    startSRT.roll = 0;
    startSRT.pitch = 0;
    startSRT.scale = 1.0f;
    laserProj = dllLoad(DLL_ID_193, 1);
    laserProj->vtbl->func0(objGetPlayer(), 0, &startSRT, 1, -1, 7, &endSRT);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_86, &startSRT, 1, -1, NULL);
    if (laserProj != NULL) {
        dllFree(laserProj);
    }
}

// offset: 0x1B08 | func: 10
void RobotAnimPatrol_animateGun(Object* self, RobotAnimPatrol_Data* objdata) {
    if (objdata->gunDeployState == 1) {
        RobotAnimPatrol_aimGun(self, objdata);
    }
    if (self->animProgress >= 1.0f) {
        objdata->gunDeployState = 1;
    } else {
        objdata->gunDeployState = 0;
    }
    objAnimAdvance(self, 0.02f, (f32) gUpdateRate, NULL);
}

// offset: 0x1BD4 | func: 11
static void RobotAnimPatrol_aimGun(Object* self, RobotAnimPatrol_Data* objdata) {
    s32 _pad;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    SeqJoint* sp2C;
    Vec3f targPos;

    if (!objdata->isGunAimed && objdata->shouldShoot) {
        if (objdata->target != NULL) {
            sp2C = objExpr_func_80034804(self, 1);
            targPos.f[0] = 0.0f;
            targPos.f[1] = 0.0f;
            targPos.f[2] = 0.0f;
            targPos.f[0] += objdata->target->srt.transl.x;
            targPos.f[1] += objdata->target->srt.transl.y;
            targPos.f[2] += objdata->target->srt.transl.z;
            dirX = targPos.f[0] - objdata->gunBasePos.f[0];
            dirY = targPos.f[1] - objdata->gunBasePos.f[1];
            dirZ = targPos.f[2] - objdata->gunBasePos.f[2];
            objdata->unk3C = (s16) -mathAtan2f(dirY, sqrtf(SQ(dirX) + SQ(dirZ)));
            sp2C->pitch = 0x238C - objdata->unk3C;
        }
        objdata->isGunAimed = TRUE;
    }
}

// offset: 0x1D04 | func: 12
void RobotAnimPatrol_updateBeam(Object* beam, RobotAnimPatrol_Data* objdata) {
    ObjectShadow* beamShadow;
    Vec3s16 voxPos;
    Vec3s16 beamEndVoxPos;
    Vec3s16 voxHitPos;
    Vec3f targetPos;
    Vec3f dir;
    f32 magnitude;

    beamShadow = beam->shadow;
    camGetObjectChildPosition(beam, &beam->globalPosition.x, &beam->globalPosition.y, &beam->globalPosition.z);
    dir.f[0] = -beamShadow->dir.x;
    dir.f[1] = -beamShadow->dir.y;
    dir.f[2] = -beamShadow->dir.z;
    targetPos.f[0] = dir.f[0] * 200.0f;
    targetPos.f[1] = dir.f[1] * 200.0f;
    targetPos.f[2] = dir.f[2] * 200.0f;
    targetPos.f[0] += beam->srt.transl.x;
    targetPos.f[1] += beam->srt.transl.y;
    targetPos.f[2] += beam->srt.transl.z;
    vox_func_80007EE0(&beam->srt.transl, &voxPos);
    vox_func_80007EE0(&targetPos, &beamEndVoxPos);
    if (vox_func_80008048(&voxPos, &beamEndVoxPos, &voxHitPos, NULL, 0) == 0) {
        vox_func_80007E2C(&targetPos, &voxHitPos);
        targetPos.f[0] -= beam->srt.transl.x;
        targetPos.f[1] -= beam->srt.transl.y;
        targetPos.f[2] -= beam->srt.transl.z;
        magnitude = sqrtf(SQ(targetPos.f[0]) + SQ(targetPos.f[1]) + SQ(targetPos.f[2])) - 10.0f;
        targetPos.f[0] = dir.f[0] * magnitude;
        targetPos.f[1] = dir.f[1] * magnitude;
        targetPos.f[2] = dir.f[2] * magnitude;
        beamShadow->tr.x = targetPos.f[0] + beam->srt.transl.x;
        beamShadow->tr.y = targetPos.f[1] + beam->srt.transl.y;
        beamShadow->tr.z = targetPos.f[2] + beam->srt.transl.z;
    } else {
        beamShadow->tr.x = beam->srt.transl.x;
        beamShadow->tr.y = beam->srt.transl.y;
        beamShadow->tr.z = beam->srt.transl.z;
    }
    objdata->beamTexV += (gUpdateRate * 4);
    if (objdata->beamTexV >= 0x400) {
        objdata->beamTexV = 0;
    }
    objExprGetTexAnimator(beam, 0, 0)->positionV = objdata->beamTexV;
}

// offset: 0x1F60 | func: 13
static void RobotAnimPatrol_gunPrint(Object* self, ModelInstance* modelInst, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** tris) {
    Vec3f aimPoint;
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
    if ((objdata->gunDeployState == 1) || (objdata->gunDeployState == 4)) {
        vtx = *vtxs;
        bone = self->def->pAttachPoints[2].bones[self->modelInstIdx]; // gun base
        boneMtx = (MtxF*) &((f32*)modelInst->matrices[modelInst->unk34 & 1])[bone << 4];
        objdata->gunBasePos.x = boneMtx->m[3][0] + gWorldX;
        objdata->gunBasePos.y = boneMtx->m[3][1];
        objdata->gunBasePos.z = boneMtx->m[3][2] + gWorldZ;
        bone = self->def->pAttachPoints[1].bones[self->modelInstIdx]; // gun barrel
        boneMtx = (MtxF*) &((f32*)modelInst->matrices[modelInst->unk34 & 1])[bone << 4];
        objdata->gunBarrelPos.x = boneMtx->m[3][0] + gWorldX;
        objdata->gunBarrelPos.y = boneMtx->m[3][1];
        objdata->gunBarrelPos.z = boneMtx->m[3][2] + gWorldZ;
        objdata->gunDir.x = objdata->gunBarrelPos.x - objdata->gunBasePos.x;
        objdata->gunDir.y = objdata->gunBarrelPos.y - objdata->gunBasePos.y;
        objdata->gunDir.z = objdata->gunBarrelPos.z - objdata->gunBasePos.z;
        magnitude = 1.0f / sqrtf(SQ(objdata->gunDir.f[0]) + SQ(objdata->gunDir.f[1]) + SQ(objdata->gunDir.f[2]));
        objdata->gunDir.x *= magnitude;
        objdata->gunDir.y *= magnitude;
        objdata->gunDir.z *= magnitude;
        aimPoint.x = objdata->gunDir.x * 200.0f;
        aimPoint.y = objdata->gunDir.y * 200.0f;
        aimPoint.z = objdata->gunDir.z * 200.0f;
        aimPoint.x += objdata->gunBarrelPos.x;
        aimPoint.y += objdata->gunBarrelPos.y;
        aimPoint.z += objdata->gunBarrelPos.z;
        RobotAnimPatrol_aimRaycast(&objdata->gunBarrelPos, &aimPoint, &objdata->unk90, objdata->target);
        texDPTextures(gdl, sRedLaserBeamTexture, NULL, RENDER_UNK10 | RENDER_Z_COMPARE, 0, FALSE, TRUE);
        dlSetPrimColor(gdl, 255, 255, 255, 255);
        magnitude = sqrtf(SQ(objdata->gunDir.x) + SQ(objdata->gunDir.z));
        srt.yaw = mathAtan2f(objdata->gunDir.x, objdata->gunDir.z);
        srt.pitch = -mathAtan2f(objdata->gunDir.y, magnitude);
        srt.roll = 0;
        srt.transl.x = objdata->gunBarrelPos.x;
        srt.transl.y = objdata->gunBarrelPos.y;
        srt.transl.z = objdata->gunBarrelPos.z;
        srt.scale = 0.1f;
        camSetupObjectSRTMatrix(gdl, mtxs, &srt, 1.0f, 0.0f, NULL);
        bcopy(sLaserTris, *tris, sizeof(sLaserTris));
        gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(*vtxs), 4, 0);
        dlTriangles(gdl, *tris, 2);

        laserZ2 = objdata->unk90.x - objdata->gunBarrelPos.x;
        laserX2 = objdata->unk90.z - objdata->gunBarrelPos.z;
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
        if (objdata->isGunAimed && objdata->shouldShoot) {
            RobotAnimPatrol_fireGun(self, objdata);
            objdata->shouldShoot = FALSE;
        }
        objdata->isGunAimed = FALSE;
    }
}

// offset: 0x24E8 | func: 14
static s32 RobotAnimPatrol_aimRaycast(Vec3f* barrelPos, Vec3f* aimPoint, Vec3f* fireAtPoint, Object* target) {
    Vec3s16 sp90;
    Vec3s16 sp88;
    Vec3s16 sp80;
    Vec3f sp74;
    Vec3f sp68;
    Vec3f sp5C;
    Vec3f sp50;
    Vec3f sp44;
    s8 unused;
    s8 sp42;
    f32 one;
    f32 sp38;
    f32 var_fv1;

    sp44.f[0] = aimPoint->f[0] - barrelPos->f[0];
    sp44.f[1] = aimPoint->f[1] - barrelPos->f[1];
    sp44.f[2] = aimPoint->f[2] - barrelPos->f[2];
    var_fv1 = sqrtf(SQ(sp44.f[0]) + SQ(sp44.f[1]) + SQ(sp44.f[2]));
    if (var_fv1 != 0.0f) {
        var_fv1 = 1.0f / var_fv1;
    }
    sp44.f[0] *= var_fv1;
    sp44.f[1] *= var_fv1;
    sp44.f[2] *= var_fv1;
    vox_func_80007EE0(barrelPos, &sp90);
    vox_func_80007EE0(aimPoint, &sp88);
    sp5C.f[0] = aimPoint->f[0];
    sp5C.f[1] = aimPoint->f[1];
    sp5C.f[2] = aimPoint->f[2];
    sp38 = 1.0f;
    one = 1.0f;
    if (target != NULL) {
        sp42 = func_8002AD3C(target, barrelPos, aimPoint, &sp5C, &sp38);
    } else {
        sp42 = 0;
    }
    unused = 0;
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
    if ((unused != 0) || (sp42 != 0)) {
        if (sp38 < 1.0f) {
            fireAtPoint->f[0] = sp5C.f[0];
            fireAtPoint->f[1] = sp5C.f[1];
            fireAtPoint->f[2] = sp5C.f[2];
            return 1;
        } else {
            // @bug: sp50 is uninitialized!!
            fireAtPoint->f[0] = sp50.f[0];
            fireAtPoint->f[1] = sp50.f[1];
            fireAtPoint->f[2] = sp50.f[2];
            return 2;
        }
    }
    fireAtPoint->f[0] = aimPoint->f[0];
    fireAtPoint->f[1] = aimPoint->f[1];
    fireAtPoint->f[2] = aimPoint->f[2];
    return 0;
}

// offset: 0x2784 | func: 15
void RobotAnimPatrol_processObjMessages(Object* self, AnimObj_Data* animObjData) {
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
            objdata->crashTick = 1;
            break;
        }
    }
}

// offset: 0x2890 | func: 16
static void RobotAnimPatrol_crashControl(Object* self, RobotAnimPatrol_Data* objdata, s32 arg2) {
    RobotAnimPatrol_Setup* setup;

    if (arg2 == 1) {
        objdata->crashTick = 2;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_355, NULL, 0, -1, NULL);
        self->velocity.y = -1.0f;
        objdata->unkAC = mathRnd(0, 70) - 35.0f;
        objdata->unkB0 = mathRnd(0, 70) - 35.0f;
        objdata->unkB4 = -objdata->unkB0 * 0.2f;
        objdata->unkB8 = objdata->unkAC * 0.2f;
        objdata->unkAC += self->srt.transl.x;
        objdata->unkB0 += self->srt.transl.z;
        objFreeObjectType(self, OBJTYPE_24);
        return;
    }
    if (objdata->crashTick++ > 180) {
        // Explode
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_355, NULL, 0, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_352, NULL, 0, -1, NULL);
        setup = (RobotAnimPatrol_Setup*)self->setup;
        if (setup->unk1E != -1) {
            mainSetBits(setup->unk1E, 1);
        }
        objDisable(self);
        objdata->flags |= ROBOTANIMPATROL_FLAGS_Destroyed;
        gDLL_6_AMSFX->vtbl->Play(self, SOUND_B19, MAX_VOLUME, NULL, NULL, 0, NULL);
        return;
    }
    self->srt.pitch += (10.0f * self->velocity.y);
    self->srt.yaw -= (30.0f * self->velocity.y);
    self->srt.roll += ABS(self->velocity.y * 40.0f);
    self->velocity.y -= 0.07f;
    objdata->unkB4 += (objdata->unkAC - self->srt.transl.x) * 0.0625f;
    objdata->unkB8 += (objdata->unkB0 - self->srt.transl.z) * 0.0625f;
    self->srt.transl.y += self->velocity.y;
    self->srt.transl.x += objdata->unkB4;
    self->srt.transl.z += objdata->unkB8;
    objdata->unkAC += (mathRnd(0, 70) - 35.0f) * 0.1f;
    objdata->unkB0 += (mathRnd(0, 70) - 35.0f) * 0.1f;
    gDLL_27->vtbl->func_1E8(self, &objdata->collider, gUpdateRateF);
    gDLL_27->vtbl->func_5A8(self, &objdata->collider);
    gDLL_27->vtbl->func_624(self, &objdata->collider, gUpdateRateF);
    if (objdata->collider.unk25C != 0) {
        // Hit track, play crash fx
        if (gDLL_6_AMSFX->vtbl->IsPlaying(objdata->soundHandle) == 0) {
            gDLL_6_AMSFX->vtbl->Play(self, mathRnd(0, 1) ? SOUND_B17 : SOUND_B18, MAX_VOLUME, &objdata->soundHandle, NULL, 0, NULL);
        }
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_355, NULL, 0, -1, NULL);
        objdata->crashTick += 4;
    }
    if (mathRnd(0, 4) == 0) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_354, NULL, 0, -1, NULL);
    }
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_356, NULL, 0, -1, NULL);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_353, NULL, 0, -1, NULL);
}

// offset: 0x2E9C | func: 17
void RobotAnimPatrol_initStunState(RobotAnimPatrol_StunState* stun) {
    stun->timer = 0;
    stun->unk38 = 0;
}

// offset: 0x2EAC | func: 18
static void RobotAnimPatrol_stun(Object* arg0, RobotAnimPatrol_StunState* stun, f32 dirX, f32 dirY, f32 dirZ, f32 arg5) {
    s32 i;
    SRT sp2C;

    if ((dirX != 0.0f) || (dirY != 0.0f) || (dirZ != 0.0f)) {
        guNormalize(&dirX, &dirY, &dirZ);
    }
    stun->unk36 = mathRnd(8, 12);
    RobotAnimPatrol_func_334C(dirX, dirY, dirZ, &stun->unk6[0], &stun->unk1E[0]);
    for (i = 1; i < stun->unk36; i++) {
        stun->unk6[i] = stun->unk6[0];
        stun->unk1E[i] = stun->unk1E[0];
    }
    stun->timer = 400;
    stun->unk38 += 0x500;
    stun->unk3A = 0xFF;
    stun->unk0 = arg5;
    sp2C.transl.x = dirX * arg5;
    sp2C.transl.y = dirY * arg5;
    sp2C.transl.z = dirZ * arg5;
    gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_35B, &sp2C, 0, -1, NULL);
}

// offset: 0x3040 | func: 19
static s32 RobotAnimPatrol_updateStunState(Object* self, RobotAnimPatrol_StunState* stun) {
    SRT sp80;
    f32 temp_fs0;
    s16 temp_s2;
    s16 temp_s3;
    s32 temp_ft0;
    s32 itr;
    s32 i;

    if (stun->timer == 0) {
        return 1;
    }
    if (stun->timer > 0xFF) {
        sp80.roll = 0xFF;
    } else {
        sp80.roll = stun->timer;
    }
    if (stun->unk3A > 0) {
        sp80.yaw = stun->unk3A;
    } else {
        sp80.yaw = 0;
    }
    stun->unk3A -= gUpdateRate;
    for (itr = 0; itr < (gUpdateRate / 2); itr++) { // @bug: this won't run at 60 Hz
        for (i = 0; i < stun->unk36; i++) {
            RobotAnimPatrol_func_33E8(stun->unk6[i], stun->unk1E[i], &sp80.transl, stun->unk0);
            gDLL_17_partfx->vtbl->spawn(self, (i % 2) + PARTICLE_35C, &sp80, 0, -1, NULL);
            temp_s2 = ((0xFFFF / stun->unk36) * i) + stun->unk38;
            temp_fs0 = mathSinfInterp(temp_s2);
            temp_ft0 = mathRnd(0, 0x600);
            temp_s3 = (s16) (((f32) temp_ft0 + (temp_fs0 * 1792.0f)) - 768.0f);
            temp_fs0 = mathCosfInterp(temp_s2);
            temp_ft0 = mathRnd(0, 0x600);
            stun->unk6[i] += temp_s3;
            temp_s3 = (s16) (((f32) temp_ft0 + (temp_fs0 * 1792.0f)) - 768.0f);
            stun->unk1E[i] += temp_s3;
        }
    }
    stun->timer -= gUpdateRate;
    if (stun->timer < 0) {
        stun->timer = 0;
    }
    return 0;
}

// offset: 0x334C | func: 20
static void RobotAnimPatrol_func_334C(f32 arg0, f32 arg1, f32 arg2, s16* arg3, s16* arg4) {
    f32 sp24;

    sp24 = sqrtf(SQ(arg0) + SQ(arg2));
    *arg3 = mathAtan2f(arg2, arg0);
    *arg4 = mathAtan2f(sp24, arg1);
}

// offset: 0x33E8 | func: 21
static void RobotAnimPatrol_func_33E8(s16 arg0, s16 arg1, Vec3f* arg2, f32 arg3) {
    arg2->x = mathSinfInterp(arg1) * mathCosfInterp(arg0) * arg3;
    arg2->y = mathCosfInterp(arg1) * arg3 + 3.0f;
    arg2->z = mathSinfInterp(arg1) * mathSinfInterp(arg0) * arg3;
}

// offset: 0x34D4 | func: 22
Object* RobotAnimPatrol_fireLfxEmitter(Object* self, s32 arg1, s32 arg2, f32 velX, f32 velY, f32 velZ) {
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
void RobotAnimPatrol_updateLfxEmitter(Vec3f* pos, Object* emitter, Vec3f* velocity) {
    emitter->srt.transl.x = pos->x;
    emitter->srt.transl.y = pos->y;
    emitter->srt.transl.z = pos->z;
    emitter->velocity.x = velocity->x;
    emitter->velocity.y = velocity->y;
    emitter->velocity.z = velocity->z;
}
