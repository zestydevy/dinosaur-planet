#include "common.h"

/*0x0*/ static u32 data_0[] = {
    0xff1a0000, 0x00000000, 0x000f0000, 0x00000000, 0x007f000f, 0x00e60000, 0x000000ff, 0x000fff1a, 
    0x000003e8, 0x00000000, 0x00000000, 0x03e8007f, 0x000000e6, 0x000003e8, 0x00ff0000
};
/*0x3C*/ static u32 data_3C[] = {
    0x00000004, 0x00030000, 0x00010004, 0x00010002, 0x00040002, 0x00050004
};
/*0x54*/ static u32 data_54[] = {
    0x00010000, 0x00040000, 0x00000002, 0x00030005
};
/*0x64*/ static u32 data_64[] = {
    0x00000001, 0x00020003, 0x00040005
};
/*0x70*/ static u32 data_70[] = {
    0x00000002, 0x00030004, 0x00050000
};
/*0x7C*/ static u32 data_7C[] = {
    0x00000006, 0x0014001a, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_162_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_162_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_162_modgfx_Func0(Object* arg0, s32 arg1, SRT* arg2, u32 arg3, s32 arg4, void* arg5);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/162/dll_162_modgfx_Func0.s")
