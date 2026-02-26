#include "common.h"
#include "sys/objanim.h"
#include "sys/objtype.h"
#include "functions.h"
#include "sys/rand.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    u16 unk4;
    u16 unk6;
    u8 unk8;
    u8 _unk9 [0x406 - 0x09];
} DLL517_Data;

/*0x0*/ static u32 _data_0[] = {
    0x00000002, 0x00000003
};
/*0x8*/ static s16 _data_8[] = {
    0x0007, 0x000a, 0x0008, 0x0009, 0x000b, 0x000c, 0x0007, 0x0000
};
/*0x18*/ static f32 _data_18[] = {
    0.01f /*0x3c23d70a*/,
    0.01f /*0x3c23d70a*/,
    0.005f /*0x3ba3d70a*/,
    0.003f /*0x3b449ba6*/,
    0.01f /*0x3c23d70a*/,
    0.01f /*0x3c23d70a*/,
    0.01f /*0x3c23d70a*/,
    0.0f  /*0x00000000*/,
    0.0f  /*0x00000000*/,
    0.0f  /*0x00000000*/,
};

/*0x0*/ static ObjFSA_StateCallback _bss_0[2];
/*0x8*/ static ObjFSA_StateCallback _bss_8[2];

static void dll_517_func_334(Object* arg0, Baddie* arg1, ObjFSA_Data* arg2);
static int dll_517_func_2FC(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3);
static s32 dll_517_func_4AC(Object* self, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_517_func_628(Object* arg0, ObjFSA_Data* FSAData, f32 arg2);
static s32 dll_517_func_7C4(Object* self, ObjFSA_Data* arg1, f32 arg2);

// offset: 0x0 | func: 0
static void dll_517_func_0(void) {
    _bss_0[0] = dll_517_func_4AC;
    _bss_0[1] = dll_517_func_628;
    _bss_8[0] = dll_517_func_7C4;
}

// offset: 0x44 | ctor
void dll_517_ctor(void* dll) {
    dll_517_func_0();
}

// offset: 0x84 | dtor
void dll_517_dtor(void *dll) { }

// offset: 0x90 | func: 1 | export: 0
void dll_517_setup(Object* self, Baddie_Setup* setup, s32 arg2) {
    s32 var_v0;
    DLL517_Data* sp38;
    Baddie* baddie;

    baddie = self->data;
    var_v0 = 0x16;
    if (arg2 != 0) {
        var_v0 = 0x17;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, (Baddie_Setup* ) setup, (Baddie* ) baddie, 2, 1, 0x108, (u8) var_v0, 20.0f);
    self->animCallback = dll_517_func_2FC;
    baddie->fsa.animState = 0;
    baddie->fsa.logicState = 0;
    sp38 = baddie->objdata;
    sp38->unk8 = rand_next(0, 1);
    if (self->shadow != NULL) {
        self->shadow->distFadeMaxOpacity = 0x64;
        self->shadow->distFadeMinOpacity = 0x96;
    }
}

// offset: 0x178 | func: 2 | export: 1
void dll_517_control(Object* self) {
    Baddie* sp24;

    sp24 = self->data;
    if (self->unkDC == 0) {
        if (gDLL_33_BaddieControl->vtbl->func11(self, (Baddie* ) sp24, 1U) != 0) {
           dll_517_func_334(self, sp24, &sp24->fsa);
        }
    }
}

// offset: 0x1FC | func: 3 | export: 2
void dll_517_update(Object *self) { }

// offset: 0x208 | func: 4 | export: 3
void dll_517_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if ((visibility != 0) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x268 | func: 5 | export: 4
void dll_517_free(Object* self, s32 arg1) {
    void* sp24;

    sp24 = self->data;
    obj_free_object_type(self, OBJTYPE_4);
    gDLL_33_BaddieControl->vtbl->free(self, (Baddie* ) sp24, 0x20U);
}


// offset: 0x2D8 | func: 6 | export: 5
u32 dll_517_get_model_flags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_8 | MODFLAGS_EVENTS;
}

// offset: 0x2E8 | func: 7 | export: 6
u32 dll_517_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL517_Data);
}

// offset: 0x2FC | func: 8
static int dll_517_func_2FC(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3) {
    Baddie* baddie = arg0->data; 
    
    if (arg0->unkDC != 0) {
        return 0;
    }
    return (u8)baddie ->unk3B4 != 0;
}


