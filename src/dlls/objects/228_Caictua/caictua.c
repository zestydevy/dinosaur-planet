#include "common.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/33_BaddieControl.h"
#include "dlls/objects/251_weapons.h"
#include "dlls/objects/228_Caictua.h"
#include "dlls/objects/258_Caictua_Thorn.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "macros.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/objlib.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

#include "prevent_bss_reordering.h"

typedef struct {
    Baddie_Setup baddie;
} DLL228_Setup;

typedef struct {
    s8 unk0[12];
    Vec3f origin;  //Coords for the attachPoint the thorn will be fired from
} CaictuaAimData;

typedef struct {
    f32 respawnTimer;           //Timer counting up until the Caictua respawns
    f32 respawnDuration;        //Interval between dying and respawning
    f32 unk8;                   //Given a random value during setup, and zeroed when dying/disengaging, but otherwise unused
    f32 unkC;                   //Zeroed when dying/disengaging, but otherwise unused
    s16 deltaYaw;               //How much to turn by on the current tick
    s16 thornCooldown;          //Countdown until more thorns can be fired at the player
    CaictuaAimData aim[2];      //Coords for the attachPoints the thorns are fired from
} Caictua_DataActual;

typedef struct {
    Baddie baddie;
    Caictua_DataActual objData;
} Caictua_Data;

typedef enum {
    Caictua_ASTATE_0_Idle,
    Caictua_ASTATE_1_Attacking,
    Caictua_ASTATE_2_Hit,
    Caictua_ASTATE_3_Dying
} Caictua_AnimStates;

typedef enum {
    Caictua_LSTATE_0_Top,
    Caictua_LSTATE_1_Hit,
    Caictua_LSTATE_2_Dying,
    Caictua_LSTATE_3_Dead,
    Caictua_LSTATE_4_Disengage,
    Caictua_LSTATE_5_Engage
} Caictua_LogicStates;

/*0x0*/ static s32 dHitAnimStateMap[] = {
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit, 
    Caictua_ASTATE_2_Hit
};
/*0x70*/ static s8 dHitDamageMap[] = {
    -1,     -1,     -1,     -1, 
    -1,     -1,     -1,     -1, 
    -1,     -1,     -1,     -1, 
    -1,     -1,     -1,     -1, 
    -1,     -1,     -1,     -1, 
    -1,     -1,     -1,     -1, 
    -1,     -1,     -1,     -1
};

/*0x18*/ static ObjFSA_StateCallback sAnimStateCallbacks[4];
/*0x28*/ static ObjFSA_StateCallback sLogicStateCallbacks[6];

