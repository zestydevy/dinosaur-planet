#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "functions.h"
#include "dll.h"

typedef struct {
    u8 _unk0[2];
} PineCone_Data;

// offset: 0x0 | ctor
void dll_254_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_254_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_254_setup(Object *self, ObjSetup *setup, s32 arg2) { }

// offset: 0x2C | func: 1 | export: 1
void dll_254_control(Object *self) { }

// offset: 0x38 | func: 2 | export: 2
void dll_254_update(Object *self) { }

// offset: 0x44 | func: 3 | export: 3
void dll_254_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x98 | func: 4 | export: 4
void dll_254_free(Object *self, s32 a1) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0xE0 | func: 5 | export: 5
u32 dll_254_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xF0 | func: 6 | export: 6
u32 dll_254_get_data_size(Object *self, u32 a1) {
    return sizeof(PineCone_Data);
}
