// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include <PR/os.h>
#include "libnaudio/n_synthInternals.h"

Acmd *n_alResamplePull(N_PVoice *e, s16 *outp, Acmd *p) {
    Acmd *ptr;
    s16 inp;
    s32 var1;
    s32 var2;
    f32 fvar1;

    ptr = p;
    inp = N_AL_DECODER_OUT;

    if (e->rs_upitch != 0) {
        ptr = n_alAdpcmPull(e, &inp, FIXED_SAMPLE, p);
        aDMEMMove(ptr++, inp, *outp, FIXED_SAMPLE << 1);
    } else {
        if (e->rs_ratio > MAX_RATIO) {
            e->rs_ratio = MAX_RATIO;
        }

        e->rs_ratio = (s32)(e->rs_ratio * UNITY_PITCH);
        e->rs_ratio = e->rs_ratio / UNITY_PITCH;

        fvar1 = e->rs_delta + (e->rs_ratio * (f32)FIXED_SAMPLE);
        var1 = (s32)fvar1;
        e->rs_delta = fvar1 - (f32)var1;

        ptr = n_alAdpcmPull(e, &inp, var1, p);

        var2 = (s32)(e->rs_ratio * UNITY_PITCH);

        n_aResample(ptr++, osVirtualToPhysical(e->rs_state), e->rs_first, var2, inp, 0);

        e->rs_first = 0;
    }

    return ptr;
}

s32 n_alResampleParam(N_PVoice *filter, s32 paramID, void *param) {
    n_alLoadParam(filter, paramID, param);
    return 0;
}
