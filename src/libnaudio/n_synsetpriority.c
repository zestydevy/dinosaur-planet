// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_libaudio.h"

void n_alSynSetPriority(N_ALVoice *voice, s16 priority) {
    voice->priority = priority;
}
