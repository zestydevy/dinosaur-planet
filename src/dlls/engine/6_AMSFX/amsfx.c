#include "PR/ultratypes.h"
#include "libnaudio/n_libaudio.h"
#include "sys/acache.h"
#include "sys/fs.h"
#include "common.h"

/*0x0*/ static const char str_0[] = "AMSFX: Error sound effects bank missing!\n";
/*0x2C*/ static const char str_2C[] = "amSfxPlayEx failed\n";
/*0x40*/ static const char str_40[] = "amSfxSetVol: Warning,sound handle '%d' out of range.\n";
/*0x78*/ static const char str_78[] = "amSfxSetVol: Warning,invalid handle '%d'.\n";
/*0xA4*/ static const char str_A4[] = "amSfxSetPitch: Warning,sound handle '%d' out of range.\n";
/*0xDC*/ static const char str_DC[] = "amSfxSetPitch: Warning,invalid handle '%d'.\n";
/*0x10C*/ static const char str_10C[] = "amSfxStop: Warning,sound handle '%d' out of range.\n";
/*0x140*/ static const char str_140[] = "amSfxIsPlaying: Warning,sound handle '%d' out of range.\n";
/*0x17C*/ static const char str_17C[] = "amSfxGetDefault: Warning,sound effect value '%d' out of range.\n";
/*0x1BC*/ static const char str_1BC[] = "amSndPlayEx: Warning,sound '%d' out of range.\n";
/*0x1EC*/ static const char str_1EC[] = "amSndPlayEx: Warning,sound handle '%d' out of range.\n";

/*0x0*/ static u32 _data_0[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static ALBankFile *_bss_0;
/*0x4*/ static void *_bss_4;
/*0x8*/ static u32 _bss_8;
/*0xC*/ static u32 _bss_C;
/*0x10*/ static u32 _bss_10;
/*0x14*/ static u32 _bss_14;
/*0x18*/ static u32 _bss_18;
/*0x1C*/ static u8 _bss_1C;
/*0x1D*/ static u8 _bss_1D;
/*0x1E*/ static u8 _bss_1E;
/*0x1F*/ static u8 _bss_1F;
/*0x20*/ static u8 _bss_20[0x8];
/*0x28*/ static u8 _bss_28[0x8];
/*0x30*/ static u8 _bss_30[0xf8];
/*0x128*/ static ACache *_bss_128;

// offset: 0x0 | ctor
void dll_6_ctor(s32 arg0) {
    u32* sp4C;
    s32 temp_v0;
    s32 temp_a3;
    ALSndpConfig sp30;

    _bss_14 = 0;
    _bss_18 = 0;
    _bss_1C = 0;
    _bss_1D = 0;
    sp4C = NULL;
    sp30.maxEvents = 0x96;
    sp30.maxStates = 0x20;
    sp30.maxSounds = 0x20;
    sp30.unk10 = 1;
    _bss_1E = 0;
    n_alSndpNew(&sp30);
    queue_alloc_load_file((void*)&sp4C, SFX_TAB);
    temp_v0 = sp4C[0];
    temp_a3 = sp4C[1] - temp_v0;
    if (temp_a3 != 0) {
        _bss_0 = mmAlloc(temp_a3, ALLOC_TAG_SFX_COL, NULL);
        queue_load_file_region_to_ptr((void*)_bss_0, SFX_BIN, temp_v0, temp_a3);
        alBnkfNew(_bss_0, (u8*)file_get_romaddr(SFX_BIN, sp4C[1]));
    }
    mmFree(sp4C);
    _bss_128 = acache_init(AUDIO_BIN, 0x40U, 0xE, 0x40U, 0);
    queue_alloc_load_file((void*)&sp4C, AUDIO_TAB);
    _bss_C = sp4C[0];
    _bss_10 = sp4C[1] - _bss_C;
    _bss_10 = _bss_10 / 14;
    mmFree(sp4C);

    _bss_8 = 0x41;
    temp_a3 = _bss_8 << 5;
    _bss_4 = mmAlloc(temp_a3, ALLOC_TAG_SFX_COL, NULL);
    bzero(_bss_4, temp_a3);
    _bss_8 = 0;
}

// offset: 0x22C | dtor
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_dtor.s")

// offset: 0x338 | func: 0 | export: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_338.s")

// offset: 0x480 | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_480.s")

// offset: 0x48C | func: 2 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_48C.s")

// offset: 0x7E4 | func: 3 | export: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_7E4.s")

// offset: 0x860 | func: 4 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_860.s")

// offset: 0x954 | func: 5 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_954.s")

// offset: 0xA1C | func: 6 | export: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_A1C.s")

// offset: 0xA6C | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_A6C.s")

// offset: 0xB48 | func: 8 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_B48.s")

// offset: 0xBBC | func: 9 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_BBC.s")

// offset: 0xDCC | func: 10 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_DCC.s")

// offset: 0xDE8 | func: 11 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_DE8.s")

// offset: 0xEE4 | func: 12 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_EE4.s")

// offset: 0xF2C | func: 13 | export: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_F2C.s")

// offset: 0x10D0 | func: 14 | export: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_10D0.s")

// offset: 0x1174 | func: 15 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_1174.s")

// offset: 0x1218 | func: 16 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_1218.s")

// offset: 0x1320 | func: 17 | export: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_1320.s")

// offset: 0x1504 | func: 18 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_1504.s")

// offset: 0x16E0 | func: 19 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_16E0.s")

// offset: 0x1C10 | func: 20 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_1C10.s")

// offset: 0x1C38 | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_1C38.s")

// offset: 0x1D58 | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_1D58.s")

// offset: 0x1E64 | func: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_1E64.s")

// offset: 0x1F78 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_1F78.s")

// offset: 0x2240 | func: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_2240.s")

// offset: 0x22FC | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_22FC.s")

// offset: 0x2438 | func: 27
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_2438.s")
