// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"

void func_800794B0(N_ALCSPlayer *seqp) {
	s32 i;

	seqp->chanMask = 0xffff;

	for (i = 0; i < seqp->maxChannels; i++) {
		seqp->chanState[i].unk0e = 0xff;
		seqp->chanState[i].unk0d = 0xff;
	}
}

void func_80079538(N_ALCSPlayer *seqp, u32 status) {
	n_alCSPSendMidi(seqp, 0, status | 0xb0, 0xfc, 0);
}

void func_8007957C(N_ALCSPlayer *seqp, u32 arg1) {
	seqp->chanMask |= 1 << arg1;

	n_alCSPSendMidi(seqp, 0, arg1 | 0xb0, 0xfc, 0xff);
}

void func_800795E0(N_ALCSPlayer *seqp, s32 arg1, u8 arg2, u8 arg3) {
	n_alCSPSendMidi(seqp, 0, arg1 | 0xb0, 0xfd, arg3);
	n_alCSPSendMidi(seqp, 0, arg1 | 0xb0, 0xff, arg2);
}

void func_80079650(N_ALCSPlayer *seqp, u32 arg1, u8 arg2) {
	n_alCSPSendMidi(seqp, 0, arg1 | 0xb0, 0x41, arg2);
}

void func_8007969C(N_ALCSPlayer *seqp, u32 arg1, u8 arg2) {
	n_alCSPSendMidi(seqp, 0, arg1 | 0xb0, 0xfc, arg2);
}
