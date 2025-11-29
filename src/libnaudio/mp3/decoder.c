// @DECOMP_OPT_FLAGS=-g
// @DECOMP_IDO_VERSION=7.1
#include "common.h"
#include "PR/gu.h"
#include "PR/os.h"

#define VERSION_2 0x0
#define VERSION_1 0x1

#define CHANNELMODE_STEREO      0
#define CHANNELMODE_JOINTSTEREO 1
#define CHANNELMODE_DUALMONO    2
#define CHANNELMODE_SINGLEMONO  3

typedef struct {
	u8 unk0;
	u8 unk1;
	u8 unk2;
	u8 unk3;
	u8 *unk4;
} mp3decthing;

typedef struct {
	s16 unk0[23];
	s16 unk2E[14];
} mp3decthing2;

typedef struct {
	u8 bytes[2];
	s8 unk2;
	s8 unk3;
} mp3decfourbytes;

typedef struct  {
	/*0x3d08*/ u32 l[22];
	/*0x3d60*/ u32 unk3d60;
	/*0x3d64*/ u32 s[3][13];
} asistream_scalefac;

typedef struct {
	f32 unk0[18];
} asistream_4f64;

typedef struct {
	/*0x0000*/ s32 unk0;
	/*0x0004*/ s32 (*dmafunc)(s32 stream, void *arg1, s32 arg2, s32 arg3);
	/*0x0008*/ s32 filesize;
	/*0x000c*/ s32 unkC;
	/*0x0010*/ s32 unk10;
	/*0x0014*/ s32 unk14;
	/*0x0018*/ s32 unk18;
	/*0x2000*/ u8 unk1C[0x2000];
	/*0x201c*/ s32 unk201C;
	/*0x2020*/ s32 unk2020;
	/*0x2024*/ u8 buffer[0x40];
	/*0x2064*/ s32 offset;
	/*0x2068*/ u32 unk2068;
	/*0x206c*/ u32 unk206C;
	/*0x2070*/ u16 unk2070[6][580];
	/*0x3ba0*/ s32 unk3BA0;
	/*0x3ba4*/ u32 version;
	/*0x3ba8*/ u32 layer;
	/*0x3bac*/ u32 crctype;
	/*0x3bb0*/ u32 bitrateindex;
	/*0x3bb4*/ u32 samplerateindex;
	/*0x3bb8*/ u32 haspadding;
	/*0x3bbc*/ u32 privatebit;
	/*0x3bc0*/ u32 channelmode;
	/*0x3bc4*/ u32 channelmodeext;
	/*0x3bc8*/ u32 copyright;
	/*0x3bcc*/ u32 isoriginal;
	/*0x3bd0*/ u32 emphasis;
	/*0x3bd4*/ s32 doneinitial;
	/*0x3bd8*/ u32 initialversion;
	/*0x3bdc*/ u32 initiallayer;
	/*0x3be0*/ u32 initialcrctype;
	/*0x3be4*/ u32 initialsamplerateindex;
	/*0x3be8*/ u32 initialchannelmode;
	/*0x3bec*/ u32 initialcopyright;
	/*0x3bf0*/ u32 initialisoriginal;
	/*0x3bf4*/ u32 main_data_begin;
	/*0x3bf8*/ u32 scfsi[1][32];
	/*0x3c78*/ u32 part2_3_length[2][1];
	/*0x3c80*/ u32 big_value[2][1];
	/*0x3c88*/ s32 global_gain[2][1];
	/*0x3c90*/ u32 scalefac_compress[2][1];
	/*0x3c98*/ u32 window_switching[2][1];
	/*0x3ca0*/ u32 block_type[2][1];
	/*0x3ca8*/ u32 mixed_block_flag[2][1];
	/*0x3cb0*/ u32 table_select[2][1][3];
	/*0x3cc8*/ u32 subblock_gain[2][1][3];
	/*0x3ce0*/ u32 region0_count[2][1];
	/*0x3ce8*/ u32 region1_count[2][1];
	/*0x3cf0*/ u32 preflag[2][1];
	/*0x3cf8*/ u32 scalefac_scale[2][1];
	/*0x3d00*/ u32 count1table_select[2][1];
	asistream_scalefac scalefac[2][1];
	/*0x3ef8*/ u32 unk3EF8;
	/*0x3efc*/ u32 unk3EFC[6];
	/*0x3f14*/ u32 unk3F14[26];
	/*0x3f7c*/ s32 bitrate;
	/*0x3f80*/ s32 samplerate;
	/*0x3f84*/ s32 unk3F84;
	/*0x3f88*/ s32 unk3F88;
	/*0x3f8c*/ s32 numchannels;
	/*0x3f90*/ s32 numgranules;
	/*0x3f94*/ s16 unk3F94[1][578];
	/*0x4418*/ u8 unk4418[1][578];
	/*0x465c*/ s32 unk465C[1];
	/*0x4660*/ s32 unk4660[1];
	/*0x4664*/ asistream_4f64 unk4664[1][32];
	/*0x4f64*/ asistream_4f64 unk4F64[2][32];
	/*0x6164*/ u8 unk6164[0x900];
	/*0x6a64*/ asistream_4f64 unk6A64[2][32];
	/*0x7c64*/ u8 unk7C64[0x810];
	/*0x8474*/ s32 unk8474;
	/*0x8478*/ s32 (*decodeframefunc)(struct asistream *stream);
	/*0x847c*/ s32 (*setsideinfofunc)(struct asistream *stream);
} DecoderStream;

