#ifndef _LIBNAUDIO_MP3_H
#define _LIBNAUDIO_MP3_H

#include <ultra64.h>
#include "libnaudio/n_sndplayer.h"

struct mp3thing {
	u16 unk00[580];
};

struct mp3vars {
	/*0x00*/ s32 romaddr;
	/*0x04*/ struct asistream *stream;
	/*0x08*/ ENVMIX_STATE *em_state;
	/*0x0c*/ s16 em_pan;
	/*0x0e*/ s16 em_volume;
	/*0x10*/ s16 em_cvolL;
	/*0x12*/ s16 em_cvolR;
	/*0x14*/ s16 em_dryamt;
	/*0x16*/ s16 em_wetamt;
	/*0x18*/ u16 em_lratl;
	/*0x1a*/ s16 em_lratm;
	/*0x1c*/ s16 em_ltgt;
	/*0x1e*/ u16 em_rratl;
	/*0x20*/ s16 em_rratm;
	/*0x22*/ s16 em_rtgt;
	/*0x24*/ s16 em_first;
	/*0x28*/ s32 em_delta;
	/*0x2c*/ s32 em_segEnd;
	/*0x30*/ s32 filesize;
	/*0x34*/ s32 dmaoffset;
	/*0x38*/ struct mp3thing *var8009c3c8;
	/*0x3c*/ s32 var8009c3cc;
	/*0x40*/ s32 var8009c3d0;
	/*0x44*/ u32 *var8009c3d4[1];
	/*0x48*/ u32 var8009c3d8;
	/*0x4c*/ void *dmafunc;
	/*0x50*/ u32 state;
	/*0x54*/ u32 currentvol;
	/*0x58*/ u32 var8009c3e8;
	/*0x5c*/ s16 currentpan;
	/*0x5e*/ s16 targetpan;
	/*0x60*/ u8 statetimer;
	/*0x61*/ u8 dualchannel;
};

struct mp3decfourbytes {
	u8 bytes[2];
	s8 unk02;
	s8 unk03;
};

struct asistream_scalefac {
	/*0x3d08*/ u32 l[22];
	/*0x3d60*/ u32 unk3d60;
	/*0x3d64*/ u32 s[3][13];
};

struct asistream_4f64 {
	f32 unk00[18];
};

struct asistream {
	/*0x0000*/ s32 unk00;
	/*0x0004*/ s32 (*dmafunc)(s32 arg0, void *arg1, s32 arg2, s32 arg3);
	/*0x0008*/ s32 filesize;
	/*0x000c*/ s32 unk0c;
	/*0x0010*/ s32 unk10;
	/*0x0014*/ s32 unk14;
	/*0x0018*/ s32 unk18;
	/*0x2000*/ u8 unk1c[0x2000];
	/*0x201c*/ s32 unk201c;
	/*0x2020*/ s32 unk2020;
	/*0x2024*/ u8 buffer[0x40];
	/*0x2064*/ s32 offset;
	/*0x2068*/ u32 unk2068;
	/*0x206c*/ u32 unk206c;
	/*0x2070*/ struct mp3thing unk2070[6];
	/*0x3ba0*/ s32 unk3ba0;
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
	struct asistream_scalefac scalefac[2][1];
	/*0x3ef8*/ u32 unk3ef8;
	/*0x3efc*/ u32 unk3efc[6];
	/*0x3f14*/ u32 unk3f14[26];
	/*0x3f7c*/ s32 bitrate;
	/*0x3f80*/ s32 samplerate;
	/*0x3f84*/ s32 unk3f84;
	/*0x3f88*/ s32 unk3f88;
	/*0x3f8c*/ s32 numchannels;
	/*0x3f90*/ s32 numgranules;
	/*0x3f94*/ s16 unk3f94[1][578];
	/*0x4418*/ u8 unk4418[1][578];
	/*0x465c*/ s32 unk465c[1];
	/*0x4660*/ s32 unk4660[1];
	/*0x4664*/ struct asistream_4f64 unk4664[1][32];
	/*0x4f64*/ struct asistream_4f64 unk4f64[2][32];
	/*0x6164*/ u8 unk6164[0x900];
	/*0x6a64*/ struct asistream_4f64 unk6a64[2][32];
	/*0x7c64*/ u8 unk7c64[0x810];
	/*0x8474*/ s32 unk8474;
	/*0x8478*/ s32 (*decoderframefunc)(struct asistream *stream);
	/*0x847c*/ s32 (*setsideinfofunc)(struct asistream *stream);
};

void func_8006758C(sndstate *state);
s32 func_800675EC(sndstate*);
void func_80067650(s32, s32);
void func_800676A4(s8, s32);

#endif
