#include "common.h"
#include "macros.h"
#include "sys/objmsg.h"
#include "sys/objlib.h"
#include "sys/objtype.h"
#include "dlls/objects/221_ChukaChuck.h"
#include "dlls/objects/251_weapons.h"

//"core_snowworm.c" seems to have been Rare's filename

typedef struct {
    Baddie_Setup baddie;
} DLL216_Setup;

typedef struct {
    f32 unk0;
    s16 unk4;
    s16 unk6;
    SRT unk8;
    SRT unk20;
    Vec3f unk38;
    u8 unk44;
    u8 unk45;
    u16 unk46;
} DLL216_DataActual;

typedef struct {
    Baddie unk0;
    DLL216_DataActual unk3FC;
} DLL216_Data;

/*0x0*/ static u32 data_0[] = {
    0x00000336, 0x00000337, 0x0000033b
};
/*0xC*/ static u32 data_C[] = {
    0x0000005a, 0x0000005b, 0x0000032b, 0x00000335
};
/*0x1C*/ static u32 data_1C[] = {
    0x00000339, 0x00000338, 0x00000339, 0x0000033c, 0x0000033d
};
/*0x30*/ static s16 data_30[] = {
    0x0005, 0x0006, 0x0008, 0x0006, 0x0005, 0x0008, 0x0006
};
/*0x40*/ static s16 data_40[] = {
    0x0008, 0x0006, 0x0009, 0x0008, 0x0006, 0x0009, 0x0009
};
/*0x50*/ static u32 data_50[] = {
    0x00000374, 0x00000375, 0x00000376, 0x0000025b, 0x0000025c
};
/*0x64*/ static s32 data_64[] = {
    0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 
    0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000c, 0x0000000a, 
    0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 
    0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a
};
/*0xD4*/ static s8 data_D4[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff
};
/*0xF0*/ static u8 data_F0[5][3] = {
    {0xff, 0xff, 0xff}, 
    {0x00, 0xff, 0x00}, 
    {0xff, 0xc0, 0x96}, 
    {0x5a, 0x5a, 0x64},
    {0xff, 0x5a, 0x00}
};
/*0x100*/ static u8 data_100[] = {
    0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x02, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*0x0*/ static ObjFSA_StateCallback bss_0[14];
/*0x38*/ static ObjFSA_StateCallback bss_38[8];
// /*0x40*/ static u8 _bss_40[0x20];

static void dll_216_func_908(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void dll_216_func_CE8(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void dll_216_func_EBC(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void dll_216_func_10D8(Object* self, Baddie* baddie);
static void dll_216_func_1374(Object* self, Baddie* baddie);
static void dll_216_func_16C4(Object* self, DLL216_DataActual* objData);

static s32 dll_216_func_1790(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_19F4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_1C34(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_1DEC(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_1F74(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_20B4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_2354(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_2620(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_2778(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_2834(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_2964(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_2B64(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_2CA4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_2E78(Object* self, ObjFSA_Data* fsa, f32 updateRate);

static s32 dll_216_func_309C(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_32E4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_33A8(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_34A0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_3524(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_3580(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_3638(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_216_func_396C(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void dll_216_func_0(void) {
    bss_0[0] = dll_216_func_1790;
    bss_0[1] = dll_216_func_19F4;
    bss_0[2] = dll_216_func_1C34;
    bss_0[3] = dll_216_func_1DEC;
    bss_0[4] = dll_216_func_1F74;
    bss_0[5] = dll_216_func_20B4;
    bss_0[6] = dll_216_func_2354;
    bss_0[7] = dll_216_func_2620;
    bss_0[8] = dll_216_func_2778;
    bss_0[9] = dll_216_func_2834;
    bss_0[10] = dll_216_func_2964;
    bss_0[11] = dll_216_func_2B64;
    bss_0[12] = dll_216_func_2CA4;
    bss_0[13] = dll_216_func_2E78;
    
    bss_38[0] = dll_216_func_309C;
    bss_38[1] = dll_216_func_32E4;
    bss_38[2] = dll_216_func_33A8;
    bss_38[3] = dll_216_func_34A0;
    bss_38[4] = dll_216_func_3524;
    bss_38[5] = dll_216_func_3580;
    bss_38[6] = dll_216_func_3638;
    bss_38[7] = dll_216_func_396C;
}

// offset: 0x12C | ctor
void dll_216_ctor(void* dll) {
    dll_216_func_0();
}

// offset: 0x16C | dtor
void dll_216_dtor(void* dll) { }

// offset: 0x178 | func: 1 | export: 0
void dll_216_setup(Object* self, DLL216_Setup* objSetup, s32 reset) {
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
        baddie->unk3E2 = 0x6E;
    }
    
    func_80023D30(self, 8, 0.0f, 0);
    
    self->unkAF |= 8;
    
    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, 0);
    baddie->fsa.logicState = 0;
    baddie->fsa.unk4.mode = 0;
}

// offset: 0x2C0 | func: 2 | export: 1
void dll_216_control(Object* self) {
    Baddie* baddie;
    Baddie_Setup* objSetup;

    baddie = self->data;
    objSetup = (Baddie_Setup*)self->setup;
    if (self->unkDC != 0) {
        if (((baddie->fsa.logicState != 3) || (baddie->unk3B0 & 1)) && (gDLL_29_Gplay->vtbl->did_time_expire(objSetup->base.uID))) {
            gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 0xE, 8, 0x102, 0x26, 20.0f);
            baddie->unk3B6 = 0;
            gDLL_6_AMSFX->vtbl->play(self, 0xB20U, MAX_VOLUME, NULL, NULL, 0, NULL);
            func_80023D30(self, 8, 0.0f, 0x10);
            baddie->fsa.unk33A = 0;
            self->opacity = 0xFF;
            self->unkAF |= 8;
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
    
    dll_216_func_908(self, baddie, &baddie->fsa);
    dll_216_func_1374(self, baddie);
    
    if (baddie->unk3B6 == 0) {
        dll_216_func_EBC(self, baddie, &baddie->fsa);
    } else {
        dll_216_func_CE8(self, baddie, &baddie->fsa);
    }
    
    if (baddie->unk3B0 & 2) {
        self->srt.transl.y = objSetup->base.y - 8.0f;
    }
}

// offset: 0x554 | func: 3 | export: 2
void dll_216_update(Object* self) {
    DLL216_Data* objData = self->data;
    gDLL_18_objfsa->vtbl->func2(self, &objData->unk0.fsa, bss_0);
}

// offset: 0x5A4 | func: 4 | export: 3
void dll_216_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie = self->data;
    
    if ((visibility == FALSE) || self->unkDC || (baddie->unk3B6 == 0)) {
        return;
    } 
    
    if (baddie->unk3E8) {
        func_80036FBC(0xC8, 0, 0, baddie->unk3E8);
    }
    
    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    dll_216_func_10D8(self, baddie);
    
    if (baddie->unk3B2 & 0x60) {
        if (baddie->unk3B2 & 0x20) {
            gDLL_32->vtbl->func2(self, 0x330, &baddie->unk3E8);
            gDLL_32->vtbl->func2(self, 0x330, &baddie->unk3E8);
        }
        
        gDLL_32->vtbl->func2(self, 0x32F, &baddie->unk3E8);
    }
}

// offset: 0x7AC | func: 5 | export: 4
void dll_216_free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    camera_disable_y_offset();
    obj_free_object_type(self, OBJTYPE_Baddie);

    if (self->linkedObject) {
        obj_destroy_object(self->linkedObject);
        self->linkedObject = NULL;
    }
    
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
}

// offset: 0x858 | func: 6 | export: 5
u32 dll_216_get_model_flags(Object* self) {
    return MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x868 | func: 7 | export: 6
u32 dll_216_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DLL216_Data);
}

// offset: 0x87C | func: 8 | export: 7
s16 dll_216_func_87C(Object* self) {
    DLL216_Data* objData = self->data;
    return objData->unk0.fsa.animState;
}

// offset: 0x88C | func: 9 | export: 8
void dll_216_func_88C(Object* self, u8 message) {
    Baddie* baddie;
    ObjFSA_Data* fsa;

    baddie = self->data;
    
    switch (message) {
    case 0x80:
        fsa = &baddie->fsa;
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 2);
        fsa->logicState = 4;
        fsa->enteredLogicState = 1;
        break;
    default:
        STUBBED_PRINTF("BADDIE:SnowWorm Unknown message [%d]\n", message);
    }
}

// offset: 0x908 | func: 10
void dll_216_func_908(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    DLL216_DataActual* objData = baddie->objdata;
    Object* player = get_player();
    Vec3f sp84;
    SRT sp6C;
    s32 i;
    s32 scaleIdx;
/*0x120*/ s16 data_120[4] = { 0x0206, 0x0167, 0x0165, 0x0206 };
/*0x128*/ s16 data_128[4] = { 0x0206, 0x0167, 0x0165, 0x0206 };

    if (fsa->target != NULL) {
        sp84.x = fsa->target->globalPosition.x - self->globalPosition.x;
        sp84.y = fsa->target->globalPosition.y - self->globalPosition.y;
        sp84.z = fsa->target->globalPosition.z - self->globalPosition.z;
        fsa->targetDist = sqrtf(SQ(sp84.f[0]) + SQ(sp84.f[1]) + SQ(sp84.f[2]));
    }
    
    if (!(baddie->unk3B0 & 0x20)) {
        gDLL_33_BaddieControl->vtbl->func14(self, (Baddie*)fsa, &baddie->unk3B2, 2, 3, baddie->unk3A6, baddie->unk3A4);
    }
    
    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 8);
    objData->unk0 += gUpdateRateF;
    if ((fsa->animState != 3) && 
        (gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, 
            &baddie->unk34C, baddie->unk39E, data_64, data_D4, 1, &baddie->unk3A8, &sp6C))
    ) {
        if (objData->unk0 < 240.0f) {
            objData->unk6++;
        } else {
            objData->unk6 = 0;
        }
        
        objData->unk0 = 0.0f;
        
        if ((fsa->hitpoints > 0) && (objData->unk6 >= 2)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 3);
            objData->unk6 = 0;
            fsa->logicState = 5;
        }
        
        scaleIdx = ((DLL_251_Weapons*)player->linkedObject->dll)->vtbl->func19(player->linkedObject);
        if (scaleIdx > 3) {
            scaleIdx = 3;
        }
        
        sp6C.scale = data_120[scaleIdx];
        gDLL_17_partfx->vtbl->spawn(self, 0x323, &sp6C, 0x200001, -1, NULL);
        if (scaleIdx != 0) {
            sp6C.transl.f[0] -= self->globalPosition.f[0];
            sp6C.transl.f[1] -= self->globalPosition.f[1];
            sp6C.transl.f[2] -= self->globalPosition.f[2];
            sp6C.scale = data_128[scaleIdx];
            
            for (i = 0; i < 0xF; i++) {
                gDLL_17_partfx->vtbl->spawn(self, 0x324, &sp6C, 2, -1, NULL);
            }
        }
    }
}

/*0x130*/ static u8 data_130 = 0;
/*0x134*/ static u8 data_134 = 0;

// offset: 0xCE8 | func: 11
void dll_216_func_CE8(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    DLL216_DataActual* objData;

    objData = baddie->objdata;
    objData->unk46 += gUpdateRate;
    
    if ((objData->unk46 >= 300)) {
        objData->unk46 = rand_next(0, 200);
        
        if (fsa->animState == 7 || fsa->animState == 8) {
            gDLL_6_AMSFX->vtbl->play(self, data_1C[rand_next(3, 4)], 0x46, NULL, NULL, 0, NULL);
        }
    } 
    
    if (baddie->unk3B0 & 2) {
        gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.0f, -1);
    } else {
        gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.17f, -1);
    }
    
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, bss_0, bss_38);
    self->animObj = baddie->unk3AC;
}

// offset: 0xEBC | func: 12
void dll_216_func_EBC(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
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
void dll_216_func_10D8(Object* self, Baddie* baddie) {
    DLL216_DataActual* objData;
    Vec3f v;
    MtxF mtx;
    f32 sp64;
    f32 var_fv0;
    SRT transform;

    objData = baddie->objdata;
    
    memcpy(&mtx, func_80032170(self, 1), sizeof(MtxF));
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

    if (baddie->fsa.animState != 4) {
        func_80031F6C(self, 2, &objData->unk20.transl.x, &objData->unk20.transl.y, &objData->unk20.transl.z, 0);
    } else {
        func_80031F6C(self, 0, &objData->unk20.transl.x, &objData->unk20.transl.y, &objData->unk20.transl.z, 0);
    }
    
    objData->unk20.transl.f[1] = self->srt.transl.y + 8.0f;
    objData->unk20.transl.f[0] -= fsin16_precise(self->srt.yaw) * 10.0f * sp64;
    objData->unk20.transl.f[2] -= fcos16_precise(self->srt.yaw) * 10.0f * sp64;
    
    transform.transl.f[0] = 0.0f;
    transform.transl.f[1] = -15.0f;
    transform.transl.f[2] = -20.0f;
    
    func_80031F6C(self, 0, &transform.transl.x, &transform.transl.y, &transform.transl.z, 1);
    
    if (objData->unk44 & 2) {
        v.f[0] = -8.0f;
        v.f[1] = 40.0f;
        v.f[2] = -20.0f;
        vec3_transform(&mtx, v.f[0], v.f[1], v.f[2], &v.x, &v.y, &v.z);
        memcpy(&objData->unk38, &v, sizeof(Vec3f));
        memcpy(&objData->unk8, &transform, sizeof(SRT));
        objData->unk44 |= 1;
    }
}

// offset: 0x1374 | func: 14
void dll_216_func_1374(Object* self, Baddie* baddie) {
    DLL216_DataActual* objData;
    s32 i;
    SRT* fxTransform;
    u8* colourRGB;
    f32 shakeFactor;

    objData = baddie->objdata;
    
    if (self->id == OBJ_SnowWormLarge) {
        objData->unk20.scale = 1.7f;
        shakeFactor = 2.0f;
    } else {
        objData->unk20.scale = 1.0f;
        shakeFactor = 1.0f;
    }
    
    i = 0;
    if (baddie->fsa.unk4.mode != 0) {
        i = data_100[baddie->fsa.unk4.unk68.unk50[0]];
        if (i > 30) { //@bug? Should this be `i >= 5` (ARRAYCOUNT(data_f0))?
            i = 0;
        }
    }
    
    colourRGB = data_F0[i];
    fxTransform = &objData->unk20;
    
    if (objData->unk44 & 1) {
        dll_216_func_16C4(self, objData);
        objData->unk44 &= ~1;
    }
    
    if ((objData->unk44 & 4) && !(baddie->unk3B0 & 0x40)) {
        for (i = 0; i < 4; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x56, fxTransform, 0x200001, -1, colourRGB);
        }
    }
    
    if ((objData->unk44 & 8) && !(baddie->unk3B0 & 0x40)) {
        gDLL_17_partfx->vtbl->spawn(self, 0x57, fxTransform, 0x200001, -1, colourRGB);
    }
    
    if (objData->unk44 & 0x10) {
        camera_enable_y_offset();
        camera_set_shake_offset(2.0f * shakeFactor);
        
        for (i = 0; i < 40; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x57, fxTransform, 0x200001, -1, colourRGB);
        }
    }
    
    if (objData->unk44 & 0x20) {
        camera_enable_y_offset();
        camera_set_shake_offset(3.0f * shakeFactor);
        
        for (i = 0; i < 40; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x57, fxTransform, 0x200001, -1, colourRGB);
        }
        
        for (i = 0; i < 10; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x58, fxTransform, 0x200001, -1, colourRGB);
        }
    }
    
    objData->unk44 = 0;
}

// offset: 0x16C4 | func: 15
void dll_216_func_16C4(Object* self, DLL216_DataActual* objData) {
    ChukaChuck_Setup* iceSetup;
    Object* ice;

    iceSetup = obj_alloc_setup(sizeof(ChukaChuck_Setup), OBJ_IceBall);
    iceSetup->base.x = objData->unk8.transl.x;
    iceSetup->base.y = objData->unk8.transl.y;
    iceSetup->base.z = objData->unk8.transl.z;
    iceSetup->base.loadFlags = 1;
    iceSetup->base.fadeFlags = 1;
    iceSetup->base.loadDistance = 0xFF;
    iceSetup->base.fadeDistance = 0xFF;
    iceSetup->unk1E = -1;
    iceSetup->unk20 = -1;
    ice = obj_create(&iceSetup->base, (4 | 1), -1, -1, NULL);
    if (ice != NULL) {
        ice->velocity.x = objData->unk38.x;
        ice->velocity.y = objData->unk38.y;
        ice->velocity.z = objData->unk38.z;
        ice->unkC4 = self;
    }
}

// offset: 0x1790 | func: 16
s32 dll_216_func_1790(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 pad[2];
    DLL216_DataActual *objData;
    Baddie *baddie;
    
    baddie = self->data;
    objData = baddie->objdata;
    
    self->objhitInfo->unk58 |= 1;
    
    fsa->unk4.mode = 1;
    self->objhitInfo->unk5F = 9;
    self->objhitInfo->unk60 = 1;
    
    func_80028D2C(self);
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 8, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        main_set_bits(baddie->unk39E, 1);
        self->unkAF &= ~8;
        self->opacity = 0xFF;
        fsa->unk341 = 1;
        fsa->animTickDelta = (baddie->unk3B8 / 50000.0f) + 0.0025f;
    }
    
    if (fsa->unk33A) {
        baddie->unk3B6 = 1;
    }
    
    if (fsa->unk308 & 0x200) {
        fsa->unk308 &= ~0x200;
        objData->unk44 |= 0x20;
    }
    
    objData->unk44 |= 4;
    if (self->animProgress < 0.4f) {
        objData->unk44 |= 8;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, rand_next(0, 1), data_C);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, data_1C);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    
    return 0;
}

// offset: 0x19F4 | func: 17
s32 dll_216_func_19F4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 pad;
    Baddie* baddie;
    DLL216_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    self->objhitInfo->unk58 |= 1;
    fsa->unk4.mode = 1;
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0xB, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        main_set_bits(baddie->unk39E, 1);
        self->unkAF &= 0xFFF7;
        self->opacity = 0xFF;
        fsa->unk341 = 1;
        fsa->animTickDelta = (baddie->unk3B8 / 10000.0f) + 0.012f;
    }
    
    if (fsa->unk33A) {
        baddie->unk3B6 = 1;
    }

    if (fsa->unk308 & 0x200) {
        fsa->unk308 &= ~0x200;
        objData->unk44 |= 0x20;
    }

    objData->unk44 |= 4;
    if (self->animProgress < 0.4f) {
        objData->unk44 |= 8;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, rand_next(0, 1), data_C);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, data_1C);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    
    return 0;
}

// offset: 0x1C34 | func: 18
s32 dll_216_func_1C34(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL216_DataActual* objData;

    baddie = self->data;
    
    if ((fsa->prevAnimState != 4) && fsa->enteredAnimState) {
        func_80023D30(self, 0xE, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    objData = baddie->objdata;
    objData->unk44 |= 4 | 8;
    
    if (fsa->enteredAnimState) {
        self->objhitInfo->unk58 &= ~1;
        fsa->animTickDelta = 0.01f;
        fsa->unk278 = 0.0f;
    }
    
    if (fsa->unk33A) {
        main_set_bits(baddie->unk39E, 0);
        func_80023D30(self, 8, 0.0f, 0);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        baddie->unk3B6 = 0;
        self->unkAF |= 8;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, rand_next(3, 4), data_1C);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, data_1C);
    
    return 0;
}

// offset: 0x1DEC | func: 19
s32 dll_216_func_1DEC(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 pad[2];
    DLL216_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;
    self->objhitInfo->unk5F = 0xA;
    self->objhitInfo->unk60 = 1;

    func_80028D2C(self);
    
    if (fsa->enteredAnimState) {
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 4, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.008f;
    if (fsa->unk308 & 0x200) {
        fsa->unk308 &= ~0x200;
        objData->unk44 |= 0x10;
    }
    objData->unk44 |= (4 | 8);
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, rand_next(0, 1), data_0);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, data_1C);
    
    return 0;
}

// offset: 0x1F74 | func: 20
s32 dll_216_func_1F74(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL216_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 9, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    objData = baddie->objdata;
    objData->unk44 |= (4 | 8);

    if (fsa->enteredAnimState) {
        self->unkAF |= 8;
        baddie->unk3B6 = 4;
    }
    
    self->srt.yaw += ((fsa->unk32A * gUpdateRateF) / 12.0f) * 182.0f;
    
    fsa->animTickDelta = 0.01f;
    fsa->unk278 = 1.0f;
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, data_1C);
    
    return 0;
}

