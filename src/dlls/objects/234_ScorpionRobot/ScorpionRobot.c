#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/27.h"
#include "dlls/engine/33_BaddieControl.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/objmsg.h"
#include "sys/rand.h"
#include "dll.h"

typedef struct {
/*0*/ f32 animDelta;
/*4*/ s16 turnStart;
/*6*/ s16 turnDir;
/*8*/ s16 spin; // extra yaw
/*A*/ s16 spinSpeed; // spin change per tick
/*C*/ u16 fireCooldown;
/*E*/ u8 enteredState : 1;
/*E*/ u8 fire : 1;
/*E*/ u8 animFinished : 1;
} ScorpionRobot_Data;

enum ScorpionRobotModAnims {
    SCORP_ROBO_MODANIM_0_Unfold = 0,
    SCORP_ROBO_MODANIM_1 = 1, // walking?
    SCORP_ROBO_MODANIM_2_Fold = 2,
    SCORP_ROBO_MODANIM_3_Firing = 3,
    SCORP_ROBO_MODANIM_4_DamageRecoil = 4,
    SCORP_ROBO_MODANIM_5_Die = 5,
    SCORP_ROBO_MODANIM_6_TurnRight = 6,
    SCORP_ROBO_MODANIM_7_TurnLeft = 7
};

enum ScorpionRobotStates {
    SCORP_ROBO_STATE_0_Spinning = 0,
    SCORP_ROBO_STATE_1_WaitForPlayer = 1,
    SCORP_ROBO_STATE_2_Unfold = 2,
    SCORP_ROBO_STATE_3_Attacking = 3,
    SCORP_ROBO_STATE_4_Fold = 4,
    SCORP_ROBO_STATE_5_DamageRecoil = 5,
    SCORP_ROBO_STATE_6_Dying = 6,
    SCORP_ROBO_STATE_7_Dead = 7
};

/*0x0*/ static s8 sHitDamageMap[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1
};
/*0x1C*/ static u16 sHitSounds[] = {
    SOUND_25B_Magic_Attack_Deflected, 
    SOUND_25C_Melee_Attack_Deflected, 
    SOUND_375_Smack1, 
    SOUND_376_Smack2
};
/*0x24*/ static s32 data_24[] = {2};

/*0x0*/ static ObjFSA_StateCallback sStateCallbacks[8];
/*0x20*/ static ObjFSA_StateCallback sAIStateCallbacks[1];