void mp3_dec_unpack_scale_fac(void*, s32, s32);
s32 mp3_util_func_80077D20(void*, void*, s32, s32, s32, s32, void**, void**);
s32 mp3_util_func_80077ED0(void*, void*, s32, s32, s32, void**, void**);
s32 mp3_util_func_80077CEC(void*, void*, s32);
void mp3_func_80078070(asistream_4f64 *arg0, s32 arg1, asistream_4f64 *arg2, asistream_4f64 *arg3, void *arg4);
void mp3_func_80078F70(void*, s32, void*, void*);
s32 mp3_main_func_80071cf0(void*);
f32 mp3_func_80077900(f32, f32);

extern mp3decthing2 D_8009FF68[6];
extern f32 D_800C05D0[256];
extern f32 D_800C01D0[256];
extern s32 D_800A208C[22];
extern s32 D_8009FD88[];
extern s16 D_8009FE10[2][3][22];
extern u8 D_8009FF18[2][3][13];
extern s32 D_800A20E4[2][3][3][4];
extern s16 D_800A014C[2][3][576];
extern f32 D_800A1C4C[8];
extern f32 D_800A1C6C[8];
extern u32 g_BitRateTable[2][15];
extern u32 g_SampleRateTable[2][4];
extern f32 sine_block[4][36];
extern mp3decfourbytes *D_800C09D0[];
extern u8 D_800A2084[8];
extern f32 D_800A2204;
extern f32 D_800A2208;
extern f32 D_800A220C;
extern f32 D_800A2210;
extern f32 D_800A2214;
extern f32 D_800A2218;
extern f32 D_800C0010[36];
extern f32 D_800C0130[36];
extern mp3decfourbytes* D_800C01C0; // sizeof 0xA410
extern f32 *D_800C01C4;
extern f32 *D_800C01C8;
extern s8 D_800C0A58[];
extern s8 D_800C2C58[];
extern mp3decthing *binStringPointers[];

