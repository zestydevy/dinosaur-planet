#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/33_BaddieControl.h"
#include "dlls/engine/6_amsfx.h"
#include "game/objects/object_id.h"
#include "sys/dll.h"
#include "sys/gfx/modgfx.h"
#include "sys/gfx/projgfx.h"
#include "sys/camera.h"
#include "sys/main.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/objmsg.h"
#include "sys/rand.h"
#include "dll.h"

typedef struct {
/*0*/ f32 animDelta;
/*4*/ s16 turnStart;
/*6*/ s16 turnDir;
/*8*/ s16 spin; // extra yaw (unused leftover from ScorpionRobot)
/*A*/ s16 spinSpeed; // spin change per tick (unused leftover from ScorpionRobot)
/*C*/ u16 fireCooldown;
/*E*/ u8 hitCount;
/*F*/ u8 enteredState : 1;
/*F*/ u8 fire : 1;
/*F*/ u8 animFinished : 1;
/*F*/ u8 playedDestroyedSound : 1;
} BigScorpionRobot_Data;

enum BigScorpionRobotModAnims {
    BIGSCORP_ROBO_MODANIM_0_Unfold = 0,
    BIGSCORP_ROBO_MODANIM_1 = 1, // walking?
    BIGSCORP_ROBO_MODANIM_2_Fold = 2,
    BIGSCORP_ROBO_MODANIM_3_Firing = 3,
    BIGSCORP_ROBO_MODANIM_4_DamageRecoil = 4,
    BIGSCORP_ROBO_MODANIM_5_Die = 5,
    BIGSCORP_ROBO_MODANIM_6_TurnRight = 6,
    BIGSCORP_ROBO_MODANIM_7_TurnLeft = 7
};

enum BigScorpionRobotStates {
    BIGSCORP_ROBO_STATE_0_Idle = 0,
    BIGSCORP_ROBO_STATE_1_Unfold = 1,
    BIGSCORP_ROBO_STATE_2_Attacking = 2,
    BIGSCORP_ROBO_STATE_3_Fold = 3,
    BIGSCORP_ROBO_STATE_4_DamageRecoil = 4,
    BIGSCORP_ROBO_STATE_5_Dying = 5,
    BIGSCORP_ROBO_STATE_6_Dead = 6
};

/*0x0*/ static u32 data_0[] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffff0000
};
/*0x1C*/ static u16 sHitSounds[] = {
    SOUND_25B_Magic_Attack_Deflected, 
    SOUND_25C_Melee_Attack_Deflected, 
    SOUND_375_Smack1, 
    SOUND_376_Smack2
};

/*0x0*/ static ObjFSA_StateCallback sStateCallbacks[7];
/*0x1C*/ static ObjFSA_StateCallback sAIStateCallbacks[1];

