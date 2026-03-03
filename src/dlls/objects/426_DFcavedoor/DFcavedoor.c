#include "common.h"

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    
} DLL426Setup;

// offset: 0x0 | ctor
void dll_426_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_426_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_426_setup(Object* self, DLL426Setup* setup, s32 arg2) {
    self->srt.roll = setup->unk18 << 8;
    self->srt.pitch = setup->unk19 << 8;
    self->srt.yaw = setup->unk1A << 8;
}

// offset: 0x48 | func: 1 | export: 1
void dll_426_control(Object* self) {
    if (!(self->srt.flags & 0x4000) && (main_get_bits(BIT_293) != 0)) {
        self->srt.flags |= 0x4000;
    }
}

// offset: 0xB0 | func: 2 | export: 2
void dll_426_update(Object *self) { }

// offset: 0xBC | func: 3 | export: 3
void dll_426_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x110 | func: 4 | export: 4
void dll_426_free(Object *self, s32 a1) { }

// offset: 0x120 | func: 5 | export: 5
u32 dll_426_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x130 | func: 6 | export: 6
u32 dll_426_get_data_size(Object* self, u32 a1) {
    return 0U;
}
