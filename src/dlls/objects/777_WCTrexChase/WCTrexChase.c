#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "functions.h"

// offset: 0x0 | ctor
void WCTrexChase_ctor(void *dll) { }

// offset: 0xC | dtor
void WCTrexChase_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCTrexChase_setup(Object *self, ObjSetup *objsetup, s32 arg2) { }

// offset: 0x2C | func: 1 | export: 1
void WCTrexChase_control(Object *self) { }

// offset: 0x38 | func: 2 | export: 2
void WCTrexChase_update(Object *self) { }

// offset: 0x44 | func: 3 | export: 3
void WCTrexChase_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x98 | func: 4 | export: 4
void WCTrexChase_free(Object *self, s32 a1) { }

// offset: 0xA8 | func: 5 | export: 5
u32 WCTrexChase_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xB8 | func: 6 | export: 6
u32 WCTrexChase_get_data_size(Object *self, u32 a1) {
    return 0;
}
