#include "common.h"

typedef struct {
    ObjSetup base;
} WaterfallControl_Setup;

// offset: 0x0 | ctor
void WaterfallControl_ctor(void *dll) { }

// offset: 0xC | dtor
void WaterfallControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WaterfallControl_setup(Object* self, WaterfallControl_Setup* arg1, s32 arg2) {
    self->unkB0 |= 0x2000;
}

// offset: 0x34 | func: 1 | export: 1
void WaterfallControl_control(Object* self) {
    gDLL_6_AMSFX->vtbl->func_16E0();
}

// offset: 0x7C | func: 2 | export: 2
void WaterfallControl_update(Object *self) { }

// offset: 0x88 | func: 3 | export: 3
void WaterfallControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0xA0 | func: 4 | export: 4
void WaterfallControl_free(Object *self, s32 a1) { }

// offset: 0xB0 | func: 5 | export: 5
u32 WaterfallControl_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xC0 | func: 6 | export: 6
u32 WaterfallControl_get_data_size(Object *self, u32 a1){
    return 0;
}
