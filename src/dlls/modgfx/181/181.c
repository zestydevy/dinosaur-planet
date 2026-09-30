#include "common.h"

/*0x0*/ static u32 data_0[] = {
    0xfc180578, 0x00000000, 0x00000000, 0x01900000, 0x00000000, 0x03e80578, 0x0000000f, 0x00000000, 
    0x09600000, 0x000f0000, 0xfc180578, 0x0fa00000, 0x001f0000, 0x01900fa0, 0x0000001f, 0x03e80578, 
    0x0fa0000f, 0x001f0000, 0x09600fa0, 0x000f001f
};
/*0x50*/ static u32 data_50[] = {
    0x00000002, 0x00060000, 0x00060004, 0x00010003, 0x00070001, 0x00070005
};
/*0x68*/ static u32 data_68[] = {
    0x00000001, 0x00020003
};
/*0x70*/ static u32 data_70[] = {
    0x00040005, 0x00060007
};
/*0x78*/ static u32 data_78[] = {
    0x00000001, 0x00020003, 0x00040005, 0x00060007, 0x00000002, 0x00040006, 0x00010003, 0x00050007
};
/*0x98*/ static u32 data_98 = 0x00000082;
/*0x9C*/ static u32 data_9C[] = {
    0x001a0000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_181_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_181_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_181_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/181/dll_181_modgfx_Func0.s")
