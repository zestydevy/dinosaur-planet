#include "PR/ultratypes.h"
#include "libnaudio/n_libaudio.h"
#include "common.h"

/*0x0*/ static const u32 _rodata_0[] = {
    0x414d5346, 0x583a2045, 0x72726f72, 0x20736f75, 0x6e642065, 0x66666563, 0x74732062, 0x616e6b20, 
    0x6d697373, 0x696e6721, 0x0a000000, 0x616d5366, 0x78506c61, 0x79457820, 0x6661696c, 0x65640a00, 
    0x616d5366, 0x78536574, 0x566f6c3a, 0x20576172, 0x6e696e67, 0x2c736f75, 0x6e642068, 0x616e646c, 
    0x65202725, 0x6427206f, 0x7574206f, 0x66207261, 0x6e67652e, 0x0a000000, 0x616d5366, 0x78536574, 
    0x566f6c3a, 0x20576172, 0x6e696e67, 0x2c696e76, 0x616c6964, 0x2068616e, 0x646c6520, 0x27256427, 
    0x2e0a0000, 0x616d5366, 0x78536574, 0x50697463, 0x683a2057, 0x61726e69, 0x6e672c73, 0x6f756e64, 
    0x2068616e, 0x646c6520, 0x27256427, 0x206f7574, 0x206f6620, 0x72616e67, 0x652e0a00, 0x616d5366, 
    0x78536574, 0x50697463, 0x683a2057, 0x61726e69, 0x6e672c69, 0x6e76616c, 0x69642068, 0x616e646c, 
    0x65202725, 0x64272e0a, 0x00000000, 0x616d5366, 0x7853746f, 0x703a2057, 0x61726e69, 0x6e672c73, 
    0x6f756e64, 0x2068616e, 0x646c6520, 0x27256427, 0x206f7574, 0x206f6620, 0x72616e67, 0x652e0a00, 
    0x616d5366, 0x78497350, 0x6c617969, 0x6e673a20, 0x5761726e, 0x696e672c, 0x736f756e, 0x64206861, 
    0x6e646c65, 0x20272564, 0x27206f75, 0x74206f66, 0x2072616e, 0x67652e0a, 0x00000000, 0x616d5366, 
    0x78476574, 0x44656661, 0x756c743a, 0x20576172, 0x6e696e67, 0x2c736f75, 0x6e642065, 0x66666563, 
    0x74207661, 0x6c756520, 0x27256427, 0x206f7574, 0x206f6620, 0x72616e67, 0x652e0a00, 0x616d536e, 
    0x64506c61, 0x7945783a, 0x20576172, 0x6e696e67, 0x2c736f75, 0x6e642027, 0x25642720, 0x6f757420, 
    0x6f662072, 0x616e6765, 0x2e0a0000, 0x616d536e, 0x64506c61, 0x7945783a, 0x20576172, 0x6e696e67, 
    0x2c736f75, 0x6e642068, 0x616e646c, 0x65202725, 0x6427206f, 0x7574206f, 0x66207261, 0x6e67652e, 
    0x0a000000
};
/*0x224*/ static const u32 _rodata_224[] = {
    0x67616d65, 0x2f616d73, 0x66782e63, 0x00000000
};
/*0x234*/ static const u32 _rodata_234[] = {
    0x00000000
};
/*0x238*/ static const u32 _rodata_238[] = {
    0x67616d65, 0x2f616d73, 0x66782e63, 0x00000000
};
/*0x248*/ static const u32 _rodata_248[] = {
    0x00000000
};
/*0x24C*/ static const u32 _rodata_24C[] = {
    0x67616d65, 0x2f616d73, 0x66782e63, 0x00000000
};
/*0x25C*/ static const u32 _rodata_25C[] = {
    0x00000000
};
/*0x260*/ static const u32 _rodata_260[] = {
    0x67616d65, 0x2f616d73, 0x66782e63, 0x00000000
};
/*0x270*/ static const u32 _rodata_270[] = {
    0x00000000, 0x414d5346, 0x583a2053, 0x6f667477, 0x61726520, 0x62756720, 0x696e206f, 0x626a6563, 
    0x74202863, 0x6f6e7472, 0x6f6c6675, 0x6e632025, 0x64290a00, 0x20202020, 0x20202061, 0x6d536678, 
    0x53746f70, 0x20697320, 0x72657175, 0x69726564, 0x20746f20, 0x72656c65, 0x61736520, 0x65666665, 
    0x63742025, 0x642e0a00, 0x414d5346, 0x583a2053, 0x6f667477, 0x61726520, 0x62756720, 0x696e206f, 
    0x626a6563, 0x74202863, 0x6f6e7472, 0x6f6c6675, 0x6e632025, 0x64290a00, 0x20202020, 0x20202061, 
    0x6d536678, 0x53746f70, 0x20697320, 0x72657175, 0x69726564, 0x20746f20, 0x73746f70, 0x20656666, 
    0x65637420, 0x25642e0a, 0x00000000
};
/*0x33C*/ static const u32 _rodata_33C[] = {
    0x67616d65, 0x2f616d73, 0x66782e63, 0x00000000
};
/*0x34C*/ static const u32 _rodata_34C[] = {
    0x00000000, 0x616d5366, 0x78536574, 0x50697463, 0x683a2057, 0x61726e69, 0x6e672c73, 0x6f756e64, 
    0x2068616e, 0x646c6520, 0x27256427, 0x206f7574, 0x206f6620, 0x72616e67, 0x652e0a00, 0x616d5366, 
    0x78536574, 0x50697463, 0x683a2057, 0x61726e69, 0x6e672c69, 0x6e76616c, 0x69642068, 0x616e646c, 
    0x65202725, 0x64272e0a, 0x00000000
};
/*0x3B8*/ static const u32 _rodata_3B8[] = {
    0x67616d65, 0x2f616d73, 0x66782e63, 0x00000000
};
/*0x3C8*/ static const u32 _rodata_3C8[] = {
    0x00000000
};
/*0x3CC*/ static const u32 _rodata_3CC[] = {
    0x67616d65, 0x2f616d73, 0x66782e63, 0x00000000
};
/*0x3DC*/ static const u32 _rodata_3DC[] = {
    0x00000000, 0x414d5346, 0x583a204d, 0x6178696d, 0x756d2073, 0x66782068, 0x616e646c, 0x65732061, 
    0x6c6c6f63, 0x61746564, 0x2c256420, 0x2866726f, 0x6d202573, 0x3a256429, 0x0a000000, 0x414d5346, 
    0x583a2061, 0x6c6c6f63, 0x53667848, 0x616e646c, 0x65206861, 0x6e646c65, 0x206f7574, 0x206f6620, 
    0x72616e67, 0x652c2025, 0x640a0000, 0x414d5346, 0x583a2066, 0x72656553, 0x66784861, 0x6e646c65, 
    0x20696e76, 0x616c6964, 0x2068616e, 0x646c652c, 0x2025640a, 0x00000000, 0x414d5346, 0x583a2066, 
    0x72656553, 0x66784861, 0x6e646c65, 0x206e6f74, 0x20757365, 0x642c2025, 0x640a0000, 0x414d5346, 
    0x583a2057, 0x61726e69, 0x6e67206d, 0x696e2f6d, 0x61782072, 0x616e6765, 0x20696465, 0x6e746963, 
    0x616c0a00
};

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
/*0x128*/ static void *_bss_128;