static s32 BigScorpionRobot_state_0_idle(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 BigScorpionRobot_state_1_unfold(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 BigScorpionRobot_state_2_attacking(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 BigScorpionRobot_state_3_fold(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 BigScorpionRobot_state_4_damage_recoil(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 BigScorpionRobot_state_5_dying(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 BigScorpionRobot_state_6_dead(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 BigScorpionRobot_ai_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void BigScorpionRobot_func_0(void) {
    sStateCallbacks[0] = BigScorpionRobot_state_0_idle;
    sStateCallbacks[1] = BigScorpionRobot_state_1_unfold;
    sStateCallbacks[2] = BigScorpionRobot_state_2_attacking;
    sStateCallbacks[3] = BigScorpionRobot_state_3_fold;
    sStateCallbacks[4] = BigScorpionRobot_state_4_damage_recoil;
    sStateCallbacks[5] = BigScorpionRobot_state_5_dying;
    sStateCallbacks[6] = BigScorpionRobot_state_6_dead;

    sAIStateCallbacks[0] = BigScorpionRobot_ai_state_0;
}

// offset: 0x80 | ctor
void BigScorpionRobot_ctor(void* dll) {
    BigScorpionRobot_func_0();
}

// offset: 0xC0 | dtor
void BigScorpionRobot_dtor(void *dll) { }

// offset: 0xCC | func: 1 | export: 0
void BigScorpionRobot_setup(Object* self, Baddie_Setup* setup, s32 reset) {
    u8 var_v0;
    Baddie* baddie = self->data;
    BigScorpionRobot_Data* objdata;
    
    var_v0 = 6;
    if (reset != 0) {
        var_v0 |= 1;
    }
    if (!(setup->unk2B & 0x20)) {
        var_v0 |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, setup, baddie, 7, 1, 0, var_v0, 20.0f);
    self->animCallback = NULL;
    objdata = baddie->objdata;
    bzero(objdata, sizeof(BigScorpionRobot_Data));
    objdata->spin = 0;
    objdata->enteredState = 1;
    objdata->spinSpeed = 0x10E1;
    objAnimSet(self, BIGSCORP_ROBO_MODANIM_2_Fold, 1.0f, 0);
    baddie->fsa.animState = BIGSCORP_ROBO_STATE_0_Idle;
    baddie->fsa.logicState = 0;
    baddie->fsa.flags |= OBJFSA_FLAG_1000000;
    baddie->fsa.hitpoints = 50;
    baddie->unk3B6 = 0;
    baddie->unk3B4 = 0;
    func_8002674C(self);
}

// offset: 0x204 | func: 2 | export: 1
void BigScorpionRobot_control(Object* self) {
    s16 sp68[] = {0x0206, 0x0167, 0x0165, 0x0206}; // texture IDs
    Baddie_Setup* setup = (Baddie_Setup*)self->setup;
    Baddie* baddie = self->data;
    BigScorpionRobot_Data* objdata = baddie->objdata;
    s32 _pad;
    f32 targVec[3];
    Object* hitBy = NULL;
    s32 sp44;
    
    if (self->unkDC == 0) {
        if (self->unkE0 == 0) {
            self->srt.transl.x = setup->base.x;
            self->srt.transl.y = setup->base.y;
            self->srt.transl.z = setup->base.z;
            gDLL_3_Animation->vtbl->start_obj_sequence(setup->unk2E, self, -1);
            self->unkE0 = 1;
            return;
        }
        if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 0) != 0) {
            objdata->spin += objdata->spinSpeed;
            objdata->animFinished = objAnimAdvance(self, objdata->animDelta * 0.3f, gUpdateRateF, NULL);
            if (gUpdateRate < objdata->fireCooldown) {
                objdata->fireCooldown -= gUpdateRate;
            } else {
                objdata->fireCooldown = 0;
            }
            if (baddie->fsa.target != NULL) {
                targVec[0] = baddie->fsa.target->globalPosition.x - self->globalPosition.x;
                targVec[1] = baddie->fsa.target->globalPosition.y - self->globalPosition.y;
                targVec[2] = baddie->fsa.target->globalPosition.z - self->globalPosition.z;
                baddie->fsa.targetDist = sqrtf(SQ(targVec[0]) + SQ(targVec[1]) + SQ(targVec[2]));
            }
            if (!(baddie->unk3B0 & 0x20)) {
                gDLL_33_BaddieControl->vtbl->func14(self, baddie, &baddie->unk3B2, -1, -1, baddie->unk3A6, baddie->unk3A4);
            }
            gDLL_33_BaddieControl->vtbl->func20(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 0);
            if ((baddie->fsa.animState != BIGSCORP_ROBO_STATE_6_Dead) && (baddie->fsa.animState != BIGSCORP_ROBO_STATE_5_Dying)) {
                if ((func_80025F40(self, &hitBy, &sp44, NULL) != 0) && (hitBy != NULL) && (hitBy->id == OBJ_projball)) {
                    if (sp44 == 1) {
                        objdata->hitCount++;
                        gDLL_6_AMSFX->vtbl->play(hitBy, sHitSounds[rand_next(0, 3)], MAX_VOLUME, NULL, NULL, 0, NULL);
                        gDLL_6_AMSFX->vtbl->play(hitBy, sHitSounds[rand_next(0, 3)], MAX_VOLUME, NULL, NULL, 0, NULL);
                        gDLL_6_AMSFX->vtbl->play(self, SOUND_6E7_ScorpionRobot_Damaged, MAX_VOLUME, NULL, NULL, 0, NULL);
                        camUseShake();
                        camSetShakeOffset(1.0f);
                        gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, BIGSCORP_ROBO_STATE_4_DamageRecoil);
                        objdata->enteredState = 1;
                    } else {
                        gDLL_6_AMSFX->vtbl->play(self, SOUND_6E7_ScorpionRobot_Damaged, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                }
                if (objdata->hitCount >= 6) {
                    gDLL_6_AMSFX->vtbl->play(self, SOUND_6F8_ScorpionRobot_Destroyed, MAX_VOLUME, NULL, NULL, 0, NULL);
                    camUseShake();
                    camSetShakeOffset(6.0f);
                    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, BIGSCORP_ROBO_STATE_5_Dying);
                    objdata->enteredState = 1;
                    baddie->fsa.target = NULL;
                }
            }
            gDLL_33_BaddieControl->vtbl->func10(self, &baddie->fsa, 0.0f, -1);
            baddie->unk3AC = self->animObj;
            self->animObj = NULL;
            gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, gUpdateRateF, gUpdateRateF, sStateCallbacks, sAIStateCallbacks);
            self->animObj = baddie->unk3AC;
        }
    }
}

// offset: 0x7B0 | func: 3 | export: 2
void BigScorpionRobot_update(Object *self) { }

// offset: 0x7BC | func: 4 | export: 3
void BigScorpionRobot_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie = self->data;
    s32 bone;
    BigScorpionRobot_Data* objdata = baddie->objdata;
    ModelInstance* modelInst;
    s32 _pad;
    MtxF* mtx;
    DLL_IProjgfx* projgfx;
    
    if ((visibility != 0) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        if ((objdata->fire) && (baddie->fsa.target != NULL)) {
            SRT srcSRT;
            SRT dstSRT;
            modelInst = self->modelInsts[self->modelInstIdx];
            bone = self->def->pAttachPoints->bones[self->modelInstIdx]; // model index as a bone index?
            mtx = (MtxF *)&((f32 **)modelInst->matrices[modelInst->unk34 & 1])[bone << 4];
            projgfx = dllLoadDeferred(DLL_ID_193, 1);
            gDLL_6_AMSFX->vtbl->play(self, SOUND_115_ScorpionRobot_LaserFire, MAX_VOLUME, NULL, NULL, 0, NULL);
            srcSRT.transl.x = mtx->m[3][0] + gWorldX;
            srcSRT.transl.y = mtx->m[3][1];
            srcSRT.transl.z = mtx->m[3][2] + gWorldZ;
            srcSRT.scale = 1.0f;
            srcSRT.yaw = 0;
            srcSRT.roll = 0;
            srcSRT.pitch = 0;
            mainGetBufferedPlayerPos(0.5f, &dstSRT.transl.x, &dstSRT.transl.y, &dstSRT.transl.z);
            dstSRT.yaw = 0;
            dstSRT.roll = 0;
            dstSRT.pitch = 0;
            dstSRT.scale = 1.0f;
            projgfx->vtbl->func0(baddie->fsa.target, 0, &srcSRT, 1, -1, 7, &dstSRT);
            if (projgfx != NULL) {
                dllFree(projgfx);
            }
        }
        objdata->fire = 0;
    }
}

// offset: 0x9E4 | func: 5 | export: 4
void BigScorpionRobot_free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    obj_free_object_type(self, OBJTYPE_Baddie);
    if (self->linkedObject != NULL) {
        objFreeObject(self->linkedObject);
        self->linkedObject = NULL;
    }
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
}

// offset: 0xA88 | func: 6 | export: 5
u32 BigScorpionRobot_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_8;
}

// offset: 0xA98 | func: 7 | export: 6
u32 BigScorpionRobot_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(Baddie) + sizeof(BigScorpionRobot_Data);
}

