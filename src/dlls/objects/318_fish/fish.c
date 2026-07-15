#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "game/objects/interaction_arrow.h"
#include "sys/math.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/objprint.h"
#include "sys/segment_53F00.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/common/foodbag.h"
#include "dll.h"

typedef struct {
/*00*/    ObjSetup base;
/*18*/    u8 yaw;
/*19*/    u8 scale;
/*1A*/    u8 peekDuration;
/*1B*/    u8 bubbleRate;
/*1C*/    u16 swimDurationBeforePeek;     //How long to stay in State 2, before transitioning into peek state (#3)
/*1E*/    u16 respawnDuration;
/*20*/    u8 maxLateralSpeed;             //Stored at 100x actual value
/*21*/    u8 jumpDistanceLateral;
/*22*/    u8 jumpDuration;
/*23*/    u8 jumpAnticipateDuration;
/*24*/    u8 jumpRecoveryDuration;
/*26*/    u16 swimDurationAfterPeek;      //How long to stay in State 4, before transitioning into jump state
/*28*/    u8 surfaceOffsetY;
} fish_Setup;

typedef struct {
/*00*/    f32 animSpeed;
/*04*/    f32 timer;
/*08*/    f32 bubbleTimer;
/*0C*/    f32 bubbleFXParams[2];
/*14*/    f32 bubbleRate;
/*18*/    u8 state;
/*19*/    u8 randomTurnsPaused;
/*1A*/    s16 turn;
/*1C*/    s16 yawSpeed;
/*20*/    f32 maxLateralSpeed;
/*24*/    f32 maxSpeedApproachingWall;
/*28*/    f32 lateralSpeed;
/*2C*/    f32 surfaceY;
/*30*/    s16 objHitsConfig;
/*34*/    f32 unk34;                      //related to lateral speed during jump anticipation
/*38*/    f32 jumpAnticipationDistance;
/*3C*/    f32 unk3C;                      //related to height during jump anticipation
/*40*/    f32 unk40;                      //related to height during jump anticipation
/*44*/    f32 jumpDistanceLateral;
/*48*/    f32 jumpDuration;
/*4C*/    f32 jumpHeightFactor;
/*50*/    f32 jumpSpeedLateral;
/*54*/    f32 timerPrevJump;
/*58*/    f32 jumpedFromX;
/*5C*/    f32 jumpedFromY;
/*60*/    f32 jumpedFromZ;
/*64*/    f32 jumpFlightDuration;
/*68*/    f32 splashTime;
/*6C*/    f32 unk6C;                      //related to roll while anticipating jump
/*70*/    f32 unk70;                      //related to roll while following jump arc
/*74*/    f32 unk74;                      //related to roll while anticipating jump
/*78*/    f32 unk78;                      //related to roll while following jump arc
/*7C*/    u8 initialised;
} fish_Data;

typedef enum {
    Fish_STATE_0_Hidden,
    Fish_STATE_1_Fade_In,
    Fish_STATE_2_Swim_before_Peek,
    Fish_STATE_3_Peek_Up,
    Fish_STATE_4_Swim_after_Peek,
    Fish_STATE_5_Jump,
    Fish_STATE_6_Jump_Flight,
    Fish_STATE_7_Jump_Recovery
} fish_States;

typedef enum {
    Fish_MODANIM_0_Swim_LOOP,
    Fish_MODANIM_1_Jump,
    Fish_MODANIM_2_Tilted_Up,
    Fish_MODANIM_3_Tilted_Down
} fish_ModAnim;

#define FADE_IN_DURATION 60.0f

static void fish_initialise(Object* self);
static s32 fish_is_jump_possible(Object* self);
static void fish_random_turn_swim(Object* self);
static void fish_swim(Object *self, s16 turnImpulse, u8 delerateAmount);
static s32 fish_handle_approaching_wall(Object *self, Vec3f* vFuture, Vec3s16* eulerRotation);
static s32 fish_jump_handle_flight(Object* self);
static s32 fish_jump_handle_anticipation(Object* self);
static void fish_caught_by_net(Object* self);

