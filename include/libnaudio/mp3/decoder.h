#ifndef _LIBNAUDIO_DECODER_H
#define _LIBNAUDIO_DECODER_H

#include "ultra64.h"
#include "unktypes.h"

// Copied from Perfect Dark
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
} DecoderThing;

typedef struct {
	s16 unk0[23];
	s16 unk2E[14];
} DecoderThing2;

typedef struct {
	u8 bytes[2];
	s8 unk2;
	s8 unk3;
} DecoderBytes;

typedef struct  {
	/*0x3d08*/ u32 l[22];
	/*0x3d60*/ u32 unk3d60;
	/*0x3d64*/ u32 s[3][13];
} DecoderScaleFac;

typedef struct {
	f32 unk0[18];
} DecoderStream4664;

struct DecoderStream;

// Struct def taken from Perfect Darkkimi
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
	DecoderScaleFac scalefac[2][1];
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
	/*0x4664*/ DecoderStream4664 unk4664[1][32];
	/*0x4f64*/ DecoderStream4664 unk4F64[2][32];
	/*0x6164*/ u8 unk6164[0x900];
	/*0x6a64*/ DecoderStream4664 unk6A64[2][32];
	/*0x7c64*/ u8 unk7C64[0x810];
	/*0x8474*/ s32 unk8474;
	/*0x8478*/ s32 (*decodeframefunc)(struct DecoderStream *stream);
	/*0x847c*/ s32 (*setsideinfofunc)(struct DecoderStream *stream);
} DecoderStream;

#endif
