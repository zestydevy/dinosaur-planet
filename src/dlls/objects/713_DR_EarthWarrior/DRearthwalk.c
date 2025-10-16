#include "common.h"
#include "game/gametexts.h"

typedef struct {
    u8 _unk0[0xA58 - 0x0];
    u16 unkA58;
    u8 _unkA5A[0xA62 - 0xA5A];
    s8 unkA62;
    s8 unkA63;
} DLL713_Data;

/*0x0*/ static u32 _data_0 = 0x0854081a;
/*0x4*/ static u32 _data_4 = 0x08530852;
/*0x8*/ static u32 _data_8[] = {
    0x08170800, 0x08180800, 0x08191000, 0x081a1000
};
/*0x18*/ static u32 _data_18[] = {
    0x07bf1000, 0x08161000, 0x081c1000
};
/*0x24*/ static u32 _data_24[] = {
    0x08ed1000, 0x08550856, 0x08570858
};
/*0x30*/ static u32 _data_30[] = {
    0xc1100000, 0x00000000, 0xc1400000, 0x41100000, 0x00000000, 0xc1400000, 0x41100000, 0x00000000, 
    0x41400000, 0xc1100000, 0x00000000, 0x41400000
};
/*0x60*/ static u32 _data_60[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/*0x70*/ static u32 _data_70[] = {
    0x00000000, 0x00000000, 0x420c0000, 0x00000000, 0x00000000, 0xc20c0000
};
/*0x88*/ static u32 _data_88[] = {
    0x41c80000, 0x41c80000, 0x037b037b
};
/*0x94*/ static u32 _data_94[] = {
    0x00000008, 0x00000007, 0x00000009, 0x0000000a, 0x01c80000
};
/*0xA8*/ static u32 _data_A8 = 0x00180000;
/*0xAC*/ static u32 _data_AC[] = {
    0x3b4b295f, 0x00030003
};
/*0xB4*/ static f32 _data_B4[] = {
    0, 0.03, 0.05, 0.7, 0.6, 1.2, 0.8, 1.93, 
    1.72, 2.7
};
/*0xDC*/ static u32 _data_DC[] = {
    0x00020003, 0x00170016, 0x00040000
};
/*0xE8*/ static u32 _data_E8 = 0x01010101;
/*0xEC*/ static u32 _data_EC[] = {
    0x000a000a, 0x00000000, 0x00000000
};
/*0xF8*/ static u32 _data_F8[] = {
    0x00140014, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static u8 _bss_0[0x20];
/*0x20*/ static u8 _bss_20[0x8];
/*0x28*/ static u8 _bss_28[0x8];
/*0x30*/ static u8 _bss_30[0x40];

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_0.s")

// offset: 0x8C | ctor
void dll_713_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_ctor.s")

// offset: 0xCC | dtor
void dll_713_dtor(void *dll) { }

// offset: 0xD8 | func: 1 | export: 0
void dll_713_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_setup.s")

// offset: 0x408 | func: 2 | export: 1
void dll_713_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_control.s")

// offset: 0xABC | func: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_ABC.s")

// offset: 0xB54 | func: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_B54.s")

// offset: 0x107C | func: 5 | export: 2
void dll_713_update(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_update.s")

// offset: 0x138C | func: 6 | export: 3
void dll_713_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_print.s")

// offset: 0x160C | func: 7 | export: 4
void dll_713_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_free.s")

// offset: 0x1660 | func: 8 | export: 5
u32 dll_713_get_model_flags(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_get_model_flags.s")

// offset: 0x1670 | func: 9 | export: 6
u32 dll_713_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL713_Data);
}

// offset: 0x1684 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1684.s")

// offset: 0x197C | func: 11 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_197C.s")

// offset: 0x1990 | func: 12 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1990.s")

// offset: 0x19B8 | func: 13 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_19B8.s")

// offset: 0x19DC | func: 14 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_19DC.s")

// offset: 0x1BB8 | func: 15 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1BB8.s")

// offset: 0x1BE0 | func: 16 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1BE0.s")

// offset: 0x1CA4 | func: 17 | export: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1CA4.s")

// offset: 0x1CB4 | func: 18 | export: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1CB4.s")

// offset: 0x1D5C | func: 19 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1D5C.s")

// offset: 0x1D84 | func: 20 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1D84.s")

// offset: 0x1DAC | func: 21 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1DAC.s")

// offset: 0x1DBC | func: 22 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1DBC.s")

// offset: 0x1DC8 | func: 23 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1DC8.s")

// offset: 0x1EBC | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1EBC.s")

// offset: 0x2174 | func: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_2174.s")

// offset: 0x21A8 | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_21A8.s")

// offset: 0x21DC | func: 27
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_21DC.s")

// offset: 0x2454 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_2454.s")

// offset: 0x2704 | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_2704.s")

// offset: 0x2860 | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_2860.s")

// offset: 0x2D80 | func: 31
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_2D80.s")

// offset: 0x2F90 | func: 32
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_2F90.s")

// offset: 0x311C | func: 33
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_311C.s")

// offset: 0x31F8 | func: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_31F8.s")

// offset: 0x3210 | func: 35
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_3210.s")

// offset: 0x32EC | func: 36
s32 dll_713_func_32EC(Object* self, u8 arg1) {
    DLL713_Data* objData;

    objData = self->data;
    switch (arg1) {
        case 5:
            objData->unkA58 |= 0x80;
            objData->unkA62 = 0xA;
            //Set Mind Read text
            if (!gDLL_22_Subtitles->vtbl->func_21C0(self->id, GAMETEXT_0CF_DR_Mind_Read_messages_4)) {
                break;
            }
        // fallthrough
        case 1:
            objData->unkA62 = 2;
            func_80000450(self, self, 0x22C, 0, 0, 0);
            break;
        case 2:
            func_80000450(self, self, 0x22E, 0, 0, 0);
            objData->unkA58 &= 0xFF7F;
            //Set Mind Read text
            gDLL_22_Subtitles->vtbl->func_21C0(self->id, GAMETEXT_0D0_DR_Mind_Read_messages_5);
    }

    return 1;
}

// offset: 0x3420 | func: 37 | export: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_3420.s")
