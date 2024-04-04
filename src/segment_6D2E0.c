#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_8006C6E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_8006C770.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_8006C840.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/n_alEnvmixerParam.s")
#else
/*O1 and either g1 or g2*/
int n_alEnvmixerParam(UnkStruct_n_alEnvmixerParam* arg0, s32 arg1, s32* arg2) {
    
    struct UnkStruct_n_alEnvmixerParam* sp24 = arg0; /*I'm guessing they were using g2 for local variable tracking*/ 
    
    switch (arg1) {                                 /* irregular */
        case 3:
            if (sp24->var80 != NULL) {
                *sp24->var80 = arg2;
            } else {
                sp24->var7C = arg2;
            }
            sp24->var80 = arg2;
            break;
        case 4:
            sp24->var78 = 1;
            sp24->var84 = 0;
            sp24->var5A = 1;
            sp24->var74 = 0;
            sp24->var4C = 0.0f;
            sp24->var50 = 1;
            sp24->var48 = 0;
            func_800777D8(sp24, 4, arg2);
            break;
        case 9:
            sp24->var84 = 1;
            break;
        default:
            func_800777D8(sp24, arg1, arg2);
            break;
    }
    return 0;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_8006D2A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_8006D58C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_8006D79C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_with_binary_strings.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_8006E0A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_8006ECDC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_8006F530.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_80070168.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_800706F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_800708B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_80070D58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_6D2E0/func_8007163C.s")
