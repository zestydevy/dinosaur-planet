#include "dll.h"
#include "functions.h"
#include "sys/objects.h"
#include "sys/objmsg.h"

// offset: 0x0 | ctor
void AnimTest_ctor(void *dll) { }

// offset: 0xC | dtor
void AnimTest_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void AnimTest_setup(Object *self, ObjSetup *setup, s32 arg2) { }

// offset: 0x2C | func: 1 | export: 1
void AnimTest_control(Object *self) {
    gDLL_6_AMSFX->vtbl->func_16E0();
    if (!get_sidekick()) {
        obj_send_mesg_many(0x1F, 0, self, 0x11, 0);
    }
}

// offset: 0xA8 | func: 2 | export: 2
void AnimTest_update(Object *self) { }

// offset: 0xB4 | func: 3 | export: 3
void AnimTest_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x108 | func: 4 | export: 4
void AnimTest_free(Object *self, s32 a1) { }

// offset: 0x118 | func: 5 | export: 5
u32 AnimTest_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x128 | func: 6 | export: 6
u32 AnimTest_get_data_size(Object *self, u32 a1) {
    return 0;
}