static void Caictua_checkHit(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void Caictua_tickWithTarget(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void Caictua_tickWithoutTarget(Object* self, Baddie* baddie, ObjFSA_Data* fsa);

static s32 Caictua_animState0Idle(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 Caictua_animState1Attacking(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 Caictua_animState2Hit(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 Caictua_animState3Dying(Object* self, ObjFSA_Data* fsa, f32 updateRate);

static s32 Caictua_logicState0Top(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 Caictua_logicState1Hit(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 Caictua_logicState3Dying(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 Caictua_logicState3Dead(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 Caictua_logicState4Disengage(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 Caictua_logicState5Engage(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void Caictua_initFSACallbacks(void) {
    sAnimStateCallbacks[Caictua_ASTATE_0_Idle]       = Caictua_animState0Idle;
    sAnimStateCallbacks[Caictua_ASTATE_1_Attacking]  = Caictua_animState1Attacking;
    sAnimStateCallbacks[Caictua_ASTATE_2_Hit]        = Caictua_animState2Hit;
    sAnimStateCallbacks[Caictua_ASTATE_3_Dying]      = Caictua_animState3Dying;
    
    sLogicStateCallbacks[Caictua_LSTATE_0_Top]       = Caictua_logicState0Top;
    sLogicStateCallbacks[Caictua_LSTATE_1_Hit]       = Caictua_logicState1Hit;
    sLogicStateCallbacks[Caictua_LSTATE_2_Dying]     = Caictua_logicState3Dying;
    sLogicStateCallbacks[Caictua_LSTATE_3_Dead]      = Caictua_logicState3Dead;
    sLogicStateCallbacks[Caictua_LSTATE_4_Disengage] = Caictua_logicState4Disengage;
    sLogicStateCallbacks[Caictua_LSTATE_5_Engage]    = Caictua_logicState5Engage;
}

// offset: 0x9C | ctor
void Caictua_ctor(void* dll) {
    Caictua_initFSACallbacks();
}

// offset: 0xDC | dtor
void Caictua_dtor(void* dll) { }

// offset: 0xE8 | func: 1 | export: 0
void Caictua_obj_Setup(Object* self, Baddie_Setup* objSetup, s32 reset) {
    Caictua_DataActual* objData;
    Baddie* baddie;
    u8 flags;

    baddie = self->data;
    
    flags = 2;
    if (reset != 0) {
        flags = 2 | 1;
    }
    if ((objSetup->unk2B & 0x20) == FALSE) {
        flags |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 4, 6, 0x100, flags, 20.0f);
    
    self->animCallback = NULL;
    
    objData = baddie->objdata;
    bzero(objData, sizeof(Caictua_DataActual));
    
    objData->respawnDuration = objSetup->unk2C * 60.0f;
    objData->unk8 = mathRnd(10, 300);
    objData->deltaYaw = 0;
    objData->thornCooldown = 0;
    
    objAnimSet(self, 0, 0.0f, 0);
    
    baddie->fsa.animState = Caictua_ASTATE_0_Idle;
    baddie->fsa.logicState = Caictua_LSTATE_0_Top;
    baddie->fsa.unk4.mode = 0;
    
    func_800267A4(self);
}

// offset: 0x244 | func: 2 | export: 1
void Caictua_obj_Control(Object* self) {
    s32 pad;
    Baddie* baddie;
    Baddie_Setup* objSetup;
    f32 time;

    baddie = self->data;
    objSetup = (Baddie_Setup*)self->setup;
    
    if (self->unkDC != 0) {
        return;
    }
    
    //Play a sequence when dying
    if (self->unkE0 == 0) {
        self->srt.transl.x = objSetup->base.x;
        self->srt.transl.y = objSetup->base.y;
        self->srt.transl.z = objSetup->base.z;
        gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->unk2E, self, -1);
        self->unkE0 = 1;
        return;
    }
    
    if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 0) == FALSE) {
        baddie->unk3B6 = FALSE;
        return;
    }
    
    if ((baddie->unk3B0 & 0x10) && (gDLL_7_Newday->vtbl->func8(&time) == FALSE)) {
        baddie->unk3B6 = FALSE;
        return;
    }
    
    Caictua_checkHit(self, baddie, &baddie->fsa);
    
    //Check whether the Caictua is still targetting the player
    if (baddie->unk3B6 == TRUE) {
        if (gDLL_33_BaddieControl->vtbl->func16(self, &baddie->fsa, baddie->unk3E2, TRUE)) {
            baddie->unk3B6 = FALSE;
        }
    }
    
    if (baddie->unk3B6 == FALSE) {
        Caictua_tickWithoutTarget(self, baddie, &baddie->fsa);
    } else {
        Caictua_tickWithTarget(self, baddie, &baddie->fsa);
    }
    
    self->srt.transl.y = objSetup->base.y - 2.0f;
}

// offset: 0x43C | func: 3 | export: 2
void Caictua_obj_Update(Object* self) { }

// offset: 0x448 | func: 4 | export: 3
void Caictua_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie;
    Caictua_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (visibility && (self->unkDC == 0)) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        objGetAttachPointWorldSpace(self, 0, &objData->aim[0].origin.x, &objData->aim[0].origin.y, &objData->aim[0].origin.z, 0);
        objGetAttachPointWorldSpace(self, 1, &objData->aim[1].origin.x, &objData->aim[1].origin.y, &objData->aim[1].origin.z, 0);
    }
}

// offset: 0x50C | func: 5 | export: 4
void Caictua_obj_Free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    objFreeObjectType(self, OBJTYPE_Baddie);

    if (self->linkedObject != NULL) {
        objFreeObject(self->linkedObject);
        self->linkedObject = NULL;
    }
    
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
}

// offset: 0x5B0 | func: 6 | export: 5
u32 Caictua_obj_GetModelFlags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_8 | MODFLAGS_EVENTS;
}

// offset: 0x5C0 | func: 7 | export: 6
u32 Caictua_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(Caictua_Data);
}

// offset: 0x5D4 | func: 8 | export: 7
s16 Caictua_GetAnimState(Object* self, s32 unused) {
    Baddie* baddie = self->data;
    return baddie->fsa.animState;
}

// offset: 0x5E8 | func: 9 | export: 8
void Caictua_ReceiveMessage(Object* self, u8 message, s32 unused) {
    Baddie* baddie = self->data;
    
    switch (message) {
    case 0x81:
        baddie->unk3B0 &= ~4;
        break;
    default:
        STUBBED_PRINTF("BADDIE:Caictua Unknown message [%d]\n", message);
        break;
    }
}

// offset: 0x618 | func: 10
void Caictua_checkHit(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
/*0x0*/ static SRT sHitSRT;
    Object* player = objGetPlayer();
/*0x8C*/ s16 dFXScales[] = { 0x0206, 0x0167, 0x0165, 0x0206 };
    Vec3f delta;
    s32 count;
    s32 scaleIdx;

    if (fsa->target != NULL) {
        delta.f[0] = fsa->target->globalPosition.f[0] - self->globalPosition.f[0];
        delta.f[1] = fsa->target->globalPosition.f[1] - self->globalPosition.f[1];
        delta.f[2] = fsa->target->globalPosition.f[2] - self->globalPosition.f[2];
        fsa->targetDist = sqrtf(SQ(delta.f[0]) + SQ(delta.f[1]) + SQ(delta.f[2]));
    }
    
    if (!(baddie->unk3B0 & 0x20)) {
        gDLL_33_BaddieControl->vtbl->func14(self, (Baddie*)fsa, &baddie->unk3B2, 2, 3, baddie->unk3A6, baddie->unk3A4);
    }

    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 0);
    if ((fsa->hitpoints > 0) && (gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, dHitAnimStateMap, dHitDamageMap, Caictua_LSTATE_1_Hit, &baddie->unk3A8, &sHitSRT))) {
        scaleIdx = ((DLL_251_Weapons*)player->linkedObject->dll)->vtbl->func19(player->linkedObject);
        if (scaleIdx > 3) {
            scaleIdx = 3;
        }

        sHitSRT.scale = dFXScales[scaleIdx];
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, &sHitSRT, 0x200001, -1, NULL);
        
        sHitSRT.transl.x -= self->srt.transl.f[0];
        sHitSRT.transl.y -= self->srt.transl.f[1];
        sHitSRT.transl.z -= self->srt.transl.f[2];
        sHitSRT.scale = dFXScales[scaleIdx];
        
        count = 4;
        while (count--) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_324, &sHitSRT, 2, -1, NULL);
        }
    }
}

