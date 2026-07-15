//"baby_snowworm.c" appears to have been Rare's filename

#include "common.h"
#include "dlls/objects/221_ChukaChuck.h"
#include "dlls/objects/222_SnowWormSmall.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/gfx/model.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

typedef struct {
    Baddie_Setup baddie;
} SnowWormSmall_Setup;

typedef struct {
    f32 soundTimer;
    f32 soundTimerThreshold;
    u8 flags;
    u8 unk9;
} SnowWormSmall_DataActual;

typedef struct {
    Baddie baddie;
    SnowWormSmall_DataActual objData;
} SnowWormSmall_Data;

typedef enum {
    SnowWormSmall_ASTATE_0,
    SnowWormSmall_ASTATE_1,
    SnowWormSmall_ASTATE_2,
    SnowWormSmall_ASTATE_3,
    SnowWormSmall_ASTATE_4,
    SnowWormSmall_ASTATE_5,
    SnowWormSmall_ASTATE_6
} SnowWormSmall_AnimStates;

typedef enum {
    SnowWormSmall_LSTATE_0,
    SnowWormSmall_LSTATE_1,
    SnowWormSmall_LSTATE_2,
    SnowWormSmall_LSTATE_3,
    SnowWormSmall_LSTATE_4,
    SnowWormSmall_LSTATE_5
} SnowWormSmall_LogicStates;

typedef enum {
    SnowWormSmall_FLAG_0 = 0,
    SnowWormSmall_FLAG_1 = 1,
    SnowWormSmall_FLAG_2 = 2,
    SnowWormSmall_FLAG_4 = 4,
    SnowWormSmall_FLAG_8 = 8
} SnowWormSmall_Flags;

/*0x0*/ static u32 dHurtSounds[] = {
    SOUND_5FF_Cry_Hurt, SOUND_600_Cry_Hurt, SOUND_601_Worm_Croak
};
/*0xC*/ static u32 dChirpSounds[] = {
    SOUND_5FB_Chirp, SOUND_5FC_Triple_Chirp, SOUND_5FD_Trill, SOUND_5FE_Squeak
};
/*0x1C*/ static u32 dBattleSounds[] = {
    SOUND_602_Emerge_Snowy, SOUND_33A_Worm_Roar, SOUND_33C_Hiss, SOUND_33D_Hiss_Long
};
/*0x2C*/ static u32 dWeaponHitSounds[] = {
    SOUND_374_Whack, 
    SOUND_375_Smack1, 
    SOUND_376_Smack2, 
    SOUND_25B_Magic_Attack_Deflected, 
    SOUND_25C_Melee_Attack_Deflected
};
/*0x40*/ static s32 dHitAnimStateMap[] = {
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_2, //NOTE: different!
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5, 
    SnowWormSmall_ASTATE_5
};

/*0xB0*/ static s8 dHitDamageMap[] = {
    -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1
};

/*0x0*/ static ObjFSA_StateCallback sAnimStateCallbacks[7];
/*0x20*/ static ObjFSA_StateCallback sLogicStateCallbacks[6];

