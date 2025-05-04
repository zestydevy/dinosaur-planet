#ifndef _SYS_AUDIO_SPEAKER_H
#define _SYS_AUDIO_SPEAKER_H

#include "PR/ultratypes.h"

#define SPEAKERMODE_MONO      1
#define SPEAKERMODE_STEREO    2
#define SPEAKERMODE_HEADPHONE 3
#define SPEAKERMODE_SURROUND  4

struct var800BFE82 {
	u8 surround;
	u8 mono;
	u8 headphone;
	u8 unk03;
};

extern struct var800BFE82 D_800BFE82;

extern u8 D_800BFE86[2];
extern u8 D_800BFE88[2];

void speaker_set_mode(u8 mode);
void speaker_func_80063bb4(s32 index, s32 arg1);

#endif //_SYS_AUDIO_SPEAKER_H