// offset: 0x20B4 | func: 21
s32 dll_216_func_20B4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL216_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    objData->unk44 |= 4;
    
    self->objhitInfo->unk5F = 0xA;
    self->objhitInfo->unk60 = 1;

    func_80028D2C(self);
    if (fsa->enteredAnimState) {
        if (rand_next(0, 1)) {
            data_130 = rand_next(0, 2);
            if (fsa->enteredAnimState) {
                func_80023D30(self, 6, 0.0f, 0);
                fsa->unk33A = 0;
            }
        } else {
            data_130 = 3;
            if (fsa->enteredAnimState) {
                func_80023D30(self, 0xA, 0.0f, 0);
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
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, data_130, data_C);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    
    return 0;
}

// offset: 0x2354 | func: 22
s32 dll_216_func_2354(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL216_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    objData->unk44 |= 4;
    self->objhitInfo->unk5F = 0xA;
    self->objhitInfo->unk60 = 1;
    
    func_80028D2C(self);
    
    if (fsa->enteredAnimState) {
        data_134 = rand_next(0, 2);
        if (rand_next(0, 1)) {
            if (fsa->enteredAnimState) {
                func_80023D30(self, 7, 0.0f, 0);
                fsa->unk33A = 0;
            }
        } else if (fsa->enteredAnimState) {
            func_80023D30(self, 3, 0.0f, 0);
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
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, data_134, data_C);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, data_134, data_C);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    
    return 0;
}

// offset: 0x2620 | func: 23
s32 dll_216_func_2620(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DLL216_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;
    objData->unk44 |= (4 | 8);
    
    if (fsa->enteredAnimState) {
        if (fsa->enteredAnimState) { //?
            func_80023D30(self, 0xF, 0.0f, 0);
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
s32 dll_216_func_2778(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    DLL216_DataActual* objData = baddie->objdata;

    objData->unk44 |= 4;

    fsa->animTickDelta = 0.01f;
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 5, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    
    return 0;
}

// offset: 0x2834 | func: 25
s32 dll_216_func_2834(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DLL216_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;
    objData->unk44 |= 4;
    fsa->animTickDelta = 0.01f;
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0xA, 0.0f, 0);
        fsa->unk33A = 0;
    }

    fsa->unk341 = 1;
    
    if (fsa->unk308 & 1) {
        objData = baddie->objdata;
        fsa->unk308 &= ~1;
        objData->unk44 |= 2;
        gDLL_6_AMSFX->vtbl->play(self, data_C[3], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    
    return 0;
}

// offset: 0x2964 | func: 26
s32 dll_216_func_2964(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DLL216_DataActual* objData;
    Baddie* baddie;
    Object* weapon;

    baddie = self->data;
    objData = baddie->objdata;
    objData->unk44 |= 4;
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0, 0.0f, 0U);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        weapon = get_player()->linkedObject;
        if (((DLL_Unknown*)weapon->dll)->vtbl->func[16].withOneArgS32(weapon)) {
            gDLL_6_AMSFX->vtbl->play(self, data_50[rand_next(3, 4)], MAX_VOLUME, NULL, NULL, 0, NULL);
        } else {
            gDLL_6_AMSFX->vtbl->play(self, data_50[rand_next(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        gDLL_6_AMSFX->vtbl->play(self, data_0[rand_next(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.015f;
    fsa->unk278 = 0.0f;
    
    return 0;
}

// offset: 0x2B64 | func: 27
s32 dll_216_func_2B64(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DLL216_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        self->unkAF |= 8;
        
        if (fsa->enteredAnimState) {
            func_80023D30(self, 2, 0.0f, 0);
            fsa->unk33A = 0;
        }
        
        baddie->unk3B6 = 2;
        fsa->unk341 = 1;
        fsa->animTickDelta = 0.015f;
    } else if (fsa->unk33A) {
        baddie->unk3B6 = 3;
    }
    
    objData = baddie->objdata;
    objData->unk44 |= 4;

    if (fsa->unk308 & 0x200) {
        fsa->unk308 &= ~0x200;
        objData->unk44 |= 0x10;
    }
    
    objData->unk44 |= 4 | 8;
    fsa->unk278 = self->animProgress;
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, data_1C);
    
    return 0;
}

// offset: 0x2CA4 | func: 28
s32 dll_216_func_2CA4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL216_DataActual* objData;

    baddie = self->data;
    
    self->objhitInfo->unk5F = 0xA;
    self->objhitInfo->unk60 = 1;
    func_80028D2C(self);
    
    if (baddie->unk3B8 > 50) {
        if (fsa->enteredAnimState) {
            func_80023D30(self, 4, 0.0f, 0);
            fsa->unk33A = 0;
        }
    } else if (fsa->enteredAnimState) {
        func_80023D30(self, 0xE, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.008f;
    
    objData = baddie->objdata;
    objData->unk44 |= (4 | 8);
    
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    if ((baddie->unk3B0 & 2) == FALSE) {
        fsa->unk278 = self->animProgress + -1.0f;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, rand_next(0, 1), data_0);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, data_1C);
    
    return 0;
}

// offset: 0x2E78 | func: 29
s32 dll_216_func_2E78(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.008f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 1, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if ((fsa->unk34A & 1) == FALSE) {
        gDLL_6_AMSFX->vtbl->play(self, data_50[rand_next(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play(self, 0xB21, MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play(self, data_0[2], MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 1;
    }
    
    if (((fsa->unk34A & 2) == FALSE) && (self->animProgress > 0.3f)) {
        gDLL_6_AMSFX->vtbl->play(self, 0xB1F, MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 2;
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, -1, 0);
    }
    
    return 0;
}

// offset: 0x309C | func: 30
s32 dll_216_func_309C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->target != NULL) {
        if (fsa->enteredLogicState) {
            fsa->unk27C = 0.0f;
            fsa->unk278 = 0.0f;
            if (baddie->unk3B8 >= 0x33) {
                if ((fsa->targetDist < (baddie->unk3E2 * 0.5f)) || (baddie->unk3B0 & 2)) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0);
                } else {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
                }
            } else {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
            }
        }
        
        if (fsa->unk33A != 0) {
            gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
            if (!(gDLL_33_BaddieControl->vtbl->func5(self, fsa, 75.0f) & 1)) {
                return 5;
            }

            if ((fsa->targetDist < (baddie->unk3E2 * 0.5f)) || (baddie->unk3B0 & 2)) {
                return 8;
            } else {
                return 7;
            }
        }
    }
    
    return 0;
}

// offset: 0x32E4 | func: 31
s32 dll_216_func_32E4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->hitpoints <= 0) {
        return 3;
    }
    
    if (fsa->unk33A) {
        if (fsa->animState == 0xC) {
            if (baddie->unk3B8 >= 0x33) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0);
            } else {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
            }
        } else {
            return 8;
        }
    }
    
    return 0;
}

// offset: 0x33A8 | func: 32
s32 dll_216_func_33A8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0xD);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        func_800267A4(self);
        self->unkAF |= 8;
    } else if (fsa->unk33A) {
        obj_send_mesg_many(0, 3, self, 0xE0000, self);
        if (self->setup == NULL) {
            obj_destroy_object(self);
        }
        
        return 4;
    }

    return 0;
}

// offset: 0x34A0 | func: 33
s32 dll_216_func_34A0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    if (fsa->enteredLogicState) {
        baddie = self->data;
        baddie->unk3B4 = 0;
        main_set_bits(baddie->unk39E, 0);
        main_set_bits(baddie->unk39C, 1);
    }
    
    return 0;
}

// offset: 0x3524 | func: 34
s32 dll_216_func_3524(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 2);
    }
    
    return 0;
}