static void SnowWormSmall_init_fsa_callbacks(void);
static void SnowWormSmall_func_8B8(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void SnowWormSmall_func_BF8(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void SnowWormSmall_func_D0C(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void SnowWormSmall_func_F50(Object* self, Baddie* baddie);
static void SnowWormSmall_throw_ice_ball(Object* self, Baddie* baddie);
static s32 SnowWormSmall_anim_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWormSmall_anim_state_1(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWormSmall_anim_state_2(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWormSmall_anim_state_3(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWormSmall_anim_state_4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWormSmall_anim_state_5(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWormSmall_anim_state_6(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWormSmall_logic_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWormSmall_logic_state_1(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWormSmall_logic_state_2(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWormSmall_logic_state_3(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWormSmall_logic_state_4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SnowWormSmall_logic_state_5(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
void SnowWormSmall_init_fsa_callbacks(void) {
    sAnimStateCallbacks[SnowWormSmall_ASTATE_0] = SnowWormSmall_anim_state_0;
    sAnimStateCallbacks[SnowWormSmall_ASTATE_1] = SnowWormSmall_anim_state_1;
    sAnimStateCallbacks[SnowWormSmall_ASTATE_2] = SnowWormSmall_anim_state_2;
    sAnimStateCallbacks[SnowWormSmall_ASTATE_3] = SnowWormSmall_anim_state_3;
    sAnimStateCallbacks[SnowWormSmall_ASTATE_4] = SnowWormSmall_anim_state_4;
    sAnimStateCallbacks[SnowWormSmall_ASTATE_5] = SnowWormSmall_anim_state_5;
    sAnimStateCallbacks[SnowWormSmall_ASTATE_6] = SnowWormSmall_anim_state_6;
    
    sLogicStateCallbacks[SnowWormSmall_LSTATE_0] = SnowWormSmall_logic_state_0;
    sLogicStateCallbacks[SnowWormSmall_LSTATE_1] = SnowWormSmall_logic_state_1;
    sLogicStateCallbacks[SnowWormSmall_LSTATE_2] = SnowWormSmall_logic_state_2;
    sLogicStateCallbacks[SnowWormSmall_LSTATE_3] = SnowWormSmall_logic_state_3;
    sLogicStateCallbacks[SnowWormSmall_LSTATE_4] = SnowWormSmall_logic_state_4;
    sLogicStateCallbacks[SnowWormSmall_LSTATE_5] = SnowWormSmall_logic_state_5;
}

// offset: 0xC0 | ctor
void SnowWormSmall_ctor(void* dll) {
    SnowWormSmall_init_fsa_callbacks();
}

// offset: 0x100 | dtor
void SnowWormSmall_dtor(void* dll) { }

// offset: 0x10C | func: 1 | export: 0
void SnowWormSmall_setup(Object* self, SnowWormSmall_Setup* objSetup, s32 reset) {
    u8 flags;
    SnowWormSmall_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    
    flags = 2 | 4;
    if (reset) {
        flags = 1 | 2 | 4;
    }
    if ((objSetup->baddie.unk2B & 0x20) == FALSE) {
        flags |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, &objSetup->baddie, baddie, 7, 6, 0x102, flags, 20.0f);
    
    self->animCallback = NULL;
    
    objData = baddie->objdata;
    objData->soundTimer = mathRnd(10, 300);

    objAnimSet(self, 8, 0.0f, 0);
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, SnowWormSmall_ASTATE_0);
    baddie->fsa.logicState = SnowWormSmall_LSTATE_0;
    baddie->fsa.unk4.mode = 0;
    func_800267A4(self);
}

// offset: 0x250 | func: 2 | export: 1
void SnowWormSmall_control(Object* self) {
    Baddie* baddie;
    SnowWormSmall_Setup* objSetup;
    ObjFSA_Data* fsa;
    f32 time;

    baddie = self->data;
    objSetup = (SnowWormSmall_Setup*)self->setup;
    fsa = &baddie->fsa;
    
    if (self->unkDC) {
        if (((baddie->fsa.logicState != SnowWormSmall_LSTATE_3) || (baddie->unk3B0 & SnowWormSmall_LSTATE_1)) && 
            (gDLL_29_Gplay->vtbl->did_time_expire(objSetup->baddie.base.uID))
        ) {
            gDLL_33_BaddieControl->vtbl->setup(self, &objSetup->baddie, baddie, 7, 6, 0x102, 0x26, 20.0f);
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
        self->srt.transl.x = objSetup->baddie.base.x;
        self->srt.transl.y = objSetup->baddie.base.y;
        self->srt.transl.z = objSetup->baddie.base.z;
        gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->baddie.unk2E, self, -1);
        self->unkE0 = 1;
        return;
    }
    
    if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 0) == 0) {
        baddie->unk3B6 = 0;
        return;
    }
    
    if ((baddie->unk3B0 & 0x10) && (gDLL_7_Newday->vtbl->func8(&time) == 0)) {
        baddie->unk3B6 = 0;
        return;
    }
    
    SnowWormSmall_func_8B8(self, baddie, fsa);
    
    if (baddie->unk3B6 == 0) {
        SnowWormSmall_func_D0C(self, baddie, fsa);
    } else {
        SnowWormSmall_func_BF8(self, baddie, fsa);
    }
    
    self->srt.transl.y = objSetup->baddie.base.y - 2.0f;
}

// offset: 0x4F0 | func: 3 | export: 2
void SnowWormSmall_update(Object* self) { }

// offset: 0x4FC | func: 4 | export: 3
void SnowWormSmall_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie = self->data;
    
    if (!visibility) {
        return;
    } 
    
    if (self->unkDC || !baddie->unk3B6) {
        return;
    }
    
    if (baddie->unk3E8) {
        objprintSetBlendColor(200, 0, 0, baddie->unk3E8);
    }
    
    objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    
    if (baddie->unk3B2 & 0x60) {
        if (baddie->unk3B2 & 0x20) {
            gDLL_32->vtbl->func2(self, 0x330, &baddie->unk3E8);
            gDLL_32->vtbl->func2(self, 0x330, &baddie->unk3E8);
        }
        gDLL_32->vtbl->func2(self, 0x32F, &baddie->unk3E8);
    }
}

// offset: 0x6E8 | func: 5 | export: 4
void SnowWormSmall_free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    objFreeObjectType(self, OBJTYPE_Baddie);

    if (self->linkedObject != NULL) {
        objFreeObject(self->linkedObject);
        self->linkedObject = NULL;
    }

    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
}

// offset: 0x78C | func: 6 | export: 5
u32 SnowWormSmall_get_model_flags(Object* self) {
    return MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x79C | func: 7 | export: 6
u32 SnowWormSmall_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(SnowWormSmall_Data);
}

// offset: 0x7B0 | func: 8 | export: 7
SnowWormSmall_AnimStates SnowWormSmall_get_anim_state(Object* self, s32 unused) {
    Baddie* baddie = self->data;
    return baddie->fsa.animState;
}

// offset: 0x7C4 | func: 9 | export: 8
void SnowWormSmall_receive_message(Object* self, u8 message, s32 unused) {
    Baddie* baddie;
    SnowWormSmall_DataActual* objData;

    baddie = self->data;
    
    switch (message) {
    case 0x80:
        objData = baddie->objdata;
        objData->unk9 |= 2;
        gDLL_6_AMSFX->vtbl->play(self, dBattleSounds[1], MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, SnowWormSmall_ASTATE_1);
        baddie->fsa.logicState = SnowWormSmall_LSTATE_4;
        baddie->fsa.enteredLogicState = TRUE;
        break;
    case 0x81:
        baddie->unk3B0 &= ~4;
        break;
    default:
        STUBBED_PRINTF("BADDIE:SnowWorm Unknown message [%d]\n", message);
        break;
    }
}

// offset: 0x8B8 | func: 10
void SnowWormSmall_func_8B8(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {    
    Vec3f d;
    s32 scaleIdx;
    s32 i;
    /*0xCC*/ s16 dFXScales1[] = { 0x0206, 0x0167, 0x0165, 0x0206 };
    /*0xD4*/ s16 dFXScales2[] = { 0x0206, 0x0167, 0x0165, 0x0206 };
    Object* player;
    Object* weapon;
    /*0x38*/ static SRT sFXTransform;
    
    player = objGetPlayer();
    if (fsa->target != NULL) {
        d.f[0] = fsa->target->globalPosition.x - self->globalPosition.x;
        d.f[1] = fsa->target->globalPosition.f[1] - self->globalPosition.f[1];
        d.f[2] = fsa->target->globalPosition.f[2] - self->globalPosition.f[2];
        fsa->targetDist = sqrtf(SQ(d.f[0]) + SQ(d.f[1]) + SQ(d.f[2]));
    }
    
    if ((baddie->unk3B0 & 0x20) == FALSE) {
        gDLL_33_BaddieControl->vtbl->func14(self, (Baddie*)fsa, &baddie->unk3B2, 2, 3, baddie->unk3A4, baddie->unk3A6);
    }
    
    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 8);
    
    if (gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, dHitAnimStateMap, dHitDamageMap, 1, &baddie->unk3A8, &sFXTransform)) {
        weapon = player->linkedObject;
        scaleIdx = ((DLL_Unknown*)weapon->dll)->vtbl->func[19].withOneArgS32(weapon);
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
    }
}

// offset: 0xBF8 | func: 11
void SnowWormSmall_func_BF8(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    SnowWormSmall_func_F50(self, baddie);
    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.0f, -1);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    self->animObj = baddie->unk3AC;
}

// offset: 0xD0C | func: 12
void SnowWormSmall_func_D0C(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    SnowWormSmall_DataActual* objData;
    Object* target;
    Vec3f d;
    Object* player;
    f32 distance;

    objData = baddie->objdata;
    
    target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, 0x8000);
    if ((target != NULL) && !(baddie->unk3B0 & 4)) {
        gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 8, -1);
        fsa->unk33D = 0;
        fsa->target = target;
        baddie->unk3B6 = 1;
        return;
    }
    
    player = objGetPlayer();
    if (player != NULL) {
        d.f[0] = player->globalPosition.x - self->globalPosition.x;
        d.f[1] = player->globalPosition.y - self->globalPosition.y;
        d.f[2] = player->globalPosition.z - self->globalPosition.z;
        distance = sqrtf(SQ(d.f[0]) + SQ(d.f[1]) + SQ(d.f[2]));
    } else {
        distance = 10000.0f;
    }
    
    //Play sound when nearby
    if ((objData->soundTimer > objData->soundTimerThreshold) && (distance < 400.0f)) {
        gDLL_6_AMSFX->vtbl->play(self, dBattleSounds[1], 0x1E, NULL, NULL, 0, NULL);
        objData->soundTimerThreshold += mathRnd(50, 250);
    }
    objData->soundTimer += gUpdateRateF;
}

// offset: 0xF50 | func: 13
void SnowWormSmall_func_F50(Object* self, Baddie* baddie) {
    SnowWormSmall_DataActual* objData;
    s32 i;

    objData = baddie->objdata;
    
    if (objData->flags & SnowWormSmall_FLAG_1) {
        SnowWormSmall_throw_ice_ball(self, baddie);
    }
    
    if (objData->flags & SnowWormSmall_FLAG_2) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_345, NULL, 1, -1, NULL);
    }
    
    if (objData->flags & SnowWormSmall_FLAG_4) {
        for (i = 0; i < 10; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_343, NULL, 1, -1, NULL);
        }
    }
    
    objData->flags = SnowWormSmall_FLAG_0;
}

