// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"

void n_alCSPDelete(N_ALCSPlayer *seqp) {
    n_alSynRemovePlayer(&seqp->node);
}
