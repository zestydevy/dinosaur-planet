//"core_snowworm.c" seems to have been Rare's filename

#include "common.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objlib.h"
#include "sys/objtype.h"
#include "dlls/objects/216_SnowWorm.h"
#include "dlls/objects/221_ChukaChuck.h"
#include "dlls/objects/251_weapons.h"

typedef struct {
    Baddie_Setup baddie;
} SnowWorm_Setup;

typedef struct {
    f32 timeSinceHit;
    s16 unk4;
    s16 consecutiveTicksHit;
    SRT throwOrigin;
    SRT fxTransform;
    Vec3f throwVelocity;
    u8 flags;
    u8 unk45;
    u16 unk46;
} SnowWorm_DataActual;

typedef struct {
    Baddie baddie;
    SnowWorm_DataActual objData;
} SnowWorm_Data;

typedef enum {
    SnowWorm_ASTATE_0, //underground?
    SnowWorm_ASTATE_1, //emerging from ground
    SnowWorm_ASTATE_2, //retreating underground
    SnowWorm_ASTATE_3,
    SnowWorm_ASTATE_4,
    SnowWorm_ASTATE_5, //attack, headbutt OR forward chomp?
    SnowWorm_ASTATE_6, //attack, clockwise sweep chomp OR double-chomp?
    SnowWorm_ASTATE_7,
    SnowWorm_ASTATE_8, //idle?
    SnowWorm_ASTATE_9,
    SnowWorm_ASTATE_10, //hurt?
    SnowWorm_ASTATE_11,
    SnowWorm_ASTATE_12,
    SnowWorm_ASTATE_13 //dying?
} SnowWorm_AnimStates;

typedef enum {
    SnowWorm_LSTATE_0, //underground?
    SnowWorm_LSTATE_1, //hurt/flinching ACTUAL?
    SnowWorm_LSTATE_2_Dying,
    SnowWorm_LSTATE_3,
    SnowWorm_LSTATE_4, //retreating underground?
    SnowWorm_LSTATE_5,
    SnowWorm_LSTATE_6,
    SnowWorm_LSTATE_7 //idle/ready to attack?
} SnowWorm_LogicStates;

typedef enum {
    SnowWorm_FLAG_0 = 0,
    SnowWorm_FLAG_1 = 1,
    SnowWorm_FLAG_2 = 2,
    SnowWorm_FLAG_4 = 4,
    SnowWorm_FLAG_8 = 8,
    SnowWorm_FLAG_10 = 0x10,
    SnowWorm_FLAG_20 = 0x20
} SnowWorm_Flags;

/*0x0*/ static u32 dHurtSounds[] = {
    SOUND_336_Worm_Wounded, SOUND_337_Worm_Wounded, SOUND_33B_Worm_Dying
};
/*0xC*/ static u32 dRoarSounds[] = {
    SOUND_5A_SandWorm_Roar, SOUND_5B_SandWorm_Roar, SOUND_32B_SandWorm_Snarl, SOUND_335_SandWorm_Snarl
};
/*0x1C*/ static u32 dBattleSounds[] = {
    SOUND_339_Worm_Emerge, SOUND_338_Blast, SOUND_339_Worm_Emerge, SOUND_33C_Hiss, SOUND_33D_Hiss_Long
};
/*0x30*/ static s16 dAnimStateSequenceA[] = {
    SnowWorm_ASTATE_5, 
    SnowWorm_ASTATE_6, 
    SnowWorm_ASTATE_8, 
    SnowWorm_ASTATE_6, 
    SnowWorm_ASTATE_5, 
    SnowWorm_ASTATE_8, 
    SnowWorm_ASTATE_6
};
/*0x40*/ static s16 dAnimStateSequenceB[] = {
    SnowWorm_ASTATE_8, 
    SnowWorm_ASTATE_6, 
    SnowWorm_ASTATE_9, 
    SnowWorm_ASTATE_8, 
    SnowWorm_ASTATE_6, 
    SnowWorm_ASTATE_9, 
    SnowWorm_ASTATE_9
};
/*0x50*/ static u32 dWeaponHitSounds[] = {
    SOUND_374_Whack, 
    SOUND_375_Smack1, 
    SOUND_376_Smack2, 
    SOUND_25B_Magic_Attack_Deflected, 
    SOUND_25C_Melee_Attack_Deflected
};
/*0x64*/ static s32 dHitAnimStateMap[] = {
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_12, //NOTE: different
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10, 
    SnowWorm_ASTATE_10
};
/*0xD4*/ static s8 dHitDamageMap[] = {
    -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1
};
/*0xF0*/ static u8 dParticleColours[5][3] = {
    {0xff, 0xff, 0xff}, 
    {0x00, 0xff, 0x00}, 
    {0xff, 0xc0, 0x96}, 
    {0x5a, 0x5a, 0x64},
    {0xff, 0x5a, 0x00}
};
/*0x100*/ static u8 dTerrainFXColourIndices[] = {
    0, 1, 2, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    2, 0, 3, 0, 0, 0, 0, 0,
    3, 3, 4, 0, 0, 0, 0, 0
};

/*0x0*/ static ObjFSA_StateCallback sAnimStateCallbacks[14];
/*0x38*/ static ObjFSA_StateCallback sLogicStateCallbacks[8];

