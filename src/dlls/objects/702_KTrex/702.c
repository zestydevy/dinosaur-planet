#include "common.h"
#include "sys/gfx/model.h"

typedef struct {
    u8 _unk0[0x26C - 0x0];
    s16 unk26C;
    u8 unk26E[0x574 - 0x26E];
} DLL702_Data;

/*0x0*/ static u32 _data_0[] = {
    0x00000002, 0x00000002
};
/*0x8*/ static u32 _data_8[] = {
    0x00090012, 0x00120000
};
/*0x10*/ static u32 _data_10[] = {
    0x00010002, 0x00030000
};
/*0x18*/ static u32 _data_18[] = {
    0x00040006, 0x00060000
};
/*0x20*/ static u32 _data_20[] = {
    0x0008000e, 0x00100011, 0x00100011
};
/*0x2C*/ static u32 _data_2C[] = {
    0x00010004, 0x00100000
};
/*0x34*/ static u32 _data_34[] = {
    0x00020008, 0x00200000
};
/*0x3C*/ static u32 _data_3C[] = {
    0x40004000, 0x40000000
};
/*0x44*/ static u32 _data_44[] = {
    0x80008000, 0x80000000
};
/*0x4C*/ static u32 _data_4C[] = {
    0x00400080, 0x01000000
};
/*0x54*/ static f32 _data_54[] = {
    0.006, 0.003, 0.003
};
/*0x60*/ static f32 _data_60[] = {
    0.0055, 0.012, 0.012
};
/*0x6C*/ static f32 _data_6C[] = {
    0, 0.025, 0.025
};
/*0x78*/ static f32 _data_78[] = {
    1, 0.975, 0.975
};
/*0x84*/ static u16 _data_84[2] = {0x06fa, 0x06fb};
/*0x88*/ static u16 _data_88[2] = {0x0691, 0x06fd};
/*0x8C*/ static u16 _data_8C[2] = {0x06fe, 0x0000};
/*0x90*/ static u16 _data_90[2] = {0x025b, 0x025c};
/*0x94*/ static u32 _data_94[] = { //uIDs?
    0x00032136, 0x0003213d, 0x00032128, 0x0003212f
};
/*0xA4*/ static u32 _data_A4[] = {
    0x00032130, 0x00032137, 0x00032123, 0x00032129
};
/*0xB4*/ static u32 _data_B4[] = {
    0x00032367, 0x00032369, 0x0003236a, 0x00032365
};
/*0xC4*/ static u32 _data_C4[] = {
    0x00032132, 0x00032139, 0x00032124, 0x0003212b
};
/*0xD4*/ static u16 _data_D4[] = {
    0x0566, 0x0567, 0x0568, 0x0569
};
/*0xDC*/ static u16 _data_DC[] = {
    0x0560, 0x0561, 0x0562, 0x0563
};
/*0xE4*/ static u32 _data_E4 = 0x00000000;
/*0xE8*/ static u32 _data_E8 = 0x02080104;
/*0xEC*/ static u32 _data_EC[] = {
    0x00000006, 0x00000069, 0x00000069, 0x000000ff, 0x00000000
};

/*0x0*/ static u8 _bss_0[0x28];
/*0x28*/ static u8 _bss_28[0x8];
/*0x30*/ static u8 _bss_30[0x28];
/*0x58*/ static void* _bss_58;
/*0x5C*/ static u8 _bss_5C[0x4];
/*0x60*/ static u8 _bss_60[0x2];
/*0x62*/ static u8 _bss_62[0x2];
/*0x64*/ static u8 _bss_64[0x4];
/*0x68*/ static u8 _bss_68[0x4];
/*0x6C*/ static u8 _bss_6C[0x4];
/*0x70*/ static u8 _bss_70[0x4];
/*0x74*/ static u8 _bss_74[0x4];
/*0x78*/ static u8 _bss_78[0x8];

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_0.s")

// offset: 0x120 | ctor
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_ctor.s")

// offset: 0x160 | dtor
void dll_702_dtor(void *dll) { }

// offset: 0x16C | func: 1 | export: 0
void dll_702_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_setup.s")

// offset: 0x448 | func: 2 | export: 1
void dll_702_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_control.s")

// offset: 0x72C | func: 3 | export: 2
void dll_702_update(Object *self) { }

// offset: 0x738 | func: 4 | export: 3
void dll_702_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_print.s")

// offset: 0xA78 | func: 5 | export: 4
void dll_702_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_free.s")

// offset: 0xB3C | func: 6 | export: 5
u32 dll_702_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_8 | MODFLAGS_EVENTS;
}

// offset: 0xB4C | func: 7 | export: 6
u32 dll_702_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL702_Data);
}

// offset: 0xB60 | func: 8 | export: 7
s16 dll_702_func_B60(Object* self) {
    DLL702_Data *objData;
    
    objData = (_bss_58 = self->data);
    return objData->unk26C;
}

// offset: 0xB88 | func: 9 | export: 8
void dll_702_func_B88(Object *self, s32 arg1) { 

}

// offset: 0xB98 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_B98.s")

// offset: 0xC2C | func: 11
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_C2C.s")
#else
u8 dll_702_func_C2C(u16 arg0) {
  s8 data_E8_local[4] = {0x02, 0x08, 0x01, 0x04};

  return data_E8_local[(arg0 >> 1) & 3];
}
#endif

// offset: 0xC74 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_C74.s")

// offset: 0xD5C | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_D5C.s")

// offset: 0xE24 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_E24.s")

// offset: 0xE88 | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_E88.s")

// offset: 0xEF0 | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_EF0.s")

// offset: 0xFE4 | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_FE4.s")

// offset: 0x119C | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_119C.s")

// offset: 0x12DC | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_12DC.s")

// offset: 0x1E9C | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_1E9C.s")

// offset: 0x1EF0 | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_1EF0.s")

// offset: 0x23EC | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_23EC.s")

// offset: 0x2454 | func: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2454.s")

// offset: 0x2644 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2644.s")

// offset: 0x28BC | func: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_28BC.s")

// offset: 0x29D0 | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_29D0.s")

// offset: 0x2AF0 | func: 27
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2AF0.s")

// offset: 0x2BA4 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2BA4.s")

// offset: 0x2C54 | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2C54.s")

// offset: 0x2CF8 | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2CF8.s")

// offset: 0x2D80 | func: 31
u32 dll_702_func_2D80(Object *self, u32 a1, u32 a2) {
    return 0;
}

// offset: 0x2D98 | func: 32
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2D98.s")

// offset: 0x2E64 | func: 33
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2E64.s")

// offset: 0x3160 | func: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3160.s")

// offset: 0x3208 | func: 35
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3208.s")

// offset: 0x3330 | func: 36
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3330.s")

// offset: 0x3490 | func: 37
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3490.s")

// offset: 0x3518 | func: 38
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3518.s")

// offset: 0x35A0 | func: 39
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_35A0.s")

// offset: 0x3720 | func: 40
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3720.s")

// offset: 0x3828 | func: 41
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3828.s")

// offset: 0x3AA0 | func: 42
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3AA0.s")

/*0x0*/ static const char str_0[] = "TREX: state stack overflow\n";
/*0x1C*/ static const char str_1C[] = "State Error!!!\n";
