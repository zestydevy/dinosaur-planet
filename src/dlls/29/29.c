#include <PR/ultratypes.h>
#include "common.h"

typedef struct {
    u8 unk0x0[0x14];
} huh2;

typedef struct {
    u8 unk0x0[0x10];
} huh3;

typedef struct {
    u8 unk0x0[0x16d4];
    f32 unk0x16d4;
    f32 unk0x16d8;
    f32 unk0x16dc;
    u8 unk0x16E0;
    u8 unk0x16E1;
    u8 unk0x016e[0x22];
    s16 unk0x1704;
} huh;

// size: 0x182c
typedef struct {
    u8 unk0x0[0x2f6];
    u8 unk0x2f6;
    u8 unk0x2f7;
    u8 unk0x2f8;
    f32 unk0x2fc;
    u8 unk0x300[0x152C];
} GplayStruct3;

typedef struct {
    u8 unk0x0;
    u8 unk0x1;
    u8 unk0x2[6];
    u8 unk0x8;
    u8 unk0x9;
    u8 unk0xa;
    s8 unk0xb;
    s8 unk0xc;
} GplayStruct4;

static const u32 _rodata_0[] = {
    0x67706c61, 0x794c6f61, 0x6447616d, 0x65206572, 0x726f723a, 0x20736176, 0x6567616d, 0x6520616e, 
    0x64206261, 0x636b7570, 0x20666169, 0x6c656420, 0x746f206c, 0x6f61642e, 0x0a000000, 0x67706c61, 
    0x794c6f61, 0x644f7074, 0x696f6e73, 0x20657272, 0x6f723a20, 0x73617665, 0x6f707469, 0x6f6e7320, 
    0x6661696c, 0x65642074, 0x6f206c6f, 0x61642e0a, 0x00000000, 0x20574152, 0x4e494e47, 0x203a204e, 
    0x6f742045, 0x6e6f7567, 0x68204d65, 0x6d6f7274, 0x20666f72, 0x20526573, 0x74617274, 0x20506f69, 
    0x6e742000, 0x0a574152, 0x4e494e47, 0x2067706c, 0x6179203a, 0x20526573, 0x74617274, 0x20506f69, 
    0x6e74204e, 0x6f742053, 0x6574200a, 0x00000000, 0x67616d65, 0x706c6179, 0x2e633a20, 0x67706c61, 
    0x79476574, 0x43757272, 0x656e7450, 0x6c617965, 0x724c6163, 0x74696f6e, 0x733a2070, 0x6c617965, 
    0x726e6f20, 0x6f766572, 0x666c6f77, 0x0a000000, 0x67616d65, 0x706c6179, 0x2e633a20, 0x67706c61, 
    0x79476574, 0x43757272, 0x656e7450, 0x6c617965, 0x72456e76, 0x61637469, 0x6f6e733a, 0x20706c61, 
    0x7965726e, 0x6f206f76, 0x6572666c, 0x6f770a00, 0x67706c61, 0x79416464, 0x54696d65, 0x3a204d41, 
    0x585f5449, 0x4d455341, 0x56455320, 0x72656163, 0x6865640a, 0x00000000, 0x4572726f, 0x7220696e, 
    0x2067706c, 0x61795365, 0x744f626a, 0x47726f75, 0x70537461, 0x74757320, 0x286d6170, 0x6e6f2025, 
    0x642c2067, 0x726f7570, 0x6e6f2025, 0x64290a00, 0x43484541, 0x54204f55, 0x54204f46, 0x2052414e, 
    0x47450a00, 0x43484541, 0x54204f55, 0x54204f46, 0x2052414e, 0x47450a00, 0x43484541, 0x54204f55, 
    0x54204f46, 0x2052414e, 0x47450a00, 0x43484541, 0x54204f55, 0x54204f46, 0x2052414e, 0x47450a00, 
    0x43494e45, 0x4d41204f, 0x5554204f, 0x46205241, 0x4e47450a, 0x00000000, 0x43494e45, 0x4d41204f, 
    0x5554204f, 0x46205241, 0x4e47450a, 0x00000000
};
static const u32 _rodata_230[] = {
    0x4727f800, 0x00000000, 0x00000000, 0x00000000
};

static u32 _data_0[] = {
    0x00000000, 0x076e08ec, 0x04fe00df, 0x00e000e1, 0x00e100e2, 0x00e300e4, 0x00e500e6, 0x00e700e8, 
    0x00e900ea, 0x00eb0492, 0x000005d0, 0x000000ed, 0x00ed00ed, 0x00f00000, 0x022900ee, 0x000000ef, 
    0x00000000, 0x000003ee, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x03490000, 0x04920492, 
    0x05470000, 0x05d00000, 0x076f0000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};
static u32 _data_F0[] = {
    0x03e003e0, 0x05db08ed, 0x050007ce, 0x04800452, 0x0452047b, 0x04ae0405, 0x0458036a, 0x04a6045a, 
    0x047c0000, 0x042e0493, 0x000005d1, 0x000003ad, 0x03ad03ad, 0x05170373, 0x044303b7, 0x04210000, 
    0x00000000, 0x00000397, 0x00000000, 0x00000473, 0x00000000, 0x000004a3, 0x00000000, 0x04930493, 
    0x05480000, 0x05d10601, 0x05dc0000, 0x00000000, 0x00000000, 0x00000000, 0x03e00000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};
static u32 _data_1E0[] = {
    0xc65d1568, 0xc2a40000, 0xc67344b6, 0x00000000
};

// .bss

