#include "PR/os.h"
#include "common.h"

extern u8 *D_80091770;
extern void *crash_thread_stack;
extern OSThread crash_thread;
void crash_func(void *arg);

void create_crash_thread(OSSched *scheduler) {
    s32 a3 = 0xE;
    if(osResetType == 1 && D_80091770[8] == 1) {
        a3 = 1;
    }
    func_8005D410(a3, scheduler, 1); //probably create thread queue
    osCreateThread(&crash_thread, 0x64, crash_func, NULL, &crash_thread_stack, 0x80);
    osStartThread(&crash_thread);
}

#pragma GLOBAL_ASM("asm/nonmatchings/crash/crash_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/crash/func_80037610.s")

#pragma GLOBAL_ASM("asm/nonmatchings/crash/func_80037678.s")
