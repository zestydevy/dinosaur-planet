#include "PR/os.h"
#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/27.h"
#include "dlls/engine/3_animation.h"
#include "dlls/engine/53_movelib.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/496_SnowHorn.h"
#include "dlls/objects/common/vehicle.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/gfx/textable.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "sys/objlib.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "types.h"
#include "prevent_bss_reordering.h"

typedef enum {
    DIMSnowHorn_IDX_0_Shackled,         //The SnowHorn shackled beside one of the snow houses
    DIMSnowHorn_IDX_1_Famished,         //The fallen SnowHorn who needs to be saved from a SharpClaw and fed Alpine Roots
    DIMSnowHorn_IDX_2_Leap,             //The SnowHorn in the Leap of Faith cave
    DIMSnowHorn_IDX_3_Blizzard          //The SnowHorn summoned by the Horn of Truth, whom Sabre can ride through the blizzard
} DIMSnowHorn_CharacterIndices;

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 characterIdx;
} DIMSnowHorn_Setup;

typedef struct {
    ObjFSA_Data fsa;
    MoveLibData moveData;
    HeadAnimation headAnim;
    Vec3f particleAttachCoords[4];
    s32 unk858;
    s32 unk85C;
    Vec3f riderPosition;
    s8 unk86C[0x8FC - 0x86C];
    s16 fidgetTimer;
    s16 minTurn;
    s16 energy;
    u8 mountState;
    u8 unk903;
    u8 characterIdx;
    u8 chatSequenceIdx;
    u8 flags;
    u8 dismountToRight;
    u8 mountFromLeft;
} DIMSnowHorn_Data;

typedef enum {
    DIMSnowHorn_ASTATE_0_Init,                      //Switches state based on the character
    DIMSnowHorn_ASTATE_1_Shackled_Idle,             //Shackled SnowHorn standing around
    DIMSnowHorn_ASTATE_2_Shackled_Fidget,           //Shackled SnowHorn scratching back/shaking off snow
    DIMSnowHorn_ASTATE_3_Leap_Idle,                 //Leap of Faith cave SnowHorn standing around
    DIMSnowHorn_ASTATE_4_Leap_Fidget,               //Leap of Faith cave SnowHorn scratching back/shaking off snow
    DIMSnowHorn_ASTATE_5_Famished_Fallen,           //Famished SnowHorn being attacked by a SharpClaw
    DIMSnowHorn_ASTATE_6_Famished_Met,              //Famished SnowHorn saved from the SharpClaw, but still needs feeding
    DIMSnowHorn_ASTATE_7_Famished_Fed_Once,     //Fanished SnowHorn fed one Alpine Root, and waiting for a second one
    DIMSnowHorn_ASTATE_8_Vehicle_Idle,              //SnowHorn standing around, waiting for the player
    DIMSnowHorn_ASTATE_9_Vehicle_Sit,               //SnowHorn sitting down and getting back up again after an input (unused?)
    DIMSnowHorn_ASTATE_10_Vehicle_Turn_on_Spot,     //SnowHorn turning (without walking forward)
    DIMSnowHorn_ASTATE_11_Vehicle_Walking,          //SnowHorn walking (can turn slightly)
    DIMSnowHorn_ASTATE_12_Vehicle_Tusk_Attack       //SnowHorn attacking with tusks
} DIMSnowHorn_AnimStates;

typedef enum {
    DIMSnowHorn_LSTATE_0_Top
} DIMSnowHorn_LogicStates;

typedef enum {
    DIMSnowHorn_FLAG_1 = 1,
    DIMSnowHorn_FLAG_2 = 2,
    DIMSnowHorn_FLAG_4 = 4,
    DIMSnowHorn_FLAG_8 = 8,
    DIMSnowHorn_FLAG_10 = 0x10,
    DIMSnowHorn_FLAG_20_Already_Freed = 0x20
} DIMSnowHorn_Flags;

/*0x0*/ static s16 dEnergyBarTexIDs[] = { TEXTABLE_1C8_CMDMENU_Early_Energy_Bar };
/*0x4*/ static Vec3f dTerrainTestPoints[] = {
    VEC3F(-12, 0, -20),
    VEC3F(12, 0, -20),
    VEC3F(12, 0, 20),
    VEC3F(-12, 0, 20)
};
/*0x34*/ static f32 dTerrainRadii[] = {
    0, 0, 0, 0
};
/*0x44*/ static Vec3f dHitsTestPoints[] = {
    VEC3F(0, 0, 35), 
    VEC3F(0, 0, -35)
};
/*0x5C*/ static f32 dHitsTestRadii[] = {
    25, 25
};
/*0x64*/ static u16 _data_64[] = { //Unused snowy footstep sounds?
    SOUND_37B_Snowy_Crunch, 
    SOUND_37B_Snowy_Crunch
};

/*0x0*/ static ObjFSA_StateCallback sAnimStateCallbacks[13];
/*0x34*/ static ObjFSA_StateCallback sLogicStateCallbacks[1];
/*0x38*/ static Texture* sEnergyBarTextures[2];
/*0x40*/ static MtxF sRiderMtx;

static void DIMSnowHorn_updateShadow(Object* self);
static void DIMSnowHorn_tickVehicle(Object* self, s32 updateRate, s32 iterationNumber);
static void DIMSnowHorn_gfxSetTexture(Gfx** gdl, Texture* tex, s32 frame);
static void DIMSnowHorn_createAnimParticles(Object* self, DIMSnowHorn_Data* objData, ObjFSA_Data* fsa);
static int DIMSnowHorn_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);
static int DIMSnowHorn_animCallbackVehicle(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);
static void DIMSnowHorn_lookAtPlayerWhenNearby(Object* self);