// offset: 0xAAC | func: 8
static s32 BigScorpionRobot_is_obj_in_range(Object* self, Object* obj, f32 range) {
    f32 xDiff;
    f32 yDiff;
    f32 zDiff;

    if ((self == NULL) || (obj == NULL)) {
        return 0;
    }
    xDiff = self->srt.transl.x - obj->srt.transl.x;
    yDiff = self->srt.transl.y - obj->srt.transl.y;
    zDiff = self->srt.transl.z - obj->srt.transl.z;

    return (SQ(xDiff) + SQ(yDiff) + SQ(zDiff)) <= SQ(range);
}

// offset: 0xB28 | func: 9
static void BigScorpionRobot_ramp_up_spin_speed(BigScorpionRobot_Data* objdata) {
    if (objdata->spinSpeed < 0x10E1) {
        objdata->spinSpeed += (gUpdateRate * 0x46);
        if (objdata->spinSpeed > 0x10E1) {
            objdata->spinSpeed = 0x10E1;
        }
    }
}

// offset: 0xB88 | func: 10
static void BigScorpionRobot_ramp_down_spin_speed(BigScorpionRobot_Data* objdata) {
    if (objdata->spinSpeed >= (gUpdateRate * 40)) {
        objdata->spinSpeed -= (gUpdateRate * 40);
    } else {
        objdata->spinSpeed = 0;
    }
}

