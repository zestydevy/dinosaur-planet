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
/*0*/ f32 stickXF;
/*4*/ f32 stickYInvF;
/*8*/ u16 pressed;
/*A*/ u16 released;
/*C*/ u16 buttons;
/*E*/ s8 stickX;
/*F*/ s8 stickYInv;
} EWTrobotpatrol_ControllerState;

typedef struct {
/*00*/ f32 moveSin;
/*04*/ f32 unk4;
/*08*/ f32 moveCos;
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
/*3C*/ Vec3f moveVec; // relative to robo orientation
/*48*/ Vec3f unk48;
/*54*/ u8 _unk54[0x6C - 0x54];
/*6C*/ f32 unk6C;
/*70*/ f32 unk70;
/*74*/ f32 deltaTime; // updateRate / 60
/*78*/ f32 unk78;
/*7C*/ f32 unk7C;
/*80*/ f32 unk80;
/*84*/ f32 baseOffsetY;
/*88*/ f32 bobScale;
/*8C*/ s16 bobTheta; // current theta for bobbing up and down while floating
/*8E*/ u8 unk8E;
} EWTrobotpatrol_Body;

// size:0x34 ?
typedef struct {
/*00*/ Vec3f unk0[4];
/*30*/ u8 unk30;
} EWTrobotpatrol_Fx;

typedef struct {
/*00*/ Object* target;
/*04*/ u8 _unk4[0x8 - 0x4];
/*08*/ Vec3f barrelPos;
/*14*/ Vec3f basePos;
/*20*/ Vec3f fireAtPoint;
/*2C*/ Vec3f unk2C;
/*38*/ Vec3f dir;
/*44*/ f32 animDelta;
/*48*/ s16 yaw;
/*4A*/ s16 pitch;
/*4C*/ s16 targetYaw;
/*4E*/ u8 isDeployed;
/*4F*/ u8 mode;
/*50*/ u8 shouldShoot;
/*51*/ u8 updateRate;
} EWTrobotpatrol_Gun;

typedef struct {
/*00*/ f32 colorPulseTValue;
/*04*/ Object* obj; // RobotBeam
/*08*/ Vec3f dir; // beam dir
/*14*/ s16 beamTexV;
/*16*/ s16 yaw;
/*18*/ s16 yawTarget;
/*1A*/ u8 unk1A;
/*1B*/ u8 unk1B; // unused
/*1C*/ u8 colorPulseDir;
} EWTrobotpatrol_Beam;

// size:0x3C
typedef struct {
/*00*/ f32 unk0;
/*04*/ s16 timer;
/*06*/ s16 unk6[12];
/*1E*/ s16 unk1E[12];
/*36*/ s16 unk36;
/*38*/ s16 unk38;
/*3A*/ s16 unk3A;
} EWTrobotpatrol_StunState;

typedef struct {
/*000*/ u8 unk0; // unused
/*004*/ s32 destCurveUID;
/*008*/ s32 prevCurveUID;
/*00C*/ u8 _unkC[0x14 - 0xC];
/*014*/ f32 destDist; // current distance to destination position
/*018*/ s16 alertTimer;
/*01A*/ s16 gunCooldown;
/*01C*/ u8 _unk1C[0x1D - 0x1C];
/*01D*/ u8 getNextCurveDebounce;
/*01E*/ u8 alertMode;
/*01F*/ u8 activateState;
/*020*/ u8 combatState;
/*021*/ u8 hasVoxLineOfSightToPlayer;
/*024*/ Unk80008E40 voxRoute;
/*04C*/ Vec3f savedPosition;
/*058*/ Vec3f destPos; // position to move to
/*064*/ u8 _unk64[0x74 - 0x64];
/*074*/ Object* unk74;
/*078*/ EWTrobotpatrolCallback baseCallback;
/*07C*/ Object* player;
/*080*/ EWTrobotpatrol_ControllerState cont;
/*090*/ EWTrobotpatrol_Body body;
/*120*/ EWTrobotpatrol_Fx fx;
/*154*/ EWTrobotpatrol_Gun gun;
/*1A8*/ EWTrobotpatrol_Beam beam;
/*1C8*/ f32 unk1C8;
/*1CC*/ u8 unk1CC;
/*1D0*/ EWTrobotpatrol_StunState stunState;
} EWTrobotpatrol_Data;

/*0x0*/ static DLTri data_0[] = {
    {0x40, 0, 2, 1, {0}}, 
    {0x40, 2, 1, 3, {0}}
};

/*0x0*/ static Texture* sLaserBeamTexture; // red laser beam
/*0x4*/ static Texture* sNoiseTexture; // noise pattern

static void EWTrobotpatrol_checkForPlayer(Object* self, EWTrobotpatrol_Data* objdata);
static void EWTrobotpatrol_initBody(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Body* a2);
static void EWTrobotpatrol_updateBody(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Body*);
static s32 EWTrobotpatrol_move(Object* arg0, EWTrobotpatrol_Body* arg1, EWTrobotpatrol_Data* arg2, s32 arg3);
static s32 EWTrobotpatrol_func_1674(Object* arg0, EWTrobotpatrol_Body* arg1, Vec3f* arg2);
static s32 EWTrobotpatrol_func_1920(Object* arg0, EWTrobotpatrol_Body* arg1, Vec3f* arg2);
static void EWTrobotpatrol_initFx(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Fx* a2);
static void EWTrobotpatrol_updateFx(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Fx*);
static void EWTrobotpatrol_initGun(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Gun* a2);
static void EWTrobotpatrol_animateGun(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Gun*);
void EWTrobotpatrol_func_231C(Object* arg0, ModelInstance* arg1, Gfx** arg2, Mtx** arg3, Vtx** arg4, DLTri** arg5);
s32 EWTrobotpatrol_aimRaycast(Vec3f* barrelPos, Vec3f* aimPoint, Vec3f* fireAtPoint, Vec3f* a3, Object* target);
static void EWTrobotpatrol_animateDeployedGun(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Gun* arg2);
static void EWTrobotpatrol_setGunMode(EWTrobotpatrol_Gun*, s32, Object*);
static void EWTrobotpatrol_initBeam(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Beam* a2);
static void EWTrobotpatrol_updateBeam(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Beam*);
static void EWTrobotpatrol_freeBeam(Object*, EWTrobotpatrol_Data*, EWTrobotpatrol_Beam*, s32);
static void EWTrobotpatrol_readController(Object*, EWTrobotpatrol_ControllerState*, u8);
static void EWTrobotpatrol_curveMove(Object* arg0, EWTrobotpatrol_Data* arg1, EWTrobotpatrol_Body* arg2);
static int EWTrobotpatrol_animCallback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3);
static void EWTrobotpatrol_initStunState(EWTrobotpatrol_StunState* a0);
static void EWTrobotpatrol_stun(Object*, EWTrobotpatrol_StunState*, f32, f32, f32, f32);
static void EWTrobotpatrol_updateStunState(Object*, EWTrobotpatrol_StunState*);
static void EWTrobotpatrol_func_4004(f32 arg0, f32 arg1, f32 arg2, s16* arg3, s16* arg4);
static void EWTrobotpatrol_func_40A0(s16 arg0, s16 arg1, Vec3f* arg2, f32 arg3);

// offset: 0x0 | ctor
void EWTrobotpatrol_ctor(void *dll) { }

// offset: 0xC | dtor
void EWTrobotpatrol_dtor(void* dll) {
    if (sLaserBeamTexture != NULL) {
        texFreeTexture(sLaserBeamTexture);
        sLaserBeamTexture = NULL;
    }
    if (sNoiseTexture != NULL) {
        texFreeTexture(sNoiseTexture);
        sNoiseTexture = NULL;
    }
}