static s32 DIMSnowHorn_animState0Init(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 DIMSnowHorn_animState1ShackledIdle(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 DIMSnowHorn_animState2ShackledFidget(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 DIMSnowHorn_animState3LeapIdle(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 DIMSnowHorn_animState4LeapFidget(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 DIMSnowHorn_animState5FamishedFallen(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 DIMSnowHorn_animState6FamishedMet(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 DIMSnowHorn_animState7FanishedFedOnce(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 DIMSnowHorn_animState8VehicleIdle(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 DIMSnowHorn_animState9VehicleSit(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 DIMSnowHorn_animState10VehicleTurn(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 DIMSnowHorn_animState11VehicleWalk(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 DIMSnowHorn_animState12VehicleAttack(Object* obj, ObjFSA_Data* fsa, f32 updateRate);

static s32 DIMSnowHorn_logicState0(Object* obj, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void DIMSnowHorn_initFSACallbacks(void) {
    //Anim states
    {
        sAnimStateCallbacks[DIMSnowHorn_ASTATE_0_Init] = DIMSnowHorn_animState0Init;

        //Shackled SnowHorn
        sAnimStateCallbacks[DIMSnowHorn_ASTATE_1_Shackled_Idle]   = DIMSnowHorn_animState1ShackledIdle;
        sAnimStateCallbacks[DIMSnowHorn_ASTATE_2_Shackled_Fidget] = DIMSnowHorn_animState2ShackledFidget;

        //Leap of Faith cave SnowHorn
        sAnimStateCallbacks[DIMSnowHorn_ASTATE_3_Leap_Idle]   = DIMSnowHorn_animState3LeapIdle;
        sAnimStateCallbacks[DIMSnowHorn_ASTATE_4_Leap_Fidget] = DIMSnowHorn_animState4LeapFidget;

        //Famished SnowHorn
        sAnimStateCallbacks[DIMSnowHorn_ASTATE_5_Famished_Fallen]   = DIMSnowHorn_animState5FamishedFallen;
        sAnimStateCallbacks[DIMSnowHorn_ASTATE_6_Famished_Met]      = DIMSnowHorn_animState6FamishedMet;
        sAnimStateCallbacks[DIMSnowHorn_ASTATE_7_Famished_Fed_Once] = DIMSnowHorn_animState7FanishedFedOnce;

        //Vehicle
        sAnimStateCallbacks[DIMSnowHorn_ASTATE_8_Vehicle_Idle]          = DIMSnowHorn_animState8VehicleIdle;
        sAnimStateCallbacks[DIMSnowHorn_ASTATE_9_Vehicle_Sit]           = DIMSnowHorn_animState9VehicleSit;
        sAnimStateCallbacks[DIMSnowHorn_ASTATE_10_Vehicle_Turn_on_Spot] = DIMSnowHorn_animState10VehicleTurn;
        sAnimStateCallbacks[DIMSnowHorn_ASTATE_11_Vehicle_Walking]      = DIMSnowHorn_animState11VehicleWalk;
        sAnimStateCallbacks[DIMSnowHorn_ASTATE_12_Vehicle_Tusk_Attack]  = DIMSnowHorn_animState12VehicleAttack;
    }
    
    sLogicStateCallbacks[DIMSnowHorn_LSTATE_0_Top] = DIMSnowHorn_logicState0;
}

// offset: 0xC8 | ctor
void DIMSnowHorn_ctor(void* dll) {
    u32 i;
    
    DIMSnowHorn_initFSACallbacks();

    for (i = 0; i < ARRAYCOUNT(dEnergyBarTexIDs); i++) {
        sEnergyBarTextures[i] = texLoadTextureActual(dEnergyBarTexIDs[i], 0);
    }
}

// offset: 0x15C | dtor
void DIMSnowHorn_dtor(void* dll) {
    u32 i;

    for (i = 0; i < ARRAYCOUNT(dEnergyBarTexIDs); i++) {
        if (sEnergyBarTextures[i]) {
            texFreeTexture(sEnergyBarTextures[i]);
        }
    }
}

// offset: 0x1CC | func: 1 | export: 0
void DIMSnowHorn_obj_Setup(Object* self, DIMSnowHorn_Setup* objSetup, s32 reset) {
    DIMSnowHorn_Data* objData;
    DLL27_Data* collider;
    s32 pad;
/*0x68*/ u8 dTerrainColliderArgs[] = { 1, 1, 1, 1};
    
    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = DIMSnowHorn_animCallback;
    
    objAddObjectType(self, OBJTYPE_Vehicle);
    
    objData = self->data;
    objData->characterIdx = objSetup->characterIdx;
    objData->minTurn = 5;
    objData->energy = 1000;

    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_DIR;
        self->shadow->maxDistScale = self->shadow->scale * 0.3f;
    }

    if (self->objhitInfo != NULL) {
        self->objhitInfo->unkA1 = 9;
    }
    
    gDLL_18_objfsa->vtbl->func0(self, &objData->fsa, 13, 1);
    objData->fsa.unk29C = 0.17f;
    objData->fsa.unk4.mode = 0;
    
    if (objData->characterIdx == DIMSnowHorn_IDX_3_Blizzard) {
        objData->flags |= DIMSnowHorn_FLAG_10;
    }
    
    if ((objData->characterIdx != DIMSnowHorn_IDX_0_Shackled) && 
        (objData->characterIdx != DIMSnowHorn_IDX_2_Leap)
    ) {
        collider = &objData->fsa.unk4;
        gDLL_27->vtbl->init(collider, DLL27FLAG_4000000 | DLL27FLAG_2000000, DLL27FLAG_200000 | DLL27FLAG_20, DLL27MODE_1);
        gDLL_27->vtbl->setup_hits_collider(collider, ARRAYCOUNT(dHitsTestPoints), dHitsTestPoints, dHitsTestRadii, 8);
        gDLL_27->vtbl->setup_terrain_collider(collider, ARRAYCOUNT(dTerrainTestPoints), dTerrainTestPoints, dTerrainRadii, dTerrainColliderArgs);
        gDLL_27->vtbl->reset(self, collider);
    }
    
    mainCreateTempDLL(DLL_ID_53_MOVELIB);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func2(self, &objData->moveData, -0x1FFF, 0x2AAA, 3);
    
    objData->moveData.unk4A9 |= 8;
}

// offset: 0x43C | func: 2 | export: 1
void DIMSnowHorn_obj_Control(Object* self) {
    DIMSnowHorn_Data* objData;
    s32 i;
    ObjFSA_Data* fsa;
    f32 distance;
    Object* player;

    player = objGetPlayer();
    fsa = self->data;
    objData = self->data;

    objData->minTurn = 5;

    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    
    //Handle rideable SnowHorn
    if (objData->mountState == VEHICLE_Mounted) {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        fsa->unk4.mode = 1;

        for (i = 0; i < gUpdateRate; i++) {
            DIMSnowHorn_tickVehicle(self, 1, i);
        }
    } else {
        fsa->unk4.mode = 0;
        gDLL_27->vtbl->reset(self, &fsa->unk4);
        DIMSnowHorn_tickVehicle(self, gUpdateRate, -1);
    }
    
    if (objData->mountState == VEHICLE_NoRider) {
        gDLL_9_Newclouds->vtbl->func7.withOneArg(0);
    } else {
        gDLL_9_Newclouds->vtbl->func7.withOneArg(1);
    }
    
    //Handle head look-at/head animation
    if ((objData->characterIdx != DIMSnowHorn_IDX_0_Shackled) && 
        (objData->characterIdx != DIMSnowHorn_IDX_2_Leap)
    ) {
        //Don't animate rideable SnowHorn
    } else {
        //Animate non-vehicle SnowHorn
        DIMSnowHorn_lookAtPlayerWhenNearby(self);
        objExpr_func_800328F0(self, &objData->headAnim, 0.0f);
    }
    
    //Handle mounting/dismounting rideable SnowHorn
    if ((objData->characterIdx == DIMSnowHorn_IDX_3_Blizzard) || 
        (objData->characterIdx == DIMSnowHorn_IDX_1_Famished)
    ) {
        if ((objData->mountState == VEHICLE_NoRider) && (fsa->animState == DIMSnowHorn_ASTATE_8_Vehicle_Idle)) {
            if ((((DLL_210_Player*)player->dll)->vtbl->func70(player) == 30) && (joyGetPressed(0) & A_BUTTON)) {
                joyDisableButtons(0, A_BUTTON);
                mainSetBits(BIT_3E3, 1);
                mainSetBits(BIT_5BA, 1);
                objData->energy = 1000;
            }
        } else if ((objData->mountState == VEHICLE_Mounted) && (joyGetPressed(0) & A_BUTTON)) {
            distance = 100.0f;
            if (objGetNearestTypeTo(OBJTYPE_DismountPoint, self, &distance) != NULL) {
                joyDisableButtons(0, A_BUTTON);
                mainSetBits(BIT_3E3, 0);
                mainSetBits(BIT_5BB, 1);
            }
        }
    }
    
    //Update eyes/eyelids, and shadow
    objExprEyeIdle(self, &objData->headAnim);
    DIMSnowHorn_updateShadow(self);
}

// offset: 0x7C0 | func: 3
void DIMSnowHorn_updateShadow(Object* self) {
    SRT xform;
    MtxF mtx;

    xform.transl.x = self->srt.transl.x;
    xform.transl.y = self->srt.transl.y;
    xform.transl.z = self->srt.transl.z;
    xform.yaw = self->srt.yaw;
    xform.pitch = self->srt.pitch;
    xform.roll = self->srt.roll;
    xform.scale = 1.0f;
    mathYprXyzMtx(&mtx, &xform);

    mathMtxXFMF(&mtx, 0.0f, -30.0f, -20.0f, 
        &self->shadow->tr.x, &self->shadow->tr.y, &self->shadow->tr.z);
}

// offset: 0x87C | func: 4
void DIMSnowHorn_tickVehicle(Object* self, s32 updateRate, s32 iterationNumber) {
    DIMSnowHorn_Data* objData;
    ObjFSA_Data* fsa;
    Camera* cam;
    s32 isLastIteration;

    //This function can be called 60 times per second (repeated gUpdateRate times per frame drawn)
    //`iterationNumber` tracks how many times the function has been called this frame.
    if (iterationNumber != -1) { //Iterating gUpdateRate times
        isLastIteration = (iterationNumber + 1) == gUpdateRate;
    } else {                     //Called once per frame drawn
        isLastIteration = TRUE;
    }
    
    cam = camGetMain();    
    
    objData = self->data;
    
    fsa = &objData->fsa;
    fsa->hitpoints = 0;
    fsa->flags &= ~OBJFSA_FLAG_8000;
    
    if (objData->mountState == VEHICLE_Mounted) {
        //Lose energy while riding through the blizzard
        //(@bug: doesn't check which SnowHorn this is, so it can affect the Famished SnowHorn if the gamebit is somehow set!)
        if (mainGetBits(BIT_DIM_Depleting_Blizzard_SnowHorn_Energy)) {
            objData->energy -= updateRate;
        }
        
        //Refill the blizzard SnowHorn's energy on eating one of the plains' scattered Alpine Roots
        if (mainGetBits(BIT_DIM_Blizzard_Alpine_Root_Eaten)) {
            mainSetBits(BIT_DIM_Blizzard_Alpine_Root_Eaten, FALSE);
            objData->energy = 1000;
        }
        
        //Handle when the SnowHorn's energy runs out
        if (objData->energy < 0) {
            objData->energy = 0;
        }
        
        //Handle controls
        fsa->xAnalogInput = joyGetStickXBuffered(0, iterationNumber);
        fsa->yAnalogInput = joyGetStickYBuffered(0, iterationNumber);
        fsa->unk310 = joyGetPressedBuffered(0, iterationNumber);
        fsa->unk30C = joyGetButtonsBuffered(0, iterationNumber);
        fsa->unk324 = cam->srt.yaw;
    } else {
        fsa->unk310 = 0;
        fsa->unk30C = 0;
        fsa->unk324 = 0;
        fsa->xAnalogInput = 0.0f;
        fsa->yAnalogInput = 0.0f;
    }

    fsa->flags |= OBJFSA_FLAG_400000;
    if (isLastIteration) {
        fsa->flags &= ~OBJFSA_FLAG_400000;
    }
    
    gDLL_18_objfsa->vtbl->tick(self, fsa, updateRate, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    DIMSnowHorn_createAnimParticles(self, objData, fsa);
}

// offset: 0xAD4 | func: 5 | export: 2
void DIMSnowHorn_obj_Update(Object *self) { }

// offset: 0xAE0 | func: 6 | export: 3
void DIMSnowHorn_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DIMSnowHorn_Data *objData;
    s32 pad1;
    Gfx *dl;
    s32 pad2;
    
    objData = self->data;
    
    if ((objData->flags & DIMSnowHorn_FLAG_10)) {
        return;
    }
    
    //Draw an energy bar while riding the Blizzard SnowHorn
    if ((objData->characterIdx == DIMSnowHorn_IDX_3_Blizzard) && (objData->mountState == VEHICLE_Mounted)) {            
        dl = *gdl;
            
        gDPSetCombineMode(dl, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        dlApplyCombine(&dl);
        gDPSetOtherMode(dl, G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | 
            G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2);
        dlApplyOtherMode(&dl);
        gSPLoadGeometryMode(dl, G_SHADE | G_SHADING_SMOOTH);
        dlApplyGeometryMode(&dl);
        dlSetPrimColor(&dl, 0xFF, 0xFF, 0xFF, 0xFF);
        DIMSnowHorn_gfxSetTexture(&dl, sEnergyBarTextures[0], 0);

        //TODO: figure out what these macros are
        {
            Gfx *_g = (Gfx *) dl;
            dl = _g + 1;
            _g->words.w0 = (((((objData->energy / 5) + 0x50) & 0xFFF) << 0xC) | 0xE4000000) | 0x90;
            _g->words.w1 = 0x00050050;
        }
        
        {
            Gfx *_g = (Gfx *) dl;
            dl = _g + 1;
            _g->words.w0 = 0xE1000000;\
            _g->words.w1 = 0x000001FF;
        }
        
        {
            Gfx *_g = (Gfx *) dl;
            dl = _g + 1; _g->words.w0 = 0xF1000000; _g->words.w1 = 0x0400FC00; 
        } 
        
        gDLBuilder->needsPipeSync = TRUE; 
        
        *gdl = dl; 
    } 
    
    if (visibility == -1) { 
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f); 
        objGetAttachPointWorldSpace(self, 1, &objData->riderPosition.x, &objData->riderPosition.y, &objData->riderPosition.z, 0); 
        objGetAttachPointBoneWorldPositions(self, 2, ARRAYCOUNT(objData->particleAttachCoords), objData->particleAttachCoords); 
    }
   
    if (objData->mountState != VEHICLE_Mounted) {
        if (visibility) {
            objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
            objGetAttachPointWorldSpace(self, 1, &objData->riderPosition.x, &objData->riderPosition.y, &objData->riderPosition.z, 0);
            objGetAttachPointBoneWorldPositions(self, 2, ARRAYCOUNT(objData->particleAttachCoords), objData->particleAttachCoords);
        }
        
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func3(self, &objData->moveData, 0);
    }
}

// offset: 0xE24 | func: 7 | export: 4
void DIMSnowHorn_obj_Free(Object* self, s32 onlySelf) {
    objFreeObjectType(self, OBJTYPE_Vehicle);
}

// offset: 0xE64 | func: 8 | export: 5
u32 DIMSnowHorn_obj_GetModelFlags(Object *self) {
    return MODFLAGS_EVENTS | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0xE74 | func: 9 | export: 6
u32 DIMSnowHorn_obj_GetDataSize(Object *self, u32 offsetAddr) {
    return sizeof(DIMSnowHorn_Data);
}

// offset: 0xE88 | func: 10
void DIMSnowHorn_gfxSetTexture(Gfx** gdl, Texture* tex, s32 frame) {
    Gfx* dl;
    s32 i;

    dl = *gdl;

    //Get the animation frame
    for (i = 0; i < frame; i++) {
        if (tex->next != NULL) {
            tex = tex->next;
        }
    }

    dl->words.w0 = tex->gdl->words.w0;
    dl->words.w1 = (u32) (OS_PHYSICAL_TO_K0(tex + 1));
    dl++;

    gSPDisplayList(dl++, OS_PHYSICAL_TO_K0(tex->gdl + 1));

    texRenderReset();

    *gdl = dl;
}

// offset: 0xFA0 | func: 11
void DIMSnowHorn_createAnimParticles(Object* self, DIMSnowHorn_Data* objData, ObjFSA_Data* fsa) {
    u8 i;
    SRT fxTransform; 
    u8 count;
    u8 bits;
    
    bits = 0;
    if (fsa->unk308 & 2) {
        bits = 1;
    }
    if (fsa->unk308 & 4) {
        bits |= 2;
    }
    if (fsa->unk308 & 8) {
        bits |= 4;
    }
    if (fsa->unk308 & 0x10) {
        bits |= 8;
    }

    i = 0;
    while (bits) {
        if (bits & 1) {
            fxTransform.transl.f[0] = objData->particleAttachCoords[i].f[0];
            fxTransform.transl.f[1] = objData->particleAttachCoords[i].f[1];
            fxTransform.transl.f[2] = objData->particleAttachCoords[i].f[2];
            fxTransform.scale = 0.004f;

            count = mathRnd(2, 6);
            while (count) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1F9 + mathRnd(0, 1), &fxTransform, 0x10001, -1, NULL);
                count--;
            }
        }

        bits >>= 1;
        i++;
    }
}

// offset: 0x1150 | func: 12 | export: 7
s32 DIMSnowHorn_vehicle_CanMount(Object* self, Object* player) {
    ObjFSA_Data* fsa = &((DIMSnowHorn_Data*)self->data)->fsa;
    DIMSnowHorn_Data* objData = self->data;
    
    if ((objData->characterIdx == DIMSnowHorn_IDX_0_Shackled) || 
        (objData->characterIdx == DIMSnowHorn_IDX_2_Leap) || 
        (fsa->animState != DIMSnowHorn_ASTATE_8_Vehicle_Idle)
    ) {
        return 0;
    }
    
    if (self->animObj) {
        return 0; 
    } 
    
    if (((DLL_210_Player*) player->dll)->vtbl->func70(player) == 30) { 
        if (joyGetPressed(0) & A_BUTTON) { 
            joyDisableButtons(0, A_BUTTON); 
            objData->flags |= DIMSnowHorn_FLAG_4;
            return TRUE;
        }
    }
    
    return FALSE;
}

// offset: 0x124C | func: 13 | export: 8
s32 DIMSnowHorn_vehicle_GetMountSide(Object* self) {
    DIMSnowHorn_Data* objData = self->data;
    return objData->mountFromLeft ? VEHICLE_SIDE_Left : VEHICLE_SIDE_Right;
}

// offset: 0x1274 | func: 14 | export: 9
void DIMSnowHorn_vehicle_GetRiderPosition(Object* self, f32* ox, f32* oy, f32* oz) {
    DIMSnowHorn_Data* objData = self->data;
    
    *ox = objData->riderPosition.x;
    *oy = objData->riderPosition.y;
    *oz = objData->riderPosition.z;
}

// offset: 0x1298 | func: 15 | export: 10
s32 DIMSnowHorn_vehicle_CanDismount(Object* self, s32 arg1) {
    DIMSnowHorn_Data* objData = self->data;

    if (objData->flags & DIMSnowHorn_FLAG_2) {
        mainSetBits(BIT_3E3, 0);
        objData->flags &= ~DIMSnowHorn_FLAG_2;
        return TRUE;
    } else {
        return FALSE;
    }
}

// offset: 0x1310 | func: 16 | export: 11
s32 DIMSnowHorn_vehicle_GetDismountSide(Object* self) {
    DIMSnowHorn_Data* objData = self->data;
    return objData->dismountToRight ? VEHICLE_SIDE_Right : VEHICLE_SIDE_Left;
}

// offset: 0x1338 | func: 17 | export: 12
void DIMSnowHorn_vehicle_GetCameraPosition(Object* self, f32* ox, f32* oy, f32* oz) {
    MtxF mtx;
    SRT xform;

    xform.transl.x = self->srt.transl.x;
    xform.transl.y = self->srt.transl.y;
    xform.transl.z = self->srt.transl.z;
    xform.yaw = self->srt.yaw;
    xform.pitch = self->srt.pitch;
    xform.roll = self->srt.roll;
    xform.scale = 1.0f;
    mathYprXyzMtx(&mtx, &xform);
    mathMtxXFMF(&mtx, 0.0f, 80.0f, -25.0f, ox, oy, oz);
}

// offset: 0x13FC | func: 18 | export: 13
s32 DIMSnowHorn_vehicle_GetMountState(Object* self) {
    return VEHICLE_NoRider; //@bug?: doesn't get objData->mountState
}

// offset: 0x140C | func: 19 | export: 14
void DIMSnowHorn_vehicle_SetMountState(Object* self, s32 state) {
    DIMSnowHorn_Data* objData = self->data;
    objData->mountState = state;
}

// offset: 0x141C | func: 20 | export: 15
void DIMSnowHorn_vehicle_GetPlayerAnim(Object* self, f32* animProgress, s32* animIndex) {
    *animProgress = 0.0f;
    *animIndex = 0;
}

// offset: 0x1444 | func: 21 | export: 16
f32 DIMSnowHorn_vehicle_Func16(Object* self, f32* oAnimDelta) {
    DIMSnowHorn_Data* objData = self->data;
    
    if (objData->fsa.animState == DIMSnowHorn_ASTATE_11_Vehicle_Walking) {
        *oAnimDelta = -objData->fsa.animTickDelta;
    } else {
        *oAnimDelta = 0.005f;
    }
    
    return 0.0f;
}

// offset: 0x148C | func: 22 | export: 17
s32 DIMSnowHorn_vehicle_GetRacePosition(Object* self) {
    return 0;
}

// offset: 0x149C | func: 23 | export: 18
void DIMSnowHorn_vehicle_Func18(Object* self) {

}

// offset: 0x14A8 | func: 24 | export: 19
void DIMSnowHorn_vehicle_HandleRiderScale(Object* self, f32 scale) {
    MtxF* jointMtx;
    f32 jointX;
    f32 jointY;
    f32 jointZ;
    SRT xform;

    jointMtx = objGetAttachPointBoneMatrix(self, 1);
    objGetAttachPointLocalSpace(self, 1, &jointX, &jointY, &jointZ);
    
    xform.yaw = 0;
    xform.pitch = 0;
    xform.roll = 0;
    xform.transl.x = jointX;
    xform.transl.y = jointY;
    xform.transl.z = jointZ;
    xform.scale = scale / self->def->scale;
    mathYprXyzMtx(&sRiderMtx, &xform);
    
    mathMtxCat4x3F(&sRiderMtx, jointMtx, &sRiderMtx);
    objprintSetModelMatrixOverride(&sRiderMtx);
}

// offset: 0x159C | func: 25
int DIMSnowHorn_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    ObjFSA_Data* fsa;
    DIMSnowHorn_Data* objData;
    s32 animState;
    s32 i;

    fsa = &((DIMSnowHorn_Data*)self->data)->fsa;
    objData = self->data;

    self->unkAF |= ARROW_FLAG_8_No_Targetting;

    switch (objData->characterIdx) {
    case DIMSnowHorn_IDX_0_Shackled: 
        animData->unk62 = 0;
        if (self->seqSlot == SEQSLOT_NONE) {
            //Gift a gear to Sabre after being freed
            for (i = 0; i < animData->messageCount; i++) {
                mainSetBits(BIT_DIM_Gear_1, 1);
            }
        }
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, DIMSnowHorn_ASTATE_1_Shackled_Idle);
        break;
    case DIMSnowHorn_IDX_1_Famished:
        animData->unk62 = 0;
        if (self->seqSlot != SEQSLOT_NONE) {
            switch (objData->chatSequenceIdx) {
            case 0:
                animState = DIMSnowHorn_ASTATE_6_Famished_Met;
                break;
            case 1:
                animState = DIMSnowHorn_ASTATE_6_Famished_Met;
                break;
            case 2: 
                animState = DIMSnowHorn_ASTATE_7_Famished_Fed_Once;
                break;
            case 3:
                animState = DIMSnowHorn_ASTATE_7_Famished_Fed_Once;
                break;
            case 4:
            default:
                animState = DIMSnowHorn_ASTATE_8_Vehicle_Idle;
                break;
            }
        } else {
            animState = DIMSnowHorn_ASTATE_8_Vehicle_Idle;
        }
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, animState);
        break;
    case DIMSnowHorn_IDX_2_Leap:
        animData->unk62 = 0;
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, DIMSnowHorn_ASTATE_3_Leap_Idle);
        break;
    case DIMSnowHorn_IDX_3_Blizzard:
        if (self->seqSlot == SEQSLOT_NONE) {
            if (animData->unk62 == 0) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, DIMSnowHorn_ASTATE_8_Vehicle_Idle);
                objData->chatSequenceIdx = 0;
            }

            if (mainGetBits(BIT_3D8)) {
                objData->chatSequenceIdx = 5;
                mainSetBits(BIT_3D8, 0);
                objData->flags &= ~DIMSnowHorn_FLAG_10;
            }
            
            if (objData->chatSequenceIdx == 5) {
                DIMSnowHorn_animCallbackVehicle(self, animObj, animData, prevCallbackValue);
            } else {
                animData->unk62 = 0;
            }
        } else {
            animData->unk62 = 0;
            fsa->enteredAnimState = TRUE;
        }
        break;
    }
    
    return animData->unk62 != 0;
}

// offset: 0x1860 | func: 26
int DIMSnowHorn_animCallbackVehicle(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    /*0x6C*/ static f32 dVehicleAnimCallbackMaxDistance = 0.0f;
    /*0x70*/ static s8 dVehicleAnimCallbackCount = 0;

    s32 pad1;
    s32 outValue;
    f32 diffX;
    f32 diffZ;
    f32 dx;
    f32 dz;
    f32 distance;
    f32 otherDist;
    s32 yawDiff;
    DIMSnowHorn_Data* objData;
    ObjFSA_Data* fsa;

    outValue = FALSE;
    
    fsa = &((DIMSnowHorn_Data*)self->data)->fsa;
    objData = self->data;

    if (animData->unk62 != 0) {
        if (animData->unk62 != 1) {
            animData->unk4C.x = self->srt.transl.f[0];
            animData->unk4C.y = self->srt.transl.f[1];
            animData->unk4C.z = self->srt.transl.f[2];
            dVehicleAnimCallbackMaxDistance = 10000.0f;
            dVehicleAnimCallbackCount = 0;
        }

        outValue = TRUE;
        
        fsa->unk4.mode = 1;
        animData->unk7A = 0;
        animData->unk62 = 1;
        
        diffX = animData->unk4C.f[0] - self->srt.transl.f[0];
        diffZ = animData->unk4C.f[2] - self->srt.transl.f[2];
        distance = sqrtf(SQ(diffX) + SQ(diffZ));
        if (distance <= dVehicleAnimCallbackMaxDistance) {
            dVehicleAnimCallbackCount++;
        }
        
        dx = animObj->srt.transl.f[0] - animData->unk4C.f[0];
        dz = animObj->srt.transl.f[2] - animData->unk4C.f[2];
        otherDist = sqrtf(SQ(dx) + SQ(dz));
        if (otherDist <= distance) {
            objData->minTurn = 0;
            yawDiff = self->srt.yaw - (animObj->srt.yaw & 0xFFFF);
            CIRCLE_WRAP(yawDiff);
            if (yawDiff > M_90_DEGREES) {
                yawDiff = M_90_DEGREES;
            }
            if (yawDiff < -M_90_DEGREES) {
                yawDiff = -M_90_DEGREES;
            }

            if ((yawDiff < 0x100) && (yawDiff > -0x100)) {
                self->srt.yaw = animObj->srt.yaw;
                animData->unk62 = 0;
                animData->unk7A = animData->unk7C;
                animData->prevTime = animData->time - 1;
                outValue = FALSE;
            } else {
                if (fsa->animState == DIMSnowHorn_ASTATE_11_Vehicle_Walking) {
                    fsa->enteredAnimState = TRUE;
                    fsa->prevAnimState = DIMSnowHorn_ASTATE_8_Vehicle_Idle;
                    fsa->animState = fsa->prevAnimState;
                }
                fsa->xAnalogInput = -mathSinfInterp(-animObj->srt.yaw) * 60.0f;
                fsa->yAnalogInput = -mathCosfInterp(-animObj->srt.yaw) * 60.0f;
                gDLL_18_objfsa->vtbl->func3(&animObj->srt);
                fsa->unk30C = 0;
                fsa->unk310 = 0;
                fsa->unk324 = 0;
                gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
            }
        } else {
            dx = dx / otherDist;
            dz = dz / otherDist;
            fsa->xAnalogInput = -dx * 50.0f;
            fsa->yAnalogInput = dz * 50.0f;
            self->srt.transl.f[0] = animData->unk4C.f[0] + (distance * dx);
            self->srt.transl.f[2] = animData->unk4C.f[2] + (distance * dz);
            gDLL_18_objfsa->vtbl->func3(&animObj->srt);
            fsa->unk30C = 0;
            fsa->unk310 = 0;
            fsa->unk324 = 0;
            gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
        }
        dVehicleAnimCallbackMaxDistance = distance;
    } else {
        fsa->unk4.mode = 0;
        gDLL_27->vtbl->reset(self, &fsa->unk4);
        animData->unk7A = animData->unk7C;
    }

    return outValue;
}

// offset: 0x1C78 | func: 27
void DIMSnowHorn_lookAtPlayerWhenNearby(Object* self) {
    DIMSnowHorn_Data* objData;
    Object* player;

    objData = self->data;
    
    player = objGetPlayer();
    if (player != NULL) {
        if ((vec3Distance(&player->globalPosition, &self->globalPosition) < 300.0f) && (objData->mountState == VEHICLE_NoRider)) {
            objData->headAnim.aimIsActive = TRUE;
            objData->headAnim.headAimX = player->srt.transl.x;
            objData->headAnim.headAimY = player->srt.transl.y;
            objData->headAnim.headAimZ = player->srt.transl.z;
            return;
        }
    }
    
    objData->headAnim.aimIsActive = FALSE;
}

// offset: 0x1D34 | func: 28
static void DIMSnowHorn_stop(Object* self, ObjFSA_Data* fsa) {
    fsa->speed = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->unk278 = 0.0f;
    
    self->velocity.x = 0.0f;
    self->velocity.y = 0.0f;
    self->velocity.z = 0.0f;
}

/*0x74*/ static s16 dIdleFidgetAnims[] = {
    SnowHorn_MODANIM1_3_Idle_Fidget, 
    SnowHorn_MODANIM0_11_Shaking_Off 
};
/*0x78*/ static f32 dIdleFidgetAnimSpeeds[] = {
    0.0031, 
    0.005 
};

/*0x80*/ static s16 dWalkingAnims[2] = {
    SnowHorn_MODANIM0_0_Idle_LOOP, 
    SnowHorn_MODANIM0_3_Walk_LOOP
};
/*0x84*/ static f32 dWalkSpeedThresholds[] = {
    0.0, 0.05, 
    0.03, 0.85
};

// offset: 0x1D68 | func: 29
s32 DIMSnowHorn_animState0Init(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData = self->data;
    
    switch (objData->characterIdx) {
    case DIMSnowHorn_IDX_0_Shackled:
        if (mainGetBits(BIT_DIM_Shackled_SnowHorn_Freed)) {
            objData->flags |= DIMSnowHorn_FLAG_20_Already_Freed;
        }
        return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_1_Shackled_Idle);
    case DIMSnowHorn_IDX_1_Famished:
        if (mainGetBits(BIT_DIM_Famished_SnowHorn_Fed_Two_Alpine_Roots)) {
            return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_8_Vehicle_Idle);
        } else if (mainGetBits(BIT_DIM_Famished_SnowHorn_Fed_One_Alpine_Root)) {
            return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_7_Famished_Fed_Once);
        } else if (mainGetBits(BIT_DIM_Famished_SnowHorn_Met)) {
            return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_6_Famished_Met);
        } else {
            return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_5_Famished_Fallen);
        }
    case DIMSnowHorn_IDX_2_Leap:
        if (mainGetBits(BIT_Horn_of_Truth)) {
            objData->chatSequenceIdx = 8;
            return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_3_Leap_Idle);
        } else {
            objData->chatSequenceIdx = 6;
            return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_3_Leap_Idle);
        }
    case DIMSnowHorn_IDX_3_Blizzard:
        return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_8_Vehicle_Idle);
    default:
        return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_8_Vehicle_Idle);
    }
}

