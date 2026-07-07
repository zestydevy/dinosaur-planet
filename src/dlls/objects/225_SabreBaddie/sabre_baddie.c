#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/33_BaddieControl.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/rand.h"
#include "types.h"
#include "prevent_bss_reordering.h"

typedef struct {
    u8 unk0;
} DLL255_DataActual;

typedef struct {
    Baddie unk0;
} DLL255_Data;

/*0x0*/ static u32 _data_0[] = {
    0x00990000, 0x0000000b, 0x01ff0000, 0x3c9374bc, 0xbf800000, 0xbf800000, 0xbf800000, 0x3e4ccccd, 
    0x3f000000, 0x3e99999a, 0x3eb33333, 0x06610000, 0x00000000, 0x00000000, 0x00000000, 0x009a0000, 
    0x0000000c, 0x01020000, 0x3c54fdf4, 0x3dcccccd, 0x3ecccccd, 0x3f000000, 0x3e9eb852, 0x3efae148, 
    0x3e800000, 0x3e99999a, 0x06620000, 0x00000000, 0x00000000, 0x00000000, 0x009b0000, 0x0000000a, 
    0x01ff0000, 0x3c449ba6, 0xbf800000, 0xbf800000, 0xbf800000, 0x3e99999a, 0x3f0a3d71, 0x3e851eb8, 
    0x3eb851ec, 0x06630000, 0x00000000, 0x00000000, 0x00000000, 0x009c0000, 0x0000000b, 0x01ff0000, 
    0x3c449ba6, 0x00000000, 0x3eb33333, 0x3f1eb852, 0x3ea3d70a, 0x3f19999a, 0x3e851eb8, 0x3ed70a3d, 
    0x06610000, 0x00000000, 0x00000000, 0x00000000, 0x009d0000, 0x00000009, 0x01ff0000, 0x3c5d2f1b, 
    0x00000000, 0x3ecccccd, 0x3f0ccccd, 0x3dcccccd, 0x3f07ae14, 0x3e75c28f, 0x3eb33333, 0x06610000, 
    0x00000000, 0x00000000, 0x00000000, 0x009e0000, 0x0000000c, 0x01ff0000, 0x3c343958, 0xbf800000, 
    0xbf800000, 0xbf800000, 0x3e851eb8, 0x3ebd70a4, 0x3eb33333, 0x3eeb851f, 0x06620000, 0x00000000, 
    0x00000000, 0x00000000
};
/*0x168*/ static s32 _data_168[] = {
    0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 
    0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 
    0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 
    0x0000000c, 0x0000000c
};
/*0x1D0*/ static s8 _data_1D0[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0x00, 0x00
};
/*0x1EC*/ static s16 _data_1EC[] = {
    0x0089, 0x008c, 0x0090
};
/*0x1F4*/ static f32 _data_1F4[] = {
    0, 0.05, 0.03, 0.7, 0.68, 1.05
};
/*0x20C*/ static u16 _data_20C[] = {
    0x067a, 0x067b, 0x067c
};

/*0x0*/ static ObjFSA_StateCallback _bss_0[14];
/*0x38*/ static ObjFSA_StateCallback _bss_38[10];

static int dll_225_func_5C8(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);
static void dll_225_func_864(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void dll_225_func_A84(Object* self, AnimObj_Data* animData, Baddie* baddie, ObjFSA_Data* fsa);
static void dll_225_func_C10(Object* self, Baddie* baddie, ObjFSA_Data* fsa);

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_0.s")

// offset: 0x12C | ctor
void dll_225_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_ctor.s")

// offset: 0x16C | dtor
void dll_225_dtor(void *dll) { }

// offset: 0x178 | func: 1 | export: 0
void dll_225_setup(Object* self, Baddie_Setup* objSetup, s32 reset) {
    DLL255_DataActual* objData;
    Baddie* baddie;
    u8 flags;

    baddie = self->data;
    
    flags = 2 | 4;
    if (reset) {
        flags = 1 | 2 | 4;
    }
    if (!(objSetup->unk2B & 1)) {
        flags |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 0xE, 8, 0x102, flags, 20.0f);
    
    self->animCallback = dll_225_func_5C8;
    
    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, 0);
    
    baddie->fsa.logicState = 0;
    baddie->fsa.flags |= 0x20000;
    
    objData = baddie->objdata;
    if (objData == NULL) {
        baddie->objdata = mmAlloc(sizeof(DLL255_DataActual), ALLOC_TAG_TEST_COL, NULL);
        objData = baddie->objdata;
    }
    objData->unk0 = 0;
    
    obj_add_object_type(self, OBJTYPE_Baddie);
}

