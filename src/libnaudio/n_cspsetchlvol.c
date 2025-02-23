// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"

void n_alCSPSetChlVol(N_ALCSPlayer *seqp, u8 chan, u8 vol){
    N_ALEvent       evt;

    evt.type            = AL_SEQP_MIDI_EVT;
    evt.msg.midi.ticks  = 0;
    evt.msg.midi.status = AL_MIDI_ControlChange | chan;
    evt.msg.midi.byte1  = AL_MIDI_VOLUME_CTRL;
    evt.msg.midi.byte2  = vol;
                    
    n_alEvtqPostEvent(&seqp->evtq, &evt, 0, 0);
}