// offset: 0x88 | func: 0 | export: 0
void EWTrobotpatrol_obj_Setup(Object* self, EWTrobotpatrol_Setup* setup, s32 reset) {
    EWTrobotpatrol_Data* objdata;

    if (sLaserBeamTexture == NULL) {
        sLaserBeamTexture = texLoadTexture(TEXTABLE_127);
    }
    if (sNoiseTexture == NULL) {
        sNoiseTexture = texLoadTexture(TEXTABLE_16F);
    }
    objAddObjectType(self, OBJTYPE_Baddie);
    objdata = self->data;
    self->animCallback = EWTrobotpatrol_animCallback;
    bzero(objdata, sizeof(EWTrobotpatrol_Data));
    vox_func_80008DC0(&objdata->voxRoute);
    objdata->unk1CC = 1;
    objdata->unk1C8 = -0.02f;
    EWTrobotpatrol_initBody(self, objdata, &objdata->body);
    EWTrobotpatrol_initFx(self, objdata, &objdata->fx);
    EWTrobotpatrol_initBeam(self, objdata, &objdata->beam);
    EWTrobotpatrol_initGun(self, objdata, &objdata->gun);
    objdata->destCurveUID = -1;
    objdata->prevCurveUID = -1;
    objdata->unk0 = setup->unk1A;
    objdata->alertMode = 0;
    objdata->activateState = 0;
    objdata->combatState = 0;
    EWTrobotpatrol_initStunState(&objdata->stunState);
}

// offset: 0x23C | func: 1 | export: 1
void EWTrobotpatrol_obj_Control(Object* self) {
    EWTrobotpatrol_Data* objdata;
    EWTrobotpatrol_Body* body;
    s32 _pad;
    MtxF sp5C;
    f32 velY;
    SRT sp40;
    s32 bobTheta;

    objdata = self->data;
    EWTrobotpatrol_updateStunState(self, &objdata->stunState);
    if (objdata->stunState.timer <= 40) {
        EWTrobotpatrol_readController(self, &objdata->cont, 2);
        body = &objdata->body;
        EWTrobotpatrol_updateBody(self, objdata, body);
        sp40.yaw = self->srt.yaw;
        sp40.pitch = 0;
        sp40.roll = 0;
        sp40.transl.x = 0.0f;
        sp40.transl.y = 0.0f;
        sp40.transl.z = 0.0f;
        sp40.scale = 1.0f;
        mathYprXyzMtx(&sp5C, &sp40);
        mathMtxXFMF(&sp5C, body->moveVec.x, 0.0f, body->moveVec.z, &self->velocity.x, &velY, &self->velocity.z);
        self->srt.transl.y = (mathSinfInterp(body->bobTheta) * body->bobScale) + body->baseOffsetY;
        bobTheta = (u16) body->bobTheta + (gUpdateRate << 8);
        if (bobTheta >= 0x10000) {
            body->bobScale = (f32) ((f32) mathRnd(15, 35) * 0.1f);
        }
        body->bobTheta = (s16) bobTheta;
        self->velocity.x *= 1.0666667f;
        self->velocity.z *= 1.0666667f;
        objMove(self, self->velocity.x * gUpdateRateF, 0.0f, self->velocity.z * gUpdateRateF);
        EWTrobotpatrol_updateFx(self, objdata, &objdata->fx);
        EWTrobotpatrol_updateBeam(self, objdata, &objdata->beam);
        EWTrobotpatrol_animateGun(self, objdata, &objdata->gun);
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
        EWTrobotpatrol_stun(self, &objdata->stunState, 
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
    ModelInstance* modelInst;
    Object* beam;
    MtxF* attachPointMtx;
    s32 bone;
    s32 _pad;
    f32 temp_fv1_2;
    MtxF sp64;
    u8 beamShadowR;
    u8 beamShadowG;
    u8 beamShadowB;
    u8 alertR;
    u8 alertG;
    u8 alertB;
    u8 savedAmbientR;
    u8 savedAmbientG;
    u8 savedAmbientB;
    ObjectShadow* beamShadow;
    Vec3f sp48;
    s32 _pad2;
    u8 ambientR;
    u8 ambientG;
    u8 ambientB;
    Vec3f* beamDir;

    lightGetAmbient(&ambientR, &ambientG, &ambientB);
    objdata = self->data;
    modelInst = self->modelInsts[self->modelInstIdx];
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, (Vertex**)vtxs, (Triangle**)pols, 1.0f);
    } else if (!(modelInst->unk34 & 8)) {
        // Not drawing self but we still need the model matrices to be live
        mod_func_8001943C(self, &sp64, 1.0f, 0.0f);
        mod_func_80019730(modelInst, modelInst->model, self, &sp64);
    }
    beam = objdata->beam.obj;
    beamDir = &objdata->beam.dir;
    // Attach beam to attach point 0
    bone = self->def->pAttachPoints[0].bones[self->modelInstIdx]; // beam projector
    attachPointMtx = (MtxF*) &((f32*)modelInst->matrices[modelInst->unk34 & 1])[bone << 4];
    beam->srt.transl.x = attachPointMtx->m[3][0] + gWorldX;
    beam->srt.transl.y = attachPointMtx->m[3][1];
    beam->srt.transl.z = attachPointMtx->m[3][2] + gWorldZ;
    camGetObjectChildPosition(beam, &beam->globalPosition.x, &beam->globalPosition.y, &beam->globalPosition.z);
    if (trackObjVisCheck(beam) != 0) {
        beamShadow = beam->shadow;
        // Pulse beam shadow color between white and red when alerted
        switch (objdata->alertMode) {
        case 1:
            alertR = 208;
            alertG = 0;
            alertB = 0;
            break;
        case 2:
            alertR = 208;
            alertG = 0;
            alertB = 0;
            break;
        case 0:
        default:
            alertR = 186;
            alertG = 255;
            alertB = 255;  
            break;
        }
        beamShadowR = 186;
        beamShadowG = 255;
        beamShadowB = 255;
        beamShadowR += (objdata->beam.colorPulseTValue * (f32) (alertR - beamShadowR));
        beamShadowG += (objdata->beam.colorPulseTValue * (f32) (alertG - beamShadowG));
        beamShadowB += (objdata->beam.colorPulseTValue * (f32) (alertB - beamShadowB));
        // Calculate shadow direction
        bone = self->def->pAttachPoints[3].bones[self->modelInstIdx]; // body center
        attachPointMtx = (MtxF*) &((f32*)modelInst->matrices[modelInst->unk34 & 1])[bone << 4];
        sp48.f[0] = attachPointMtx->m[3][0] + gWorldX;
        sp48.f[1] = attachPointMtx->m[3][1];
        sp48.f[2] = attachPointMtx->m[3][2] + gWorldZ;
        sp48.f[0] = beam->srt.transl.x - sp48.f[0];
        sp48.f[1] = beam->srt.transl.y - sp48.f[1];
        sp48.f[2] = beam->srt.transl.z - sp48.f[2];
        temp_fv1_2 = 1.0f / sqrtf(SQ(sp48.f[0]) + SQ(sp48.f[1]) + SQ(sp48.f[2]));
        beamDir->x = (f32) (sp48.f[0] * temp_fv1_2);
        beamDir->y = (f32) (sp48.f[1] * temp_fv1_2);
        beamDir->z = (f32) (sp48.f[2] * temp_fv1_2);
        // Update shadow
        beamShadow->dir.x = -beamDir->x;
        beamShadow->dir.y = -beamDir->y;
        beamShadow->dir.z = -beamDir->z;
        beamShadow->tr.x = beam->srt.transl.x;
        beamShadow->tr.y = beam->srt.transl.y;
        beamShadow->tr.z = beam->srt.transl.z;
        beamShadow->flags |= OBJ_SHADOW_FLAG_FADE_OUT; // this results in the shadow never appearing
        beamShadow->r = beamShadowR;
        beamShadow->g = beamShadowG;
        beamShadow->b = beamShadowB;
        // Update beam
        beam->prevLocalPosition.x = beam->srt.transl.x;
        beam->prevLocalPosition.y = beam->srt.transl.y;
        beam->prevLocalPosition.z = beam->srt.transl.z;
        beam->srt.yaw = objdata->beam.yaw;
        beam->srt.pitch = 0;
        beam->srt.roll = 0;
        beam->srt.scale = 0.2f;
        beam->opacityWithFade = self->opacityWithFade;
        if (beam->opacityWithFade > 160) {
            beam->opacityWithFade = 160;
        }
        beam->opacityWithFade = (u8) ((beam->opacityWithFade * (beam->opacity + 1)) >> 8);
        // ? Was this originally trying to apply the shadow color to the beam model itself?
        savedAmbientR = ambientR;
        savedAmbientG = ambientG;
        savedAmbientB = ambientB;
        ambientR = beamShadowR;
        ambientG = beamShadowG;
        ambientB = beamShadowB;
        // Draw beam
        objprintDrawModel(beam, gdl, mtxs, (Vertex**)vtxs, (Triangle**)pols, 1.0f);
        ambientR = savedAmbientR;
        ambientG = savedAmbientG;
        ambientB = savedAmbientB;
        // Mark beam model matrices as no longer live (why?)
        beam->modelInsts[beam->modelInstIdx]->unk34 &= ~0x8;
    }
    EWTrobotpatrol_func_231C(self, modelInst, gdl, mtxs, vtxs, pols);
}
#endif

