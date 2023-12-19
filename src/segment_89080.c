#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/func_80088480.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/func_8008852C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/__rmonSetBreak.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/func_800887D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/__rmonClearBreak.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/__rmonGetBranchTarget.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/func_80088B64.s")
#else
    /* uses O1 */
    int func_80088B64(u32 arg0) {
    
    switch ((arg0 >> 0x1A) & 0x3F) {                              /* irregular */
        case 0:
            if (!((arg0 >> 5) & 0x7FFF) && ((arg0 & 0x3F) == 8)) {
                return 1;
            }
            if (!((arg0 >> 0x10) & 0x1F) && ((arg0 & 0x7FF) == 9)) {
                return 1;
            }
        break;
        case 2:
        case 3:
            return 1;
        }
        
    return 0;
    }
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/__rmonSetSingleStep.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/__rmonGetExceptionStatus.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/func_80088CFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/__rmonHitBreak.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/__rmonHitSpBreak.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/func_80088E50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/func_80088E80.s")