// offset: 0x2A8 | func: 2 | export: 1
void dll_225_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_control.s")

// offset: 0x450 | func: 3 | export: 2
void dll_225_update(Object* self) {
    gDLL_18_objfsa->vtbl->func2(self, self->data, _bss_0);
}

// offset: 0x4A0 | func: 4 | export: 3
void dll_225_print(Object* arg0, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility && (arg0->unkDC == 0)) {
        draw_object(arg0, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x500 | func: 5 | export: 4
void dll_225_free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    obj_free_object_type(self, OBJTYPE_Baddie);

    if (self->linkedObject != NULL) {
        obj_destroy_object(self->linkedObject);
        self->linkedObject = NULL;
    }
    
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 1);
}

// offset: 0x5A4 | func: 6 | export: 5
u32 dll_225_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_8 | MODFLAGS_EVENTS | MODFLAGS_100;
}

// offset: 0x5B4 | func: 7 | export: 6
u32 dll_225_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DLL255_Data);
}

// offset: 0x5C8 | func: 8
int dll_225_func_5C8(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    Baddie_Setup* objSetup;
    Baddie* baddie;

    objSetup = (Baddie_Setup*)self->setup;
    baddie = self->data;
    
    if (self->unkDC) {
        return 0;
    }
    
    if (self->seqSlot != -1) {
        if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 1) == 0) {
            return 1;
        }
        
        dll_225_func_864(self, baddie, &baddie->fsa);
        
        if ((baddie->unk3A0 != NO_GAMEBIT) && main_get_bits(baddie->unk3A0)) {
            gDLL_3_Animation->vtbl->func21(animData, objSetup->unk2C);
            baddie->unk3A0 = -1;
        }

        switch (baddie->unk3B4) {
        case 2:
            animData->unk7A = 0;
            dll_225_func_A84(self, animData, baddie, &baddie->fsa);
            if (baddie->unk3B4 == 1) {
                baddie->fsa.logicState = 6;
                gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, 1.0f, 1.0f, _bss_0, _bss_38);
                animData->unk62 = 0;
            }
            break;
        case 1:
            if (gDLL_33_BaddieControl->vtbl->func12(self, animData, baddie, _bss_0, _bss_38, 0)) {
                gDLL_33_BaddieControl->vtbl->func10(self, &baddie->fsa, 0.17f, 1);
            }
            break;
        case 0:
        default:
            animData->unk7A = -1;
            animData->unk7A &= ~0x40;
            dll_225_func_C10(self, baddie, &baddie->fsa);
            break;
        }
    }
    
    if (self->seqSlot == -1) {
        baddie->unk3B2 |= 2;
        return 0;
    }
    
    return baddie->unk3B4 != 0;
}

// offset: 0x864 | func: 9
void dll_225_func_864(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    Vec3f d;
    Unk80009024* sp44;

    if (self->linkedObject != NULL) {
        self->linkedObject->parent = self->parent;
    }

    if (fsa->target != NULL) {
        d.f[0] = fsa->target->globalPosition.f[0] - self->globalPosition.f[0];
        d.f[1] = fsa->target->globalPosition.f[1] - self->globalPosition.f[1];
        d.f[2] = fsa->target->globalPosition.f[2] - self->globalPosition.f[2];
        fsa->targetDist = sqrtf(SQ(d.f[0]) + SQ(d.f[1]) + SQ(d.f[2]));
    }
    
    func_80032A08(self, &baddie->unk3BC);
    
    if (!(baddie->unk3B0 & 1)) {
        gDLL_33_BaddieControl->vtbl->func14(self, (Baddie*)fsa, &baddie->unk3B2, 4, 5, baddie->unk3A6, baddie->unk3A4);
    }
    
    gDLL_33_BaddieControl->vtbl->change_weapon(self, baddie);
    sp44 = &baddie->unk34C;
    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 0, 0, 0);
    if (gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, _data_168, _data_1D0, 1, &baddie->unk3A8, NULL) >= 4) {
        baddie->unk3B4 = 2;
        fsa->target = get_player();
    }
}

