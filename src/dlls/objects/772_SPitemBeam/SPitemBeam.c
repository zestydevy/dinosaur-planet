#include "common.h"
#include "sys/objtype.h"

// offset: 0x0 | ctor
void dll_772_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_772_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_772_setup(Object* self, ObjSetup* setup, s32 arg2) {
    f32 sp24;

    sp24 = 10000.0f;
    self->unkB0 |= 0x6000;
    self->unkDC = obj_get_nearest_type_to(0xA, self, &sp24);
}

// offset: 0x88 | func: 1 | export: 1
void dll_772_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/772_SPitemBeam/dll_772_control.s")

// offset: 0x170 | func: 2 | export: 2
void dll_772_update(Object *self) { }

// offset: 0x17C | func: 3 | export: 3
void dll_772_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x194 | func: 4 | export: 4
void dll_772_free(Object *self, s32 a1) { }

// offset: 0x1A4 | func: 5 | export: 5
u32 dll_772_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1B4 | func: 6 | export: 6
s32 dll_772_get_data_size(s32 arg0, s32 arg1) {
    return 0;
}
 