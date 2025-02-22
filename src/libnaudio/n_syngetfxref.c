// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"

ALFxRef n_alSynGetFXRef(s16 bus) {
    N_ALMainBus *m = n_syn->mainBus;

    if (m->filter.handler == (N_ALCmdHandler)n_alFxPull) {
        return n_syn->auxBus[bus].fx;
    } else {
        return NULL;
    }
}

struct auxbus44 *func_8007A8B8(s16 bus) {
    N_ALMainBus *m = n_syn->mainBus;

    if (m->filter.handler == (N_ALCmdHandler)n_alFxPull) {
        return n_syn->auxBus[bus].unk44;
    } else {
        return NULL;
    }
}