// offset: 0xA84 | func: 10
void dll_225_func_A84(Object* self, AnimObj_Data* animData, Baddie* baddie, ObjFSA_Data* fsa) {
    Baddie_Setup* objSetup;
    
    objSetup = (Baddie_Setup*)self->setup;
    self->objhitInfo->unk58 |= 1;
    
    if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2 + 0x28, 1)) {
        fsa->target = baddie->unk3AC;
        fsa->unk33D = 0;
        if (objSetup->unk24 != -1) {
            if (animData != NULL) {
                gDLL_3_Animation->vtbl->func21(animData, objSetup->unk24);
            }
            baddie->unk3B4 = 1;
        } else {
            fsa->target = NULL;
        }
    }

    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.17f, 1);
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, _bss_0, _bss_38);
    self->animObj = baddie->unk3AC;
}

// offset: 0xC10 | func: 11
void dll_225_func_C10(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    Object* target;

    if (baddie->unk3B0 & 2) {
        return;
    }
    
    target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, 0x8000);
    if (target) {
        gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 0, 0, 0, 1);
        fsa->unk33D = 0;
        fsa->target = target;
    }
}

// offset: 0xD08 | func: 12
s32 dll_225_func_D08(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 5);
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0x89, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->animTickDelta = 0.01f;
    
    return 0;
}

// offset: 0xDB4 | func: 13
s32 dll_225_func_DB4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 idx;
    f32 animProgress;
    s32 animChanged;
    f32* thresholds;

    gDLL_33_BaddieControl->vtbl->func3(self, fsa, self->data, 1.0f, 12.0f);

    animChanged = FALSE;
    animProgress = self->animProgress;
    
    idx = 0;
    while (self->curModAnimId != _data_1EC[idx] && idx != 3) {idx++;}
    if (idx >= 3) {
        idx = 0;
    }
    
    thresholds = &_data_1F4[idx << 1];
    if (fsa->speed < thresholds[0]) {
        animChanged = TRUE;
        if (idx == 1) {
            return 1;
        }
        idx--;
    } else if (thresholds[1] <= fsa->speed) {
        animChanged = TRUE;
        if (idx == 0) {
            animProgress = 0.0f;
        }
        idx++;
    }

    if (animChanged) {
        func_80023D30(self, _data_1EC[idx], animProgress, 0);
    }
    func_8002493C(self, fsa->unk278, &fsa->animTickDelta);
    
    return 0;
}

// offset: 0xF38 | func: 14
s32 dll_225_func_F38(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Vec3f d;

    if (fsa->enteredAnimState) {
        func_80023D30(self, 0x8C, 0.0f, 0);
        fsa->unk33A = 0;
    }

    fsa->unk341 = 1;
    fsa->animTickDelta = 0.025f;
    fsa->unk278 = 0.0f;
    
    if (fsa->target != NULL) {
        d.f[0] = fsa->target->globalPosition.f[0] - self->globalPosition.f[0];
        d.f[1] = fsa->target->globalPosition.f[1] - self->globalPosition.f[1];
        d.f[2] = fsa->target->globalPosition.f[2] - self->globalPosition.f[2];
        if (sqrtf(SQ(d.f[0]) + SQ(d.f[1]) + SQ(d.f[2])) < 26.0f) {
            fsa->unk33A = 1;
            gDLL_18_objfsa->vtbl->tick(self, fsa, 1.0f, 1.0f, _bss_0, _bss_38);
        }
    }
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    
    return 0;
}

// offset: 0x10EC | func: 15
s32 dll_225_func_10EC(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0x90, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.025f;
    fsa->unk278 = 1.0f;
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    
    return 0;
}