// offset: 0x938 | func: 11
static f32 Caictua_calculateDeltaYaw(s32 yawDiff, f32 yawSpeed, f32 maxAngle) {
    f32 temp_ft4;
    f32 temp_ft4_2;
    f32 temp_fv0;
    f32 var_fa0;
    s32 var_a0;
    f32 var_fa1;
    f32 var_fv0;
    f32 var_fv1;
    s32 isNegative;

    var_fv0 = 0.0f;
    var_fv1 = 0.0f;
    var_fa0 = 0.0f;
    
    isNegative = yawDiff < 0;
    if (isNegative) {
        yawDiff = -yawDiff;
        yawSpeed = -yawSpeed;
    }
    
    if (yawSpeed < 0.0f) {
        return (isNegative) ? -maxAngle : maxAngle;
    }
    
    do {
        var_fv1 += maxAngle;
        var_fv0 += var_fv1;
    } while ((var_fv0 + var_fv1) < yawDiff);
    
    if ((yawDiff <= maxAngle) && (yawSpeed <= maxAngle) && 
        (((yawDiff >= 0) && (yawSpeed >= 0.0f)) || ((yawDiff <= 0) && (yawSpeed <= 0.0f)))
    ) {
        var_fa0 = 0.0f;
    } else {
        temp_fv0 = var_fv1 - maxAngle;
        temp_ft4 = yawSpeed + maxAngle;
        if (temp_ft4 <= temp_fv0) {
            var_fa0 = temp_ft4;
        } else {
            temp_ft4_2 = yawSpeed - maxAngle;
            if (temp_ft4_2 < var_fv1) {
                var_fa0 = temp_fv0;
                if (var_fv1 == maxAngle) {
                    var_fa0 = maxAngle;
                }
            } else {
                var_fa0 = temp_ft4_2;
                if (temp_ft4_2 == 0.0f) {
                    var_fa0 = maxAngle;
                }
            }
        }
    }
    
    if (isNegative) {
        var_fa0 = -var_fa0;
        yawSpeed = -yawSpeed;
    }
    
    return var_fa0 - yawSpeed;
}