static s32 ScorpionRobot_state_0_spinning(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 ScorpionRobot_state_1_wait_for_player(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 ScorpionRobot_state_2_unfold(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 ScorpionRobot_state_3_attacking(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 ScorpionRobot_state_4_fold(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 ScorpionRobot_state_5_damage_recoil(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 ScorpionRobot_state_6_dying(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 ScorpionRobot_state_7_dead(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 ScorpionRobot_ai_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void ScorpionRobot_setup_state_callbacks(void) {
    sStateCallbacks[0] = ScorpionRobot_state_0_spinning;
    sStateCallbacks[1] = ScorpionRobot_state_1_wait_for_player;
    sStateCallbacks[2] = ScorpionRobot_state_2_unfold;
    sStateCallbacks[3] = ScorpionRobot_state_3_attacking;
    sStateCallbacks[4] = ScorpionRobot_state_4_fold;
    sStateCallbacks[5] = ScorpionRobot_state_5_damage_recoil;
    sStateCallbacks[6] = ScorpionRobot_state_6_dying;
    sStateCallbacks[7] = ScorpionRobot_state_7_dead;
    
    sAIStateCallbacks[0] = ScorpionRobot_ai_state_0;
}

// offset: 0x8C | ctor
void ScorpionRobot_ctor(void* dll) {
    ScorpionRobot_setup_state_callbacks();
}

// offset: 0xCC | dtor
void ScorpionRobot_dtor(void *dll) { }

// offset: 0xD8 | func: 1 | export: 0
void ScorpionRobot_setup(Object* self, Baddie_Setup* setup, s32 reset) {
    u8 baddieFlags;
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata;
    
    baddieFlags = 0x2 | 0x4 | 0x10;
    if (reset != 0) {
        baddieFlags |= 1;
    }
    if (!(setup->unk2B & 0x20)) {
        baddieFlags |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, setup, baddie, 8, 1, 0x108, baddieFlags, 20.0f);
    
    self->animCallback = NULL;

    objdata = baddie->objdata;
    bzero(objdata, sizeof(ScorpionRobot_Data));
    objdata->spin = 0;
    objdata->enteredState = 1;
    objdata->spinSpeed = 0x10E1;

    objAnimSet(self, SCORP_ROBO_MODANIM_2_Fold, 1.0f, 0);
    baddie->fsa.animState = SCORP_ROBO_STATE_0_Spinning;
    baddie->fsa.logicState = 0;
    baddie->fsa.flags |= OBJFSA_FLAG_1000000;
    baddie->fsa.hitpoints = 12;
    baddie->unk3B6 = 0;
    baddie->unk3B4 = 0;
    baddie->fsa.unk4.mode = DLL27MODE_DISABLED;
    func_800267A4(self);
}

// offset: 0x218 | func: 2 | export: 1
void ScorpionRobot_control(Object* self) {
    s16 sp90[] = {0x206, 0x167, 0x165, 0x206}; // texture IDs
    Baddie_Setup* setup = (Baddie_Setup*)self->setup;
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;
    s32 var_v1;
    Object* player = objGetPlayer();
    f32 sp70[3];
    SRT hitSRT;
    s32 sp54;
    s32 _pad;
    
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
            objdata->animFinished = objAnimAdvance(self, objdata->animDelta, gUpdateRateF, NULL);
            if (gUpdateRate < objdata->fireCooldown) {
                objdata->fireCooldown -= gUpdateRate;
            } else {
                objdata->fireCooldown = 0;
            }
            if (objdata->animFinished) {
                if (self->curModAnimId == SCORP_ROBO_MODANIM_2_Fold) {
                    func_800267A4(self);
                } else if (self->curModAnimId == SCORP_ROBO_MODANIM_0_Unfold) {
                    func_8002674C(self);
                }
            }
            if (baddie->fsa.target != NULL) {
                sp70[0] = baddie->fsa.target->globalPosition.x - self->globalPosition.x;
                sp70[1] = baddie->fsa.target->globalPosition.y - self->globalPosition.y;
                sp70[2] = baddie->fsa.target->globalPosition.z - self->globalPosition.z;
                baddie->fsa.targetDist = sqrtf(SQ(sp70[0]) + SQ(sp70[1]) + SQ(sp70[2]));
            }
            if (!(baddie->unk3B0 & 0x20)) {
                gDLL_33_BaddieControl->vtbl->func14(self, baddie, &baddie->unk3B2, -1, -1,  baddie->unk3A6, baddie->unk3A4);
            }
            gDLL_33_BaddieControl->vtbl->func20(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 0);
            if (baddie->fsa.hitpoints > 0) {
                if (gDLL_33_BaddieControl->vtbl->check_hit(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, NULL, sHitDamageMap, 0, &baddie->unk3A8, &hitSRT) != 0) {
                    var_v1 = ((DLL_Unknown*)player->linkedObject->dll)->vtbl->func[19].withOneVoidArgS32(player->linkedObject);
                    if (var_v1 >= 4) {
                        var_v1 = 3;
                    }
                    hitSRT.scale = (f32) sp90[var_v1];
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, &hitSRT, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                    hitSRT.transl.x -= self->srt.transl.x;
                    hitSRT.transl.y -= self->srt.transl.y;
                    hitSRT.transl.z -= self->srt.transl.z;
                    hitSRT.scale = (f32) sp90[var_v1];
                    sp54 = 4;
                    while (sp54--) {
                        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_324, &hitSRT, PARTFXFLAG_2, -1, NULL);
                    }
                    gDLL_6_AMSFX->vtbl->play(self, sHitSounds[rand_next(0, 3)], MAX_VOLUME, NULL, NULL, 0, NULL);
                    gDLL_6_AMSFX->vtbl->play(self, SOUND_6E7_ScorpionRobot_Damaged, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
            } else {
                if ((baddie->fsa.animState != SCORP_ROBO_STATE_7_Dead) && (baddie->fsa.animState != SCORP_ROBO_STATE_6_Dying)) {
                    gDLL_6_AMSFX->vtbl->play(self, SOUND_6F8_ScorpionRobot_Destroyed, MAX_VOLUME, NULL, NULL, 0, NULL);
                    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, SCORP_ROBO_STATE_6_Dying);
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

// offset: 0x85C | func: 3 | export: 2
void ScorpionRobot_update(Object *self) { }

// offset: 0x868 | func: 4 | export: 3
void ScorpionRobot_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie = self->data;
    s32 _pad;
    ScorpionRobot_Data* objdata = baddie->objdata;

    if ((visibility != 0) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        if ((objdata->fire) && (baddie->fsa.target != NULL)) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_115_ScorpionRobot_LaserFire, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        objdata->fire = 0;
    }
}

// offset: 0x948 | func: 5 | export: 4
void ScorpionRobot_free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    obj_free_object_type(self, OBJTYPE_Baddie);
    if (self->linkedObject != NULL) {
        objFreeObject(self->linkedObject);
        self->linkedObject = NULL;
    }
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
}

// offset: 0x9EC | func: 6 | export: 5
u32 ScorpionRobot_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_8;
}

// offset: 0x9FC | func: 7 | export: 6
u32 ScorpionRobot_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(Baddie) + sizeof(ScorpionRobot_Data);
}

// offset: 0xA10 | func: 8
static s32 ScorpionRobot_is_obj_in_range(Object* self, Object* obj, f32 range) {
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

// offset: 0xA8C | func: 9 | export: 7
s32 ScorpionRobot_get_spin(Object* self) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;

    return objdata->spin;
}

// offset: 0xAA0 | func: 10
static void ScorpionRobot_ramp_up_spin_speed(ScorpionRobot_Data* objdata) {
    if (objdata->spinSpeed < 0x10E1) {
        objdata->spinSpeed += (gUpdateRate * 0x46);
        if (objdata->spinSpeed > 0x10E1) {
            objdata->spinSpeed = 0x10E1;
        }
    }
}

// offset: 0xB00 | func: 11
static void ScorpionRobot_ramp_down_spin_speed(ScorpionRobot_Data* objdata) {
    if (objdata->spinSpeed >= (gUpdateRate * 40)) {
        objdata->spinSpeed -= (gUpdateRate * 40);
    } else {
        objdata->spinSpeed = 0;
    }
}

// offset: 0xB44 | func: 12 | anim state 0
static s32 ScorpionRobot_state_0_spinning(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;
    UnkCurvesStruct* sp54 = baddie->unk3F8;
    f32 sp50 = self->velocity.x;
    f32 sp4C = self->velocity.z;
    f32 temp;
    f32 temp_fs0;
    f32 temp_fa1;
    Object* player = objGetPlayer();
    
    fsa->unk341 = 0;
    if (objdata->enteredState) {
        objdata->enteredState = 0;
        baddie->unk3B4 = 0;
        baddie->unk3B6 = 0;
        if ((baddie->unk3F8 != NULL) && (gDLL_26_Curves->vtbl->func_4288(baddie->unk3F8, self, 500.0f, data_24, -1) != 0)) {
            baddie->unk3B2 &= ~8;
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SCORP_ROBO_STATE_1_WaitForPlayer);
            objdata->enteredState = 1;
            return 0;
        }
        baddie->unk3B2 |= 8;
    }

    if (baddie->unk3B2 & 8) {
        temp_fs0 = sp54->unk0.unk68.x - self->srt.transl.x;
        temp_fa1 = sp54->unk0.unk68.z - self->srt.transl.z;
        temp = 10.0f / sqrtf(SQ(temp_fs0) + SQ(temp_fa1));
        if (((curves_func_800053B0(&sp54->unk0, temp) != 0) || (sp54->unk0.unk10 != 0)) 
                && (gDLL_26_Curves->vtbl->func_4704(sp54) != 0) 
                && (gDLL_26_Curves->vtbl->func_4288(baddie->unk3F8, self, 500.0f, data_24, -1) != 0)) {
            baddie->unk3B2 &= ~8;
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SCORP_ROBO_STATE_1_WaitForPlayer);
            objdata->enteredState = 1;
            return 0;
        }
        temp_fs0 = (sp54->unk0.unk68.x - self->srt.transl.x) * 0.02f;
        temp_fa1 = (sp54->unk0.unk68.z - self->srt.transl.z) * 0.02f;
        if (temp_fs0 > 0.25f) {
            temp_fs0 = 0.25f;
        } else if (temp_fs0 < -0.25f) {
            temp_fs0 = -0.25f;
        }
        if (temp_fa1 > 0.25f) {
            temp_fa1 = 0.25f;
        } else if (temp_fa1 < -0.25f) {
            temp_fa1 = -0.25f;
        }
        self->velocity.x += temp_fs0;
        self->velocity.z += temp_fa1;
    }

    self->velocity.x *= 0.97f;
    self->velocity.z *= 0.97f;
    fsa->flags |= 0x4000;
    temp_fs0 = self->velocity.x - sp50;
    temp_fa1 = self->velocity.z - sp4C;
    self->srt.yaw = atan2f_to_s(temp_fs0, temp_fa1);
    self->srt.pitch = (s16) (sqrtf((temp_fs0 * temp_fs0) + (temp_fa1 * temp_fa1)) * 10000.0f);
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);
    if (ScorpionRobot_is_obj_in_range(self, player, (f32) baddie->unk3E2) != 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SCORP_ROBO_STATE_2_Unfold);
        objdata->enteredState = 1;
        fsa->target = player;
    }
    ScorpionRobot_ramp_up_spin_speed(objdata);
    return 0;
}

// offset: 0x100C | func: 13 | anim state 1
static s32 ScorpionRobot_state_1_wait_for_player(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;
    Object* player = objGetPlayer();
    
    fsa->unk341 = 0;
    if (ScorpionRobot_is_obj_in_range(self, player, (f32) baddie->unk3E2) != 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SCORP_ROBO_STATE_2_Unfold);
        objdata->enteredState = 1;
        fsa->target = player;
    }
    objdata->spinSpeed = 0;
    return 0;
}

// offset: 0x1100 | func: 14 | anim state 2
static s32 ScorpionRobot_state_2_unfold(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;

    fsa->unk341 = 1;
    if (objdata->enteredState) {
        objdata->enteredState = 0;
        objAnimSet(self, SCORP_ROBO_MODANIM_0_Unfold, 0.0f, 0);
        objdata->animDelta = 0.02f;
        gDLL_6_AMSFX->vtbl->play(self, SOUND_6E4_ScorpionRobot_Activate, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    self->velocity.x *= 0.97f;
    self->velocity.z *= 0.97f;
    fsa->flags |= 0x4000;
    self->srt.pitch -= (self->srt.pitch >> 2);
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);
    if ((self->curModAnimId == SCORP_ROBO_MODANIM_0_Unfold) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SCORP_ROBO_STATE_3_Attacking);
        objdata->enteredState = 1;
    }
    ScorpionRobot_ramp_down_spin_speed(objdata);
    return 0;
}

// offset: 0x12F4 | func: 15 | anim state 3
static s32 ScorpionRobot_state_3_attacking(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;
    s16 temp_v0_4;
    s32 modanimIdx;
    s16 temp;

    fsa->unk341 = 1;
    if (objdata->enteredState) {
        objdata->enteredState = 0;
    }
    self->velocity.x *= 0.85f;
    self->velocity.z *= 0.85f;
    fsa->flags |= 0x4000;
    self->srt.pitch -= (self->srt.pitch >> 2);
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);
    if (objdata->animFinished) {
        objAnimSet(self, SCORP_ROBO_MODANIM_0_Unfold, 1.0f, 0);
        objdata->animDelta = 0.0f;
    }
    if ((self->curModAnimId == SCORP_ROBO_MODANIM_7_TurnLeft) || (self->curModAnimId == SCORP_ROBO_MODANIM_6_TurnRight)) {
        self->srt.yaw = (s16) ((f32) objdata->turnStart + (5461.3335f * (f32) objdata->turnDir * self->animProgress));
    }
    if ((self->curModAnimId == SCORP_ROBO_MODANIM_0_Unfold) && (self->animProgress == 1.0f)) {
        if (fsa->target != NULL) {
            temp = (u16)arctan2_f(
                    self->srt.transl.x - fsa->target->srt.transl.x, 
                    self->srt.transl.z - fsa->target->srt.transl.z);
            temp_v0_4 = (u16)rotation16_sub_wrap(
                temp, 
                self->srt.yaw + objdata->spin);
            if (temp_v0_4 > 0x2000) {
                modanimIdx = SCORP_ROBO_MODANIM_6_TurnRight;
                objdata->turnDir = -1;
            } else if (temp_v0_4 < -0x2000) {
                modanimIdx = SCORP_ROBO_MODANIM_7_TurnLeft;
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
                objdata->fireCooldown = rand_next(0x5A, 0xD2);
                objdata->fire = 1;
                objdata->animDelta = 0.04f;
                objAnimSet(self, SCORP_ROBO_MODANIM_3_Firing, 0.0f, 0);
            }
        }
    }
    if (ScorpionRobot_is_obj_in_range(self, fsa->target, (f32) baddie->unk3E2 * 1.5f) == 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SCORP_ROBO_STATE_4_Fold);
        objdata->enteredState = 1;
        fsa->target = NULL;
    }
    ScorpionRobot_ramp_down_spin_speed(objdata);
    return 0;
}

