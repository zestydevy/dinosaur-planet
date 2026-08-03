#include "common.h"

typedef struct {
    s32 unk0;
    u16 unk4;
    u16 unk6;
    s32 unk8;
    s32 unkC;
    u8 _unk10[0x14 - 0x10];
    s32 unk14;
    s32 unk18;
    u8 _unk1C[0x24 - 0x1C];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    u8 _unk30[0x3C - 0x30];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    u8 _unk4C[0x58 - 0x4C];
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70[33];
    f32 unkF4[33];
    f32 unk178[33];
    f32 unk1FC[22];
    f32 unk254[22];
    f32 unk2AC[22];
    f32 unk304;
    f32 unk308;
    f32 unk30C;
    f32 unk310;
    s8 unk314;
    s8 unk315;
    s8 unk316;
    s8 unk317;
} BSS0;

/*0x0*/ static u8 data_0 = 1;
/*0x4*/ static u8 data_4[] = {
    0x00, 0x00, 0x01, 0x02, 
    0x03, 0x04, 0x05, 0x06, 
    0x07, 0x00, 0x00, 0x00
};
/*0x10*/ static u32 data_10 = 0x00000001;
/*0x14*/ static u32 data_14[] = {
    0x00000000, 0x00000000, 0x3f800000
};
/*0x20*/ static u32 data_20[] = {
    0xc47a0000, 0xc47a0000, 0xc47a0000
};
/*0x2C*/ static u32 data_2C = 0x00000000;
/*0x30*/ static u32 data_30[] = {
    0x01000000, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static BSS0* bss_0[2];
/*0x8*/ static u8 bss_8[0x8];
/*0x10*/ static u8 bss_10[0x60];

// offset: 0x0 | ctor
void dll_8_ctor(void* dll) {
    D_80092A7C[0] = -1;
    D_80092A7C[1] = -1;
    if (bss_0[0] != NULL) {
        mmFree(bss_0[0]);
    }
    if (bss_0[1] != NULL) {
        mmFree(bss_0[1]);
    }
    bss_0[0] = NULL;
    bss_0[1] = NULL;
}

// offset: 0x94 | dtor
void dll_8_dtor(void* dll) { }

// offset: 0xA0 | func: 0 | export: 0
void dll_8_Func_A0(Object *, Object *, EnvFxAction *, s32, u16);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/8_newfog/dll_8_Func_A0.s")

// offset: 0x464 | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/8_newfog/dll_8_Func_464.s")

// offset: 0x5AC | func: 2 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/8_newfog/dll_8_Func_5AC.s")

// offset: 0x1234 | func: 3 | export: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/8_newfog/dll_8_Func_1234.s")

// offset: 0x139C | func: 4 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/8_newfog/dll_8_Func_139C.s")

// offset: 0x13AC | func: 5 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/8_newfog/dll_8_Func_13AC.s")

// offset: 0x14B0 | func: 6 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/8_newfog/dll_8_Func_14B0.s")

// offset: 0x1608 | func: 7 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/8_newfog/dll_8_Func_1608.s")

// offset: 0x1764 | func: 8 | export: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/8_newfog/dll_8_Func_1764.s")

// offset: 0x18C0 | func: 9 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/8_newfog/dll_8_Func_18C0.s")

// offset: 0x18C8 | func: 10 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/8_newfog/dll_8_Func_18C8.s")

// offset: 0x1964 | func: 11
void dll_8_func_1964(s32 arg0) {
    f32 var_fv0;
    s32 i;

    i = 0;
    if (bss_0[arg0]->unk304 >= 1.0f) {
        bss_0[arg0]->unk4 &= 0xFEFF;
        bss_0[arg0]->unk308 = 0.0f;
        bss_0[arg0]->unk304 = 0.0f;
        bss_0[arg0]->unk30C = 1.0f;
        if ((bss_0[arg0]->unk316 != 0) && !(bss_0[arg0]->unk6 & 0x40)) {
            bss_0[arg0]->unk316 = 0;
        }
        for (i = 0; i < 33; i++) {
            bss_0[arg0]->unk70[i] = bss_0[arg0]->unkF4[i];
        }
        for (i = 0; i < 22; i++) {
            bss_0[arg0]->unk1FC[i] = bss_0[arg0]->unk254[i];
        }
        return;
    }
    if (bss_0[arg0]->unk315 != 0) {
        var_fv0 = ((f32) bss_0[arg0]->unk3C / 10.0f) * 60.0f;
        if (var_fv0 == 0.0f) {
            var_fv0 = 1.0f;
        }
        bss_0[arg0]->unk308 = 1.0f / var_fv0;
        for (i = 0; i < 33; i++) {
            bss_0[arg0]->unk178[i] = (bss_0[arg0]->unkF4[i] - bss_0[arg0]->unk70[i]) / var_fv0;
        }
        for (i = 0; i < 22; i++) {
            bss_0[arg0]->unk2AC[i] = (bss_0[arg0]->unk254[i] - bss_0[arg0]->unk1FC[i]) / var_fv0;
        }
        bss_0[arg0]->unk315 = 0;
    }
    for (i = 0; i < 33; i++) {
        bss_0[arg0]->unk70[i] += (bss_0[arg0]->unk178[i] * gUpdateRateF);
    }
    for (i = 0; i < 22; i++) {
        bss_0[arg0]->unk1FC[i] += bss_0[arg0]->unk2AC[i] * gUpdateRateF;
    }
    bss_0[arg0]->unk304 += (bss_0[arg0]->unk308 * gUpdateRateF);
    if ((bss_0[arg0]->unk4 & 1) && (bss_0[arg0]->unk310 > 0.0f)) {
        bss_0[arg0]->unk310 -= (255.0f * bss_0[arg0]->unk304);
        if (bss_0[arg0]->unk310 < 0.0f) {
            bss_0[arg0]->unk310 = 0.0f;
            data_0 = 1;
        }
    } else if ((bss_0[arg0]->unk4 & 4) && (bss_0[arg0]->unk310 < 255.0f)) {
        bss_0[arg0]->unk310 = bss_0[arg0]->unk304 * 255.0f;
        if (bss_0[arg0]->unk310 > 255.0f) {
            bss_0[arg0]->unk310 = 255.0f;
        }
    } else if (!(bss_0[arg0]->unk4 & 1) && (bss_0[arg0]->unk310 < 255.0f)) {
        bss_0[arg0]->unk310 = bss_0[arg0]->unk304 * 255.0f;
        if (bss_0[arg0]->unk310 > 255.0f) {
            bss_0[arg0]->unk310 = 255.0f;
        }
    }
    bss_0[arg0]->unk30C = bss_0[arg0]->unk304;
}

// offset: 0x1F58 | func: 12
void dll_8_func_1F58(EnvFxAction* arg0, u8 arg1) {
    s32 i;
    s32 var_v0;
    f32 temp;
    
    if (arg0->unk58 & 0x80) {
        var_v0 = 1;
    } else {
        var_v0 = 0;
    }
    /* default.dol
    if (bss_0[var_v0] == NULL) {
        STUBBED_PRINTF("warning in newfog dll no spare memory for available\n");
    }
    */
    bss_0[var_v0]->unk0 = 0;
    bss_0[var_v0]->unk317 = 1;
    for (i = 0; i < 33; i++) {
        bss_0[var_v0]->unk178[i] = 0.0f;
    }
    for (i = 0; i < 33; i++) {
        bss_0[var_v0]->unk70[i] = 0.0f;
    }
    for (i = 0; i < 22; i++) {
        bss_0[var_v0]->unk2AC[i] = 0.0f;
    }
    for (i = 0; i < 11; i++) {
        bss_0[var_v0]->unk1FC[i] = 1400.0f;
        bss_0[var_v0]->unk1FC[i + 11] = 1600.0f;
    }
    for (i = 0; i < 11; i++) {
        bss_0[var_v0]->unkF4[i] = (f32) arg0->unkC[data_4[i]];
        bss_0[var_v0]->unkF4[i + 11] = (f32) arg0->unk14[data_4[i]];
        bss_0[var_v0]->unkF4[i + 22] = (f32) arg0->unk1C[data_4[i]];
        bss_0[var_v0]->unk254[i] = (f32) arg0->unk3E[data_4[i]];
        bss_0[var_v0]->unk254[i + 11] = (f32) arg0->unk2E[data_4[i]];
    }
    bss_0[var_v0]->unk4 = arg0->unk58;
    bss_0[var_v0]->unk6 = arg0->unk59;
    bss_0[var_v0]->unk64 = 0.0f;
    bss_0[var_v0]->unk68 = 0.0f;
    bss_0[var_v0]->unk314 = -1;
    bss_0[var_v0]->unk6C = 0.0f;
    if (arg0->unk2A == 0) {
        arg0->unk2A = 1;
    }
    if (arg0->unk2A != 0) {
        
        bss_0[var_v0]->unk3C = arg0->unk2A;
        bss_0[var_v0]->unk48 = 1;
        bss_0[var_v0]->unk8 = arg0->unk2E[0];
        bss_0[var_v0]->unk5C = 1.0f / (f32) arg0->unk2A;
    } else {
        bss_0[var_v0]->unk3C = 0;
        bss_0[var_v0]->unk5C = 1.0f;
    }
    if (arg0->unk2C == 0) {
        arg0->unk2C = 1;
    }
    if (arg0->unk2C != 0) {
        bss_0[var_v0]->unk40 = arg0->unk2C;
        temp = ((f32) arg0->unk2C / 10.0f) * 60.0f;
        bss_0[var_v0]->unk58 = 255.0f / temp;
        bss_0[var_v0]->unkC = 1500;
        bss_0[var_v0]->unk60 = 1.0f / (f32) arg0->unk2C;
    } else {
        bss_0[var_v0]->unk40 = 0;
        bss_0[var_v0]->unk60 = 1.0f;
    }
    bss_0[var_v0]->unk44 = 0;
}

// offset: 0x22FC | func: 13 | export: 7
void dll_8_Func_22FC(s32* arg0, s32* arg1, s32* arg2, f32* arg3) {
    if (bss_0[0] != NULL) {
        *arg0 = bss_0[0]->unk24;
        *arg1 = bss_0[0]->unk28;
        *arg2 = bss_0[0]->unk2C;
        *arg3 = bss_0[0]->unk30C;
    }
}

// offset: 0x2350 | func: 14 | export: 8
void dll_8_Func_2350(s32* arg0, s32* arg1) {
    if (bss_0[0] != NULL) {
        *arg0 = bss_0[0]->unk14;
        *arg1 = bss_0[0]->unk18;
    }
}