// offset: 0x1EB0 | func: 30
s32 DIMSnowHorn_animState1ShackledIdle(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData;

    objData = self->data;
    
    DIMSnowHorn_stop(self, fsa);

    fsa->flags |= OBJFSA_FLAG_200000;
    
    //Play standing animation
    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.005f;
        if (self->curModAnimId != dWalkingAnims[0]) {
            objAnimSet(self, dWalkingAnims[0], 0.0f, 0);
        }
        objData->fidgetTimer = mathRnd(1200, 2400);
    }
    
    //Play an idle fidget animation at random intervals (scratching her back/shaking off snow)
    objData->fidgetTimer -= (s32)updateRate;
    if (objData->fidgetTimer <= 0) {
        return FSA_NEXTSTATE_ASYNC(DIMSnowHorn_ASTATE_2_Shackled_Fidget);
    }
    
    //Play a sequence when talked to ("Can you help me get free?")
    if (!(objData->flags & DIMSnowHorn_FLAG_20_Already_Freed) && (self->unkAF & ARROW_FLAG_1_Interacted)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(5, self, -1);
        joyDisableButtons(0, A_BUTTON);
    }
    
    return 0;
}

// offset: 0x2024 | func: 31
s32 DIMSnowHorn_animState2ShackledFidget(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 animIdx;
    DIMSnowHorn_Data* objData;

    objData = self->data;
    
    DIMSnowHorn_stop(self, fsa);

    fsa->flags |= OBJFSA_FLAG_200000;
    
    if (fsa->enteredAnimState) {
        animIdx = mathRnd(0, ARRAYCOUNT(dIdleFidgetAnims) - 1);
        fsa->animTickDelta = dIdleFidgetAnimSpeeds[animIdx];
        objAnimSet(self, dIdleFidgetAnims[animIdx], 0.0f, 0);
    }
    
    if (fsa->unk33A) {
        return FSA_NEXTSTATE_ASYNC(DIMSnowHorn_ASTATE_1_Shackled_Idle);
    }
    
    if (!(objData->flags & DIMSnowHorn_FLAG_20_Already_Freed) && (self->unkAF & ARROW_FLAG_1_Interacted)) {
        dll_anim->start_obj_sequence(5, self, -1);
        joyDisableButtons(0, A_BUTTON);
    }
    
    return 0;
}

