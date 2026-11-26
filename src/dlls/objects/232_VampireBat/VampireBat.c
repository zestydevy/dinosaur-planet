#include "common.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/33_BaddieControl.h"
#include "sys/math.h"
#include "sys/objmsg.h"
#include "sys/objanim.h"
#include "sys/objtype.h"

typedef struct {
    Vec3f home;
    Vec3f goal;
    f32 prevVelocityY;
    f32 heightAboveFloor;
    u16 _unk20;
    s16 dyingPitchSpeed;
    s16 dyingYawSpeed;
    s16 dyingRollSpeed;
    s32 _unk28;
} VampireBat_Data;

typedef enum {
    VampireBat_ASTATE_0_Flying,
    VampireBat_ASTATE_1_Hit
} VampireBat_AnimStates;

typedef enum {
    VampireBat_LSTATE_0_Top,
    VampireBat_LSTATE_1_Fly_Around_Randomly,
    VampireBat_LSTATE_2_Fly_To_Target,
    VampireBat_LSTATE_3_Dying,
    VampireBat_LSTATE_4_Dead
} VampireBat_LogicStates;

/*0x0*/ static s32 dHitAnimStateMap[] = {
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit,
    VampireBat_ASTATE_1_Hit
};
/*0x70*/ static s8 dHitDamageMap[] = {
    -1, -1, -1, -1, 
    -1, -1, -1, -1, 
    -1, -1, -1, -1, 
    -1, -1, -1, -1, 
    -1, -1, -1, -1, 
    -1, -1, -1, -1, 
    -1, -1, -1, -1
};

/*0x0*/ static ObjFSA_StateCallback sAnimStateCallbacks[2];
/*0x8*/ static ObjFSA_StateCallback sLogicStateCallbacks[5];

