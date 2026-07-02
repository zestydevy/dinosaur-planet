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
} DLL222_Setup;

typedef struct {
    f32 unk0;
    f32 unk4;
    u8 unk8; //flags
    u8 unk9;
} DLL222_DataActual;

typedef struct {
    Baddie unk0;
    DLL222_DataActual unk3FC;
} DLL222_Data; //408

/*0x0*/ static u32 data_0[] = {
    0x000005ff, 0x00000600, 0x00000601
};
/*0xC*/ static u32 data_C[] = {
    0x000005fb, 0x000005fc, 0x000005fd, 0x000005fe
};
/*0x1C*/ static u32 data_1C[] = {
    0x00000602, 0x0000033a, 0x0000033c, 0x0000033d
};
/*0x2C*/ static u32 data_2C[] = {
    0x00000374, 0x00000375, 0x00000376, 0x0000025b, 0x0000025c
};
/*0x40*/ static u32 data_40[] = {
    0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000005, 
    0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000002, 0x00000005, 
    0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000005, 
    0x00000005, 0x00000005, 0x00000005, 0x00000005
};
/*0xB0*/ static s8 data_B0[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff
};

/*0x0*/ static ObjFSA_StateCallback bss_0[7];
/*0x20*/ static ObjFSA_StateCallback bss_20[6];