// offset: 0x2174 | func: 32
s32 DIMSnowHorn_animState3LeapIdle(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData = self->data;
    
    DIMSnowHorn_stop(self, fsa);

    fsa->flags |= OBJFSA_FLAG_200000;
    
    //Play standing animation
    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.005f;
        if (self->curModAnimId != dWalkingAnims[0]) {
            objAnimSet(self, dWalkingAnims[0], 0.0f, 0);
        }
        objData->fidgetTimer = mathRnd(1200, 2400);
    }
    
    objData->fidgetTimer -= (s32) updateRate;
    if (objData->fidgetTimer <= 0) {
        return FSA_NEXTSTATE_ASYNC(DIMSnowHorn_ASTATE_4_Leap_Fidget);
    }
    
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        if (mainGetBits(BIT_Horn_of_Truth)) {
            objData->chatSequenceIdx = 8;
        }
        
        gDLL_3_Animation->vtbl->start_obj_sequence(objData->chatSequenceIdx, self, -1);

        if (objData->chatSequenceIdx == 6) {
            objData->chatSequenceIdx++;
        }
        
        joyDisableButtons(0, A_BUTTON);
    }
    
    return 0;
}

// offset: 0x22FC | func: 33
s32 DIMSnowHorn_animState4LeapFidget(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData;
    s32 animIdx;

    objData = self->data;
    
    DIMSnowHorn_stop(self, fsa);
    
    fsa->flags |= OBJFSA_FLAG_200000;

    if (fsa->enteredAnimState) {
        animIdx = mathRnd(0, 1);
        fsa->animTickDelta = dIdleFidgetAnimSpeeds[animIdx];
        objAnimSet(self, dIdleFidgetAnims[animIdx], 0.0f, 0);
    }
    
    if (fsa->unk33A) {
        return FSA_NEXTSTATE_ASYNC(DIMSnowHorn_ASTATE_3_Leap_Idle);
    }
    
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        if (mainGetBits(BIT_Horn_of_Truth)) {
            objData->chatSequenceIdx = 8;
        }
        
        dll_anim->start_obj_sequence(objData->chatSequenceIdx, self, -1);
        if (objData->chatSequenceIdx == 6) {
            objData->chatSequenceIdx++;
        }
        
        joyDisableButtons(0, A_BUTTON);
    }
    
    return 0;
}