// offset: 0xAA0 | func: 12
static s16 Caictua_calculateThornPitch(f32 originX, f32 originY, f32 originZ, f32 targetX, f32 targetY, f32 targetZ, f32 speed, f32 arg7, s32 arg8) {
    f32 sp6C;
    f32 temp_fa0;
    f32 temp_fs3;
    f32 dy;
    f32 dx;
    f32 dz;
    f32 var_fa1;
    f32 temp;
    s32 iterations;
    s32 idx;
    s16 sp7C[2]; //7C

    idx = 0;
    
    dx = originX - targetX;
    dz = originZ - targetZ;
    dx = sqrtf(SQ(dx) + SQ(dz));

    dy = targetY - originY;
    sp6C = ((dy * arg7) + SQ(speed));
    dz = SQ(sp6C);
    temp_fa0 = SQ(arg7) * (SQ(dx) + SQ(dy));
    temp = dz - temp_fa0;
    
    if (temp_fa0 <= dz) {
        iterations = 2;
        temp_fs3 = sqrtf(temp);
        while (iterations--) {
            temp = (SQ(dy) / (SQ(dx))) + 1.0f;
            var_fa1 = (((iterations != 0) ? temp_fs3 : -temp_fs3) + sp6C) / (2.0f * temp);
            if (var_fa1 >= 0.0f) {
                var_fa1 = sqrtf(var_fa1);
                if (dx < 0.0f) {
                    var_fa1 = -var_fa1;
                }
                sp7C[idx] = mathAtan2f(((dy / dx) * var_fa1) - ((arg7 * dx) / (2.0f * var_fa1)), var_fa1);
                idx++;
            }
        }
    }

    switch (idx) {
        case 2:
            if (sp7C[0] < sp7C[1]) {
                return (arg8 != 0) ? sp7C[1] : sp7C[0];
            } else {
                return (arg8 != 0) ? sp7C[0] : sp7C[1];
            }
            break;

        case 1:
            return sp7C[0];
            break;

        default:
            return M_45_DEGREES;
    }
}

// offset: 0xD08 | func: 13
void Caictua_tickWithTarget(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    s16 angle;
    s16 yawDiff;
    Caictua_DataActual* objData;
    Object* player;
    f32 dx;
    f32 dz;

    player = objGetPlayer();
    objData = baddie->objdata;

    angle = (u16)mathAtan2f(self->srt.transl.x - player->srt.transl.x, self->srt.transl.z - player->srt.transl.z);
    yawDiff = angle - (self->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(yawDiff);
    
    objData->deltaYaw += Caictua_calculateDeltaYaw(yawDiff, objData->deltaYaw, 45);
    self->srt.yaw += objData->deltaYaw;
    
    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.0f, -1);
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    self->animObj = baddie->unk3AC;
}

// offset: 0xEC0 | func: 14
void Caictua_tickWithoutTarget(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    Caictua_DataActual* objData;
    Object* target;
    Baddie_Setup* objSetup;

    objData = baddie->objdata;
    target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, M_180_DEGREES);
    objSetup = (Baddie_Setup*)self->setup;
    
    //Acquire a target
    if ((target != NULL) && !(baddie->unk3B0 & 4)) {
        gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 0, -1);
        fsa->unk33D = 0;
        fsa->target = target;
        baddie->unk3B6 = TRUE;
        return;
    }
    
    //Respawn after a while
    if ((objData->respawnTimer > 0.0f) && ((fsa->logicState != Caictua_LSTATE_3_Dead) || (baddie->unk3B0 & 1))) {
        if (objData->respawnTimer >= objData->respawnDuration) {
            gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 0, -1);
            objData->respawnTimer = 0.0f;
            fsa->hitpoints = objSetup->quarterHitpoints * 4;
            self->srt.transl.x = objSetup->base.x;
            self->srt.transl.y = objSetup->base.y;
            self->srt.transl.z = objSetup->base.z;
            self->srt.yaw = objSetup->unk2A << 8;
            return;
        }
        
        objData->respawnTimer += gUpdateRateF;
    }
}

