#include "common.h"
#include "sys/objanim.h"

static int dll_566_func_194(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3);

// offset: 0x0 | ctor
void dll_566_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_566_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_566_setup(Object* self, ObjSetup* setup, s32 arg2) {
    obj_set_update_priority(self, OBJPRIORITY_MOBILE_MAP);
    self->animCallback = dll_566_func_194;
    func_80023D30(self, 0, 0.0f, 0U);
    func_80024108(self, 0.005f, gUpdateRateF, NULL);
}

// offset: 0xB4 | func: 1 | export: 1
void dll_566_control(Object *self) { }

// offset: 0xC0 | func: 2 | export: 2
void dll_566_update(Object *self) { }

// offset: 0xCC | func: 3 | export: 3
void dll_566_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        func_80024108(self, 0.005f, gUpdateRateF, NULL);
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x160 | func: 4 | export: 4
void dll_566_free(Object *self, s32 a1) { }

// offset: 0x170 | func: 5 | export: 5
u32 dll_566_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x180 | func: 6 | export: 6
u32 dll_566_get_data_size(Object *self, u32 a1) {
    return 0;
}

// offset: 0x194 | func: 7
static int dll_566_func_194(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3) {
    arg2->unk7A = -1;
    arg2->unk62 = 0;
    return 0;
}
