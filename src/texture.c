#include "common.h"

// This function is
// is not adding incrementing arg0 like it should

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003CC50.s")
#else
void _func_8003CC50(Gfx **arg0, u32 arg1, s32 arg2) {
    gSPSegment(*arg0++, arg1, &((u8 *)0x80000000)[arg2]);

    // lower level gSPSegment, in case
	// gDma1p(*arg0++, 0xDB, &((u8 *)0x80000000)[arg2], arg1 * 4, 0x06);
}
#endif
