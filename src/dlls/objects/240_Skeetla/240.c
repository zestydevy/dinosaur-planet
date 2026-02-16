#include "common.h"

typedef struct {
    u8 _unk0[0x490];
} DLL240_Data;

/*0x0*/ static u32 _data_0[] = {
    0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0xffffffff, 
    0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 
    0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 
    0x00000003, 0x00000003
};
/*0x68*/ static u32 _data_68[] = {
    0xffffffff, 0xffffff00, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffff0000
};
/*0x84*/ static u32 _data_84[] = {
    0x02060167, 0x01650206, 0x00000000
};

/*0x0*/ static u8 _bss_0[0x14];
/*0x14*/ static u8 _bss_14[0x4];
/*0x18*/ static u8 _bss_18[0x8];
/*0x20*/ static u8 _bss_20[0x4];
/*0x24*/ static u8 _bss_24[0x4];
/*0x28*/ static u8 _bss_28[0x4];
/*0x2C*/ static u8 _bss_2C[0x4];

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_0.s")

// offset: 0x68 | ctor
void dll_240_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_ctor.s")

// offset: 0xA8 | dtor
void dll_240_dtor(void *dll) { }

// offset: 0xB4 | func: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_B4.s")

// offset: 0x104 | func: 2 | export: 0
void dll_240_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_setup.s")

// offset: 0x294 | func: 3 | export: 1
void dll_240_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_control.s")

// offset: 0x970 | func: 4 | export: 2
void dll_240_update(Object *self) { }

// offset: 0x97C | func: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_97C.s")

// offset: 0xAA4 | func: 6 | export: 3
void dll_240_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_print.s")

// offset: 0xC40 | func: 7 | export: 4
void dll_240_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_free.s")

// offset: 0xCE4 | func: 8 | export: 5
u32 dll_240_get_model_flags(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_get_model_flags.s")

// offset: 0xCF4 | func: 9 | export: 6
u32 dll_240_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL240_Data);
}

// offset: 0xD08 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_D08.s")

// offset: 0xF04 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_F04.s")

// offset: 0x11F4 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_11F4.s")

// offset: 0x13C0 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_13C0.s")

// offset: 0x18D8 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_18D8.s")

// offset: 0x21C8 | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_21C8.s")

// offset: 0x235C | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_235C.s")

// offset: 0x23E8 | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_23E8.s")

// offset: 0x2594 | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_2594.s")

// offset: 0x2A10 | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_2A10.s")

// offset: 0x2C38 | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_2C38.s")

// offset: 0x2F1C | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_2F1C.s")

// offset: 0x2FE8 | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/240_Skeetla/dll_240_func_2FE8.s")
