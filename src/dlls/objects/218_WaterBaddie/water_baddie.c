#include "common.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/objects/common/weapon.h"
#include "game/objects/interaction_arrow.h"
#include "sys/gfx/model.h"
#include "sys/math.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

typedef struct {
    f32 pitchSpeed;
    f32 rollSpeed;
    f32 pitchAcceleration;
    f32 waterHeight;
    f32 turnSpeed;
    f32 moveSpeed;
    f32 unk18;
    s16 bobPhaseAngle;
    s16 prevYaw;
    u16 rippleFXTimer;
    u16 turnFXTimer;
    u8 createTurnRipples;
    s32 diveAmount;
} WaterBaddie_DataActual;

typedef enum {
    WaterBaddie_ASTATE_0_Turn_To_Target,
    WaterBaddie_ASTATE_1_Swimming,
    WaterBaddie_ASTATE_2_Hit
} WaterBaddie_AnimStates;

typedef enum {
    WaterBaddie_LSTATE_0_Hit,
    WaterBaddie_LSTATE_1_Dying,
    WaterBaddie_LSTATE_2_Dead,
    WaterBaddie_LSTATE_3_Swimming,
    WaterBaddie_LSTATE_4_Top
} WaterBaddie_LogicStates;

/*0x0*/ static s32 dCurveTypes[] = { 2, 3 };
/*0x8*/ static u16 dWaterSounds[] = { SOUND_53F_Water_Paddle, SOUND_540_Water_Paddle };
/*0xC*/ static u16 dBaddieSounds[] = {
    SOUND_541_WaterBaddie_Squeak, SOUND_542_WaterBaddie_Squeak, SOUND_543_WaterBaddie_Cry, SOUND_547_Light_Bump, SOUND_53E_Crunch
};
/*0x18*/ static s32 dHitAnimStateMap[] = {
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit, 
    WaterBaddie_ASTATE_2_Hit
};
/*0x88*/ static s8 dHitDamageMap[] = {
    -1, -1, -1, -1,
    -1, -1, -1, -1,
    -1, -1, -1, -1,
    -1, -1, -1, -1,
    -1, -1, -1, -1,
    -1, -1, -1, -1,
    -1, -1, -1, -1
};

/*0x0*/ static ObjFSA_StateCallback sAnimStateCallbacks[3];
/*0x10*/ static ObjFSA_StateCallback sLogicStateCallbacks[5];

