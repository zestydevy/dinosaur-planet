#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/func_80088480.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/func_8008852C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/__rmonSetBreak.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/func_800887D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/__rmonClearBreak.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/__rmonGetBranchTarget.s")
#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/func_80088B64.s")
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

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/__rmonSetSingleStep.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/__rmonGetExceptionStatus.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/func_80088CFC.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/__rmonHitBreak.s")
#else
    void __rmonHitBreak(void) {
    func_8008852C();
    __rmonStopUserThreads(0);
    func_80088E80();
}
#endif

/* NEED __rmonRcpAtBreak */
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/__rmonHitSpBreak.s")
#else
void __rmonHitSpBreak(void) {
    /* Only thing that's wrong is the stack frame size and __rmonRcpAtBreak*/
    s32 unused[18]; /* This is to get the stack frame size to match*/
    
    s32 sp24;
    
    __rmonWriteWordTo(0x04080000, __rmonReadWordAt(0x04080000) - 4);
    __rmonGetThreadStatus(1, 0x3E8, &sp24);
    __rmonGetExceptionStatus(&sp24);
    __rmonSendReply(&sp24, 0x4C, 2);
    __rmonRcpAtBreak = 1;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/func_80088E50.s")
#else
    void func_80088E50(void) {
        __rmonMaskIdleThreadInts();
        __rmonStopUserThreads(0);
        func_80088E80();
    }
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/func_80088E80.s")