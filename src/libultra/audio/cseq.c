// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libultra/audio/synthInternals.h"

/*
  Note: If there are no valid tracks (ie. all tracks have
  reached the end of their data stream), then return FALSE
  to indicate that there is no next event.
*/
char __alCSeqNextDelta(ALCSeq *seq, s32 *pDeltaTicks) {
    u32 i;
    u32 firstTime;
    u32 lastTicks;

    firstTime = 0xFFFFFFFF;
    lastTicks = seq->lastDeltaTicks;

    if (seq->validTracks == 0) {
        return FALSE;
    }

    for (i = 0; i < 16; i++) {
        if ((seq->validTracks >> i) & 1) {
            if (seq->deltaFlag != 0) {
                seq->evtDeltaTicks[i] -= lastTicks;
            }

            if (seq->evtDeltaTicks[i] < firstTime) {
                firstTime = seq->evtDeltaTicks[i];
            }
        }
    }

    seq->deltaFlag = 0;
    *pDeltaTicks = firstTime;

    return TRUE;
}
