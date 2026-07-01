#include "common.h"
#include "dlls/engine/18_objfsa.h"
#include "sys/objmsg.h"
#include "sys/objanim.h"
#include "sys/objtype.h"
#include "dlls/objects/260_Pollen.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s8 unk2E;
} DLL236_Setup;

typedef struct {
    f32 unk0;
    s8 _unk4;
    u8 unk5;
    u8 unk6;
    u8 _unk7[0x404-0x07];
} DLL236_Data; //404

/*0x0*/ static s8 _data_0[] = {
    0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0x00, 0x00
};

// /*0x1C*/ static u32 _data_1C[] = {
//     0x02060167, 0x01650206, 0x00000000, 0x00000000, 0x00000000
// };

/*0x0*/ static ObjFSA_StateCallback _bss_0[4];
/*0x10*/ static ObjFSA_StateCallback _bss_10[1];

static void dll_236_func_0(void);
static s32 dll_236_func_A20(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_236_func_B44(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_236_func_C60(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_236_func_D64(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_236_func_EA0(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
void dll_236_func_0(void) {
    _bss_0[0] = dll_236_func_A20;
    _bss_0[1] = dll_236_func_B44;
    _bss_0[2] = dll_236_func_C60;
    _bss_0[3] = dll_236_func_D64;
    _bss_10[0] = dll_236_func_EA0;
}
// offset: 0x5C | ctor
void dll_236_ctor(void* dll) {
    dll_236_func_0();
}

// offset: 0x9C | dtor
void dll_236_dtor(void *dll) { }

// offset: 0xA8 | func: 1 | export: 0
void dll_236_setup(Object* self, Baddie_Setup* arg1, s32 reset) {
    s32 pad[2];
    DLL236_Data* objData;
    Baddie* baddie;
    u8 flags;

    baddie = self->data;
    
    flags = 2 | 4;
    if (reset) {
        flags = 1 | 2 | 4;
    }
    
    if ((arg1->unk2B & 0x20) == FALSE) {
        flags |= 8;
    }
    
    gDLL_33_BaddieControl->vtbl->setup(self, arg1, baddie, 4, 1, 0x108, flags, 20.0f);
    self->animCallback = NULL;
    
    objData = baddie->objdata;
    bzero(objData, 8);
    objData->unk5 = 1;
    objData->unk0 = 0.0f;
    
    func_80023D30(self, 0, 0.5f, 0);
    
    baddie->fsa.flags |= 0x01000000;
    baddie->fsa.animState = 0;
    baddie->fsa.logicState = 0;
    baddie->fsa.hitpoints = 1;
    baddie->fsa.flags |= 0x4000;
    self->srt.pitch = M_90_DEGREES;
    baddie->unk3B6 = 0;
    baddie->unk3B4 = 0;
    baddie->fsa.unk4.mode = 0;
    func_800267A4(self);
}

// offset: 0x1EC | func: 2 | export: 1
void dll_236_control(Object* self) {
    /*0x1C*/ s16 _data_1C[] = { 0x0206, 0x0167, 0x0165, 0x00206};
    DLL236_Data* objData;
    Baddie* baddie;
    DLL236_Setup* objSetup;
    Object* player;
    Object* weapon;
    Vec3f d;
    SRT fxTransform;
    s32 scaleIdx;
    f32 distance;
    s32 i;
    
    baddie = self->data;
    objSetup = (DLL236_Setup*)self->setup;
    objData = baddie->objdata;
    player = get_player();
    
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
    
    if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 0) == 0) {
        return;
    }
    
    func_80024108(self, objData->unk0, gUpdateRateF, NULL);
    baddie->fsa.target = NULL;
    baddie->fsa.targetDist = 0.0f;
    
    if (player != NULL) {
        if ((baddie->fsa.animState != 2) && (baddie->fsa.animState != 3)) {
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
        if (gDLL_33_BaddieControl->vtbl->check_hit(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, NULL, _data_0, 0, &baddie->unk3A8, &fxTransform)) {
            weapon = player->linkedObject;
            scaleIdx = ((DLL_Unknown*)weapon->dll)->vtbl->func[19].withOneArgS32(weapon);
            if (scaleIdx > 3) {
                scaleIdx = 3;
            }
            
            fxTransform.scale = _data_1C[scaleIdx];
            gDLL_17_partfx->vtbl->spawn(self, 0x323, &fxTransform, 0x200001, -1, NULL);
            fxTransform.transl.f[0] -= self->srt.transl.f[0];
            fxTransform.transl.f[1] -= self->srt.transl.f[1];
            fxTransform.transl.f[2] -= self->srt.transl.f[2];
            fxTransform.scale = _data_1C[scaleIdx];
            
            i = 4;
            while (i--) {
                gDLL_17_partfx->vtbl->spawn(self, 0x324, &fxTransform, 2, -1, NULL);   
            }
        }
    } else {
        if ((baddie->fsa.animState != 3) && (baddie->fsa.animState != 2)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, 2);
            objData->unk5 = 1;
        }
    }
    
    gDLL_33_BaddieControl->vtbl->func10(self, &baddie->fsa, 0.0f, -1);
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, gUpdateRateF, gUpdateRateF, _bss_0, _bss_10);
    self->animObj = baddie->unk3AC;
}

// offset: 0x6C4 | func: 3 | export: 2
void dll_236_update(Object *self) { }

// offset: 0x6D0 | func: 4 | export: 3
void dll_236_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if ((visibility) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x730 | func: 5 | export: 4
void dll_236_free(Object* self, s32 a1) {
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
u32 dll_236_get_model_flags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_8;
}

// offset: 0x7E4 | func: 7 | export: 6
u32 dll_236_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL236_Data);
}

// offset: 0x7F8 | func: 8
static void dll_236_func_7F8(Object* self, Baddie* baddie) {
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
        pollenSetup->loadFlags = 1;
        pollenSetup->byte5 = 1;
        pollenSetup->byte6 = 0xFF;
        pollenSetup->fadeDistance = 0xFF;
        pollen = obj_create(pollenSetup, 4 | 1, -1, -1, NULL);
        
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
s32 dll_236_func_A20(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DLL236_Data* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;

    fsa->unk341 = 0;

    if (objData->unk5 != 0) {
        objData->unk5 = 0;
        objData->unk0 = 0.0f;
        func_80023D30(self, 0, 0.5f, 0);
        func_8002674C(self);
    }

    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;

    func_80028D2C(self);

    if (fsa->target != NULL) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
        objData->unk5 = 1;
    }

    return 0;
}

// offset: 0xB44 | func: 10
s32 dll_236_func_B44(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL236_Data* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    fsa->unk341 = 1;
    if (objData->unk5 != 0) {
        objData->unk5 = 0;
        objData->unk6 = 0;
        objData->unk0 = 0.02f;
        func_80023D30(self, 0, 0.5f, 0);
        func_8002674C(self);
    }
    
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    if (objData->unk6 == 0) {
        objData->unk6 = 1;
        dll_236_func_7F8(self, baddie);
    }
    
    return 0;
}

// offset: 0xC60 | func: 11
s32 dll_236_func_C60(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL236_Data* objData;
    void* sp20;

    baddie = self->data;
    objData = baddie->objdata;
    
    fsa->unk341 = 0;
    if (objData->unk5 != 0) {
        objData->unk5 = 0;
        objData->unk0 = 0.01f;
        func_80023D30(self, 0, 0.5f, 0);
        func_800267A4(self);
    }
    
    self->objhitInfo->unk5E = 0;
    
    if ((self->curModAnimId == 0) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 3);
        objData->unk5 = 1;
    }
    
    return 0;
}

// offset: 0xD64 | func: 12
s32 dll_236_func_D64(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL236_Data* triffid;

    baddie = self->data;
    triffid = baddie->objdata;

    fsa->unk341 = 0;

    if (triffid->unk5 != 0) {
        triffid->unk5 = 0;
        obj_send_mesg_many(0, OBJMSG_SEND_IGNORE_SENDER | OBJMSG_SEND_ALL, self, 0xE0000U, self);
        gDLL_33_BaddieControl->vtbl->func18(self, baddie->unk3E0, -1, 0);
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
s32 dll_236_func_EA0(Object* arg0, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
} 