// offset: 0x16E0 | func: 16 | anim state 4
static s32 ScorpionRobot_state_4_fold(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;

    fsa->unk341 = 0;
    if (objdata->enteredState) {
        objdata->enteredState = 0;
        objAnimSet(self, SCORP_ROBO_MODANIM_2_Fold, 0.0f, 0);
        objdata->animDelta = 0.01f;
        gDLL_6_AMSFX->vtbl->play(self, SOUND_6E4_ScorpionRobot_Activate, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    self->velocity.x *= 0.97f;
    self->velocity.z *= 0.97f;
    fsa->flags |= 0x4000;
    self->srt.pitch -= (self->srt.pitch >> 2);
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);
    if ((self->curModAnimId == SCORP_ROBO_MODANIM_2_Fold) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SCORP_ROBO_STATE_0_Spinning);
        objdata->enteredState = 1;
        fsa->target = NULL;
    }
    ScorpionRobot_ramp_up_spin_speed(objdata);
    return 0;
}

// offset: 0x18D4 | func: 17 | anim state 5
static s32 ScorpionRobot_state_5_damage_recoil(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;

    fsa->unk341 = 0;
    if (objdata->enteredState) {
        objdata->enteredState = 0;
        objAnimSet(self, SCORP_ROBO_MODANIM_4_DamageRecoil, 0.0f, 0);
        objdata->animDelta = 0.02f;
    }
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    if ((self->curModAnimId == SCORP_ROBO_MODANIM_4_DamageRecoil) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SCORP_ROBO_STATE_3_Attacking);
        objdata->enteredState = 1;
    }
    ScorpionRobot_ramp_down_spin_speed(objdata);
    return 0;
}

