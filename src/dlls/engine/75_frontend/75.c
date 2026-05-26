#include "common.h"

/*0x0*/ static u32 data_0[] = {
    0x03050307, 0x03080309, 0x030a030b, 0x030c0000
};
/*0x10*/ static u32 data_10 = 0x3c000000;
/*0x14*/ static u32 data_14[] = {
    0x3e000000, 0x00000000, 0x00000000
};

/*0x0*/ static u8 bss_0[0x1c];
/*0x1C*/ static u8 bss_1C[0x4];

// offset: 0x0 | ctor
void dll_75_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_ctor.s")

// offset: 0x7C | dtor
void dll_75_dtor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_dtor.s")

// offset: 0xE0 | func: 0 | export: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_func_E0.s")

// offset: 0x1C4 | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_func_1C4.s")

// offset: 0x284 | func: 2 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_func_284.s")

// offset: 0x354 | func: 3 | export: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_func_354.s")

// offset: 0x38C | func: 4 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_func_38C.s")

// offset: 0x6A0 | func: 5 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_func_6A0.s")

// offset: 0x77C | func: 6 | export: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_func_77C.s")

// offset: 0x790 | func: 7 | export: 7
void dll_75_func_790(FrontEndSlider* slider, s32 enable) {
    if (enable) {
        slider->unk4 |= 1;
    } else {
        slider->unk4 &= ~1;
    }
}

// offset: 0x7C0 | func: 8 | export: 8
s16 dll_75_func_7C0(FrontEndSlider* slider) {
    return slider->initialValue;
}

// offset: 0x7CC | func: 9 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_func_7CC.s")

// offset: 0x7E0 | func: 10 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_func_7E0.s")

// offset: 0x7F4 | func: 11 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_func_7F4.s")

// offset: 0x820 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_func_820.s")

// offset: 0x938 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_func_938.s")

// offset: 0xA00 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/75_frontend/dll_75_func_A00.s")