// offset: 0x334 | func: 9
static void dll_517_func_334(Object* arg0, Baddie* baddie, ObjFSA_Data* arg2) {
    DLL517_Data* sp2C = baddie->objdata;

    gDLL_33_BaddieControl->vtbl->func4(arg0, get_player(), 0x10U, &sp2C->unk0, &sp2C->unk2, &sp2C->unk4);

    arg2->targetDist = sp2C->unk4;
    func_80032A08(arg0, &baddie -> unk3BC);
    arg2->target = get_player();
    gDLL_33_BaddieControl->vtbl->func10(arg0, arg2, 0.17f, 1);
    baddie->unk3AC = (Object* ) arg0->unkC0;
    arg0->unkC0 = NULL;
    gDLL_18_objfsa->vtbl->tick(arg0, arg2, gUpdateRateF, gUpdateRateF, _bss_0, _bss_8);
    arg0->unkC0 = baddie->unk3AC;
}

// offset: 0x4AC | func: 10
static s32 dll_517_func_4AC(Object* self, ObjFSA_Data* arg1, f32 arg2) {
    Baddie* baddie = self->data;
    DLL517_Data* sp28;
    Object* sp24;
    u16 temp_v0;

    sp24 = arg1->target;
    if (sp24 != NULL) {
        func_80032C0C(self, sp24, &baddie -> unk3BC, 0x19);
    }
    sp28 = baddie->objdata;
    if ((arg1->enteredAnimState != 0) || (arg1->unk33A != 0)) {
        sp28->unk6 = rand_next(0, 6);
        func_80023D30(self, _data_8[sp28->unk6], 0.0f, 0U);
    }
    if (((s32) sp28->unk4 < (s32) baddie->unk3E2) && (arg1->logicState != 0)) {
        arg1->animTickDelta = ( f32) _data_18[sp28->unk6] * 5.0f;
    } else {
        arg1->animTickDelta = ( f32 ) _data_18[sp28->unk6];
    }
    gDLL_18_objfsa->vtbl->func7(self, arg1, arg2, 0);
    return 0;
}

// offset: 0x628 | func: 11
static s32 dll_517_func_628(Object* arg0, ObjFSA_Data* FSAData, f32 arg2) {
    Baddie* baddie = arg0->data;
    DLL517_Data* d517Data;
    s16 sp36;
    s8 pad2;
    f32 temp;
    Object* sp2C;
    s8 pad;

    sp2C = FSAData->target;
    if (sp2C != NULL) {
        func_80032C0C(arg0, sp2C, &baddie -> unk3BC, 0x19);
    }
    d517Data = baddie->objdata;
    sp36 = d517Data->unk2;
    if ((FSAData->enteredAnimState != 0) || (FSAData->unk33A != 0)) {
        if (sp36 < 0x7FFF) {
            func_80023D30(arg0, 0xE, 0.0f, 0U);
        } else {
            func_80023D30(arg0, 0xD, 0.0f, 0U);
        }
    
    }
    if ((f32) sp36 < 0.0f) {
        temp = -(f32) sp36;
    } else {
        temp = (f32) sp36;
    }
    FSAData->animTickDelta = (temp / 32767.0f) * 0.1f;
    if ((temp / 32767.0f) * 0.1f < 0.01f) {
        FSAData->animTickDelta = 0.01f;
    }
    arg0->srt.yaw += (s32) (( sp36 * gUpdateRateF) / 12.0f) & 0xFFFF & 0xFFFF;
    gDLL_18_objfsa->vtbl->func7(arg0, FSAData, arg2, 0);
    return 0;
}

// offset: 0x7C4 | func: 12
static s32 dll_517_func_7C4(Object* self, ObjFSA_Data* arg1, f32 arg2) {
    u16 sp20; 
    DLL517_Data* sp28;
    Baddie* baddie = self->data;

    if (arg1->target != NULL) {
        sp28 = baddie->objdata;
        if (sp28->unk2 < 0) {
            sp20 = -sp28->unk2;
        } else {
            sp20 = sp28->unk2;
        }

        if (sp20 < 0x1770 && (self->unkAF & 1)) {
            joy_set_button_mask(0, A_BUTTON);
            gDLL_3_Animation->vtbl->func17((s32) sp28->unk8, self, -1);
            sp28->unk8++;
            if (sp28->unk8 >= 2) {
                sp28->unk8 = 0U;
            }
        }
        if ((arg1->enteredLogicState != 0) || arg1->unk33A != 0) {
            if (sp20 < 0x222) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, arg1, 0);
            } else {
                gDLL_18_objfsa->vtbl->set_anim_state(self, arg1, 1);
            }
        }
    }

    return 0;
   
}