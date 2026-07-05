#include "common.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "dlls/objects/221_ChukaChuck.h"

//"core_snowworm.c" seems to have been Rare's filename

typedef struct {
    u8 unk0[0x14];
    Vec3f unk14;
    u8 unk20[0x38 - 0x20];
    Vec3f unk38;
    u8 unk44;
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
/*0x30*/ static u32 data_30[] = {
    0x00050006, 0x00080006, 0x00050008, 0x00060000
};
/*0x40*/ static u32 data_40[] = {
    0x00080006, 0x00090008, 0x00060009, 0x00090000
};
/*0x50*/ static u32 data_50[] = {
    0x00000374, 0x00000375, 0x00000376, 0x0000025b, 0x0000025c
};
/*0x64*/ static u32 data_64[] = {
    0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 
    0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000c, 0x0000000a, 
    0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 
    0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a
};
/*0xD4*/ static u32 data_D4[] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
};
/*0xF0*/ static u32 data_F0[] = {
    0xffffff00, 0xff00ffc0, 0x965a5a64, 0xff5a0000
};
/*0x100*/ static u32 data_100[] = {
    0x00010200, 0x00000000, 0x00000000, 0x00000000, 0x02000300, 0x00000000, 0x03030400, 0x00000000
};
/*0x120*/ static u32 data_120[] = {
    0x02060167, 0x01650206
};
/*0x128*/ static u32 data_128[] = {
    0x02060167, 0x01650206
};
/*0x130*/ static u32 data_130 = 0x00000000;
/*0x134*/ static u32 data_134[] = {
    0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static ObjFSA_StateCallback bss_0[14];
/*0x38*/ static ObjFSA_StateCallback bss_38[2];
/*0x40*/ static u8 _bss_40[0x20];

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_0.s")

// offset: 0x12C | ctor
void dll_216_ctor(void* dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_ctor.s")

// offset: 0x16C | dtor
void dll_216_dtor(void* dll) { }

// offset: 0x178 | func: 1 | export: 0
void dll_216_setup(Object* self, ObjSetup* setup, s32 reset);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_setup.s")

// offset: 0x2C0 | func: 2 | export: 1
void dll_216_control(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_control.s")

// offset: 0x554 | func: 3 | export: 2
void dll_216_update(Object* self) {
    DLL216_Data* objData = self->data;
    gDLL_18_objfsa->vtbl->func2(self, &objData->unk0.fsa, bss_0);
}

// offset: 0x5A4 | func: 4 | export: 3
void dll_216_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_print.s")

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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_908.s")

// offset: 0xCE8 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_CE8.s")

// offset: 0xEBC | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_EBC.s")

// offset: 0x10D8 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_10D8.s")

// offset: 0x1374 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_1374.s")

// offset: 0x16C4 | func: 15
void dll_216_func_16C4(Object* self, DLL216_DataActual* objData) {
    ChukaChuck_Setup* iceSetup;
    Object* ice;

    iceSetup = obj_alloc_setup(sizeof(ChukaChuck_Setup), OBJ_IceBall);
    iceSetup->base.x = objData->unk14.x;
    iceSetup->base.y = objData->unk14.y;
    iceSetup->base.z = objData->unk14.z;
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_1790.s")

// offset: 0x19F4 | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_19F4.s")

// offset: 0x1C34 | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_1C34.s")

// offset: 0x1DEC | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_1DEC.s")

// offset: 0x1F74 | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_1F74.s")

// offset: 0x20B4 | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_20B4.s")

// offset: 0x2354 | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_2354.s")

// offset: 0x2620 | func: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_2620.s")

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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_2964.s")

// offset: 0x2B64 | func: 27
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_2B64.s")

// offset: 0x2CA4 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_2CA4.s")

// offset: 0x2E78 | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_2E78.s")

// offset: 0x309C | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_309C.s")

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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_3638.s")

// offset: 0x396C | func: 37
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/216_SnowWorm/dll_216_func_396C.s")