// offset: 0x0 | ctor
void fish_ctor(void *dll) { }

// offset: 0xC | dtor
void fish_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void fish_setup(Object *self, fish_Setup *setup, s32 reset) {
    fish_Data *objdata = self->data;

    if (setup->surfaceOffsetY == 0) {
        setup->surfaceOffsetY = 1;
    }

    objdata->initialised = FALSE;
    fish_initialise(self);
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x84 | func: 1
static void fish_initialise(Object* self) {
    fish_Data* objdata;
    fish_Setup* setup;
    f32 curveParam;
    Vec4f curve;
    f32 jumpHeightFactorSq;
    f32 splashOffsetFraction;
    f32 jumpCalcSq;

    objdata = self->data;
    setup = (fish_Setup*)self->setup;

    objdata->timer = 0.0f;
    objdata->bubbleTimer = 0.0f;
    objdata->bubbleRate = (setup->bubbleRate / SQ(SQ(SQ((f32) setup->swimDurationAfterPeek))));
    objdata->state = Fish_STATE_1_Fade_In;
    objdata->turn = 0;
    objdata->randomTurnsPaused = FALSE;
    objdata->yawSpeed = 0;
    objdata->maxLateralSpeed = setup->maxLateralSpeed / 100.0f;
    objdata->maxSpeedApproachingWall = setup->maxLateralSpeed / 1000.0f;
    objdata->lateralSpeed = 0.0f;

    if (func_80058B1C(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &objdata->surfaceY, 0)) {
        objdata->surfaceY = (setup->base.y + objdata->surfaceY) - setup->surfaceOffsetY;
        objdata->bubbleFXParams[0] = setup->surfaceOffsetY / 0.05f;
        objdata->bubbleFXParams[1] = (setup->scale / 100.0f) * 20.0f;
        objdata->jumpDistanceLateral = setup->jumpDistanceLateral;
        objdata->jumpDuration = setup->jumpDuration;
        objdata->jumpSpeedLateral = objdata->jumpDistanceLateral / objdata->jumpDuration;
        objdata->jumpHeightFactor = ((objdata->jumpDuration * -0.04f) * objdata->jumpDuration) / -objdata->jumpDuration; //? This just works out as (objdata->jumpDuration / 25)

        curveParam = objdata->jumpDuration * ((2.0f * objdata->jumpHeightFactor) + (objdata->jumpDuration * -0.04f)) * 0.25f;
        curve.f[0] = curveParam * 0.5f;
        curve.f[1] = curveParam;
        curve.f[2] = SQ(curveParam * 0.5f);
        curve.f[3] = SQ(curveParam);        
        objdata->unk70 = M_90_DEGREES / ((2.0f * curve.f[2]) - curve.f[3]);
        objdata->unk78 = ((M_45_DEGREES) - (objdata->unk70 * curve.f[2])) / curve.f[0];

        jumpHeightFactorSq = SQ(objdata->jumpHeightFactor);
        splashOffsetFraction = -((f32) setup->surfaceOffsetY) * -0.16f;
        if (!(jumpHeightFactorSq < splashOffsetFraction)) {
            jumpCalcSq = sqrtf(jumpHeightFactorSq - splashOffsetFraction);
            objdata->jumpFlightDuration = (-objdata->jumpHeightFactor + jumpCalcSq) / -0.08f;
            objdata->splashTime = (-objdata->jumpHeightFactor - jumpCalcSq) / -0.08f;
            objdata->unk34 = (sqrtf(SQ(objdata->jumpSpeedLateral) + SQ(objdata->jumpHeightFactor)) - objdata->maxLateralSpeed) / setup->jumpAnticipateDuration;
            objdata->jumpAnticipationDistance = (objdata->unk34 * setup->jumpAnticipateDuration * setup->jumpAnticipateDuration) + (objdata->maxLateralSpeed * setup->jumpAnticipateDuration);
            objdata->unk40 = -(setup->surfaceOffsetY * 4.0f);
            objdata->unk40 /= SQ(objdata->jumpAnticipationDistance);
            objdata->unk3C = -objdata->unk40 * objdata->jumpAnticipationDistance;
            curve.f[2] = SQ((f32) setup->surfaceOffsetY * 0.5f);
            curve.f[0] = setup->surfaceOffsetY * 0.5f;
            curve.f[3] = SQ((f32) setup->surfaceOffsetY);
            curve.f[1] = setup->surfaceOffsetY;
            objdata->unk6C = M_45_DEGREES / ((2.0f * curve.f[2]) - curve.f[3]);
            objdata->unk74 = ((M_45_DEGREES/2) - (objdata->unk6C * curve.f[2])) / curve.f[0];
            
            self->srt.yaw = setup->yaw << 8;
            self->srt.scale = self->def->scale * (setup->scale / 100.0f);
            objdata->objHitsConfig = self->objhitInfo->unk58;
            objdata->initialised = TRUE;
        }
    }
}

// offset: 0x630 | func: 2 | export: 1
void fish_control(Object* self) {
    s32 damageType;
    fish_Setup* setup;
    f32 bubblesPerSecond;
    fish_Data* objdata;
    Object* player;
    f32 timerPow4;
    f32 timerSq;

    setup = (fish_Setup*)self->setup;
    objdata = self->data;
    
    player = objGetPlayer();

    if (objdata->initialised == FALSE) {
        fish_initialise(self);
    }

    //Allow targetting based on player-specific gamebits (whether Krystal/Sabre own a fishing net)
    if (player->id == OBJ_Krystal) {
        if (mainGetBits(BIT_Krystal_Fishing_Net) == FALSE) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
        } else {
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        }
    } else {
        if (mainGetBits(BIT_Sabre_Fishing_Net) == FALSE) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
        } else {
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        }
    }

    //Handle fishing net collision
    damageType = func_80025F40(self, NULL, NULL, NULL);
    if (damageType && (damageType == Damage_Type_Fishing_Net)) {
        fish_caught_by_net(self);
        return;
    }

    objdata->timer += gUpdateRateF;

    switch (objdata->state) {
    case Fish_STATE_0_Hidden:
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        if (objdata->timer >= setup->respawnDuration) {
            objdata->state = Fish_STATE_1_Fade_In;
            objdata->timer -= setup->respawnDuration;
        }
        break;
    case Fish_STATE_1_Fade_In:
        if (objdata->timer <= FADE_IN_DURATION) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
            self->opacity = (s32) ((objdata->timer / FADE_IN_DURATION) * OBJECT_OPACITY_MAX);
            fish_swim(self, 0, 0);
        } else {
            self->opacity = OBJECT_OPACITY_MAX;
            self->objhitInfo->unk58 = objdata->objHitsConfig;
            objdata->state = Fish_STATE_2_Swim_before_Peek;
            objdata->timer -= FADE_IN_DURATION;
            fish_swim(self, 0, 0);
        }
        break;
    case Fish_STATE_2_Swim_before_Peek:
        if (objdata->timer <= setup->swimDurationBeforePeek) {
            fish_random_turn_swim(self);
        } else {
            self->srt.transl.y = setup->base.y;
            objdata->state = Fish_STATE_3_Peek_Up;
            objdata->timer -= setup->swimDurationBeforePeek;
            fish_swim(self, 0, 0);
            objAnimSet(self, Fish_MODANIM_2_Tilted_Up, 0.0f, 0);
            objAnim_func_80024D74(self, 30);
        }
        break;
    case Fish_STATE_3_Peek_Up:
        if (objdata->timer <= setup->peekDuration) {
            self->srt.transl.y = setup->base.y + (((objdata->surfaceY - setup->base.y) * objdata->timer) / setup->peekDuration);
            fish_random_turn_swim(self);
        } else {
            self->srt.transl.y = objdata->surfaceY;
            objdata->state = Fish_STATE_4_Swim_after_Peek;
            objdata->timer -= setup->peekDuration;
            fish_swim(self, 0, 0);
            objAnimSet(self, Fish_MODANIM_0_Swim_LOOP, 0.0f, 0);
            objAnim_func_80024D74(self, 30);
        }
        break;
    case Fish_STATE_4_Swim_after_Peek:
        if (objdata->timer >= setup->swimDurationAfterPeek) {
            if (fish_is_jump_possible(self)) {
                objdata->timer = gUpdateRateF;
                objdata->state = Fish_STATE_5_Jump;
                objdata->jumpedFromX = self->srt.transl.x;
                objdata->jumpedFromY = self->srt.transl.y;
                objdata->jumpedFromZ = self->srt.transl.z;
                objdata->timerPrevJump = 0.0f;
                
                objAnimSet(self, Fish_MODANIM_1_Jump, 0.0f, 0);
                objAnim_func_80024D74(self, 30);
                fish_jump_handle_anticipation(self);
            } else {
                fish_swim(self, 0, 0);
            }
        } else {
            fish_random_turn_swim(self);
        }

        //Create bubble particles when the player's nearby
        {
            timerSq = SQ(objdata->timer);
            timerPow4 = SQ(SQ(objdata->timer));
            objdata->bubbleTimer += gUpdateRateF;

            bubblesPerSecond = SQ(timerPow4) * objdata->bubbleRate;
            if (bubblesPerSecond > setup->bubbleRate) {
                bubblesPerSecond = setup->bubbleRate;
            }

            if (objdata->bubbleTimer >= 60.0f / bubblesPerSecond) {
                objdata->bubbleTimer -= 60.0f / bubblesPerSecond;
                if ((player != NULL) && (vec3DistanceSquared(&self->globalPosition, &player->globalPosition) < SQ(100))) {
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_26, NULL, 0, -1, objdata->bubbleFXParams);
                }
            }
        }
        break;
    case Fish_STATE_5_Jump:
        //Anticipate downwards before bursting up through the water
        if (fish_jump_handle_anticipation(self) == FALSE) {
            //Store position and advance state when leaving the water
            objdata->jumpedFromX = self->srt.transl.x;
            objdata->jumpedFromY = self->srt.transl.y;
            objdata->jumpedFromZ = self->srt.transl.z;
            fish_jump_handle_flight(self);
        }
        break;
    case Fish_STATE_6_Jump_Flight:
        if (fish_jump_handle_flight(self) == FALSE) {
            fish_swim(self, 0, 0);
        }
        break;
    case Fish_STATE_7_Jump_Recovery:
        //Settling back into regular swimming, after the end of a jump
        if (objdata->timer <= setup->jumpRecoveryDuration) {
            self->srt.transl.y = objdata->surfaceY - (((objdata->surfaceY - setup->base.y) * objdata->timer) /setup->jumpRecoveryDuration);
            fish_random_turn_swim(self);
        } else {
            self->srt.transl.y = setup->base.y;
            objdata->state = Fish_STATE_2_Swim_before_Peek;
            objdata->timer -= setup->jumpRecoveryDuration;
            fish_swim(self, 0, 0);
            objAnimSet(self, Fish_MODANIM_0_Swim_LOOP, 0.0f, 0);
            objAnim_func_80024D74(self, 30);
        }
        break;
    }

    objAnimAdvance(self, objdata->animSpeed, gUpdateRateF, NULL);
}

