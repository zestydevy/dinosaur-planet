#include "common.h"

// TODO: turn this into gSPSegment (is the gbi.h old?)

void dl_segment(Gfx **gdl, u32 segment, void *base) {
	gSPSegment((*gdl)++, segment, OS_K0_TO_PHYSICAL(base));
}
