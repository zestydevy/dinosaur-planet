#include "common.h"

/*0x0*/ static u32 data_0[] = {
    0x00000258, 0x0258000f, 0x001f0258, 0x00000258, 0x00000000, 0xfda80000, 0x0258000f, 0x00000258, 
    0x0000fda8, 0x001f0000, 0xfda80000, 0x0258001f, 0x00000000, 0x0258fda8, 0x001f0000
};
/*0x3C*/ static u32 data_3C[] = {
    0x00000001, 0x00020000, 0x00020004, 0x00000005, 0x00040005, 0x00040003, 0x00000003, 0x00050000, 
    0x00010003, 0x00010002, 0x00040001, 0x00030004, 0x00000000, 0x00010002, 0x00030000
};
/*0x78*/ static u32 data_78[] = {
    0x00000001, 0x00020003, 0x00040005
};
/*0x84*/ static u32 data_84 = 0x00000000;
/*0x88*/ static u32 data_88[] = {
    0x000000aa, 0x00000000, 0x00000000, 0x00000000
};
/*0x98*/ static u32 data_98[] = {
    0x00050014, 0x00000000
};

// offset: 0x0 | ctor
void dll_180_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_180_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_180_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/180/dll_180_modgfx_Func0.s")

/*0x0*/ static const char str_0[] = "!!!! This modgfx needs an owner object\n";
/*0x28*/ static const char str_28[] = "";
/*0x2C*/ static const char str_2C[] = "";
