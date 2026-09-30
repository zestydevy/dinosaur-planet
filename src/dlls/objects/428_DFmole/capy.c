#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/33_BaddieControl.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object_id.h"
#include "sys/math.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/objprint.h"
#include "dlls/objects/common/foodbag.h"
#include "dlls/objects/210_player.h"
#include "dll.h"

#include "prevent_bss_reordering.h"

// size:0x14
typedef struct {
/*00*/ u8 fed;
/*01*/ u8 ateMagicPlant;
/*02*/ u8 turnSpeedFactor;
/*04*/ u16 timer;
/*08*/ u32 soundHandle;
/*0C*/ f32 prevX;
/*10*/ f32 prevZ;
} capy_data;

enum CapyAnimStates {
    CAPY_ASTATE_0_Standing = 0,
    CAPY_ASTATE_1_Burrowed = 1,
    CAPY_ASTATE_2_Unburrow = 2,
    CAPY_ASTATE_3_Burrow = 3,
    CAPY_ASTATE_4_Sniff = 4,
    CAPY_ASTATE_5_Eat = 5,
    CAPY_ASTATE_6_DoneEating = 6,
    CAPY_ASTATE_7_DigWall = 7,
    CAPY_ASTATE_8_Walking = 8
};

enum CapyLogicStates {
    CAPY_LSTATE_0_Init = 0,
    CAPY_LSTATE_1_DigWall = 1,
    CAPY_LSTATE_2_GoToDigSpot = 2,
    CAPY_LSTATE_3_Eating = 3,
    CAPY_LSTATE_4_Idle = 4,             //This state also handles walking to a target (Magic Plant/Energy Egg)
    CAPY_LSTATE_5_Underground = 5
};

enum CapyModAnimIndices {
    CAPY_MODANIM_0_Burrow = 0,
    CAPY_MODANIM_1_Unburrow = 1,
    CAPY_MODANIM_2_DigWall = 2,
    CAPY_MODANIM_3_Standing = 3,
    CAPY_MODANIM_4_Walking = 4,
    CAPY_MODANIM_5_Sniffing = 5,
    CAPY_MODANIM_6_Eating = 6,
    CAPY_MODANIM_7_DoneEating = 7
};

/*0x0*/ static s32 dCurveTypes[1] = {2};
/*0x4*/ static u16 sTunnelGamebits[] = {
    BIT_CapyTunnel1, BIT_CapyTunnel2, BIT_CapyTunnel3
};

/*0x0*/ static  ObjFSA_StateCallback sAnimCallbacks[9];
/*0x28*/ static ObjFSA_StateCallback sLogicCallbacks[6];

