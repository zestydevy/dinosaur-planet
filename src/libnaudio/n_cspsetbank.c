// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"

void n_alCSPSetBank(N_ALCSPlayer *seqp, ALBank *b) {
    N_ALEvent evt;

    evt.type = AL_SEQP_BANK_EVT;
    evt.msg.spbank.bank = b;

    n_alEvtqPostEvent(&seqp->evtq, &evt, 0, 0);
}
