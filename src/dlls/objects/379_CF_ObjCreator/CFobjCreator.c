#include "common.h"

typedef struct {
    ObjSetup base;
    s32 _unk18;
    u16 _unk1C;
    s8 unk1E;    
}DLL379_Setup;

// offset: 0x0 | ctor
void dll_379_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_379_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_379_setup(Object* self, DLL379_Setup* setup, s32 arg2) {
    self->srt.yaw = setup->unk1E << 8;
}

// offset: 0x30 | func: 1 | export: 1
void dll_379_control(Object *self) { }

// offset: 0x3C | func: 2 | export: 2
void dll_379_update(Object *self) { }

// offset: 0x48 | func: 3 | export: 3
void dll_379_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x9C | func: 4 | export: 4
void dll_379_free(Object *self, s32 a1) { }

// offset: 0xAC | func: 5 | export: 5
u32 dll_379_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xBC | func: 6 | export: 6
u32 dll_379_get_data_size(Object *self, u32 a1) {
    return 0;
}
