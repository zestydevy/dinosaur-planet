#include "dlls/engine/29_gplay.h"
#include "sys/lfx.h"
#include "sys/lighting.h"
#include "dll.h"

/*0x0*/ static s32 data_0[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/*0x10*/ static s32 data_10[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

// size: 0x18
typedef struct {
    u8 _unk0[0x8 - 0x0];
    f32 unk8;
    f32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 _unk16[0x18 - 0x16];
} BSS0;

/*0x0*/ static BSS0 bss_0[8];

// offset: 0x0 | ctor
void dll_58_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_58_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_58_Func_18(void) {
    data_0[0] = 0;
    data_0[1] = 0;
    data_0[2] = 0;
    data_0[3] = 0;
    data_10[0] = 0;
    data_10[1] = 0;
    data_10[2] = 0;
    data_10[3] = 0;
    bss_0[0].unk10 = 0x64;
    bss_0[0].unk11 = 0x64;
    bss_0[0].unk12 = 0x64;
    bss_0[0].unk13 = 0;
    bss_0[0].unk14 = 0;
    bss_0[0].unk15 = 0;
    bss_0[1].unk10 = 0x64;
    bss_0[1].unk11 = 0x64;
    bss_0[1].unk12 = 0x64;
    bss_0[1].unk13 = 0xDC;
    bss_0[1].unk14 = 0xDC;
    bss_0[1].unk15 = 0xFA;
    bss_0[2].unk10 = 0;
    bss_0[2].unk11 = 0;
    bss_0[2].unk12 = 0;
    bss_0[2].unk13 = 0;
    bss_0[2].unk14 = 0;
    bss_0[2].unk15 = 0;
    bss_0[3].unk10 = 0x64;
    bss_0[3].unk11 = 0x64;
    bss_0[3].unk12 = 0x64;
    bss_0[3].unk13 = 0xFA;
    bss_0[3].unk14 = 0xDC;
    bss_0[3].unk15 = 0xDC;
    bss_0[4].unk10 = 0;
    bss_0[4].unk11 = 0;
    bss_0[4].unk12 = 0;
    bss_0[4].unk13 = 0;
    bss_0[4].unk14 = 0;
    bss_0[4].unk15 = 0;
    bss_0[5].unk10 = 0x64;
    bss_0[5].unk11 = 0x64;
    bss_0[5].unk12 = 0x64;
    bss_0[5].unk13 = 0xFA;
    bss_0[5].unk14 = 0xDC;
    bss_0[5].unk15 = 0xDC;
    bss_0[6].unk10 = 0;
    bss_0[6].unk11 = 0;
    bss_0[6].unk12 = 0;
    bss_0[6].unk13 = 0;
    bss_0[6].unk14 = 0;
    bss_0[6].unk15 = 0;
    bss_0[7].unk10 = 0x64;
    bss_0[7].unk11 = 0x64;
    bss_0[7].unk12 = 0x64;
    bss_0[7].unk13 = 0xFA;
    bss_0[7].unk14 = 0xDC;
    bss_0[7].unk15 = 0xDC;
    light_func_8001D548(bss_0[1].unk10, bss_0[1].unk11, bss_0[1].unk12, bss_0[1].unk13, bss_0[1].unk14, bss_0[1].unk15, 0xE, 0);
    light_func_8001D548(bss_0[3].unk10, bss_0[3].unk11, bss_0[3].unk12, bss_0[3].unk13, bss_0[3].unk14, bss_0[3].unk15, 0xD, 0);
    light_func_8001D548(bss_0[5].unk10, bss_0[5].unk11, bss_0[5].unk12, bss_0[5].unk13, bss_0[5].unk14, bss_0[5].unk15, 0xC, 0);
    light_func_8001D548(bss_0[7].unk10, bss_0[7].unk11, bss_0[7].unk12, bss_0[7].unk13, bss_0[7].unk14, bss_0[7].unk15, 0xB, 0);
    light_func_8001D548(0, 0, 0x32, 0xF0, 0xF0, 0xFF, 0xA, 0);
}

// offset: 0x264 | func: 1 | export: 1
void dll_58_Func_264(LightAction* arg0) {
    PlayerLightActions* lactions;
    BSS0* temp_v1;
    s32 idx;

    lactions = gDLL_29_Gplay->vtbl->get_current_player_lactions();
    idx = arg0->unk10 * 2;
    temp_v1 = &bss_0[idx];
    if (temp_v1->unk8 != 1.0f) {
        temp_v1->unk10 = (u8) (((f32) ((temp_v1 + 1)->unk10 - temp_v1->unk10) * temp_v1->unk8) + temp_v1->unk10);
        temp_v1->unk11 = (u8) (((f32) ((temp_v1 + 1)->unk11 - temp_v1->unk11) * temp_v1->unk8) + temp_v1->unk11);
        temp_v1->unk12 = (u8) (((f32) ((temp_v1 + 1)->unk12 - temp_v1->unk12) * temp_v1->unk8) + temp_v1->unk12);
        temp_v1->unk13 = (u8) (((f32) ((temp_v1 + 1)->unk13 - temp_v1->unk13) * temp_v1->unk8) + temp_v1->unk13);
        temp_v1->unk14 = (u8) (((f32) ((temp_v1 + 1)->unk14 - temp_v1->unk14) * temp_v1->unk8) + temp_v1->unk14);
        temp_v1->unk15 = (u8) (((f32) ((temp_v1 + 1)->unk15 - temp_v1->unk15) * temp_v1->unk8) + temp_v1->unk15);
    }
    idx = arg0->unk10 * 2 + 1;
    temp_v1 = &bss_0[idx];
    temp_v1->unk10 = arg0->unk15;
    temp_v1->unk11 = arg0->unk16;
    temp_v1->unk12 = arg0->unk17;
    temp_v1->unk13 = arg0->unk18;
    temp_v1->unk14 = arg0->unk19;
    temp_v1->unk15 = arg0->unk1a;
    data_0[arg0->unk10] = arg0->unk0;
    if (data_0[arg0->unk10] <= 0) {
        data_0[arg0->unk10] = 1;
    }
    if (arg0->unk14 & 1) {
        data_10[arg0->unk10] = 1;
    } else {
        data_10[arg0->unk10] = 0;
    }
    idx = arg0->unk10 * 2;
    temp_v1 = &bss_0[idx];
    temp_v1->unk8 = 0.0f;
    temp_v1->unkC = (f32) (1.0f / (f32) data_0[arg0->unk10]);
    lactions->unk0[arg0->unk10] = (s16) arg0->unke - 1;
}

// offset: 0x8A4 | func: 2 | export: 2
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/58/dll_58_Func_8A4.s")
#else
// https://decomp.me/scratch/PrL5x
void dll_58_Func_8A4(void) {
    u8 sp5D;
    u8 sp5C;
    u8 sp5B;
    u8 sp5A;
    u8 sp59;
    u8 sp58;
    s32 var_s1;
    BSS0* temp_v0;

    for (var_s1 = 0; var_s1 < 4; var_s1++){
        if (data_10[var_s1] != 0) {
            // FAKE?
            if (data_0[var_s1]){}
            if (data_0[var_s1] > 0) {
                temp_v0 = &bss_0[var_s1 * 2];
                sp58 = ((temp_v0[1].unk10 - temp_v0->unk10) * temp_v0->unk8) + temp_v0->unk10;
                sp59 = ((temp_v0[1].unk11 - temp_v0->unk11) * temp_v0->unk8) + temp_v0->unk11;
                sp5A = ((temp_v0[1].unk12 - temp_v0->unk12) * temp_v0->unk8) + temp_v0->unk12;
                sp5B = ((temp_v0[1].unk13 - temp_v0->unk13) * temp_v0->unk8) + temp_v0->unk13;
                sp5C = ((temp_v0[1].unk14 - temp_v0->unk14) * temp_v0->unk8) + temp_v0->unk14;
                sp5D = ((temp_v0[1].unk15 - temp_v0->unk15) * temp_v0->unk8) + temp_v0->unk15;
                temp_v0->unk8 += temp_v0->unkC;
                data_0[var_s1]--;
                if (data_0[var_s1] <= 0) {
                    temp_v0->unk10 = sp58;
                    temp_v0->unk11 = sp59;
                    temp_v0->unk12 = sp5A;
                    temp_v0->unk13 = sp5B;
                    temp_v0->unk14 = sp5C;
                    temp_v0->unk15 = sp5D;
                    data_0[var_s1] = 0;
                    light_func_8001D548(sp58, sp59, sp5A, sp5B, sp5C & 0xFF, sp5D & 0xFF, 0xE - var_s1, 0xFF);
                } else {
                    light_func_8001D548(sp58, sp59, sp5A, sp5B, sp5C, sp5D, 0xE - var_s1, bss_0[var_s1 * 2].unk8 * 255.0f);
                }
            }
        } else {
            data_0[var_s1] = data_0[var_s1];
            if (data_0[var_s1] > 0) {
                temp_v0 = &bss_0[var_s1 * 2];
                sp58 = ((temp_v0[1].unk10 - temp_v0->unk10) * temp_v0->unk8) + temp_v0->unk10;
                sp59 = ((temp_v0[1].unk11 - temp_v0->unk11) * temp_v0->unk8) + temp_v0->unk11;
                sp5A = ((temp_v0[1].unk12 - temp_v0->unk12) * temp_v0->unk8) + temp_v0->unk12;
                sp5B = ((temp_v0[1].unk13 - temp_v0->unk13) * temp_v0->unk8) + temp_v0->unk13;
                sp5C = ((temp_v0[1].unk14 - temp_v0->unk14) * temp_v0->unk8) + temp_v0->unk14;
                sp5D = ((temp_v0[1].unk15 - temp_v0->unk15) * temp_v0->unk8) + temp_v0->unk15;
                temp_v0->unk8 += temp_v0->unkC;
                data_0[var_s1]--;
                if (data_0[var_s1] <= 0) {
                    temp_v0->unk10 = sp58;
                    temp_v0->unk11 = sp59;
                    temp_v0->unk12 = sp5A;
                    temp_v0->unk13 = sp5B;
                    temp_v0->unk14 = sp5C;
                    temp_v0->unk15 = sp5D;
                    data_0[var_s1] = 0;
                }
                light_func_8001D548(sp58, sp59, sp5A, sp5B, sp5C, sp5D, 0xE - var_s1, 0);
            }
        }
    }
}
#endif

// offset: 0x14D4 | func: 3 | export: 3
void dll_58_Func_14D4(s32 arg0) {

}

// offset: 0x14E0 | func: 4 | export: 4
void dll_58_Func_14E0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {

}
