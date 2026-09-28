#include "common.h"

/*0x0*/ static u32 data_0[] = {
    0x000000e6, 0x07080000, 0x001f0000, 0xff1a0708, 0x001f001f, 0x00000000, 0x0000000f, 0x00100000
};
/*0x20*/ static u32 data_20[] = {
    0x00000001, 0x00020000
};
/*0x28*/ static u32 data_28[] = {
    0x00020000, 0x00010000, 0x00010002
};
/*0x34*/ static u32 data_34[] = {
    0x00000001, 0x00020000
};
/*0x3C*/ static u32 data_3C[] = {
    0x0000000a, 0x0028003c, 0x00280000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_170_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_170_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_170_modgfx_Func0(Object* arg0, s32 arg1, SRT* arg2, u32 arg3, s32 arg4, void* arg5);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/170/dll_170_modgfx_Func0.s")

/*0x1C*/ static const u32 rodata_1C[] = {
    0x3fe66666
};