static void dll_222_func_0(void);
static void dll_222_func_8B8(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void dll_222_func_BF8(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void dll_222_func_D0C(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void dll_222_func_F50(Object* self, Baddie* baddie);
static void dll_222_func_106C(Object* self, Baddie* baddie);
static s32 dll_222_func_13F8(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_222_func_164C(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_222_func_17DC(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_222_func_1A1C(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_222_func_1B04(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_222_func_1C80(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_222_func_1E80(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_222_func_20A4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_222_func_2134(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_222_func_2170(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_222_func_227C(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_222_func_2318(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_222_func_2390(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
void dll_222_func_0(void) {
    bss_0[0] = &dll_222_func_13F8;
    bss_0[1] = &dll_222_func_164C;
    bss_0[2] = &dll_222_func_17DC;
    bss_0[3] = &dll_222_func_1A1C;
    bss_0[4] = &dll_222_func_1B04;
    bss_0[5] = &dll_222_func_1C80;
    bss_0[6] = &dll_222_func_1E80;
    
    bss_20[0] = &dll_222_func_20A4;
    bss_20[1] = &dll_222_func_2134;
    bss_20[2] = &dll_222_func_2170;
    bss_20[3] = &dll_222_func_227C;
    bss_20[4] = &dll_222_func_2318;
    bss_20[5] = &dll_222_func_2390;
}

// offset: 0xC0 | ctor
void dll_222_ctor(void* dll) {
    dll_222_func_0();
}

// offset: 0x100 | dtor
void dll_222_dtor(void* dll) { }

// offset: 0x10C | func: 1 | export: 0
void dll_222_setup(Object* self, DLL222_Setup* objSetup, s32 reset) {
    u8 flags;
    DLL222_DataActual* objData;
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
    objData->unk0 = rand_next(10, 300);

    func_80023D30(self, 8, 0.0f, 0);
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, 0);
    baddie->fsa.logicState = 0;
    baddie->fsa.unk4.mode = 0;
    func_800267A4(self);
}

// offset: 0x250 | func: 2 | export: 1
void dll_222_control(Object* self) {
    Baddie* baddie;
    DLL222_Setup* objSetup;
    ObjFSA_Data* fsa;
    f32 time;

    baddie = self->data;
    objSetup = (DLL222_Setup*)self->setup;
    fsa = &baddie->fsa;
    
    if (self->unkDC) {
        if (((baddie->fsa.logicState != 3) || (baddie->unk3B0 & 1)) && 
            (gDLL_29_Gplay->vtbl->did_time_expire(objSetup->baddie.base.uID))
        ) {
            gDLL_33_BaddieControl->vtbl->setup(self, &objSetup->baddie, baddie, 7, 6, 0x102, 0x26, 20.0f);
            baddie->unk3B6 = 0;
            gDLL_6_AMSFX->vtbl->play(self, 0xB20, MAX_VOLUME, NULL, NULL, 0, NULL);
            func_80023D30(self, 8, 0.0f, 0x10);
            baddie->fsa.unk33A = 0;
            self->opacity = 0xFF;
            self->unkAF |= 8;
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
    
    dll_222_func_8B8(self, baddie, fsa);
    
    if (baddie->unk3B6 == 0) {
        dll_222_func_D0C(self, baddie, fsa);
    } else {
        dll_222_func_BF8(self, baddie, fsa);
    }
    
    self->srt.transl.y = objSetup->baddie.base.y - 2.0f;
}

// offset: 0x4F0 | func: 3 | export: 2
void dll_222_update(Object* self) { }

// offset: 0x4FC | func: 4 | export: 3
void dll_222_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie = self->data;
    
    if (!visibility) {
        return;
    } 
    
    if (self->unkDC || !baddie->unk3B6) {
        return;
    }
    
    if (baddie->unk3E8) {
        func_80036FBC(200, 0, 0, baddie->unk3E8);
    }
    
    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    
    if (baddie->unk3B2 & 0x60) {
        if (baddie->unk3B2 & 0x20) {
            gDLL_32->vtbl->func2(self, 0x330, &baddie->unk3E8);
            gDLL_32->vtbl->func2(self, 0x330, &baddie->unk3E8);
        }
        gDLL_32->vtbl->func2(self, 0x32F, &baddie->unk3E8);
    }
}

// offset: 0x6E8 | func: 5 | export: 4
void dll_222_free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    obj_free_object_type(self, OBJTYPE_Baddie);

    if (self->linkedObject != NULL) {
        obj_destroy_object(self->linkedObject);
        self->linkedObject = NULL;
    }

    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
}

// offset: 0x78C | func: 6 | export: 5
u32 dll_222_get_model_flags(Object* self) {
    return MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x79C | func: 7 | export: 6
u32 dll_222_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DLL222_Data);
}

// offset: 0x7B0 | func: 8 | export: 7
s16 dll_222_func_7B0(Object* self, s32 unused) {
    Baddie* baddie = self->data;
    return baddie->fsa.animState;
}

// offset: 0x7C4 | func: 9 | export: 8
void dll_222_func_7C4(Object* self, u8 message, s32 unused) {
    Baddie* baddie;
    DLL222_DataActual* objData;

    baddie = self->data;
    
    switch (message) {
    case 0x80:
        objData = baddie->objdata;
        objData->unk9 |= 2;
        gDLL_6_AMSFX->vtbl->play(self, data_1C[1], MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, 1);
        baddie->fsa.logicState = 4;
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
void dll_222_func_8B8(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {    
    Vec3f d;
    s32 scaleIdx;
    s32 i;
/*0xCC*/ s16 data_CC[] = { 0x0206, 0x0167, 0x0165, 0x0206 };
/*0xD4*/ s16 data_D4[] = { 0x0206, 0x0167, 0x0165, 0x0206 };
    Object* player;
    Object* weapon;
/*0x38*/ static SRT bss_38;
    
    player = get_player();
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
    
    if (gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, data_40, data_B0, 1, &baddie->unk3A8, &bss_38)) {
        weapon = player->linkedObject;
        scaleIdx = ((DLL_Unknown*)weapon->dll)->vtbl->func[19].withOneArgS32(weapon);
        if (scaleIdx > 3) {
            scaleIdx = 3;
        }
        
        bss_38.scale = data_CC[scaleIdx];
        gDLL_17_partfx->vtbl->spawn(self, 0x323, &bss_38, 0x200001, -1, NULL);
        bss_38.transl.x -= self->srt.transl.x;
        bss_38.transl.y -= self->srt.transl.y;
        bss_38.transl.z -= self->srt.transl.z;
        bss_38.scale = data_D4[scaleIdx];

        for (i = 0; i < 4; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x324, &bss_38, 2, -1, NULL);
        }
    }
}

// offset: 0xBF8 | func: 11
void dll_222_func_BF8(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    dll_222_func_F50(self, baddie);
    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.0f, -1);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, bss_0, bss_20);
    self->animObj = baddie->unk3AC;
}

// offset: 0xD0C | func: 12
void dll_222_func_D0C(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    DLL222_DataActual* objData;
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
    
    player = get_player();
    if (player != NULL) {
        d.f[0] = player->globalPosition.x - self->globalPosition.x;
        d.f[1] = player->globalPosition.y - self->globalPosition.y;
        d.f[2] = player->globalPosition.z - self->globalPosition.z;
        distance = sqrtf(SQ(d.f[0]) + SQ(d.f[1]) + SQ(d.f[2]));
    } else {
        distance = 10000.0f;
    }
    
    if ((objData->unk4 < objData->unk0) && (distance < 400.0f)) {
        gDLL_6_AMSFX->vtbl->play(self, data_1C[1], 0x1E, NULL, NULL, 0, NULL);
        objData->unk4 += rand_next(50, 250);
    }
    objData->unk0 += gUpdateRateF;
}

// offset: 0xF50 | func: 13
void dll_222_func_F50(Object* self, Baddie* baddie) {
    DLL222_DataActual* objData;
    s32 i;

    objData = baddie->objdata;
    
    if (objData->unk8 & 1) {
        dll_222_func_106C(self, baddie);
    }
    
    if (objData->unk8 & 2) {
        gDLL_17_partfx->vtbl->spawn(self, 0x345, NULL, 1, -1, NULL);
    }
    
    if (objData->unk8 & 4) {
        for (i = 0; i < 10; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x343, NULL, 1, -1, NULL);
        }
    }
    
    objData->unk8 = 0;
}

// offset: 0x106C | func: 14
void dll_222_func_106C(Object* self, Baddie* baddie) {
    ChukaChuck_Setup* chuckSetup;
    Object* chuck;
    f32 throwFactor;
    f32 throwTime; //Duration of throw in frames

    chuckSetup = obj_alloc_setup(sizeof(ChukaChuck_Setup), OBJ_IceBallSmall);
    chuckSetup->base.x = self->srt.transl.x;
    chuckSetup->base.y = self->srt.transl.y + 15.0f;
    chuckSetup->base.z = self->srt.transl.z;
    chuckSetup->base.loadFlags = OBJSETUP_LOAD_LEVEL;
    chuckSetup->base.fadeFlags = OBJSETUP_FADE_MANUAL;
    chuckSetup->base.loadDistance = 0xFF;
    chuckSetup->base.fadeDistance = 0xFF;
    chuck = obj_create(&chuckSetup->base, 5, -1, -1, NULL);
    
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
static void dll_222_func_11E4(Object* self, s32* alliesLowestAnimState, s32* numAllies, s32* numAlliesInAnimState4) {
    s32 index;
    s32 count;
    Object* obj;
    Object** objects;
    s32 animState;

    *numAllies = 0;
    *numAlliesInAnimState4 = 0;
    *alliesLowestAnimState = 0;
    
    for (objects = get_world_objects(&index, &count); index < count; index++) {
        obj = objects[index];
        if ((self != obj) && (obj->id == OBJ_SnowWormSmall)) {
            *numAllies += 1;
            obj = objects[index];
            animState = ((DLL_222_SnowWormSmall*)obj->dll)->vtbl->get_anim_state(obj, 0);
            
            if (*alliesLowestAnimState < animState) {
                *alliesLowestAnimState = animState;
            }
            if (animState == 4) {
                *numAlliesInAnimState4 += 1;
            }
        }
    }
}

// offset: 0x132C | func: 16
//SnowWormSmall_worm_to_worm_communication
static void dll_222_func_132C(Object* self) {
    s32 index;
    s32 count;
    Object* obj;
    Object** objects;

    for (objects = get_world_objects(&index, &count); index < count; index++) {
        obj = objects[index];
        if ((self != obj) && (obj->id == OBJ_SnowWormSmall)) {
            ((DLL_222_SnowWormSmall*)obj->dll)->vtbl->receive_message(obj, 0x81, 0);
        }
    }
}

// offset: 0x13F8 | func: 17
s32 dll_222_func_13F8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 pad;
    Baddie* baddie;
    DLL222_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0xB, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState != 0) {
        fsa->unk4.mode = 1;
        main_set_bits(baddie->unk39E, 1);
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
    
    if (fsa->unk33A != 0) {
        baddie->unk3B6 = 1;
    }

    if (fsa->unk308 & 0x200) {
        fsa->unk308 &= ~0x200;
        objData->unk8 |= 4;
    }
    
    if (self->animProgress < 0.7f) {
        objData->unk8 |= 2;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, rand_next(0, 1), data_C);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, data_1C);
    
    return 0;
}

// offset: 0x164C | func: 18
s32 dll_222_func_164C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL222_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0xE, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (self->animProgress > 0.25f) {
        objData = baddie->objdata;
        objData->unk8 |= 2;
    }
    
    if (fsa->enteredAnimState) {
        func_800267A4(self);
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
        
        if ((objData->unk9 & 2) == FALSE) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
        }
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 0, data_1C);
    
    return 0;
}

// offset: 0x17DC | func: 19
s32 dll_222_func_17DC(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        func_8002674C(self);
    }
    
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    
    if (fsa->enteredAnimState) {
        dll_222_func_132C(self);
        
        if (rand_next(0, 1)) {
            if (fsa->enteredAnimState) {
                func_80023D30(self, 6, 0.0f, 0);
                fsa->unk33A = 0;
            }
        } else {
            if (fsa->enteredAnimState) {
                func_80023D30(self, 7, 0.0f, 0);
                fsa->unk33A = 0;
            }
        }
        
        fsa->unk341 = 1;
        fsa->animTickDelta = (baddie->unk3B8 / 20000.0f) + 0.005f;
    }
    
    fsa->unk278 = 0.0f;
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, rand_next(0, 2), data_C);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, rand_next(0, 2), data_C);
    
    return 0;
}

// offset: 0x1A1C | func: 20
s32 dll_222_func_1A1C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_8002674C(self);
    }
    
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    fsa->animTickDelta = 0.01f;
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 5, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    
    return 0;
}

// offset: 0x1B04 | func: 21
s32 dll_222_func_1B04(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DLL222_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        func_8002674C(self);
    }
    
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    
    if (fsa->enteredAnimState) {
        dll_222_func_132C(self);
    }
    
    fsa->animTickDelta = 0.01f;
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0xA, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    if (fsa->unk308 & 1) {
        objData = baddie->objdata;
        fsa->unk308 &= ~1;
        objData->unk8 |= 1;
        gDLL_6_AMSFX->vtbl->play(self, data_C[3], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    return 0;
}

// offset: 0x1C80 | func: 22
s32 dll_222_func_1C80(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Object* weapon;

    if (fsa->enteredAnimState) {
        func_80023D30(self, 0, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        dll_222_func_132C(self);
        weapon = get_player()->linkedObject;
        if (((DLL_Unknown*)weapon->dll)->vtbl->func[16].withOneArgS32(weapon)) {
            gDLL_6_AMSFX->vtbl->play(self, data_2C[rand_next(3, 4)], MAX_VOLUME, NULL, NULL, 0, NULL);
        } else {
            gDLL_6_AMSFX->vtbl->play(self, data_2C[rand_next(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        gDLL_6_AMSFX->vtbl->play(self, data_0[rand_next(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.015f;
    fsa->unk278 = 0.0f;
    
    return 0;
}

// offset: 0x1E80 | func: 23
s32 dll_222_func_1E80(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    baddie = self->data;
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.008f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 1, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if ((fsa->unk34A & 1) == FALSE) {
        gDLL_6_AMSFX->vtbl->play(self, data_2C[rand_next(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play(self, 0xB21, MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play(self, data_0[2], MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 1;
    }
    
    if (((fsa->unk34A & 2) == FALSE) && (self->animProgress > 0.3f)) {
        gDLL_6_AMSFX->vtbl->play(self, 0xB1F, MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 2;
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, NO_GAMEBIT, 0);
    }
    
    return 0;
}

// offset: 0x20A4 | func: 24
s32 dll_222_func_20A4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->target != NULL) {
        if (fsa->enteredLogicState) {
            fsa->unk27C = 0.0f;
            fsa->unk278 = 0.0f;
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0);
        }
        
        if (fsa->unk33A) {
            return 6;
        }
    }
    
    return 0;
}

// offset: 0x2134 | func: 25
s32 dll_222_func_2134(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->hitpoints <= 0) {
        return 3;
    }
    if (fsa->unk33A != 0) {
        return 6;
    }
    return 0;
}

// offset: 0x2170 | func: 26
s32 dll_222_func_2170(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DLL222_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    
    if (fsa->enteredLogicState) {
        objData = baddie->objdata;
        objData->unk0 = 0.0f;
        objData->unk4 = 0.0f;
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 6);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        func_800267A4(self);
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    } else if (fsa->unk33A) {
        obj_send_mesg_many(0, 3, self, 0xE0000, self);
        if (self->setup == NULL) {
            obj_destroy_object(self);
        }
        return 4;
    }
    
    return 0;
}

// offset: 0x227C | func: 27
s32 dll_222_func_227C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    if (fsa->enteredLogicState) {
        baddie = self->data;
        baddie->unk3B4 = 0;

        if (baddie->unk39E != NO_GAMEBIT) {
            main_set_bits(baddie->unk39E, 0);
        }
        
        if (baddie->unk39C != NO_GAMEBIT) {
            main_set_bits(baddie->unk39C, 1);
        }
    }
    
    return 0;
}

// offset: 0x2318 | func: 28
s32 dll_222_func_2318(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL222_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
        objData = baddie->objdata;
        objData->unk0 = 0.0f;
        objData->unk4 = 0.0f;
    }
    
    return 0;
}

// offset: 0x2390 | func: 29
s32 dll_222_func_2390(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL222_DataActual* objData;
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

        dll_222_func_11E4(self, &alliesLowestAnimState, &numAllies, &numAlliesInAnimState4);
        
        random = rand_next(0, baddie->unk3B8);
        
        if ((alliesLowestAnimState >= 5) || (objData->unk9 & 1)) {
            if (baddie->unk3B0 & 2) {
                objData->unk9 |= 1;
            }
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 4);
        } else if (random > 0x20) {
            if (numAlliesInAnimState4 >= 2) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 2);
            } else {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 4);
            }
        } else if (random > 0x10) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 2);
        } else {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 3);
        }
    }
    
    return 0;
}
