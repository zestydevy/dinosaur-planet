// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"

void n_alCSPSetVol(N_ALCSPlayer *seqp, s16 vol) {
    N_ALEvent       evt;

    evt.type            = AL_SEQP_VOL_EVT;
    evt.msg.spvol.vol   = vol;
    
    n_alEvtqPostEvent(&seqp->evtq, &evt, 0, 0);
}
