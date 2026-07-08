#include "common.h"
#include "dlls/objects/221_ChukaChuck.h"
#include "macros.h"
#include "sys/objlib.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12_0 : 1;
    u8 unk12_1 : 1;
    Object* unk14;
    u8 unk18;
    f32 unk1C;
} DLL688_DataActual;

typedef struct {
    Baddie unk0;
    DLL688_DataActual unk3FC;
} DLL688_Data;

/*0x0*/ static u32 data_0[] = {
    0x000005ff, 0x0000061b, 0x00000601
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
    0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000002, 0x00000005, 0x00000005, 0x00000005, 
    0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000005
};
/*0x9C*/ static u32 data_9C[] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffff00
};
/*0xB4*/ static u32 data_B4[] = {
    0x00000000, 0x00000001, 0x00000004
};
/*0xC0*/ static u32 data_C0[] = {
    0x40000000, 0x40800000, 0x3fc00000
};
/*0xCC*/ static u32 data_CC[] = {
    0x00000000, 0x00000007, 0x00000008, 0x00000007, 0x00000009, 0x00000001
};
/*0xE4*/ static u32 data_E4[] = {
    0x00260026, 0x00000020, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x3f800000
};
/*0x110*/ static u32 data_110[] = {
    0x02060167, 0x01650206
};
/*0x118*/ static u32 data_118[] = {
    0x02060167, 0x01650206
};

/*0x0*/ static ObjFSA_StateCallback bss_0[10];
/*0x28*/ static ObjFSA_StateCallback bss_28[2];
/*0x30*/ static u8 _bss_30[0x10];
/*0x40*/ static u8 bss_40[0x8];
/*0x48*/ static u8 bss_48[0x4];
/*0x4C*/ static u8 bss_4C[0x4];
/*0x50*/ static u8 bss_50[0x4];
/*0x54*/ static u8 bss_54[0x4];
/*0x58*/ static u8 _bss_58[0x8];

static void dll_688_func_F20(Object* self, Baddie* baddie);
static void dll_688_func_1048(Object* self, Baddie* baddie);

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/688_DBstealerworm/dll_688_func_0.s")

// offset: 0xE4 | ctor
void dll_688_ctor(void* dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/688_DBstealerworm/dll_688_ctor.s")

// offset: 0x124 | dtor
void dll_688_dtor(void* dll) { }

// offset: 0x130 | func: 1 | export: 0
void dll_688_setup(Object* self, ObjSetup* setup, s32 reset);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/688_DBstealerworm/dll_688_setup.s")

// offset: 0x2E8 | func: 2 | export: 1
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/688_DBstealerworm/dll_688_control.s")
#else

static void dll_688_func_72C(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {}
static void dll_688_func_A74(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {}
static void dll_688_func_BA0(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {}

void dll_688_control(Object* self) {
    Baddie* baddie;
    ObjFSA_Data* fsa;
    Baddie_Setup* objSetup;
    f32 time;

    baddie = self->data;
    fsa = &baddie->fsa;
    objSetup = (Baddie_Setup*)self->setup;
    
    self->unkAF |= 8;
    
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
        baddie->unk3B6 = 0;
        return;
    }
    
    if (baddie->unk3B0 & 0x10) {
        if (gDLL_7_Newday->vtbl->func8(&time) == 0) {
            baddie->unk3B6 = 0;
            return;
        }
    }
    
    dll_688_func_72C(self, baddie, fsa);
    if (baddie->unk3B6 == 0) {
        dll_688_func_BA0(self, baddie, fsa);
    } else {
        dll_688_func_A74(self, baddie, fsa);
    }
}
#endif

// offset: 0x474 | func: 3 | export: 2
void dll_688_update(Object* self) {
    gDLL_18_objfsa->vtbl->func2(self, self->data, bss_0);
}

// offset: 0x4C4 | func: 4 | export: 3
void dll_688_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie;
    DLL688_DataActual* objData;
    Object* target;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (objData->unk14) {
        objData->unk14->srt.transl.x = self->srt.transl.x;
        objData->unk14->srt.transl.y = self->srt.transl.f[1];
        objData->unk14->srt.transl.z = self->srt.transl.f[2];
        objData->unk14->srt.transl.y += 30.0f;
    }
    
    if (visibility && (self->unkDC == 0) && baddie->unk3B6) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);

        target = objData->unk14;
        if ((target != NULL) && !(objData->unk14->stateFlags & 0x40)) {
            func_80031F6C(self, 3, &target->srt.transl.x, &target->srt.transl.y, &target->srt.transl.z, 0);
            draw_object(objData->unk14, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0x614 | func: 5 | export: 4
void dll_688_free(Object* self, s32 onlySelf) {
    Baddie* baddie;

    baddie = self->data;
    obj_free_object_type(self, OBJTYPE_Baddie);

    if (self->linkedObject != NULL) {
        obj_destroy_object(self->linkedObject);
        self->linkedObject = NULL;
    }
    
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
}

// offset: 0x6B8 | func: 6 | export: 5
u32 dll_688_get_model_flags(Object* self) {
    return (MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_1);
}

// offset: 0x6C8 | func: 7 | export: 6
u32 dll_688_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DLL688_Data);
}

// offset: 0x6DC | func: 8 | export: 7
s16 dll_688_func_6DC(Object* self, s32 unused) {
    Baddie* baddie = self->data;
    return baddie->fsa.animState;
}

// offset: 0x6F0 | func: 9 | export: 8
void dll_688_func_6F0(Object* self, u8 message, s32 arg2) {
    Baddie* baddie = self->data;

    switch (message) {
    case 0x80:
        break;
    case 0x81:
        baddie->unk3B0 &= ~4;
        break;
    default:
        STUBBED_PRINTF("BADDIE:SnowWorm Unknown message [%d]\n", message); //@typo: copy-pasted from SnowWorm?
        break;
    }
}

// offset: 0x72C | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/688_DBstealerworm/dll_688_func_72C.s")

// offset: 0xA74 | func: 11
void dll_688_func_A74(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    DLL688_DataActual* objData = baddie->objdata;
    
    dll_688_func_F20(self, baddie);
    
    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.17f, 1);
    
    if (!(objData->unk11 & 4)) {
        gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, gUpdateRateF, 4);
    }
    
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, bss_0, bss_28);
    self->animObj = baddie->unk3AC;
}

