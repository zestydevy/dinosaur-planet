#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "functions.h"

typedef struct {
/*00*/ s16 gamebit;
} WL_SpiritSet_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw;
/*19*/ s8 _unk19[0x1E-0x19];
/*1E*/ s16 gamebit;
} WL_SpiritSet_Setup;

// offset: 0x0 | ctor
void WL_SpiritSet_ctor(void *dll) { }

// offset: 0xC | dtor
void WL_SpiritSet_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_SpiritSet_setup(Object *self, WL_SpiritSet_Setup *setup, s32 arg2) {
    WL_SpiritSet_Data *objdata;

    objdata = self->data;
    self->srt.yaw = setup->yaw << 8;
    if (self->id == OBJ_WL_SpiritSet) {
        self->srt.scale = 0.0085f;
    }
    objdata->gamebit = setup->gamebit;
}

// offset: 0x60 | func: 1 | export: 1
void WL_SpiritSet_control(Object *self) { }

// offset: 0x6C | func: 2 | export: 2
void WL_SpiritSet_update(Object *self) { }

// offset: 0x78 | func: 3 | export: 3
void WL_SpiritSet_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    WL_SpiritSet_Data *objdata;

    objdata = self->data;
    do {
        if (objdata->gamebit != -1 && !main_get_bits(objdata->gamebit))
            return;

        if (visibility) {
            draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        }
    } while (0);
}

// offset: 0x114 | func: 4 | export: 4
void WL_SpiritSet_free(Object *self, s32 arg1) { }

// offset: 0x124 | func: 5 | export: 5
u32 WL_SpiritSet_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x134 | func: 6 | export: 6
u32 WL_SpiritSet_get_data_size(Object *self, u32 arg1) {
    return sizeof(WL_SpiritSet_Data);
}
