#include "common.h"

// offset: 0x0 | ctor
void dll_465_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_465_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_465_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/465_CCdockdoor/dll_465_setup.s")

// offset: 0x3C | func: 1 | export: 1
void dll_465_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/465_CCdockdoor/dll_465_control.s")

// offset: 0xCC | func: 2 | export: 2
void dll_465_update(Object *self) { }

// offset: 0xD8 | func: 3 | export: 3
void dll_465_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0xF0 | func: 4 | export: 4
void dll_465_free(Object *self, s32 a1) { }

// offset: 0x100 | func: 5 | export: 5
u32 dll_465_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x110 | func: 6 | export: 6
u32 dll_465_get_data_size(Object* self, u32 a1) {
    return 1U;
}

