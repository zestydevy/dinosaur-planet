#include "common.h"

// offset: 0x0 | ctor
void dll_438_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_438_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_438_setup(Object *self, ObjSetup *setup, s32 arg2) { }

// offset: 0x2C | func: 1 | export: 1
void dll_438_control(Object *self) { }

// offset: 0x38 | func: 2 | export: 2
void dll_438_update(Object *self) { }

// offset: 0x44 | func: 3 | export: 3
void dll_438_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x5C | func: 4 | export: 4
void dll_438_free(Object *self, s32 a1) { }

// offset: 0x6C | func: 5 | export: 5
s32 dll_438_get_model_flags(s32 arg0) {
    return MODFLAGS_1 | MODFLAGS_SHADOW;
}

// offset: 0x7C | func: 6 | export: 6
u32 dll_438_get_data_size(Object *self, u32 a1) {
    return 0;
}
