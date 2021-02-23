#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80000B40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/n_alSeqpDelete.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80000D50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80000DA4.s")

void bootproc(void) {
    osInitialize();
    osCreateThread(&gIdleThread, 1, &idle, NULL, &gIdleThreadStack[IDLE_THREAD_SIZE], OS_PRIORITY_IDLE);
    osStartThread(&gIdleThread);
}

void idle(void * arg)
{
    osCreateThread(&gMainThread, 3, &mainproc, NULL, &gMainThreadStack, 0xA);

    gMainThreadStack[1024] = 0;
    gMainThreadStack[0] = 0;

    osStartThread(&gMainThread);
    osSetThreadPri(NULL, OS_PRIORITY_IDLE);

    while (TRUE) {}
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/tick_two_timers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80001178.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800011F4.s")