// offset: 0x2470 | func: 34
s32 DIMSnowHorn_animState5FamishedFallen(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData;

    DIMSnowHorn_stop(self, fsa);
    
    fsa->flags |= OBJFSA_FLAG_200000;

    objData = self->data;
    
    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.005f;
        if (self->curModAnimId != SnowHorn_MODANIM0_19_Fallen_Idle_LOOP) {
            objAnimSet(self, SnowHorn_MODANIM0_19_Fallen_Idle_LOOP, 0.0f, 0);
        }
    }
    
    //Don't allow interaction until the SharpClaw attacker is defeated
    if (mainGetBits(BIT_DIM_Defeated_SharpClaw_Hurting_Fallen_SnowHorn)) {
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    } else {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    }
    
    //Play a sequence when talked to, teaching the Flame Command to Tricky
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        objData->chatSequenceIdx = 0;
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        mainSetBits(BIT_Tricky_Learned_Flame, 1);
        mainSetBits(BIT_DIM_Famished_SnowHorn_Met, 1);
        joyDisableButtons(0, A_BUTTON);
    }
    
    return 0;
}

// offset: 0x25E4 | func: 35
s32 DIMSnowHorn_animState6FamishedMet(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData;

    DIMSnowHorn_stop(self, fsa);

    fsa->flags |= OBJFSA_FLAG_200000;
    
    objData = self->data;
    
    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    
    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.005f;
        if (self->curModAnimId != SnowHorn_MODANIM0_19_Fallen_Idle_LOOP) {
            objAnimSet(self, SnowHorn_MODANIM0_19_Fallen_Idle_LOOP, 0.0f, 0);
        }
    }
    
    if (self->unkAF & ARROW_FLAG_4_Highlighted) {
        if (gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_DIM_Alpine_Roots)) {
            objData->chatSequenceIdx = 2;
            gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
            mainSetBits(BIT_DIM_Famished_SnowHorn_Fed_One_Alpine_Root, 1);
            mainSetBits(BIT_DIM_Alpine_Roots, mainGetBits(BIT_DIM_Alpine_Roots) - 1);
            joyDisableButtons(0, A_BUTTON);
        } else if (self->unkAF & ARROW_FLAG_1_Interacted) {
            objData->chatSequenceIdx = 1;
            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
            joyDisableButtons(0, A_BUTTON);
        }
    }
    
    return 0;
}

