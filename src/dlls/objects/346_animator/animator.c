#include "common.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} animator_Data;

// offset: 0x0 | ctor
void animator_ctor(void *dll) { }

// offset: 0xC | dtor
void animator_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void animator_setup(Object *self, ObjSetup *setup, s32 arg2) { }

// offset: 0x2C | func: 1 | export: 1
void animator_control(Object *self) { }

// offset: 0x38 | func: 2 | export: 2
void animator_update(Object *self) { }

// offset: 0x44 | func: 3 | export: 3
void animator_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x98 | func: 4 | export: 4
void animator_free(Object *self, s32 a1) { }

// offset: 0xA8 | func: 5 | export: 5
u32 animator_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xB8 | func: 6 | export: 6
u32 animator_get_data_size(Object *self, u32 a1){
    return sizeof(animator_Data);
}
