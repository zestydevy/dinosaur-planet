// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include "PR/ultratypes.h"
#include "libnaudio/n_synthInternals.h"
#include "sys/audio/speaker.h"

u8 D_800BFE80[2];
struct var800BFE82 D_800BFE82;
u8 D_800BFE86[2];
u8 D_800BFE88[2];

void speaker_set_mode(u8 mode) {
	s32 i;

	D_800BFE82.surround = 0;
	D_800BFE82.mono = 0;
	D_800BFE82.headphone = 0;

	switch (mode) {
	case SPEAKERMODE_MONO:
        D_800BFE82.mono = 1;
		break;
	case SPEAKERMODE_HEADPHONE:
        D_800BFE82.headphone = 1;
		break;
	case SPEAKERMODE_SURROUND:
        D_800BFE82.surround = 1;
		break;
	}

	for (i = 0; i < 2; i++) {
		speaker_func_80063bb4(i, 0);
	}
}

void speaker_func_80063bb4(s32 index, s32 arg1) {
	if (arg1 == 0) {
		arg1 = D_800BFE80[index];
	}

	D_800BFE86[index] = 0;
	D_800BFE88[index] = 0;

	switch (arg1) {
	case 2:
		if (D_800BFE82.surround) {
			D_800BFE88[index] = 1;
		}
		break;
	case 3:
		if (D_800BFE82.surround) {
			D_800BFE86[index] = 1;
		}
		break;
	case 4:
		if (!D_800BFE82.mono) {
			D_800BFE86[index] = 1;
		}
		break;
	case 5:
		if (!D_800BFE82.mono) {
			D_800BFE86[index] = 1;
			D_800BFE88[index] = 1;
		}
		break;
	}

	D_800BFE80[index] = arg1;
}