// offset: 0xBF4 | func: 4 | export: 4
void EWTrobotpatrol_obj_Free(Object* self, s32 onlySelf) {
    EWTrobotpatrol_Data* objdata;

    objFreeObjectType(self, OBJTYPE_Baddie);
    objdata = self->data;
    vox_func_80008E08(&objdata->voxRoute);
    gDLL_13_Expgfx->vtbl->func5(self);
    EWTrobotpatrol_freeBeam(self, objdata, &objdata->beam, onlySelf);
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
static void EWTrobotpatrol_moveAndShoot(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Body* body, EWTrobotpatrol_ControllerState* cont) {
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
    objdata->hasVoxLineOfSightToPlayer = vox_func_80008048(&playerVoxPos, &selfVoxPos, NULL, NULL, 0);
    if ((objdata->activateState != 0) && (objdata->activateState == 2)) {
        // Activated, do combat
#ifndef AVOID_UB
#ifdef __clang__
        PRAGMA_IGNORE_PUSH("-Wcast-function-type-mismatch")
#else
        PRAGMA_IGNORE_PUSH("-Wcast-function-type")
#endif
        // @fake cast
        ((s32 (*)(Object *, Object *, s32, u32))objdata->baseCallback)(objdata->unk74, self, /*aggro*/TRUE, 0);
        PRAGMA_IGNORE_POP()
#else
        objdata->baseCallback(objdata->unk74, self, /*aggro*/TRUE, 0);
#endif
        switch (objdata->combatState) {
        case 0:
            EWTrobotpatrol_setGunMode(&objdata2->gun, 2, player);
            if (EWTrobotpatrol_move(self, body, objdata, 0) != 0) {
                bcopy(&objdata->savedPosition, &objdata->destPos, sizeof(objdata->destPos));
                objdata->combatState = 1;
                objdata->alertTimer = 300;
            }
            break;
        case 1:
            EWTrobotpatrol_move(self, body, objdata, 1);
            if ((playerDist < 300.0f) && objdata->hasVoxLineOfSightToPlayer) {
                objdata->combatState = 2;
                objdata->alertTimer = 300;
                objdata->gunCooldown = 0;
            } else {
                objdata->alertTimer -= gUpdateRate;
                if (objdata->alertTimer < 0) {
                    objdata->alertMode = 0;
                    objdata->activateState = 0;
                    objdata->combatState = 0;
                }
            }
            break;
        case 2:
            EWTrobotpatrol_move(self, body, objdata, 1);
            if ((playerDist > 300.0f) || !objdata->hasVoxLineOfSightToPlayer) {
                objdata->combatState = 1;
            }
            objdata->gunCooldown -= gUpdateRate;
            if (objdata->gunCooldown < 0) {
                objdata->gunCooldown = 0;
            }
            if (objdata->gunCooldown == 0) {
                if (objdata->hasVoxLineOfSightToPlayer 
                        && (((DLL_210_Player*)player->dll)->vtbl->func66(player, 9) == 0) 
                        && (((DLL_210_Player*)player->dll)->vtbl->func66(player, 1) != 0)) {
                    objdata2->gun.shouldShoot = 3;
                }
                objdata->gunCooldown = 150;
            }
            break;
        }
    } else {
        // Not alerted
        objdata2->gun.animDelta = -0.02f; // hide weapon
        if (objdata->destCurveUID != -1) {
            EWTrobotpatrol_curveMove(self, objdata, body);
            if (objdata->getNextCurveDebounce > 0) {
                objdata->getNextCurveDebounce -= 1;
            }
            if ((objdata->getNextCurveDebounce == 0) && (objdata->destDist < 3.0f)) {
                objdata->baseCallback(objdata->unk74, self, 0, objdata->destCurveUID);
                objdata->getNextCurveDebounce = 120;
            }
        }
        EWTrobotpatrol_checkForPlayer(self, objdata);
        if (objdata->alertMode == 2) {
            dll_amSfx->Play(self, SOUND_112_EWT_Robot_Activate, MAX_VOLUME, NULL, NULL, 0, NULL);
            if (objdata2){} // @fake
            objdata2->gun.animDelta = 0.02f; // show weapon
            objdata2->beam.colorPulseDir = 0;
            objdata2->beam.colorPulseTValue = 0.0f;
            bcopy(&self->srt.transl, &objdata->savedPosition, sizeof(objdata->savedPosition));
            objdata->activateState = 2;
            objdata->combatState = 0;
        }
    }
}

// offset: 0x1164 | func: 8
static void EWTrobotpatrol_checkForPlayer(Object* self, EWTrobotpatrol_Data* objdata) {
    Object* player;
    Vec3f vec2Player;
    Vec3f sp64;
    f32 temp_fa1;
    s32 temp_v0_3;
    EWTrobotpatrol_Data* objdata2;
    EWTrobotpatrol_Beam* beam;
    SRT sp3C;
    s32 _pad;

    player = objGetPlayer();
    objdata->player = player;
    if (objdata->alertMode == 2) {
        return;
    }
    objdata2 = self->data;
    beam = &objdata2->beam;
    vec2Player.f[0] = player->srt.transl.x - self->srt.transl.x;
    vec2Player.f[1] = player->srt.transl.y - self->srt.transl.y;
    vec2Player.f[2] = player->srt.transl.z - self->srt.transl.z;
    if (sqrtf(SQ(vec2Player.f[0]) + SQ(vec2Player.f[1]) + SQ(vec2Player.f[2])) < 150.0f) {
        // Player in range
        if (objdata->hasVoxLineOfSightToPlayer) {
            // Player in LOS
            sp64.x = player->srt.transl.x - beam->obj->srt.transl.x;
            sp64.y = player->srt.transl.y - beam->obj->srt.transl.y;
            sp64.z = player->srt.transl.z - beam->obj->srt.transl.z;
            temp_fa1 = sqrtf(SQ(beam->dir.x) + SQ(beam->dir.z));
            sp3C.yaw = -beam->obj->srt.yaw;
            sp3C.pitch = -mathAtan2f(beam->dir.y, temp_fa1);
            sp3C.roll = 0;
            mathRotateYPR(&sp3C, &sp64);
            // Check if in beam cone?
            if (sqrtf(SQ(sp64.x) + SQ(sp64.y)) <= 40.0f) {
                objdata->alertMode = 2;
            }
        }
        if (objdata->alertMode != 2) {
            // Check if player is moving?
            temp_v0_3 = (s32)((DLL_210_Player*)player->dll)->vtbl->func66(player, 2);
            if ((temp_v0_3 != 3) && (temp_v0_3 != 4)) {
                objdata->alertMode = 0;
            } else {
                objdata->alertMode = 2;
                objdata->player = player;
            }
        }
    }
}

// offset: 0x139C | func: 9
static void EWTrobotpatrol_initBody(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Body* body) {
    body->unk6C = 50.0f;
    body->unk7C = 1.0f / body->unk6C;
    body->unk78 = (body->unk6C * 0.5f) * 0.5f;
    body->unk80 = 1.0f / body->unk78;
    body->unk70 = 7.0f;
    body->baseOffsetY = self->srt.transl.y + body->unk6C;
    body->bobScale = 2.0f;
    body->bobTheta = 0;
}

// offset: 0x1414 | func: 10
static void EWTrobotpatrol_updateBody(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Body* body) {
    f32 temp_fa1;

    body->deltaTime = gUpdateRateF / 60.0f;
    body->unk24 = body->unk28 = body->unk2C = 0.0f;
    temp_fa1 = body->unk70 * 0.65f;
    body->unk30 = SQ(body->moveVec.x);
    body->unk34 = SQ(body->moveVec.y);
    body->unk38 = SQ(body->moveVec.z);
    body->unk30 *= temp_fa1;
    body->unk34 *= temp_fa1;
    body->unk38 *= temp_fa1;
    if (body->moveVec.x > 0.0f) {
        body->unk30 = -body->unk30;
    }
    if (body->moveVec.y > 0.0f) {
        body->unk34 = -body->unk34;
    }
    if (body->moveVec.z > 0.0f) {
        body->unk38 = -body->unk38;
    }
    body->unk30 = 0.0f;
    body->unk34 = 0.0f;
    body->unk38 = 0.0f;
    body->unk18 = body->unk24;
    body->unk1C = body->unk28;
    body->unk20 = body->unk2C;
    EWTrobotpatrol_moveAndShoot(self, objdata, body, &objdata->cont);
    body->unk18 += body->moveSin;
    body->unk1C += body->unk4;
    body->unk20 += body->moveCos;
    body->unk48.x = body->unk18 * body->unk7C;
    body->unk48.y = body->unk1C * body->unk7C;
    body->unk48.z = body->unk20 * body->unk7C;
    body->unk48.x *= body->deltaTime;
    body->unk48.y *= body->deltaTime;
    body->unk48.z *= body->deltaTime;
    body->moveVec.x = body->unk48.x + body->moveVec.x;
    body->moveVec.y = body->unk48.y + body->moveVec.y;
    body->moveVec.z = body->unk48.z + body->moveVec.z;
}

// offset: 0x15F8 | func: 11
static s32 EWTrobotpatrol_move(Object* self, EWTrobotpatrol_Body* body, EWTrobotpatrol_Data* objdata, s32 mode) {
    switch (mode) {
    case 0:
        return EWTrobotpatrol_func_1674(self, body, &objdata->destPos);
    case 1:
        return EWTrobotpatrol_func_1920(self, body, &objdata->destPos);
    default:
        return 0; 
    }
}

// offset: 0x1674 | func: 12
static s32 EWTrobotpatrol_func_1674(Object* self, EWTrobotpatrol_Body* body, Vec3f* destPos) {
    Vec3f dir;
    f32 temp_fa0;
    f32 speed;
    f32 turnRate;
    s32 angleToDest;

    if ((body->moveVec.x > -0.01f) && (body->moveVec.x < 0.01f)) {
        body->moveVec.x = 0.0f;
    }
    if ((body->moveVec.z > -0.01f) && (body->moveVec.z < 0.01f)) {
        body->moveVec.z = 0.0f;
    }
    speed = sqrtf(SQ(body->moveVec.f[0]) + SQ(body->moveVec.f[1]) + SQ(body->moveVec.f[2]));
    if (speed < 0.1f) {
        bzero(&body->moveVec, sizeof(body->moveVec));
        bzero(&body->unk48, sizeof(body->unk48));
        body->moveSin = 0.0f;
        body->moveCos = 0.0f;
        return 1;
    }
    temp_fa0 = speed / body->deltaTime;
    dir.x = destPos->x - self->srt.transl.x;
    dir.y = destPos->y - self->srt.transl.y;
    dir.z = destPos->z - self->srt.transl.z;
    if (((dir.f[0] * body->moveVec.f[0]) + (dir.f[1] * body->moveVec.f[1]) + (dir.f[2] * body->moveVec.f[2])) >= 0.0f) {
        turnRate = -body->unk6C * temp_fa0;
    } else {
        turnRate = body->unk6C * temp_fa0;
    }
    if (turnRate > 30.0f) {
        turnRate = 30.0f;
    }
    if (turnRate < -30.0f) {
        turnRate = -30.0f;
    }
    if ((turnRate > -0.1f) && (turnRate < 0.1f)) {
        turnRate = 0.0f;
    }
    angleToDest = mathAtan2f(dir.x, dir.z);
    body->moveSin = mathSinfInterp(angleToDest) * turnRate;
    body->moveCos = mathCosfInterp(angleToDest) * turnRate;
    return 0;
}

// offset: 0x1920 | func: 13
static s32 EWTrobotpatrol_func_1920(Object* self, EWTrobotpatrol_Body* body, Vec3f* destPos) {
    Vec3f dir;
    f32 speedSq;
    f32 temp;
    f32 mag;
    f32 sp44;
    f32 speed;
    s32 _pad[2];
    f32 turnRate;
    s32 angleToDest;
    s32 sp2C;

    turnRate = 0.0f;
    dir.x = destPos->x - self->srt.transl.x;
    dir.y = destPos->y - self->srt.transl.y;
    dir.z = destPos->z - self->srt.transl.z;
    dir.x /= 64;
    dir.y = 0.0f;
    dir.z /= 64;
    mag = sqrtf(SQ(dir.f[0]) + SQ(dir.f[1]) + SQ(dir.f[2]));
    if (((dir.f[0] * body->moveVec.f[0]) + (dir.f[1] * body->moveVec.f[1]) + (dir.f[2] * body->moveVec.f[2])) >= 0.0f) {
        sp44 = -mag;
    } else {
        sp44 = mag;
    }
    speed = sqrtf(SQ(body->moveVec.f[0]) + SQ(body->moveVec.f[1]) + SQ(body->moveVec.f[2]));
    speedSq = SQ(speed);
    temp = (body->unk7C * 30.0f);
    if (mag > 0.05f) {
        sp2C = 0;
        if (mag < (speedSq / (2.0f * temp))) {
            body->unk8E = 0;
            turnRate = body->unk6C * (speedSq / (2.0f * sp44));
        } else {
            if (speed < 0.6f) {
                body->unk8E = 1;
            } else if (speed >= 1.15f) {
                body->unk8E = 0;
            }
            if (body->unk8E != 0) {
                turnRate = mag * 0.5f;
                turnRate *= 30.0f;
            }
        }
    } else {
        sp2C = 1;
    }
    if (turnRate > 30.0f) {
        turnRate = 30.0f;
    }
    if (turnRate < -30.0f) {
        turnRate = -30.0f;
    }
    if ((turnRate > -0.1f) && (turnRate < 0.1f)) {
        turnRate = 0.0f;
    }
    angleToDest = mathAtan2f(dir.x, dir.z);
    body->moveSin = mathSinfInterp(angleToDest) * turnRate;
    body->moveCos = mathCosfInterp(angleToDest) * turnRate;
    return sp2C;
}

// offset: 0x1BF0 | func: 14
static void EWTrobotpatrol_initFx(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Fx* fx) {
    Vec3f sp20[] = {
        VEC3F(9.5f, 4.0f, 0.0f), 
        VEC3F(-9.5f, 4.0f, 0.0f), 
        VEC3F(0.0f, 4.0f, 9.5f), 
        VEC3F(0.0f, 4.0f, -9.5f)
    };

    bcopy(&sp20, fx->unk0, sizeof(sp20));
}

// offset: 0x1C74 | func: 15
static void EWTrobotpatrol_updateFx(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Fx* fx) {
    s32 i;
    f32 temp_fs0;
    f32 temp_fv1;
    s32 temp_s3;
    Vec3f* temp_s0;
    f32 sp8C[4];
    s32 var_v1;
    Vec3f sp7C;
    SRT partSRT;
    TextureAnimator* temp_v0;

    fx->unk30 = 0;
    if (objdata->body.moveSin < 0.0f) {
        fx->unk30 |= 1;
    } else if (objdata->body.moveSin > 0.0f) {
        fx->unk30 |= 2;
    }
    if (objdata->body.moveCos < 0.0f) {
        fx->unk30 |= 4;
    } else if (objdata->body.moveCos > 0.0f) {
        fx->unk30 |= 8;
    }
    if (fx->unk30 & 1) {
        sp8C[0] = 3.0f;
    } else {
        sp8C[0] = 0.0f;
    }
    if (fx->unk30 & 2) {
        sp8C[1] = 3.0f;
    } else {
        sp8C[1] = 0.0f;
    }
    if (fx->unk30 & 4) {
        sp8C[2] = 3.0f;
    } else {
        sp8C[2] = 0.0f;
    }
    if (fx->unk30 & 8) {
        sp8C[3] = 3.0f;
    } else {
        sp8C[3] = 0.0f;
    }
    if (self->modelInstIdx < 2) {
        for (i = 0; i < 4; i++) {
            if (fx->unk30 & (1 << i)) {
                temp_s0 = &fx->unk0[i];
                temp_s3 = mathAtan2f(temp_s0->x, temp_s0->z);
                partSRT.transl.x = temp_s0->x;
                partSRT.transl.y = temp_s0->y;
                partSRT.transl.z = temp_s0->z;
                temp_fs0 = temp_s0->y;
                temp_s0->y = 0.0f;
                temp_fv1 = 1.0f / sqrtf(SQ(partSRT.transl.f[0]) + SQ(partSRT.transl.f[1]) + SQ(partSRT.transl.f[2]));
                sp8C[i] *= 0.01f;
                sp7C.x = temp_s0->x * temp_fv1;
                sp7C.y = temp_s0->y * temp_fv1;
                sp7C.z = temp_s0->z * temp_fv1;
                sp7C.x = sp8C[i] * sp7C.x;
                sp7C.y = sp8C[i] * sp7C.y;
                sp7C.z = sp8C[i] * sp7C.z;
                partSRT.yaw = temp_s3 + 0x1000;
                partSRT.pitch = 0;
                partSRT.roll = 0;
                partSRT.scale = 255.0f;
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_6B, &partSRT, 
                    PARTFXFLAG_4000000 | PARTFXFLAG_2000000 | PARTFXFLAG_10000 | PARTFXFLAG_2, 
                    -1, &sp7C);
                partSRT.yaw = temp_s3 - 0x1000;
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_6B, &partSRT, 
                    PARTFXFLAG_4000000 | PARTFXFLAG_2000000 | PARTFXFLAG_10000 | PARTFXFLAG_2, 
                    -1, &sp7C);
                temp_s0->y = temp_fs0;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        temp_v0 = objExprGetTexAnimator(self, i, 0);
        temp_v0->multiplyR = 255;
        temp_v0->multiplyG = 93;
        temp_v0->multiplyB = 0;
        if (fx->unk30 & (1 << i)) {
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
static void EWTrobotpatrol_initGun(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Gun* gun) {
    gun->animDelta = -0.02f;
    gun->mode = 0;
}

// offset: 0x208C | func: 17
static void EWTrobotpatrol_animateGun(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Gun* gun) {
    if (gun->isDeployed) {
        EWTrobotpatrol_animateDeployedGun(self, objdata, gun);
    }
    gun->isDeployed = self->animProgress >= 1.0f;
    gun->updateRate = gUpdateRate;
    objAnimAdvance(self, gun->animDelta, (f32) gUpdateRate, NULL);
}

// offset: 0x2158 | func: 18
void EWTrobotpatrol_fireGun(Object* self, EWTrobotpatrol_Gun* gun) {
    s32 angle;
    SRT startSRT;
    SRT endSRT;
    DLL_IProjgfx* laserProj;

    angle = gun->targetYaw - (gun->yaw & 0xFFFF);
    CIRCLE_WRAP(angle);
    if ((angle >= -0x1000) && (angle <= 0x1000)) {
        dll_amSfx->Play(self, SOUND_115_ScorpionRobot_LaserFire, MAX_VOLUME, NULL, NULL, 0, NULL);
        endSRT.transl.x = gun->fireAtPoint.x;
        endSRT.transl.y = gun->fireAtPoint.y;
        endSRT.transl.z = gun->fireAtPoint.z;
        endSRT.scale = 1.0f;
        endSRT.yaw = 0;
        endSRT.roll = 0;
        endSRT.pitch = 0;
        startSRT.transl.x = gun->barrelPos.x;
        startSRT.transl.y = gun->barrelPos.y;
        startSRT.transl.z = gun->barrelPos.z;
        startSRT.scale = 1.0f;
        startSRT.yaw = 0;
        startSRT.roll = 0;
        startSRT.pitch = 0;
        laserProj = dllLoad(DLL_ID_193, 1);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_86, &startSRT, PARTFXFLAG_1, -1, NULL);
        laserProj->vtbl->func0(objGetPlayer(), 0, &startSRT, 1, -1, 7, &endSRT);
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

void EWTrobotpatrol_func_231C(Object* self, ModelInstance* modelInst, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** pols) {
    static s16 data_50 = 0;
    Vec3f aimPoint;
    SRT spCC;
    Object* player;
    MtxF* jointMtx;
    f32 spC0;
    Vtx* vtx;
    f32 temp_fv0;
    f32 var_fv1_2;
    f32 ft2;
    EWTrobotpatrol_Gun* gun;
    s32 bone;
    EWTrobotpatrol_Data* objdata;
    f32 var_fa0;
    f32 ft3;
    f32 temp;
    f32 temp2;

    data_50 += 3;
    if (data_50 >= 0x1F) {
        data_50 = 0;
    }
    objdata = self->data;
    gun = &objdata->gun;
    if (objdata->gun.isDeployed == 0) {
        return;
    }
    vtx = *vtxs;
    player = objGetPlayer();
    bone = self->def->pAttachPoints[2].bones[self->modelInstIdx]; // gun base
    jointMtx = (MtxF*) &((f32*)modelInst->matrices[modelInst->unk34 & 1])[bone << 4];
    gun->basePos.x = jointMtx->m[3][0] + gWorldX;
    gun->basePos.y = jointMtx->m[3][1];
    gun->basePos.z = jointMtx->m[3][2] + gWorldZ;
    bone = self->def->pAttachPoints[1].bones[self->modelInstIdx]; // gun barrel
    jointMtx = (MtxF*) &((f32*)modelInst->matrices[modelInst->unk34 & 1])[bone << 4];
    gun->barrelPos.x = jointMtx->m[3][0] + gWorldX;
    gun->barrelPos.y = jointMtx->m[3][1];
    gun->barrelPos.z = jointMtx->m[3][2] + gWorldZ;
    gun->dir.x = gun->barrelPos.x - gun->basePos.x;
    gun->dir.y = gun->barrelPos.y - gun->basePos.y;
    gun->dir.z = gun->barrelPos.z - gun->basePos.z;
    temp_fv0 = sqrtf(SQ(gun->dir.f[0]) + SQ(gun->dir.f[1]) + SQ(gun->dir.f[2]));
    if (temp_fv0 != 0.0f) {
        spC0 = 1.0f / temp_fv0;
    } else {
        spC0 = 0.0f;
    }
    gun->dir.x *= spC0;
    gun->dir.y *= spC0;
    gun->dir.z *= spC0;
    aimPoint.x = gun->dir.x * 300.0f;
    aimPoint.y = gun->dir.y * 300.0f;
    aimPoint.z = gun->dir.z * 300.0f;
    aimPoint.x += gun->barrelPos.x;
    aimPoint.y += gun->barrelPos.y;
    aimPoint.z += gun->barrelPos.z;
    EWTrobotpatrol_aimRaycast(&gun->barrelPos, &aimPoint, &gun->fireAtPoint, &gun->unk2C, player);
    gDPLoadTextureBlockS((*gdl)++,
        /*timg*/sNoiseTexture + 1,
        /*fmt*/G_IM_FMT_IA,
        /*siz*/G_IM_SIZ_8b,
        /*width*/sNoiseTexture->width,
        /*height*/sNoiseTexture->height,
        /*pal*/0,
        /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
        /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
        /*masks*/5,
        /*maskt*/5,
        /*shifts*/G_TX_NOLOD,
        /*shiftt*/G_TX_NOLOD            
    );
    gDPTileSync((*gdl)++);
    gDPLoadMultiBlockS((*gdl)++,
        /*timg*/sLaserBeamTexture + 1,
        /*tmem*/sNoiseTexture->sizeBytes >> 3,
        /*rtile*/1,
        /*fmt*/G_IM_FMT_RGBA,
        /*siz*/G_IM_SIZ_32b,
        /*width*/sLaserBeamTexture->width,
        /*height*/sLaserBeamTexture->height,
        /*pal*/0,
        /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
        /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
        /*masks*/5,
        /*maskt*/5,
        /*shifts*/G_TX_NOLOD,
        /*shiftt*/G_TX_NOLOD    
    );
    dlSetPrimColor(gdl, 0xFF, 0xFF, 0xFF, 0x55);
    gDPSetCombineMode(*gdl, G_CC_DINO_PRIM_RGB_INTERFERENCE_A, G_CC_DINO_MODULATERGB_PRIMA2);
    dlApplyCombine(gdl);
    gDPSetOtherMode(*gdl, 
        G_AD_PATTERN | G_CD_NOISE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_ZB_CLD_SURF2);
    dlApplyOtherMode(gdl);
    spC0 = sqrtf(SQ(gun->dir.x) + SQ(gun->dir.z));
    spCC.yaw = mathAtan2f(gun->dir.x, gun->dir.z);
    spCC.pitch = -mathAtan2f(gun->dir.y, spC0);
    spCC.roll = 0;
    spCC.transl.x = gun->barrelPos.x;
    spCC.transl.y = gun->barrelPos.y;
    spCC.transl.z = gun->barrelPos.z;
    spCC.scale = 0.1f;
    camSetupObjectSRTMatrix(gdl, mtxs, &spCC, 1.0f, 0.0f, NULL);
    bcopy(data_0, *pols, sizeof(data_0));
    gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(*vtxs), 4, 0);
    dlTriangles(gdl, *pols, 2);
    spC0 = sqrtf(SQ(gun->fireAtPoint.x - gun->barrelPos.x) + SQ(gun->fireAtPoint.z - gun->barrelPos.z));
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
    // @bug: The laser pointer tris set up here don't show up because the tex coords are uninitialized
    vtx->v.ob[0] = (s32)temp/*f6*/;
    vtx->v.ob[1] = (s32)ft2/*f8*/ + 14;
    vtx->v.ob[2] = (s32)ft3/*f10*/;
    vtx->v.cn[0] = 0xFF;
    vtx->v.cn[1] = 0;
    vtx->v.cn[2] = 0;
    vtx->v.cn[3] = 0xCD;
    vtx++;

    vtx->v.ob[0] = (s32)ft3/*f10*/;
    vtx->v.ob[2] = (s32)ft3/*f10*/;
    vtx->v.ob[1] = (s32)temp2/*f10*/ - 14;
    vtx->v.cn[0] = 0xFF;
    vtx->v.cn[1] = 0;
    vtx->v.cn[2] = 0;
    vtx->v.cn[3] = 0xCD;
    vtx++;

    vtx->v.ob[0] = (s32)ft3/*f18*/;
    vtx->v.ob[1] = (s32)ft2/*f8*/ + 14;
    vtx->v.ob[2] = var_fv1_2;
    vtx->v.cn[3] = 0x69;
    vtx->v.cn[0] = 0xFF;
    vtx->v.cn[1] = 0;
    vtx->v.cn[2] = 0;
    vtx++;

    vtx->v.ob[0] = (s32)ft3/*f18*/;
    vtx->v.ob[1] = (s32)temp2/*f10*/ - 14;
    vtx->v.ob[2] = var_fv1_2;
    vtx->v.cn[3] = 0x69;
    vtx->v.cn[0] = 0xFF;
    vtx->v.cn[1] = 0;
    vtx->v.cn[2] = 0;
    vtx++;
    spCC.roll = 0x4000;
    camSetupObjectSRTMatrix(gdl, mtxs, &spCC, 1.0f, 0.0f, NULL);
    gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(*vtxs), 4, 0);
    dlTriangles(gdl, *pols, 2);
    *vtxs = vtx;
    *pols += 2;
    if (gun->shouldShoot > 0) {
        EWTrobotpatrol_fireGun(self, gun);
        gun->shouldShoot = 0;
    }
    texRenderReset();
    lightAmbientDL(gdl);
}
#endif

// offset: 0x2C38 | func: 20
s32 EWTrobotpatrol_aimRaycast(Vec3f* barrelPos, Vec3f* aimPoint, Vec3f* fireAtPoint, Vec3f* a3, Object* target) {
    Vec3s16 voxBarrelPos;
    Vec3s16 voxAimPoint;
    Vec3s16 voxHitPos;
    f32 var_fv1;
    s32 _pad;
    s32 _pad2;
    Vec3f hitPos;
    Vec3f sp5C;
    Vec3f sp50;
    Vec3f sp44;
    s8 _pad_sp43;
    s8 sp42;
    s32 _pad3;
    f32 sp38;
    s32 _pad4;

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
    if (a3){} // @fake
    vox_func_80007EE0(barrelPos, &voxBarrelPos);
    vox_func_80007EE0(aimPoint, &voxAimPoint);
    sp5C.f[0] = aimPoint->f[0];
    sp5C.f[1] = aimPoint->f[1];
    sp5C.f[2] = aimPoint->f[2];
    sp38 = 1.0f;
    if (target != NULL) {
        // Hit sphere raycast(?)
        sp42 = func_8002AD3C(target, barrelPos, aimPoint, &sp5C, &sp38);
    } else {
        sp42 = 0;
    }
    fireAtPoint->f[0] = aimPoint->f[0];
    fireAtPoint->f[1] = aimPoint->f[1];
    fireAtPoint->f[2] = aimPoint->f[2];
    a3->f[0] = aimPoint->f[0];
    a3->f[1] = aimPoint->f[1];
    a3->f[2] = aimPoint->f[2];
    if (vox_func_80008048(&voxBarrelPos, &voxAimPoint, &voxHitPos, NULL, 0) == 0) {
        vox_func_80007E2C(&hitPos, &voxHitPos);
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
            fireAtPoint->f[0] = sp5C.f[0];
            fireAtPoint->f[1] = sp5C.f[1];
            fireAtPoint->f[2] = sp5C.f[2];
            return 1;
        } else {
            fireAtPoint->f[0] = sp50.f[0];
            fireAtPoint->f[1] = sp50.f[1];
            fireAtPoint->f[2] = sp50.f[2];
            return 2;
        }
    }
    return 0;
}

// offset: 0x2ED4 | func: 21
static void EWTrobotpatrol_animateDeployedGun(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Gun* gun) {
    s32 yawDelta;
    s32 var_a0;
    s32 targetPitch;
    s32 pitchDelta;
    s32 _pad;
    f32 predictedDirX;
    f32 predictedDirY;
    f32 predictedDirZ;
    SeqJoint* bone0;
    SeqJoint* bone1;
    Vec3f predictedTargetPos;
    s32 yawRange;
    s32 pitchRange;

    bone0 = objExpr_func_80034804(self, 0);
    bone1 = objExpr_func_80034804(self, 1);
    if (gun->target != NULL) {
        predictedTargetPos.f[0] = gun->target->velocity.f[0] * 25.0f;
        predictedTargetPos.f[1] = gun->target->velocity.f[1] * 25.0f;
        predictedTargetPos.f[2] = gun->target->velocity.f[2] * 25.0f;
        predictedTargetPos.f[0] += gun->target->srt.transl.f[0];
        predictedTargetPos.f[1] += gun->target->srt.transl.f[1];
        predictedTargetPos.f[2] += gun->target->srt.transl.f[2];
        predictedDirX = predictedTargetPos.f[0] - gun->basePos.x;
        predictedDirY = predictedTargetPos.f[1] - gun->basePos.y;
        predictedDirZ = predictedTargetPos.f[2] - gun->basePos.z;
        targetPitch = -mathAtan2f(predictedDirY, sqrtf(SQ(predictedDirX) + SQ(predictedDirZ)));
    } else {
        targetPitch = gun->pitch;
    }
    yawDelta = gun->targetYaw - (gun->yaw & 0xFFFF);
    CIRCLE_WRAP(yawDelta);
    pitchDelta = targetPitch - (gun->pitch & 0xFFFF);
    CIRCLE_WRAP(pitchDelta);
    switch (gun->mode) {
    case 2:
        gun->targetYaw = mathAtan2f(-predictedDirX, -predictedDirZ);
        yawRange = 0x800;
        pitchRange = 0x800;
        break;
    case 1:
        if ((yawDelta >= -0xFF) && (yawDelta < 0x100)) {
            gun->targetYaw = mathRnd(0, 0xFFFF);
        }
        yawRange = 0x200;
        pitchRange = 0x200;
        break;
    case 0:
    default:
        yawRange = 0;
        pitchRange = 0;
        break;
    }
    yawDelta *= gUpdateRate;
    yawDelta >>= 3;
    if (yawDelta > yawRange) {
        yawDelta = yawRange;
    }
    if (yawDelta < -yawRange) {
        yawDelta = -yawRange;
    }
    gun->yaw += yawDelta;
    pitchDelta *= gUpdateRate;
    pitchDelta >>= 2;
    if (pitchDelta > pitchRange) {
        pitchDelta = pitchRange;
    }
    if (pitchDelta < -pitchRange) {
        pitchDelta = -pitchRange;
    }
    gun->pitch += pitchDelta;
    bone0->yaw = gun->yaw;
    bone1->pitch = 0x238C - gun->pitch;
}

// offset: 0x31E4 | func: 22
static void EWTrobotpatrol_setGunMode(EWTrobotpatrol_Gun* gun, s32 mode, Object* target) {
    gun->mode = mode;
    gun->target = target;
}

// offset: 0x31F4 | func: 23
static void EWTrobotpatrol_initBeam(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Beam* beam) {
    ObjSetup* beamSetup;
    SRT sp64;
    LightAction laction;
    ObjectShadow* beamShadow;
    Object* beamObj;

    beamSetup = objAllocSetup(sizeof(ObjSetup), OBJ_RobotBeam);
    beamSetup->loadFlags = 8;
    beamSetup->byte5 = 4;
    beamSetup->byte6 = 0x78;
    beamSetup->fadeDistance = 0x78;
    beamSetup->quarterSize = 0x18;
    beamSetup->objId = OBJ_RobotBeam;
    beamObj = objSetupObject(beamSetup, OBJINIT_STANDALONE | OBJINIT_FLAG4, -1, -1, self->parent);
    beamObj->srt.transl.x = self->srt.transl.x;
    beamObj->srt.transl.y = self->srt.transl.y;
    beamObj->srt.transl.z = self->srt.transl.z;
    camGetObjectChildPosition(beamObj, 
        &beamObj->globalPosition.x, 
        &beamObj->globalPosition.y, 
        &beamObj->globalPosition.z);
    sp64.yaw = self->def->pAttachPoints->rot.x;
    sp64.pitch = self->def->pAttachPoints->rot.y;
    sp64.roll = self->def->pAttachPoints->rot.z;
    beam->dir.x = 0.0f;
    beam->dir.y = 1.0f;
    beam->dir.z = 0.0f;
    mathRotateRPY(&sp64, beam->dir.f);
    beamShadow = beamObj->shadow;
    if (beamShadow != NULL) {
        beamShadow->flags |= (OBJ_SHADOW_FLAG_100 | OBJ_SHADOW_FLAG_CUSTOM_COLOR 
            | OBJ_SHADOW_FLAG_CUSTOM_OBJ_POS | OBJ_SHADOW_FLAG_CUSTOM_DIR);
        beamShadow->dir.x = beam->dir.x;
        beamShadow->dir.y = beam->dir.y;
        beamShadow->dir.z = beam->dir.z;
        beamShadow->r = 255;
        beamShadow->g = 255;
        beamShadow->b = 255;
        beamShadow->a = 60;
    }
    beam->obj = beamObj;
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
    beam->unk1A = self->unkD6;
    beam->yawTarget = 0;
    beam->yaw = 0;
}

// offset: 0x3454 | func: 24
static void EWTrobotpatrol_updateBeam(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Beam* beam) {
    Object* beamObj;
    s32 _pad;
    Vec3f targetPos;
    Vec3f dir;
    Vec3s16 voxPos;
    Vec3s16 voxTargetPos;
    Vec3s16 voxHitPos;
    ObjectShadow* beamShadow;
    f32 magnitude;
    s32 yawDelta;

    beamObj = beam->obj;
    beamShadow = beamObj->shadow;
    dir.f[0] = -beamShadow->dir.f[0];
    dir.f[1] = -beamShadow->dir.f[1];
    dir.f[2] = -beamShadow->dir.f[2];
    targetPos.f[0] = dir.f[0] * 200.0f;
    targetPos.f[1] = dir.f[1] * 200.0f;
    targetPos.f[2] = dir.f[2] * 200.0f;
    targetPos.f[0] += beamObj->srt.transl.f[0];
    targetPos.f[1] += beamObj->srt.transl.f[1];
    targetPos.f[2] += beamObj->srt.transl.f[2];
    vox_func_80007EE0(&beamObj->srt.transl, &voxPos);
    vox_func_80007EE0(&targetPos, &voxTargetPos);
    if (vox_func_80008048(&voxPos, &voxTargetPos, &voxHitPos, NULL, 0) == 0) {
        vox_func_80007E2C(&targetPos, &voxHitPos);
        targetPos.f[0] -= beamObj->srt.transl.f[0];
        targetPos.f[1] -= beamObj->srt.transl.f[1];
        targetPos.f[2] -= beamObj->srt.transl.f[2];
        magnitude = sqrtf(SQ(targetPos.f[0]) + SQ(targetPos.f[1]) + SQ(targetPos.f[2])) - 10.0f;
        targetPos.f[0] = dir.f[0] * magnitude;
        targetPos.f[1] = dir.f[1] * magnitude;
        targetPos.f[2] = dir.f[2] * magnitude;
        beamShadow->tr.x = targetPos.f[0] + beamObj->srt.transl.x;
        beamShadow->tr.y = targetPos.f[1] + beamObj->srt.transl.y;
        beamShadow->tr.z = targetPos.f[2] + beamObj->srt.transl.z;
    } else {
        beamShadow->tr.x = beamObj->srt.transl.x;
        beamShadow->tr.y = beamObj->srt.transl.y;
        beamShadow->tr.z = beamObj->srt.transl.z;
    }
    yawDelta = beam->yawTarget - (beam->yaw & 0xFFFF);
    CIRCLE_WRAP(yawDelta);
    yawDelta *= gUpdateRate;
    yawDelta >>= 4;
    if (yawDelta > 0x800) {
        yawDelta = 0x800;
    }
    if (yawDelta < -0x800) {
        yawDelta = -0x800;
    }
    beam->yaw += yawDelta;
    objExpr_func_80034804(self, 2)->yaw = beam->yaw;
    beam->beamTexV += gUpdateRate * 4;
    if (beam->beamTexV >= 0x400) {
        beam->beamTexV = 0;
    }
    // Red pulse animation for the beam when alerted
    if (beam->colorPulseDir == 0) {
        beam->colorPulseTValue += 0.02f * (f32) gUpdateRate;
    } else {
        beam->colorPulseTValue -= 0.02f * (f32) gUpdateRate;
    }
    if (beam->colorPulseTValue < 0.0f) {
        beam->colorPulseTValue = 0.0f;
        beam->colorPulseDir ^= 1;
    } else if (beam->colorPulseTValue > 1.0f) {
        beam->colorPulseTValue = 1.0f;
        beam->colorPulseDir ^= 1;
    }
    objExprGetTexAnimator(beamObj, 0, 0)->positionV = beam->beamTexV;
}

// offset: 0x380C | func: 25
static void EWTrobotpatrol_freeBeam(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Beam* beam, s32 onlySelf) {
    LightAction laction;

    laction.unk12 = 2;
    laction.unke = 0;
    laction.unk1b = 0;
    laction.unk0 = 0;
    laction.unk10 = beam->unk1A;
    gDLL_11_Newlfx->vtbl->func0(self, self, &laction, 0, 0, 0);
    if (onlySelf == 0) {
        if (beam->obj != NULL) {
            objFreeObject(beam->obj);
            beam->obj = NULL;
        }
    }
}

// offset: 0x38C4 | func: 26
static void EWTrobotpatrol_readController(Object* self, EWTrobotpatrol_ControllerState* cont, u8 port) {
    cont->stickX = joyGetStickX(port);
    cont->stickYInv = -joyGetStickY(port);
    cont->buttons = joyGetButtons(port);
    cont->pressed = joyGetPressed(port);
    cont->released = joyGetReleased(port);
    cont->stickXF = (f32) cont->stickX;
    cont->stickYInvF = (f32) cont->stickYInv;
}

// offset: 0x39AC | func: 27
static void EWTrobotpatrol_curveMove(Object* self, EWTrobotpatrol_Data* objdata, EWTrobotpatrol_Body* arg2) {
    f32 xDiff;
    f32 zDiff;
    CurveSetup* destNode;
    EWTrobotpatrol_Data* objdata2;

    objdata2 = self->data;
    destNode = gDLL_26_Curves->vtbl->func_39C(objdata->destCurveUID);
    if (destNode != NULL) {
        objdata->destPos.x = destNode->pos.x;
        objdata->destPos.y = self->srt.transl.y;
        objdata->destPos.z = destNode->pos.z;
        EWTrobotpatrol_move(self, arg2, objdata, 1);
        xDiff = destNode->pos.x - self->srt.transl.x;
        zDiff = destNode->pos.z - self->srt.transl.z;
        objdata2->beam.yawTarget = mathAtan2f(-xDiff, -zDiff);
        xDiff = self->srt.transl.x - destNode->pos.x;
        zDiff = self->srt.transl.z - destNode->pos.z;
        objdata->destDist = sqrtf(SQ(xDiff) + SQ(zDiff));
    }
}

// offset: 0x3AC8 | func: 28
static int EWTrobotpatrol_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 a3) {
    animObjData->unk62 = 0;
    return 0;
}

// offset: 0x3AE4 | func: 29 | export: 7
void EWTrobotpatrol_SetBase(Object* self, Object* base, EWTrobotpatrolCallback callback) {
    EWTrobotpatrol_Data* objdata = self->data;
    objdata->unk74 = base;
    objdata->baseCallback = callback;
}

// offset: 0x3AF8 | func: 30 | export: 8
void EWTrobotpatrol_MoveTo(Object* self, s32 curveUID) {
    EWTrobotpatrol_Data* objdata = self->data;
    objdata->prevCurveUID = objdata->destCurveUID;
    objdata->destCurveUID = curveUID;
    objdata->getNextCurveDebounce = 0;
    objdata->destDist = 1000.0f;
    bzero(&objdata->body.moveVec, sizeof(objdata->body.moveVec));
}

// offset: 0x3B60 | func: 31
static void EWTrobotpatrol_initStunState(EWTrobotpatrol_StunState* stun) {
    stun->timer = 0;
    stun->unk38 = 0;
}

// offset: 0x3B70 | func: 32
static void EWTrobotpatrol_stun(Object* self, EWTrobotpatrol_StunState* stun, f32 dirX, f32 dirY, f32 dirZ, f32 arg5) {
    s32 var_v1;
    SRT sp2C;

    if ((dirX != 0.0f) || (dirY != 0.0f) || (dirZ != 0.0f)) {
        guNormalize(&dirX, &dirY, &dirZ);
    }
    stun->unk36 = mathRnd(8, 12);
    EWTrobotpatrol_func_4004(dirX, dirY, dirZ, &stun->unk6[0], &stun->unk1E[0]);
    var_v1 = 1;
    while (var_v1 < stun->unk36) {
        stun->unk6[var_v1] = stun->unk6[0];
        stun->unk1E[var_v1] = stun->unk1E[0];
        var_v1 += 1;
    }
    stun->timer = 400;
    stun->unk38 += 0x500;
    stun->unk3A = 0xFF;
    stun->unk0 = arg5;
    sp2C.transl.x = dirX * arg5;
    sp2C.transl.y = dirY * arg5;
    sp2C.transl.z = dirZ * arg5;
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_35B, &sp2C, PARTFXFLAG_NONE, -1, NULL);
}