static void SnowWorm_func_908(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void SnowWorm_func_CE8(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void SnowWorm_func_EBC(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void SnowWorm_func_10D8(Object* self, Baddie* baddie);
static void SnowWorm_func_1374(Object* self, Baddie* baddie);
static void SnowWorm_throw_ice_ball(Object* self, SnowWorm_DataActual* objData);

static s32 SnowWorm_anim_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_anim_state_1(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_anim_state_2(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_anim_state_3(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_anim_state_4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_anim_state_5(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_anim_state_6(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_anim_state_7(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_anim_state_8(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_anim_state_9(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_anim_state_10(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_anim_state_11(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_anim_state_12(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_anim_state_13(Object* self, ObjFSA_Data* fsa, f32 updateRate);

static s32 SnowWorm_logic_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_logic_state_1(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_logic_state_2_dying(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_logic_state_3(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_logic_state_4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_logic_state_5(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_logic_state_6(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWorm_logic_state_7(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void SnowWorm_init_fsa_callbacks(void) {
    sAnimStateCallbacks[SnowWorm_ASTATE_0] = SnowWorm_anim_state_0;
    sAnimStateCallbacks[SnowWorm_ASTATE_1] = SnowWorm_anim_state_1;
    sAnimStateCallbacks[SnowWorm_ASTATE_2] = SnowWorm_anim_state_2;
    sAnimStateCallbacks[SnowWorm_ASTATE_3] = SnowWorm_anim_state_3;
    sAnimStateCallbacks[SnowWorm_ASTATE_4] = SnowWorm_anim_state_4;
    sAnimStateCallbacks[SnowWorm_ASTATE_5] = SnowWorm_anim_state_5;
    sAnimStateCallbacks[SnowWorm_ASTATE_6] = SnowWorm_anim_state_6;
    sAnimStateCallbacks[SnowWorm_ASTATE_7] = SnowWorm_anim_state_7;
    sAnimStateCallbacks[SnowWorm_ASTATE_8] = SnowWorm_anim_state_8;
    sAnimStateCallbacks[SnowWorm_ASTATE_9] = SnowWorm_anim_state_9;
    sAnimStateCallbacks[SnowWorm_ASTATE_10] = SnowWorm_anim_state_10;
    sAnimStateCallbacks[SnowWorm_ASTATE_11] = SnowWorm_anim_state_11;
    sAnimStateCallbacks[SnowWorm_ASTATE_12] = SnowWorm_anim_state_12;
    sAnimStateCallbacks[SnowWorm_ASTATE_13] = SnowWorm_anim_state_13;
    
    sLogicStateCallbacks[SnowWorm_LSTATE_0] = SnowWorm_logic_state_0;
    sLogicStateCallbacks[SnowWorm_LSTATE_1] = SnowWorm_logic_state_1;
    sLogicStateCallbacks[SnowWorm_LSTATE_2_Dying] = SnowWorm_logic_state_2_dying;
    sLogicStateCallbacks[SnowWorm_LSTATE_3] = SnowWorm_logic_state_3;
    sLogicStateCallbacks[SnowWorm_LSTATE_4] = SnowWorm_logic_state_4;
    sLogicStateCallbacks[SnowWorm_LSTATE_5] = SnowWorm_logic_state_5;
    sLogicStateCallbacks[SnowWorm_LSTATE_6] = SnowWorm_logic_state_6;
    sLogicStateCallbacks[SnowWorm_LSTATE_7] = SnowWorm_logic_state_7;
}

// offset: 0x12C | ctor
void SnowWorm_ctor(void* dll) {
    SnowWorm_init_fsa_callbacks();
}

// offset: 0x16C | dtor
void SnowWorm_dtor(void* dll) { }

// offset: 0x178 | func: 1 | export: 0
void SnowWorm_setup(Object* self, SnowWorm_Setup* objSetup, s32 reset) {
    Baddie* baddie;
    u8 flags;

    baddie = self->data;
    
    flags = 2 | 4;
    if (reset) {
        flags = 1 | 2 | 4;
    }
    if ((objSetup->baddie.unk2B & 0x20) == FALSE) {
        flags |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, &objSetup->baddie, baddie, 0xE, 8, 0x102, flags, 20.0f);

    self->animCallback = NULL;

    if ((baddie->unk3E2 * 0.5f) < 55.0f) {
        baddie->unk3E2 = 110;
    }
    
    objAnimSet(self, 8, 0.0f, 0);
    
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    
    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, SnowWorm_ASTATE_0);
    baddie->fsa.logicState = SnowWorm_LSTATE_0;
    baddie->fsa.unk4.mode = 0;
}

// offset: 0x2C0 | func: 2 | export: 1
void SnowWorm_control(Object* self) {
    Baddie* baddie;
    Baddie_Setup* objSetup;

    baddie = self->data;
    objSetup = (Baddie_Setup*)self->setup;

    if (self->unkDC != 0) {
        if (((baddie->fsa.logicState != SnowWorm_LSTATE_3) || (baddie->unk3B0 & 1)) && (gDLL_29_Gplay->vtbl->did_time_expire(objSetup->base.uID))) {
            gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 0xE, 8, 0x102, 0x26, 20.0f);
            baddie->unk3B6 = 0;
            gDLL_6_AMSFX->vtbl->play(self, SOUND_B20_Low_Grunt, MAX_VOLUME, NULL, NULL, 0, NULL);
            objAnimSet(self, 8, 0.0f, 0x10);
            baddie->fsa.unk33A = 0;
            self->opacity = OBJECT_OPACITY_MAX;
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
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
    
    if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 0) == 0) {
        baddie->unk3B6 = 0;
        return;
    }
    
    SnowWorm_func_908(self, baddie, &baddie->fsa);
    SnowWorm_func_1374(self, baddie);
    
    if (baddie->unk3B6 == 0) {
        SnowWorm_func_EBC(self, baddie, &baddie->fsa);
    } else {
        SnowWorm_func_CE8(self, baddie, &baddie->fsa);
    }
    
    if (baddie->unk3B0 & 2) {
        self->srt.transl.y = objSetup->base.y - 8.0f;
    }
}

// offset: 0x554 | func: 3 | export: 2
void SnowWorm_update(Object* self) {
    SnowWorm_Data* objData = self->data;
    gDLL_18_objfsa->vtbl->func2(self, &objData->baddie.fsa, sAnimStateCallbacks);
}

// offset: 0x5A4 | func: 4 | export: 3
void SnowWorm_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie = self->data;
    
    if ((visibility == FALSE) || self->unkDC || (baddie->unk3B6 == 0)) {
        return;
    } 
    
    if (baddie->unk3E8) {
        objprintSetBlendColor(0xC8, 0, 0, baddie->unk3E8);
    }
    
    objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    SnowWorm_func_10D8(self, baddie);
    
    if (baddie->unk3B2 & 0x60) {
        if (baddie->unk3B2 & 0x20) {
            gDLL_32->vtbl->func2(self, 0x330, &baddie->unk3E8);
            gDLL_32->vtbl->func2(self, 0x330, &baddie->unk3E8);
        }
        
        gDLL_32->vtbl->func2(self, 0x32F, &baddie->unk3E8);
    }
}

// offset: 0x7AC | func: 5 | export: 4
void SnowWorm_free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    camIgnoreShake();
    objFreeObjectType(self, OBJTYPE_Baddie);

    if (self->linkedObject) {
        objFreeObject(self->linkedObject);
        self->linkedObject = NULL;
    }
    
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
}

// offset: 0x858 | func: 6 | export: 5
u32 SnowWorm_get_model_flags(Object* self) {
    return MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x868 | func: 7 | export: 6
u32 SnowWorm_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(SnowWorm_Data);
}

// offset: 0x87C | func: 8 | export: 7
SnowWorm_AnimStates SnowWorm_get_anim_state(Object* self) {
    SnowWorm_Data* objData = self->data;
    return objData->baddie.fsa.animState;
}

// offset: 0x88C | func: 9 | export: 8
void SnowWorm_receive_message(Object* self, u8 message) {
    Baddie* baddie;
    ObjFSA_Data* fsa;

    baddie = self->data;
    
    switch (message) {
    case 0x80:
        fsa = &baddie->fsa;
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_2);
        fsa->logicState = SnowWorm_LSTATE_4;
        fsa->enteredLogicState = TRUE;
        break;
    default:
        STUBBED_PRINTF("BADDIE:SnowWorm Unknown message [%d]\n", message);
    }
}

// offset: 0x908 | func: 10
void SnowWorm_func_908(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    SnowWorm_DataActual* objData = baddie->objdata;
    Object* player = objGetPlayer();
    Vec3f d;
    SRT fxTransform;
    s32 i;
    s32 scaleIdx;
/*0x120*/ s16 dFXScales1[4] = { 0x0206, 0x0167, 0x0165, 0x0206 };
/*0x128*/ s16 dFXScales2[4] = { 0x0206, 0x0167, 0x0165, 0x0206 };

    if (fsa->target != NULL) {
        d.x = fsa->target->globalPosition.x - self->globalPosition.x;
        d.y = fsa->target->globalPosition.y - self->globalPosition.y;
        d.z = fsa->target->globalPosition.z - self->globalPosition.z;
        fsa->targetDist = sqrtf(SQ(d.f[0]) + SQ(d.f[1]) + SQ(d.f[2]));
    }
    
    if (!(baddie->unk3B0 & 0x20)) {
        gDLL_33_BaddieControl->vtbl->func14(self, (Baddie*)fsa, &baddie->unk3B2, 2, 3, baddie->unk3A6, baddie->unk3A4);
    }
    
    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 8);
    objData->timeSinceHit += gUpdateRateF;

    if ((fsa->animState != SnowWorm_ASTATE_3) && 
        (gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, 
            &baddie->unk34C, baddie->unk39E, dHitAnimStateMap, dHitDamageMap, 1, &baddie->unk3A8, &fxTransform))
    ) {
        if (objData->timeSinceHit < 240.0f) {
            objData->consecutiveTicksHit++;
        } else {
            objData->consecutiveTicksHit = 0;
        }
        
        objData->timeSinceHit = 0.0f;
        
        if ((fsa->hitpoints > 0) && (objData->consecutiveTicksHit >= 2)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_3);
            objData->consecutiveTicksHit = 0;
            fsa->logicState = SnowWorm_LSTATE_5;
        }
        
        scaleIdx = ((DLL_251_Weapons*)player->linkedObject->dll)->vtbl->func19(player->linkedObject);
        if (scaleIdx > 3) {
            scaleIdx = 3;
        }
        
        fxTransform.scale = dFXScales1[scaleIdx];
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, &fxTransform, 0x200001, -1, NULL);
        if (scaleIdx != 0) {
            fxTransform.transl.f[0] -= self->globalPosition.f[0];
            fxTransform.transl.f[1] -= self->globalPosition.f[1];
            fxTransform.transl.f[2] -= self->globalPosition.f[2];
            fxTransform.scale = dFXScales2[scaleIdx];
            
            for (i = 0; i < 15; i++) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_324, &fxTransform, 2, -1, NULL);
            }
        }
    }
}

// offset: 0xCE8 | func: 11
void SnowWorm_func_CE8(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    SnowWorm_DataActual* objData;

    objData = baddie->objdata;
    objData->unk46 += gUpdateRate;
    
    if ((objData->unk46 >= 300)) {
        objData->unk46 = rand_next(0, 200);
        
        if (fsa->animState == SnowWorm_ASTATE_7 || fsa->animState == SnowWorm_ASTATE_8) {
            gDLL_6_AMSFX->vtbl->play(self, dBattleSounds[rand_next(3, 4)], 0x46, NULL, NULL, 0, NULL);
        }
    } 
    
    if (baddie->unk3B0 & 2) {
        gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.0f, -1);
    } else {
        gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.17f, -1);
    }
    
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    self->animObj = baddie->unk3AC;
}

// offset: 0xEBC | func: 12
void SnowWorm_func_EBC(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    Object* target;

    if (baddie->unk3B0 & 4) {
        target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, 55.0f, 0x8000);
    } else if (baddie->unk3B0 & 8) {
        target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2 * 0.5f, 0x8000);
    } else {
        target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, 0x8000);
    }
    
    if (target != NULL) {
        gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
        if (!(gDLL_33_BaddieControl->vtbl->func5(self, fsa, 75.0f) & 1)) {
            target = NULL;
        }
    }
    
    if (target != NULL) {
        gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 8, -1);
        fsa->unk33D = 0;
        fsa->target = target;
        baddie->unk3B6 = 1;
    }
}

// offset: 0x10D8 | func: 13
void SnowWorm_func_10D8(Object* self, Baddie* baddie) {
    SnowWorm_DataActual* objData;
    Vec3f v;
    MtxF mtx;
    f32 sp64;
    f32 var_fv0;
    SRT transform;

    objData = baddie->objdata;
    
    memcpy(&mtx, objGetAttachPointBoneMatrix(self, 1), sizeof(MtxF));
    mtx.m[3][0] = mtx.m[3][1] = mtx.m[3][2] = 0;
    
    if (self->id == OBJ_SnowWormLarge) {
        var_fv0 = 1.0f;
    } else {
        var_fv0 = 0.3f;
    }
    
    if (baddie->fsa.unk278 < var_fv0) {
        sp64 = var_fv0;
    } else {
        sp64 = baddie->fsa.unk278;
    }

    if (baddie->fsa.animState != SnowWorm_ASTATE_4) {
        objGetAttachPointWorldSpace(self, 2, &objData->fxTransform.transl.x, &objData->fxTransform.transl.y, &objData->fxTransform.transl.z, 0);
    } else {
        objGetAttachPointWorldSpace(self, 0, &objData->fxTransform.transl.x, &objData->fxTransform.transl.y, &objData->fxTransform.transl.z, 0);
    }
    
    objData->fxTransform.transl.f[1] = self->srt.transl.y + 8.0f;
    objData->fxTransform.transl.f[0] -= fsin16_precise(self->srt.yaw) * 10.0f * sp64;
    objData->fxTransform.transl.f[2] -= fcos16_precise(self->srt.yaw) * 10.0f * sp64;
    
    transform.transl.f[0] = 0.0f;
    transform.transl.f[1] = -15.0f;
    transform.transl.f[2] = -20.0f;
    
    objGetAttachPointWorldSpace(self, 0, &transform.transl.x, &transform.transl.y, &transform.transl.z, 1);
    
    if (objData->flags & SnowWorm_FLAG_2) {
        v.f[0] = -8.0f;
        v.f[1] = 40.0f;
        v.f[2] = -20.0f;
        vec3_transform(&mtx, v.f[0], v.f[1], v.f[2], &v.x, &v.y, &v.z);
        memcpy(&objData->throwVelocity, &v, sizeof(Vec3f));
        memcpy(&objData->throwOrigin, &transform, sizeof(SRT));
        objData->flags |= SnowWorm_FLAG_1;
    }
}

// offset: 0x1374 | func: 14
void SnowWorm_func_1374(Object* self, Baddie* baddie) {
    SnowWorm_DataActual* objData;
    s32 i;
    SRT* fxTransform;
    u8* colourRGB;
    f32 shakeFactor;

    objData = baddie->objdata;
    
    if (self->id == OBJ_SnowWormLarge) {
        objData->fxTransform.scale = 1.7f;
        shakeFactor = 2.0f;
    } else {
        objData->fxTransform.scale = 1.0f;
        shakeFactor = 1.0f;
    }
    
    i = 0;
    if (baddie->fsa.unk4.mode != 0) {
        i = dTerrainFXColourIndices[baddie->fsa.unk4.unk68.unk50[0]];
        if (i > 30) { //@bug? Should this be `i >= 5` (ARRAYCOUNT(dParticleColours))?
            i = 0;
        }
    }
    
    colourRGB = dParticleColours[i];
    fxTransform = &objData->fxTransform;
    
    if (objData->flags & SnowWorm_FLAG_1) {
        SnowWorm_throw_ice_ball(self, objData);
        objData->flags &= ~SnowWorm_FLAG_1;
    }
    
    if ((objData->flags & SnowWorm_FLAG_4) && !(baddie->unk3B0 & 0x40)) {
        for (i = 0; i < 4; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_56, fxTransform, 0x200001, -1, colourRGB);
        }
    }
    
    if ((objData->flags & SnowWorm_FLAG_8) && !(baddie->unk3B0 & 0x40)) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_57, fxTransform, 0x200001, -1, colourRGB);
    }
    
    if (objData->flags & SnowWorm_FLAG_10) {
        camUseShake();
        camSetShakeOffset(2.0f * shakeFactor);
        
        for (i = 0; i < 40; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_57, fxTransform, 0x200001, -1, colourRGB);
        }
    }
    
    if (objData->flags & SnowWorm_FLAG_20) {
        camUseShake();
        camSetShakeOffset(3.0f * shakeFactor);
        
        for (i = 0; i < 40; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_57, fxTransform, 0x200001, -1, colourRGB);
        }
        
        for (i = 0; i < 10; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_58, fxTransform, 0x200001, -1, colourRGB);
        }
    }
    
    objData->flags = SnowWorm_FLAG_0;
}

