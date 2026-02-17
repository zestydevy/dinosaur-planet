#include "common.h"

// offset: 0x0 | ctor
void dll_412_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_412_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_412_setup(Object *self, ObjSetup *setup, s32 arg2) { }

// offset: 0x2C | func: 1 | export: 1
void dll_412_control(Object *self) { }

// offset: 0x38 | func: 2 | export: 2
void dll_412_update(Object *self) { }

// offset: 0x44 | func: 3 | export: 3
void dll_412_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}


// offset: 0x98 | func: 4 | export: 4
void dll_412_free(Object *self, s32 a1) { }

// offset: 0xA8 | func: 5 | export: 5
u32 dll_412_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xB8 | func: 6 | export: 6
u32 dll_412_get_data_size(Object* self, u32 a1) {
    return 4U;
}

