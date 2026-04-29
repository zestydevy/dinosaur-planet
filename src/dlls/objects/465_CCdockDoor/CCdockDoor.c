#include "common.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 _unk19;
    s16 _unk1A;
    s16 _unk1C;
    s16 unk1E; 
}DLL465_Setup;

typedef struct {
    u8 unk0;    
} DLL465_Data;

// offset: 0x0 | ctor
void dll_465_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_465_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_465_setup(Object* self, DLL465_Setup* setup, s32 arg2) {
    self->srt.yaw = setup->unk18 << 8;
    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);
}

// offset: 0x3C | func: 1 | export: 1
void dll_465_control(Object* self) {
    DLL465_Data* objData;
    DLL465_Setup* objSetup;

    objData = self->data;
    objSetup = (DLL465_Setup*)self->setup;
    
    objData->unk0 = main_get_bits(objSetup->unk1E);
    if (objData->unk0 == 1) {
        self->srt.flags |= OBJFLAG_INVISIBLE;
        self->stateFlags |= OBJSTATE_CONTROL_DISABLED;
        func_800267A4(self);
    }
}

// offset: 0xCC | func: 2 | export: 2
void dll_465_update(Object *self) { }

// offset: 0xD8 | func: 3 | export: 3
void dll_465_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0xF0 | func: 4 | export: 4
void dll_465_free(Object *self, s32 a1) { }

// offset: 0x100 | func: 5 | export: 5
u32 dll_465_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x110 | func: 6 | export: 6
u32 dll_465_get_data_size(Object* self, u32 a1) {
    return 1U;
}