// offset: 0xBA0 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/688_DBstealerworm/dll_688_func_BA0.s")

// offset: 0xF20 | func: 13
void dll_688_func_F20(Object* self, Baddie* baddie) {
    DLL688_DataActual* objData;
    s32 i;

    objData = baddie->objdata;
    
    if ((objData->unk10 & 1) && (baddie->fsa.target != NULL)) {
        dll_688_func_1048(self, baddie);
    }
    
    if (objData->unk10 & 2) {
        gDLL_17_partfx->vtbl->spawn(self, 0x345, NULL, 1, -1, NULL);
    }
    
    if (objData->unk10 & 4) {
        for (i = 0; i < 10; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_343, NULL, 1, -1, NULL);
        }
    }
    
    objData->unk10 = 0;
}

// offset: 0x1048 | func: 14
void dll_688_func_1048(Object* self, Baddie* baddie) {
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
    chuck = obj_create(&chuckSetup->base, (OBJINIT_STANDALONE | OBJINIT_FLAG4), -1, -1, NULL);
    
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
void dll_688_func_11C0(Object* self, s16 pitch, s16 roll) {
    SeqJoint* seqJoint;
    s32* seqJointID;
    s32 i;

    seqJointID = func_800349B0();
    
    for (i = 4, seqJointID++; i < 0x24; i += 4, seqJointID++) {
        seqJoint = (SeqJoint*)func_80034804(self, *seqJointID);
        if (seqJoint != NULL) {
            seqJoint->roll = roll;
            seqJoint->pitch = pitch;
        }
    }
}

// offset: 0x1284 | func: 16
s32 dll_688_func_1284(Object* self, Object* arg1, f32 arg2, f32 arg3) {
    Baddie* baddie; //3C
    f32 ySelf;
    f32 yTarget;
    f32 sp30; //30
    s16 angle;
    f32 var_fa0;

    
    baddie = self->data;
    angle = func_80031DD8(self, arg1, &sp30);
    if (gUpdateRateF == 0.0f) {
        return 0;
    }
    
    sp30 /= gUpdateRateF;
    if (sp30 < arg2) {
        ySelf = self->srt.transl.y;
        yTarget = arg1->srt.transl.y;
        
        if (yTarget <= ySelf) {
            var_fa0 = ySelf - yTarget;
        } else {
            var_fa0 = -(ySelf - yTarget);
        }
        
        if (var_fa0 < 3.0f) {
            return 1;
        }
    }

    baddie->fsa.unk278 = (1.0f - ((f32) angle / 65536.0f)) * arg3;
    baddie->fsa.unk27C = 0;
    
    return 0;
}

/*0x28*/ static const char str_28[] = " Fireing Projectile at Player ";
/*0x48*/ static const char str_48[] = " Error : main_fdt is zero \n";
/*0x64*/ static const char str_64[] = " WARNING AVOIDANCE FAILED: Obj Intersection \n";

// offset: 0x13B0 | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/688_DBstealerworm/dll_688_func_13B0.s")

// offset: 0x169C | func: 18
Object* dll_688_func_169C(Object* self, s32* objTypes, s32 count, f32* distance) {
    Object* obj;
    Object* matchObj;

    matchObj = NULL;

    for (count--; count >= 0; count--) {
        obj = obj_get_nearest_type_to(objTypes[count], self, distance);
        if (obj != NULL) {
            matchObj = obj;
        }
    }
    
    return matchObj;
}

// offset: 0x1734 | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/688_DBstealerworm/dll_688_func_1734.s")

// offset: 0x19B0 | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/688_DBstealerworm/dll_688_func_19B0.s")

// offset: 0x1B50 | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/688_DBstealerworm/dll_688_func_1B50.s")

// offset: 0x1DA4 | func: 22
s32 dll_688_func_1DA4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s8 entered = fsa->enteredAnimState;
    
    if (entered) {
        func_8002674C(self);
    }
    
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    fsa->animTickDelta = 0.01f;
    
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 5, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    
    return 0;
}