static SaveStruct *DAT_81080264;
static void *DAT_81080268;
static GplayStruct4 *DAT_8108026C;
static GplayStruct3 DAT_81080274;
static s8 DAT_81081aa0;
static u8 _bss_0x1840[0x28];
static u8 _bss_0x1868[0x1e0];
static s8 DAT_81081cac;
static u8 _bss_0x1a48[0x4];
static s32 DAT_81081cb4;

void dll_29_func_3E4(s8,s8);
void dll_29_func_6AC();
static void dll_29_func_1314();

/*export*/ void dll_29_ctor(DLLFile *self)  {
    DAT_81080264 = (SaveStruct*)malloc(sizeof(SaveStruct), 0xFFFF00FF, NULL);
    dll_29_func_1314();
    DAT_81080268 = NULL;
    DAT_8108026C = (GplayStruct4*)malloc(128, 0xFFFF00FF, NULL);
    DAT_81081cac = -1;
    DAT_81081cb4 = -1;
}

/*export*/ void dll_29_dtor(DLLFile *self)  {
    free(DAT_81080264);
    if (DAT_81080268 != NULL) {
        free(DAT_81080268);
    }
    free(DAT_8108026C);
}

void dll_29_func_110(s8 param1) {
    dll_29_func_1314();
    DAT_81081aa0 = param1;
    DAT_81080274.unk0x2f6 = 1;
    DAT_81080274.unk0x2fc = -1.0f;
    dll_29_func_6AC();
    dll_29_func_6AC();
}

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_198.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_3E4.s")

void dll_29_func_638(s8 param1, s8 param2) {
    dll_29_func_3E4(param1, 0);
    DAT_81081aa0 = param2;
    dll_29_func_6AC();
    dll_29_func_6AC();
}

void dll_29_func_6AC() {
    if (DAT_81081aa0 != -1) {
        DAT_81080274.unk0x2f8 += 1;

        bcopy(&DAT_81080274, DAT_81080264, 0x13d4);

        gDLL_savegame->exports->flashSaveGame(
            DAT_81080264, 
            DAT_81081aa0 + (DAT_81080264->unk0x2f8 % 2) * 4, 
            0x1800, 
            1);
        
        if (DAT_81080268 != NULL) {
            bcopy(&DAT_81080274, DAT_81080268, 0x15d4);
        }
    }
}

u32 dll_29_func_79C() {
    u32 var;
    s32 ret;
    
    var = 1;

    ret = gDLL_savegame->exports->flashLoadGame(
        DAT_8108026C, 3, 0x80, 0);
    
    if (ret == 0) {
        bzero(DAT_8108026C, 0x80);
        var = 0;
        DAT_8108026C->unk0x8 = 0x7f;
        DAT_8108026C->unk0x9 = 0x7f;
        DAT_8108026C->unk0xa = 0x7f;
    }

    DAT_8108026C->unk0x0 = 0;
    
    if (DAT_8108026C->unk0xb < -7) {
        DAT_8108026C->unk0xb = -7;
    }
    if (DAT_8108026C->unk0xb > 7) {
        DAT_8108026C->unk0xb = 7;
    }
    if (DAT_8108026C->unk0xc < -7) {
        DAT_8108026C->unk0xc = -7;
    }
    if (DAT_8108026C->unk0xc > 7) {
        DAT_8108026C->unk0xc = 7;
    }

    return var;
}

void dll_29_func_8D8() {
    gDLL_savegame->exports->flashSaveGame(
        DAT_8108026C, 3, 0x80, 0);
}

GplayStruct4 *dll_29_func_930() {
    return DAT_8108026C;
}

void dll_29_func_94C(s32 param1) {

}

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_958.s")
#else
void dll_29_func_958(Vec3f *param1, s16 param2, s32 param3, s32 param4) {
    if ((param3 & 1) != 0) {
        bcopy(&DAT_81080274, DAT_81080264, 0x15d4);
    } else {
        if (func_8001EBE0() != 0) {
            ((huh*)&((huh2*)&DAT_81080274)[DAT_81080274.unk0x2f7])->unk0x1704 |= 1;
        } else {
            ((huh*)&((huh2*)&DAT_81080274)[DAT_81080274.unk0x2f7])->unk0x1704 &= ~1;
        }

        ((huh*)&((huh3*)&DAT_81080274)[DAT_81080274.unk0x2f7])->unk0x16d4 = param1->x;
        ((huh*)&((huh3*)&DAT_81080274)[DAT_81080274.unk0x2f7])->unk0x16d8 = param1->y;
        ((huh*)&((huh3*)&DAT_81080274)[DAT_81080274.unk0x2f7])->unk0x16dc = param1->z;
        ((huh*)&((huh3*)&DAT_81080274)[DAT_81080274.unk0x2f7])->unk0x16E0 = (u8)(param2 >> 8);
        ((huh*)&((huh3*)&DAT_81080274)[DAT_81080274.unk0x2f7])->unk0x16E1 = param4;

        bcopy(&DAT_81080274, DAT_81080264, 0x17ac);
    }

    if (DAT_81080268 != NULL) {
        bcopy(&DAT_81080274, DAT_81080268, 0x15d4);
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_AE0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_B3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_CBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_D20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_D70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_D94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_E74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_E90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_EAC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_ED4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_F04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_F30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_F60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_FA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_FE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_1014.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_109C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_10F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_115C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_121C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_1238.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_1254.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_1270.s")

static void dll_29_func_1314() {
    DAT_81081aa0 = -1;
    bzero(&DAT_81080274, 0x182c);
    bzero(DAT_81080264, 0x1800);
}

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_1378.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_139C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_143C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_14F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_1590.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_15B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_163C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_1680.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_16C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_1974.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_19B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_19FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_1A48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_1A90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_1AF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_1B78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/29/dll_29_func_1BC4.s")
