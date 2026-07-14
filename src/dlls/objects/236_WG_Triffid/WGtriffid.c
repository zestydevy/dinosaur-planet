#include "PR/ultratypes.h"
#include "common.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/objects/260_Pollen.h"
#include "game/objects/object.h"
#include "sys/objmsg.h"
#include "sys/objanim.h"
#include "sys/objtype.h"

typedef struct {
    Baddie_Setup baddie;
} WGTriffid_Setup;

typedef struct {
    f32 animSpeed;
    s8 unk4;
    u8 changedState;
    u8 createdPollen;
} WGTriffid_ActualData;

typedef struct {
/*000*/    Baddie baddie;
/*3FC*/    WGTriffid_ActualData data;
} WGTriffid_Data;

typedef enum {
    WGTriffid_ASTATE_0,
    WGTriffid_ASTATE_1,
    WGTriffid_ASTATE_2,
    WGTriffid_ASTATE_3
} WGTriffid_FSA_AnimStates;

typedef enum {
    WGTriffid_LSTATE_0
} WGTriffid_FSA_LogicStates;

/*0x0*/ static s8 dHitDamageMap[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

/*0x0*/ static ObjFSA_StateCallback sAnimStates[sizeof(WGTriffid_FSA_AnimStates)];
/*0x10*/ static ObjFSA_StateCallback sLogicStates[sizeof(WGTriffid_FSA_LogicStates)];

static void WGTriffid_fsa_state_setup(void);
static s32 WGTriffid_anim_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WGTriffid_anim_state_1(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WGTriffid_anim_state_2(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WGTriffid_anim_state_3(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WGTriffid_logic_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
void WGTriffid_fsa_state_setup(void) {
    sAnimStates[WGTriffid_ASTATE_0] = WGTriffid_anim_state_0;
    sAnimStates[WGTriffid_ASTATE_1] = WGTriffid_anim_state_1;
    sAnimStates[WGTriffid_ASTATE_2] = WGTriffid_anim_state_2;
    sAnimStates[WGTriffid_ASTATE_3] = WGTriffid_anim_state_3;

    sLogicStates[WGTriffid_LSTATE_0] = WGTriffid_logic_state_0;
}
// offset: 0x5C | ctor
void WGTriffid_ctor(void* dll) {
    WGTriffid_fsa_state_setup();
}

// offset: 0x9C | dtor
void WGTriffid_dtor(void *dll) { }

// offset: 0xA8 | func: 1 | export: 0
void WGTriffid_setup(Object* self, WGTriffid_Setup* objSetup, s32 reset) {
    s32 pad[2];
    WGTriffid_ActualData* objData;
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
    
    gDLL_33_BaddieControl->vtbl->setup(self, &objSetup->baddie, baddie, 4, 1, 0x108, flags, 20.0f);
    self->animCallback = NULL;
    
    objData = baddie->objdata;
    bzero(objData, 8);
    objData->changedState = TRUE;
    objData->animSpeed = 0.0f;
    
    func_80023D30(self, 0, 0.5f, 0);
    
    baddie->fsa.flags |= OBJFSA_FLAG_1000000;
    baddie->fsa.animState = WGTriffid_ASTATE_0;
    baddie->fsa.logicState = WGTriffid_LSTATE_0;
    baddie->fsa.hitpoints = 1;
    baddie->fsa.flags |= OBJFSA_FLAG_4000;

    self->srt.pitch = M_90_DEGREES;
    baddie->unk3B6 = 0;
    baddie->unk3B4 = 0;
    baddie->fsa.unk4.mode = 0;
    func_800267A4(self);
}

// offset: 0x1EC | func: 2 | export: 1
void WGTriffid_control(Object* self) {
    /*0x1C*/ s16 dFXScales[] = { 0x0206, 0x0167, 0x0165, 0x0206};
    WGTriffid_ActualData* objData;
    Baddie* baddie;
    WGTriffid_Setup* objSetup;
    Object* player;
    Object* weapon;
    Vec3f d;
    SRT fxTransform;
    s32 scaleIdx;
    f32 distance;
    s32 i;
    
    baddie = self->data;
    objSetup = (WGTriffid_Setup*)self->setup;
    objData = baddie->objdata;
    player = get_player();
    
    if (self->unkDC) {
        return;
    }
    
    if (self->unkE0 == 0) {
        self->srt.transl.f[0] = objSetup->baddie.base.x;
        self->srt.transl.f[1] = objSetup->baddie.base.y;
        self->srt.transl.f[2] = objSetup->baddie.base.z;
        gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->baddie.unk2E, self, -1);
        self->unkE0 = 1;
        return;
    }
    
    if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 0) == 0) {
        return;
    }
    
    func_80024108(self, objData->animSpeed, gUpdateRateF, NULL);
    baddie->fsa.target = NULL;
    baddie->fsa.targetDist = 0.0f;
    
    if (player != NULL) {
        if ((baddie->fsa.animState != WGTriffid_ASTATE_2) && (baddie->fsa.animState != WGTriffid_ASTATE_3)) {
            d.f[0] = player->globalPosition.f[0] - self->globalPosition.f[0];
            d.f[1] = player->globalPosition.f[1] - self->globalPosition.f[1];
            d.f[2] = player->globalPosition.f[2] - self->globalPosition.f[2];
            distance = sqrtf(SQ(d.f[0]) + SQ(d.f[1]) + SQ(d.f[2]));
            
            if (distance < baddie->unk3E2) {
                baddie->fsa.target = player;
                baddie->fsa.targetDist = distance;
            }
        }
    }
    
    if ((baddie->unk3B0 & 0x20) == FALSE) {
        gDLL_33_BaddieControl->vtbl->func14(self, baddie, &baddie->unk3B2, -1, -1, baddie->unk3A6, baddie->unk3A4);
    }

    gDLL_33_BaddieControl->vtbl->func20(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 0);
    
    if (baddie->fsa.hitpoints > 0) {
        if (gDLL_33_BaddieControl->vtbl->check_hit(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, NULL, dHitDamageMap, 0, &baddie->unk3A8, &fxTransform)) {
            weapon = player->linkedObject;
            scaleIdx = ((DLL_Unknown*)weapon->dll)->vtbl->func[19].withOneArgS32(weapon); //TODO: weapon DLL interface
            if (scaleIdx > 3) {
                scaleIdx = 3;
            }
            
            fxTransform.scale = dFXScales[scaleIdx];
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, &fxTransform, 0x200001, -1, NULL);
            fxTransform.transl.f[0] -= self->srt.transl.f[0];
            fxTransform.transl.f[1] -= self->srt.transl.f[1];
            fxTransform.transl.f[2] -= self->srt.transl.f[2];
            fxTransform.scale = dFXScales[scaleIdx];
            
            i = 4;
            while (i--) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_324, &fxTransform, 2, -1, NULL);   
            }
        }
    } else {
        if ((baddie->fsa.animState != WGTriffid_ASTATE_3) && (baddie->fsa.animState != WGTriffid_ASTATE_2)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, WGTriffid_ASTATE_2);
            objData->changedState = TRUE;
        }
    }
    
    gDLL_33_BaddieControl->vtbl->func10(self, &baddie->fsa, 0.0f, -1);
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, gUpdateRateF, gUpdateRateF, sAnimStates, sLogicStates);
    self->animObj = baddie->unk3AC;
}

