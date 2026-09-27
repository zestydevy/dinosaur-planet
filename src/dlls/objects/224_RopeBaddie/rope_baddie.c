#include "common.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/420_DFropenode.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/objhits.h"
#include "sys/objtype.h"

typedef struct {
    f32 initialYDiff;         //The difference between the RopeBaddie's initial Y and its Y after being snapped to the rope
    f32 initialPointY;        //The Baddie's initial Y coord, after being snapped to the rope
    f32 initialY;             //The Baddie's initial Y coord, before finding by the rope
    Vec4f spline;             //Rope spline
    Vec3f initialPoint;       //Coords for the Baddie's initial position along the rope (worldSpace)
    u8 _unk28[0x34 - 0x28];
    Object* initialRopeNode;  //DFropenode Object
    Object* rope;             //DFropenode Object
    f32 nodeDistance;         //For initial rope node search: distance to the DFropenode
    f32 searchedRopePosition; //For initial rope node search: measures the Baddie's position along the searched rope
    s8 _unk44;
    s8 direction;             //Which way the Baddie's facing along the rope (0 or 1)
    u8 unk46;                 //Set to 0, but otherwise unused
    f32 ropePosition;         //Measures the Baddie's current position along the rope (a value from ~0.3 to 6.7)
    f32 initialRopePosition;  //Measures the Baddie's initial position along the rope (a value from ~0.3 to 6.7)
    f32 freezeTimer;          //The Baddie's joints glow when this is greater than zero, similar to SharpClaws' frozen effect. Seems to be unused!
    f32 goalPosition;         //The position the Baddie will try to move towards along the rope
    s16 ropeYaw;              //The angle between the rope's ends (viewed from above)
} RopeBaddie_DataActual;

typedef enum {
    RopeBaddie_ASTATE_0_Walking,     //Traversing the rope idly
    RopeBaddie_ASTATE_1_Running,     //Chasing after the player once they're on the rope
    RopeBaddie_ASTATE_2_Turning,     //Turning 180 on the rope
    RopeBaddie_ASTATE_3_Idle,        //Waiting in place on the rope
    RopeBaddie_ASTATE_4_Combat_Idle, //After attacking, waiting to bite
    RopeBaddie_ASTATE_5_Bite,        //After waiting to attack again
    RopeBaddie_ASTATE_6_Attack,      //Tackles or punches the player
    RopeBaddie_ASTATE_7_Hit,         //Recoils from being hit
    RopeBaddie_ASTATE_8_Dying,       //Curls up, goes limp, and disappears
    RopeBaddie_ASTATE_9_Respawning   //After Gplay timer expires
} RopeBaddie_AnimStates;

typedef enum {
    RopeBaddie_LSTATE_0_Top,
    RopeBaddie_LSTATE_1_Respawning,
    RopeBaddie_LSTATE_2_Chase,
    RopeBaddie_LSTATE_3_Hit,
    RopeBaddie_LSTATE_4_Dying,
    RopeBaddie_LSTATE_5_Dead
} RopeBaddie_LogicStates;

typedef enum {
    RopeBaddie_MODANIM_0_Walk_LOOP,  //Moving hand-over-hand along the rope, leading with left arm. More of a climb really!
    RopeBaddie_MODANIM_1_Tackle,     //Coils back in anticipation, then lunges forward violently. Takes one "step" forward and back with left arm in the process, before settling back into netural.
    RopeBaddie_MODANIM_2_Idle_LOOP,  //Clinging on in neutral pose (right arm leading and left arm behind), with mouth agape. Tail swing around gently.
    RopeBaddie_MODANIM_3_Turn,       //Turns anticlockwise (viewed from above) to face the other way. Root Y counter-rotates throughout with even/linear spacing.
    RopeBaddie_MODANIM_4_Punch,      //Similar to [mAnim1], winds up in anticipation, but then unleashes a mean left hook! Clings on with right arm throughout.
    RopeBaddie_MODANIM_5_Sway_LOOP,  //Snakes body back and forth from the shoulders, as though ready to fight.
    RopeBaddie_MODANIM_6_Bite,       //Winds back slightly, then bites out ahead. Returns to near neutral.
    RopeBaddie_MODANIM_7_Recoil,     //Reels head backwards in pain and curls up slightly, before returning to neutral.
    RopeBaddie_MODANIM_8_Dying,      //Curls body up fully between arms, then goes limp with arms still hooked onto the rope.
    RopeBaddie_MODANIM_9_Dead        //From [mAnim8]'s end pose, sways back and forward slightly (unused?) [Root shifted way up vertically!]
} RopeBaddie_ModAnims;