// offset: 0x27D4 | func: 36
s32 DIMSnowHorn_animState7FanishedFedOnce(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData;
    s8 value;

    DIMSnowHorn_stop(self, fsa);
    
    fsa->flags |= OBJFSA_FLAG_200000;
    
    objData = self->data;
    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.005f;
        if (self->curModAnimId != SnowHorn_MODANIM0_19_Fallen_Idle_LOOP) {
            objAnimSet(self, SnowHorn_MODANIM0_19_Fallen_Idle_LOOP, 0.0f, 0);
        }
    }
    
    if (self->unkAF & ARROW_FLAG_4_Highlighted) {
        if (gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_DIM_Alpine_Roots)) {
            objData->chatSequenceIdx = 4;
            gDLL_3_Animation->vtbl->start_obj_sequence(4, self, -1);
            mainSetBits(BIT_DIM_Famished_SnowHorn_Fed_Two_Alpine_Roots, 1);
            
            value = mainGetBits(BIT_DIM_Alpine_Roots);
            if (--value < 0) {
                value = 0;
            }
            mainSetBits(BIT_DIM_Alpine_Roots, value);
        } else if (self->unkAF & ARROW_FLAG_1_Interacted) {
            objData->chatSequenceIdx = 3;
            gDLL_3_Animation->vtbl->start_obj_sequence(3, self, -1);
            joyDisableButtons(0, A_BUTTON);
        }
    }
    
    return 0;
}

