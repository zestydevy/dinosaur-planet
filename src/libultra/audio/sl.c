// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include <PR/libaudio.h>

void alLink(ALLink *ln, ALLink *to) {
	ln->next = to->next;
	ln->prev = to;

	if (to->next) {
		to->next->prev = ln;
	}

	to->next = ln;
}

void alUnlink(ALLink *ln) {
	if (ln->next) {
		ln->next->prev = ln->prev;
	}

	if (ln->prev) {
		ln->prev->next = ln->next;
	}
}