// offset: 0xBCC | func: 11 | state 0
static s32 BigScorpionRobot_state_0_idle(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    BigScorpionRobot_Data* objdata = baddie->objdata;
    Object* player = objGetPlayer();
    
    fsa->unk341 = 0;
    if (BigScorpionRobot_is_obj_in_range(self, player, (f32) baddie->unk3E2) != 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, BIGSCORP_ROBO_STATE_1_Unfold);
        objdata->enteredState = 1;
        fsa->target = player;
    }
    objdata->spinSpeed = 0;
    return 0;
}

// offset: 0xCC0 | func: 12 | state 1
static s32 BigScorpionRobot_state_1_unfold(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    BigScorpionRobot_Data* objdata = baddie->objdata;
    
    fsa->unk341 = 1;
    if (objdata->enteredState) {
        objdata->enteredState = 0;
        objAnimSet(self, BIGSCORP_ROBO_MODANIM_0_Unfold, 0.0f, 0);
        objdata->animDelta = 0.024f;
        gDLL_6_AMSFX->vtbl->play(self, SOUND_6E4_ScorpionRobot_Activate, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    if ((self->curModAnimId == BIGSCORP_ROBO_MODANIM_0_Unfold) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, BIGSCORP_ROBO_STATE_2_Attacking);
        objdata->enteredState = 1;
    }
    BigScorpionRobot_ramp_down_spin_speed(objdata);
    return 0;
}

// offset: 0xE48 | func: 13 | state 2
static s32 BigScorpionRobot_state_2_attacking(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    BigScorpionRobot_Data* objdata = baddie->objdata;
    s16 temp_v0_4;
    s32 modanimIdx;
    s16 temp;
    
    fsa->unk341 = 1;
    if (objdata->enteredState) {
        objdata->enteredState = 0;
    }
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    if (objdata->animFinished) {
        objAnimSet(self, BIGSCORP_ROBO_MODANIM_0_Unfold, 1.0f, 0);
        objdata->animDelta = 0.0f;
    }
    if ((self->curModAnimId == BIGSCORP_ROBO_MODANIM_7_TurnLeft) || (self->curModAnimId == BIGSCORP_ROBO_MODANIM_6_TurnRight)) {
        self->srt.yaw = (s16) ((f32) objdata->turnStart + (5461.3335f * (f32) objdata->turnDir * self->animProgress));
    }
    if ((self->curModAnimId == BIGSCORP_ROBO_MODANIM_0_Unfold) && (self->animProgress == 1.0f)) {
        if (fsa->target != NULL) {
            temp = (u16)arctan2_f(
                self->srt.transl.x - fsa->target->srt.transl.x, 
                self->srt.transl.z - fsa->target->srt.transl.z);
            temp_v0_4 = (u16)rotation16_sub_wrap(
                temp, 
                self->srt.yaw + objdata->spin);
            if (temp_v0_4 > 0x2000) {
                modanimIdx = BIGSCORP_ROBO_MODANIM_6_TurnRight;
                objdata->turnDir = -1;
            } else if (temp_v0_4 < -0x2000) {
                modanimIdx = BIGSCORP_ROBO_MODANIM_7_TurnLeft;
                objdata->turnDir = 1;
            } else {
                modanimIdx = -1;
            }
            if (modanimIdx != -1) {
                objdata->animDelta = 0.04f;
                objdata->turnStart = self->srt.yaw;
                objAnimSet(self, modanimIdx, 0.0f, 0);
                gDLL_6_AMSFX->vtbl->play(self, SOUND_6E5_ScorpionRobot_Moving, MAX_VOLUME, NULL, NULL, 0, NULL);
            } else if (objdata->fireCooldown == 0) {
                objdata->fireCooldown = rand_next(0x96, 0x138);
                objdata->fire = 1;
                objdata->animDelta = 0.04f;
                objAnimSet(self, BIGSCORP_ROBO_MODANIM_3_Firing, 0.0f, 0);
            }
        }
    }
    if (BigScorpionRobot_is_obj_in_range(self, fsa->target, (f32) baddie->unk3E2 * 1.2f) == 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, BIGSCORP_ROBO_STATE_3_Fold);
        objdata->enteredState = 1;
        fsa->target = NULL;
    }
    BigScorpionRobot_ramp_down_spin_speed(objdata);
    return 0;
}