// offset: 0xEA8 | func: 3 | export: 2
void fish_update(Object *self) { }

// offset: 0xEB4 | func: 4 | export: 3
void fish_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    fish_Data* objdata = self->data;

    if ((objdata->state != Fish_STATE_0_Hidden) && visibility) {
        self->srt.yaw -= M_180_DEGREES;
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        self->srt.yaw -= M_180_DEGREES;
    }
}

// offset: 0xF40 | func: 5 | export: 4
void fish_free(Object *self, s32 onlySelf) { }

// offset: 0xF50 | func: 6 | export: 5
u32 fish_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xF60 | func: 7 | export: 6
u32 fish_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(fish_Data);
}

// offset: 0xF74 | func: 8
static s32 fish_is_jump_possible(Object *self) {
    fish_Data* objdata;
    Vec3s16 eulerRotation;
    s32 _pad[3];
    Vec3f vFuture;
    Vec3f vNow;
    s32 _pad2;
    
    objdata = self->data;
    
    if (objdata->lateralSpeed != objdata->maxLateralSpeed) {
        return 0;
    }

    eulerRotation.x = self->srt.yaw;
    eulerRotation.y = 0;
    eulerRotation.z = 0;

    //Check where the fish will be at the end of the jump
    vFuture.x = objdata->jumpAnticipationDistance;
    vFuture.x = objdata->jumpAnticipationDistance + objdata->jumpDistanceLateral + 10.0f;
    vFuture.y = 0.0f;
    vFuture.z = 0.0f;
    mathRotateRPY((const SRT *)&eulerRotation, vFuture.f);
    vFuture.x += self->srt.transl.x;
    vFuture.y += self->srt.transl.y;
    vFuture.z += self->srt.transl.z;

    vNow.x = self->srt.transl.x;
    vNow.y = self->srt.transl.y;
    vNow.z = self->srt.transl.z;

    if (func_80059C40(&vNow, &vFuture, 0.1f, 1, NULL, self, 4, -1, 0xFF, 0)) {
        return FALSE;
    } else {
        return TRUE;
    }
}