// mp3dec_init in pd
s32 mp3_dec_init(void) {
    s32 i;
    s32 sp258;
    s32 sp254;
    s32 sp250 = 1;
    s32 sp24C;
    u8* sp248;
    mp3decthing* sp244;
    mp3decfourbytes *sp240;
    u8 sp238[8] = D_800A2084;
    s32 sp234;
    u8 sp233;
    s32 sp22C;
    s32 sp228;
    s32 sp224;
    s32 sp220;
    s32 sp21C;
    s32 sp218;
    s16 sp18[256];

    sp224 = 0;
    for (i = 0; i < 36; i++) {
        sine_block[0][i] = sinf((i + 0.5f) * D_800A2204);
    }
    for (i = 0; i < 18; i++) {
        D_800C0010[i] = sinf((i + 0.5f) * D_800A2208);
    }
    for (i = 18; i < 24; i++) {
        D_800C0010[i] = 1.0f;
    }
    for (i = 24; i < 30; i++) {
        D_800C0010[i] = sinf(((i + 0.5f) - 18.0f) * D_800A220C);
    }
    for (i = 30; i < 36; i++) {
        D_800C0010[i] = 0.0f;
    }

    for (i = 0; i < 6; i++) {
        D_800C0130[i] = 0.0f;
    }
    for (i = 6; i < 12; i++) {
        D_800C0130[i] = sinf(((i + 0.5f) - 6.0f) * D_800A2210);
    }
    for (i = 12; i < 18; i++) {
        D_800C0130[i] = 1.0f;
    }
    for (i = 18; i < 36; i++) {
        D_800C0130[i] = sinf((i + 0.5f) * D_800A2214);
    }
    if (D_800C01C0 == NULL) {
        return 0;
    }
    bzero(D_800C01C0, 10500 * sizeof(mp3decfourbytes));
    for (sp254 = 0; sp254 < 34; sp254++) {
        sp244 = binStringPointers[sp254];
        if (sp244 == NULL) {
            D_800C09D0[sp254] = 0;
            continue;
        }

        D_800C09D0[sp254] = &D_800C01C0[sp224];
        sp240 = D_800C09D0[sp254];
        for (sp258 = 0; sp258 < 10500; sp258++) {
            sp240[sp258].unk2 = -1;
            sp240[sp258].unk3 = -1;
        }
        sp220 = 1;
        sp218 = 1;
        bzero(&sp18, sizeof(sp18));
        sp250 = 1;
        while (sp218 != 0) {
            sp218 = 0;
            for (sp21C = 0, sp244 = binStringPointers[sp254]; sp244->unk0 != 100; sp244++, sp21C++) {
                sp24C = sp18[sp21C];
                if (sp220 <= sp244->unk2) {
                    sp248 = &sp240[sp24C].bytes[sp244->unk4[sp220 - 1]] - 48;
                    if (*sp248 != 0) {
                        sp24C = sp18[sp21C] + *sp248;
                    } else {
                        *sp248 = sp250 - sp24C;
                        sp24C = sp250++;
                    }
                    if (sp244->unk2 == sp220) {
                        sp240[sp24C].unk2 = sp244->unk0;
                        sp240[sp24C].unk3 = sp244->unk1;
                    } else {
                        sp218++;
                    }
                    sp18[sp21C] = sp24C;
                }
            }
            sp220++;
        }
        sp224 += sp250;
        for (sp244 = binStringPointers[sp254]; sp244->unk0 != 100; sp244++) {
            sp234 = sp244->unk2 < 8 ? sp244->unk2 : 8;
            sp233 = 0;
            for (i = 0; i < sp234; i++) {
                if (sp244->unk4[i] - 48) {
                    sp233 |= sp238[i];
                }
            }

            if (sp244->unk2 >= 9) {
                D_800C2C58[sp254 * 256 + sp233] = 0;
                continue;
            }

            sp22C = 256 >> sp234;
            sp228 = (sp244->unk1 * 16) | sp244->unk0;
            for (i = 0; i < sp22C; i++) {
                D_800C0A58[sp254 * 256 + (sp233 | i)] = (s8) sp228;
                D_800C2C58[sp254 * 256 + (sp233 | i)] = (s8) sp234;
            }
        }
    }

    D_800C01C8 = D_800C01C4;
    if (D_800C01C8 == 0) {
        return 0;
    }
    for (i = 0; i < 8192; i++) {
        D_800C01C8[i] = mp3_func_80077900(i, D_800A2218);
    }
    for (i = 0; i < 256; i++) {
        D_800C01D0[i] = mp3_func_80077900(2.0f, i * -0.5f);
        D_800C05D0[i] = mp3_func_80077900(2.0f, i * -2.0f);
    }
    return 1;
}

