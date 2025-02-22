// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include <PR/ultraerror.h>
#include "libnaudio/n_synthInternals.h"

void n_alSynFilter0x12(N_ALVoice *v, u8 arg1) {
	ALParam *update;

	if (v->pvoice) {
		update = (ALParam *)__n_allocParam();
		ALFailIf(update == 0, ERR_ALSYN_NO_UPDATE);

		update->delta  = n_syn->paramSamples + v->pvoice->offset;
		update->type   = AL_FILTER_0x12;
		update->data.i = arg1;
		update->next   = 0;

		n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);
	}
}