static void capy_findFoodTarget(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void capy_handlePlayerTarget(Object* self, s32 arg1, Baddie* baddie, ObjFSA_Data* fsa);
static void capy_searchForTarget(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static int capy_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

static s32 capy_animState0Standing(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 capy_animState1Burrowed(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 capy_animState2Unburrow(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 capy_animState3Burrow(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 capy_animState4Sniff(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 capy_animState5Eat(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 capy_animState6DoneEating(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 capy_animState7DigWall(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 capy_animState8Walking(Object* self, ObjFSA_Data* fsa, f32 updateRate);

static s32 capy_logicState0Init(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 capy_logicState1DigWall(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 capy_logicState2GoToDigSpot(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 capy_logicState3Eating(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 capy_logicState4Idle(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 capy_logicState5Underground(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void capy_setupFSACallbacks(void) {
    sAnimCallbacks[CAPY_ASTATE_0_Standing]     = capy_animState0Standing;
    sAnimCallbacks[CAPY_ASTATE_1_Burrowed]     = capy_animState1Burrowed;
    sAnimCallbacks[CAPY_ASTATE_2_Unburrow]     = capy_animState2Unburrow;
    sAnimCallbacks[CAPY_ASTATE_3_Burrow]       = capy_animState3Burrow;
    sAnimCallbacks[CAPY_ASTATE_4_Sniff]        = capy_animState4Sniff;
    sAnimCallbacks[CAPY_ASTATE_5_Eat]          = capy_animState5Eat;
    sAnimCallbacks[CAPY_ASTATE_6_DoneEating]   = capy_animState6DoneEating;
    sAnimCallbacks[CAPY_ASTATE_7_DigWall]      = capy_animState7DigWall;
    sAnimCallbacks[CAPY_ASTATE_8_Walking]      = capy_animState8Walking;
    
    sLogicCallbacks[CAPY_LSTATE_0_Init]        = capy_logicState0Init;
    sLogicCallbacks[CAPY_LSTATE_1_DigWall]     = capy_logicState1DigWall;
    sLogicCallbacks[CAPY_LSTATE_2_GoToDigSpot] = capy_logicState2GoToDigSpot;
    sLogicCallbacks[CAPY_LSTATE_3_Eating]      = capy_logicState3Eating;
    sLogicCallbacks[CAPY_LSTATE_4_Idle]        = capy_logicState4Idle;
    sLogicCallbacks[CAPY_LSTATE_5_Underground] = capy_logicState5Underground;
}

// offset: 0xD8 | ctor
void capy_ctor(void* dll) {
    capy_setupFSACallbacks();
}

// offset: 0x118 | dtor
void capy_dtor(void* dll) { }

// offset: 0x124 | func: 1 | export: 0
void capy_obj_Setup(Object* self, Baddie_Setup* setup, s32 reset) {
    Baddie* baddie = self->data;
    u8 flags;
    
    flags = 0x10 | 4 | 2;
    if (reset) {
        flags = 0x10 | 4 | 2 | 1;
    }
    if (!(setup->unk2B & 1)) {
        flags |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, setup, baddie, 9, 6, 0x102, flags, 20.0f);

    self->animCallback = capy_animCallback;

    baddie->fsa.animState = CAPY_ASTATE_1_Burrowed;
    baddie->fsa.logicState = CAPY_LSTATE_0_Init;
    baddie->fsa.unk33A = FALSE;
}

// offset: 0x1DC | func: 2 | export: 1
void capy_obj_Control(Object* self) {
    Baddie* baddie = self->data;
    
    gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, 1.0f, 1.0f, sAnimCallbacks, sLogicCallbacks);
    
    capy_findFoodTarget(self, baddie, &baddie->fsa);

    if ((baddie->fsa.target != NULL) || (baddie->fsa.hitpoints == 0)) { 
        capy_handlePlayerTarget(self, 0, baddie, &baddie->fsa);
    } else {
        capy_searchForTarget(self, baddie, &baddie->fsa);
    }
}

// offset: 0x2D4 | func: 3 | export: 2
void capy_obj_Update(Object* self) {
    gDLL_18_objfsa->vtbl->func2(self, self->data, sAnimCallbacks);
}

// offset: 0x324 | func: 4 | export: 3
void capy_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility && (self->unkDC == 0)) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x384 | func: 5 | export: 4
void capy_obj_Free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;

    objFreeObjectType(self, OBJTYPE_Baddie);

    if (self->linkedObject != NULL) {
        objFreeObject(self->linkedObject);
        self->linkedObject = NULL;
    }

    gDLL_33_BaddieControl->vtbl->free(self, baddie, 1);
}

// offset: 0x428 | func: 6 | export: 5
u32 capy_obj_GetModelFlags(Object* self) {
    return MODFLAGS_100 | MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0x438 | func: 7 | export: 6
u32 capy_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(Baddie) + sizeof(capy_data);
}

// offset: 0x44C | func: 8
static int capy_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    return 0; 
}

// offset: 0x468 | func: 9
static void capy_findFoodTarget(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    Vec3f dTarget;
    Object* player;
    Object* dinoEgg;
    Object* foodbag; 

    dinoEgg = NULL;

    if (self->linkedObject != NULL) {
        self->linkedObject->parent = self->parent;
    }

    if (fsa->target != NULL) {
        dTarget.x = fsa->target->globalPosition.x - self->globalPosition.x;
        dTarget.y = fsa->target->globalPosition.y - self->globalPosition.y;
        dTarget.z = fsa->target->globalPosition.z - self->globalPosition.z;
        fsa->targetDist = sqrtf(SQ(dTarget.f[0]) + SQ(dTarget.f[1]) + SQ(dTarget.f[2]));
    }

    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, CAPY_LSTATE_0_Init, CAPY_LSTATE_0_Init, 0);

    player = objGetPlayer();
    if ((player == fsa->target) || ((fsa->target != NULL) && (fsa->target->id == OBJ_foodbagNewMeat))) {
        foodbag = dll_player(player)->func66(player, 15);
        if (foodbag != NULL) {
            dinoEgg = ((DLL_IFoodbag*)foodbag->dll)->vtbl->get_nearest_placed_food_of_type(foodbag, self, FOOD_Dino_Egg);
        }
        if ((dinoEgg != NULL) && (vec3Distance(&self->globalPosition, &dinoEgg->globalPosition) < 150.0f)) {
            fsa->target = dinoEgg;
        } else {
            fsa->target = NULL;
        }
    }
}

// offset: 0x644 | func: 10
static void capy_handlePlayerTarget(Object* self, s32 arg1, Baddie* baddie, ObjFSA_Data* fsa) {
    Object* player;

    player = objGetPlayer();
    self->objhitInfo->unk58 |= 1;

    if (player != fsa->target) {
        return;
    }

    //Check if the player should be disengaged as a target
    if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2, 1)) {
        fsa->target = NULL;
    }
}

// offset: 0x704 | func: 11
static void capy_searchForTarget(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    Object* target;

    self->objhitInfo->unk58 &= ~1;
    
    target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, M_180_DEGREES);
    if (target != NULL) {
        fsa->target = target;
        fsa->unk33D = 0;
    }
}

// offset: 0x7A0 | func: 12
s32 capy_animState0Standing(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 count;
    Object** magicPlants;
    s32 i;
    s32 magicPlantNearby;
    Baddie* baddie;
    capy_data* objData;

    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        objAnimSet(self, CAPY_MODANIM_3_Standing, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    fsa->animTickDelta = 0.03f;

    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 5);

    if (fsa->enteredAnimState) {
        objData->timer = 0;
    }

    objData->timer++; //@framerate-dependent

    if ((objGetPlayer() == fsa->target) && (objData->timer > 60)) {
        if (objData->ateMagicPlant) {
            fsa->enteredAnimState = TRUE;
            fsa->unk33A = FALSE;
            fsa->logicState = CAPY_LSTATE_5_Underground;
            return FSA_NEXTSTATE_SYNC(CAPY_ASTATE_3_Burrow);
        }

        magicPlants = objGetAllOfType(OBJTYPE_MagicPlant, &count);
        magicPlantNearby = FALSE;
        for (i = 0; i < count; i++) {
            if (!magicPlantNearby && vec3Distance(&self->globalPosition, &magicPlants[i]->globalPosition) < 300.0f) {
                magicPlantNearby = TRUE;
                fsa->target = magicPlants[i];
            }
        }
    }

    return 0;
}

// offset: 0x97C | func: 13
s32 capy_animState1Burrowed(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    capy_data* objData;

    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        objAnimSet(self, CAPY_MODANIM_1_Unburrow, 0.0f, 0);
        fsa->unk33A = FALSE;
    }

    if (objData->timer != 0) {
        objData->timer--; //@framerate-dependent
    }

    fsa->animTickDelta = 0.0f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;

    return 0;
}

// offset: 0xA0C | func: 14
s32 capy_animState2Unburrow(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, CAPY_MODANIM_1_Unburrow, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    fsa->animTickDelta = 0.04f;

    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;

    if (fsa->unk33A) {
        fsa->logicState = CAPY_LSTATE_4_Idle;
    }

    return 0;
}


// offset: 0xA9C | func: 15
s32 capy_animState3Burrow(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    capy_data* objData;
    CurveSetup* curveSetup;
    s32 curveUID;
    s32 curveType;

    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        objAnimSet(self, CAPY_MODANIM_0_Burrow, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    fsa->animTickDelta = 0.04f;

    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;

    if (fsa->unk33A) {
        curveType = 0x25;
        curveUID = gDLL_26_Curves->vtbl->func_1E4(self->srt.transl.f[0], self->srt.transl.f[1], self->srt.transl.f[2], &curveType, 1, 0);
        if (curveUID != -1) {
            curveSetup = gDLL_26_Curves->vtbl->func_39C(curveUID);
            self->srt.transl.x = curveSetup->pos.x;
            self->srt.transl.z = curveSetup->pos.z;
        }
        objData->timer = 80;

        return FSA_NEXTSTATE_SYNC(CAPY_ASTATE_1_Burrowed);
    }

    return 0;
}

// offset: 0xBC4 | func: 16
s32 capy_animState4Sniff(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    capy_data* objData;

    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        objAnimSet(self, CAPY_MODANIM_5_Sniffing, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    fsa->animTickDelta = 0.04f;

    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;

    if (fsa->unk308 & 1) {
        fsa->unk308 &= ~1;
        objData->soundHandle = dll_amSfx->Play(self, SOUND_77C_Capy_Sniff, MAX_VOLUME, NULL, NULL, 0, NULL);
        dll_amSfx->SetPitch(objData->soundHandle, (mathRnd(-10, 10) / 100.0f) + 1.0f);
    }

    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(CAPY_ASTATE_5_Eat);
    } else {
        return 0;
    }
}

// offset: 0xD1C | func: 17
s32 capy_animState5Eat(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Object* target;
    Object* player;
    Object* foodbag;
    Baddie* baddie;
    capy_data* objData;
    
    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        objAnimSet(self, CAPY_MODANIM_6_Eating, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    fsa->animTickDelta = 0.1f;

    target = fsa->target;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;

    if (target != NULL) {
        if (fsa->enteredAnimState) {
            func_8002635C(target, NULL, 7, 1, 0);
        }
        target->srt.scale *= 0.97f;
    }

    if (fsa->unk308 & 1) {
        fsa->unk308 &= ~1;
        objData->soundHandle = dll_amSfx->Play(self, SOUND_77D_Capy_Eat, MAX_VOLUME, NULL, NULL, 0, NULL);
        dll_amSfx->SetPitch(objData->soundHandle, (mathRnd(-10, 10) / 100.0f) + 1.0f);
    }
    if (fsa->unk33A) {
        if (target != NULL) {
            if (target->srt.scale > 0.01f) {
                return 0;
            }

            if (target->id == OBJ_MagicPlant) {
                objData->ateMagicPlant = TRUE;
                objFreeObject(target);
            } else {
                objData->fed = TRUE;
                player = objGetPlayer();
                foodbag = dll_player(player)->func66(player, 15);
                if (foodbag != NULL) {
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->destroy_placed_food(foodbag, target);
                }
            }

            fsa->target = NULL;
        }
        return FSA_NEXTSTATE_SYNC(CAPY_ASTATE_6_DoneEating);
    }
    return 0;
}

// offset: 0xF8C | func: 18
s32 capy_animState6DoneEating(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    baddie = self->data;

    if (fsa->enteredAnimState) {
        objAnimSet(self, CAPY_MODANIM_7_DoneEating, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    fsa->animTickDelta = 0.08f;

    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;

    if (fsa->unk33A) {
        if ((gDLL_26_Curves->vtbl->func_4288(baddie->unk3F8, self, 150.0f, dCurveTypes, -1) == 0) 
                && (mainGetBits(sTunnelGamebits[baddie->unk3F8->unk9C->unk18]) == FALSE)) {
            fsa->logicState = CAPY_LSTATE_2_GoToDigSpot;
            return FSA_NEXTSTATE_SYNC(CAPY_ASTATE_8_Walking);
        } else {
            fsa->logicState = CAPY_LSTATE_4_Idle;
            return FSA_NEXTSTATE_SYNC(CAPY_ASTATE_0_Standing);
        }
    }

    return 0;
}

// offset: 0x10BC | func: 19
s32 capy_animState7DigWall(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    capy_data* objData;

    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        objAnimSet(self, CAPY_MODANIM_2_DigWall, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    fsa->animTickDelta = 0.015f;

    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;

    if (fsa->unk308 & 1) {
        fsa->unk308 &= ~1;
        objData->soundHandle = dll_amSfx->Play(self, SOUND_77D_Capy_Eat, MAX_VOLUME, NULL, NULL, 0, NULL);
        dll_amSfx->SetPitch(objData->soundHandle, (mathRnd(-10, 10) / 100.0f) + 1.0f);
    }

    if (fsa->unk308 & 0x200) {
        mainSetBits(sTunnelGamebits[baddie->unk3F8->unk9C->unk18], 1);
    }

    if (fsa->unk33A) {
        fsa->logicState = CAPY_LSTATE_4_Idle;
        return FSA_NEXTSTATE_SYNC(CAPY_ASTATE_0_Standing);
    }

    return 0;
}

// offset: 0x1264 | func: 20
s32 capy_animState8Walking(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    capy_data* objData;
    f32 dz;
    f32 dx;
    
    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        objAnimSet(self, CAPY_MODANIM_4_Walking, 0.0f, 0);
        fsa->unk33A = FALSE;
    }

    if (fsa->enteredAnimState) {
        objData->timer = 0;
        objData->prevX = self->srt.transl.x;
        objData->prevZ = self->srt.transl.z;
    }

    //Get current speed from position delta (@framerate-dependent)
    dx = self->srt.transl.x - objData->prevX;
    dz = self->srt.transl.z - objData->prevZ;
    objData->prevX = self->srt.transl.x;
    objData->prevZ = self->srt.transl.z;

    if (((SQ(dx) + SQ(dz)) < 0.1f) && (fsa->logicState != CAPY_LSTATE_2_GoToDigSpot)) {
        objData->timer++; //@framerate-dependent
    } else {
        objData->timer = 0;
    }

    if (objData->timer >= 13) {
        fsa->enteredAnimState = TRUE;
        fsa->unk33A = FALSE;
        fsa->logicState = CAPY_LSTATE_5_Underground;
        return FSA_NEXTSTATE_SYNC(CAPY_ASTATE_3_Burrow);
    }

    fsa->animTickDelta = 0.08f;
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    gDLL_33_BaddieControl->vtbl->func3(self, fsa, baddie, 2.0f, 12.0f);

    return 0;
}

// offset: 0x1408 | func: 21
s32 capy_logicState0Init(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return FSA_NEXTSTATE_SYNC(CAPY_LSTATE_5_Underground);
}

// offset: 0x1420 | func: 22
s32 capy_logicState1DigWall(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
}

// offset: 0x1438 | func: 23
s32 capy_logicState2GoToDigSpot(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 _pad[2];
    capy_data* objData;
    UnkCurvesStruct* baddieCurves;
    Baddie* baddie;
    s32 startDig;
    f32 dYaw;
    f32 dYawAbs;
    s32 curveAngle;

    baddie = self->data;
    baddieCurves = baddie->unk3F8;
    objData = baddie->objdata;

    startDig = curves_func_800053B0(&baddieCurves->unk0, 5.0f / (SQ(baddieCurves->unk0.unk68.f[2] - self->srt.transl.f[2]) + SQ(baddieCurves->unk0.unk68.f[0] - self->srt.transl.f[0])));
    if ((startDig || baddieCurves->unk0.unk10) && gDLL_26_Curves->vtbl->func_4704(baddieCurves)) {
        fsa->animState = CAPY_ASTATE_7_DigWall;
        fsa->enteredAnimState = FALSE;
        fsa->unk33A = FALSE;
        fsa->logicState = CAPY_LSTATE_1_DigWall;

        if (self){} // @fake

        return 0;
    } else {
        //Get angle from curve tangent
        curveAngle = (mathAtan2f(baddieCurves->unk0.unk74, baddieCurves->unk0.unk7C) & 0xFFFF);

        //Get yaw diff (choose shortest angular path)
        dYaw = (((curveAngle - ((u16)self->srt.yaw & 0xFFFF))) + M_180_DEGREES);
        if (dYaw > M_180_DEGREES) {
            dYaw = -(M_360_DEGREES - 1) + dYaw;
        }
        if (dYaw < -M_180_DEGREES) {
            dYaw = (M_360_DEGREES - 1) + dYaw;
        }

        //Get magnitude of yaw diff
        if (dYaw < 0.0f) {
            dYawAbs = -dYaw;
        } else {
            dYawAbs = dYaw;
        }

        //Set turn speed based on yaw diff
        fsa->unk278 = 1.0f - (dYawAbs / (M_180_DEGREES - 1.0f));
        if (fsa->unk278 < 0.01f) {
            fsa->unk278 = 0.01f;
        }
        fsa->unk278 *= objData->turnSpeedFactor / 100.0f;

        //Use turn speed as movement speed too
        fsa->speed = fsa->unk278;

        gDLL_18_objfsa->vtbl->func6(self, fsa, baddieCurves->unk0.unk68.f[0], baddieCurves->unk0.unk68.f[2], 0, 0, 60.0f);
        fsa->animState = CAPY_ASTATE_8_Walking;

        return 0;
    }
}

// offset: 0x1698 | func: 24
s32 capy_logicState3Eating(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if ((fsa->animState != CAPY_ASTATE_6_DoneEating) && (fsa->target == NULL || objGetPlayer() == fsa->target)) {
        fsa->animState = CAPY_ASTATE_3_Burrow;
        fsa->enteredAnimState = TRUE;
        fsa->unk33A = FALSE;
        return FSA_NEXTSTATE_SYNC(CAPY_LSTATE_5_Underground);
    }

    return 0;
}

// offset: 0x1728 | func: 25
s32 capy_logicState4Idle(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    Unk80009024* voxData;
    capy_data* objData;
    u8 isTargetPlayer;

    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->target != NULL) {
        isTargetPlayer = objGetPlayer() == fsa->target;
        fsa->animState = CAPY_ASTATE_8_Walking;
        fsa->xAnalogInput = 0.0f;
        fsa->yAnalogInput = 0.0f;
        voxData = &baddie->unk34C;
        bcopy(&self->srt.transl, &voxData->unk0, sizeof(Vec3f));
        bcopy(&fsa->target->srt.transl, &baddie->unk34C.unkC, sizeof(Vec3f));
        vox_func_80009024(voxData, &baddie->unk374);

        if (isTargetPlayer) {
            if ((fsa->targetDist < 50.0f) || !objData->fed) {
                fsa->animState = CAPY_ASTATE_0_Standing;
            }
        } else { 
            if ((fsa->targetDist < 20.0f) || ((objData->timer >= 11) && (fsa->targetDist < 30.0f))) {
                fsa->animState = CAPY_ASTATE_4_Sniff;
                fsa->target->unkAF |= ARROW_FLAG_8_No_Targetting;
                return FSA_NEXTSTATE_SYNC(CAPY_LSTATE_3_Eating);
            }
        }

        if (voxData->unk25 == 0) {
            gDLL_18_objfsa->vtbl->func6(self, fsa, voxData->unk18.x, voxData->unk18.z, 0.0f, 0.0f, 60.0f);
        } else if (isTargetPlayer) {
            gDLL_18_objfsa->vtbl->func6(self, fsa, voxData->unk18.x, voxData->unk18.z, 20.0f, 30.0f, 60.0f);
        } else {
            gDLL_18_objfsa->vtbl->func6(self, fsa, voxData->unk18.x, voxData->unk18.z, 10.0f, 10.0f, 60.0f);
        }
    } else {
        fsa->animState = CAPY_ASTATE_3_Burrow;
        fsa->enteredAnimState = TRUE;
        fsa->unk33A = FALSE;
        return FSA_NEXTSTATE_SYNC(CAPY_LSTATE_5_Underground);
    }
    
    return 0;
}

// offset: 0x19DC | func: 26
s32 capy_logicState5Underground(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    capy_data* objData = baddie->objdata;
    
    if ((fsa->target != NULL) && (fsa->animState == CAPY_ASTATE_1_Burrowed) && (objData->timer == 0)) {
        gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, 3.0f, 1);
        fsa->animState = CAPY_ASTATE_2_Unburrow;
        fsa->enteredAnimState = TRUE;
        fsa->unk33A = FALSE;
    }

    return 0;
}