// offset: 0x3D04 | func: 33
static void EWTrobotpatrol_updateStunState(Object* self, EWTrobotpatrol_StunState* stun) {
    SRT sp80;
    f32 temp_fs0;
    s16 temp_s2;
    s16 temp_s3;
    s32 temp_ft0;
    s32 var_t0;
    s32 var_s1;

    if (stun->timer != 0) {
        if (stun->timer >= 0x100) {
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
        for (var_t0 = 0; var_t0 < (gUpdateRate / 2); var_t0++) {
            for (var_s1 = 0; var_s1 < stun->unk36; var_s1++) {
                EWTrobotpatrol_func_40A0(stun->unk6[var_s1], stun->unk1E[var_s1], &sp80.transl, stun->unk0);
                gDLL_17_partfx->vtbl->spawn(self, (var_s1 % 2) + PARTICLE_35C, &sp80, PARTFXFLAG_NONE, -1, NULL);
                temp_s2 = ((0xFFFF / (s16) stun->unk36) * var_s1) + stun->unk38;
                temp_fs0 = mathSinfInterp(temp_s2);
                temp_s3 = (s16) (s32) (((f32) mathRnd(0, 0x600) + (temp_fs0 * 1792.0f)) - 768.0f);
                temp_fs0 = mathCosfInterp(temp_s2);
                temp_ft0 = mathRnd(0, 0x600);
                stun->unk6[var_s1] += temp_s3;
                temp_s3 = (s32) (((f32) temp_ft0 + (temp_fs0 * 1792.0f)) - 768.0f);
                stun->unk1E[var_s1] += temp_s3;
            }
        }
        stun->timer -= gUpdateRate;
        if (stun->timer < 0) {
            stun->timer = 0;
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
