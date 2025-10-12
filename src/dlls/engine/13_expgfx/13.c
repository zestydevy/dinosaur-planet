#include "PR/ultratypes.h"
#include "common.h"
#include "prevent_bss_reordering.h"

/*0x0*/ static s16 _data_0[] = {
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
/*0x3C*/ static u32 _data_3C[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/*0x5C*/ static u32 _data_5C = 0x00000000;
/*0x60*/ static u32 _data_60 = 0x00000000;
/*0x64*/ static u32 _data_64 = 0x00000000;
/*0x68*/ static u32 _data_68[] = {
    0x00000000, 0x00000000
};
/*0x70*/ static u32 _data_70[] = {
    0x40000102, 0x00000000, 0x00000000, 0x00000000, 0x40000203, 0x00000000, 0x00000000, 0x00000000
};
/*0x90*/ static u32 _data_90 = 0x00000000;
/*0x94*/ static u32 _data_94 = 0x00000000;
/*0x98*/ static u32 _data_98 = 0x00000000;
/*0x9C*/ static u32 _data_9C[] = {
    0xfc180000, 0x03e803e8, 0x000003e8, 0x03e80000, 0xfc18fc18, 0x0000fc18
};
/*0xB4*/ static u32 _data_B4[] = {
    0xfc18fc18, 0x000003e8, 0xfc180000, 0x03e803e8, 0x0000fc18, 0x03e80000, 0xfc18fc18, 0x000003e8, 
    0xfc180000, 0x03e803e8, 0x0000fc18, 0x03e80000, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static u8 _bss_0[0x78];
/*0x78*/ static s32 _bss_78[30];
/*0xF0*/ static s8 _bss_F0[30];
/*0x110*/ static u8 _bss_110[0x8];
/*0x118*/ static Object *_bss_118[30];
/*0x190*/ static u8 _bss_190[0x4];
/*0x194*/ static u8 _bss_194[0x4];
/*0x198*/ static u8 _bss_198[0x4];
/*0x19C*/ static u8 _bss_19C[0x4];
/*0x1A0*/ static u8 _bss_1A0[0x4];
/*0x1A4*/ static u8 _bss_1A4[0x4];
/*0x1A8*/ static u8 _bss_1A8[0x4];
/*0x1AC*/ static u8 _bss_1AC[0x4];
/*0x1B0*/ static u8 _bss_1B0[0x1c0];
/*0x370*/ static u8 _bss_370[0x80];

// offset: 0x0 | ctor
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_ctor.s")

// offset: 0xE0 | dtor
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_dtor.s")

// offset: 0x158 | func: 0 | export: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_158.s")

// offset: 0x228 | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_228.s")

// offset: 0xC18 | func: 2 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_C18.s")

// offset: 0xCEC | func: 3 | export: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_CEC.s")

// offset: 0xED4 | func: 4 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_ED4.s")

// offset: 0x1048 | func: 5 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_1048.s")

// offset: 0x1080 | func: 6 | export: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_1080.s")

// offset: 0x1EEC | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_1EEC.s")

// offset: 0x1EF4 | func: 8 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_1EF4.s")

// offset: 0x1F0C | func: 9 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_1F0C.s")

// offset: 0x1F44 | func: 10 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_1F44.s")

// offset: 0x2060 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_2060.s")

// offset: 0x2254 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_2254.s")

// offset: 0x2298 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_2298.s")

// offset: 0x4684 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_4684.s")

// offset: 0x4A18 | func: 15
s32 dll_13_func_4A18(s16* arg0, s16* arg1, s16 arg2, s32 arg3, Object *arg4) {
    s32 var_t0;
    s32 var_t1_2;
    s16 var_v1;
    s32 var_t1;

    var_v1 = -1;
    var_t1 = 0;
    var_t0 = 0;
    for (var_t0 = 0; var_t0 < 30; var_t0++) {
        if ((arg4 == _bss_118[var_t0]) && (arg2 == _data_0[var_t0]) && (_bss_F0[var_t0] < 30)) {
            var_v1 = var_t0;
            var_t1 = 1;
            break;
        }
    }
    
    if (var_t1 != 0) {
        for (var_t1_2 = 0; var_t1_2 < 30; var_t1_2++) {
            if (!(_bss_78[var_v1] & (1 << var_t1_2))) {
                *arg1 = var_t1_2;
                *arg0 = var_v1;
                _bss_78[var_v1] |= (1 << var_t1_2);
                _bss_F0[var_v1]++;
                return 1;
            }
        }
    }
    
    var_t1 = 0;
    if (arg3 == -1) {
        for (var_t0 = 0; var_t0 < 29; var_t0++) {
            if (_bss_F0[var_t0] <= 0) {
                var_v1 = var_t0;
                var_t1 = 1;
                _bss_F0[var_t0] = 0;
                break;
            }
        }
    } else if (arg3 != -1) {
        var_t0 = arg3;
        if (_bss_F0[arg3] < 30) {
            var_v1 = arg3;
            var_t1 = 1;
        }
    }
    
    if (var_t1 != 0) {
        for (var_t1_2 = 0; var_t1_2 < 30; var_t1_2++) {
            if (arg2){} // fake

            if (!(_bss_78[var_v1] & (1 << var_t1_2))) {
                *arg1 = var_t1_2;
                *arg0 = var_v1;
                _bss_78[var_v1] |= (1 << var_t1_2);
                _data_0[var_t0] = arg2;
                _bss_F0[var_v1]++;
                return 1;
            }
        }

        return -1;
    }
    
    return -1;
}

// offset: 0x4DCC | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_4DCC.s")

// offset: 0x4F2C | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_4F2C.s")

// offset: 0x5068 | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_5068.s")

// offset: 0x52B4 | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_52B4.s")

// offset: 0x5358 | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_5358.s")

/*0x0*/ static const char str_0[] = "WARN no free particle\n";
/*0x18*/ static const char str_18[] = "error no texture\n";
/*0x2C*/ static const char str_2C[] = "expgfx.c: invalid tabindex\n";
/*0x48*/ static const char str_48[] = "expgfx.c: scale overflow\n";
/*0x64*/ static const char str_64[] = "expgfx.c: Error on owner free\n";
/*0x84*/ static const char str_84[] = "\t exp x %f\n";
/*0x90*/ static const char str_90[] = "\t exp z %f\n";
/*0x9C*/ static const char str_9C[] = "expgfx.c: addToTable usage overflow\n";
/*0xC4*/ static const char str_C4[] = "expgfx.c: exptab is FULL\n";
/*0xE0*/ static const char str_E0[] = "expgfx.c: mismatch in add/remove in exptab\n";
/*0x10C*/ static const char str_10C[] = "notexture \n";
/*0x118*/ static const char str_118[] = "notexture \n";