// offset: 0x16C4 | func: 15
/**
  * Creates an `IceBall` Object, thrown at the player.
  */
void SnowWorm_throw_ice_ball(Object* self, SnowWorm_DataActual* objData) {
    ChukaChuck_Setup* iceSetup;
    Object* ice;

    iceSetup = objAllocSetup(sizeof(ChukaChuck_Setup), OBJ_IceBall);
    iceSetup->base.x = objData->throwOrigin.transl.x;
    iceSetup->base.y = objData->throwOrigin.transl.y;
    iceSetup->base.z = objData->throwOrigin.transl.z;
    iceSetup->base.loadFlags = OBJSETUP_LOAD_LEVEL;
    iceSetup->base.fadeFlags = OBJSETUP_FADE_MANUAL;
    iceSetup->base.loadDistance = 0xFF;
    iceSetup->base.fadeDistance = 0xFF;
    iceSetup->unk1E = -1;
    iceSetup->unk20 = -1;
    ice = objSetupObject(&iceSetup->base, (OBJINIT_STANDALONE | OBJINIT_FLAG4), -1, -1, NULL);
    
    if (ice != NULL) {
        ice->velocity.x = objData->throwVelocity.x;
        ice->velocity.y = objData->throwVelocity.y;
        ice->velocity.z = objData->throwVelocity.z;
        ice->unkC4 = self;
    }
}