static int WaterBaddie_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);
static void WaterBaddie_tick(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void WaterBaddie_engageTarget(Object* self, AnimObj_Data* animData, Baddie* baddie, ObjFSA_Data* fsa);
static void WaterBaddie_searchForTarget(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void WaterBaddie_handleRotating(Object* self, Baddie* fsa, ObjFSA_Data* baddie);
static void WaterBaddie_handleEffects(Object* self, Baddie* fsa, ObjFSA_Data* baddie);

static s32 WaterBaddie_animState0TurnToTarget(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WaterBaddie_animState1Swimming(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WaterBaddie_animState2Hit(Object* self, ObjFSA_Data* fsa, f32 updateRate);

static s32 WaterBaddie_logicState0Hit(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WaterBaddie_logicState1Dying(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WaterBaddie_logicState2Dead(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WaterBaddie_logicState3Swimming(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WaterBaddie_logicState4Top(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void WaterBaddie_initFSACallbacks(void) {
    sAnimStateCallbacks[WaterBaddie_ASTATE_0_Turn_To_Target] = WaterBaddie_animState0TurnToTarget;
    sAnimStateCallbacks[WaterBaddie_ASTATE_1_Swimming]       = WaterBaddie_animState1Swimming;
    sAnimStateCallbacks[WaterBaddie_ASTATE_2_Hit]            = WaterBaddie_animState2Hit;
    
    sLogicStateCallbacks[WaterBaddie_LSTATE_0_Hit]      = WaterBaddie_logicState0Hit;
    sLogicStateCallbacks[WaterBaddie_LSTATE_1_Dying]    = WaterBaddie_logicState1Dying;
    sLogicStateCallbacks[WaterBaddie_LSTATE_2_Dead]     = WaterBaddie_logicState2Dead;
    sLogicStateCallbacks[WaterBaddie_LSTATE_3_Swimming] = WaterBaddie_logicState3Swimming;
    sLogicStateCallbacks[WaterBaddie_LSTATE_4_Top]      = WaterBaddie_logicState4Top;
}

// offset: 0x84 | ctor
void WaterBaddie_ctor(void* dll) {
    WaterBaddie_initFSACallbacks();
}

// offset: 0xC4 | dtor
void WaterBaddie_dtor(void* dll) { }

// offset: 0xD0 | func: 1 | export: 0
void WaterBaddie_obj_Setup(Object* self, Baddie_Setup* objSetup, s32 reset) {
    Baddie* baddie;
    WaterBaddie_DataActual* objData;
    s32 count;
    f32 depth;
    s32 i;
    TrackHeightResult** trackResult;
    u8 flags;

    baddie = self->data;
    
    flags = 0x10;
    if (reset) {
        flags = 0x10 | 1;
    }
    if (!(objSetup->unk2B & 1)) {
        flags |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 3, 5, 0x100, flags, 20.0f);
    
    self->animCallback = WaterBaddie_animCallback;
    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, WaterBaddie_ASTATE_0_Turn_To_Target);
    baddie->fsa.logicState = WaterBaddie_LSTATE_4_Top;
    
    objData = baddie->objdata;
    objData->prevYaw = self->srt.yaw;
    objData->moveSpeed = objSetup->unk2F / 100.0f;
    
    count = trackGetHeight(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &trackResult, 0, 0);
    objData->waterHeight = 0.0f;
    
    //Find the local water height
    //(This is only done during setup, so it assumes the WaterBaddie will be swimming around a perfectly flat water plane)
    if (count) {
        for (i = 0, objData->waterHeight = -9999.0f; i < count; i++){
            depth = trackResult[i]->y - self->srt.transl.y;
            if ((trackResult[i]->unk14 == 0xE) && (objData->waterHeight < depth)) {
                objData->waterHeight = depth;
            }
        }
    }    
    
    objData->waterHeight += self->srt.transl.y;
    objData->unk18 = 0.075f;
}

// offset: 0x3C4 | func: 2 | export: 1
void WaterBaddie_obj_Control(Object* self) {
    Baddie* baddie;
    Baddie_Setup* objSetup;

    baddie = self->data;
    objSetup = (Baddie_Setup*)self->setup;
    
    if (self->unkDC != 0) {
        if (gDLL_29_Gplay->vtbl->did_time_expire(objSetup->base.uID)) {
            gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 3, 5, 0x100, 0x30, 20.0f);
            dll_amSfx->Play(self, SOUND_B20_Low_Grunt, MAX_VOLUME, NULL, NULL, 0, NULL);
            baddie->fsa.unk33A = FALSE;
            self->opacity = OBJECT_OPACITY_MAX;
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
            baddie->unk3B2 |= 0x100;
        }
        return;
    }

    if (self->unkE0 == 0) {
        self->srt.transl.x = objSetup->base.x;
        self->srt.transl.y = objSetup->base.y;
        self->srt.transl.z = objSetup->base.z;
        gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->unk2E, self, -1);
        self->unkE0 = 1;
        return;
    }
    
    if (baddie->unk3B2 & 2) {
        gDLL_33_BaddieControl->vtbl->func9(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, (s8*)&baddie->unk3B4, 4, 0, 0, 1);
        baddie->unk3B2 &= ~2;
    }
    
    if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 1)) {
        WaterBaddie_tick(self, baddie, &baddie->fsa);
        if ((baddie->fsa.target != NULL) && (baddie->fsa.hitpoints != 0)) {
            WaterBaddie_engageTarget(self, 0, baddie, &baddie->fsa);
        } else {
            WaterBaddie_searchForTarget(self, baddie, &baddie->fsa);
        }
    }
}

// offset: 0x648 | func: 3 | export: 2
void WaterBaddie_obj_Update(Object* self) {
    gDLL_18_objfsa->vtbl->func2(self, self->data, sAnimStateCallbacks);
}

// offset: 0x698 | func: 4 | export: 3
void WaterBaddie_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie = self->data;
    
    if (visibility && (self->unkDC == 0)) {
        if (baddie->unk3E8) {
            objprintSetBlendColor(0xC8, 0, 0, baddie->unk3E8);
        }
        
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);

        if (baddie->unk3B2 & 0x60) {
            if (baddie->unk3B2 & 0x20) {
                gDLL_32_modelfx->vtbl->func2(self, 0x330, &baddie->unk3E8);
                gDLL_32_modelfx->vtbl->func2(self, 0x330, &baddie->unk3E8);
            }
            gDLL_32_modelfx->vtbl->func2(self, 0x32F, &baddie->unk3E8);
        }
        
        if (baddie->unk3B2 & 0x100) {
            gDLL_32_modelfx->vtbl->func2(self, 0x333, &baddie->unk3E8);
            gDLL_32_modelfx->vtbl->func2(self, 0x334, &baddie->unk3E8);
            baddie->unk3B2 &= ~0x100;
        }
    }
}

// offset: 0x8E8 | func: 5 | export: 4
void WaterBaddie_obj_Free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    objFreeObjectType(self, 4);

    if (self->linkedObject != NULL) {
        objFreeObject(self->linkedObject);
        self->linkedObject = NULL;
    }
    
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 1);
}

