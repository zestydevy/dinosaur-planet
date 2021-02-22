#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80000B40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/n_alSeqpDelete.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80000D50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80000DA4.s")

void bootproc(void) {
    osInitialize();
    osCreateThread(&gMainThread, 1, &idle, NULL, &gMainThreadStack, OS_PRIORITY_IDLE);
    osStartThread(&gMainThread);
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/idle.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80001130.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80001178.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800011F4.s")
