#include "common.h"

/*0x0*/ static u32 data_0[] = {
    0xfe0cfe0c, 0xfe0c0000, 0x000001f4, 0xfe0cfe0c, 0x00200020, 0x01f4fe0c, 0x01f40000, 0x0000fe0c, 
    0xfe0c01f4, 0x00200020, 0xfe0c01f4, 0xfe0c0000, 0x000001f4, 0x01f4fe0c, 0x00200020, 0x01f401f4, 
    0x01f40000, 0x0000fe0c, 0x01f401f4, 0x00200020
};
/*0x50*/ static u32 data_50[] = {
    0x00000004, 0x00050000, 0x00050001, 0x00010005, 0x00060001, 0x00060002, 0x00020006, 0x00070002, 
    0x00070003, 0x00030007, 0x00040003, 0x00040000, 0x00000001, 0x00020000, 0x00020003, 0x00040007, 
    0x00060004, 0x00060005
};
/*0x98*/ static u32 data_98[] = {
    0x00000001, 0x00020003, 0x00040005, 0x00060007
};
/*0xA8*/ static u32 data_A8 = 0x0000000a;
/*0xAC*/ static u32 data_AC[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_154_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_154_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_154_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/154/dll_154_modgfx_Func0.s")