static s32 VampireBat_animState0Flying(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 VampireBat_animState1Hit(Object* self, ObjFSA_Data* fsa, f32 updateRate);

static s32 VampireBat_logicState0Top(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 VampireBat_logicState2FlyRandom(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 VampireBat_logicState2FlyTarget(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 VampireBat_logicState3Dying(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 VampireBat_logicState4Dead(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void VampireBat_initFSACallbacks(void) {
    sAnimStateCallbacks[VampireBat_ASTATE_0_Flying] = VampireBat_animState0Flying;
    sAnimStateCallbacks[VampireBat_ASTATE_1_Hit]    = VampireBat_animState1Hit;
    
    sLogicStateCallbacks[VampireBat_LSTATE_0_Top]                 = VampireBat_logicState0Top;
    sLogicStateCallbacks[VampireBat_LSTATE_1_Fly_Around_Randomly] = VampireBat_logicState2FlyRandom;
    sLogicStateCallbacks[VampireBat_LSTATE_2_Fly_To_Target]       = VampireBat_logicState2FlyTarget;
    sLogicStateCallbacks[VampireBat_LSTATE_3_Dying]               = VampireBat_logicState3Dying;
    sLogicStateCallbacks[VampireBat_LSTATE_4_Dead]                = VampireBat_logicState4Dead;
}

// offset: 0x78 | ctor
void VampireBat_ctor(void* dll) {
    VampireBat_initFSACallbacks();
}

// offset: 0xB8 | dtor
void VampireBat_dtor(void* dll) { }

// offset: 0xC4 | func: 1 | export: 0
void VampireBat_obj_Setup(Object* self, Baddie_Setup* setup, s32 reset) {
    VampireBat_Data* objData;
    Baddie* baddie;
    u8 flags;

    baddie = self->data;

    flags = 2 | 4;
    if (reset) {
        flags = 1 | 2 | 4;
    }
    if ((setup->unk2B & 0x20) == FALSE) {
        flags |= 8;
    }
    dll_BaddieControl->setup(self, setup, baddie, 2, 5, 0x108, flags, 20.0f);

    self->animCallback = NULL;

    objData = baddie->objdata;
    bzero(objData, sizeof(VampireBat_Data));

    objData->home.x = setup->base.x;
    objData->home.y = setup->base.y;
    objData->home.z = setup->base.z;
    objData->prevVelocityY = 0;
    objData->heightAboveFloor = 0;

    objAnimSet(self, 0, 0, 0);

    baddie->fsa.animState = VampireBat_ASTATE_0_Flying;
    baddie->fsa.logicState = VampireBat_LSTATE_0_Top;
    baddie->fsa.flags |= OBJFSA_FLAG_1000000;
    baddie->fsa.hitpoints = 1;
    baddie->unk3B6 = 0;
    baddie->unk3B4 = 0;

    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_8000 | OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_OBJ_POS | OBJ_SHADOW_FLAG_CUSTOM_DIR;
    }

    baddie->fsa.unk4.mode = 0;

    func_800267A4(self);
}

// offset: 0x23C | func: 2 | export: 1
void VampireBat_obj_Control(Object* self) {
    s16 dFXScales[4] = {0x0206, 0x0167, 0x0165, 0x0206};
    Baddie_Setup* objSetup;
    Baddie* baddie;
    ObjectShadow* objShadow;
    f32 dVelocityY;
    VampireBat_Data* objData;
    Object* player;
    Vec3f delta;
    SRT fxTransform;
    s32 count;
    f32 playerDistBase;
    s32 idx;

    baddie = self->data;
    objSetup = (Baddie_Setup*)self->setup;
    objShadow = self->shadow;
    objData = baddie->objdata;
    player = objGetPlayer();

    if (self->unkDC) {
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

    //Do nothing when dead
    if (baddie->fsa.logicState == VampireBat_LSTATE_4_Dead) {
        return;
    }

    //Advance animation based on change in velocityY
    dVelocityY = self->velocity.y - objData->prevVelocityY;
    objData->prevVelocityY = self->velocity.y;
    if (dVelocityY < 0.0f) {
        dVelocityY *= -0.04f;
    } else {
        dVelocityY *= 0.08f;
    }
    if (dVelocityY > 0.1f) {
        dVelocityY = 0.1f;
    }
    objAnimAdvance(self, dVelocityY, gUpdateRateF, NULL);

    //Get the distance between the player and the bat's base position
    delta.x = objData->home.x - player->srt.transl.x;
    delta.y = objData->home.y - player->srt.transl.y;
    delta.z = objData->home.z - player->srt.transl.z;
    playerDistBase = sqrtf(SQ(delta.f[0]) + SQ(delta.f[1]) + SQ(delta.f[2]));

    //Acquire the player as a target if they're in range
    if (baddie->unk3B6 == 0) {
        if (playerDistBase < baddie->unk3E2) {
            baddie->fsa.target = player;
            baddie->unk3B6 = 1;
        }
    } else {
        if (playerDistBase > baddie->unk3E2) {
            baddie->fsa.target = NULL;
            baddie->unk3B6 = 0;
        }
    }

    //Set shadow position
    objShadow->tr.x = self->srt.transl.x;
    objShadow->tr.z = self->srt.transl.z;
    if (trackGetHeightFloor(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &objData->heightAboveFloor, 0)) {
        objShadow->tr.y = self->srt.transl.y - objData->heightAboveFloor;
    }

    //Get distance from bat to target
    if (baddie->fsa.target != NULL) {
        delta.x = baddie->fsa.target->globalPosition.x - self->globalPosition.x;
        delta.y = baddie->fsa.target->globalPosition.y - self->globalPosition.y;
        delta.z = baddie->fsa.target->globalPosition.z - self->globalPosition.z;
        baddie->fsa.targetDist = sqrtf(SQ(delta.f[0]) + SQ(delta.f[1]) + SQ(delta.f[2]));
    }

    if ((baddie->unk3B0 & 0x20) == FALSE) {
        dll_BaddieControl->func14(self, baddie, &baddie->unk3B2, -1, -1, baddie->unk3A6, baddie->unk3A4);
    }
    dll_BaddieControl->func20(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 0);

    //Create particles when hit by weapon (@bug: not working?)
    if ((baddie->fsa.hitpoints > 0) && 
            (dll_BaddieControl->check_hit(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, dHitAnimStateMap, dHitDamageMap, 1, &baddie->unk3A8, &fxTransform))) {
        idx = ((DLL_Unknown*)player->linkedObject->dll)->vtbl->func[19].withOneVoidArgS32(player->linkedObject);
        if (idx > 3) {
            idx = 3;
        }

        fxTransform.scale = dFXScales[idx];
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, &fxTransform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);

        fxTransform.transl.x -= self->srt.transl.x;
        fxTransform.transl.y -= self->srt.transl.y;
        fxTransform.transl.z -= self->srt.transl.z;
        fxTransform.scale = dFXScales[idx];

        count = 4;
        while (count--) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_324, &fxTransform, PARTFXFLAG_2, -1, NULL);
        }
    }

    dll_BaddieControl->func10(self, &baddie->fsa, 0.0f, -1);
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    self->animObj = baddie->unk3AC;
}

// offset: 0x828 | func: 3 | export: 2
void VampireBat_obj_Update(Object *self) { }

// offset: 0x834 | func: 4 | export: 3
void VampireBat_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** pols, s8 visibility) {
    if (visibility && (self->unkDC == 0)) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x894 | func: 5 | export: 4
void VampireBat_obj_Free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;

    objFreeObjectType(self, OBJTYPE_Baddie);

    if (self->linkedObject != NULL) {
        objFreeObject(self->linkedObject);
        self->linkedObject = NULL;
    }

    dll_BaddieControl->free(self, baddie, 0x20);
}

// offset: 0x938 | func: 6 | export: 5
u32 VampireBat_obj_GetModelFlags(Object* self) {
    return MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x948 | func: 7 | export: 6
u32 VampireBat_obj_GetDataSize(Object *self, u32 offsetAddr) {
    return sizeof(Baddie) + sizeof(VampireBat_Data);
}

// offset: 0x95C | func: 8
static s32 VampireBat_animState0Flying(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    fsa->unk341 = 1;

    return 0;
}

// offset: 0x978 | func: 9
static s32 VampireBat_animState1Hit(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    VampireBat_Data* bat;

    baddie = self->data;
    bat = baddie->objdata;

    fsa->unk341 = 3;

    if (fsa->enteredAnimState) {
        func_800267A4(self);
        self->velocity.x = -self->velocity.x;
        self->velocity.y += 5.0f;
        self->velocity.z = -self->velocity.z;

        bat->dyingYawSpeed = mathRnd(-4000, 4000);
        bat->dyingPitchSpeed = mathRnd(-4000, 4000);
        bat->dyingRollSpeed = mathRnd(-4000, 4000);

        fsa->logicState = VampireBat_LSTATE_3_Dying;
    }

    self->objhitInfo->unk5E = 0;

    return 0;
}

// offset: 0xA88 | func: 10
static s32 VampireBat_logicState0Top(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    fsa->unk27C = 0.0f;
    fsa->unk278 = 0.0f;
    fsa->animState = VampireBat_ASTATE_0_Flying;

    return FSA_NEXTSTATE_SYNC(VampireBat_LSTATE_1_Fly_Around_Randomly);
}

// offset: 0xAB8 | func: 11
static void VampireBat_handleMotion(Object* self, VampireBat_Data* objData) {
    s16 dYaw;

    //Gravitate towards the current goal point (@framerate-dependent)
    {
        if (self->srt.transl.x < objData->goal.x) {
            self->velocity.x += 1.2f;
        } else {
            self->velocity.x -= 1.2f;
        }

        if (self->srt.transl.y < objData->goal.y) {
            self->velocity.y += 1.0f;
        } else {
            self->velocity.y -= 0.75f;
        }

        if (self->srt.transl.z < objData->goal.z) {
            self->velocity.z += 1.2f;
        } else {
            self->velocity.z -= 1.2f;
        }
    }

    if (objData->heightAboveFloor < 25.0f) {
        self->velocity.y = (self->velocity.y * 0.9f) + 0.1f;
    }

    //Lose momentum
    self->velocity.x *= 0.985f;
    self->velocity.y *= 0.945f;
    self->velocity.z *= 0.985f;

    // Limit velocity's x and z components 
    /* @bug: doesn't limit based on the magnitude of the lateral vector,
       so there's a larger max speed when moving diagonally. */
    {
        if (self->velocity.x > 6.0f) {
            self->velocity.x = 6.0f;
        } else if (self->velocity.x < -6.0f) {
            self->velocity.x = -6.0f;
        }

        if (self->velocity.z > 6.0f) {
            self->velocity.z = 6.0f;
        } else if (self->velocity.z < -6.0f) {
            self->velocity.z = -6.0f;
        }
    }

    //Set yaw based on lateral component of velocity vector
    dYaw = self->srt.yaw;
    self->srt.yaw = Arctanf(self->velocity.x, self->velocity.z);
    
    //Roll with changes in yaw
    dYaw = self->srt.yaw - dYaw;
    self->srt.roll += (self->srt.roll - dYaw) >> 2;

    //Move
    objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);
}

// offset: 0xCF0 | func: 12
static s32 VampireBat_logicState2FlyRandom(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    VampireBat_Data* objData;
    s16 randomAngle;
    f32 distance;

    baddie = self->data;
    objData = baddie->objdata;

    //Randomise the goal point: a fixed distance from home laterally in a random direction, at a random height
    randomAngle = mathRnd(-M_180_DEGREES, M_180_DEGREES - 1);
    distance = baddie->unk3E2 * 0.75f;
    objData->goal.x = objData->home.x + Sinf(randomAngle) * distance;
    objData->goal.y = objData->home.y + mathRnd(30, 100);
    objData->goal.z = objData->home.z + Cosf(randomAngle) * distance;

    VampireBat_handleMotion(self, objData);

    //Advance state when a target is acquired
    if (baddie->unk3B6 == 1) {
        return FSA_NEXTSTATE_SYNC(VampireBat_LSTATE_2_Fly_To_Target);
    }

    return 0;
}

// offset: 0xE3C | func: 13
static s32 VampireBat_logicState2FlyTarget(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    VampireBat_Data* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;

    //If the bat has no target, return to picking a random goal point
    if (fsa->target == NULL) {
        return FSA_NEXTSTATE_SYNC(VampireBat_LSTATE_1_Fly_Around_Randomly);
    }

    //Otherwise, gravitate towards the target
    objData->goal.x = fsa->target->srt.transl.x;
    objData->goal.y = fsa->target->srt.transl.y + 30.0f;
    objData->goal.z = fsa->target->srt.transl.z;

    VampireBat_handleMotion(self, objData);

    return 0;
}

// offset: 0xEC8 | func: 14
static s32 VampireBat_logicState3Dying(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    VampireBat_Data* objData;

    baddie = self->data;
    objData = baddie->objdata;

    //Fall and lose momentum laterally (@framerate-dependent)
    self->velocity.x *= 0.985f;
    self->velocity.y = (self->velocity.y - 0.4f) * 0.945f;
    self->velocity.z *= 0.985f;

    fsa->flags |= OBJFSA_FLAG_4000;

    //Spin (@framerate-dependent)
    self->srt.yaw += objData->dyingYawSpeed;
    self->srt.pitch += objData->dyingPitchSpeed;
    self->srt.roll += objData->dyingRollSpeed;

    objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);

    //When hitting the floor: send a message, advance state, and free self
    if (objData->heightAboveFloor <= 0.0f) {
        objSendMesgMany(0, OBJMSG_SEND_ALL | OBJMSG_SEND_IGNORE_SENDER, self, MSG_Clear_Target, self);
        objFreeObject(self);
        return FSA_NEXTSTATE_SYNC(VampireBat_LSTATE_4_Dead);
    }

    return 0;
}

// offset: 0x100C | func: 15
static s32 VampireBat_logicState4Dead(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;

    //Drop a collectable
    if (fsa->enteredLogicState) {
        dll_BaddieControl->drop_collectable(self, baddie->unk3E0, -1, 0);
        gDLL_18_objfsa->vtbl->func21(self, fsa, PARTICLE_3C, 10, 0);
        baddie->unk3B4 = 0;
    }

    return 0;
}
