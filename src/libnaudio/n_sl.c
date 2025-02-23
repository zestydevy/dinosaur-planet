// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"

N_ALGlobals *n_alGlobals = NULL;
N_ALSynth *n_syn = NULL;

void n_alInit(N_ALGlobals *g, ALSynConfig *c) {
	if (!n_alGlobals) {
		n_alGlobals = g;

		if (!n_syn) {
			n_syn = &n_alGlobals->drvr;
			n_alSynNew(c);
		}
	}
}

void n_alClose(N_ALGlobals *g) {
	if (n_alGlobals) {
		n_alSynDelete();
		n_alGlobals = NULL;
		n_syn = NULL;
	}
}
