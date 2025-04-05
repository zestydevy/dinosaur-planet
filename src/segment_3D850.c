#include "common.h"

// TODO: turn this into gSPSegment (is the gbi.h old?)
// https://github.com/DavidSM64/Diddy-Kong-Racing/blob/master/src/set_rsp_segment.c ?
void dl_segment(Gfx **gdl, u32 segment, void *base) {
	gSPSegment((*gdl)++, segment, OS_K0_TO_PHYSICAL(base));
}
