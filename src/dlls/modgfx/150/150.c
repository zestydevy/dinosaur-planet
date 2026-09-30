#include "common.h"

/*0x0*/ static u32 data_0 = 0x00000000;
/*0x4*/ static u32 data_4 = 0x000000ff;
/*0x8*/ static u32 data_8[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_150_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_150_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_150_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/150/dll_150_modgfx_Func0.s")
