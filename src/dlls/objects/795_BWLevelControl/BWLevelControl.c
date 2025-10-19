#include "sys/objtype.h"
#include "functions.h"
#include "dll.h"

// offset: 0x0 | ctor
void BWLevelControl_ctor(void *dll) { }

// offset: 0xC | dtor
void BWLevelControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void BWLevelControl_setup(Object *self, ObjSetup *setup, s32 arg2) {
    obj_add_object_type(self, OBJTYPE_10);
}

// offset: 0x5C | func: 1 | export: 1
void BWLevelControl_control(Object *self) {
    gDLL_6_AMSFX->vtbl->func_16E0();
}

// offset: 0xA4 | func: 2 | export: 2
void BWLevelControl_update(Object *self) { }

// offset: 0xB0 | func: 3 | export: 3
void BWLevelControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x104 | func: 4 | export: 4
void BWLevelControl_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_10);
}

// offset: 0x144 | func: 5 | export: 5
u32 BWLevelControl_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x154 | func: 6 | export: 6
u32 BWLevelControl_get_data_size(Object *self, u32 a1) {
    return 0;
}
