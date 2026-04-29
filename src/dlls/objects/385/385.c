#include "common.h"

static int dll_385_func_110(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3);

// offset: 0x0 | ctor
void dll_385_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_385_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_385_setup(Object* self, ObjSetup* setup, s32 arg2) {
    obj_set_update_priority(self, OBJPRIORITY_MOBILE_MAP);
    self->animCallback = dll_385_func_110;
}

// offset: 0x70 | func: 1 | export: 1
void dll_385_control(Object *self) { }

// offset: 0x7C | func: 2 | export: 2
void dll_385_update(Object *self) { }

// offset: 0x88 | func: 3 | export: 3
void dll_385_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xDC | func: 4 | export: 4
void dll_385_free(Object *self, s32 a1) { }

// offset: 0xEC | func: 5 | export: 5
u32 dll_385_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xFC | func: 6 | export: 6
u32 dll_385_get_data_size(Object *self, u32 a1) {
    return 0;
}

// offset: 0x110 | func: 7
static int dll_385_func_110(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3) {
    arg2->unk7A = -1;
    arg2->unk62 = 0;
    return 0;
}

