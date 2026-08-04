#include "common.h"
#include "sys/lighting.h"
#include "sys/segment_13D0.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    u8 _unk28[0x2C - 0x28];
    void* unk2C;
    u8 _unk30[0x7090 - 0x30];
    s32 unk7090;
    u8 _unk7094[0x70AC - 0x7094];
    s32 unk70AC;
    s32 unk70A0;
    s32 unk70B4;
    u8 _unk70B8[0x70FE - 0x70B8];
    u8 unk70FE;
    u8 unk70FF;
    void* unk7100;
} BSSA8;

// size: 0x1C
typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    u8 unk18;
    u8 _unk19[0x1C - 0x19];
} UnkCloudStruct;

/*0x0*/ static void* data_0 = NULL;
/*0x4*/ static u32 data_4[] = {
    0xffce0000, 0xffceffce, 0x0064ffce, 0x00320064, 0xffce0032, 0x0000ffce, 0xffce0000, 
    0x0032ffce, 0x00640032, 0x00320064, 0x00320032, 0x00000032
};
/*0x34*/ static u32 data_34[] = {
    0x00000000, 0x00000006, 0x00000002, 0x00000008, 0x00000002, 0x00000010, 0x00000008, 0x00000020, 
    0x00000028, 0x00000030
};
/*0x5C*/ static u32 data_5C[] = {
    0x00000001, 0x00000002, 0x00000002, 0x00000004, 0x00000003, 0x00000006, 0x00000006, 0x0000000c, 
    0x0000000c, 0x00000018, 0x00000018, 0x00000020, 0x00000020, 0x00000028, 0x00000028, 0x00000030, 
    0x00000030, 0x00000038, 0x00000001, 0x00000000
};
/*0xAC*/ static u32 data_AC = 0x00000001;
/*0xB0*/ static u32 data_B0 = 0x00000000;
/*0xB4*/ static u32 data_B4[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0xC0*/ static u32 data_C0[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0xCC*/ static u32 data_CC = 0x00000000;
/*0xD0*/ static u32 data_D0 = 0x00000000;
/*0xD4*/ static u32 data_D4 = 0x00000000;
/*0xD8*/ static u32 data_D8 = 0x000000ff;
/*0xDC*/ static u32 data_DC = 0x000000ff;
/*0xE0*/ static u32 data_E0 = 0x000000ff;
/*0xE4*/ static u32 data_E4 = 0x00000001;
/*0xE8*/ static u32 data_E8[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0xF4*/ static u32 data_F4[] = {
    0x00000000, 0x3f800000, 0x00000000
};
/*0x100*/ static u32 data_100 = 0x00000000;

/*0x0*/ static u8 bss_0[0x8];
/*0x8*/ static u8 bss_8[0x4];
/*0xC*/ static u8 bss_C[0x4];
/*0x10*/ static u8 bss_10[0x4];
/*0x14*/ static u8 bss_14[0x4];
/*0x18*/ static u8 _bss_18[0x8];
/*0x20*/ static u8 _bss_20[0x4];
/*0x24*/ static u8 bss_24[0x4];
/*0x28*/ static u8 bss_28[0x4];
/*0x2C*/ static u8 bss_2C[0x4];
/*0x30*/ static u8 bss_30[0x10];
/*0x40*/ static u8 bss_40[0x4];
/*0x44*/ static u8 bss_44[0x4];
/*0x48*/ static u8 bss_48[0x8];
/*0x50*/ static u8 _bss_50[0xc];
/*0x5C*/ static u8 bss_5C[0x4];
/*0x60*/ static u8 bss_60[0x4];
/*0x64*/ static u8 bss_64[0x4];
/*0x68*/ static u8 _bss_68[0x8];
/*0x70*/ static u8 _bss_70[0x8];
/*0x78*/ static u8 bss_78[0x4];
/*0x7C*/ static u8 bss_7C[0x4];
/*0x80*/ static u8 bss_80[0x14];
/*0x94*/ static u8 bss_94[0x4];
/*0x98*/ static u8 bss_98[0x4];
/*0x9C*/ static u8 bss_9C[0x4];
/*0xA0*/ static u8 _bss_A0[0x8];
/*0xA8*/ static BSSA8* bss_A8[8];
/*0xC8*/ static Texture* bss_C8;
/*0xCC*/ static Texture* bss_CC;
/*0xD0*/ static Texture* bss_D0;
/*0xD4*/ static Texture* bss_D4;
/*0xD8*/ static u8 bss_D8[0x8];

// offset: 0x0 | ctor
void newclouds_ctor(void* dll) {
    bss_C8 = texLoadTexture(0x16A);
    bss_D0 = texLoadTexture(0xE7);
    bss_D4 = texLoadTexture(0xDC);
    bss_CC = texLoadTexture(0x151);
}

// offset: 0x8C | dtor
void newclouds_dtor(void* dll) {
    if (bss_C8 != NULL) {
        texFreeTexture(bss_C8);
        bss_C8 = NULL;
    }
    if (bss_D0 != NULL) {
        texFreeTexture(bss_D0);
        bss_D0 = NULL;
    }
    if (bss_D4 != NULL) {
        texFreeTexture(bss_D4);
        bss_D4 = NULL;
    }
    if (bss_CC != NULL) {
        texFreeTexture(bss_CC);
        bss_CC = NULL;
    }
    if (data_0 != NULL) {
        dllFree(data_0);
    }
}

// offset: 0x178 | func: 0 | export: 0
void newclouds_Func_178(Object *, Object *, EnvFxAction *, s32);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/9_newclouds/newclouds_Func_178.s")

// offset: 0xB18 | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/9_newclouds/newclouds_Func_B18.s")

// offset: 0xBB8 | func: 2 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/9_newclouds/newclouds_Func_BB8.s")

// offset: 0xCD4 | func: 3 | export: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/9_newclouds/newclouds_Func_CD4.s")

// offset: 0x1388 | func: 4 | export: 4
void newclouds_Func_1388(Gfx **);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/9_newclouds/newclouds_Func_1388.s")

// offset: 0x1418 | func: 5 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/9_newclouds/newclouds_Func_1418.s")

// offset: 0x1434 | func: 6 | export: 6
void newclouds_Func_1434(Gfx **, u8, s32);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/9_newclouds/newclouds_Func_1434.s")

// offset: 0x1B98 | func: 7
void newclouds_func_1B98(EnvFxAction*, Object*, f32, f32, f32);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/9_newclouds/newclouds_func_1B98.s")

// offset: 0x2B44 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/9_newclouds/newclouds_func_2B44.s")

// offset: 0x2DF8 | func: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/9_newclouds/newclouds_func_2DF8.s")

// offset: 0x3844 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/9_newclouds/newclouds_func_3844.s")

// offset: 0x44AC | func: 11 | export: 7
void newclouds_Func_44AC(s32 arg0) {
    if (bss_A8[0] != NULL) {
        bss_A8[0]->unk70FE = arg0;
    }
}

// offset: 0x44D4 | func: 12
void newclouds_func_44D4(s32 arg0) {
    s32 i;
    s32 k;

    for (i = 0; i < 8; i++) {
        if (bss_A8[i] != NULL && arg0 == bss_A8[i]->unk7090) {
            break;
        }
    }
    k = i;
    if ((bss_A8[k] != NULL) && (i != 8) && (arg0 == bss_A8[k]->unk7090)) {
        if (bss_A8[k]->unk7100 != NULL) {
            mmFree(bss_A8[k]->unk7100);
            bss_A8[arg0]->unk10 = 0;
            bss_A8[arg0]->unk14 = 0;
            bss_A8[arg0]->unk0 = 0;
            bss_A8[arg0]->unk4 = 0;
            bss_A8[arg0]->unk18 = 0;
            bss_A8[arg0]->unk1C = 0;
            bss_A8[arg0]->unk8 = 0;
            bss_A8[arg0]->unkC = 0;
            bss_A8[arg0]->unk20 = 0;
            bss_A8[arg0]->unk24 = 0;
        }
        if (bss_A8[k]->unk2C != NULL) {
            mmFree(bss_A8[k]->unk2C);
            bss_A8[k]->unk2C = 0;
        }
        if (bss_A8[k] != NULL) {
            mmFree(bss_A8[k]);
            bss_A8[k] = 0;
        }
    }
}

// offset: 0x464C | func: 13
void newclouds_func_464C(UnkCloudStruct* arg0, f32 arg1, f32 arg2, s32 arg3) {
    f32 temp_fs0;
    f32 temp_fv0;
    s32 var_s1;
    s32 i;
    f32 temp;
    /*0x104*/ static s16 data_104 = 0;
    /*0x108*/ static f32 data_108 = 0.0f;
    /*0x10C*/ static f32 data_10C = 0.0f;

    for (i = 0; i < 8; i++) {
        if (bss_A8[i] != NULL && arg3 == bss_A8[i]->unk7090) {
            break;
        }
    }
    if ((bss_A8[i] != NULL) && (data_108 != 8.0f) && (arg3 == bss_A8[i]->unk7090)) {
        var_s1 = bss_A8[i]->unk70B4;
        while (var_s1 < (bss_A8[i]->unk70B4 + 4000)) {
            if (var_s1 == 1024) {
                bss_A8[i]->unk70AC = 0;
                bss_A8[i]->unk70B4 = 0;
                return;
            }
            if (var_s1 == 0) {
                data_104 = 0;
                data_10C = 0.0f;
                data_108 = 0.0f;
            }
            temp_fs0 = mathSinfInterp(data_104);
            temp_fv0 = mathCosfInterp(data_104);
            arg0[var_s1].unk14 = -0x10;
            arg0[var_s1].unk16 = 8;
            arg0[var_s1].unk18 = 0xFF;
            arg0[var_s1].unkC = (s16) (-16.0f * temp_fv0);
            arg0[var_s1].unkE = (s16) (-temp_fs0 * -16.0f);
            arg0[var_s1].unk10 = (s16) (16.0f * temp_fv0);
            arg0[var_s1].unk12 = (s16) (-temp_fs0 * 16.0f);
            if (0) { } // @fake
            arg0[var_s1].unk0 = data_108;
            arg0[var_s1].unk0 *= ((arg1 * arg2) / 1024);
            arg0[var_s1].unk4 = arg0[var_s1].unk0;
            arg0[var_s1].unk8 = 0.0f;
            temp = 63.999023f;
            temp += 63.9990230f;
            data_104 += temp;
            data_108 += 1.0f;
            var_s1 += 1;
        }
        bss_A8[i]->unk70B4 += 4000;
    }
}

// offset: 0x48F8 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/9_newclouds/newclouds_func_48F8.s")

/*0x0*/ static const char str_0[] = "!!! Error non-existant cloud id - %i - in snowKillSnowCloud\n";
/*0x40*/ static const char str_40[] = "warning in newcloud dll no spare memory for light available\n";
/*0x80*/ static const char str_80[] = "warning in newclouds dll no spare memory for clouds available\n";
/*0xC0*/ static const char str_C0[] = "warning in newclouds dll no spare memory for clouds available\n";
/*0x100*/ static const char str_100[] = "!!! Error non-existant cloud id - %i - in snowReposSnowCloud\n";
/*0x140*/ static const char str_140[] = "!!! Error non-existant cloud id - %i - in snowPrintSnowCloud\n";
/*0x180*/ static const char str_180[] = "!!! Error non-existant cloud id - %i - in snowFreeSnowCloud\n";
/*0x1C0*/ static const char str_1C0[] = "!!! Error non-existant cloud id - %i - in snowFreeSnowCloud\n";