// offset: 0x98C | func: 6 | export: 5
u32 WaterBaddie_obj_GetModelFlags(Object* self) {
    return MODFLAGS_100 | MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0x99C | func: 7 | export: 6
u32 WaterBaddie_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(Baddie) + sizeof(WaterBaddie_DataActual);
}

// offset: 0x9B0 | func: 8 | export: 7
s16 WaterBaddie_GetAnimState(Object* self) {
    Baddie* baddie = self->data;
    return baddie->fsa.animState;
}

// offset: 0x9C0 | func: 9 | export: 8
void WaterBaddie_ReceiveMessage(Object* self, u8 message) {

}

// offset: 0x9D0 | func: 10
int WaterBaddie_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    Baddie_Setup* objSetup;
    Baddie* baddie;

    objSetup = (Baddie_Setup*)self->setup;
    baddie = self->data;
    
    if (self->unkDC != 0) {
        return 0;
    }
    
    if (self->seqSlot != -1) {
        if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 1) == 0) {
            return 1;
        }
        
        WaterBaddie_tick(self, baddie, &baddie->fsa);
        
        if (baddie->unk3A0 != NO_GAMEBIT && mainGetBits(baddie->unk3A0)) {
            gDLL_3_Animation->vtbl->func21(animData, objSetup->unk2C);
            baddie->unk3A0 = -1;
        }

        switch (baddie->unk3B4) {
        case 2:
            animData->unk7A = 0;
            WaterBaddie_engageTarget(self, animData, baddie, &baddie->fsa);
            if (baddie->unk3B4 == 1) {
                baddie->fsa.logicState = WaterBaddie_LSTATE_4_Top;
                gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, 1.0f, 1.0f, sAnimStateCallbacks, sLogicStateCallbacks);
                animData->unk62 = 0;
            }
            break;
        case 1:
            if (gDLL_33_BaddieControl->vtbl->func12(self, animData, baddie, sAnimStateCallbacks, sLogicStateCallbacks, 0)) {
                gDLL_33_BaddieControl->vtbl->func10(self, &baddie->fsa, 0.17f, 1);
            }
            break;
        case 0:
        default:
            animData->unk7A = -1;
            animData->unk7A &= ~0x40;
            WaterBaddie_searchForTarget(self, baddie, &baddie->fsa);
            break;
        }
    }
    
    if (self->seqSlot == -1) {
        baddie->unk3B2 |= 2;
        return 0;
    }
    
    return baddie->unk3B4 != 0;
}