// offset: 0x3580 | func: 35
s32 dll_216_func_3580(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 3);
    }
    
    if (fsa->unk33A) {
        if (fsa->animState == 3) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0);
        } else {
            return 8;
        }
    }
    
    return 0;
}

// offset: 0x3638 | func: 36
s32 dll_216_func_3638(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    Unk80009024* sp3C;

    baddie = self->data;
    
    if ((fsa->unk33A != 0) && !(gDLL_33_BaddieControl->vtbl->func5(self, fsa, 75.0f) & 1)) {
        return 5;
    }
    
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0xB);
    } else if (baddie->unk3B6 == 3) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 4);
    } else if (baddie->unk3B6 == 4) {
        if ((fsa->targetDist < 110.0f) && fsa->unk33A) {
            if (baddie->unk3B8 >= 0x33) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0);
            } else {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
            }
        }
    } else if (baddie->unk3B6 == 1) {
        return 8;
    }
    
    fsa->xAnalogInput = 0.0f;
    fsa->yAnalogInput = 0.0f;

    
    sp3C = &baddie->unk34C;
    
    bcopy(&self->srt.transl, sp3C, sizeof(Vec3f));
    bcopy(&fsa->target->srt.transl, &baddie->unk34C.unkC, sizeof(Vec3f));
    func_80009024(sp3C, &baddie->unk374);
    
    if (sp3C->unk25 == 0) {
        gDLL_18_objfsa->vtbl->func6(self, fsa, sp3C->unk18.x, sp3C->unk18.f[2], 0.0f, 0.0f, 60.0f);
    } else {
        gDLL_18_objfsa->vtbl->func6(self, fsa, sp3C->unk18.x, sp3C->unk18.f[2], 15.0f, 30.0f, 60.0f);
    }
    
    if (fsa->logicStateTime >= 0x79) {
        if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2, 1)) {
            return 5;
        }
    }

    return 0;
}