// offset: 0x29B8 | func: 37
s32 DIMSnowHorn_animState8VehicleIdle(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData = self->data;
    
    DIMSnowHorn_stop(self, fsa);
    
    fsa->flags |= OBJFSA_FLAG_200000;

    if (fsa->enteredAnimState) {
        fsa->animStateTime = 0;
        fsa->animTickDelta = 0.005f;
        fsa->unk2B0 = 8.0f;
        if (self->curModAnimId != dWalkingAnims[0]) {
            objAnimSet(self, dWalkingAnims[0], 0.0f, 0);
        }
    }
    
    if (((self->curModAnimId == SnowHorn_MODANIM2_9_Walk_Outro_L) || 
         (self->curModAnimId == SnowHorn_MODANIM2_10_Walk_Outro_R)) 
         && fsa->unk33A
    ) {
        objAnimSet(self, dWalkingAnims[0], 0.0f, 0);
        fsa->animTickDelta = 0.005f;
    }
    
    if (fsa->analogInputPower < 0.05f) {
        fsa->unk328 = 0;
        fsa->unk32A = 0;
        fsa->analogInputPower = 0;
    }
    
    if ((fsa->prevAnalogInputPower > 0) && (fsa->analogInputPower > 0) && (fsa->unk328 >= objData->minTurn)) {
        return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_10_Vehicle_Turn_on_Spot);
    }
    if ((fsa->prevAnalogInputPower > 0.1f) && (fsa->analogInputPower > 0.1f) && (fsa->unk328 < objData->minTurn)) {
        return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_11_Vehicle_Walking);
    }
    
    if (fsa->unk310 & A_BUTTON) {
        return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_12_Vehicle_Tusk_Attack);
    } else {
        return 0;
    }
}

// offset: 0x2BA0 | func: 38
/**
  * An unused state! Maybe the idle state's intended to branch to this after a while, 
  * or it could be meant for after running out of energy in the blizzard? 
  */
s32 DIMSnowHorn_animState9VehicleSit(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData = self->data;
    
    fsa->flags |= OBJFSA_FLAG_200000;

    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    
    switch (self->curModAnimId) {
    case SnowHorn_MODANIM2_6_Sit_Intro:
        if (fsa->unk33A) {
            if (fsa->animTickDelta > 0.0f) {
                objAnimSet(self, SnowHorn_MODANIM2_5_Sit_Idle_LOOP, 0.0f, 0);
                fsa->animTickDelta = 0.005f;
            } else {
                return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_8_Vehicle_Idle);
            }
        }
        
        if (objData->energy != 0) {
            if ((fsa->animTickDelta > 0.0f) && ((fsa->unk310 != 0) || (fsa->xAnalogInput != 0.0f) || (fsa->yAnalogInput != 0.0f))) {
                fsa->animTickDelta = -fsa->animTickDelta;
            }
        }

        break;
    case SnowHorn_MODANIM2_5_Sit_Idle_LOOP:
        //Get bacl up when pressing a button or moving the joystick
        if ((objData->energy != 0) && ((fsa->unk310 != 0) || (fsa->xAnalogInput != 0.0f) || (fsa->yAnalogInput != 0.0f))) {
            objAnimSet(self, SnowHorn_MODANIM2_7_Sit_Outro, 0.0f, 0);
            fsa->animTickDelta = 0.014f;
        }
        break;
    case SnowHorn_MODANIM2_7_Sit_Outro:
        if (fsa->unk33A) {
            return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_8_Vehicle_Idle);
        }
        break;
    default:
        objAnimSet(self, SnowHorn_MODANIM2_6_Sit_Intro, 0.0f, 0);
        fsa->animTickDelta = 0.014f;
        break;
    }

    return 0;
}