// offset: 0xC6C | func: 11
void WaterBaddie_tick(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
/*0x28*/ static SRT sFXTransform;
    Object* player = objGetPlayer();
/*0xA4*/ s16 dFXScales1[] = { 0x0206, 0x0167, 0x0165, 0x0206 };
/*0xAC*/ s16 dFXScales2[] = { 0x0206, 0x0167, 0x0165, 0x0206 };
    Vec3f d;
    Object* weapon;
    s32 i;
    s32 scaleIdx;
    
    if (self->linkedObject != NULL) {
        self->linkedObject->parent = self->parent;
    }
    
    if (fsa->target != NULL) {
        d.f[0] = fsa->target->globalPosition.x - self->globalPosition.x;
        d.f[1] = fsa->target->globalPosition.y - self->globalPosition.y;
        d.f[2] = fsa->target->globalPosition.z - self->globalPosition.z;
        fsa->targetDist = sqrtf(SQ(d.f[0]) + SQ(d.f[1]) + SQ(d.f[2]));
    }
    
    //Apply eye animation (WaterBaddie's eye textures can't animate, though! Must be unfinished?)
    objExprEyeIdle(self, &baddie->unk3BC);

    WaterBaddie_handleRotating(self, baddie, fsa);
    WaterBaddie_handleEffects(self, baddie, fsa);
    
    if (!(baddie->unk3B0 & 1)) {
        gDLL_33_BaddieControl->vtbl->func14(self, (Baddie*)fsa, &baddie->unk3B2, 1, 2, baddie->unk3A6, baddie->unk3A4);
    }
    
    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 4, 4, 0);

    //Handle being hit
    if (gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, dHitAnimStateMap, dHitDamageMap, 0, &baddie->unk3A8, &sFXTransform)) {
        weapon = player->linkedObject;
        scaleIdx = ((DLL_IWeapon*)weapon->dll)->vtbl->func19(weapon);
        if (scaleIdx > 3) {
            scaleIdx = 3;
        }
        
        sFXTransform.scale = dFXScales1[scaleIdx];
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, &sFXTransform, 0x200001, -1, NULL);
        sFXTransform.transl.x -= self->srt.transl.x;
        sFXTransform.transl.y -= self->srt.transl.y;
        sFXTransform.transl.z -= self->srt.transl.z;
        sFXTransform.scale = dFXScales2[scaleIdx];
        
        for (i = 0; i < 4; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_324, &sFXTransform, 2, -1, NULL);
        }
        
        dll_amSfx->Play(self, dBaddieSounds[2], MAX_VOLUME, NULL, NULL, 0, NULL);
        dll_amSfx->Play(self, dBaddieSounds[4], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
}

// offset: 0x1088 | func: 12
void WaterBaddie_engageTarget(Object* self, AnimObj_Data* animData, Baddie* baddie, ObjFSA_Data* fsa) {
    Baddie_Setup* objSetup = (Baddie_Setup*)self->setup;
    
    self->objhitInfo->unk58 |= 1;
    
    if (fsa->hitpoints != 0) {
        fsa->unk33A = TRUE;
        if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2 + 50.0f, 1)) {
            fsa->target = baddie->unk3AC;
            fsa->unk33D = 0;
            if (objSetup->unk24 >= 0) {
                if (animData != NULL) {
                    gDLL_3_Animation->vtbl->func21(animData, objSetup->unk24);
                }
                baddie->unk3B4 = 1;
            } else {
                fsa->target = NULL;
            }
        }
        gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.0f, 0);
    }
    
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    self->animObj = baddie->unk3AC;
}

// offset: 0x1238 | func: 13
void WaterBaddie_searchForTarget(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    WaterBaddie_DataActual* objData;
    Object* target;

    objData = baddie->objdata;
    
    self->objhitInfo->unk58 &= ~1;
    
    if (fsa->hitpoints != 0) {
        if (baddie->unk3B2 & 8) {
            fsa->logicState = WaterBaddie_LSTATE_3_Swimming;
        }

        target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, 0x8000);
        if (target != NULL) {
            gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, (s8*)&baddie->unk3B4, 4, 0, 0, 1);
            fsa->unk33D = 0;
            fsa->target = target;
            if (1) {
                objData->diveAmount = 0; 
                objData->pitchAcceleration = 0;
            }
        }
    }
    
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    self->animObj = baddie->unk3AC;
}

