// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include "common.h"
#include "PR/gu.h"
#include "PR/os.h"

typedef struct  {
	/*0x3d08*/ u32 l[22];
	/*0x3d60*/ u32 unk3d60;
	/*0x3d64*/ u32 s[3][13];
}asistream_scalefac;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
    u8 pad20[0x2020 - 0x20];    
    s32 unk2020;
    u8 pad2024[0x3BA4-0x2024];
    s32 unk3BA4;
    u8 pad3BA8[0x3BB4-0x3BA8];
    s32 unk3BB4;
    u8 pad3BB8[0x3BC4-0x3BB8];
    s32 unk3BC4;
    u8 pad3BC8[0x3C78-0x3BC8];
    s32 unk3C78[2][1];
    s32 unk3C80[2][1];
    u8 pad3C88[0x3C90-0x3C88];
    s32 unk3C90[2][1];
    s32 unk3C98[2][1];
    s32 unk3CA0[2][1];
    s32 unk3CA8[2][1];
    s32 unk3CB0[2][1][3];
    u8 pad3CC8[0x3CE0-0x3CC8];
    s32 unk3CE0[2][1];
    s32 unk3CE8[2][1];
    s32 unk3CF0[2][1];
    u8 pad3CF8[0x3D00-0x3CF8];
    s32 unk3D00[2][1];
    asistream_scalefac scalefac[2][1]; // 3D08
    s32 unk3EF8;
    s32 unk3EFC[1];
    u8 pad3F00[0x3F14-0x3F00];
    s32 unk3F14[1];
    u8 pad3F18[0x3f94-0x3F18];
	s16 unk3F94[1][578];
	u8 unk4418[1][578];
    s32 unk465C[1];
    s32 unk4660[1];
} Arg0;