// offset: 0x2D90 | func: 39
s32 DIMSnowHorn_animState10VehicleTurn(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData = self->data;

    fsa->flags |= OBJFSA_FLAG_200000;

    if ((fsa->unk328 < objData->minTurn) || (fsa->analogInputPower == 0.0f)) {
        return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_8_Vehicle_Idle);
    }

    if (fsa->unk32A < -175) {
        fsa->unk32A = -fsa->unk32A;
    }
    
    if ((fsa->unk32A > 0) && (self->curModAnimId != SnowHorn_MODANIM2_1_Turn_Left_LOOP)) {
        objAnimSet(self, SnowHorn_MODANIM2_1_Turn_Left_LOOP, 0.0f, 0);
    } else if ((fsa->unk32A <= 0) && (self->curModAnimId != SnowHorn_MODANIM2_0_Turn_Right_LOOP)) {
        objAnimSet(self, SnowHorn_MODANIM2_0_Turn_Right_LOOP, 0.0f, 0);
    }
    fsa->animTickDelta = 0.012f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 8);
    
    if (fsa->unk310 & A_BUTTON) {
        return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_12_Vehicle_Tusk_Attack);
    } else {
        return 0;
    }
}

// offset: 0x2EEC | func: 40
s32 DIMSnowHorn_animState11VehicleWalk(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    u8 one;
    DIMSnowHorn_Data *objData = self->data;
    s32 pad;
    f32 walkFactor;
    f32 walkSpeed;
    f32 animProgress;
    s16 curModAnimId;
    s32 animChanged;
    s32 returnValue;
    s32 startingWalk;
    s32 walkAnimIdx;
    f32 *thresholds;

    fsa->flags |= OBJFSA_FLAG_200000;

    if (fsa->enteredAnimState) {
        self->srt.yaw += fsa->unk32A * M_1_DEGREE;
        fsa->unk328 = 0;
        fsa->unk32A = 0;
    }

    //Handle joystick deadzone
    if (fsa->analogInputPower < 0.05f){
        fsa->analogInputPower = 0.0f;
        fsa->unk328 = 0;
        fsa->unk32A = 0;
    }

    //Turn while walking
    if (fsa->unk328 < 90){
        self->srt.yaw += ((fsa->unk32A * updateRate) / 36.0f) * 182.0f;
    } else {
        return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_8_Vehicle_Idle);
    }

    //Handle walk speed
    {
        walkFactor = fsa->analogInputPower;
        if (walkFactor < 0.0f){
            walkFactor = 0.0f;
        }
        if (walkFactor > 1.0f){
            walkFactor = 1.0f;
        }

        // Handle SnowHorn running out of energy 
        /* (@bug: causes a softlock, since you're stuck riding the SnowHorn and 
          there's no fail state cutscene to reset you back to before the blizzard) */
        if (objData->energy == 0){
            walkFactor = 0;
        }

        walkSpeed = walkFactor * 0.85f;
        if (walkSpeed < 0){
            walkSpeed = 0;
        }

        fsa->speed += ((walkSpeed - fsa->speed) / fsa->unk2B0) * updateRate;
        if (self->srt.pitch > 0){
            walkSpeed -= mathSinfInterp(self->srt.pitch) * 0.3f;
        } else {
            walkSpeed -= mathSinfInterp(self->srt.pitch) * 0.15f;
        }

        if (walkSpeed < dWalkSpeedThresholds[2]){
            walkSpeed = dWalkSpeedThresholds[2];
        }

        fsa->unk278 += ((walkSpeed - fsa->unk278) / fsa->unk2B0) * updateRate;
    }

    //Handle animations
    {
        animChanged = FALSE;
        one = 1;

        animProgress = self->animProgress;

        //Find the current walkAnimIdx
        {
            for (walkAnimIdx = 0; self->curModAnimId != dWalkingAnims[walkAnimIdx] && walkAnimIdx < ARRAYCOUNT_S(dWalkingAnims); walkAnimIdx++);
            
            if (walkAnimIdx > 1){
                walkAnimIdx = 0;
            }

            if (self->curModAnimId == SnowHorn_MODANIM2_8_Walk_Intro){
                walkAnimIdx = 1;
            }
        }

        //Compare the walk speed with the walk anims' min/max speed thresholds, to determine whether anim should change
        thresholds = &dWalkSpeedThresholds[walkAnimIdx * 2];
        if (fsa->speed < thresholds[0]){
            animChanged = TRUE;
            if (walkAnimIdx == 1){
                return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_8_Vehicle_Idle);
            }
            walkAnimIdx -= one;
        } else if (thresholds[1] <= fsa->speed){
            animChanged = TRUE;
            if (walkAnimIdx == 0){
                animProgress = 0.0f;
            }
            walkAnimIdx++;
        }

        //Check whether a walk intro animation should be played
        startingWalk = TRUE;
        if (fsa->unk33A && (self->curModAnimId == SnowHorn_MODANIM2_8_Walk_Intro)){
            animChanged = TRUE;
            startingWalk = FALSE;
        }

        //Play/advance walk animations
        if (animChanged){
            if ((walkAnimIdx == 1) && startingWalk){
                objAnimSet(self, SnowHorn_MODANIM2_8_Walk_Intro, animProgress, 0);
            } else {
                objAnimSet(self, dWalkingAnims[walkAnimIdx], animProgress, 0);
            }
        }
        objGetAnimChange(self, fsa->unk278, &fsa->animTickDelta);
    }

    if (fsa->unk310 & A_BUTTON){
        return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_12_Vehicle_Tusk_Attack);
    } else {
        return 0;
    }
}

// offset: 0x32C0 | func: 41
s32 DIMSnowHorn_animState12VehicleAttack(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    ObjectHitInfo* objHits;
    DIMSnowHorn_Data* objData;

    objData = self->data;
    objHits = self->objhitInfo;
    
    fsa->flags |= OBJFSA_FLAG_200000;
    
    DIMSnowHorn_stop(self, fsa);
    
    if (fsa->enteredAnimState) {
        objData->flags &= ~DIMSnowHorn_FLAG_8;
        objHits->unk58 |= 0x200;
        objAnimSet(self, SnowHorn_MODANIM2_4_Tusk_Attack, 0.0f, 0);
        fsa->animTickDelta = 0.013f;
    }
    
    if ((objHits->unk58 & 0x200) && (objHits->unk9D & 2)) {
        objData->flags |= DIMSnowHorn_FLAG_8;
    }

    if (objData->flags & DIMSnowHorn_FLAG_8) {
        objHits->unk5F = 0;
        objHits->unk60 = 0;
        objHits->unk58 &= ~0x200;
    } else {
        objHits->unk5F = 0xB;
        objHits->unk60 = 1;
        objHits->unk58 |= 0x200;
    }
    
    if (self->animProgress > 0.9f) {
        return FSA_NEXTSTATE_SYNC(DIMSnowHorn_ASTATE_8_Vehicle_Idle);
    } else {
        return 0;
    }
}

// offset: 0x3430 | func: 42
s32 DIMSnowHorn_logicState0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
}

// offset: 0x3448 | func: 43 | export: 20
void DIMSnowHorn_vehicle_Func20(Object* self, ObjFSA_Data* fsa, f32 updateRate) {

}

