#include <PR/os_internal.h>
#include "common.h"
#include "crash.h"

typedef struct _UnkCrashStruct {
    u8 unk0x0[8];
    u8 unk0x8;
} UnkCrashStruct;

extern UnkCrashStruct *D_80091770;

// Note: Unsure of actual stack size
extern u8 gCrashThreadStack[OS_MIN_STACKSIZE];
extern OSThread gCrashThread;

void func_8005D410(s32 videoMode, OSSched* scheduler, s32 someBool);

void crash_thread_entry(void *arg);

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/crash/start_crash_thread.s")
#else
void start_crash_thread(OSSched* scheduler) { 
    s32 videoMode = 0xe;

    if (osResetType == 1 && D_80091770->unk0x8 == 1) {
        videoMode = 1;
    }

    func_8005D410(videoMode, scheduler, TRUE);

    osCreateThread(
        /*t*/       &gCrashThread, 
        /*id*/      CRASH_THREAD_ID, 
        /*entry*/   &crash_thread_entry, 
        /*arg*/     NULL, 
        /*sp*/      &gCrashThreadStack[OS_MIN_STACKSIZE], 
        /*pri*/     0x80
    );

    osStartThread(&gCrashThread);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/crash/crash_thread_entry.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/crash/stop_active_app_threads_2.s")
#else
/**
 * Stops all active application threads (those with priorities between 1 and OS_PRIORITY_APPMAX).
 * 
 * Identical to stop_active_app_threads.
 */
void stop_active_app_threads_2() {
    OSThread *thread = __osGetActiveQueue();

    while (thread->priority != -1) {
        if (thread->priority > OS_PRIORITY_IDLE && 
            thread->priority <= OS_PRIORITY_APPMAX) {
            osStopThread(thread);
        }

        thread = thread->tlnext;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/crash/func_80037678.s")