// offset: 0x1E8C | func: 23
s32 dll_688_func_1E8C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s8 entered;
    Baddie* baddie;
    DLL688_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        func_8002674C(self);
    }
    
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    fsa->animTickDelta = 0.01f;

    entered = fsa->enteredAnimState;
    if (entered) {
        func_80023D30(self, 0xA, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    objData = baddie->objdata;
    objData->unk10 |= 2;
    
    if (fsa->unk308 & 1) {
        fsa->unk308 &= ~1;
        objData->unk10 |= 1;
        gDLL_6_AMSFX->vtbl->play(self, data_C[3], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    if (fsa->unk33A != 0) {
        objData->unk12_1 = 1;
    }
    
    return 0;
}

// offset: 0x2010 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/688_DBstealerworm/dll_688_func_2010.s")

// offset: 0x2278 | func: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/688_DBstealerworm/dll_688_func_2278.s")

// offset: 0x25BC | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/688_DBstealerworm/dll_688_func_25BC.s")

// offset: 0x2924 | func: 27
s32 dll_688_func_2924(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DLL688_DataActual* objData;
    Baddie* baddie;

    if (1) {}
    
    baddie = self->data;
    
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    objData = baddie->objdata;
    objData->unk11 |= 4;
    
    if (fsa->unk308 & 0x200) {
        objData->unk14 = fsa->target;
        obj_send_mesg(objData->unk14, 0x11, self, (void*)0x12);
        obj_free_object_type(objData->unk14, 0x26);
    }
    
    fsa->unk341 = 0x12;
    
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 0x10, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->unk33A) {
        objData->unk12_1 = 1;
        objData->unk18++;
    }
    
    return 0;
}

// offset: 0x2A30 | func: 28
s32 dll_688_func_2A30(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL688_DataActual* objData;

    if (1) {}
    
    baddie = self->data;

    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    objData = baddie->objdata;
    objData->unk11 |= 4;
    
    if (fsa->unk308 & 0x200) {
        obj_send_mesg(objData->unk14, 0x11, self, (void*)0x11);
        obj_add_object_type(objData->unk14, OBJTYPE_38);
        objData->unk14 = NULL;
    }
    
    fsa->unk341 = 0x11;
    
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 0x12, 0.0f, 0);
        fsa->unk33A = 0;
    }

    if (fsa->unk33A) {
        objData->unk12_1 = 1;
        objData->unk18++;
    }
    
    return 0;
}

// offset: 0x2B38 | func: 29
s32 dll_688_func_2B38(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s8 entered = fsa->enteredLogicState;
    
    if (fsa->target != 0) {
        if (entered) {
            fsa->unk27C = 0.0f;
            fsa->unk278 = 0.0f;
            return 6;
        }
        if (fsa->unk33A != 0) {
            return 6;
        }
    }
    return 0;
}

// offset: 0x2B98 | func: 30
s32 dll_688_func_2B98(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL688_DataActual* objData;
    
    baddie = self->data;

    if (fsa->hitpoints <= 0) {
        return 3;
    }
    
    if (fsa->unk33A) {
        objData = baddie->objdata;
        objData->unk1C += 170.0f;
        return 6;
    }
    
    return 0;
}

// offset: 0x2BF8 | func: 31
s32 dll_688_func_2BF8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie *baddie;
    DLL688_DataActual *objData;
    s8 entered;
    
    baddie = self->data;
    entered = fsa->enteredLogicState;
    
    if (entered) {
        objData = baddie->objdata;
        objData->unk8 = 0.0f;
        objData->unkC = 0.0f;
        objData->unk0 = 0.0f;
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 6);
        objData->unk18 = 0;
    } else if (fsa->unk33A) {
        obj_send_mesg_many(0, 3, self, 0xE0000, self);
        
        if (self->setup == NULL) {
            obj_destroy_object(self);
        }
        
        return 4;
    }
    return 0;
}

// offset: 0x2CE0 | func: 32
s32 dll_688_func_2CE0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    s8 entered = fsa->enteredLogicState;
    
    if (entered) {
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, NO_GAMEBIT, 0);
    }
    
    return 0;
}

// offset: 0x2D48 | func: 33
s32 dll_688_func_2D48(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL688_DataActual* objData;
    s8 entered;

    baddie = self->data;

    entered = fsa->enteredLogicState;    
    if (entered) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
        objData = baddie->objdata;
        objData->unk8 = 0.0f;
        objData->unkC = 0.0f;
        objData->unk0 = 0.0f;
    }
    
    return 0;
}

// offset: 0x2DC4 | func: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/688_DBstealerworm/dll_688_func_2DC4.s")
