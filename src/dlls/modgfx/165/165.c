#include "common.h"

/*0x0*/ static u32 data_0[] = {
    0xfce001f4, 0xfce00008, 0x001f0320, 0x01f4fce0, 0x0078001f, 0x032001f4, 0x03200008, 0x001ffce0, 
    0x01f40320, 0x0078001f, 0xfc180000, 0xfc180008, 0x000003e8, 0x0000fc18, 0x00780000, 0x03e80000, 
    0x03e80008, 0x0000fc18, 0x000003e8, 0x00780000
};
/*0x50*/ static u32 data_50[] = {
    0x00000001, 0x00050000, 0x00050004, 0x00010002, 0x00060001, 0x00060005, 0x00020003, 0x00070002, 
    0x00070006, 0x00030000, 0x00040003, 0x00040007, 0x00000001, 0x00020003
};
/*0x88*/ static u32 data_88[] = {
    0x00040005, 0x00060007
};
/*0x90*/ static u32 data_90[] = {
    0x00000001, 0x00020003, 0x00040005, 0x00060007
};
/*0xA0*/ static u32 data_A0[] = {
    0x00000316, 0x000a0000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_165_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_165_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_165_modgfx_Func0(Object* arg0, s32 arg1, SRT* arg2, u32 arg3, s32 arg4, void* arg5);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/165/dll_165_modgfx_Func0.s")
