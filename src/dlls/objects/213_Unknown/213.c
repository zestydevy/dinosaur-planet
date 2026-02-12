#include "common.h"

// offset: 0x0 | ctor
void dll_213_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_213_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_213_setup(Object* self, ObjSetup* setup, s32 arg2) {
    self->unkB0 |= 0x6000;
}

// offset: 0x34 | func: 1 | export: 1
void dll_213_control(Object *self) { }

// offset: 0x40 | func: 2 | export: 2
void dll_213_update(Object *self) { }

// offset: 0x4C | func: 3 | export: 3
void dll_213_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x64 | func: 4 | export: 4
void dll_213_free(Object *self, s32 a1) { }

// offset: 0x74 | func: 5 | export: 5
u32 dll_213_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x84 | func: 6 | export: 6
u32 dll_213_get_data_size(Object* self, u32 a1) {
    return 0U;
}

