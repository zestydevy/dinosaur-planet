#include "common.h"

/*0x0*/ static u32 data_0[] = {
    0xfc180000, 0x00000000, 0x00000000, 0x0000fc18, 0x00000000, 0x03e80000, 0x0000000f, 0x00000000, 
    0x000003e8, 0x000f0000, 0xfc180fa0, 0x00000000, 0x001f0000, 0x0fa0fc18, 0x0000001f, 0x03e80fa0, 
    0x0000000f, 0x001f0000, 0x0fa003e8, 0x000f001f
};
/*0x50*/ static u32 data_50[] = {
    0x00000002, 0x00060000, 0x00060004, 0x00010003, 0x00070001, 0x00070005, 0x00000001, 0x00020003, 
    0x00040005, 0x00060007
};
/*0x78*/ static u32 data_78[] = {
    0x00000001, 0x00020003, 0x00040005, 0x00060007, 0x00000002, 0x00040006, 0x00010003, 0x00050007
};
/*0x98*/ static u32 data_98 = 0x00000104;
/*0x9C*/ static u32 data_9C[] = {
    0x001e0001, 0x01040000, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_121_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_121_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_121_modgfx_Func0(Object* arg0, s32 arg1, SRT* arg2, u32 arg3, s32 arg4, void* arg5);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/121/dll_121_modgfx_Func0.s")