// offset: 0x1A28 | func: 18 | anim state 6
static s32 ScorpionRobot_state_6_dying(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;

    fsa->unk341 = 0;
    if (objdata->enteredState) {
        objdata->enteredState = 0;
        objAnimSet(self, SCORP_ROBO_MODANIM_5_Die, 0.0f, 0);
        objdata->animDelta = 0.0042f;
    }
    self->velocity.x *= 0.97f;
    self->velocity.z *= 0.97f;
    fsa->flags |= OBJSTATE_PRINT_DISABLED;
    self->srt.pitch -= (self->srt.pitch >> 2);
    objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);
    if ((self->curModAnimId == SCORP_ROBO_MODANIM_5_Die) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SCORP_ROBO_STATE_7_Dead);
        objdata->enteredState = 1;
        fsa->target = NULL;
    }
    ScorpionRobot_ramp_down_spin_speed(objdata);
    return 0;
}

// offset: 0x1B94 | func: 19 | anim state 7
static s32 ScorpionRobot_state_7_dead(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;

    fsa->unk341 = 3;
    if (objdata->enteredState) {
        objdata->enteredState = 0;
        gDLL_33_BaddieControl->vtbl->func18(self, baddie->unk3E0, -1, 0);
        gDLL_18_objfsa->vtbl->func21(self, fsa, 0x3C, 0xA, 0);
        baddie->unk3B4 = 0;
        mainSetBits(baddie->unk39C, 1);
        if (self->setup == NULL) {
            objFreeObject(self);
        }
        // Tell player to break z-lock
        objSendMesgMany(0, OBJMSG_SEND_ALL | OBJMSG_SEND_IGNORE_SENDER, self, 0xE0000, self);
    }

    if (self->opacity >= (gUpdateRate * 3)) {
        self->opacity -= (gUpdateRate * 3);
    } else {
        self->opacity = 0;
    }
    ScorpionRobot_ramp_down_spin_speed(objdata);
    return 0;
}

// offset: 0x1D08 | func: 20 | logic state 0
static s32 ScorpionRobot_ai_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
}
