#include "common.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/objects/221_ChukaChuck.h"
#include "dlls/objects/251_weapons.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/math.h"
#include "sys/objlib.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

typedef struct {
    f32 resetTimer;
    f32 resetTimeThreshold;
    f32 roarSoundTimer;
    f32 roarSoundInterval;
    union {
        struct {
            u32 pad10_19: 13;
            u32 unk10_18: 1;
            u32 pad10_15 : 3;
            u32 unk10_14 : 1;
            u32 pad10_2 : 10;
            u32 pad10_1 : 1;
            u32 pad10_0 : 1;
        };
        struct {
            u8 flags;
            u8 unk11;
            u8 unk12_0 : 1;
            u8 unk12_1 : 1;
            u8 unk13;
        };
    };
    Object* stolenEgg;
    u8 targetIdx;
    f32 unk1C;
} DBStealerWorm_DataActual;

typedef struct {
    Baddie baddie;
    DBStealerWorm_DataActual objData;
} DBStealerWorm_Data;

typedef enum {
    DBStealerWorm_ASTATE_0_Pop_Out_of_Ground,
    DBStealerWorm_ASTATE_1_Burst_Into_Ground,
    DBStealerWorm_ASTATE_2_Bite_Attack,
    DBStealerWorm_ASTATE_3_Stand_Still,
    DBStealerWorm_ASTATE_4_Stand_and_Spit,
    DBStealerWorm_ASTATE_5_Hit,
    DBStealerWorm_ASTATE_6_Dying,
    DBStealerWorm_ASTATE_7_Run_to_Object,
    DBStealerWorm_ASTATE_8_Pick_Up_Egg,
    DBStealerWorm_ASTATE_9_Throw_Egg
} DBStealerWorm_AnimStates;

typedef enum {
    DBStealerWorm_LSTATE_0_Top,
    DBStealerWorm_LSTATE_1_Hit,
    DBStealerWorm_LSTATE_2_Dying,
    DBStealerWorm_LSTATE_3_Dead,
    DBStealerWorm_LSTATE_4_Dormant,
    DBStealerWorm_LSTATE_5_Engage
} DBStealerWorm_LogicStates;

/*0x0*/ static u32 dHurtSounds[] = {
    SOUND_5FF_Cry_Hurt, 
    SOUND_61B, 
    SOUND_601_Worm_Croak
};
/*0xC*/ static u32 dChirpSounds[] = {
    SOUND_5FB_Chirp, 
    SOUND_5FC_Triple_Chirp, 
    SOUND_5FD_Trill, 
    SOUND_5FE_Squeak
};
/*0x1C*/ static u32 dBattleSounds[] = {
    SOUND_602_Emerge_Snowy, 
    SOUND_33A_Worm_Roar, 
    SOUND_33C_Hiss, 
    SOUND_33D_Hiss_Long
};
/*0x2C*/ static u32 dWeaponHitSounds[] = {
    SOUND_374_Whack, 
    SOUND_375_Smack1, 
    SOUND_376_Smack2, 
    SOUND_25B_Magic_Attack_Deflected, 
    SOUND_25C_Melee_Attack_Deflected
};
/*0x40*/ static s32 dHitAnimStateMap[] = {
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_2_Bite_Attack, //NOTE: different!
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit, 
    DBStealerWorm_ASTATE_5_Hit
};
/*0x9C*/ static s8 dHitDamageMap[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1
};
/*0xB4*/ static s32 dObjTypes[] = {
    OBJTYPE_Player, OBJTYPE_Sidekick, OBJTYPE_Baddie
};
/*0xC0*/ static f32 dAvoidScaleFactors[] = {
    2, 4, 1.5
};
/*0xCC*/ static u32 dTargetAnimStates[] = {
    DBStealerWorm_ASTATE_0_Pop_Out_of_Ground, 
    DBStealerWorm_ASTATE_7_Run_to_Object, 
    DBStealerWorm_ASTATE_8_Pick_Up_Egg, 
    DBStealerWorm_ASTATE_7_Run_to_Object, 
    DBStealerWorm_ASTATE_9_Throw_Egg, 
    DBStealerWorm_ASTATE_1_Burst_Into_Ground
};
/*0xE4*/ static s16 dTargetObjTypes[6] = {
    OBJTYPE_38, 
    OBJTYPE_38, 
    OBJTYPE_Player, 
    OBJTYPE_32, 
    OBJTYPE_Player, 
    OBJTYPE_Player
};
/*0xF0*/ static s32 data_F0[] = {
    0, 0, 0, 0,0,0, 0
};
/*0x10C*/ static f32 data_10C = 1.0f;

/*0x0*/ static ObjFSA_StateCallback sAnimStateCallbacks[10];
/*0x28*/ static ObjFSA_StateCallback sLogicStateCallbacks[6];

