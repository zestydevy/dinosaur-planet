#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/n_alSynNew.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_800642F4.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80064514.s")
#else

/* This funciton needs either -g1 or -g2, but it messes up the rest of the file for some reason. 
    I think it adds extra nop an jra assembly code
    Otherwise, works fine*/

extern struct UnkStruct_n_syn* n_syn;
struct UnkStruct_80064514* func_80064514(void) {
        struct UnkStruct_80064514* sp4;

        sp4 = NULL;
        if (n_syn->unk2C != NULL) {
            sp4 = n_syn->unk2C;
            n_syn->unk2C = (struct UnkStruct_80064514* ) n_syn->unk2C->unk0; /*This seems kinda scuffed*/
            sp4->unk0 = NULL;
        }
        return sp4;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80064574.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80064598.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_800645A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80064604.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80064644.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_800646A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_800646E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/alSndpNew.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/_sndpVoiceHandler.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/soundstate_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80065D7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80065DEC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80065E6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80065F60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80066064.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80066294.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_800663B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_800663D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_800663E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80066408.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/some_sound_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/stop_sound_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80066794.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80066854.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_8006687C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_800668A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/modify_sound_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_8006694C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80066968.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80066970.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80066978.s")
