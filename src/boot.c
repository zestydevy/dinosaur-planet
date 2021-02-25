#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/boot/func_80000B40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/boot/n_alSeqpDelete.s")

#pragma GLOBAL_ASM("asm/nonmatchings/boot/func_80000D50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/boot/func_80000DA4.s")

void bootproc(void) {
    osInitialize();
    osCreateThread(&gIdleThread, 1, &idle, NULL, &gIdleThreadStack[IDLE_THREAD_SIZE], OS_PRIORITY_IDLE);
    osStartThread(&gIdleThread);
}

void idle(void * arg)
{
    osCreateThread(&gMainThread, 3, &mainproc, NULL, &gMainThreadStack[MAIN_THREAD_SIZE], 0xA);

    gMainThreadStack[1024] = 0;
    gMainThreadStack[0] = 0;

    osStartThread(&gMainThread);
    osSetThreadPri(NULL, OS_PRIORITY_IDLE);

    while (TRUE) {}
}

void threadTimerTick(void) {
    ++gMainThreadStack[1024];
    ++gMainThreadStack[0];
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/boot/func_80001178.s")
#else
// regalloc, swap $a2 and $v1
void func_80001178(s32 a0, s32 *a1)
{
    u8  *ptr1;
    s32 *ptr2;
    s32  count;

    count = 0;

    ptr2 = &a1[2];
    ptr1 =  a1[2];

    while (ptr1) {
        while ((u32)ptr1 < ptr2[1])
            count += *ptr1++;

        ptr1 = ptr2[2];
        ptr2 += 2;
    }

    if (a1[0] == 1) {
        a1[0] = 2;
        a1[1] = count;
    } else if (a1[0] == 2) {
        // matches with any local var, not just a1
        if (a1)
            ;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/boot/func_800011F4.s")