static void DBStealerWorm_func_72C(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void DBStealerWorm_func_A74(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void DBStealerWorm_func_BA0(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void DBStealerWorm_handle_spit_and_fx_flags(Object* self, Baddie* baddie);
static void DBStealerWorm_spit(Object* self, Baddie* baddie);

static s32 DBStealerWorm_anim_state_0_pop_out_of_ground(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 DBStealerWorm_anim_state_1_burst_into_ground(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 DBStealerWorm_anim_state_2_bite_attack(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 DBStealerWorm_anim_state_3_stand_still(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 DBStealerWorm_anim_state_4_stand_and_spit(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 DBStealerWorm_anim_state_5_hit(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 DBStealerWorm_anim_state_6_dying(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 DBStealerWorm_anim_state_7_run_to_object(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 DBStealerWorm_anim_state_8_pick_up_egg(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 DBStealerWorm_anim_state_9_throw_egg(Object* self, ObjFSA_Data* fsa, f32 updateRate);

static s32 DBStealerWorm_logic_state_0_top(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 DBStealerWorm_logic_state_1_hit(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 DBStealerWorm_logic_state_2_dying(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 DBStealerWorm_logic_state_3_dead(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 DBStealerWorm_logic_state_4_dormant(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 DBStealerWorm_logic_state_5_engage(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void DBStealerWorm_init_fsa_callbacks(void) {
    sAnimStateCallbacks[DBStealerWorm_ASTATE_0_Pop_Out_of_Ground] = DBStealerWorm_anim_state_0_pop_out_of_ground;
    sAnimStateCallbacks[DBStealerWorm_ASTATE_1_Burst_Into_Ground] = DBStealerWorm_anim_state_1_burst_into_ground;
    sAnimStateCallbacks[DBStealerWorm_ASTATE_2_Bite_Attack]       = DBStealerWorm_anim_state_2_bite_attack;
    sAnimStateCallbacks[DBStealerWorm_ASTATE_3_Stand_Still]       = DBStealerWorm_anim_state_3_stand_still;
    sAnimStateCallbacks[DBStealerWorm_ASTATE_4_Stand_and_Spit]    = DBStealerWorm_anim_state_4_stand_and_spit;
    sAnimStateCallbacks[DBStealerWorm_ASTATE_5_Hit]               = DBStealerWorm_anim_state_5_hit;
    sAnimStateCallbacks[DBStealerWorm_ASTATE_6_Dying]             = DBStealerWorm_anim_state_6_dying;
    sAnimStateCallbacks[DBStealerWorm_ASTATE_7_Run_to_Object]     = DBStealerWorm_anim_state_7_run_to_object;
    sAnimStateCallbacks[DBStealerWorm_ASTATE_8_Pick_Up_Egg]       = DBStealerWorm_anim_state_8_pick_up_egg;
    sAnimStateCallbacks[DBStealerWorm_ASTATE_9_Throw_Egg]         = DBStealerWorm_anim_state_9_throw_egg;
    
    sLogicStateCallbacks[DBStealerWorm_LSTATE_0_Top]     = DBStealerWorm_logic_state_0_top;
    sLogicStateCallbacks[DBStealerWorm_LSTATE_1_Hit]     = DBStealerWorm_logic_state_1_hit;
    sLogicStateCallbacks[DBStealerWorm_LSTATE_2_Dying]   = DBStealerWorm_logic_state_2_dying;
    sLogicStateCallbacks[DBStealerWorm_LSTATE_3_Dead]    = DBStealerWorm_logic_state_3_dead;
    sLogicStateCallbacks[DBStealerWorm_LSTATE_4_Dormant] = DBStealerWorm_logic_state_4_dormant;
    sLogicStateCallbacks[DBStealerWorm_LSTATE_5_Engage]  = DBStealerWorm_logic_state_5_engage;
}

// offset: 0xE4 | ctor
void DBStealerWorm_ctor(void* dll) {
    DBStealerWorm_init_fsa_callbacks();
}

// offset: 0x124 | dtor
void DBStealerWorm_dtor(void* dll) { }

// offset: 0x130 | func: 1 | export: 0
void DBStealerWorm_setup(Object* self, Baddie_Setup* objSetup, s32 reset) {
    Baddie* baddie;
    DBStealerWorm_DataActual* objData;
    u8 flags;

    baddie = self->data;

    flags = 2 | 4;
    if (reset) {
        flags = 1 | 2 | 4;
    }
    if (!(objSetup->unk2B & 0x20)) {
        flags |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 0xA, 6, 0x102, flags, 20.0f);
    
    objAddObjectType(self, OBJTYPE_Baddie);
    
    self->animCallback = NULL;
    
    objData = baddie->objdata;
    bzero(objData, sizeof(DBStealerWorm_DataActual));
    objData->resetTimeThreshold = (f32) (objSetup->unk2C * 60.0f);
    objData->roarSoundTimer = mathRnd(10, 300);
    
    objAnimSet(self, 8, 0.0f, 0);
    
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    
    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, DBStealerWorm_ASTATE_3_Stand_Still);
    baddie->fsa.logicState = DBStealerWorm_LSTATE_0_Top;
    baddie->fsa.unk4.mode = 1;
    
    func_8002674C(self);
    objInitMesgQueue(self, 4);
}

// offset: 0x2E8 | func: 2 | export: 1
void DBStealerWorm_control(Object* self) {
    Baddie* baddie;
    ObjFSA_Data* fsa;
    Baddie_Setup* objSetup;
    f32 time;

    baddie = self->data;
    fsa = &baddie->fsa;
    objSetup = (Baddie_Setup*)self->setup;
    
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    
    if (self->unkDC) {
        return;
    }
    
    if (self->unkE0 == 0) {
        self->srt.transl.f[0] = objSetup->base.x;
        self->srt.transl.f[1] = objSetup->base.y;
        self->srt.transl.f[2] = objSetup->base.z;
        gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->unk2E, self, -1);
        self->unkE0 = 1;
        return;
    }
    
    //Different behaviour if the Baddie's local Block is unloaded
    if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, FALSE) == FALSE) {
        baddie->unk3B6 = FALSE;
        return;
    }
    
    if (baddie->unk3B0 & 0x10) {
        //Different behaviour by day
        if (gDLL_7_Newday->vtbl->func8(&time) == FALSE) {
            baddie->unk3B6 = FALSE;
            return;
        }
    }
    
    DBStealerWorm_func_72C(self, baddie, fsa);

    //Branching behaviour
    if (baddie->unk3B6 == FALSE) {
        DBStealerWorm_func_BA0(self, baddie, fsa);
    } else {
        DBStealerWorm_func_A74(self, baddie, fsa);
    }
}

// offset: 0x474 | func: 3 | export: 2
void DBStealerWorm_update(Object* self) {
    gDLL_18_objfsa->vtbl->func2(self, self->data, sAnimStateCallbacks);
}

// offset: 0x4C4 | func: 4 | export: 3
void DBStealerWorm_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie;
    DBStealerWorm_DataActual* objData;
    Object* egg;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (objData->stolenEgg) {
        objData->stolenEgg->srt.transl.x = self->srt.transl.x;
        objData->stolenEgg->srt.transl.y = self->srt.transl.y;
        objData->stolenEgg->srt.transl.z = self->srt.transl.z;
        objData->stolenEgg->srt.transl.y += 30.0f;
    }
    
    if (visibility && (self->unkDC == 0) && baddie->unk3B6) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);

        egg = objData->stolenEgg;
        if ((egg != NULL) && !(objData->stolenEgg->stateFlags & OBJSTATE_DESTROYED)) {
            objGetAttachPointWorldSpace(self, 3, &egg->srt.transl.x, &egg->srt.transl.y, &egg->srt.transl.z, 0);
            objprintDrawModel(objData->stolenEgg, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0x614 | func: 5 | export: 4
void DBStealerWorm_free(Object* self, s32 onlySelf) {
    Baddie* baddie;

    baddie = self->data;
    objFreeObjectType(self, OBJTYPE_Baddie);

    if (self->linkedObject != NULL) {
        objFreeObject(self->linkedObject);
        self->linkedObject = NULL;
    }
    
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
}

// offset: 0x6B8 | func: 6 | export: 5
u32 DBStealerWorm_get_model_flags(Object* self) {
    return (MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_1);
}

// offset: 0x6C8 | func: 7 | export: 6
u32 DBStealerWorm_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DBStealerWorm_Data);
}

// offset: 0x6DC | func: 8 | export: 7
s16 DBStealerWorm_get_anim_state(Object* self, s32 unused) {
    Baddie* baddie = self->data;
    return baddie->fsa.animState;
}

// offset: 0x6F0 | func: 9 | export: 8
void DBStealerWorm_receive_message(Object* self, u8 message, s32 unused) {
    Baddie* baddie = self->data;

    switch (message) {
    case 0x80:
        break;
    case 0x81:
        baddie->unk3B0 &= ~4;
        break;
    default:
        //@typo: copy-pasted from SnowWorm, or a leftover after branching off from the SnowWorm DLL?
        STUBBED_PRINTF("BADDIE:SnowWorm Unknown message [%d]\n", message);
        break;
    }
}

// offset: 0x72C | func: 10
void DBStealerWorm_func_72C(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    Vec3f d;
    s32 scaleIdx;
    s32 i;
/*0x110*/ s16 dFXScales1[] = { 0x0206, 0x0167, 0x0165, 0x0206 };
/*0x118*/ s16 dFXScales2[] = { 0x0206, 0x0167, 0x0165, 0x0206 };
    Object* player;
/*40*/ static SRT sFXTransform;

    player = objGetPlayer();

    if (fsa->target != NULL) {
        d.f[0] = fsa->target->globalPosition.f[0] - self->globalPosition.f[0];
        d.f[1] = fsa->target->globalPosition.f[1] - self->globalPosition.f[1];
        d.f[2] = fsa->target->globalPosition.f[2] - self->globalPosition.f[2];
        fsa->targetDist = sqrtf(SQ(d.f[0]) + SQ(d.f[1]) + SQ(d.f[2]));
    }
    
    if (!(baddie->unk3B0 & 0x20)) {
        gDLL_33_BaddieControl->vtbl->func14(self, (Baddie*)fsa, &baddie->unk3B2, 2, 3, baddie->unk3A4, baddie->unk3A6);
    }
    
    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 8);
    if ((fsa->hitpoints > 0) && (gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, dHitAnimStateMap, dHitDamageMap, 1, &baddie->unk3A8, &sFXTransform) != 0)) {
        scaleIdx = ((DLL_251_Weapons*)player->linkedObject->dll)->vtbl->func19(player->linkedObject);
        if (scaleIdx >= 4) {
            scaleIdx = 3;
        }
        
        sFXTransform.scale = dFXScales1[scaleIdx];
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, &sFXTransform, 0x200001, -1, NULL);
        sFXTransform.transl.x -= self->srt.transl.x;
        sFXTransform.transl.y -= self->srt.transl.f[1];
        sFXTransform.transl.z -= self->srt.transl.f[2];
        sFXTransform.scale = dFXScales2[scaleIdx];
        
        for (i = 0; i < 4; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_324, &sFXTransform, 2, -1, NULL);
        }
    }
}

// offset: 0xA74 | func: 11
void DBStealerWorm_func_A74(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    DBStealerWorm_DataActual* objData = baddie->objdata;
    
    DBStealerWorm_handle_spit_and_fx_flags(self, baddie);
    
    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.17f, 1);
    
    if (!(objData->unk11 & 4)) {
        gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    }
    
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    self->animObj = baddie->unk3AC;
}

// offset: 0xBA0 | func: 12
void DBStealerWorm_func_BA0(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    DBStealerWorm_DataActual* objData;
    Object* target;
    Vec3f d;
    f32 playerDistance;
    f32 searchDistance;
    Object* player;
    Baddie_Setup* objSetup;

    objData = baddie->objdata;
    searchDistance = 100.0f;
    objSetup = (Baddie_Setup*)self->setup;

    target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, M_180_DEGREES);
    if (target == NULL) {
        target = objGetNearestTypeTo(OBJTYPE_38, self, &searchDistance);
    }
    
    if ((target != NULL) && !(baddie->unk3B0 & 4)) {
        gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 8, -1);
        fsa->unk33D = 0;
        
        fsa->target = target;

        if (baddie->fsa.target) {}

        baddie->unk3B6 = TRUE;
        return;
    }

    objData->unk11 = 0;
    
    if (objData->resetTimer > 0.0f) {
        if ((fsa->logicState != DBStealerWorm_LSTATE_3_Dead) || (baddie->unk3B0 & 1)) {
            if (objData->resetTimer >= objData->resetTimeThreshold) {
                gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 8, -1);
                objData->resetTimer = 0.0f;
                fsa->hitpoints = objSetup->quarterHitpoints * 4;
                self->srt.transl.x = objSetup->base.x;
                self->srt.transl.y = objSetup->base.y;
                self->srt.transl.z = objSetup->base.z;
                self->srt.yaw = objSetup->unk2A << 8;
                return;
            }
            
            objData->resetTimer += gUpdateRateF;
        }
    } else {
        player = objGetPlayer();
        if (player != NULL) {
            d.f[0] = player->globalPosition.f[0] - self->globalPosition.f[0];
            d.f[1] = player->globalPosition.f[1] - self->globalPosition.f[1];
            d.f[2] = player->globalPosition.f[2] - self->globalPosition.f[2];
            playerDistance = sqrtf(SQ(d.f[0]) + SQ(d.f[1]) + SQ(d.f[2]));
        } else {
            playerDistance = 10000.0f;
        }
        
        /* @bug: roarSoundTimer's value can increase way beyond roarSoundInterval's value while the player is distant.
            This causes the roar sound to play every tick when the player approaches, until roarSoundInterval's 
            value catches up with and overtakes roarSoundTimer's value. */
        if ((objData->roarSoundTimer > objData->roarSoundInterval) && (playerDistance < 400.0f)) {
            dll_amSfx->Play(self, dBattleSounds[1], 0x1E, NULL, NULL, 0, NULL);
            objData->roarSoundInterval += mathRnd(50, 250);
        }
        
        objData->roarSoundTimer += gUpdateRateF;
    }
}

// offset: 0xF20 | func: 13
void DBStealerWorm_handle_spit_and_fx_flags(Object* self, Baddie* baddie) {
    DBStealerWorm_DataActual* objData;
    s32 i;

    objData = baddie->objdata;
    
    if ((objData->flags & 1) && (baddie->fsa.target != NULL)) {
        STUBBED_PRINTF(" Fireing Projectile at Player ");
        DBStealerWorm_spit(self, baddie);
    }
    
    if (objData->flags & 2) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_345, NULL, 1, -1, NULL);
    }
    
    if (objData->flags & 4) {
        for (i = 0; i < 10; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_343, NULL, 1, -1, NULL);
        }
    }
    
    objData->flags = 0;
}

// offset: 0x1048 | func: 14
void DBStealerWorm_spit(Object* self, Baddie* baddie) {
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

// offset: 0x11C0 | func: 15
static void DBStealerWorm_func_11C0(Object* self, s16 pitch, s16 roll) {
    SeqJoint* seqJoint;
    s32* seqJointID;
    s32 i;

    seqJointID = objExpr_func_800349B0();
    
    for (i = 4, seqJointID++; i < 36; i += 4, seqJointID++) {
        seqJoint = (SeqJoint*)objExpr_func_80034804(self, *seqJointID);
        if (seqJoint != NULL) {
            seqJoint->roll = roll;
            seqJoint->pitch = pitch;
        }
    }
}

// offset: 0x1284 | func: 16
static s32 DBStealerWorm_func_1284(Object* self, Object* target, f32 arg2, f32 turnSpeedFactor, f32 arg4) {
    Baddie* baddie;
    f32 ySelf;
    f32 yTarget;
    f32 distance;
    s16 dYaw;
    f32 yDiff;

    
    baddie = self->data;
    dYaw = objAngleToObjectXZ(self, target, &distance);

    if (gUpdateRateF == 0.0f) {
        STUBBED_PRINTF(" Error : main_fdt is zero \n");
        return 0;
    } else {
        distance /= gUpdateRateF;
    }

    if (distance < arg2) {
        ySelf = self->srt.transl.y;
        yTarget = target->srt.transl.y;
        
        if (yTarget <= ySelf) {
            yDiff = ySelf - yTarget;
        } else {
            yDiff = -(ySelf - yTarget);
        }
        
        if (yDiff < 3.0f) {
            return 1;
        }
    }

    baddie->fsa.unk278 = (1.0f - ((f32) dYaw / M_360_DEGREES)) * turnSpeedFactor;
    baddie->fsa.unk27C = 0;
    
    return 0;
}

// offset: 0x13B0 | func: 17
static s32 DBStealerWorm_avoid_objects(Object* self, s32* objTypes, f32* avoidScaleFactors, s32 count, f32 maxTurnSpeed) {
    Baddie* baddie;
    s32 i;
    Object* obj;
    f32 normalisingFactor;
    f32 distance;
    f32 avoidScale;
    f32 sin;
    f32 cos;
    Vec3f d;
    f32 rotateFactor1;
    f32 rotateFactor2;
    f32 speed;
    f32 var_fv1;

    rotateFactor1 = 0.0f;
    rotateFactor2 = 0.0f;
    
    baddie = self->data;

    for (i = 0; i < count; i++) {
        distance = 260.0f;
        obj = objGetNearestTypeToExcludingSelf(objTypes[i], self, &distance);
        if (obj != NULL) {
            if (distance == 0) {
                STUBBED_PRINTF(" WARNING AVOIDANCE FAILED: Obj Intersection \n");
                return 0;
            }

            avoidScale = 1.0f - (distance / 260.0f);
            avoidScale = SQ(avoidScale);
            avoidScale = SQ(avoidScale);
            diPrintf(" Avoid Scale %f \n", &avoidScale);

            d.f[0] = obj->srt.transl.x - self->srt.transl.x;
            d.f[1] = obj->srt.transl.y - self->srt.transl.y;
            d.f[2] = obj->srt.transl.z - self->srt.transl.z;
            normalisingFactor = 1.0f / distance;
            d.f[0] *= normalisingFactor;
            d.f[1] *= normalisingFactor;
            d.f[2] *= normalisingFactor;
            
            rotateFactor1 -= d.f[0] * avoidScale * avoidScaleFactors[i] * maxTurnSpeed;
            rotateFactor2 -= d.f[2] * avoidScale * avoidScaleFactors[i] * maxTurnSpeed;
        } 
    }

    sin = mathSinfInterp(self->srt.yaw);
    cos = mathCosfInterp(self->srt.yaw);

    baddie->fsa.unk27C += ((rotateFactor1 * cos) - (rotateFactor2 * sin));
    baddie->fsa.unk278 += ((-rotateFactor2 * cos) - (rotateFactor1 * sin));

    if (baddie->fsa.enteredAnimState) {}
    
    if (baddie->fsa.unk278 < -maxTurnSpeed) {
        baddie->fsa.unk278 = -maxTurnSpeed;
    } else {
        speed = MIN(maxTurnSpeed, baddie->fsa.unk278);
        baddie->fsa.unk278 = speed;
        
        // FAKE
        if (baddie->fsa.unk278) {}
    }

    if (baddie->fsa.enteredAnimState) {}
    
    if (baddie->fsa.unk27C < -maxTurnSpeed) {
        baddie->fsa.unk27C = -maxTurnSpeed;
    } else {
        speed = MIN(maxTurnSpeed, baddie->fsa.unk27C);
        baddie->fsa.unk27C = speed;
    }

    return 0;
}

// offset: 0x169C | func: 18
/**
  * Used to find either the player or the sidekick (whoever's closest).
  */
static Object* DBStealerWorm_find_target(Object* self, s32* objTypes, s32 count, f32* distance) { //DBstealerworm_find_target?
    Object* obj;
    Object* matchObj;

    matchObj = NULL;

    for (count--; count >= 0; count--) {
        obj = objGetNearestTypeTo(objTypes[count], self, distance);
        if (obj != NULL) {
            matchObj = obj;
        }
    }
    
    return matchObj;
}

// offset: 0x1734 | func: 19
s32 DBStealerWorm_anim_state_0_pop_out_of_ground(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 pad;
    Baddie* baddie;
    DBStealerWorm_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        STUBBED_PRINTF("New State [%s]\n", "popOutOfGround");
    }

    if (fsa->enteredAnimState) {
        objAnimSet(self, 0xB, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        fsa->unk4.mode = 1;
        mainSetBits(baddie->unk39E, 1);
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        self->opacity = OBJECT_OPACITY_MAX;
        fsa->unk341 = 1;
        fsa->animTickDelta = (baddie->unk3B8 / 10000.0f) + 0.012f;
        func_8002674C(self);
    } else {
        func_80026128(self, 0xA, 1, -1);
        self->objhitInfo->unk5D = 0xA;
        self->objhitInfo->unk5E = 1;
        func_80028D2C(self);
    }
    
    if (fsa->unk33A) {
        baddie->unk3B6 = TRUE;
        objData->unk12_1 = 1;
        objData->targetIdx++;
    }

    if (fsa->unk308 & 0x200) {
        fsa->unk308 &= ~0x200;
        objData->flags |= 4;
    }
    if (self->animProgress < 0.7f) {
        objData->flags |= 2;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, mathRnd(0, 1), dChirpSounds);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, dBattleSounds);
    
    return 0;
}

// offset: 0x19B0 | func: 20
s32 DBStealerWorm_anim_state_1_burst_into_ground(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DBStealerWorm_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        STUBBED_PRINTF("New State [%s]\n", "burstIntoGround");
    }

    if (fsa->enteredAnimState) {
        objAnimSet(self, 0xE, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    
    if (self->animProgress > 0.25f) {
        objData = baddie->objdata;
        objData->flags |= 2;
    }
    
    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.01f;
        fsa->unk278 = 0.0f;
    }
    
    if (fsa->unk33A) {
        objData->resetTimer = 1.0f;
        mainSetBits(baddie->unk39E, 0);
        objAnimSet(self, 8, 0.0f, 0);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        baddie->unk3B6 = FALSE;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        objData->unk12_1 = 1;
        objData->targetIdx++;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, dBattleSounds);
    
    return 0;
}

// offset: 0x1B50 | func: 21
s32 DBStealerWorm_anim_state_2_bite_attack(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DBStealerWorm_DataActual* objData;

    baddie = self->data;

    if (fsa->enteredAnimState) {
        STUBBED_PRINTF("New State [%s]\n", "biteAttack");
    }

    objData = baddie->objdata;

    if (fsa->enteredAnimState) {
        func_8002674C(self);
    }
    
    func_80026128(self, 0xA, 1, -1);
    
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    
    if (fsa->enteredAnimState) {
        if (mathRnd(0, 1)) {
            if (fsa->enteredAnimState) {
                objAnimSet(self, 6, 0.0f, 0);
                fsa->unk33A = 0;
            }
        } else if (fsa->enteredAnimState) {
            objAnimSet(self, 7, 0.0f, 0);
            fsa->unk33A = 0;
        }
        
        fsa->unk341 = 1;
        fsa->animTickDelta = (baddie->unk3B8 / 20000.0f) + 0.005f;
    }
    
    fsa->unk278 = 0.0f;
    
    if (fsa->unk33A) {
        objData->unk12_1 = 1;
    }
    
    objData->flags |= 2;
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, mathRnd(0, 2), dChirpSounds);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, mathRnd(0, 2), dChirpSounds);
    
    return 0;
}

