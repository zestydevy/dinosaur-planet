// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"

Acmd *n_alSavePull(s32 sampleOffset, Acmd *p) {
	Acmd *ptr = p;

	ptr = n_alMainBusPull(sampleOffset, ptr);

	n_aInterleave(ptr++);
	n_aSaveBuffer(ptr++, FIXED_SAMPLE << 2, 0, n_syn->sv_dramout);

	return ptr;
}
