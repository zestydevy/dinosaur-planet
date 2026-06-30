#include "common.h"
#include "sys/objmsg.h"
#include "sys/objanim.h"
#include "sys/objtype.h"
#include "dlls/objects/260_Pollen.h"

typedef struct {
    f32 unk0;
    s8 _unk4;
    u8 unk5;
    u8 unk6;
    u8 _unk7[0x404-0x07];
} DLL236_Data; //404

/*0x0*/ static u32 _data_0[] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffff0000
};
/*0x1C*/ static u32 _data_1C[] = {
    0x02060167, 0x01650206, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static u8 _bss_0[0x10];
/*0x10*/ static u8 _bss_10[0x10];

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/236_WG_Triffid/dll_236_func_0.s")

// offset: 0x5C | ctor
void dll_236_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/236_WG_Triffid/dll_236_ctor.s")

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
void dll_236_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/236_WG_Triffid/dll_236_control.s")

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
s32 dll_236_func_A20(Object* self, ObjFSA_Data* fsa, s8 arg2) {
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
s32 dll_236_func_B44(Object* self, ObjFSA_Data* arg1, s32 arg2) {
    Baddie* baddie;
    DLL236_Data* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    arg1->unk341 = 1;
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
s32 dll_236_func_C60(Object* self, ObjFSA_Data* fsaData, s32 arg2) {
    Baddie* baddie;
    DLL236_Data* objData;
    void* sp20;

    baddie = self->data;
    objData = baddie->objdata;
    
    fsaData->unk341 = 0;
    if (objData->unk5 != 0) {
        objData->unk5 = 0;
        objData->unk0 = 0.01f;
        func_80023D30(self, 0, 0.5f, 0);
        func_800267A4(self);
    }
    
    self->objhitInfo->unk5E = 0;
    
    if ((self->curModAnimId == 0) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsaData, 3);
        objData->unk5 = 1;
    }
    
    return 0;
}

// offset: 0xD64 | func: 12
s32 dll_236_func_D64(Object* self, ObjFSA_Data* arg1, s8 arg2) {
    Baddie* baddie;
    DLL236_Data* triffid;

    baddie = self->data;
    triffid = baddie->objdata;

    arg1->unk341 = 0;

    if (triffid->unk5 != 0) {
        triffid->unk5 = 0;
        obj_send_mesg_many(0, OBJMSG_SEND_IGNORE_SENDER | OBJMSG_SEND_ALL, self, 0xE0000U, self);
        gDLL_33_BaddieControl->vtbl->func18(self, baddie->unk3E0, -1, 0);
        gDLL_18_objfsa->vtbl->func21(self, arg1, PARTICLE_3C, 0xA, 0);
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
s32 dll_236_func_EA0(s32 arg0, s32 arg1, s32 arg2) {
    return 0;
} 

