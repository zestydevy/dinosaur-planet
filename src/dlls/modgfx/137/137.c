#include "common.h"

/*0x0*/ static u32 data_0[] = {
    0x03e80000, 0x0190001f, 0x001f02c3, 0xfd3d0190, 0x0000001f, 0x0000fc18, 0x0190001f, 0x001ffd3d, 
    0xfd3d0190, 0x0000001f, 0xfc180000, 0x0190001f, 0x001ffd3d, 0x02c30190, 0x0000001f, 0x000003e8, 
    0x0190001f, 0x001f02c3, 0x02c30190, 0x0000001f, 0x00000000, 0xfbb4000f, 0x00000000
};
/*0x5C*/ static u32 data_5C[] = {
    0x00000001, 0x00080001, 0x00020008, 0x00020003, 0x00080003, 0x00040008, 0x00040005, 0x00080005, 
    0x00060008, 0x00060007, 0x00080007, 0x00000008
};
/*0x8C*/ static u32 data_8C[] = {
    0x00000001, 0x00020003, 0x00040005, 0x00060007, 0x00080000, 0x00080000
};
/*0xA4*/ static u32 data_A4[] = {
    0x00000001, 0x00020003, 0x00040005, 0x00060007
};
/*0xB4*/ static u32 data_B4[] = {
    0x00000002, 0x00040006
};
/*0xBC*/ static u32 data_BC = 0x00000032;
/*0xC0*/ static u32 data_C0[] = {
    0x001e0001, 0x00010000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_137_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_137_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_137_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/137/dll_137_modgfx_Func0.s")
