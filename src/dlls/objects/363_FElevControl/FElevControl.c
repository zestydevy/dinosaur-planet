#include "sys/objmsg.h"
#include "functions.h"

// offset: 0x0 | ctor
void FElevControl_ctor(void *dll) { }

// offset: 0xC | dtor
void FElevControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void FElevControl_setup(Object *self, ObjSetup *setup, s32 arg2) {
    obj_init_mesg_queue(self, 2);
}

// offset: 0x5C | func: 1 | export: 1
void FElevControl_control(Object *self) { }

// offset: 0x68 | func: 2 | export: 2
void FElevControl_update(Object *self) { }

// offset: 0x74 | func: 3 | export: 3
void FElevControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xC8 | func: 4 | export: 4
void FElevControl_free(Object *self, s32 a1) { }

// offset: 0xD8 | func: 5 | export: 5
u32 FElevControl_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xE8 | func: 6 | export: 6
u32 FElevControl_get_data_size(Object *self, u32 a1) {
    return 0;
}
