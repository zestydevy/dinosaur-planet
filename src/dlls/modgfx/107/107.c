#include "common.h"

/*0x0*/ static u32 data_0[] = {
    0x00000258, 0x0000000f, 0x001f0258, 0x00000000, 0x00000000, 0xfda80000, 0x0258000f, 0x0000fda8, 
    0x0000fda8, 0x001f0000
};
/*0x28*/ static u32 data_28[] = {
    0x00000001, 0x00020000, 0x00020003, 0x00000003, 0x00010001, 0x00030002
};
/*0x40*/ static u32 data_40[] = {
    0x00000000, 0x00010002, 0x00030000
};
/*0x4C*/ static u32 data_4C[] = {
    0x00000001, 0x00020003
};
/*0x54*/ static u32 data_54 = 0x00000046;
/*0x58*/ static u32 data_58[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0x64*/ static u32 data_64[] = {
    0x00050014, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_107_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_107_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_107_modgfx_Func0(Object* arg0, s32 arg1, SRT* arg2, u32 arg3, s32 arg4, void* arg5);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/107/dll_107_modgfx_Func0.s")

/*0x0*/ static const char str_0[] = "!!!! This modgfx needs an owner object\n";
/*0x2C*/ static const u32 rodata_2C[] = {
    0xbd8f5c29
};