// offset: 0x1790 | func: 16
s32 SnowWorm_anim_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 pad[2];
    SnowWorm_DataActual *objData;
    Baddie *baddie;
    
    baddie = self->data;
    objData = baddie->objdata;
    
    self->objhitInfo->unk58 |= 1;
    
    fsa->unk4.mode = 1;
    self->objhitInfo->unk5F = Damage_Type_9;
    self->objhitInfo->unk60 = 1;
    
    func_80028D2C(self);
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 8, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        mainSetBits(baddie->unk39E, 1);
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        self->opacity = OBJECT_OPACITY_MAX;
        fsa->unk341 = 1;
        fsa->animTickDelta = (baddie->unk3B8 / 50000.0f) + 0.0025f;
    }
    
    if (fsa->unk33A) {
        baddie->unk3B6 = 1;
    }
    
    if (fsa->unk308 & 0x200) {
        fsa->unk308 &= ~0x200;
        objData->flags |= SnowWorm_FLAG_20;
    }
    
    objData->flags |= SnowWorm_FLAG_4;
    if (self->animProgress < 0.4f) {
        objData->flags |= SnowWorm_FLAG_8;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, rand_next(0, 1), dRoarSounds);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, dBattleSounds);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    
    return 0;
}

// offset: 0x19F4 | func: 17
s32 SnowWorm_anim_state_1(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 pad;
    Baddie* baddie;
    SnowWorm_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    self->objhitInfo->unk58 |= 1;
    fsa->unk4.mode = 1;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0xB, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        mainSetBits(baddie->unk39E, 1);
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        self->opacity = OBJECT_OPACITY_MAX;
        fsa->unk341 = 1;
        fsa->animTickDelta = (baddie->unk3B8 / 10000.0f) + 0.012f;
    }
    
    if (fsa->unk33A) {
        baddie->unk3B6 = 1;
    }

    if (fsa->unk308 & 0x200) {
        fsa->unk308 &= ~0x200;
        objData->flags |= SnowWorm_FLAG_20;
    }

    objData->flags |= 4;
    if (self->animProgress < 0.4f) {
        objData->flags |= SnowWorm_FLAG_8;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, rand_next(0, 1), dRoarSounds);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, dBattleSounds);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    
    return 0;
}

