#include "common.h"

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_6CE20/func_8006C220.s")
#else
/* -O1 and either -g1 or -g2*/
void func_8006C220(UnkStruct_8006C220* arg0, s32 arg1, f32 arg2, s16 arg3, u8 arg4, u8 arg5, u8 arg6, f32 arg7, u8 arg8, s32 arg9) {
    struct UnkStruct_80064514* sp1C;

    if (arg0->unk8 != NULL) {
        sp1C = func_80064514();
        if (sp1C == NULL) {
            return;
        }
        
        sp1C->unk4 = (s32) (n_syn->unk1C + arg0->unk8->unk88);
        sp1C->unk0 = 0;
        sp1C->unk8 = 0xD;
        sp1C->unkA = (s16) arg0->unk1A;
        sp1C->unk12 = arg4;
        sp1C->unk10 = arg3;
        sp1C->unk13 = arg5;
        sp1C->unkC = arg2;
        sp1C->unk14 = arg8;
        sp1C->unk15 = arg6;
        sp1C->unk18 = arg7;
        sp1C->unk1C = func_800646A8(arg9);
        sp1C->unk20 = arg1;
        n_alEnvmixerParam(arg0->unk8, 3, sp1C);
    }
}

#endif
