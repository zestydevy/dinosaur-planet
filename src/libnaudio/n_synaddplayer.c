// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include <PR/os.h>
#include "libnaudio/n_synthInternals.h"

void n_alSynAddPlayer(ALPlayer *client) {
	OSIntMask mask = osSetIntMask(OS_IM_NONE);

	client->samplesLeft = n_syn->curSamples;

	client->next = n_syn->head;
	n_syn->head = client;

	osSetIntMask(mask);
}

void n_alSynAddSndPlayer(ALPlayer *client) {
	OSIntMask mask = osSetIntMask(OS_IM_NONE);

	client->samplesLeft = n_syn->curSamples;

	client->next = n_syn->head;
	n_syn->head = client;

	osSetIntMask(mask);
}

void n_alSynAddSeqPlayer(ALPlayer *client) {
	OSIntMask mask = osSetIntMask(OS_IM_NONE);

	client->samplesLeft = n_syn->curSamples;

	client->next = n_syn->head;
	n_syn->head = client;

	osSetIntMask(mask);
}