// offset: 0x396C | func: 37
s32 dll_216_func_396C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL216_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredLogicState) {
        if ((s32) fsa->targetDist > 55) {
            if ((baddie->unk3B0 & 2) == FALSE) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 7);
            } else {
                objData = baddie->objdata;
                if (baddie->unk3B0 & 0x10) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, data_40[objData->unk4++]);
                } else {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, data_30[objData->unk4++]);
                }
                if (objData->unk4 >= ARRAYCOUNT_S(data_40)) {
                    objData->unk4 = 0;
                }
            }
        } else if (fsa->animState == 6) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 5);
        } else {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 6);
        }
        
    } else if (fsa->unk33A) {
        if ((gDLL_33_BaddieControl->vtbl->func5(self, fsa, 75.0f) & 1) == FALSE) {
            return 5;
        }
        
        if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2, 1)) {
            return 5;
        }
        
        if ((s32) fsa->targetDist > 55) {
            if (!(baddie->unk3B0 & 2)) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 7);
            } else {
                objData = baddie->objdata;
                if (baddie->unk3B0 & 0x10) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, data_40[objData->unk4++]);
                } else {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, data_30[objData->unk4++]);
                }
                if (objData->unk4 >= ARRAYCOUNT_S(data_40)) {
                    objData->unk4 = 0;
                }
            }
        } else if (fsa->animState == 6) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 5);
        } else {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 6);
        }
        
    } else if ((fsa->animState == 7) && ((s32) fsa->targetDist < 55)) {
        if (fsa->animState == 6) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 5);
        } else {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 6);
        }
    }

    return 0;
}
