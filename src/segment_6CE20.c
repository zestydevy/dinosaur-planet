#include "common.h"

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_6CE20/func_8006C220.s")
#else
/* -O1 and either -g1 or -g2*/
void func_8006C220(UnkStruct_8006C220* arg0, s32 arg1, f32 arg2, s16 arg3, u8 arg4, u8 arg5, u8 arg6, f32 arg7, u8 arg8, s32 arg9) {
    struct UnkStruct_80064514* sp1C;

    if (arg0->var8 != NULL) {
        sp1C = func_80064514();
        if (sp1C == NULL) {
            return;
        }
        
        sp1C->var4 = (s32) (n_syn->var1C + arg0->var8->var88);
        sp1C->var0 = 0;
        sp1C->var8 = 0xD;
        sp1C->varA = (s16) arg0->var1A;
        sp1C->var12 = arg4;
        sp1C->var10 = arg3;
        sp1C->var13 = arg5;
        sp1C->varC = arg2;
        sp1C->var14 = arg8;
        sp1C->var15 = arg6;
        sp1C->var18 = arg7;
        sp1C->var1C = func_800646A8(arg9);
        sp1C->var20 = arg1;
        n_alEnvmixerParam(arg0->var8, 3, sp1C);
    }
}

#endif