// offset: 0x10BC | func: 15
static void Caictua_shootThorn(Object* self, Baddie* baddie) {
    Caictua_DataActual* objData;
    ObjSetup* thornSetup;
    Object* thorn;
    f32 speed;
    f32 tickCount;
    s16 pitch;
    f32 originX;
    f32 originY;
    f32 originZ;
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    s32 i;

    objData = baddie->objdata;
    
    //Fire two thorns
    i = 2;
    while (i--) {
        thornSetup = objAllocSetup(sizeof(Caictua_Thorn_Setup), OBJ_Caictua_Thorn);
        originX = objData->aim[i].origin.x;
        originY = objData->aim[i].origin.y;
        originZ = objData->aim[i].origin.z;
        targetX = baddie->fsa.target->srt.transl.x;
        targetY = baddie->fsa.target->srt.transl.y + 25.0f;
        targetZ = baddie->fsa.target->srt.transl.z;
        thornSetup->loadFlags = OBJSETUP_LOAD_LEVEL;
        thornSetup->fadeFlags = OBJSETUP_FADE_MANUAL;
        thornSetup->loadDistance = 0xFF;
        thornSetup->fadeDistance = 0xFF;
        thornSetup->x = originX;
        thornSetup->y = originY;
        thornSetup->z = originZ;

        thorn = objSetupObject(thornSetup, OBJINIT_STANDALONE | OBJINIT_FLAG4, -1, -1, NULL);
        if (thorn != NULL) {
            speed = baddie->fsa.targetDist / 30.0f;
            if (speed < 1.5f) {
                speed = 1.5f;
            } else if (speed > 5.0f) {
                speed = 5.0f;
            }
            
            tickCount = baddie->fsa.targetDist / (speed * gUpdateRateF);
            targetX += tickCount * baddie->fsa.target->velocity.x;
            targetY += tickCount * baddie->fsa.target->velocity.y;
            targetZ += tickCount * baddie->fsa.target->velocity.z;
            pitch = Caictua_calculateThornPitch(originX, originY, originZ, targetX, targetY, targetZ, speed, -0.03f, 0);

            thorn->srt.pitch = pitch;
            thorn->srt.yaw = mathAtan2f(targetX - originX, targetZ - originZ);
            thorn->velocity.x = Cosf(pitch) * speed * Sinf(thorn->srt.yaw);
            thorn->velocity.y = Sinf(pitch) * speed;
            thorn->velocity.z = Cosf(pitch) * speed * Cosf(thorn->srt.yaw);
            thorn->unkC4 = self;
        }
    }
}

// offset: 0x1394 | func: 16
static void Caictua_cactusToCactusCommunication(Object* self) {
    s32 index;
    s32 count;
    Object* obj;
    Object** objects;

    for (objects = objGetObjects(&index, &count); index < count; index++) {
        obj = objects[index];
        if ((self != obj) && (obj->id == OBJ_Caictua)) {
            dll_Caictua(obj)->ReceiveMessage(obj, 0x81, 0);
        }
    }
}

