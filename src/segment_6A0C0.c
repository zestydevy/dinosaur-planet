#include "common.h"

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_6A0C0/clear_al_syn_head.s")
#else
extern s32* n_syn;

void clear_al_syn_head(void) {
    *n_syn = NULL;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_6A0C0/noop_800694d4.s")
#else
void noop_800694d4(void) {
    return;
}
#endif