// offset: 0x1DA4 | func: 22
s32 DBStealerWorm_anim_state_3_stand_still(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        STUBBED_PRINTF("New State [%s]\n", "standStill");
    }

    if (fsa->enteredAnimState) {
        func_8002674C(self);
    }
    
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    fsa->animTickDelta = 0.01f;
    
    if (fsa->enteredAnimState != 0) {
        objAnimSet(self, 5, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    
    return 0;
}

// offset: 0x1E8C | func: 23
s32 DBStealerWorm_anim_state_4_stand_and_spit(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s8 entered;
    Baddie* baddie;
    DBStealerWorm_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        STUBBED_PRINTF("New State [%s]\n", "standAndSpit");

        func_8002674C(self);
    }
    
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    fsa->animTickDelta = 0.01f;

    entered = fsa->enteredAnimState;
    if (entered) {
        objAnimSet(self, 0xA, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    objData = baddie->objdata;
    objData->flags |= 2;
    
    if (fsa->unk308 & 1) {
        fsa->unk308 &= ~1;
        objData->flags |= 1;
        dll_amSfx->Play(self, dChirpSounds[3], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    if (fsa->unk33A != 0) {
        objData->unk12_1 = 1;
    }
    
    return 0;
}

// offset: 0x2010 | func: 24
s32 DBStealerWorm_anim_state_5_hit(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Object* weapon;
    Baddie* baddie;
    DBStealerWorm_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        if (fsa->enteredAnimState) {
            STUBBED_PRINTF("New State [%s]\n", "hitFightMain");
        }
        
        objAnimSet(self, 0, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        objData = baddie->objdata;
        if (objData->stolenEgg != NULL) {
            objSendMesg(objData->stolenEgg, 0x11, self, (void*)0x10);
            objAddObjectType(objData->stolenEgg, OBJTYPE_38);
            objData->stolenEgg = NULL;
            objData->targetIdx = 1;
        }
        
        weapon = objGetPlayer()->linkedObject;
        if (((DLL_251_Weapons*)weapon->dll)->vtbl->func16(weapon)) {
            dll_amSfx->Play(self, dWeaponHitSounds[mathRnd(3, 4)], MAX_VOLUME, NULL, NULL, 0, NULL);
        } else {
            dll_amSfx->Play(self, dWeaponHitSounds[mathRnd(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        dll_amSfx->Play(self, dHurtSounds[mathRnd(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    fsa->unk341 = 0x10;
    fsa->animTickDelta = 0.015f;
    fsa->unk278 = 0.0f;
    if (fsa->unk33A != 0) {
        objData = baddie->objdata;
        objData->unk12_1 = 1;
    }
    
    return 0;
}

// offset: 0x2278 | func: 25
s32 DBStealerWorm_anim_state_6_dying(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DBStealerWorm_DataActual* objData;

    baddie = self->data;
    
    fsa->unk341 = 0x11;

    if (fsa->enteredAnimState) {
        if (fsa->enteredAnimState) {
            STUBBED_PRINTF("New State [%s]\n", "fight_die");
        }

        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        fsa->unk27C = 0.0f;
        fsa->unk278 = 0.0f;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;

        gDLL_18_objfsa->vtbl->func21(self, fsa, PARTICLE_3C, 0xA, 0);
        dll_amSfx->Play(self, SOUND_57, MAX_VOLUME, NULL, NULL, 0, NULL);
        func_800267A4(self);

        objData = baddie->objdata;
        objData->stolenEgg = NULL;
    }
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 1, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->animTickDelta = 0.008f;
    
    if (gUpdateRateF <= self->opacity) {
        self->opacity -= gUpdateRateF;
    } else {
        self->opacity = 0;
    }
    
    //Start the respawn timer when the dying animation and opacity fade-out are finished
    if ((fsa->unk33A != 0) && (self->opacity == 0)) {
        objData = baddie->objdata;
        if (objData->resetTimer == 0.0f) {
            objData->resetTimer = 1.0f;

            mainSetBits(baddie->unk39E, 0);
            mainSetBits(baddie->unk39C, 1);

            //Reset to initial state, waiting to respawn
            objAnimSet(self, 8, 0.0f, 0);
            baddie->unk3B6 = FALSE;
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, DBStealerWorm_ASTATE_0_Pop_Out_of_Ground);
        }
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 2, dHurtSounds);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, dBattleSounds);
    
    return 0;
}

// offset: 0x25BC | func: 26
s32 DBStealerWorm_anim_state_7_run_to_object(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DBStealerWorm_DataActual* objData;
    f32 turnSpeedFactor;
    s32 minAngle;
    s16 roll;
    s16 pitch;
    
    baddie = self->data;
    objData = baddie->objdata;
    objData->flags |= 2;
    objData->unk10_18 = 0;
    
    if (fsa->enteredAnimState) {
        STUBBED_PRINTF("New State [%s]\n", "run to object");
        func_8002674C(self);
        func_80026160(self);
    }
    
    func_80028D2C(self);
    fsa->animTickDelta = 0.01f;
    
    if (objData->stolenEgg == NULL) {
        if (fsa->enteredAnimState) {
            objAnimSet(self, 0xF, 0.0f, 0);
            fsa->unk33A = 0;
        }
    } else {
        if (fsa->enteredAnimState) {
            objAnimSet(self, 0x11, 0.0f, 0);
            fsa->unk33A = 0;
        }
        fsa->animTickDelta = 0.018f;
    }
    
    fsa->unk341 = 0x1F;

    turnSpeedFactor = baddie->unk3B8 / 40.0f;
    if (DBStealerWorm_func_1284(self, fsa->target, 7, turnSpeedFactor, 0.2f)) {
        objData->unk10_14 = 1;
        objData->targetIdx++;
    }

    if (objIsObjectType(fsa->target, dTargetObjTypes[objData->targetIdx]) == FALSE) {
        STUBBED_PRINTF(" LOCK is Not of Given Type");
        objData->unk10_14 = 1;
    }

    DBStealerWorm_avoid_objects(self, dObjTypes, dAvoidScaleFactors, ARRAYCOUNT(dObjTypes), turnSpeedFactor);
    
    if (objData->unk10_14) {
        DBStealerWorm_func_11C0(self, 0, 0);
    } else if (objData->stolenEgg == NULL) {
        pitch = -(fsa->unk278 * 2535.0f);
        roll = -(fsa->unk27C * 2535.0f);
        
        if (pitch < -0x500) {
            pitch = -0x500;
        } else {
            minAngle = MIN(0x500, pitch);
            pitch = minAngle;
        }
        
        if (roll < -0x500) {
            roll = -0x500;
        } else {
            minAngle = MIN(0x500, roll);
            roll = minAngle;
        }
        
        DBStealerWorm_func_11C0(self, pitch, roll);
    }
    
    objGetAnimChange(self, fsa->unk278, &fsa->animTickDelta);
    
    return 0;
}

// offset: 0x2924 | func: 27
s32 DBStealerWorm_anim_state_8_pick_up_egg(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DBStealerWorm_DataActual* objData;
    Baddie* baddie;

    if (fsa->enteredAnimState) {
        STUBBED_PRINTF("New State [%s]\n", "pick up egg object");
    }
    
    baddie = self->data;
    
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    objData = baddie->objdata;
    objData->unk11 |= 4;
    
    if (fsa->unk308 & 0x200) {
        objData->stolenEgg = fsa->target;
        objSendMesg(objData->stolenEgg, 0x11, self, (void*)0x12);
        objFreeObjectType(objData->stolenEgg, OBJTYPE_38);
    }
    
    fsa->unk341 = 0x12;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x10, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->unk33A) {
        objData->unk12_1 = 1;
        objData->targetIdx++;
    }
    
    return 0;
}

// offset: 0x2A30 | func: 28
s32 DBStealerWorm_anim_state_9_throw_egg(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DBStealerWorm_DataActual* objData;

    if (fsa->enteredAnimState) {
        STUBBED_PRINTF("New State [%s]\n", "throw egg object");
    }
    
    baddie = self->data;

    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    objData = baddie->objdata;
    objData->unk11 |= 4;
    
    if (fsa->unk308 & 0x200) {
        objSendMesg(objData->stolenEgg, 0x11, self, (void*)0x11);
        objAddObjectType(objData->stolenEgg, OBJTYPE_38);
        objData->stolenEgg = NULL;
    }
    
    fsa->unk341 = 0x11;
    
    if (fsa->enteredAnimState != 0) {
        objAnimSet(self, 0x12, 0.0f, 0);
        fsa->unk33A = 0;
    }

    if (fsa->unk33A) {
        objData->unk12_1 = 1;
        objData->targetIdx++;
    }
    
    return 0;
}

// offset: 0x2B38 | func: 29
s32 DBStealerWorm_logic_state_0_top(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        STUBBED_PRINTF("New AI State [%s]\n", "aiTop");
    }

    if (fsa->target != 0) {
        if (fsa->enteredLogicState) {
            fsa->unk27C = 0.0f;
            fsa->unk278 = 0.0f;
            return FSA_NEXTSTATE_SYNC(DBStealerWorm_LSTATE_5_Engage);
        }

        if (fsa->unk33A != 0) {
            return FSA_NEXTSTATE_SYNC(DBStealerWorm_LSTATE_5_Engage);
        }
    }

    return 0;
}

// offset: 0x2B98 | func: 30
s32 DBStealerWorm_logic_state_1_hit(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DBStealerWorm_DataActual* objData;
    
    baddie = self->data;

    if (fsa->enteredLogicState) {
        STUBBED_PRINTF("New AI State [%s]\n", "aiIveBeenHit");
    }

    if (fsa->hitpoints <= 0) {
        return FSA_NEXTSTATE_SYNC(DBStealerWorm_LSTATE_2_Dying);
    }
    
    if (fsa->unk33A) {
        objData = baddie->objdata;
        objData->unk1C += 170.0f;
        return FSA_NEXTSTATE_SYNC(DBStealerWorm_LSTATE_5_Engage);
    }
    
    return 0;
}

// offset: 0x2BF8 | func: 31
s32 DBStealerWorm_logic_state_2_dying(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie *baddie;
    DBStealerWorm_DataActual *objData;
    
    baddie = self->data;
    
    if (fsa->enteredLogicState) {
        STUBBED_PRINTF("New AI State [%s]\n", "aiIamDying");
    }

    if (fsa->enteredLogicState) {
        objData = baddie->objdata;
        objData->roarSoundTimer = 0.0f;
        objData->roarSoundInterval = 0.0f;
        objData->resetTimer = 0.0f;
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, DBStealerWorm_ASTATE_6_Dying);
        objData->targetIdx = 0;
    } else if (fsa->unk33A) {
        objSendMesgMany(0, 3, self, 0xE0000, self);
        
        if (self->setup == NULL) {
            objFreeObject(self);
        }
        
        return FSA_NEXTSTATE_SYNC(DBStealerWorm_LSTATE_3_Dead);
    }

    return 0;
}

// offset: 0x2CE0 | func: 32
s32 DBStealerWorm_logic_state_3_dead(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredLogicState) {
        STUBBED_PRINTF("New AI State [%s]\n", "aiIamDead");
    }

    if (fsa->enteredLogicState) {
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, NO_GAMEBIT, 0);
    }
    
    return 0;
}

// offset: 0x2D48 | func: 33
s32 DBStealerWorm_logic_state_4_dormant(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DBStealerWorm_DataActual* objData;

    baddie = self->data;

    if (fsa->enteredLogicState) {
        STUBBED_PRINTF("New AI State [%s]\n", "aiDormant");
    }
 
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, DBStealerWorm_ASTATE_1_Burst_Into_Ground);
        objData = baddie->objdata;
        objData->roarSoundTimer = 0.0f;
        objData->roarSoundInterval = 0.0f;
        objData->resetTimer = 0.0f;
    }
    
    return 0;
}

// offset: 0x2DC4 | func: 34
s32 DBStealerWorm_logic_state_5_engage(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 pad;
    DBStealerWorm_DataActual* objData;
    Baddie_Setup* objSetup;
    f32 distance;
    s16 objType;
    u32 temp;
    Baddie* baddie;
    
    baddie = self->data;
    objSetup = (Baddie_Setup*)self->setup;
    distance = 1500.0f;

    if (fsa->enteredLogicState) {
        STUBBED_PRINTF("New AI State [%s]\n", "aiL2_MainEngageControl");
    }
    
    objData = baddie->objdata;
    if (fsa->enteredLogicState || objData->unk10_14) { 
        objData->unk10_18 = 0;
        objData->unk10_14 = 0;
        
        if (objData->targetIdx == 0) {
            self->srt.transl.x = objSetup->base.x;
            self->srt.transl.y = objSetup->base.y;
            self->srt.transl.z = objSetup->base.z;
        }
        
        objType = dTargetObjTypes[objData->targetIdx];
        if (objType != 0) {
            fsa->target = objGetNearestTypeTo(objType, self, &distance);
        }
        
        if (fsa->target != NULL) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, dTargetAnimStates[objData->targetIdx]);
        }

        temp = objData->targetIdx;
        if (temp >= 6) {
            objData->targetIdx = 0;
        }
    } else if ((fsa->unk33A != 0) && (objData->stolenEgg == NULL)) {
        if (objData->unk1C > 100.0f) {
            objData->unk1C -= 100.0f;
            distance = 200.0f;
            fsa->target = DBStealerWorm_find_target(self, dObjTypes, ARRAYCOUNT(dObjTypes) - 1, &distance);
            if (fsa->target != NULL) {
                if (distance < 50.0f) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, DBStealerWorm_ASTATE_2_Bite_Attack);
                } else {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, DBStealerWorm_ASTATE_4_Stand_and_Spit);
                }
            }
        }
    }
    
    return 0;
}