// offset: 0x1460 | func: 17
s32 Caictua_animState0Idle(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s16 angle;
    Caictua_DataActual* objData;
    s16 yawDiff;
    Object* player;
    Baddie* baddie;
    Vec3f v;
    Vec3s16 vCactus16;
    Vec3s16 vPlayer16;
    u8 hasLineOfSight;

    baddie = self->data;
    objData = baddie->objdata;
    
    fsa->unk341 = 1;
    
    if (fsa->enteredAnimState) {
        func_8002674C(self);
    }
    
    func_80026128(self, Damage_Type_Sword_Staff_Strike1, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    
    if (objData->thornCooldown >= gUpdateRate) {
        objData->thornCooldown -= gUpdateRate;
    } else {
        //Check if the caictua has line-of-sight to the player
        player = objGetPlayer();
        
        //Get yaw diff
        angle = (u16)mathAtan2f(self->srt.transl.x - player->srt.transl.x, self->srt.transl.f[2] - player->srt.transl.f[2]);
        yawDiff = angle - (self->srt.yaw & 0xFFFF);
        CIRCLE_WRAP(yawDiff);
        
        //If the player is within a 90 viewing wedge
        if ((-M_45_DEGREES < yawDiff) && (yawDiff < M_45_DEGREES)) {
            v.f[0] = self->srt.transl.x; 
            v.f[1] = self->srt.transl.y + 55.0f; 
            v.f[2] = self->srt.transl.z; 
            vox_func_80007EE0(&v, &vCactus16);

            v.f[0] = player->srt.transl.x;
            v.f[1] = player->srt.transl.y + 25.0f; 
            v.f[2] = player->srt.transl.z; 
            vox_func_80007EE0(&v, &vPlayer16);

            if (vox_func_80008048(&vPlayer16, &vCactus16, NULL, &hasLineOfSight, 0) || (hasLineOfSight == TRUE)) {
                objData->thornCooldown = mathRnd(120, 240);
                return FSA_NEXTSTATE_SYNC(Caictua_ASTATE_1_Attacking);
            }
        }
    }

    return 0;
}

// offset: 0x16B0 | func: 18
s32 Caictua_animState1Attacking(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    fsa->unk341 = 1;

    if (fsa->enteredAnimState) {
        objAnimSet(self, 0, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    fsa->animTickDelta = 0.015f;
    
    if (fsa->unk308 & 0x40) {
        fsa->unk308 &= ~0x40;
        Caictua_shootThorn(self, baddie);
        dll_amSfx->Play(self, SOUND_71F, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(Caictua_ASTATE_0_Idle);
    } else {
        return 0;
    }
}

// offset: 0x17B4 | func: 19
s32 Caictua_animState2Hit(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    fsa->unk341 = 3;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    fsa->animTickDelta = 0.015f;
    
    if (fsa->enteredAnimState) {
        Caictua_cactusToCactusCommunication(self);
        dll_amSfx->Play(self, SOUND_720, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    return 0;
}

// offset: 0x1888 | func: 20
s32 Caictua_animState3Dying(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    Caictua_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    fsa->unk341 = 3;
    
    if (fsa->enteredAnimState) {
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        gDLL_18_objfsa->vtbl->func21(self, fsa, 60, 10, 0);
        dll_amSfx->Play(self, SOUND_723, MAX_VOLUME, NULL, NULL, 0, NULL);
        func_800267A4(self);
    }
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    fsa->animTickDelta = 0.0f;

    if (self->opacity >= (gUpdateRate * 2)) {
        self->opacity -= (gUpdateRate * 2);
    } else {
        self->opacity = 0;
    }
    
    if (fsa->unk33A && (self->opacity == 0) && (objData->respawnTimer == 0.0f)) {
        objData->respawnTimer = 1.0f;
        mainSetBits(baddie->unk39E, FALSE);
        mainSetBits(baddie->unk39C, TRUE);
        objAnimSet(self, 0, 0.0f, 0);
        baddie->unk3B6 = FALSE;
        fsa->animState = Caictua_ASTATE_0_Idle;
    }
    
    return 0;
}

// offset: 0x1A74 | func: 21
s32 Caictua_logicState0Top(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->target != NULL) {
        fsa->unk27C = 0.0f;
        fsa->unk278 = 0.0f;
        return FSA_NEXTSTATE_SYNC(Caictua_LSTATE_5_Engage);
    }
    
    return 0;
}

// offset: 0x1AB4 | func: 22
s32 Caictua_logicState1Hit(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->hitpoints <= 0) {
        return FSA_NEXTSTATE_SYNC(Caictua_LSTATE_2_Dying);
    }
    
    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(Caictua_LSTATE_5_Engage);
    }
    
    return 0;
}

// offset: 0x1AF0 | func: 23
s32 Caictua_logicState3Dying(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    Caictua_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (fsa->enteredLogicState) {
        objData->respawnTimer = 0.0f;
        objData->unkC = 0.0f;
        objData->unk8 = 0.0f;
        fsa->animState = Caictua_ASTATE_3_Dying;
    } else if (fsa->unk33A) {
        objSendMesgMany(0, 3, self, 0xE0000, self);
        
        //@bug?: freeing before reaching next state, so a collectable won't be dropped?
        if (self->setup == NULL) {
            objFreeObject(self);
        }

        return FSA_NEXTSTATE_SYNC(Caictua_LSTATE_3_Dead);
    }
    
    return 0;
}

// offset: 0x1BAC | func: 24
s32 Caictua_logicState3Dead(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    //Drop a collectable
    if (fsa->enteredLogicState) {
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, -1, 0);
    }
    
    return 0;
}

// offset: 0x1C14 | func: 25
s32 Caictua_logicState4Disengage(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Caictua_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (fsa->enteredLogicState) {
        objData->respawnTimer = 0.0f;
        objData->unkC = 0.0f;
        objData->unk8 = 0.0f;
        fsa->animState = Caictua_ASTATE_0_Idle;
    }

    return 0;
}

// offset: 0x1C58 | func: 26
s32 Caictua_logicState5Engage(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;

    if (fsa->unk33A || fsa->enteredLogicState) {
        if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2, TRUE)) {
            return FSA_NEXTSTATE_SYNC(Caictua_LSTATE_4_Disengage);
        }
        
        fsa->animState = Caictua_ASTATE_0_Idle;
    }
    
    return 0;
}