// offset: 0x6C4 | func: 3 | export: 2
void WGTriffid_update(Object *self) { }

// offset: 0x6D0 | func: 4 | export: 3
void WGTriffid_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if ((visibility) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x730 | func: 5 | export: 4
void WGTriffid_free(Object* self, s32 a1) {
    Baddie* objData;

    objData = self->data;
    obj_free_object_type(self, OBJTYPE_Baddie);

    if (self->linkedObject != NULL) {
        obj_destroy_object(self->linkedObject);
        self->linkedObject = NULL;
    }

    gDLL_33_BaddieControl->vtbl->free(self, objData, 0x20);
}

// offset: 0x7D4 | func: 6 | export: 5
u32 WGTriffid_get_model_flags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_8;
}

// offset: 0x7E4 | func: 7 | export: 6
u32 WGTriffid_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(WGTriffid_Data);
}

// offset: 0x7F8 | func: 8
static void WGTriffid_create_pollen(Object* self, Baddie* baddie) {
    ObjSetup* pollenSetup;
    Object* pollen;
    s16 thetaA;
    s16 thetaB;
    s32 i;

    i = 0;
    do {
        pollenSetup = obj_alloc_setup(sizeof(Pollen_Setup), OBJ_Pollen);
        pollenSetup->x = (fsin16(self->srt.yaw) * 16.0f) + self->srt.transl.x;
        pollenSetup->y = self->srt.transl.y;
        pollenSetup->z = (fcos16(self->srt.yaw) * 16.0f) + self->srt.transl.z;
        pollenSetup->loadFlags = OBJSETUP_LOAD_LEVEL;
        pollenSetup->fadeFlags = OBJSETUP_FADE_MANUAL;
        pollenSetup->loadDistance = 0xFF;
        pollenSetup->fadeDistance = 0xFF;
        pollen = obj_create(pollenSetup, (OBJINIT_STANDALONE | OBJINIT_FLAG4), -1, -1, NULL);
        
        if (pollen != NULL) {
            thetaA = rand_next(-2000, 2000) + self->srt.yaw;
            thetaB = rand_next(-2000, 2000);
            pollen->velocity.x = fcos16(thetaB) * fsin16(thetaA) * 4.0f;
            pollen->velocity.y = fsin16(thetaB) * 4.0f;
            pollen->velocity.z = fcos16(thetaB) * fcos16(thetaA) * 4.0f;
            pollen->unkC4 = self;
        }
        
    } while (i--);
}