s32 mp3_dec_8006f530(Arg0* arg0, UNK_TYPE_32 arg1, s32 arg2);
void mp3_dec_unpack_scale_fac(void*, s32, s32);
s32 mp3_util_func_80077D20(void*, void*, s32, s32, s32, s32, void**, void**);
s32 mp3_util_func_80077ED0(void*, void*, s32, s32, s32, void**, void**);
s32 mp3_util_func_80077CEC(void*, void*, s32);
extern s32 D_8009FD88[];
extern s16 D_8009FE10[2][3][22];
extern u8 D_8009FF18[2][3][13];
extern s32 D_800C09D0[];
extern s8 D_800A20E4[0x120];

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/mp3/decoder/mp3_dec_init.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/mp3/decoder/mp3_dec_8006e0a0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/mp3/decoder/mp3_dec_unpack_scale_fac.s")

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/mp3/decoder/mp3_dec_8006f530.s")
#else
s32 mp3_dec_8006f530(Arg0* arg0, UNK_TYPE_32 arg1, s32 arg2) {
    s8 sp70[0x120] = D_800A20E4;
    s32 sp60[4];
    s32 sp5C;
    s32* sp58;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 sp48;
    s32 sp44;
    s32 i;
    s32 j;
    s32 sp38;
    s32 sp28[4];
    s32 sp24;
    s32 sp20;
    s32 sp1C;

    sp5C = arg0->unk3C90[arg2][0];
    sp58 = arg0->unk3CF0[arg2];
    sp54 = arg0->unk3CA8[arg2][0];
    sp50 = arg0->unk3CA0[arg2][0];
    sp4C = arg0->unk3C98[arg2][0];
    if (((arg0->unk3BC4 != 1U) && (arg0->unk3BC4 != 3U)) || (arg2 != 1)) {
        sp48 = 0;
        if (sp5C < 0x190) {
            sp60[0] = (sp5C >> 4) / 5;
            sp60[1] = (sp5C >> 4) % 5;
            sp60[2] = (sp5C % 16) >> 2;
            sp60[3] = sp5C % 4;
            sp58[0] = 0;
            sp44 = 0;
        } else if ((sp5C >= 0x190) && (sp5C < 0x1F4)) {
            sp60[0] = ((sp5C - 0x190) >> 2) / 5;
            sp60[1] = ((sp5C - 0x190) >> 2) % 5;
            sp60[2] = (sp5C - 0x190) % 4;
            sp60[3] = 0;
            sp58[0] = 0;
            sp44 = 1;
        } else if ((sp5C >= 0x1F4) && (sp5C < 0x200)) {
            sp60[0] = (sp5C - 0x1F4) / 3;
            sp60[1] = (sp5C - 0x1F4) % 3;
            sp60[2] = 0;
            sp60[3] = 0;
            sp58[0] = 1;
            sp44 = 2;
        }
    }
    if (((arg0->unk3BC4 == 1) || (arg0->unk3BC4 == 3)) && (arg2 == 1)) {
        arg0->unk3EF8 = sp5C % 2;
        sp38 = sp5C >> 1;
        sp48 = 1;
        if (sp38 < 0xB4) {
            sp60[0] = sp38 / 36;
            sp60[1] = (sp38 % 36) / 6;
            sp60[2] = (sp38 % 36) % 6;
            sp60[3] = 0;
            sp58[0] = 0;
            sp44 = 0;
        } else if ((sp38 >= 0xB4) && (sp38 < 0xF4)) {
            sp60[0] = ((sp38 - 0xB4) % 64) >> 4;
            sp60[1] = ((sp38 - 0xB4) % 16) >> 2;
            sp60[2] = (sp38 - 0xB4) % 4;
            sp60[3] = 0;
            sp58[0] = 0;
            sp44 = 1;
        } else if ((sp38 >= 0xF4) && (sp38 < 0xFF)) {
            sp60[0] = (sp38 - 0xF4) / 3;
            sp60[1] = (sp38 - 0xF4) % 3;
            sp60[2] = 0;
            sp60[3] = 0;
            sp58[0] = 0;
            sp44 = 2;
        }
    }
    if ((sp4C != 0) && (sp50 == 2)) {
        bcopy((sp48 * 0x90) + (sp44 * 0x30) + (sp54 * 0x10) + 0x10 + sp70, &sp28, 0x10);
    } else {
        bcopy((sp48 * 0x90) + (sp44 * 0x30) + sp70, &sp28, 0x10);
    }
    sp24 = 0;
    if ((sp4C == 0) || ((sp4C != 0) && (sp50 != 2))) {
        for (i = 0; i < 4; i++) {
            sp20 = sp60[i];
            sp1C = (1 << sp20) - 1;
            for (j = 0; j < sp28[i]; j++) {
                arg0->scalefac[0][arg2].l[sp24] = sp20 ? mp3_util_func_80077CEC(&arg0->unk1C, &arg0->unk2020, sp20) : 0;
                if (arg2 != 0) {
                    arg0->unk3EFC[sp24] = sp1C;
                }
                sp24 += 1;
            }
        }
    } else if (sp50 == 2) {
        if (sp54 == 0) {
            for (i = 0; i < 4; i++) {
                sp20 = sp60[i];
                sp1C = (1 << sp20) - 1;
                for (j = 0; j < sp28[i]; j += 3) {
                    arg0->scalefac[0][arg2].s[0][sp24] = sp20 ? mp3_util_func_80077CEC(&arg0->unk1C, &arg0->unk2020, sp20) : 0;
                    arg0->scalefac[0][arg2].s[1][sp24] = sp20 ? mp3_util_func_80077CEC(&arg0->unk1C, &arg0->unk2020, sp20) : 0;
                    arg0->scalefac[0][arg2].s[2][sp24] = sp20 ? mp3_util_func_80077CEC(&arg0->unk1C, &arg0->unk2020, sp20) : 0;
                    if (arg2 != 0) {
                        arg0->unk3F14[sp24] = sp1C;
                    }
                    sp24 += 1;
                }
            }
        } else {
            sp20 = sp60[0];
            sp1C = (1 << sp20) - 1;
            for (j = 0; j < 6; j++) {
                arg0->scalefac[0][arg2].l[sp24] = sp20 ? mp3_util_func_80077CEC(&arg0->unk1C, &arg0->unk2020, sp20) : 0;
                if (arg2 != 0) {
                    arg0->unk3EFC[sp24] = sp1C;
                }
                sp24 += 1;
            }
            sp28[0] -= 6;
            sp24 = 3;
            for (i = 0; i < 4; i++) {
                sp20 = sp60[i];
                sp1C = (1 << sp20) - 1;
                for (j = 0; j < sp28[i]; j += 3) {
                    arg0->scalefac[0][arg2].s[0][sp24] = sp20 ? mp3_util_func_80077CEC(&arg0->unk1C, &arg0->unk2020, sp20) : 0;
                    arg0->scalefac[0][arg2].s[1][sp24] = sp20 ? mp3_util_func_80077CEC(&arg0->unk1C, &arg0->unk2020, sp20) : 0;
                    arg0->scalefac[0][arg2].s[2][sp24] = sp20 ? mp3_util_func_80077CEC(&arg0->unk1C, &arg0->unk2020, sp20) : 0;
                    if (arg2 != 0) {
                        arg0->unk3F14[sp24] = sp1C;
                    }
                    sp24 += 1;
                }
            }
        }
    }
    return 1;
}
#endif

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/mp3/decoder/mp3_dec_func_80070168.s")
#else
s32 mp3_dec_func_80070168(Arg0* arg0, s32 arg1, s32 arg2) {
    s32 sp64;
    s32 sp60;
    s32 sp54[3];
    s16* sp50;
    u8* sp4C;
    s32 sp48;
    s16* sp44;
    u8* sp40;
    s32 i; // sp3C
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;

    sp64 = arg0->unk2020;
    if (arg0->unk3BA4 != 0) {
        mp3_dec_unpack_scale_fac(arg0, arg1, arg2);
    } else {
        mp3_dec_8006f530(arg0, arg1, arg2);
    }
    sp60 = arg0->unk3C80[arg1][arg2] * 2;
    sp50 = D_8009FE10[arg0->unk3BA4][arg0->unk3BB4];
    sp4C = D_8009FF18[arg0->unk3BA4][arg0->unk3BB4];
    if (arg0->unk3C98[arg1][arg2] == 0 && arg0->unk3CA0[arg1][arg2] == 0) {
        sp54[0] = sp50[arg0->unk3CE0[arg1][arg2]] + 1;
        if (sp54[0] > sp60) {
            sp54[0] = sp54[1] = sp60;
        } else {
            sp54[1] = sp50[arg0->unk3CE0[arg1][arg2] + arg0->unk3CE8[arg1][arg2] + 1] + 1;
            if (sp54[1] > sp60) {
                sp54[1] = sp60;
            }
        }
    } else {
        if (arg0->unk3CA0[arg1][arg2] == 2 && arg0->unk3CA8[arg1][arg2] == 0) {
            sp54[0] = (sp4C[2] * 3) + 3;
        } else {
            sp54[0] = sp50[7] + 1;
        }
        if (sp54[0] > sp60) {
            sp54[0] = sp60;
        }
        sp54[1] = sp60;
    }
    sp54[2] = sp60;
    sp48 = 0;
    sp44 = arg0->unk3F94[arg2];
    sp40 = arg0->unk4418[arg2];
    for (i = 0; i < 3; i++) {
        sp38 = arg0->unk3CB0[arg1][arg2][i];
        sp34 = D_8009FD88[sp38];
        sp30 = sp54[i];
        if (D_800C09D0[sp38] == 0) {
            sp2C = sp30 - sp48;
            bzero(sp44, sizeof(s16) * sp2C);
            sp44 += sp2C;
            sp48 = sp30;
        } else {
            sp48 = mp3_util_func_80077D20(&arg0->unk1C, &arg0->unk2020, sp38, sp48, sp34, sp30, (void**)&sp44, (void**)&sp40);
        }
    }
    sp38 = arg0->unk3D00[arg1][arg2] + 0x20;
    sp28 = arg0->unk3C78[arg1][arg2] + sp64;
    sp48 = mp3_util_func_80077ED0(&arg0->unk1C, &arg0->unk2020, sp38, sp48, sp28, (void**)&sp44, (void**)&sp40);
    arg0->unk2020 = sp28;
    if (sp48 > 0x240) {
        arg0->unk465C[arg2] = 0x240;
    } else {
        arg0->unk465C[arg2] = sp48;
    }
    if (sp48 < 0x240) {
        arg0->unk4660[arg2] = 0x240 - sp48;
        bzero(sp44, sizeof(s16) * arg0->unk4660[arg2]);
    } else {
        arg0->unk4660[arg2] = 0;
    }
    return 1;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/mp3/decoder/mp3_dec_func_800706f8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/mp3/decoder/mp3_dec_reduce_aliases.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/mp3/decoder/mp3_dec_set_side_info.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/mp3/decoder/mp3_dec_decode_frame.s")
