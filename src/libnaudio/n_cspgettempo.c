// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"

s32 n_alCSPGetTempo(N_ALCSPlayer *seqp)  {
    /* sct 1/8/96 - If no target sequence has been set, just return zero (instead of crashing). */
    if (seqp->target == NULL)
	return 0;

    return seqp->uspt/seqp->target->qnpt;
}
