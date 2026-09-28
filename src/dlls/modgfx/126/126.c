#include "common.h"

/*0x0*/ static u32 data_0[] = {
    0xfc180000, 0xfc180000, 0x000003e8, 0x0000fc18, 0x003f0000, 0x03e80000, 0x03e8003f, 0x003ffc18, 
    0x000003e8, 0x0000003f, 0x00000000, 0x00000020, 0x00200000
};
/*0x34*/ static u32 data_34[] = {
    0x00000001, 0x00040001, 0x00020004, 0x00040002, 0x00030000, 0x00040003, 0x00000000, 0x00000000
};
/*0x54*/ static u32 data_54[] = {
    0x00000001, 0x00020003, 0x00040000, 0x00000001, 0x00020003, 0x00040000
};
/*0x6C*/ static u32 data_6C = 0x00000050;
/*0x70*/ static u32 data_70[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_126_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_126_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_126_modgfx_Func0(Object* arg0, s32 arg1, SRT* arg2, u32 arg3, s32 arg4, void* arg5);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/126/dll_126_modgfx_Func0.s")