// offset: 0x106C | func: 14
void SnowWormSmall_throw_ice_ball(Object* self, Baddie* baddie) {
    ChukaChuck_Setup* chuckSetup;
    Object* chuck;
    f32 throwFactor;
    f32 throwTime; //Duration of throw in frames

    chuckSetup = objAllocSetup(sizeof(ChukaChuck_Setup), OBJ_IceBallSmall);
    chuckSetup->base.x = self->srt.transl.x;
    chuckSetup->base.y = self->srt.transl.y + 15.0f;
    chuckSetup->base.z = self->srt.transl.z;
    chuckSetup->base.loadFlags = OBJSETUP_LOAD_LEVEL;
    chuckSetup->base.fadeFlags = OBJSETUP_FADE_MANUAL;
    chuckSetup->base.loadDistance = 0xFF;
    chuckSetup->base.fadeDistance = 0xFF;
    chuck = objSetupObject(&chuckSetup->base, (OBJINIT_STANDALONE | OBJINIT_FLAG4), -1, -1, NULL);
    
    if (chuck != NULL) {
        throwFactor = baddie->fsa.targetDist / baddie->unk3E2;

        //Different throw times based on PAL/NTSC
        if (osTvType == 1) {
            //NTSC
            throwTime = throwFactor * 60.0f;
        } else {
            //PAL
            throwTime = throwFactor * 50.0f;
        }
        
        chuck->velocity.x = (baddie->fsa.target->srt.transl.x - self->srt.transl.x) / throwTime;
        chuck->velocity.y = ((baddie->fsa.target->srt.transl.y + (90.0f * throwFactor)) - self->srt.transl.y) / throwTime;
        chuck->velocity.z = (baddie->fsa.target->srt.transl.z - self->srt.transl.z) / throwTime;
        chuck->unkC4 = self;
    }
}

