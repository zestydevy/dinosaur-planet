// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"

ALFxRef n_alSynAllocFX(s16 bus, ALSynConfig *c, ALHeap *hp) {
	n_alFxNew(n_syn->auxBus[bus].fx_array, c, bus, hp);
	return n_syn->auxBus[bus].fx_array[0];
}