// offset: 0x13DC | func: 14
void WaterBaddie_handleRotating(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    WaterBaddie_DataActual* objData;
    f32 pitchBobAcceleration;
    s16 yaw;
    s16 yawDiff;
    s16 rollAngle;

    objData = baddie->objdata;

    self->velocity.y = 0.0f;

    //Get yaw diff compared to previous value
    {
        yaw = self->srt.yaw;
        yawDiff = yaw - (objData->prevYaw & 0xFFFF);
        CIRCLE_WRAP(yawDiff);
        if (yawDiff < -2500) {
            yawDiff = -2500;
        }
        if (yawDiff > 2500) {
            yawDiff = 2500;
        }
        objData->prevYaw = yaw;
    }

    //Roll into turns
    objData->rollSpeed += yawDiff >> 1;
    
    //Affect pitch and create water effects when turning
    objData->turnSpeed = ((yawDiff / 182 < 0) ? -(yawDiff / 182) : (yawDiff / 182)) * 0.25f;
    if (objData->turnSpeed > 1.0f) {
        if (objData->pitchAcceleration < 0.4f) {
            objData->pitchAcceleration += 0.01f;
        } else {
            objData->pitchAcceleration = 0.0f;
        }
        objData->createTurnRipples = TRUE;
    }
    
    //Update pitch and Y coordinate
    {
        pitchBobAcceleration = objData->waterHeight - self->srt.transl.y;
        objData->bobPhaseAngle += M_90_DEGREES >> 4; //@framerate-dependent
        pitchBobAcceleration += mathSinInterp(objData->bobPhaseAngle) / (M_360_DEGREES * 2.0f);
        
        objData->pitchSpeed += ((pitchBobAcceleration / 50.0f) - objData->pitchAcceleration) * gUpdateRateF;
        self->srt.transl.y += objData->pitchSpeed;
        
        if (objData->pitchSpeed > 10) {
            objData->pitchSpeed = 10;
        }
        if (objData->pitchSpeed < -10) {
            objData->pitchSpeed = -10;
        }
        self->srt.pitch = objData->pitchSpeed * (M_90_DEGREES / 8.0f);
    }

    //Update roll
    {
        rollAngle = -(self->srt.roll & 0xFFFF);
        CIRCLE_WRAP(rollAngle);
        objData->rollSpeed += (rollAngle / M_1_DEGREE) * gUpdateRateF;
        
        self->srt.roll += (s16)objData->rollSpeed;
        if (self->srt.roll < -M_90_DEGREES) {
            self->srt.roll = -M_90_DEGREES;
        }
        if (self->srt.roll > M_90_DEGREES) {
            self->srt.roll = M_90_DEGREES;
        }
    }
    
    objData->pitchSpeed /= 1.07f;
    objData->rollSpeed /= 1.04f;
}