// offset: 0x11E4 | func: 15
static void SnowWormSmall_get_ally_info(Object* self, s32* oAlliesLowestAnimState, s32* oNumAllies, s32* oNumAlliesInAnimState4) {
    s32 index;
    s32 count;
    Object* obj;
    Object** objects;
    s32 animState;

    *oNumAllies = 0;
    *oNumAlliesInAnimState4 = 0;
    *oAlliesLowestAnimState = 0;
    
    for (objects = objGetObjects(&index, &count); index < count; index++) {
        obj = objects[index];
        if ((self != obj) && (obj->id == OBJ_SnowWormSmall)) {
            *oNumAllies += 1;
            obj = objects[index];
            animState = ((DLL_222_SnowWormSmall*)obj->dll)->vtbl->get_anim_state(obj, 0);
            
            if (*oAlliesLowestAnimState < animState) {
                *oAlliesLowestAnimState = animState;
            }

            if (animState == SnowWormSmall_ASTATE_4) {
                *oNumAlliesInAnimState4 += 1;
            }
        }
    }
}

// offset: 0x132C | func: 16
static void SnowWormSmall_worm_to_worm_communication(Object* self) {
    s32 index;
    s32 count;
    Object* obj;
    Object** objects;

    for (objects = objGetObjects(&index, &count); index < count; index++) {
        obj = objects[index];
        if ((self != obj) && (obj->id == OBJ_SnowWormSmall)) {
            ((DLL_222_SnowWormSmall*)obj->dll)->vtbl->receive_message(obj, 0x81, 0);
        }
    }
}

