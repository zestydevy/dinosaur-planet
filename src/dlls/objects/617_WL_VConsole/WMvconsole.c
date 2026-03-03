#include "common.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
} WLConsole_Setup;

// offset: 0x0 | ctor
void WL_VConsole_ctor(void *dll) { }

// offset: 0xC | dtor
void WL_VConsole_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_VConsole_setup(Object* self, WLConsole_Setup* objSetup, s32 arg2) {
    self->srt.yaw = objSetup->yaw << 8;
}

// offset: 0x30 | func: 1 | export: 1
void WL_VConsole_control(Object *self) { }

// offset: 0x3C | func: 2 | export: 2
void WL_VConsole_update(Object *self) { }

// offset: 0x48 | func: 3 | export: 3
void WL_VConsole_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility){
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x9C | func: 4 | export: 4
void WL_VConsole_free(Object *self, s32 a1) { }

// offset: 0xAC | func: 5 | export: 5
u32 WL_VConsole_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xBC | func: 6 | export: 6
u32 WL_VConsole_get_data_size(Object *self, u32 a1) {
    return 0;
}
