// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"

void n_alCSPSetTempo(N_ALCSPlayer *seqp, s32 tempo) {
    N_ALEvent       evt;
    
    evt.type             = AL_SEQP_META_EVT;
    evt.msg.tempo.status = AL_MIDI_Meta;
    evt.msg.tempo.type   = AL_MIDI_META_TEMPO;
    evt.msg.tempo.byte1  = (tempo & 0xff0000)>>16;
    evt.msg.tempo.byte2  = (tempo & 0xff00)>>8;
    evt.msg.tempo.byte3  = tempo & 0xff;
    
    n_alEvtqPostEvent(&seqp->evtq, &evt, 0, 0);
}
