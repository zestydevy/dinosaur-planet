// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"
#include "mp3/mp3.h"
#include "sys/audio/speaker.h"

void func_8006AEC4(struct fx *fx, f32 outputrate);

Acmd *n_alMainBusPull(s32 sampleOffset, Acmd *p) {
	Acmd *ptr = p;
	s32 i;

	if (!func_800676F0(FIXED_SAMPLE, &ptr)) {
		aClearBuffer(ptr++, N_AL_MAIN_L_OUT, N_AL_DIVIDED << 1);
	}

	for (i = 0; i < n_syn->maxAuxBusses; i++) {
		ptr = (n_syn->mainBus->filter.handler)(sampleOffset, ptr, i);

		if (D_800BFE86[i]) {
			if (D_800BFE88[i]) {
				aMix(ptr++, 0, 0x8000, N_AL_AUX_L_OUT, N_AL_MAIN_L_OUT);
			} else {
				aMix(ptr++, 0, 0x7fff, N_AL_AUX_L_OUT, N_AL_MAIN_R_OUT);
			}
		} else {
			if (D_800BFE88[i]) {
				aMix(ptr++, 0, 0x8000, N_AL_AUX_L_OUT, N_AL_MAIN_R_OUT);
			} else {
				aMix(ptr++, 0, 0x7fff, N_AL_AUX_L_OUT, N_AL_MAIN_R_OUT);
			}

			aMix(ptr++, 0, 0x7fff, N_AL_AUX_L_OUT, N_AL_MAIN_L_OUT);
		}

		if (n_syn->auxBus[i].unk44->fx.unk02 > 0) {
			struct auxbus44 *bus44 = n_syn->auxBus[i].unk44;

			if (bus44->unk28) {
				func_8006AEC4(&bus44->fx, n_syn->outputRate);
			}

			n_aLoadADPCM(ptr++, 32, osVirtualToPhysical(&bus44->fx.unk08));

			n_aPoleFilter(ptr++, 0, N_AL_MAIN_L_OUT, 0, osVirtualToPhysical(bus44->unk2c) & 0xffffff);
			n_aPoleFilter(ptr++, 0, N_AL_MAIN_R_OUT, 0, osVirtualToPhysical(bus44->unk30) & 0xffffff);

			bus44->unk28 = 0;
		}
	}

	return ptr;
}
