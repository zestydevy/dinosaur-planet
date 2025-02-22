// @DECOMP_OPT_FLAGS=-O1
#include "common.h"

/* Problem with this file is that it seems some funcitons need -g1 or -g2, but it adds extra jr ra nop assembly code to some functions.
Could probably split this file into multiple files, no idea how that would work.
Might just be the case that -g1/-g2 shouldn't be used, I dont know.*/

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_synthesizer/n_alSynNew.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_synthesizer/n_alAudioFrame.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_synthesizer/__n_allocParam.s")
#else

/* This funciton needs either -g1 or -g2, but it messes up the rest of the file for some reason. 
    I think it adds extra nop an jra assembly code
    Otherwise, works fine*/

extern struct UnkStruct_n_syn* n_syn;
struct UnkStruct_80064514* __n_allocParam(void) {
        struct UnkStruct_80064514* sp4;

        sp4 = NULL;
        if (n_syn->var2C != NULL) {
            sp4 = n_syn->var2C;
            n_syn->var2C = (struct UnkStruct_80064514* ) n_syn->var2C->var0; /*This seems kinda scuffed*/
            sp4->var0 = NULL;
        }
        return sp4;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_synthesizer/_n_freeParam.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_synthesizer/func_80064598.s")
#else
/* Funtions like this DONT match with -g1/-g2 flags*/
void func_80064598(void) {
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_synthesizer/_n_collectPVoices.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_synthesizer/_n_freePVoice.s")
#else
void _n_freePVoice(ALLink* arg0) {
    alUnlink(arg0);
    alLink(arg0, (u32)n_syn + 0x14);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_synthesizer/_n_timeToSamplesNoRound.s")
#else
extern f32 D_8009B820;
extern struct UnkStruct_n_syn* n_syn;

s32 _n_timeToSamplesNoRound(s32 arg0) {
    f32 sp4;
    sp4 = (((f32) arg0 * (f32) n_syn->var40) / D_8009B820) + 0.5f;
    return (s32) sp4;
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_synthesizer/_n_timeToSamples.s")
#else
/*needs ido 6.0*/
s32 _n_timeToSamples(s32 arg0) {
    return _n_timeToSamplesNoRound(arg0) & ~0xF;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_synthesizer/__n_nextSampleTime.s")
