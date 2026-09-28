#include "common.h"

/*0x0*/ static u32 data_0[] = {
    0x001e0000, 0x00000000, 0x001fffe2, 0x00000000, 0x000f001f, 0x00000000, 0x03e80008, 0x00000000
};
/*0x20*/ static u32 data_20[] = {
    0x000f0000, 0x00000000, 0x001ffff1, 0x00000000, 0x000f001f, 0x000f0000, 0x07d00008, 0x0000fff1, 
    0x000007d0, 0x00080000
};
/*0x48*/ static u32 data_48[] = {
    0x00000001, 0x00020000
};
/*0x50*/ static u32 data_50[] = {
    0x00000001, 0x00020001, 0x00030002
};
/*0x5C*/ static u32 data_5C[] = {
    0x00000001, 0x00020000
};
/*0x64*/ static u32 data_64[] = {
    0x00000001, 0x00020003
};
/*0x6C*/ static u32 data_6C = 0x00000050;
/*0x70*/ static u32 data_70[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_106_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_106_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_106_modgfx_Func0(Object* arg0, s32 arg1, SRT* arg2, u32 arg3, s32 arg4, void* arg5);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/106/dll_106_modgfx_Func0.s")