// offset: 0x16FC | func: 15
void WaterBaddie_handleEffects(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    WaterBaddie_DataActual* objData;
    SRT fxTransform;
    f32 speed;
    f32 yDisplacement;
    s32 fxType;
    s32 fxTurn;
    f32 absPitchSpeed;

    fxTurn = FALSE;
    
    objData = baddie->objdata;
    
    fxType = objData->createTurnRipples;
    if ((objData->pitchSpeed < -0.025f) && (fsa->unk278 < 0.25f)) {
        fxType = 1;
    }
    
    if (objData->pitchSpeed < 0.0f) {
        absPitchSpeed = -objData->pitchSpeed;
    } else {
        absPitchSpeed = objData->pitchSpeed;
    }
    
    //Create a large radial ripple
    if (absPitchSpeed > 1.0f) {
        fxType = 2;
    }
    
    if ((objData->turnFXTimer > 6) && (fsa->unk278 > 0.1f)) {
        fxTurn = TRUE;
    }
    
    if (fxTurn || fxType) {
        if (objData->waterHeight < self->srt.transl.y) {
            yDisplacement = -(objData->waterHeight - self->srt.transl.y);
        } else {
            yDisplacement = objData->waterHeight - self->srt.transl.y;
        }

        if (yDisplacement < 14.0f) {
            fxTransform.transl.y = objData->waterHeight;

            if (fxTurn) {
                //Create wake particles when turning
                speed = ((1.0f - (yDisplacement / 14.0f)) * 12.0f) + (fsa->unk278 * 4.0f);
                fxTransform.transl.x = self->srt.transl.x - mathSinfInterp(self->srt.yaw) * speed;
                fxTransform.transl.z = self->srt.transl.z - mathCosfInterp(self->srt.yaw) * speed;
                fxTransform.scale = fsa->unk278;
                fxTransform.yaw = self->srt.yaw;
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_32A, &fxTransform, 0x200001, -1, NULL);
                objData->turnFXTimer = 0;
            }
            
            if (fxType) {
                if (objData->rippleFXTimer > mathRnd(30, 60)) {
                    speed = fsa->unk278 * 20.0f;
                    fxTransform.transl.x = self->srt.transl.x - mathSinfInterp(self->srt.yaw) * speed;
                    fxTransform.transl.z = self->srt.transl.z - mathCosfInterp(self->srt.yaw) * speed;
                    fxTransform.scale = 1.0f - (yDisplacement / 14.0f);
                    dll_amSfx->Play(self, dWaterSounds[mathRnd(0, 1)], (s32) (fxTransform.scale * 60.0f) + 0x3F, NULL, NULL, 0, NULL);
                    if (fxType == 2) {
                        //Create a big radial ripple
                        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_32C, &fxTransform, 0x200001, -1, NULL);
                    } else {
                        //Create a small radial ripple
                        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_32B, &fxTransform, 0x200001, -1, NULL);
                    }
                    objData->rippleFXTimer = 0;
                }
            }
        }

        objData->createTurnRipples = FALSE;
    }

    objData->rippleFXTimer += gUpdateRateF;
    objData->turnFXTimer += gUpdateRateF;
}

// offset: 0x1C48 | func: 16
s32 WaterBaddie_animState0TurnToTarget(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    fsa->unk27C = 0.0f;
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 5);
    
    return 0;
}

// offset: 0x1CA4 | func: 17
s32 WaterBaddie_animState1Swimming(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    gDLL_33_BaddieControl->vtbl->func3(self, fsa, self->data, 1.0f, 12.0f);
    return 0;
}

// offset: 0x1D04 | func: 18
s32 WaterBaddie_animState2Hit(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    WaterBaddie_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objData = baddie->objdata;
        fsa->unk27C = 0.0f;
        if (self->animProgress > 0.35f) {
            objData->pitchSpeed += 4.0f;
        } else {
            objData->pitchSpeed -= 4.0f;
        }
        self->animProgress = 0.0f;
    }
    
    return 0;
}

// offset: 0x1D8C | func: 19
s32 WaterBaddie_logicState0Hit(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
/*40*/ static f32 sStunnedTimer; //@bug?: won't this affect all WaterBaddies when there are multiple of them?
    
    if (fsa->enteredLogicState) {
        sStunnedTimer = 0.0f;
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, WaterBaddie_ASTATE_2_Hit);
    }
    
    if (fsa->hitpoints <= 0) {
        return FSA_NEXTSTATE_SYNC(WaterBaddie_LSTATE_1_Dying);
    }
    
    //Drift straight ahead for about 3 seconds
    if (sStunnedTimer > 200.0f) {
        return FSA_NEXTSTATE_SYNC(WaterBaddie_LSTATE_4_Top);
    } else {
        sStunnedTimer += gUpdateRateF;
    }
    
    return 0;
}

