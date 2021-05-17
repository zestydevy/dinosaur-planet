#include <PR/os_internal.h>
#include "common.h"
#include "crash.h"
#include "exception.h"
#include "video.h"

// Length of gCrashMesgQueueBuffer
#define CRASH_MESG_QUEUE_BUFFER_LENGTH 1

typedef struct _UnkCrashStruct {
    /*0x0*/  DLLInst* loadedDllList;
    /*0x4*/  s32 loadedDllCount;
    /*0x8*/  u8 unk0x8;
} UnkCrashStruct;

extern UnkCrashStruct *D_80091770;
extern OSThread *D_80091774[2];

// Note: Unsure of actual stack size
extern u8 gCrashThreadStack[OS_MIN_STACKSIZE];
extern OSThread gCrashThread;

extern int D_800B3748;

extern OSMesg gCrashMesgQueueBuffer[1];
extern OSMesgQueue gCrashMesgQueue;

extern s16 D_800B3770;

OSSched *get_ossched(void);

void crash_thread_entry(void *arg);
void stop_active_app_threads_2();
void func_80037678();

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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/crash/crash_thread_entry.s")
#else
// Functionally equivalent, mainly regalloc
void crash_thread_entry(void *_) {
    s16 *queueMesg;
    OSSched *scheduler;
    OSThread *stack0x20[2];

    scheduler = get_ossched();
    queueMesg = NULL;

    stack0x20[0] = D_80091774[0];
    stack0x20[1] = D_80091774[1];

    osCreateMesgQueue(
        &gCrashMesgQueue, 
        &gCrashMesgQueueBuffer[0], 
        CRASH_MESG_QUEUE_BUFFER_LENGTH
    );

    osScAddClient(scheduler, &D_800B3748, &gCrashMesgQueue, 3);

    if (osResetType == 1 && D_80091770->unk0x8 == 1) {
        D_800B3770 = 5;

        osSendMesg(&gCrashMesgQueue, &D_800B3770, OS_MESG_NOBLOCK);
    }

    D_80091770->unk0x8 = 0;

    osRecvMesg(&gCrashMesgQueue, (OSMesg)&queueMesg, OS_MESG_BLOCK);

    if (*queueMesg == 4) {
        stop_active_app_threads_2();
        func_80037678();

        // Halt
        while (TRUE) { }
    }

    osViBlack(FALSE);
    osViSwapBuffer(gFramebufferCurrent);

    stop_active_app_threads();
    check_video_mode_crash_and_clear_framebuffer();

    some_crash_setter(D_80091770->loadedDllList, D_80091770->loadedDllCount);
    some_crash_print(&stack0x20[0], 2, 0);

    // Halt
    while (TRUE) { }
}
#endif

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