// offset: 0x10BC | func: 9
static void fish_random_turn_swim(Object *self) {
    fish_Data *objdata = self->data;

    if (objdata->randomTurnsPaused == FALSE) {
        if (mathRnd(0, 99) == 0) {
            fish_swim(self, mathRnd(-0x100, 0x100), mathRnd(1, 100));
        } else {
            fish_swim(self, mathRnd(-0x100, 0x100), 0);
        }
    } else {
        fish_swim(self, 0, 0);
    }
}

// offset: 0x11C8 | func: 10
static void fish_swim(Object *self, s16 turnImpulse, u8 delerateAmount) {
    fish_Data *objdata;
    Vec3s16 eulerRotation;
    Vec3f delta;
    Vec3f vFuture;

    objdata = self->data;
    objdata->turn += turnImpulse * gUpdateRateF;

    if (delerateAmount) {
        objdata->lateralSpeed -= (objdata->maxLateralSpeed * delerateAmount) / 100.0f;
        if (objdata->lateralSpeed < 0.0f) {
            objdata->lateralSpeed = 0.0f;
        }
    }

    if (objdata->turn > 0) {
        if (objdata->lateralSpeed == 0.0f) {
            objdata->yawSpeed += gUpdateRateF;
        } else {
            objdata->yawSpeed += 5.0f * gUpdateRateF;
        }

        if (objdata->yawSpeed > 0x100) {
            objdata->yawSpeed = 0x100;
        }

        self->srt.yaw += objdata->yawSpeed * gUpdateRateF;
        objdata->turn -= objdata->yawSpeed * gUpdateRateF;
        if (objdata->turn <= 0) {
            objdata->turn = 0;
            objdata->yawSpeed = 0;
        }
    } else if (objdata->turn < 0) {
        if (objdata->lateralSpeed == 0.0f) {
            objdata->yawSpeed -= gUpdateRateF;
        } else {
            objdata->yawSpeed -= 5.0f * gUpdateRateF;
        }

        if (objdata->yawSpeed < -0x100) {
            objdata->yawSpeed = -0x100;
        }

        self->srt.yaw += objdata->yawSpeed * gUpdateRateF;
        objdata->turn -= objdata->yawSpeed * gUpdateRateF;
        if (objdata->turn >= 0) {
            objdata->turn = 0;
            objdata->yawSpeed = 0;
        }
    }

    eulerRotation.x = self->srt.yaw;
    eulerRotation.y = 0;
    eulerRotation.z = 0;

    //Turn away from imminent wall, without changing coords
    {
        delta.x = gUpdateRateF * 10.0f;
        delta.y = 0.0f;
        delta.z = 0.0f;
        mathRotateRPY((const SRT *)&eulerRotation, delta.f);
        delta.x += self->srt.transl.x;
        delta.y += self->srt.transl.y;
        delta.z += self->srt.transl.z;

        if (func_80059C40(&self->srt.transl, &delta, 1.0f, 0, NULL, self, 4, -1, 0xFF, 0)) {
            objdata->animSpeed = 0.06f;

            if (objdata->yawSpeed > 0) {
                self->srt.yaw += gUpdateRateF * 0x200;
                objdata->turn = 0x500;
            } else {
                self->srt.yaw -= gUpdateRateF * 0x200;
                objdata->turn = -0x500;
            }

            objdata->lateralSpeed = 0.0f;
            return;
        }
    }
    
    //Correlate animSpeed with lateral speed
    if (objdata->lateralSpeed == objdata->maxLateralSpeed) {
        objAnimSetProgress(self, 0.0f);
        objdata->animSpeed = 0.0f;
    } else {
        objdata->animSpeed = (objdata->lateralSpeed / objdata->maxLateralSpeed) * 0.06f;
    }

    //Update position, and adjust speed/turn away from more distant walls
    {
        //Calculate a future position
        vFuture.x = gUpdateRateF * 30.0f * 5.0f;
        vFuture.y = 0.0f;
        vFuture.z = 0.0f;
        mathRotateRPY((const SRT *)&eulerRotation, vFuture.f);
        vFuture.x += self->srt.transl.x;
        vFuture.y += self->srt.transl.y;
        vFuture.z += self->srt.transl.z;

        //Move
        delta.x = objdata->lateralSpeed * gUpdateRateF;
        delta.y = 0.0f;
        delta.z = 0.0f;
        mathRotateRPY((const SRT *)&eulerRotation, delta.f);
        self->srt.transl.x += delta.x;
        self->srt.transl.y += delta.y;
        self->srt.transl.z += delta.z;

        if (func_80059C40(&self->srt.transl, &vFuture, 1.0f, 0, NULL, self, 4, -1, 0xFF, 0)) {
            fish_handle_approaching_wall(self, &vFuture, &eulerRotation);

            if (objdata->lateralSpeed < objdata->maxSpeedApproachingWall) {
                objdata->lateralSpeed += ((objdata->maxLateralSpeed / 100.0f) * gUpdateRateF);
            } else {
                objdata->lateralSpeed -= ((objdata->maxLateralSpeed / 50.0f) * gUpdateRateF);
                if (objdata->lateralSpeed < 0.0f) {
                    objdata->lateralSpeed = 0.0f;
                }
            }
        } else {
            objdata->randomTurnsPaused = FALSE;
            objdata->lateralSpeed += ((objdata->maxLateralSpeed / 100.0f) * gUpdateRateF);
            if (objdata->lateralSpeed > objdata->maxLateralSpeed) {
                objdata->lateralSpeed = objdata->maxLateralSpeed;
            }
        }
    }
}

