#include "common.h"
#include "sys/objanim.h"

// offset: 0x0 | ctor
void dll_382_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_382_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_382_setup(Object* self, ObjSetup* setup, s32 arg2) {
    self->opacity = 0;
}

// offset: 0x2C | func: 1 | export: 1
void dll_382_control(Object* self) {
    s32 var_v0; 
    Object* temp_v0;

    temp_v0 = get_player();
    if (self->curModAnimId == 0) {
        func_80024108(self, 0.01f, gUpdateRateF, NULL);
    }
    var_v0 = self->srt.yaw - (temp_v0->srt.yaw & 0xFFFF);
    if (var_v0 >= 0x8001) {
        var_v0 += 0xFFFF0001;
    }
    if (var_v0 < -0x8000) {
        var_v0 += 0xFFFF;
    }
    self->srt.yaw = self->srt.yaw - (var_v0 * gUpdateRate >> 3);
}

// offset: 0xF4 | func: 2 | export: 2
void dll_382_update(Object *self) { }

// offset: 0x100 | func: 3 | export: 3
void dll_382_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x118 | func: 4 | export: 4
void dll_382_free(Object *self, s32 a1) { }

// offset: 0x128 | func: 5 | export: 5
u32 dll_382_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x138 | func: 6 | export: 6
s32 dll_382_get_data_size(s32 arg0, s32 arg1) {
    return 0;
}
