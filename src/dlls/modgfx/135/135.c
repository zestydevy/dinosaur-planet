#include "common.h"

/*0x0*/ static u32 data_0 = 0x0000009b;
/*0x4*/ static u32 data_4[] = {
    0x00c80001, 0x009b0000, 0x00000000
};

// offset: 0x0 | ctor
void dll_135_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_135_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_135_modgfx_Func0(Object* arg0, s32 arg1, SRT* arg2, u32 arg3, s32 arg4, void* arg5);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/135/dll_135_modgfx_Func0.s")
