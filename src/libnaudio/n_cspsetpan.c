// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"

void n_alCSPSetChlPan(N_ALCSPlayer *seqp, u8 chan, ALPan pan) {
    N_ALEvent       evt;

    evt.type            = AL_SEQP_MIDI_EVT;
    evt.msg.midi.ticks  = 0;
    evt.msg.midi.status = AL_MIDI_ControlChange | chan;
    evt.msg.midi.byte1  = AL_MIDI_PAN_CTRL;
    evt.msg.midi.byte2  = pan;
                    
    n_alEvtqPostEvent(&seqp->evtq, &evt, 0, 0);
}

void func_8006721C(N_ALCSPlayer *seqp, u8 arg1, f32 arg2) {
    N_ALEvent       evt;

    evt.type            = AL_1A_EVT;
    evt.msg.evt1a.unk00 = arg1;
    evt.msg.evt1a.unk01 = arg2;
                    
    n_alEvtqPostEvent(&seqp->evtq, &evt, 0, 0);
}
