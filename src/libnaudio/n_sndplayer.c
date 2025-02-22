#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/n_alSndpNew.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/_n_sndpVoiceHandler.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/_n_handleEvent.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/func_80065D7C.s")
#else
void func_80065D7C(UnkStruct_80065D7C* arg0) {
    if (arg0->var44 & 4) {
        n_alSynStopVoice((u32)arg0 + 0xC);
        n_alSynFreeVoice((u32)arg0 + 0xC);
    }
    func_80066294(arg0);
    func_80065E6C(AlSndPlayer_pointer + 0x14, arg0, 0xFFFF);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/func_80065DEC.s")
#else
void func_80065DEC(UnkStruct_80065DEC* arg0) {
    s32 unused2C; /*to get stack frame to match*/
    u32 sp28;
    UnkStruct_80065DEC* sp24;
    s16 unused22; /*to get stack frame to match*/
    s16 sp20;
    f32 sp1C;
    
    
    sp1C = (alCents2Ratio(arg0->var8->var4->var5) * arg0->var2C);
    sp20 = 0x10;
    sp24 = arg0;
    sp28 = *(u32*)(&sp1C); /*this is crazy*/
    alEvtPostEvent(AlSndPlayer_pointer + 0x14, &sp20, 0x8235, 0);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/_removeEvents.s")
#else
void _removeEvents(ALLink* arg0, s32 arg1, u16 arg2) {
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

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/sndpCountStates.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/func_80066064.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/sndpFreeState.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/sndSetPriority.s")
#else
/* needs -g3*/
void sndSetPriority(UnkStruct_800663B8* arg0, u8 arg1) {
    if (arg0 != NULL) {
        //arg0->var40 = (s8) (arg1 & 0xFF);
        arg0->var40 = ((arg1 << 0x10) >> 0x10);
    }
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/func_800663D8.s")
#else
void func_800663D8(void) {

}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/sndGetState.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/func_80066408.s")
#else
void func_80066408(void) {
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/some_sound_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/audioStop.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/func_80066794.s")
#else
extern s32 AlSndPlayer_pointer;
extern struct UnkStruct_D_800938C0* D_800938C0;

void func_80066794(u8 arg0) {

    u32 sp2C;
    s32 unused28;
    s32 unused24;
    struct UnkStruct_D_800938C0* sp20;
    s8 unused1E;
    s16 sp1C;
    struct UnkStruct_D_800938C0* sp18;
    
    
    sp2C = osSetIntMask(1U);
    sp18 = D_800938C0;
    if (sp18 != NULL) {
        do {
            sp1C = 0x400;
            sp20 = sp18;
            if ((sp18->var44 & arg0) == arg0) {
                sp20->var44 = (u8) (sp20->var44 & ~0x10);
                alEvtPostEvent(AlSndPlayer_pointer + 0x14, &sp1C, 0, 0);
            }
            sp18 = sp18->var0;
        } while (sp18 != NULL);
    }
    osSetIntMask(sp2C);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/func_80066854.s")
#else
void func_80066854(void) {
    func_80066794(1);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/func_8006687C.s")
#else
void func_8006687C(void) {
    func_80066794(0x11);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/func_800668A4.s")
#else
void func_800668A4(void) {
    func_80066794(3);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/audioPostEvent.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/func_8006694C.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/func_80066968.s")
#else
void func_80066968(void) {
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/func_80066970.s")
#else
void func_80066970(void) {
}
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/libnaudio/n_sndplayer/func_80066978.s")
