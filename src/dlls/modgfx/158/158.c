#include "common.h"

/*0x0*/ static u32 data_0[] = {
    0x000000e6, 0x05140000, 0x001f0000, 0xff1a0514, 0x001f001f, 0x00000000, 0x0000000f, 0x00100000
};
/*0x20*/ static u32 data_20[] = {
    0x00000001, 0x00020000, 0x00000000, 0x00010002
};
/*0x30*/ static u32 data_30[] = {
    0x00000001, 0x00020000
};
/*0x38*/ static u32 data_38 = 0x0000008c;
/*0x3C*/ static u32 data_3C[] = {
    0x008c0000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_158_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_158_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_158_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/158/dll_158_modgfx_Func0.s")
