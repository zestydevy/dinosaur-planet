#include "common.h"

// offset: 0x0 | ctor
void dll_700_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_700_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_700_setup(Object* self, ObjSetup* setup, s32 arg2) {
    self->opacity = 0;
    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);
}

// offset: 0x38 | func: 1 | export: 1
void dll_700_control(Object* self) {
    Camera* temp_v0;

    temp_v0 = get_camera();
    self->srt.transl.f[0] = temp_v0->srt.transl.f[2];
    self->srt.transl.f[1] = temp_v0->srt.transl.f[1];
    self->srt.transl.f[2] = temp_v0->srt.transl.f[0];
    self->globalPosition.f[0] = temp_v0->tz;
    self->globalPosition.f[1] = temp_v0->ty;
    self->globalPosition.f[2] = temp_v0->tx;
    self->srt.roll = temp_v0->srt.roll;
    self->srt.pitch = temp_v0->srt.pitch;
    self->srt.yaw = temp_v0->srt.yaw;
}

// offset: 0xBC | func: 2 | export: 2
void dll_700_update(Object *self) { }

// offset: 0xC8 | func: 3 | export: 3
void dll_700_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0xE0 | func: 4 | export: 4
void dll_700_free(Object *self, s32 a1) { }

// offset: 0xF0 | func: 5 | export: 5
u32 dll_700_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x100 | func: 6 | export: 6
u32 dll_700_get_data_size(Object* self, u32 a1) {
    return 0U;
}

