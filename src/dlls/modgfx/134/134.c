#include "common.h"

/*0x0*/ static u32 data_0 = 0x0000009b;
/*0x4*/ static u32 data_4[] = {
    0x00c80001, 0x009b0000, 0x00000000
};

// offset: 0x0 | ctor
void dll_134_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_134_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_134_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/134/dll_134_modgfx_Func0.s")