// offset: 0x1C34 | func: 18
s32 SnowWorm_anim_state_2(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SnowWorm_DataActual* objData;

    baddie = self->data;
    
    if ((fsa->prevAnimState != 4) && fsa->enteredAnimState) {
        objAnimSet(self, 0xE, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    objData = baddie->objdata;
    objData->flags |= (SnowWorm_FLAG_4 | SnowWorm_FLAG_8);
    
    if (fsa->enteredAnimState) {
        self->objhitInfo->unk58 &= ~1;
        fsa->animTickDelta = 0.01f;
        fsa->unk278 = 0.0f;
    }
    
    if (fsa->unk33A) {
        mainSetBits(baddie->unk39E, 0);
        objAnimSet(self, 8, 0.0f, 0);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        baddie->unk3B6 = 0;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, rand_next(3, 4), dBattleSounds);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, dBattleSounds);
    
    return 0;
}

// offset: 0x1DEC | func: 19
s32 SnowWorm_anim_state_3(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 pad[2];
    SnowWorm_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;
    self->objhitInfo->unk5F = Damage_Type_Sword_Staff_Strike1;
    self->objhitInfo->unk60 = 1;

    func_80028D2C(self);
    
    if (fsa->enteredAnimState) {
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 4, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.008f;
    if (fsa->unk308 & 0x200) {
        fsa->unk308 &= ~0x200;
        objData->flags |= SnowWorm_FLAG_10;
    }
    objData->flags |= (SnowWorm_FLAG_4 | SnowWorm_FLAG_8);
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, rand_next(0, 1), dHurtSounds);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, dBattleSounds);
    
    return 0;
}