// mp3dec00040164 in pd
s32 mp3_dec_8006e0a0(DecoderStream* stream, s32 gr, s32 ch) {
    s32 sp1BC;
    s32 sp1B8;
    s32 sp1B4;
    s32 sp1B0;
    s32 sp158[22] = D_800A208C;
	f32 sp100[22];
	f32 sp64[3][13];
	f32 sp60;
	s32 sfb;
	s32 sp58;
	s32 scale;
	s32 sp50;
	s32 sp4c;
	s32 sp48;
	s32 sp44;
	f32 *sp40;
	s16 *sp3c;
	u8 *sp38;
	s32 sp34;
	s32 sp30;
	s32 window_switching;
	s32 sp28;
	f32 *sp24;

	sp1BC = stream->samplerateindex + stream->version * 3;

	if (stream->window_switching[gr][ch] && stream->block_type[gr][ch] == 2) {
		if (stream->mixed_block_flag[gr][ch]) {
			sp1B8 = D_8009FF68[sp1BC].unk0[1];
		} else {
			sp1B8 = D_8009FF68[sp1BC].unk2E[1] * 3;
			sp1B0 = D_8009FF68[sp1BC].unk2E[1];
			sp1B4 = 0;
		}
	} else {
		sp1B8 = D_8009FF68[sp1BC].unk0[1];
	}

	sp60 = mp3_func_80077900(2, (stream->global_gain[gr][ch] - 210.0f) * 0.25f);
	scale = stream->scalefac_scale[gr][ch];

	for (sfb = 0; sfb < 22; sfb++) {
		sp4c = (stream->scalefac[gr][ch].l[sfb] + stream->preflag[gr][ch] * sp158[sfb]) * (scale + 1);
		sp100[sfb] = D_800C01D0[sp4c] * sp60;
	}

	for (sp58 = 0; sp58 < 3; sp58++) {
		for (sfb = 0; sfb < 13; sfb++) {
			sp50 = stream->subblock_gain[gr][ch][sp58];
			sp4c = stream->scalefac[gr][ch].s[sp58][sfb] * (scale + 1);
			sp64[sp58][sfb] = sp60 * D_800C05D0[sp50] * D_800C01D0[sp4c];
		}
	}

	sfb = 0;
	sp48 = 0;
	sp44 = stream->unk465C[ch];
	sp40 = stream->unk4664[ch][0].unk0;
	sp3c = stream->unk3F94[ch];
	sp38 = stream->unk4418[ch];
	sp34 = stream->block_type[gr][ch] == 2 && stream->mixed_block_flag[gr][ch] == 0;
	sp30 = stream->block_type[gr][ch] == 2 && stream->mixed_block_flag[gr][ch] != 0;
	window_switching = stream->window_switching[gr][ch];

	while (sp48 < sp44) {
		if (sp1B8 > sp44) {
			sp1B8 = sp44;
		}

		if (window_switching) {
			if (sp34 || (sp30 && sp48 >= 36)) {
				sp58 = (sp48 - sp1B4) / sp1B0;
				sp28 = sp1B4 + sp1B0;
			}
		}

		while (sp48 < sp1B8) {
			if (window_switching && (sp34 || (sp30 && sp48 >= 36))) {
				if (sp48 >= sp28) {
					sp28 += sp1B0;
					sp58++;
				}

				sp24 = sp64[sp58];

				if (*sp38++) {
					*sp40++ = -(D_800C01C8[*sp3c++] * sp24[sfb]);
				} else {
					*sp40++ = D_800C01C8[*sp3c++] * sp24[sfb];
				}
			} else {
				if (*sp38++) {
					*sp40++ = -(D_800C01C8[*sp3c++] * sp100[sfb]);
				} else {
					*sp40++ = D_800C01C8[*sp3c++] * sp100[sfb];
				}
			}

			sp48++;
		}

		if (stream->window_switching[gr][ch] && stream->block_type[gr][ch] == 2) {
			if (stream->mixed_block_flag[gr][ch]) {
				if (D_8009FF68[sp1BC].unk0[8] == sp48) {
					sp1B8 = D_8009FF68[sp1BC].unk2E[4] * 3;
					sfb = 3;
					sp1B0 = D_8009FF68[sp1BC].unk2E[sfb + 1] - D_8009FF68[sp1BC].unk2E[sfb];
					sp1B4 = D_8009FF68[sp1BC].unk2E[sfb] * 3;
				} else if (sp48 < D_8009FF68[sp1BC].unk0[8]) {
					sp1B8 = D_8009FF68[sp1BC].unk0[++sfb + 1];
				} else {
					sp1B8 = D_8009FF68[sp1BC].unk2E[++sfb + 1] * 3;
					sp1B0 = D_8009FF68[sp1BC].unk2E[sfb + 1] - D_8009FF68[sp1BC].unk2E[sfb];
					sp1B4 = D_8009FF68[sp1BC].unk2E[sfb] * 3;
				}
			} else {
				sp1B8 = D_8009FF68[sp1BC].unk2E[++sfb + 1] * 3;
				sp1B0 = D_8009FF68[sp1BC].unk2E[sfb + 1] - D_8009FF68[sp1BC].unk2E[sfb];
				sp1B4 = D_8009FF68[sp1BC].unk2E[sfb] * 3;
			}
		} else {
			sp1B8 = D_8009FF68[sp1BC].unk0[++sfb + 1];
		}
	}

	if (sp48 < 576) {
		bzero(sp40, stream->unk4660[ch] * 4);
		return 1;
	}

	return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/mp3/decoder/mp3_dec_unpack_scale_fac.s")

// mp3dec00041600 in pd
s32 mp3_dec_8006f530(DecoderStream* stream, UNK_TYPE_32 arg1, s32 arg2) {
    s32 sp70[2][3][3][4] = D_800A20E4;
    s32 sp60[4];
    s32 sp5C;
    u32* sp58;
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

    sp5C = stream->scalefac_compress[arg2][0];
    sp58 = stream->preflag[arg2];
    sp54 = stream->mixed_block_flag[arg2][0];
    sp50 = stream->block_type[arg2][0];
    sp4C = stream->window_switching[arg2][0];
    if (((stream->channelmodeext != 1U) && (stream->channelmodeext != 3U)) || (arg2 != 1)) {
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
    if (((stream->channelmodeext == 1) || (stream->channelmodeext == 3)) && (arg2 == 1)) {
        stream->unk3EF8 = sp5C % 2;
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
        bcopy(sp70[sp48][sp44][sp54 + 1], &sp28, 0x10);
    } else {
        bcopy(sp70[sp48][sp44][0], &sp28, 0x10);
    }
    sp24 = 0;
    if ((sp4C == 0) || ((sp4C != 0) && (sp50 != 2))) {
        for (i = 0; i < 4; i++) {
            sp20 = sp60[i];
            sp1C = (1 << sp20) - 1;
            for (j = 0; j < sp28[i]; j++) {
                stream->scalefac[0][arg2].l[sp24] = sp20 ? mp3_util_func_80077CEC(stream->unk1C, &stream->unk2020, sp20) : 0;
                if (arg2 != 0) {
                    stream->unk3EFC[sp24] = sp1C;
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
                    stream->scalefac[0][arg2].s[0][sp24] = sp20 ? mp3_util_func_80077CEC(stream->unk1C, &stream->unk2020, sp20) : 0;
                    stream->scalefac[0][arg2].s[1][sp24] = sp20 ? mp3_util_func_80077CEC(stream->unk1C, &stream->unk2020, sp20) : 0;
                    stream->scalefac[0][arg2].s[2][sp24] = sp20 ? mp3_util_func_80077CEC(stream->unk1C, &stream->unk2020, sp20) : 0;
                    if (arg2 != 0) {
                        stream->unk3F14[sp24] = sp1C;
                    }
                    sp24 += 1;
                }
            }
        } else {
            sp20 = sp60[0];
            sp1C = (1 << sp20) - 1;
            for (j = 0; j < 6; j++) {
                stream->scalefac[0][arg2].l[sp24] = sp20 ? mp3_util_func_80077CEC(stream->unk1C, &stream->unk2020, sp20) : 0;
                if (arg2 != 0) {
                    stream->unk3EFC[sp24] = sp1C;
                }
                sp24 += 1;
            }
            sp28[0] -= 6;
            sp24 = 3;
            for (i = 0; i < 4; i++) {
                sp20 = sp60[i];
                sp1C = (1 << sp20) - 1;
                for (j = 0; j < sp28[i]; j += 3) {
                    stream->scalefac[0][arg2].s[0][sp24] = sp20 ? mp3_util_func_80077CEC(stream->unk1C, &stream->unk2020, sp20) : 0;
                    stream->scalefac[0][arg2].s[1][sp24] = sp20 ? mp3_util_func_80077CEC(stream->unk1C, &stream->unk2020, sp20) : 0;
                    stream->scalefac[0][arg2].s[2][sp24] = sp20 ? mp3_util_func_80077CEC(stream->unk1C, &stream->unk2020, sp20) : 0;
                    if (arg2 != 0) {
                        stream->unk3F14[sp24] = sp1C;
                    }
                    sp24 += 1;
                }
            }
        }
    }
    return 1;
}

// mp3dec00042238 in pd
s32 mp3_dec_func_80070168(DecoderStream* stream, s32 arg1, s32 arg2) {
    s32 sp64;
    s32 sp60;
    s32 sp54[3];
    s16* sp50;
    u8* sp4C;
    s32 sp48;
    s16* sp44;
    u8* sp40;
    s32 i;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;

    sp64 = stream->unk2020;
    if (stream->version != 0) {
        mp3_dec_unpack_scale_fac(stream, arg1, arg2);
    } else {
        mp3_dec_8006f530(stream, arg1, arg2);
    }
    sp60 = stream->big_value[arg1][arg2] * 2;
    sp50 = D_8009FE10[stream->version][stream->samplerateindex];
    sp4C = D_8009FF18[stream->version][stream->samplerateindex];
    if (stream->window_switching[arg1][arg2] == 0 && stream->block_type[arg1][arg2] == 0) {
        sp54[0] = sp50[stream->region0_count[arg1][arg2]] + 1;
        if (sp54[0] > sp60) {
            sp54[0] = sp54[1] = sp60;
        } else {
            sp54[1] = sp50[stream->region0_count[arg1][arg2] + stream->region1_count[arg1][arg2] + 1] + 1;
            if (sp54[1] > sp60) {
                sp54[1] = sp60;
            }
        }
    } else {
        if (stream->block_type[arg1][arg2] == 2 && stream->mixed_block_flag[arg1][arg2] == 0) {
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
    sp44 = stream->unk3F94[arg2];
    sp40 = stream->unk4418[arg2];
    for (i = 0; i < 3; i++) {
        sp38 = stream->table_select[arg1][arg2][i];
        sp34 = D_8009FD88[sp38];
        sp30 = sp54[i];
        if (D_800C09D0[sp38] == 0) {
            sp2C = sp30 - sp48;
            bzero(sp44, sizeof(s16) * sp2C);
            sp44 += sp2C;
            sp48 = sp30;
        } else {
            sp48 = mp3_util_func_80077D20(stream->unk1C, &stream->unk2020, sp38, sp48, sp34, sp30, (void**)&sp44, (void**)&sp40);
        }
    }
    sp38 = stream->count1table_select[arg1][arg2] + 0x20;
    sp28 = stream->part2_3_length[arg1][arg2] + sp64;
    sp48 = mp3_util_func_80077ED0(stream->unk1C, &stream->unk2020, sp38, sp48, sp28, (void**)&sp44, (void**)&sp40);
    stream->unk2020 = sp28;
    if (sp48 > 0x240) {
        stream->unk465C[arg2] = 0x240;
    } else {
        stream->unk465C[arg2] = sp48;
    }
    if (sp48 < 0x240) {
        stream->unk4660[arg2] = 0x240 - sp48;
        bzero(sp44, sizeof(s16) * stream->unk4660[arg2]);
    } else {
        stream->unk4660[arg2] = 0;
    }
    return 1;
}

// mp3dec000427d8 in pd
s32 mp3_dec_func_800706f8(DecoderStream* stream, s32 arg1) {
    s16* sp14;
    f32* sp10;
    f32* spC;
    s32 i;

    sp14 = D_800A014C[stream->version][stream->samplerateindex];
    sp10 = (f32*)stream->unk4F64;
    spC = (f32*)stream->unk4664;
    i = 0;
    if ((stream->window_switching[arg1][0] != 0) && (stream->block_type[arg1][0] == 2)) {
        if (stream->mixed_block_flag[arg1][0] != 0) {
            while (i++ < 36) {
                (sp10++)[0] = (spC++)[0];
            }
        }
        while (i < 576) {
            sp10[sp14[i++]] = (spC++)[0];
        }
    } else {
        while (i++ < 576) {
            (sp10++)[0] = (spC++)[0];
        }
    }
    return 1;
}

// mp3dec_reduce_aliases in pd
s32 mp3_dec_reduce_aliases(DecoderStream *stream, s32 arg1, s32 arg2) {
    s32 i;

    if (stream->window_switching[arg1][arg2] != 0 && stream->block_type[arg1][arg2] == 2) {
        return 1;
    }

	for (i = 1; i < 32; i++) {
		asistream_4f64 *sp8 = &stream->unk4F64[arg2][i];
        f32 sp4;
        f32 sp0;
        sp4 = sp8->unk0[0];
        sp0 = sp8->unk0[-1];
        sp8->unk0[-1] = (sp0 * D_800A1C6C[0]) - (D_800A1C4C[0] * sp4);
        sp8->unk0[0] = (sp4 * D_800A1C6C[0]) + (D_800A1C4C[0] * sp0);
        sp4 = sp8->unk0[1];
        sp0 = sp8->unk0[-2];
        sp8->unk0[-2] = (sp0 * D_800A1C6C[1]) - (D_800A1C4C[1] * sp4);
        sp8->unk0[1] = (sp4 * D_800A1C6C[1]) + (D_800A1C4C[1] * sp0);
        sp4 = sp8->unk0[2];
        sp0 = sp8->unk0[-3];
        sp8->unk0[-3] = (sp0 * D_800A1C6C[2]) - (D_800A1C4C[2] * sp4);
        sp8->unk0[2] = (sp4 * D_800A1C6C[2]) + (D_800A1C4C[2] * sp0);
        sp4 = sp8->unk0[3];
        sp0 = sp8->unk0[-4];
        sp8->unk0[-4] = (sp0 * D_800A1C6C[3]) - (D_800A1C4C[3] * sp4);
        sp8->unk0[3] = (sp4 * D_800A1C6C[3]) + (D_800A1C4C[3] * sp0);
        sp4 = sp8->unk0[4];
        sp0 = sp8->unk0[-5];
        sp8->unk0[-5] = (sp0 * D_800A1C6C[4]) - (D_800A1C4C[4] * sp4);
        sp8->unk0[4] = (sp4 * D_800A1C6C[4]) + (D_800A1C4C[4] * sp0);
        sp4 = sp8->unk0[5];
        sp0 = sp8->unk0[-6];
        sp8->unk0[-6] = (sp0 * D_800A1C6C[5]) - (D_800A1C4C[5] * sp4);
        sp8->unk0[5] = (sp4 * D_800A1C6C[5]) + (D_800A1C4C[5] * sp0);
        sp4 = sp8->unk0[6];
        sp0 = sp8->unk0[-7];
        sp8->unk0[-7] = (sp0 * D_800A1C6C[6]) - (D_800A1C4C[6] * sp4);
        sp8->unk0[6] = (sp4 * D_800A1C6C[6]) + (D_800A1C4C[6] * sp0);
        sp4 = sp8->unk0[7];
        sp0 = sp8->unk0[-8];
        sp8->unk0[-8] = (sp0 * D_800A1C6C[7]) - (D_800A1C4C[7] * sp4);
        sp8->unk0[7] = (sp4 * D_800A1C6C[7]) + (D_800A1C4C[7] * sp0);
    }

    return 1;
}

// mp3dec_set_side_info in pd
s32 mp3_dec_set_side_info(DecoderStream* stream) {
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    s32 sp1C;

    if (stream->version != VERSION_2) {
        stream->unk206C = stream->channelmode == CHANNELMODE_SINGLEMONO ? 17 : 32;
    } else {
        stream->unk206C = stream->channelmode == CHANNELMODE_SINGLEMONO ? 9 : 17;
    }
    sp34 = stream->dmafunc(stream->unk0, &stream->buffer[stream->unk2068], stream->unk206C, -1);
    if (stream->unk206C != sp34) {
        return 0;
    }
    stream->unk18 += stream->unk206C;
    stream->numchannels = stream->channelmode == CHANNELMODE_SINGLEMONO ? 1 : 2;
    stream->numgranules = stream->version != VERSION_2 ? 2 : 1;
    if (stream->version != VERSION_2) {
        stream->main_data_begin = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 9);
        stream->numchannels == 1 
            ? mp3_util_func_80077CEC(stream->buffer, &stream->offset, 5)
            : mp3_util_func_80077CEC(stream->buffer, &stream->offset, 3);
    } else {
        stream->main_data_begin = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 8);
        stream->numchannels == 1 
            ? mp3_util_func_80077CEC(stream->buffer, &stream->offset, 1)
        : mp3_util_func_80077CEC(stream->buffer, &stream->offset, 2);
    }
    if (stream->version != VERSION_2) {
        for (sp30 = 0; sp30 < stream->numchannels; sp30++) {
            for (sp2C = 0; sp2C < 4; sp2C++) {
                stream->scfsi[sp30][sp2C] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 1);
            }
        }
    }
    if (stream->version != VERSION_2) {
        sp28 = 4;
    } else {
        sp28 = 9;
    }
    for (sp24 = 0; sp24 < stream->numgranules; sp24++) {
        for (sp30 = 0; sp30 < stream->numchannels; sp30++) {
            stream->part2_3_length[sp24][sp30] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 0xC);
            stream->big_value[sp24][sp30] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 9);
            stream->global_gain[sp24][sp30] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 8);
            stream->scalefac_compress[sp24][sp30] = sp28 != 0 ? mp3_util_func_80077CEC(stream->buffer, &stream->offset, sp28) : 0;
            stream->window_switching[sp24][sp30] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 1);
            if (stream->window_switching[sp24][sp30] != 0) {
                stream->block_type[sp24][sp30] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 2);
                stream->mixed_block_flag[sp24][sp30] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 1);
                for (sp20 = 0; sp20 < 2; sp20++) {
                    stream->table_select[sp24][sp30][sp20] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 5);
                }
                stream->table_select[sp24][sp30][2] = 0;
                for (sp1C = 0; sp1C < 3; sp1C++) {
                    stream->subblock_gain[sp24][sp30][sp1C] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 3);
                }
            } else {
                stream->block_type[sp24][sp30] = 0;
                stream->mixed_block_flag[sp24][sp30] = 0;
                for (sp20 = 0; sp20 < 3; sp20++) {
                    stream->table_select[sp24][sp30][sp20] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 5);
                }
                stream->region0_count[sp24][sp30] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 4);
                stream->region1_count[sp24][sp30] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 3);
            }
            if (stream->version != VERSION_2) {
                stream->preflag[sp24][sp30] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 1);
            }
            stream->scalefac_scale[sp24][sp30] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 1);
            stream->count1table_select[sp24][sp30] = mp3_util_func_80077CEC(stream->buffer, &stream->offset, 1);
        }
    }
    stream->bitrate = g_BitRateTable[stream->version][stream->bitrateindex];
    stream->samplerate = g_SampleRateTable[stream->version][stream->samplerateindex];
    if (stream->version != VERSION_2) {
        stream->unk3F84 = (stream->bitrate * 0x90) / stream->samplerate;
    } else {
        stream->unk3F84 = (stream->bitrate * 0x48) / stream->samplerate;
    }
    stream->unk3F88 = (stream->unk3F84 + stream->haspadding) - (stream->unk2068 + stream->unk206C);
    return 1;
}