// offset: 0xA20 | func: 9
s32 WGTriffid_anim_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    WGTriffid_ActualData* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;

    fsa->unk341 = 0;

    if (objData->changedState) {
        objData->changedState = FALSE;
        objData->animSpeed = 0.0f;
        func_80023D30(self, 0, 0.5f, 0);
        func_8002674C(self);
    }

    func_80026128(self, Damage_Type_Sword_Staff_Strike1, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;

    func_80028D2C(self);

    if (fsa->target != NULL) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, WGTriffid_ASTATE_1);
        objData->changedState = TRUE;
    }

    return 0;
}

// offset: 0xB44 | func: 10
s32 WGTriffid_anim_state_1(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    WGTriffid_ActualData* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    fsa->unk341 = 1;

    if (objData->changedState) {
        objData->changedState = FALSE;
        objData->createdPollen = FALSE;
        objData->animSpeed = 0.02f;
        func_80023D30(self, 0, 0.5f, 0);
        func_8002674C(self);
    }
    
    func_80026128(self, Damage_Type_Sword_Staff_Strike1, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);

    if (objData->createdPollen == FALSE) {
        objData->createdPollen = TRUE;
        WGTriffid_create_pollen(self, baddie);
    }
    
    return 0;
}

// offset: 0xC60 | func: 11
s32 WGTriffid_anim_state_2(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    WGTriffid_ActualData* objData;
    void* sp20;

    baddie = self->data;
    objData = baddie->objdata;
    
    fsa->unk341 = 0;
    
    if (objData->changedState) {
        objData->changedState = FALSE;
        objData->animSpeed = 0.01f;
        func_80023D30(self, 0, 0.5f, 0);
        func_800267A4(self);
    }
    
    self->objhitInfo->unk5E = 0;
    
    if ((self->curModAnimId == 0) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, WGTriffid_ASTATE_3);
        objData->changedState = TRUE;
    }
    
    return 0;
}

// offset: 0xD64 | func: 12
s32 WGTriffid_anim_state_3(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    WGTriffid_ActualData* objData;

    baddie = self->data;
    objData = baddie->objdata;

    fsa->unk341 = 0;

    if (objData->changedState) {
        objData->changedState = FALSE;
        obj_send_mesg_many(0, OBJMSG_SEND_IGNORE_SENDER | OBJMSG_SEND_ALL, self, 0xE0000, self);
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, -1, 0);
        gDLL_18_objfsa->vtbl->func21(self, fsa, PARTICLE_3C, 0xA, 0);
        baddie->unk3B4 = 0;
    }

    self->objhitInfo->unk5E = 0;

    if (self->opacity >= gUpdateRate * 2) {
        self->opacity -= gUpdateRate * 2;
    } else {
        self->opacity = 0;
    }

    if (self->opacity == 0) {
        obj_destroy_object(self);
    }

    return 0;
}

// offset: 0xEA0 | func: 13
s32 WGTriffid_logic_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
} 

