// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"

ALPan n_alCSPGetChlPan(N_ALCSPlayer *seqp, u8 chan) {
    return seqp->chanState[chan].pan;
}