/*0x0*/ static s32 dHitAnimStateMap[] = {
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit,
    RopeBaddie_ASTATE_7_Hit
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
/*0x8C*/ static u32 dAttackSoundIDs[] = {
    SOUND_483_RopeBaddie_Squeak, SOUND_484_RopeBaddie_Roar, SOUND_486_RopeBaddie_Cry
};
/*0x98*/ static u32 dBiteSounds[] = {
    SOUND_485_RopeBaddie_Babble
};

/*0x0*/ static ObjFSA_StateCallback sAnimStateCallbacks[10];
/*0x28*/ static ObjFSA_StateCallback sLogicStateCallbacks[6];

static void RopeBaddie_handleMessagesAndDamage(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void RopeBaddie_engageTarget(Object* self, s32 arg1, Baddie* baddie, ObjFSA_Data* fsa);
static void RopeBaddie_searchForTarget(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void RopeBaddie_findRopeNodes(Object* self);

static s32 RopeBaddie_initialiseRopeAttach(Object* self, RopeBaddie_DataActual* objData);
static int RopeBaddie_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

static s32 RopeBaddie_animState0Walking(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_animState1Running(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_animState2Turning(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_animState3Idle(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_animState4CombatIdle(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_animState5Bite(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_animState6Attack(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_animState7Hit(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_animState8Dying(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_animState9Respawning(Object* self, ObjFSA_Data* fsa, f32 updateRate);

static s32 RopeBaddie_logicState0Top(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_logicState1Respawning(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_logicState2Chase(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_logicState3Hit(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_logicState4Dying(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 RopeBaddie_logicState5Dead(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void RopeBaddie_initFSACallbacks(void) {
    sAnimStateCallbacks[RopeBaddie_ASTATE_0_Walking]     = RopeBaddie_animState0Walking;
    sAnimStateCallbacks[RopeBaddie_ASTATE_1_Running]     = RopeBaddie_animState1Running;
    sAnimStateCallbacks[RopeBaddie_ASTATE_2_Turning]     = RopeBaddie_animState2Turning;
    sAnimStateCallbacks[RopeBaddie_ASTATE_3_Idle]        = RopeBaddie_animState3Idle;
    sAnimStateCallbacks[RopeBaddie_ASTATE_4_Combat_Idle] = RopeBaddie_animState4CombatIdle;
    sAnimStateCallbacks[RopeBaddie_ASTATE_5_Bite]        = RopeBaddie_animState5Bite;
    sAnimStateCallbacks[RopeBaddie_ASTATE_6_Attack]      = RopeBaddie_animState6Attack;
    sAnimStateCallbacks[RopeBaddie_ASTATE_7_Hit]         = RopeBaddie_animState7Hit;
    sAnimStateCallbacks[RopeBaddie_ASTATE_8_Dying]       = RopeBaddie_animState8Dying;
    sAnimStateCallbacks[RopeBaddie_ASTATE_9_Respawning]  = RopeBaddie_animState9Respawning;

    sLogicStateCallbacks[RopeBaddie_LSTATE_0_Top]        = RopeBaddie_logicState0Top;
    sLogicStateCallbacks[RopeBaddie_LSTATE_1_Respawning] = RopeBaddie_logicState1Respawning;
    sLogicStateCallbacks[RopeBaddie_LSTATE_2_Chase]      = RopeBaddie_logicState2Chase;
    sLogicStateCallbacks[RopeBaddie_LSTATE_3_Hit]        = RopeBaddie_logicState3Hit;
    sLogicStateCallbacks[RopeBaddie_LSTATE_4_Dying]      = RopeBaddie_logicState4Dying;
    sLogicStateCallbacks[RopeBaddie_LSTATE_5_Dead]       = RopeBaddie_logicState5Dead;
}

// offset: 0xE4 | ctor
void RopeBaddie_ctor(void* dll) {
    RopeBaddie_initFSACallbacks();
}

// offset: 0x124 | dtor
void RopeBaddie_dtor(void* dll) { }

// offset: 0x130 | func: 1 | export: 0
void RopeBaddie_obj_Setup(Object* self, Baddie_Setup* objSetup, s32 reset) {
    Baddie* baddie;
    RopeBaddie_DataActual* objData;
    u8 flags;

    baddie = self->data;

    flags = 2;
    if (reset) {
        flags = 3;
    }
    dll_BaddieControl->setup(self, objSetup, baddie, 0, 0, 0, flags, 20.0f);

    self->animCallback = RopeBaddie_animCallback;

    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, RopeBaddie_ASTATE_0_Walking);
    baddie->fsa.logicState = RopeBaddie_LSTATE_0_Top;
    baddie->fsa.unk278 = 0.0f;

    objData = baddie->objdata;
    objData->initialRopeNode = NULL;
}

// offset: 0x1F8 | func: 2 | export: 1
void RopeBaddie_obj_Control(Object* self) {
    Baddie* baddie;
    RopeBaddie_DataActual* objData;
    Baddie_Setup* objSetup;

    baddie = self->data;
    objSetup = (Baddie_Setup*)self->setup;
    objData = baddie->objdata;

    if (self->unkDC) {
        if (gDLL_29_Gplay->vtbl->did_time_expire(objSetup->base.uID)) {
            dll_BaddieControl->setup(self, objSetup, baddie, 10, 6, 0x10E, 0x36, 20.0f);
            baddie->fsa.logicState = RopeBaddie_LSTATE_1_Respawning;
            baddie->fsa.enteredLogicState = TRUE;
            self->opacity = 0;
        }
        return;
    }

    if (objData->initialRopeNode) {
        gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, 1.0f, 1.0f, sAnimStateCallbacks, sLogicStateCallbacks);

        dll_DFropenode(objData->rope)->func8(objData->rope, objData->ropePosition, &self->srt.transl.x, &self->srt.transl.y, &self->srt.transl.z);

        RopeBaddie_handleMessagesAndDamage(self, baddie, &baddie->fsa);
        if ((baddie->fsa.target != NULL) || (baddie->fsa.hitpoints == 0)) {
            RopeBaddie_engageTarget(self, 0, baddie, &baddie->fsa);
        } else {
            RopeBaddie_searchForTarget(self, baddie, &baddie->fsa);
        }
    } else {
        RopeBaddie_findRopeNodes(self);
    }
}

// offset: 0x3F0 | func: 3 | export: 2
void RopeBaddie_obj_Update(Object* self) {
    gDLL_18_objfsa->vtbl->func2(self, self->data, sAnimStateCallbacks);
}

// offset: 0x440 | func: 4 | export: 3
void RopeBaddie_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie;
    RopeBaddie_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;

    if (visibility && (self->unkDC == 0)) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);

        //Unused? Creates a glowing effect, very similar to when SharpClaws are frozen with Ice Blast!
        if (objData->freezeTimer > 0.0f) {
            gDLL_32_modelfx->vtbl->func2(self, PARTICLE_52A, NULL);
        }

        if (baddie->unk3B2 & (0x40 | 0x20)) {
            if (baddie->unk3B2 & 0x20) {
                gDLL_32_modelfx->vtbl->func2(self, PARTICLE_330, &baddie->unk3E8);
                gDLL_32_modelfx->vtbl->func2(self, PARTICLE_330, &baddie->unk3E8);
            }
            gDLL_32_modelfx->vtbl->func2(self, PARTICLE_32F, &baddie->unk3E8);
        }
        if (baddie->unk3B2 & 0x100) {
            gDLL_32_modelfx->vtbl->func2(self, PARTICLE_333, &baddie->unk3E8);
            gDLL_32_modelfx->vtbl->func2(self, PARTICLE_334, &baddie->unk3E8);
            baddie->unk3B2 &= ~0x100;
        }
    }
}

// offset: 0x600 | func: 5 | export: 4
void RopeBaddie_obj_Free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    objFreeObjectType(self, OBJTYPE_Baddie);
    dll_BaddieControl->free(self, baddie, 0);
}


// offset: 0x670 | func: 6 | export: 5
u32 RopeBaddie_obj_GetModelFlags(Object* self) {
    return MODFLAGS_EVENTS | MODFLAGS_10 | MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x680 | func: 7 | export: 6
u32 RopeBaddie_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(Baddie) + sizeof(RopeBaddie_DataActual);
}

// offset: 0x694 | func: 8
void RopeBaddie_handleMessagesAndDamage(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    s32 damageType;

    //Handle messages
    dll_BaddieControl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 0, 0, 0);

    //Check for damage
    damageType = dll_BaddieControl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, dHitAnimStateMap, dHitDamageMap, RopeBaddie_LSTATE_3_Hit, &baddie->unk3A8, NULL);
    if (damageType == Damage_Type_Projectile) {
        baddie->unk3B4 = 2;
        fsa->target = objGetPlayer();
    }
}

// offset: 0x794 | func: 9
void RopeBaddie_engageTarget(Object* self, s32 arg1, Baddie* baddie, ObjFSA_Data* fsa) {
    self->objhitInfo->unk58 |= ObjHitInfo_FLAG_1;

    //Check if the baddie should disengage the player
    if (dll_BaddieControl->func16(self, fsa, baddie->unk3E2, 1)) {
        fsa->target = NULL;
    }
}

// offset: 0x830 | func: 10
void RopeBaddie_searchForTarget(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    Object* target;

    self->objhitInfo->unk58 &= ~ObjHitInfo_FLAG_1;

    target = dll_BaddieControl->func17(self, fsa, baddie->unk3E2, M_180_DEGREES);
    if (target != NULL) {
        fsa->target = target;
        fsa->unk33D = 0;
    }
}

// offset: 0x8CC | func: 11
void RopeBaddie_findRopeNodes(Object* self) {
    Baddie* baddie;
    RopeBaddie_DataActual* objData;
    Object** ropes;
    s32 i;
    s32 count;
    f32 distance;
    f32 ropePosition;
    s8 sp53;
    s32 yawOffset;

    baddie = self->data;
    ropes = objGetAllOfType(OBJTYPE_RopeNode, &count);
    if (count == 0) {
        return;
    }

    objData = baddie->objdata;
    objData->initialRopeNode = NULL;
    objData->nodeDistance = 200.0f;

    //Find the closest rope node
    for (i = 0; i < count; i++) {
        if (dll_DFropenode(ropes[i])->func11(ropes[i], self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &distance, &ropePosition, &sp53) && (distance < objData->nodeDistance)) {
            objData->initialRopeNode = ropes[i];
            objData->nodeDistance = distance;
            objData->searchedRopePosition = ropePosition;
        }
    }

    if (objData->initialRopeNode != NULL) {
        objData->ropePosition = objData->searchedRopePosition;
        objData->rope = objData->initialRopeNode;
        RopeBaddie_initialiseRopeAttach(self, objData);

        //Set yaw, adding M_180_DEGREES (effectively) if the baddie's facing the other way
        yawOffset = objData->direction == 0;
        self->srt.yaw = objData->ropeYaw + (yawOffset << 0xF);

        objData->goalPosition = objData->ropePosition - ((mathRnd(10, 60) / 10.0f) * ((objData->direction * 2) - 1));

        if (objData->goalPosition > 1.0f) {
            objData->goalPosition = objData->goalPosition;
        } else {
            objData->goalPosition = 1.0f;
        }

        if (objData->goalPosition < 6.0f) {
            objData->goalPosition = objData->goalPosition;
        } else {
            objData->goalPosition = 6.0f;
        }
    }
}

// offset: 0xAE4 | func: 12
s32 RopeBaddie_initialiseRopeAttach(Object* self, RopeBaddie_DataActual* objData) {
    s32 yawDiff;

    dll_DFropenode(objData->rope)->func7(objData->rope, &objData->spline);
    dll_DFropenode(objData->rope)->func8(objData->rope, objData->ropePosition, &objData->initialPoint.x, &objData->initialPoint.y, &objData->initialPoint.z);

    objData->ropeYaw = dll_DFropenode(objData->rope)->func12(objData->rope);
    objData->unk46 = 0;
    objData->initialRopePosition = objData->ropePosition;
    objData->initialPointY = objData->initialPoint.y;
    objData->initialY = self->srt.transl.y;
    objData->initialYDiff = objData->initialPoint.y - objData->initialY;

    yawDiff = self->srt.yaw - (objData->ropeYaw & 0xFFFF);
    CIRCLE_WRAP(yawDiff);

    objData->direction = !(yawDiff > (M_90_DEGREES - 4)) && !(yawDiff < -(M_90_DEGREES - 4));

    return 0;
}

// offset: 0xC10 | func: 13
static s16 RopeBaddie_getPitchAngle(RopeBaddie_DataActual* objData) {
    s32 angle;
    f32 Ax;
    f32 Ay;
    f32 Az;
    f32 Bx;
    f32 By;
    f32 Bz;

    dll_DFropenode(objData->rope)->func8(objData->rope, objData->ropePosition - 0.1f, &Ax, &Ay, &Az);
    dll_DFropenode(objData->rope)->func8(objData->rope, objData->ropePosition + 0.1f, &Bx, &By, &Bz);

    Ax -= Bx;
    Ay -= By;
    Az -= Bz;

    Ax = sqrtf(SQ(Ax) + SQ(Az));
    angle = mathAtan2f(Ay, Ax);
    return (s16)angle * ((objData->direction * 2) - 1);

}

// offset: 0xD48 | func: 14
/**
  * Clamps objData->ropePosition so it's between 0.3 and 6.7 (inclusive).
  * Returns TRUE if the Baddie is at the minimum or maximum position along the rope.
  */
static s32 RopeBaddie_clampRopePosition(RopeBaddie_DataActual* objData) {
    if (objData->ropePosition < 0.3f) {
        objData->ropePosition = 0.3f;
        return TRUE;
    }

    if (objData->ropePosition > 6.7f) {
        objData->ropePosition = 6.7f;
        return TRUE;
    }

    return FALSE;
}

// offset: 0xDAC | func: 15
static void RopeBaddie_squawk(Object* self) {
    dll_amSfx->Play(self, SOUND_482_RopeBaddie_Squawk, MAX_VOLUME, NULL, NULL, 0, NULL);
}

// offset: 0xE08 | func: 16
static void RopeBaddie_handleAnimRopeSound(Object* self, ObjFSA_Data* fsa) {
    if (fsa->unk308 & 1) {
        fsa->unk308 &= ~1;
        dll_amSfx->Play(self, SOUND_768_Rope_Climb, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
}

// offset: 0xE84 | func: 17
int RopeBaddie_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    return 0;
}

// offset: 0xEA0 | func: 18
s32 RopeBaddie_animState0Walking(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    RopeBaddie_DataActual* objData;
    Baddie* baddie;
    u16 turnAmount;
    s16 yawDiff;
    u16 distance;

    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        objAnimSet(self, RopeBaddie_MODANIM_0_Walk_LOOP, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    fsa->animTickDelta = 0.03f;

    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);

    dll_DFropenode(objData->rope)->func9(objData->rope, &objData->ropePosition, (1 - (objData->direction * 2)) * fsa->unk278);

    RopeBaddie_clampRopePosition(objData);

    dll_BaddieControl->func4(self, fsa->target, 0x10, &turnAmount, &yawDiff, &distance);

    if ((turnAmount >= 4) && (turnAmount < 0xC) && (distance > 400)) {
        if ((objData->ropePosition > 2.0f) && (objData->ropePosition < 5.0f)) {
            return FSA_NEXTSTATE_SYNC(RopeBaddie_ASTATE_2_Turning);
        }
    }

    if (((objData->direction != (0, objData->goalPosition <= objData->ropePosition)) != 0) && fsa->unk33A) { //fake
        return FSA_NEXTSTATE_SYNC(RopeBaddie_ASTATE_2_Turning);
    }

    RopeBaddie_handleAnimRopeSound(self, fsa);
    self->srt.pitch = RopeBaddie_getPitchAngle(objData);

    return 0;
}

// offset: 0x10C4 | func: 19
s32 RopeBaddie_animState1Running(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    RopeBaddie_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        objAnimSet(self, RopeBaddie_MODANIM_0_Walk_LOOP, 0.0f, 0);
        fsa->unk33A = FALSE;
    }

    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);

    RopeBaddie_handleAnimRopeSound(self, fsa);

    dll_DFropenode(objData->rope)->func9(objData->rope, &objData->ropePosition, (1 - (objData->direction * 2)) * fsa->animTickDelta * 50.4f);

    if (RopeBaddie_clampRopePosition(objData)) {
        return FSA_NEXTSTATE_SYNC(RopeBaddie_ASTATE_6_Attack);
    }

    self->srt.pitch = RopeBaddie_getPitchAngle(objData);

    return 0;
}

// offset: 0x1218 | func: 20
s32 RopeBaddie_animState2Turning(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    u16 turnAmount;
    s16 yawDiff;
    u16 distance;
    f32 delta;
    Baddie* baddie;
    RopeBaddie_DataActual* objData;
    s32 direction;

    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        objAnimSet(self, RopeBaddie_MODANIM_3_Turn, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    fsa->animTickDelta = 0.03f;

    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 9);

    dll_DFropenode(objData->rope)->func9(objData->rope, &objData->ropePosition, (1 - (objData->direction * 2)) * fsa->unk278);

    RopeBaddie_clampRopePosition(objData);
    self->srt.pitch = RopeBaddie_getPitchAngle(objData) * (1.0f - (2.0f * self->animProgress));

    if (fsa->unk33A) {
        dll_BaddieControl->func4(self, fsa->target, 0x10, &turnAmount, &yawDiff, &distance);
        objData->direction = 1 - objData->direction;

        direction = objData->direction == 0;
        self->srt.yaw = objData->ropeYaw + (direction << 0xF);

        delta = (mathRnd(50, 100) / 100.0f) * ((objData->direction * 2) - 1);
        if ((turnAmount < 4) || (turnAmount >= 12)) {
            if (distance > 500) {
                delta *= 1.0f + (distance / 100.0f);
            } else {
                delta *= 1.0f + (distance / 300.0f);
            }
        }

        objData->goalPosition = objData->ropePosition - delta;

        if (objData->goalPosition > 1.0f) {
            objData->goalPosition = objData->goalPosition;
        } else {
            objData->goalPosition = 1.0f;
        }

        if (objData->goalPosition < 6.0f) {
            objData->goalPosition = objData->goalPosition;
        } else {
            objData->goalPosition = 6.0f;
        }

        return FSA_NEXTSTATE_SYNC(RopeBaddie_ASTATE_3_Idle);
    }

    return 0;
}

// offset: 0x1514 | func: 21
s32 RopeBaddie_animState3Idle(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    RopeBaddie_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        objAnimSet(self, RopeBaddie_MODANIM_2_Idle_LOOP, 0.0f, 0);
        fsa->unk33A = FALSE;
    }

    fsa->animTickDelta = 0.025f;
    self->srt.pitch = RopeBaddie_getPitchAngle(objData);

    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(RopeBaddie_ASTATE_0_Walking);
    } else {
        return 0;
    }
}

// offset: 0x15D0 | func: 22
s32 RopeBaddie_animState4CombatIdle(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    RopeBaddie_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        objAnimSet(self, RopeBaddie_MODANIM_5_Sway_LOOP, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    fsa->animTickDelta = 0.03f;

    self->srt.pitch = RopeBaddie_getPitchAngle(objData);

    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(RopeBaddie_ASTATE_5_Bite);
    } else {
        return 0;
    }
}

// offset: 0x168C | func: 23
s32 RopeBaddie_animState5Bite(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    RopeBaddie_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        objAnimSet(self, RopeBaddie_MODANIM_6_Bite, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    fsa->animTickDelta = 0.03f;

    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 0, dBiteSounds);
    self->srt.pitch = RopeBaddie_getPitchAngle(objData);

    //@bug: the animState can get stuck here temporarily if the Baddie loses its target and stays in logicState0

    return 0;
}

// offset: 0x1768 | func: 24
s32 RopeBaddie_animState6Attack(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    RopeBaddie_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;

    self->objhitInfo->unk5F = 9;
    self->objhitInfo->unk60 = 1;
    func_80028D2C(self);

    if (mathRnd(0, 100) < 50) {
        if (fsa->enteredAnimState) {
            objAnimSet(self, RopeBaddie_MODANIM_1_Tackle, 0.0f, 0);
            fsa->unk33A = FALSE;
        }
    } else {
        if (fsa->enteredAnimState) {
            objAnimSet(self, RopeBaddie_MODANIM_4_Punch, 0.0f, 0);
            fsa->unk33A = FALSE;
        }
    }

    fsa->animTickDelta = 0.03f;
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, mathRnd(0, ARRAYCOUNT(dAttackSoundIDs) - 1), dAttackSoundIDs);

    dll_DFropenode(objData->rope)->func9(objData->rope, &objData->ropePosition, (1 - (objData->direction * 2)) * fsa->unk278);

    RopeBaddie_clampRopePosition(objData);
    self->srt.pitch = RopeBaddie_getPitchAngle(objData);

    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(RopeBaddie_ASTATE_4_Combat_Idle);
    } else {
        return 0;
    }
}

// offset: 0x1974 | func: 25
s32 RopeBaddie_animState7Hit(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 yawDiff;
    Baddie* baddie;
    RopeBaddie_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        objAnimSet(self, RopeBaddie_MODANIM_7_Recoil, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    if (fsa->enteredAnimState) {
        RopeBaddie_squawk(self);
    }
    fsa->animTickDelta = 0.018f;

    yawDiff = self->srt.yaw - (objData->ropeYaw & 0xFFFF);
    CIRCLE_WRAP(yawDiff);
    self->srt.yaw = objData->ropeYaw;

    if ((yawDiff > (M_90_DEGREES - 4)) || (yawDiff < -(M_90_DEGREES - 4))) {
        self->srt.yaw += M_180_DEGREES;
    }

    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;

    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(RopeBaddie_ASTATE_0_Walking);
    } else {
        return 0;
    }
}

// offset: 0x1AA4 | func: 26
s32 RopeBaddie_animState8Dying(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;

    if (fsa->enteredAnimState) {
        objAnimSet(self, RopeBaddie_MODANIM_8_Dying, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    fsa->animTickDelta = 0.048f;

    if (fsa->unk308 & 0x200) {
        dll_amSfx->Play(self, SOUND_B1F_Slow_Magic_Chimes, MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk308 &= ~0x200;
        dll_BaddieControl->drop_collectable(self, baddie->unk3E0, -1, 1);
    }

    return 0;
}

// offset: 0x1BAC | func: 27
s32 RopeBaddie_animState9Respawning(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;

    fsa->unk341 = 0;
    fsa->animTickDelta = 0.01f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;

    if (fsa->enteredAnimState) {
        dll_amSfx->Play(self, SOUND_B20_Low_Grunt, MAX_VOLUME, NULL, NULL, 0, NULL);
        if (fsa->enteredAnimState) {
            objAnimSet(self, RopeBaddie_MODANIM_2_Idle_LOOP, 0.0f, 0);
            fsa->unk33A = FALSE;
        }
        fsa->animTickDelta = 0.025f;
        fsa->unk33A = FALSE;
        self->opacity = OBJECT_OPACITY_MAX;
        baddie->unk3B2 |= 0x100;
    }

    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(RopeBaddie_ASTATE_0_Walking);
    } else {
        return 0;
    }
}

// offset: 0x1CBC | func: 28
s32 RopeBaddie_logicState0Top(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Object* target;
    RopeBaddie_DataActual* objData;
    u16 turnAmount;
    s16 yawDiff;
    u16 distance;
    Baddie* baddie;

    target = fsa->target;
    baddie = self->data;
    objData = baddie->objdata;

    if (target != NULL) {
        if ((dll_player(target)->func45(target) == objData->rope) && (fsa->animState != RopeBaddie_ASTATE_2_Turning) && ((4.0f * gUpdateRateF) < fsa->logicStateTime)) {
            dll_BaddieControl->func4(self, fsa->target, 0x10, &turnAmount, &yawDiff, &distance);
            if (turnAmount < 4 || turnAmount >= 12) {
                return FSA_NEXTSTATE_SYNC(RopeBaddie_LSTATE_2_Chase);
            }

            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, RopeBaddie_ASTATE_2_Turning);
            fsa->animTickDelta = 0.028f;
            fsa->unk33A = FALSE;
        }
    }

    return 0;
}

// offset: 0x1E10 | func: 29
s32 RopeBaddie_logicState1Respawning(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, RopeBaddie_ASTATE_9_Respawning);
    }

    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(RopeBaddie_LSTATE_0_Top);
    } else {
        return 0;
    }
}

// offset: 0x1E84 | func: 30
s32 RopeBaddie_logicState2Chase(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    f32 distance;
    Object* target;
    f32 dx;
    f32 dz;
    s32 yawDiff;

    target = fsa->target;

    //Return to non-combat state if the target's lost, or if the player's no longer on a rope
    if ((target == NULL) || dll_player(target)->func45(target) == NULL) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, RopeBaddie_ASTATE_0_Walking);
        return FSA_NEXTSTATE_SYNC(RopeBaddie_LSTATE_0_Top);
    }

    if (fsa->animState != RopeBaddie_ASTATE_6_Attack) {
        //Get the angle to the player
        dx = self->srt.transl.x - target->srt.transl.x;
        dz = self->srt.transl.z - target->srt.transl.z;
        yawDiff = (mathAtan2f(dx, dz) - self->srt.yaw) & 0xFFFF;
        
        //Check if the player's behind the Baddie, or get their distance when they're in front
        if ((yawDiff > M_90_DEGREES) && (yawDiff < M_90_DEGREES * 3)) {
            dx = -100.0f;
        } else {
            dx = sqrtf(SQ(dx) + SQ(dz)) - 45.0f;
        }

        if (dx < 0.0f) {
            distance = -dx;
        } else {
            distance = dx;
        }

        //Attack when the player's close
        if (distance < 1.0f && (fsa->animState == RopeBaddie_ASTATE_1_Running || (fsa->animState == RopeBaddie_ASTATE_5_Bite && fsa->unk33A))) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, RopeBaddie_ASTATE_6_Attack);
        } else if (fsa->animState != RopeBaddie_ASTATE_1_Running) {
            //Chase forward
            if ((dx > 2.5f) && (fsa->animState != RopeBaddie_ASTATE_4_Combat_Idle) && (fsa->animState != RopeBaddie_ASTATE_5_Bite || fsa->unk33A)) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, RopeBaddie_ASTATE_1_Running);
            }

            //Backpedal if the player is too close
            if (dx < -2.5f) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, RopeBaddie_ASTATE_1_Running);
            }
        }

        if (fsa->animState == RopeBaddie_ASTATE_1_Running) {
            if (dx > 0.0f) {
                fsa->animTickDelta = 0.04f;
            } else {
                //Backpedal if the player is too close
                fsa->animTickDelta = -0.07f;
            }
        }
    }

    return 0;
}

// offset: 0x2174 | func: 31
s32 RopeBaddie_logicState3Hit(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->hitpoints <= 0) {
        return FSA_NEXTSTATE_SYNC(RopeBaddie_LSTATE_4_Dying);
    } else {
        return FSA_NEXTSTATE_SYNC(RopeBaddie_LSTATE_0_Top);
    }
}

// offset: 0x219C | func: 32
s32 RopeBaddie_logicState4Dying(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, RopeBaddie_ASTATE_8_Dying);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        func_800267A4(self);
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    }

    if (self->opacity == 0) {
        if (self->setup == NULL) {
            objFreeObject(self);
        }
        return FSA_NEXTSTATE_SYNC(RopeBaddie_LSTATE_5_Dead);
    }

    return 0;
}

// offset: 0x2264 | func: 33
s32 RopeBaddie_logicState5Dead(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;

    if (fsa->enteredLogicState) {
        baddie->unk3B4 = 0;
        mainSetBits(baddie->unk39E, FALSE);
        mainSetBits(baddie->unk39C, TRUE);
    }

    return 0;
}
