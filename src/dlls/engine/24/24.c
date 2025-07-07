#include "PR/ultratypes.h"
#include "sys/gfx/texture.h"
#include "sys/memory.h"

void dll_24_func_24C(void);

/*0x0*/ static const u32 _rodata_0[] = {
    0x436f756c, 0x64206e6f, 0x7420616c, 0x6c6f6361, 0x7465206d, 0x656d6f72, 0x7920666f, 0x72207761, 
    0x74657266, 0x7820646c, 0x6c0a0000
};

/*0x0*/ static s32 *_bss_0;
/*0x4*/ static s32 *_bss_4;
/*0x8*/ static s32 *_bss_8;
/*0xC*/ static s32 *_bss_C;
/*0x10*/ static s32 *_bss_10;
/*0x14*/ static s32 *_bss_14;
/*0x18*/ static s32 *_bss_18;
/*0x1C*/ static s32 *_bss_1C;
/*0x20*/ static s32 *_bss_20;
/*0x24*/ static s32 *_bss_24;
/*0x28*/ static s32 *_bss_28;
/*0x2C*/ static s32 *_bss_2C;
/*0x30*/ static s32 *_bss_30;
/*0x34*/ static s32 *_bss_34;
/*0x38*/ static s32 *_bss_38;
/*0x3C*/ static s32 *_bss_3C;
/*0x40*/ Texture *_bss_40;
/*0x44*/ Texture *_bss_44;
/*0x48*/ Texture *_bss_48;
/*0x4C*/ Texture *_bss_4C;
/*0x50*/ static s32 _bss_50[4];

// offset: 0x0 | ctor
void dll_24_ctor(s32 arg0) {
    s32 *state;

    state = malloc(0x3E80, 0x13, NULL);
    if (state != NULL) {
        _bss_4 = state;
        _bss_C = state + (0x3C0 / 4);
        _bss_14 = state + (0xB40 / 4);
        _bss_1C = state + (0xF00 / 4);
        _bss_0 = state + (0x12C0 / 4);
        _bss_8 = state + (0x1A40 / 4);
        _bss_10 = state + (0x2300 / 4);
        _bss_18 = state + (0x2A80 / 4);
        state += (0x3200 / 4);
        _bss_24 = state;
        state += (0x348 / 4);
        _bss_2C = state;
        state += (0x398 / 4);
        _bss_3C = state;
        state += (0x258 / 4);
        _bss_34 = state;
        _bss_20 = 0;
        _bss_28 = 0;
        _bss_38 = 0;
        _bss_30 = 0;
        _bss_40 = queue_load_texture_proxy(0x56);
        _bss_44 = queue_load_texture_proxy(0x59);
        _bss_48 = queue_load_texture_proxy(0x22);
        _bss_4C = queue_load_texture_proxy(0x57);
        dll_24_func_24C();
    }
}

// offset: 0x160 | dtor
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_dtor.s")

// offset: 0x24C | func: 0 | export: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_24C.s")

// offset: 0x564 | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_564.s")

// offset: 0x6E8 | func: 2 | export: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_6E8.s")

// offset: 0xC7C | func: 3 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_C7C.s")

// offset: 0x135C | func: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_135C.s")

// offset: 0x174C | func: 5 | export: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_174C.s")

// offset: 0x1B28 | func: 6 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_1B28.s")

// offset: 0x1C88 | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_1C88.s")

// offset: 0x1CC8 | func: 8 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_1CC8.s")
