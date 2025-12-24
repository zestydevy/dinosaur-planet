#include "sys/main.h"
#include "sys/objanim.h"
#include "functions.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw;
} SB_ShipFlag_Setup;

// offset: 0x0 | ctor
void SB_ShipFlag_ctor(void *dll) { }

// offset: 0xC | dtor
void SB_ShipFlag_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SB_ShipFlag_setup(Object *self, SB_ShipFlag_Setup *setup, s32 arg2) {
    self->srt.yaw = setup->yaw << 8;
    func_80023D30(self, 0, 0.0f, 0);
}

// offset: 0x74 | func: 1 | export: 1
void SB_ShipFlag_control(Object *self) {
    func_80024108(self, 0.007f, gUpdateRate, NULL);
}

// offset: 0xE0 | func: 2 | export: 2
void SB_ShipFlag_update(Object *self) { }

// offset: 0xEC | func: 3 | export: 3
void SB_ShipFlag_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x140 | func: 4 | export: 4
void SB_ShipFlag_free(Object *self, s32 a1) { }

// offset: 0x150 | func: 5 | export: 5
u32 SB_ShipFlag_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x160 | func: 6 | export: 6
u32 SB_ShipFlag_get_data_size(Object *self, u32 a1) {
    return 0;
}