// mp3dec_decode_frame in pd
s32 mp3_dec_decode_frame(DecoderStream* stream) {
	s32 sp954;
	s32 gr = 0;
	s32 ch;
	s32 sp948;
	s32 sp944;
	s32 sp940;
	s32 i;
	s32 j;
	u16 *sp934;
	asistream_4f64 sp34[32];
	f32 sp30;
	f32 sp2c;
	f32 sp28;
	s32 sp24;
	s32 sp20;

	sp934 = stream->unk2070[stream->unk3BA0];
	sp954 = mp3_main_func_80071cf0(stream);

	if (sp954 == -1) {
		return 0;
	}

	stream->unk2020 = (sp954 - stream->main_data_begin) * 8;

	if (stream->unk2020 < 0) {
		return 1;
	}

	for (ch = 0; ch < stream->numchannels; ch++) {
		mp3_dec_func_80070168(stream, gr, ch);
		mp3_dec_8006e0a0(stream, gr, ch);
	}

	mp3_dec_func_800706f8(stream, gr);

	if (stream->window_switching[gr][0] != 0 && stream->block_type[gr][0] == 2) {
		sp948 = 32;
	} else {
		sp944 = (stream->unk465C[0] - 1) / 18 + 1;
		sp948 = sp944;
	}

	for (ch = 0; ch < stream->numchannels; ch++) {
		mp3_dec_reduce_aliases(stream, gr, ch);

		if (stream->window_switching[gr][ch] != 0
				&& stream->block_type[gr][ch] == 2
				&& stream->mixed_block_flag[gr][ch] != 0) {
			sp940 = 0;
		} else if (stream->window_switching[gr][ch] == 0) {
			sp940 = 0;
		} else {
			sp940 = stream->block_type[gr][ch];
		}

		if (sp940 == 2) {
			for (i = 0; i < 2; i++) {
				mp3_func_80078F70(&stream->unk4F64[ch][i], i, &sp34[i], &stream->unk6A64[ch][i]);
			}
		} else {
			for (i = 0; i < 2; i++) {
				mp3_func_80078070(&stream->unk4F64[ch][i], i, &sp34[i], &stream->unk6A64[ch][i], sine_block[sp940]);
			}
		}

		if (stream->window_switching[gr][ch]
					&& stream->block_type[gr][ch] == 2
					&& stream->mixed_block_flag[gr][ch]) {
			sp940 = 2;
		}

		if (sp940 == 2) {
			for (i = 2; i < sp948; i++) {
				mp3_func_80078F70(&stream->unk4F64[ch][i], i, &sp34[i], &stream->unk6A64[ch][i]);
			}
		} else {
			for (i = 2; i < sp948; i++) {
				mp3_func_80078070(&stream->unk4F64[ch][i], i, &sp34[i], &stream->unk6A64[ch][i], sine_block[sp940]);
			}
		}

		while (i < 32) {
			bcopy(&stream->unk6A64[ch][i], &sp34[i], sizeof(asistream_4f64));
			bzero(&stream->unk6A64[ch][i], sizeof(asistream_4f64));
			i++;
		}

		sp30 = 65536;
		sp28 = 1;
		sp20 = sp30 * sp28 * 16;
		*sp934 = sp20 >> 16;
		sp934++;

		*sp934 = sp20 & 0xffff;
		sp934++;

		sp20 = -sp20;
		*sp934 = sp20 >> 16;
		sp934++;

		*sp934 = sp20 & 0xffff;
		sp934++;

		sp2c = 2048 / sp28;

		for (j = 0; j < 18; j++) {
			for (i = 0; i < 32; i++) {
				sp24 = sp34[i].unk0[j] * sp2c;
				*sp934 = sp24;
				sp934++;
			}
		}
	}

	return 1;
}
