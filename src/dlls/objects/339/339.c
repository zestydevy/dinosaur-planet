#include "functions.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 roll;
/*19*/ u8 pitch;
/*1A*/ u8 yaw;
/*1B*/ u8 scale;
} DLL339_Setup;

// offset: 0x0 | ctor
void dll_339_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_339_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_339_setup(Object *self, DLL339_Setup *setup, s32 arg2) {
    self->srt.roll = setup->roll << 8;
    self->srt.pitch = setup->pitch << 8;
    self->srt.yaw = setup->yaw << 8;
    if (setup->scale != 0) {
        self->srt.scale = setup->scale / 255.0f;
        if (self->srt.scale == 0.0f) {
            self->srt.scale = 1.0f;
        }
        self->srt.scale *= self->def->scale;
    }
    self->unkB0 |= 0xA000;
}

// offset: 0xD4 | func: 1 | export: 1
void dll_339_control(Object *self) { }

// offset: 0xE0 | func: 2 | export: 2
void dll_339_update(Object *self) { }

// offset: 0xEC | func: 3 | export: 3
void dll_339_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x140 | func: 4 | export: 4
void dll_339_free(Object *self, s32 a1) { }

// offset: 0x150 | func: 5 | export: 5
u32 dll_339_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x160 | func: 6 | export: 6
u32 dll_339_get_data_size(Object *self, u32 a1) {
    return 0;
}
