#ifndef _LIBNAUDIO_MP3_INTERNAL_H
#define _LIBNAUDIO_MP3_INTERNAL_H

#include "libnaudio/n_sndplayer.h"

#include "mp3.h"

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
	const char *unk4; // string of the characters '0' and '1'
} DecoderThing;

typedef struct {
	s16 unk0[23];
	s16 unk2E[14];
} DecoderThing2;

s32 mp3_dec_init(void);

u32 mp3_main_init(void);
struct asistream* mp3_main_func_80072380(s32 arg0, s32 (*arg1)(s32, void*, s32, s32), s32 arg2);
s32 mp3_main_func_8007245c(struct asistream* arg0, struct mp3thing** arg1, s32* arg2);

#endif