// offset: 0x13F8 | func: 17
s32 SnowWormSmall_anim_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 pad;
    Baddie* baddie;
    SnowWormSmall_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0xB, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState != 0) {
        fsa->unk4.mode = 1;
        mainSetBits(baddie->unk39E, 1);
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        self->opacity = OBJECT_OPACITY_MAX;
        fsa->unk341 = 1;
        
        fsa->animTickDelta = (baddie->unk3B8 / 10000.0f) + 0.012f;
        func_8002674C(self);
    } else {
        func_80026128(self, Damage_Type_Sword_Staff_Strike1, 1, -1);
        self->objhitInfo->unk5D = 0xA;
        self->objhitInfo->unk5E = 1;
        func_80028D2C(self);
    }
    
    if (fsa->unk33A != 0) {
        baddie->unk3B6 = 1;
    }

    if (fsa->unk308 & 0x200) {
        fsa->unk308 &= ~0x200;
        objData->flags |= SnowWormSmall_FLAG_4;
    }
    
    if (self->animProgress < 0.7f) {
        objData->flags |= SnowWormSmall_FLAG_2;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, mathRnd(0, 1), dChirpSounds);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, dBattleSounds);
    
    return 0;
}

// offset: 0x164C | func: 18
s32 SnowWormSmall_anim_state_1(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SnowWormSmall_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0xE, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (self->animProgress > 0.25f) {
        objData = baddie->objdata;
        objData->flags |= SnowWormSmall_FLAG_2;
    }
    
    if (fsa->enteredAnimState) {
        func_800267A4(self);
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
        
        if ((objData->unk9 & 2) == FALSE) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
        }
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, dBattleSounds);
    
    return 0;
}

// offset: 0x17DC | func: 19
s32 SnowWormSmall_anim_state_2(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        func_8002674C(self);
    }
    
    func_80026128(self, Damage_Type_Sword_Staff_Strike1, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    
    if (fsa->enteredAnimState) {
        SnowWormSmall_worm_to_worm_communication(self);
        
        if (mathRnd(0, 1)) {
            if (fsa->enteredAnimState) {
                objAnimSet(self, 6, 0.0f, 0);
                fsa->unk33A = 0;
            }
        } else {
            if (fsa->enteredAnimState) {
                objAnimSet(self, 7, 0.0f, 0);
                fsa->unk33A = 0;
            }
        }
        
        fsa->unk341 = 1;
        fsa->animTickDelta = (baddie->unk3B8 / 20000.0f) + 0.005f;
    }
    
    fsa->unk278 = 0.0f;
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, mathRnd(0, 2), dChirpSounds);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, mathRnd(0, 2), dChirpSounds);
    
    return 0;
}

// offset: 0x1A1C | func: 20
s32 SnowWormSmall_anim_state_3(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_8002674C(self);
    }
    
    func_80026128(self, Damage_Type_Sword_Staff_Strike1, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    fsa->animTickDelta = 0.01f;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 5, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    
    return 0;
}