// offset: 0x1F74 | func: 20
s32 SnowWorm_anim_state_4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SnowWorm_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 9, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    objData = baddie->objdata;
    objData->flags |= (SnowWorm_FLAG_4 | SnowWorm_FLAG_8);

    if (fsa->enteredAnimState) {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        baddie->unk3B6 = 4;
    }
    
    self->srt.yaw += ((fsa->unk32A * gUpdateRateF) / 12.0f) * 182.0f;
    
    fsa->animTickDelta = 0.01f;
    fsa->unk278 = 1.0f;
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, dBattleSounds);
    
    return 0;
}


// offset: 0x20B4 | func: 21
s32 SnowWorm_anim_state_5(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SnowWorm_DataActual* objData;
    /*0x130*/ static u8 dRoarSoundIdxA = 0;

    baddie = self->data;
    objData = baddie->objdata;
    
    objData->flags |= SnowWorm_FLAG_4;
    
    self->objhitInfo->unk5F = Damage_Type_Sword_Staff_Strike1;
    self->objhitInfo->unk60 = 1;

    func_80028D2C(self);
    if (fsa->enteredAnimState) {
        if (rand_next(0, 1)) {
            dRoarSoundIdxA = rand_next(0, 2);
            if (fsa->enteredAnimState) {
                objAnimSet(self, 6, 0.0f, 0);
                fsa->unk33A = 0;
            }
        } else {
            dRoarSoundIdxA = 3;
            if (fsa->enteredAnimState) {
                objAnimSet(self, 0xA, 0.0f, 0);
                fsa->unk33A = 0;
            }
        }
        
        fsa->unk341 = 1;
        fsa->animTickDelta = (baddie->unk3B8 / 20000.0f) + 0.005f;
    }
    
    if ((baddie->unk3B8 >= 0x33) && !(baddie->unk3B0 & 2)) {
        if ((fsa->targetDist > 55.0f) && (fsa->unk33A == 0)) {
            fsa->unk278 = (fsa->targetDist / 55.0f) - 1.0f;
            fsa->unk278 *= baddie->unk3B8 / 50.0f;
        } else {
            fsa->unk278 = 0.0f;
        }
    } else {
        fsa->unk278 = 0.0f;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, dRoarSoundIdxA, dRoarSounds);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    
    return 0;
}


// offset: 0x2354 | func: 22
s32 SnowWorm_anim_state_6(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SnowWorm_DataActual* objData;
    /*0x134*/ static u8 dRoarSoundIdxB = 0;

    baddie = self->data;
    objData = baddie->objdata;
    
    objData->flags |= SnowWorm_FLAG_4;
    self->objhitInfo->unk5F = Damage_Type_Sword_Staff_Strike1;
    self->objhitInfo->unk60 = 1;
    
    func_80028D2C(self);
    
    if (fsa->enteredAnimState) {
        dRoarSoundIdxB = rand_next(0, 2);
        if (rand_next(0, 1)) {
            if (fsa->enteredAnimState) {
                objAnimSet(self, 7, 0.0f, 0);
                fsa->unk33A = 0;
            }
        } else if (fsa->enteredAnimState) {
            objAnimSet(self, 3, 0.0f, 0);
            fsa->unk33A = 0;
        }
        
        fsa->unk341 = 1;
        fsa->animTickDelta = (baddie->unk3B8 / 20000.0f) + 0.005f;
    }
    
    if ((baddie->unk3B8 >= 0x33) && !(baddie->unk3B0 & 2)) {
        if ((fsa->targetDist > 55.0f) && (fsa->unk33A == 0)) {
            fsa->unk278 = (fsa->targetDist / 55.0f) - 1.0f;
            fsa->unk278 *= baddie->unk3B8 / 50.0f;
        } else {
            fsa->unk278 = 0.0f;
        }
    } else {
        fsa->unk278 = 0.0f;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, dRoarSoundIdxB, dRoarSounds);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, dRoarSoundIdxB, dRoarSounds);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    
    return 0;
}

// offset: 0x2620 | func: 23
s32 SnowWorm_anim_state_7(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    SnowWorm_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;
    objData->flags |= (SnowWorm_FLAG_4 | SnowWorm_FLAG_8);
    
    if (fsa->enteredAnimState) {
        if (fsa->enteredAnimState) { //?
            objAnimSet(self, 0xF, 0.0f, 0);
            fsa->unk33A = 0;
        }
        fsa->unk341 = 1;
    }
    
    fsa->animTickDelta = fsa->targetDist / 5000.0f;
    if (fsa->animTickDelta > 0.02f) {
        fsa->animTickDelta = 0.02f;
    } else if (fsa->animTickDelta < 0.01f) {
        fsa->animTickDelta = 0.01f;
    }

    if (self->animProgress < 0.5f) {
        fsa->unk278 = self->animProgress * 4.0f;
    } else {
        fsa->unk278 = (1.0f - self->animProgress) * 4.0f;
    }
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    
    return 0;
}

// offset: 0x2778 | func: 24
s32 SnowWorm_anim_state_8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    SnowWorm_DataActual* objData = baddie->objdata;

    objData->flags |= SnowWorm_FLAG_4;

    fsa->animTickDelta = 0.01f;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 5, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    
    return 0;
}