// offset: 0x1E50 | func: 20
s32 WaterBaddie_logicState1Dying(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        objSendMesg(objGetPlayer(), 0xE0000, self, NULL);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        func_800267A4(self);
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        dll_amSfx->Play(self, SOUND_B21_Dissipating_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
        dll_amSfx->Play(self, SOUND_B1F_Slow_Magic_Chimes, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    return FSA_NEXTSTATE_SYNC(WaterBaddie_LSTATE_2_Dead);
}

// offset: 0x1F70 | func: 21
s32 WaterBaddie_logicState2Dead(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    if (fsa->enteredLogicState) {
        baddie = self->data;
        if (baddie->unk39E != NO_GAMEBIT) {
            mainSetBits(baddie->unk39E, FALSE);
        }
        if (baddie->unk39C != NO_GAMEBIT) {
            mainSetBits(baddie->unk39C, TRUE);
        }
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, -1, 0);
    }
    
    return 0;
}

// offset: 0x203C | func: 22
s32 WaterBaddie_logicState3Swimming(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
/*0x44*/ static s32 sCurveValue; //@bug?: won't this affect all WaterBaddies when there are multiple of them?
    
    Baddie* baddie;
    CurvesStruct* sp3C;
    UnkCurvesStruct* curves;
    f32 curveDelta;
    f32 dx;
    f32 dz;
    f32 speed;
    f32 turnSpeed;
    f32 magnitude;
    f32 temp;
    WaterBaddie_DataActual* objData;

    baddie = self->data;
    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, WaterBaddie_ASTATE_1_Swimming);
    curves = baddie->unk3F8;
    objData = baddie->objdata;
    
    dx = curves->unk0.unk68.x - self->srt.transl.x;
    dz = curves->unk0.unk68.z - self->srt.transl.z;
    curveDelta = 10.0f / sqrtf(SQ(dx) + SQ(dz));
    
    if (self->animProgress > 0.01f) {
        self->animProgress -= 0.01f;
    } else {
        self->animProgress = 0.0f;
    }
    
    if ((curves_func_800053B0(&curves->unk0, curveDelta) || sCurveValue != curves->unk0.unk10) && 
        gDLL_26_Curves->vtbl->func_4704(curves) && 
        gDLL_26_Curves->vtbl->func_4288(baddie->unk3F8, self, 400.0f, dCurveTypes, -1)
    ) {
        baddie->unk3B2 &= ~8;
    }
    
    sCurveValue = curves->unk0.unk10;

    //Do a bobbing dive through the water occasionally
    if ((fsa->unk278 > 0.15f) && (objData->diveAmount == 0)) {
        if (objData->pitchSpeed < 0.0f) {
            magnitude = -objData->pitchSpeed;
        } else {
            magnitude = objData->pitchSpeed;
        }

        if (magnitude < 0.015f) {
            if (objData->waterHeight < self->srt.transl.y) {
                magnitude = -(objData->waterHeight - self->srt.transl.y);
            } else {
                magnitude = objData->waterHeight - self->srt.transl.y;
            }

            if ((magnitude < 1.0f) && mathRnd(0, 100)) {
                objData->diveAmount = mathRnd(25, 75);
            }
        }
    }

    if (objData->diveAmount) {
        if (objData->pitchAcceleration > 0.4f) {
            if (mathRnd(0, 1) != 0) {
                objData->pitchAcceleration = 0.0f;
            }
            objData->diveAmount = 0;
            dll_amSfx->Play(self, dBaddieSounds[mathRnd(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
        } else {
            objData->pitchAcceleration += 0.0001f * objData->diveAmount;
        }
    } else if (objData->pitchAcceleration > 0.0f) {
        objData->pitchAcceleration /= 1.04f;
    }
    
    gDLL_18_objfsa->vtbl->func6(self, fsa, curves->unk0.unk68.x, curves->unk0.unk68.z, 0, 0, 60.0f);

    turnSpeed = objData->turnSpeed;
    if (objData->turnSpeed) {
        temp = objData->moveSpeed / turnSpeed;
        speed = temp;
    } else {
        speed = objData->moveSpeed;
    }
    
    fsa->xAnalogInput *= speed;
    fsa->yAnalogInput *= speed;

    if (self->animProgress > 0.01f) {
        self->animProgress -= 0.01f;
    }
    
    return 0;
}

// offset: 0x247C | func: 23
s32 WaterBaddie_logicState4Top(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return FSA_NEXTSTATE_SYNC(WaterBaddie_LSTATE_3_Swimming);
}