// offset: 0x1B04 | func: 21
s32 SnowWormSmall_anim_state_4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    SnowWormSmall_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        func_8002674C(self);
    }
    
    func_80026128(self, Damage_Type_Sword_Staff_Strike1, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    
    if (fsa->enteredAnimState) {
        SnowWormSmall_worm_to_worm_communication(self);
    }
    
    fsa->animTickDelta = 0.01f;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0xA, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    if (fsa->unk308 & 1) {
        objData = baddie->objdata;
        fsa->unk308 &= ~1;
        objData->flags |= SnowWormSmall_FLAG_1;
        gDLL_6_AMSFX->vtbl->play(self, dChirpSounds[3], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    return 0;
}

// offset: 0x1C80 | func: 22
s32 SnowWormSmall_anim_state_5(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Object* weapon;

    if (fsa->enteredAnimState) {
        objAnimSet(self, 0, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        SnowWormSmall_worm_to_worm_communication(self);

        weapon = objGetPlayer()->linkedObject;
        if (((DLL_Unknown*)weapon->dll)->vtbl->func[16].withOneArgS32(weapon)) {
            gDLL_6_AMSFX->vtbl->play(self, dWeaponHitSounds[mathRnd(3, 4)], MAX_VOLUME, NULL, NULL, 0, NULL);
        } else {
            gDLL_6_AMSFX->vtbl->play(self, dWeaponHitSounds[mathRnd(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        gDLL_6_AMSFX->vtbl->play(self, dHurtSounds[mathRnd(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.015f;
    fsa->unk278 = 0.0f;
    
    return 0;
}

// offset: 0x1E80 | func: 23
s32 SnowWormSmall_anim_state_6(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    baddie = self->data;
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.008f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 1, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if ((fsa->unk34A & 1) == FALSE) {
        gDLL_6_AMSFX->vtbl->play(self, dWeaponHitSounds[mathRnd(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play(self, SOUND_B21_Dissipating_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play(self, dHurtSounds[2], MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 1;
    }
    
    if (((fsa->unk34A & 2) == FALSE) && (self->animProgress > 0.3f)) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_B1F_Slow_Magic_Chimes, MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 2;
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, NO_GAMEBIT, 0);
    }
    
    return 0;
}

// offset: 0x20A4 | func: 24
s32 SnowWormSmall_logic_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->target != NULL) {
        if (fsa->enteredLogicState) {
            fsa->unk27C = 0.0f;
            fsa->unk278 = 0.0f;
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWormSmall_ASTATE_0);
        }
        
        if (fsa->unk33A) {
            return 6;
        }
    }
    
    return 0;
}

// offset: 0x2134 | func: 25
s32 SnowWormSmall_logic_state_1(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->hitpoints <= 0) {
        return 3;
    }
    if (fsa->unk33A != 0) {
        return 6;
    }
    return 0;
}

// offset: 0x2170 | func: 26
s32 SnowWormSmall_logic_state_2(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    SnowWormSmall_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    
    if (fsa->enteredLogicState) {
        objData = baddie->objdata;
        objData->soundTimer = 0.0f;
        objData->soundTimerThreshold = 0.0f;
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWormSmall_ASTATE_6);
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
        return 4;
    }
    
    return 0;
}

// offset: 0x227C | func: 27
s32 SnowWormSmall_logic_state_3(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    if (fsa->enteredLogicState) {
        baddie = self->data;
        baddie->unk3B4 = 0;

        if (baddie->unk39E != NO_GAMEBIT) {
            mainSetBits(baddie->unk39E, 0);
        }
        
        if (baddie->unk39C != NO_GAMEBIT) {
            mainSetBits(baddie->unk39C, 1);
        }
    }
    
    return 0;
}

// offset: 0x2318 | func: 28
s32 SnowWormSmall_logic_state_4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SnowWormSmall_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWormSmall_ASTATE_1);
        objData = baddie->objdata;
        objData->soundTimer = 0.0f;
        objData->soundTimerThreshold = 0.0f;
    }
    
    return 0;
}

// offset: 0x2390 | func: 29
s32 SnowWormSmall_logic_state_5(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SnowWormSmall_DataActual* objData;
    s32 random;
    s32 alliesLowestAnimState;
    s32 numAllies;
    s32 numAlliesInAnimState4;

    baddie = self->data;
    
    if (fsa->unk33A || fsa->enteredLogicState) {
        objData = baddie->objdata;

        if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2, 1)) {
            objData->unk9 &= ~2;
            return 5;
        }

        SnowWormSmall_get_ally_info(self, &alliesLowestAnimState, &numAllies, &numAlliesInAnimState4);
        
        random = mathRnd(0, baddie->unk3B8);
        
        if ((alliesLowestAnimState >= SnowWormSmall_ASTATE_5) || (objData->unk9 & 1)) {
            if (baddie->unk3B0 & 2) {
                objData->unk9 |= 1;
            }
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWormSmall_ASTATE_4);
        } else if (random > 32) {
            if (numAlliesInAnimState4 > 1) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWormSmall_ASTATE_2);
            } else {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWormSmall_ASTATE_4);
            }
        } else if (random > 16) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWormSmall_ASTATE_2);
        } else {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SnowWormSmall_ASTATE_3);
        }
    }
    
    return 0;
}