// offset: 0x1880 | func: 11
static s32 fish_handle_approaching_wall(Object *self, Vec3f* vFuture, Vec3s16* eulerRotation) {
    fish_Data* objdata;

    objdata = self->data;

    if (objdata->randomTurnsPaused == FALSE) {
        vFuture->x -= self->srt.transl.x;
        vFuture->y -= self->srt.transl.y;
        vFuture->z -= self->srt.transl.z;
        eulerRotation->x = -self->srt.yaw;
        mathRotateRPY((const SRT *)eulerRotation, vFuture->f);
        objdata->randomTurnsPaused = TRUE;
        if (vFuture->z > 0.0f) {
            objdata->turn = -0x100;
        } else {
            objdata->turn = 0x100;
        }
    } else if (objdata->turn > 0) {
        objdata->turn = 0x100;
    } else {
        objdata->turn = -0x100;
    }

    return 1;
}

// offset: 0x1978 | func: 12
static s32 fish_jump_handle_flight(Object *self) {
    fish_Data* objdata;
    fish_Setup* setup;
    Vec3f vJump;
    Vec3s16 eulerRotation;
    s32 flightStillOngoing;

    objdata = self->data;
    setup = (fish_Setup*)self->setup;

    flightStillOngoing = TRUE;

    eulerRotation.x = self->srt.yaw;
    eulerRotation.y = 0;
    eulerRotation.z = 0;

    if (objdata->timerPrevJump < objdata->jumpFlightDuration) {
        if (objdata->timer > objdata->jumpFlightDuration) {
            Vec3f vSplash;
            vSplash.f[0] = objdata->jumpFlightDuration * objdata->jumpSpeedLateral;
            vSplash.f[1] = 0.0f;
            vSplash.f[2] = 0.0f;
            mathRotateRPY((const SRT *)&eulerRotation, vSplash.f);
            vSplash.f[0] += objdata->jumpedFromX;
            vSplash.f[1] = setup->surfaceOffsetY + objdata->surfaceY;
            vSplash.f[2] += objdata->jumpedFromZ;
            gDLL_24_Waterfx->vtbl->spawn_splash(vSplash.f[0],        vSplash.f[1], vSplash.f[2], 0.0f);
            gDLL_24_Waterfx->vtbl->spawn_splash(vSplash.f[0] + 5.0f, vSplash.f[1], vSplash.f[2], 0.0f);
            gDLL_24_Waterfx->vtbl->spawn_circular_ripple(vSplash.f[0] - 5.0f, vSplash.f[1], vSplash.f[2] - 5.0f, self->srt.yaw, 0.0f, 4);
            gDLL_24_Waterfx->vtbl->spawn_circular_ripple(vSplash.f[0] - 5.0f, vSplash.f[1], vSplash.f[2],        self->srt.yaw, 0.0f, 3);
            gDLL_24_Waterfx->vtbl->spawn_circular_ripple(vSplash.f[0] - 5.0f, vSplash.f[1], vSplash.f[2] + 5.0f, self->srt.yaw, 0.0f, 4);
        }
    } else {
        if ((objdata->timerPrevJump < objdata->splashTime) && (objdata->timer > objdata->splashTime)) {
            Vec3f vSplash;
            vSplash.f[0] = objdata->splashTime * objdata->jumpSpeedLateral;
            vSplash.f[1] = 0.0f;
            vSplash.f[2] = 0.0f;
            mathRotateRPY((const SRT*)&eulerRotation, vSplash.f);
            vSplash.f[0] += objdata->jumpedFromX;
            vSplash.f[1] = setup->surfaceOffsetY + objdata->surfaceY;
            vSplash.f[2] += objdata->jumpedFromZ;
            gDLL_24_Waterfx->vtbl->spawn_splash(vSplash.f[0],        vSplash.f[1], vSplash.f[2], 0.0f);
            gDLL_24_Waterfx->vtbl->spawn_splash(vSplash.f[0] - 5.0f, vSplash.f[1], vSplash.f[2], 0.0f);
            gDLL_24_Waterfx->vtbl->spawn_circular_ripple(vSplash.f[0] + 5.0f, vSplash.f[1], vSplash.f[2] - 5.0f, self->srt.yaw, 0.0f, 4);
            gDLL_24_Waterfx->vtbl->spawn_circular_ripple(vSplash.f[0] + 5.0f, vSplash.f[1], vSplash.f[2], self->srt.yaw, 0.0f, 3);
            gDLL_24_Waterfx->vtbl->spawn_circular_ripple(vSplash.f[0] + 5.0f, vSplash.f[1], vSplash.f[2] + 5.0f, self->srt.yaw, 0.0f, 4);
        }
    }

    if (objdata->timer > objdata->jumpDuration) {
        vJump.f[0] = objdata->jumpDistanceLateral;
        vJump.f[1] = 0.0f;
        vJump.f[2] = 0.0f;
        self->srt.roll = 0;
        objdata->state = Fish_STATE_7_Jump_Recovery;
        objdata->timer -= objdata->jumpDuration;
        objAnimSet(self, Fish_MODANIM_3_Tilted_Down, 0.0f, 0);
        objAnim_func_80024D74(self, 30);
        flightStillOngoing = FALSE;
    } else {
        //Quadratic jump arc
        vJump.f[0] = objdata->timer * objdata->jumpSpeedLateral;
        vJump.f[1] = (objdata->jumpHeightFactor * objdata->timer) + (-0.04f * objdata->timer * objdata->timer);
        vJump.f[2] = 0.0f;

        if (objdata->timer < (objdata->jumpDuration * 0.5f)) {
            self->srt.roll = -((vJump.f[1] * objdata->unk78) + (objdata->unk70 * vJump.y * vJump.y));
        } else {
            self->srt.roll =  ((vJump.f[1] * objdata->unk78) + (objdata->unk70 * vJump.y * vJump.y));
        }

        objdata->timerPrevJump = objdata->timer;
    }

    mathRotateRPY((const SRT*)&eulerRotation, vJump.f);
    self->srt.transl.x = objdata->jumpedFromX + vJump.f[0];
    self->srt.transl.y = objdata->jumpedFromY + vJump.f[1];
    self->srt.transl.z = objdata->jumpedFromZ + vJump.f[2];

    return flightStillOngoing;
}