// offset: 0x2834 | func: 25
s32 SnowWorm_anim_state_9(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    SnowWorm_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;
    objData->flags |= SnowWorm_FLAG_4;
    fsa->animTickDelta = 0.01f;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0xA, 0.0f, 0);
        fsa->unk33A = 0;
    }

    fsa->unk341 = 1;
    
    if (fsa->unk308 & 1) {
        objData = baddie->objdata;
        fsa->unk308 &= ~1;
        objData->flags |= SnowWorm_FLAG_2;
        gDLL_6_AMSFX->vtbl->play(self, dRoarSounds[3], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    
    return 0;
}

// offset: 0x2964 | func: 26
s32 SnowWorm_anim_state_10(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    SnowWorm_DataActual* objData;
    Baddie* baddie;
    Object* weapon;

    baddie = self->data;
    objData = baddie->objdata;
    objData->flags |= SnowWorm_FLAG_4;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0, 0.0f, 0U);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        weapon = objGetPlayer()->linkedObject;
        if (((DLL_Unknown*)weapon->dll)->vtbl->func[16].withOneArgS32(weapon)) {
            gDLL_6_AMSFX->vtbl->play(self, dWeaponHitSounds[rand_next(3, 4)], MAX_VOLUME, NULL, NULL, 0, NULL);
        } else {
            gDLL_6_AMSFX->vtbl->play(self, dWeaponHitSounds[rand_next(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        gDLL_6_AMSFX->vtbl->play(self, dHurtSounds[rand_next(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.015f;
    fsa->unk278 = 0.0f;
    
    return 0;
}

// offset: 0x2B64 | func: 27
s32 SnowWorm_anim_state_11(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    SnowWorm_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        
        if (fsa->enteredAnimState) {
            objAnimSet(self, 2, 0.0f, 0);
            fsa->unk33A = 0;
        }
        
        baddie->unk3B6 = 2;
        fsa->unk341 = 1;
        fsa->animTickDelta = 0.015f;
    } else if (fsa->unk33A) {
        baddie->unk3B6 = 3;
    }
    
    objData = baddie->objdata;
    objData->flags |= SnowWorm_FLAG_4;

    if (fsa->unk308 & 0x200) {
        fsa->unk308 &= ~0x200;
        objData->flags |= SnowWorm_FLAG_10;
    }
    
    objData->flags |= (SnowWorm_FLAG_4 | SnowWorm_FLAG_8);
    fsa->unk278 = self->animProgress;
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, dBattleSounds);
    
    return 0;
}

// offset: 0x2CA4 | func: 28
s32 SnowWorm_anim_state_12(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SnowWorm_DataActual* objData;

    baddie = self->data;
    
    self->objhitInfo->unk5F = Damage_Type_Sword_Staff_Strike1;
    self->objhitInfo->unk60 = 1;
    func_80028D2C(self);
    
    if (baddie->unk3B8 > 50) {
        if (fsa->enteredAnimState) {
            objAnimSet(self, 4, 0.0f, 0);
            fsa->unk33A = 0;
        }
    } else if (fsa->enteredAnimState) {
        objAnimSet(self, 0xE, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.008f;
    
    objData = baddie->objdata;
    objData->flags |= (SnowWorm_FLAG_4 | SnowWorm_FLAG_8);
    
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    if ((baddie->unk3B0 & 2) == FALSE) {
        fsa->unk278 = self->animProgress + -1.0f;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, rand_next(0, 1), dHurtSounds);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, dBattleSounds);
    
    return 0;
}

// offset: 0x2E78 | func: 29
s32 SnowWorm_anim_state_13(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.008f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 1, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if ((fsa->unk34A & 1) == FALSE) {
        gDLL_6_AMSFX->vtbl->play(self, dWeaponHitSounds[rand_next(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play(self, SOUND_B21_Dissipating_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play(self, dHurtSounds[2], MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 1;
    }
    
    if (((fsa->unk34A & 2) == FALSE) && (self->animProgress > 0.3f)) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_B1F_Slow_Magic_Chimes, MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 2;
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, -1, 0);
    }
    
    return 0;
}

// offset: 0x309C | func: 30
s32 SnowWorm_logic_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->target != NULL) {
        if (fsa->enteredLogicState) {
            fsa->unk27C = 0.0f;
            fsa->unk278 = 0.0f;
            if (baddie->unk3B8 > 50) {
                if ((fsa->targetDist < (baddie->unk3E2 * 0.5f)) || (baddie->unk3B0 & 2)) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_0);
                } else {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_1);
                }
            } else {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_1);
            }
        }
        
        if (fsa->unk33A) {
            gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
            if (!(gDLL_33_BaddieControl->vtbl->func5(self, fsa, 75.0f) & 1)) {
                return FSA_NEXTSTATE_SYNC(SnowWorm_LSTATE_4);
            }

            if ((fsa->targetDist < (baddie->unk3E2 * 0.5f)) || (baddie->unk3B0 & 2)) {
                return FSA_NEXTSTATE_SYNC(SnowWorm_LSTATE_7);
            } else {
                return FSA_NEXTSTATE_SYNC(SnowWorm_LSTATE_6);
            }
        }
    }
    
    return 0;
}

// offset: 0x32E4 | func: 31
s32 SnowWorm_logic_state_1(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->hitpoints <= 0) {
        return FSA_NEXTSTATE_SYNC(SnowWorm_LSTATE_2_Dying);
    }
    
    if (fsa->unk33A) {
        if (fsa->animState == SnowWorm_ASTATE_12) {
            if (baddie->unk3B8 > 50) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_0);
            } else {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_1);
            }
        } else {
            return FSA_NEXTSTATE_SYNC(SnowWorm_LSTATE_7);
        }
    }
    
    return 0;
}