// offset: 0x11CC | func: 14 | state 3
static s32 BigScorpionRobot_state_3_fold(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    BigScorpionRobot_Data* objdata = baddie->objdata;
    
    fsa->unk341 = 0;
    if (objdata->enteredState) {
        objdata->enteredState = 0;
        objAnimSet(self, BIGSCORP_ROBO_MODANIM_2_Fold, 0.0f, 0);
        objdata->animDelta = 0.024f;
        gDLL_6_AMSFX->vtbl->play(self, SOUND_6E4_ScorpionRobot_Activate, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    if ((self->curModAnimId == BIGSCORP_ROBO_MODANIM_2_Fold) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, BIGSCORP_ROBO_STATE_0_Idle);
        objdata->enteredState = 1;
        fsa->target = NULL;
    }
    // @bug: Big robo doesn't spin, so incrementing the spin speed here desyncs its rotation for the next attack
    BigScorpionRobot_ramp_up_spin_speed(objdata);
    return 0;
}

// offset: 0x1358 | func: 15 | state 4
static s32 BigScorpionRobot_state_4_damage_recoil(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    BigScorpionRobot_Data* objdata = baddie->objdata;

    fsa->unk341 = 0;
    if (objdata->enteredState) {
        objdata->enteredState = 0;
        objAnimSet(self, BIGSCORP_ROBO_MODANIM_4_DamageRecoil, 0.0f, 0);
        objdata->animDelta = 0.02f;
    }
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    if ((self->curModAnimId == BIGSCORP_ROBO_MODANIM_4_DamageRecoil) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, BIGSCORP_ROBO_STATE_2_Attacking);
        objdata->enteredState = 1;
    }
    BigScorpionRobot_ramp_down_spin_speed(objdata);
    return 0;
}

// offset: 0x14A4 | func: 16 | state 5
static s32 BigScorpionRobot_state_5_dying(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    BigScorpionRobot_Data* objdata = baddie->objdata;

    fsa->unk341 = 0;
    if (objdata->enteredState) {
        objdata->enteredState = 0;
        objAnimSet(self, BIGSCORP_ROBO_MODANIM_5_Die, 0.0f, 0);
        objdata->animDelta = 0.012f;
    }
    if ((self->animProgress > 0.186f) && !objdata->playedDestroyedSound) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_6F8_ScorpionRobot_Destroyed, MAX_VOLUME, NULL, NULL, 0, NULL);
        objdata->playedDestroyedSound = 1;
    }
    if ((self->curModAnimId == BIGSCORP_ROBO_MODANIM_5_Die) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, BIGSCORP_ROBO_STATE_6_Dead);
        objdata->enteredState = 1;
        fsa->target = NULL;
    }
    BigScorpionRobot_ramp_down_spin_speed(objdata);
    return 0;
}

// offset: 0x161C | func: 17 | state 6
static s32 BigScorpionRobot_state_6_dead(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    BigScorpionRobot_Data* objdata = baddie->objdata;
    u8 partCount = 30; // why init up here?
    DLL_IModgfx* modgfx;
    
    fsa->unk341 = 3;
    if (objdata->enteredState) {
        while (partCount--) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_555, NULL, PARTFXFLAG_2, -1, NULL);
        }
        modgfx = dllLoadDeferred(DLL_ID_107, 1);
        objdata->enteredState = 0;
        gDLL_33_BaddieControl->vtbl->func18(self, baddie->unk3E0, -1, 0);
        modgfx->vtbl->func0(self, 0x11, 0, 2, -1, 0);
        modgfx->vtbl->func0(self, 0x11, 0, 2, -1, 0);
        modgfx->vtbl->func0(self, 0x11, 0, 2, -1, 0);
        baddie->unk3B4 = 0;
        mainSetBits(baddie->unk39C, 1);
        dllFree(modgfx);
        if (self->setup == NULL) {
            objFreeObject(self);
        }
        gDLL_6_AMSFX->vtbl->play(self, SOUND_6F8_ScorpionRobot_Destroyed, MAX_VOLUME, NULL, NULL, 0, NULL);
        camUseShake();
        camSetShakeOffset(6.0f);
        obj_send_mesg_many(0, OBJMSG_SEND_ALL | OBJMSG_SEND_IGNORE_SENDER, self, 0xE0000, self);
    }
    if (self->opacity >= (gUpdateRate * 3)) {
        self->opacity -= (gUpdateRate * 3);
    } else {
        self->opacity = 0;
    }
    BigScorpionRobot_ramp_down_spin_speed(objdata);
    return 0;
}

// offset: 0x18DC | func: 18 | ai state 0
static s32 BigScorpionRobot_ai_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
}
