#include "common.h"
#include "sys/objtype.h"

static int dll_498_func_108(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3);

// offset: 0x0 | ctor
void dll_498_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_498_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_498_setup(Object* self, ObjSetup* setup, s32 arg2) {
    self->animCallback = dll_498_func_108;
    self->unkB0 |= 0x6000;
    obj_add_object_type(self, OBJTYPE_62);
}
//type was 0x3E, I think that is OBJTYPE_62? I had to create it
//just commenting to make sure that that's right

// offset: 0x74 | func: 1 | export: 1
void dll_498_control(Object *self) { }

// offset: 0x80 | func: 2 | export: 2
void dll_498_update(Object *self) { }

// offset: 0x8C | func: 3 | export: 3
void dll_498_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0xA4 | func: 4 | export: 4
void dll_498_free(Object* self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_62);
}
//see line 18 for comment

// offset: 0xE4 | func: 5 | export: 5
u32 dll_498_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xF4 | func: 6 | export: 6
u32 dll_498_get_data_size(Object *self, u32 a1) {
    return 0;
}

// offset: 0x108 | func: 7
static int dll_498_func_108(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3) {
    return 0;
}

