#include "common.h"

typedef struct {
    ObjSetup base;
    u8 roll;
    u8 pitch;
    u8 yaw;
} DFcavedoor_Setup;

// offset: 0x0 | ctor
void DFcavedoor_ctor(void *dll) { }

// offset: 0xC | dtor
void DFcavedoor_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFcavedoor_setup(Object* self, DFcavedoor_Setup* setup, s32 reset) {
    self->srt.roll = setup->roll << 8;
    self->srt.pitch = setup->pitch << 8;
    self->srt.yaw = setup->yaw << 8;
}

// offset: 0x48 | func: 1 | export: 1
void DFcavedoor_control(Object* self) {
    if (!(self->srt.flags & OBJFLAG_INVISIBLE) && (main_get_bits(BIT_DF_Cave_Door_Hidden))) {
        self->srt.flags |= OBJFLAG_INVISIBLE;
    }
}

// offset: 0xB0 | func: 2 | export: 2
void DFcavedoor_update(Object *self) { }

// offset: 0xBC | func: 3 | export: 3
void DFcavedoor_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x110 | func: 4 | export: 4
void DFcavedoor_free(Object *self, s32 a1) { }

// offset: 0x120 | func: 5 | export: 5
u32 DFcavedoor_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x130 | func: 6 | export: 6
u32 DFcavedoor_get_data_size(Object* self, u32 a1) {
    return 0;
}
