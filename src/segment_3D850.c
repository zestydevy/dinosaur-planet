#include "common.h"

// TODO: turn this into gSPSegment (is the gbi.h old?)

void func_8003CC50(Gfx **arg0, u32 arg1, s32 arg2) {
	gDma1p((*arg0)++, 0xDB, 0x80000000 + arg2, arg1 * 4, 0x06);
}
