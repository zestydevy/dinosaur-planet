#include "common.h"

// size:0x1C
typedef struct {
/*00*/ u8 _unk0[0x1C];
} DLL605_Data;

/*0x0*/ static u32 _data_0[] = {0x00000000, 0x00000000, 0x00000000, 0x00000000};

// offset: 0x0 | ctor
void dll_605_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_605_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_605_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/605_WLworm/dll_605_setup.s")

// offset: 0x88 | func: 1 | export: 1
void dll_605_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/605_WLworm/dll_605_control.s")

// offset: 0x388 | func: 2 | export: 2
void dll_605_update(Object *self) { }

// offset: 0x394 | func: 3 | export: 3
void dll_605_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x3AC | func: 4 | export: 4
void dll_605_free(Object* self, s32 a1) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x3F4 | func: 5 | export: 5
u32 dll_605_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x404 | func: 6 | export: 6
u32 dll_605_get_data_size(Object* self, u32 a1) {
    return sizeof(DLL605_Data);
}

