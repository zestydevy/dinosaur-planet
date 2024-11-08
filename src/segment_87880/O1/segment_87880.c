#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/segment_87880/O1/segment_87880/__rmonMaskIdleThreadInts.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_87880/O1/segment_87880/__rmonGetTCB.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_87880/O1/segment_87880/__rmonStopUserThreads.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_87880/O1/segment_87880/__rmonListThreads.s")

/* Uses g0*/
/* gets loads of warnings about pointers and integers, but it matches so it should be fine*/
/* one error is OSThread->OSThreadContext.pc is a void pointer, so we've gotta figure that out*/
#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_87880/O1/segment_87880/__rmonGetThreadStatus.s")
#else

extern u8 __rmonRcpAtBreak;

int __rmonGetThreadStatus(s32 arg0, s32 arg1, UnkStruct_rmonGetThreadStatus_arg2* arg2) {

    s32 sp1C;

    struct OSThread* sp18;

    arg2->var14 = arg1;
    if (arg0 == 1) {
        arg2->var18 = 0x3E9;
    } else {
        arg2->var18 = 0x3EA;
    }
    arg2->var10 = 1;
    arg2->var12 = 0;
    arg2->var24 = 0;
    arg2->var26 = 0;
    arg2->var30 = 0;
    
    if (arg0 == 1) {
        arg2->var2C = 0x04001000; /*was arg2->unk2C = (void* )0x04001000;*/
        arg2->var28 = 0x2A;
        if (__rmonRCPrunning() != 0) {
            arg2->varC = 4;
            arg2->var20 = NULL;
            arg2->var1C = 0;
        } else {
            arg2->varC = 1;
            arg2->var20 = (s32* ) (__rmonReadWordAt((s32* )0x04080000) + 0x04001000);
            sp1C = __rmonReadWordAt(arg2->var20);
            if ((sp1C & 0xFC00003F) == 0xD) {
                sp1C = 0xD;
            }
            if (__rmonRcpAtBreak != 0) {
                arg2->var10 = 2;
                arg2->var24 = 2;
                arg2->var26 = 4;
            }
            arg2->var1C = sp1C;
        }

    }else{
            
        sp18 = __osGetActiveQueue();
    
    
        while (sp18->priority != -1) {
            if (sp18->id == arg1) {
                break;  
            }
            sp18 = sp18->tlnext;
        }
        
        if (sp18->priority == -1) {
            return -2;
        }
        arg2->var28 = (s32) sp18->priority;
         /*was temp_t0 = sp18->unk10;*/
        
        if (sp18->state != 0) {
            arg2->varC = (s32) sp18->state;
        } else {
            arg2->varC = 1;
        }
        arg2->var20 = (s32* ) sp18->context.pc;
        /*was temp_t9 = *sp18->unk11C;*/
        sp1C = *sp18->context.pc;
        if ((sp1C & 0xFC00003F) == 0xD) {
            sp1C = 0xD;
        }
        
        arg2->var1C = sp1C;
        arg2->var2C = sp18;
        if (sp18->flags & 1) {
            arg2->var10 = 2;
            arg2->var24 = 2;
            arg2->var26 = 4;
        } else if (sp18->flags & 2) {
            arg2->var10 = 2;
            arg2->var24 = 1;
            arg2->var26 = 2;
        }
    }

    return 0;
}



#endif

#pragma GLOBAL_ASM("asm/nonmatchings/segment_87880/O1/segment_87880/__rmonThreadStatus.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_87880/O1/segment_87880/__rmonStopThread.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_87880/O1/segment_87880/__rmonRunThread.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_87880/O1/segment_87880/__rmonSetFault.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_87880/O1/segment_87880/__rmonInit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_87880/O1/segment_87880/func_80087818.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_87880/O1/segment_87880/__rmonSetComm.s")