// offset: 0x1EF4 | func: 13
static s32 fish_jump_handle_anticipation(Object* self) {
    fish_Data* objdata;
    fish_Setup* setup;
    Vec3f vAntic;
    Vec3s16 eulerRotation;
    s32 anticipationStillOngoing;

    setup = (fish_Setup*)self->setup;
    objdata = self->data;

    eulerRotation.x = self->srt.yaw;
    eulerRotation.y = 0;
    eulerRotation.z = 0;

    anticipationStillOngoing = TRUE;

    if (objdata->timer > setup->jumpAnticipateDuration) {
        vAntic.f[0] = objdata->jumpAnticipationDistance;
        vAntic.f[1] = 0.0f;
        vAntic.f[2] = 0.0f;
        self->srt.roll = 0;
        objAnimSet(self, Fish_MODANIM_0_Swim_LOOP, 0.0f, 0);
        objAnim_func_80024D74(self, 30);
        objdata->animSpeed = 0.0f;
        objdata->timer -= setup->jumpAnticipateDuration;
        objdata->state = Fish_STATE_6_Jump_Flight;
        objdata->timerPrevJump = 0.0f;
        anticipationStillOngoing = FALSE;
    } else {
        vAntic.f[0] = (objdata->unk34 * objdata->timer * objdata->timer) + (objdata->maxLateralSpeed * objdata->timer);
        vAntic.f[1] = (objdata->unk40 * vAntic.f[0] * vAntic.f[0]) + (objdata->unk3C * vAntic.x);
        vAntic.f[2] = 0.0f;

        if (vAntic.f[0] < (objdata->jumpAnticipationDistance * 0.5f)) {
            self->srt.roll = ((vAntic.f[1] * objdata->unk74) + (objdata->unk6C * vAntic.y * vAntic.y));
        } else {
            self->srt.roll = -((vAntic.f[1] * objdata->unk74) + (objdata->unk6C * vAntic.y * vAntic.y));
        }

        objdata->timerPrevJump = objdata->timer;
        objdata->animSpeed = 0.12f;
    }

    mathRotateRPY((const SRT *)&eulerRotation, vAntic.f);
    self->srt.transl.x = objdata->jumpedFromX + vAntic.f[0];
    self->srt.transl.y = objdata->jumpedFromY - vAntic.f[1];
    self->srt.transl.z = objdata->jumpedFromZ + vAntic.f[2];

    return anticipationStillOngoing;
}

// offset: 0x2150 | func: 14
static void fish_caught_by_net(Object *self) {
    fish_Data *objdata;
    s32 _pad;
    Object* player;
    ObjSetup *setup;
    Object* foodbag;

    objdata = self->data;
    setup = self->setup;

    if (self->id == OBJ_DF_Lantern) { // ????????
        /* OBJ_SC_golden_nugge is indexed just before OBJ_DF_Lantern: maybe a nugget was 
           once going to be fished out of the water, and it would've shared this DLL? */
        mainSetBits(BIT_Gold_Nugget_LFV, 1);
    } else {
        player = objGetPlayer();
        foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 15);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Fish);
    }

    objdata->state = Fish_STATE_0_Hidden;
    objdata->timer = 0.0f;
    self->srt.transl.x = setup->x;
    self->srt.transl.y = setup->y;
    self->srt.transl.z = setup->z;
    self->srt.roll = 0;
    self->srt.yaw = 0;
    func_800267A4(self);
}