// offset: 0x11A4 | func: 16
s32 dll_225_func_11A4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0x91, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.025f;
    fsa->unk278 = -1.0f;
    fsa->unk27C = 0.0f;
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    
    return 0;
}

// offset: 0x1264 | func: 17
s32 dll_225_func_1264(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0x92, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.024f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 3);
    
    return 0;
}

// offset: 0x134C | func: 18
s32 dll_225_func_134C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0x93, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.024f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 3);
    
    return 0;
}

// offset: 0x1434 | func: 19
s32 dll_225_func_1434(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0x8F, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.015f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 8);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 2);
    
    return 0;
}

// offset: 0x151C | func: 20
s32 dll_225_func_151C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0x8E, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.015f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 8);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 2);
    
    return 0;
}

// offset: 0x1604 | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_1604.s")

// offset: 0x1878 | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_1878.s")

// offset: 0x1AEC | func: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_1AEC.s")

// offset: 0x1D60 | func: 24
s32 dll_225_func_1D60(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0xA2, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        gDLL_6_AMSFX->vtbl->play(self, 0x66B, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.015f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x1E60 | func: 25
s32 dll_225_func_1E60(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.015f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 0x8B, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    main_set_bits(baddie->unk39E, 0);
    main_set_bits(baddie->unk39C, 1);
    
    return 0;
}

// offset: 0x1F20 | func: 26
s32 dll_225_func_1F20(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 7;
}

// offset: 0x1F38 | func: 27
s32 dll_225_func_1F38(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->hitpoints <= 0) {
        return 5;
    }
    
    if (fsa->hitpoints < 5) {
        main_set_bits(BIT_391, 1);
        fsa->hitpoints = 1;
        return 3;
    }
    
    if (fsa->unk33A != 0) {
        if (fsa->hitpoints < rand_next(2, 4)) {
            return 4;
        } else {
            baddie->unk3B6 = 300;
            return 8;
        }
    }
    
    return 0;
}

// offset: 0x2010 | func: 28
s32 dll_225_func_2010(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
}

// offset: 0x2028 | func: 29
s32 dll_225_func_2028(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 4);
    } else if (fsa->unk33A) {
        return 8;
    }
    
    return 0;
}

// offset: 0x209C | func: 30
s32 dll_225_func_209C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0xD);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        self->objhitInfo->unk58 &= ~1;
        self->unkAF |= 8;
    } else if ((gUpdateRate * 4) >= self->opacity) {
        obj_send_mesg(get_player(), 0xE0000, self, NULL);
        return 6;
    }

    return 0;
}

// offset: 0x2190 | func: 31
s32 dll_225_func_2190(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredLogicState) {
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, -1, 0);
    }
    
    return 0;
}

// offset: 0x21F8 | func: 32
s32 dll_225_func_21F8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    Unk80009024* temp_s0;

    baddie = self->data;
    temp_s0 = &baddie->unk34C;
    
    if (fsa->target != NULL) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
        fsa->xAnalogInput = 0.0f;
        fsa->yAnalogInput = 0.0f;
        bcopy(&self->srt.transl, temp_s0, sizeof(Vec3f));
        bcopy(&fsa->target->srt.transl, &baddie->unk34C.unkC, sizeof(Vec3f));
        func_80009024(temp_s0, &baddie->unk374);

        if ((fsa->targetDist < baddie->unk3E2) && (baddie->unk3B4 == 2)) {
            return 8;
        }
        
        if (temp_s0->unk25 == 0) {
            gDLL_18_objfsa->vtbl->func6(self, fsa, temp_s0->unk18.x, temp_s0->unk18.f[2], 0.0f, 0.0f, 60.0f);
        } else {
            gDLL_18_objfsa->vtbl->func6(self, fsa, temp_s0->unk18.x, temp_s0->unk18.f[2], 15.0f, 30.0f, 60.0f);
        }
    } else {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0);
        fsa->unk33A = 0;
    }
    
    return 0;
}

// offset: 0x2404 | func: 33
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_2404.s")