// offset: 0x0 | ctor
// TODO: this dll is currently not being compiled due to this function. asm-processor fails on its asm
//       and it's not matching. solving either issue would let compilation be re-enabled
// https://decomp.me/scratch/MtgTF
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_ctor.s")
#else
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
    queue_alloc_load_file((void*)&sp4C, 2);
    temp_v0 = sp4C[0];
    temp_a3 = sp4C[1] - temp_v0;
    if (temp_a3 != 0) {
        _bss_0 = malloc(temp_a3, 0xD, NULL);
        queue_load_file_region_to_ptr((void*)_bss_0, 3, temp_v0, temp_a3);
        alBnkfNew(_bss_0, (u8*)file_get_romaddr(3U, sp4C[1]));
    }
    free(sp4C);
    _bss_128 = func_80000B40(1U, 0x40U, 0xE, 0x40U, 0);
    queue_alloc_load_file((void*)&sp4C, 0);
    _bss_C = sp4C[0];
    _bss_10 = sp4C[1] - _bss_C;
    _bss_10 = _bss_10 / 14U;
    //_bss_10 = temp_t8 / 14U;
    free(sp4C);

    _bss_8 = 0x41;
    temp_a3 = _bss_8 << 5;
    _bss_4 = malloc(temp_a3, 0xD, NULL);
    bzero(_bss_4, temp_a3);
    _bss_8 = 0;
}
#endif

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
