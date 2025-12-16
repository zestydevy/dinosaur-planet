#ifndef _MP3_INTERNAL_H
#define _MP3_INTERNAL_H

#include "libnaudio/n_sndplayer.h"

// Copied from Perfect Dark
#define VERSION_2 0x0
#define VERSION_1 0x1

#define CHANNELMODE_STEREO      0
#define CHANNELMODE_JOINTSTEREO 1
#define CHANNELMODE_DUALMONO    2
#define CHANNELMODE_SINGLEMONO  3

#define LAYER_3 1
#define LAYER_2 2
#define LAYER_1 3

#define CRC_PROTECTED   0
#define CRC_UNPROTECTED 1

#define MP3RESPONSETYPE_NONE        0
#define MP3RESPONSETYPE_ACKNOWLEDGE 1
#define MP3RESPONSETYPE_WHISPER     2
#define MP3RESPONSETYPE_GREETING    3

#define MP3STATE_IDLE      0
#define MP3STATE_PLAYING   1
#define MP3STATE_PAUSED    2
#define MP3STATE_STOPPED   3
#define MP3STATE_LOADING   4
#define MP3STATE_UNPAUSING 5

typedef s32 (*mp3_dmafunc)(s32 arg0, u8 *arg1, s32 arg2, s32 arg3);

typedef struct {
	u8 unk0;
	u8 unk1;
	u8 unk2;
	u8 unk3;
	const char *unk4; // string of the characters '0' and '1'
} DecoderThing;

typedef struct {
	s16 unk0[23];
	s16 unk2E[14];
} DecoderThing2;

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
	/*0x38*/ u16 *var8009c3c8;
	/*0x3c*/ s32 var8009c3cc;
	/*0x40*/ s32 var8009c3d0;
	/*0x44*/ u32 *var8009c3d4[1];
	/*0x48*/ u32 var8009c3d8;
	/*0x4c*/ mp3_dmafunc dmafunc;
	/*0x50*/ u32 state;
	/*0x54*/ s32 currentvol;
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
	/*0x0004*/ mp3_dmafunc dmafunc;
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
	/*0x206c*/ s32 unk206c;
	/*0x2070*/ u16 unk2070[6][580];
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

extern struct mp3vars g_Mp3Vars; // 0x800bff00
extern struct mp3decfourbytes* D_800C01C0; // sizeof 0xA410
extern f32 *D_800C01C4;

s32 mp3_dec_init(void);

u32 mp3_main_init(void);
struct asistream* mp3_main_func_80072380(s32 arg0, mp3_dmafunc dmafunc, s32 filesize);
s32 mp3_main_func_8007245c(struct asistream* stream, u16** arg1, s32* arg2);
s32 mp3_main_func_80071cf0(struct asistream* stream);

s32 mp3_dec_set_side_info(struct asistream* stream);
s32 mp3_dec_decode_frame(struct asistream* stream);

f32 mp3_func_8007BEB0(f32 arg0);
f32 mp3_func_8007C080(f32 param1);
f32 mp3_func_80077900(f32 arg0, f32 arg1);

s32 mp3_util_func_80077D20(void*, void*, s32, s32, s32, s32, void**, void**);
s32 mp3_util_func_80077ED0(void*, void*, s32, s32, s32, void**, void**);
s32 mp3util_get_bits(u8 *buffer, s32 *offset, s32 numbits);

void mp3_func_80078070(struct asistream_4f64 *arg0, s32 arg1, struct asistream_4f64 *arg2, struct asistream_4f64 *arg3, void *arg4);
void mp3_func_80078F70(void*, s32, void*, void*);

#endif
