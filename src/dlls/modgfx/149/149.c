#include "common.h"

/*0x0*/ static u32 data_0[] = {
    0x001e0000, 0x00000000, 0x0000ffe2, 0x00000000, 0x000f0000, 0xffe203e8, 0x0000000f, 0x000f001e, 
    0x03e80000, 0x0000000f
};
/*0x28*/ static u32 data_28[] = {
    0x00000001, 0x00020000, 0x00020003, 0x00000000
};
/*0x38*/ static u32 data_38 = 0x00000001;
/*0x3C*/ static u32 data_3C[] = {
    0x00000001, 0x00020003
};
/*0x44*/ static u32 data_44 = 0x00020003;
/*0x48*/ static u32 data_48 = 0x0000000a;
/*0x4C*/ static u32 data_4C[] = {
    0x000f0050, 0x00000000, 0x00000000
};
/*0x58*/ static u32 data_58[] = {
    0x05270528, 0x00df00de, 0x00df0200, 0x01fb01fb, 0x00df00de, 0x00000000
};

// offset: 0x0 | ctor
void dll_149_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_149_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_149_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/modgfx/149/dll_149_modgfx_Func0.s")
