// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"

void func_8006AEC4(struct fx *fx, f32 outputRate);

void n_alSynSetFXParam(ALFxRef fx, s16 paramID, void *param) {
	ALFx *f = (ALFx *)fx;

	n_alFxParamHdl(f, (s32)paramID, param);
}

void func_8007A974(struct fx *fx, s16 arg1, void *param) {
    if (arg1 == 8) {
		fx->unk02 = (*(s32 *)param * 0.1f);
	} else if (arg1 == 9) {
		fx->unk00 = *(s32 *)param;
	}

    func_8006AEC4(fx, n_syn->outputRate);
}
