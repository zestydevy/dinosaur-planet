// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include <PR/ultraerror.h>
#include "libnaudio/n_synthInternals.h"

void n_alSynStopVoice(N_ALVoice *v) {
	ALParam *update;
	ALFilter *f;

	if (v->pvoice) {
		update = __n_allocParam();
		ALFailIf(update == 0, ERR_ALSYN_NO_UPDATE);

		update->delta = n_syn->paramSamples + v->pvoice->offset;
		update->type = AL_FILTER_STOP_VOICE;
		update->next = 0;

		n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);
	}
}