// offset: 0x33A8 | func: 32
s32 SnowWorm_logic_state_2_dying(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_13);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        func_800267A4(self);
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    } else if (fsa->unk33A) {
        objSendMesgMany(0, 3, self, 0xE0000, self);
        if (self->setup == NULL) {
            objFreeObject(self);
        }
        
        return FSA_NEXTSTATE_SYNC(SnowWorm_LSTATE_3);
    }

    return 0;
}

// offset: 0x34A0 | func: 33
s32 SnowWorm_logic_state_3(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    if (fsa->enteredLogicState) {
        baddie = self->data;
        baddie->unk3B4 = 0;
        mainSetBits(baddie->unk39E, 0);
        mainSetBits(baddie->unk39C, 1);
    }
    
    return 0;
}

// offset: 0x3524 | func: 34
s32 SnowWorm_logic_state_4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_2);
    }
    
    return 0;
}

// offset: 0x3580 | func: 35
s32 SnowWorm_logic_state_5(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_3);
    }
    
    if (fsa->unk33A) {
        if (fsa->animState == SnowWorm_ASTATE_3) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_0);
        } else {
            return FSA_NEXTSTATE_SYNC(SnowWorm_LSTATE_7);
        }
    }
    
    return 0;
}

// offset: 0x3638 | func: 36
s32 SnowWorm_logic_state_6(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    Unk80009024* sp3C;

    baddie = self->data;
    
    if ((fsa->unk33A != 0) && !(gDLL_33_BaddieControl->vtbl->func5(self, fsa, 75.0f) & 1)) {
        return FSA_NEXTSTATE_SYNC(SnowWorm_LSTATE_4);
    }
    
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_11);
    } else if (baddie->unk3B6 == 3) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_4);
    } else if (baddie->unk3B6 == 4) {
        if ((fsa->targetDist < 110.0f) && fsa->unk33A) {
            if (baddie->unk3B8 >= 0x33) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_0);
            } else {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_1);
            }
        }
    } else if (baddie->unk3B6 == 1) {
        return FSA_NEXTSTATE_SYNC(SnowWorm_LSTATE_7);
    }
    
    fsa->xAnalogInput = 0.0f;
    fsa->yAnalogInput = 0.0f;
    
    sp3C = &baddie->unk34C;
    
    bcopy(&self->srt.transl, sp3C, sizeof(Vec3f));
    bcopy(&fsa->target->srt.transl, &baddie->unk34C.unkC, sizeof(Vec3f));
    vox_func_80009024(sp3C, &baddie->unk374);
    
    if (sp3C->unk25 == 0) {
        gDLL_18_objfsa->vtbl->func6(self, fsa, sp3C->unk18.x, sp3C->unk18.z, 0.0f, 0.0f, 60.0f);
    } else {
        gDLL_18_objfsa->vtbl->func6(self, fsa, sp3C->unk18.x, sp3C->unk18.z, 15.0f, 30.0f, 60.0f);
    }
    
    if (fsa->logicStateTime > 120) {
        if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2, 1)) {
            return FSA_NEXTSTATE_SYNC(SnowWorm_LSTATE_4);
        }
    }

    return 0;
}

// offset: 0x396C | func: 37
s32 SnowWorm_logic_state_7(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SnowWorm_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredLogicState) {
        if ((s32) fsa->targetDist > 55) {
            if ((baddie->unk3B0 & 2) == FALSE) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_7);
            } else {
                objData = baddie->objdata;
                if (baddie->unk3B0 & 0x10) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, dAnimStateSequenceB[objData->unk4++]);
                } else {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, dAnimStateSequenceA[objData->unk4++]);
                }
                if (objData->unk4 >= ARRAYCOUNT_S(dAnimStateSequenceB)) {
                    objData->unk4 = 0;
                }
            }
        } else if (fsa->animState == SnowWorm_ASTATE_6) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_5);
        } else {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_6);
        }
        
    } else if (fsa->unk33A) {
        if ((gDLL_33_BaddieControl->vtbl->func5(self, fsa, 75.0f) & 1) == FALSE) {
            return FSA_NEXTSTATE_SYNC(SnowWorm_LSTATE_4);
        }
        
        if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2, 1)) {
            return FSA_NEXTSTATE_SYNC(SnowWorm_LSTATE_4);
        }
        
        if ((s32) fsa->targetDist > 55) {
            if ((baddie->unk3B0 & 2) == FALSE) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_7);
            } else {
                objData = baddie->objdata;
                if (baddie->unk3B0 & 0x10) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, dAnimStateSequenceB[objData->unk4++]);
                } else {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, dAnimStateSequenceA[objData->unk4++]);
                }
                if (objData->unk4 >= ARRAYCOUNT_S(dAnimStateSequenceB)) {
                    objData->unk4 = 0;
                }
            }
        } else if (fsa->animState == SnowWorm_ASTATE_6) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_5);
        } else {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_6);
        }
        
    } else if ((fsa->animState == SnowWorm_ASTATE_7) && ((s32) fsa->targetDist < 55)) {
        if (fsa->animState == SnowWorm_ASTATE_6) {
            //Unreachable?
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_5);
        } else {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWorm_ASTATE_6);
        }
    }

    return 0;
}
