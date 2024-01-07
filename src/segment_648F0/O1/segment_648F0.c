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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80065D7C.s")
#else
void func_80065D7C(UnkStruct_80065D7C* arg0) {
    if (arg0->var44 & 4) {
        func_8006C560((u32)arg0 + 0xC);
        func_8006C600((u32)arg0 + 0xC);
    }
    func_80066294(arg0);
    func_80065E6C(AlSndPlayer_pointer + 0x14, arg0, 0xFFFF);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80065DEC.s")
#else
void func_80065DEC(UnkStruct_80065DEC* arg0) {
    s32 unused2C; /*to get stack frame to match*/
    u32 sp28;
    UnkStruct_80065DEC* sp24;
    s16 unused22; /*to get stack frame to match*/
    s16 sp20;
    f32 sp1C;
    
    
    sp1C = (func_8006C6E0(arg0->var8->var4->var5) * arg0->var2C);
    sp20 = 0x10;
    sp24 = arg0;
    sp28 = *(u32*)(&sp1C); /*this is crazy*/
    alEvtPostEvent(AlSndPlayer_pointer + 0x14, &sp20, 0x8235, 0);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_648F0/O1/segment_648F0/func_80065E6C.s")
#else
void func_80065E6C(ALLink* arg0, s32 arg1, u16 arg2) {
    ALLink* sp2C;
    ALLink* sp28;
    ALLink* sp24;
    ALLink* sp20;
    struct UnkStruct_80065E6C* sp1C;
    u32 sp18;

    sp18 = osSetIntMask(1U);
    sp2C = arg0->var8;
    if (sp2C != NULL) {
        do {
            sp28 = sp2C->next;
            sp24 = sp2C;
            sp20 = sp28;
            sp1C = ((u32)sp24 + 0xC);
            if ((sp1C->var4 == arg1) && (sp1C->var0 & arg2)) {
                if (sp20 != NULL) {
                    sp20->var8 = (sp20->var8 + sp24->var8);
                }
                alUnlink(sp2C);
                alLink(sp2C, arg0);
            }
            sp2C = sp28;
        } while (sp2C != NULL);
    }
    osSetIntMask(sp18);
}
#endif

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
