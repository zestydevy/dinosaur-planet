#include "common.h"
/* needs O1 and either -g1 or -g2*/
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_6A0E0/O1/segment_6A0E0/func_800694E0.s")
#else
extern struct UnkStruct_n_syn* n_syn;

s32 func_800694E0(s16 arg0, s32 arg1, s32 arg2) {
    func_8006B0B0(((u32)n_syn->var34) + (arg0 * 0x48) + 0x24, arg1, arg0, arg2);
    return *(u32*)(((u32)n_syn->var34) + (arg0 * 0x48) + 0x24); /*seems scuffed*/
}
#endif