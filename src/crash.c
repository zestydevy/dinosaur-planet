#include "common.h"
#include <PR/os_internal.h>

// Length of gCrashMesgQueueBuffer
#define CRASH_MESG_QUEUE_BUFFER_LENGTH 1

#define CRASH_ASSET_THREAD_COPY ((OSThread *)0x807FF000)
#define CRASH_MAIN_THREAD_COPY ((OSThread *)0x807FF230)

typedef struct _UnkCrashStruct {
    /*0x0*/  DLLInst* loadedDllList;
    /*0x4*/  s32 loadedDllCount;
    /*0x8*/  u8 unk0x8;
} UnkCrashStruct;

typedef struct {
    /*0x0*/  OSThread *threads[2];
} UnkCrashStruct2;

UnkCrashStruct *D_80091770 = (UnkCrashStruct *)0x807FF460;
UnkCrashStruct2 D_80091774 = { { CRASH_MAIN_THREAD_COPY, CRASH_ASSET_THREAD_COPY } };

// Note: Unsure of actual stack size
extern u8 gCrashThreadStack[OS_MIN_STACKSIZE];
extern OSThread gCrashThread;

extern OSScClient D_800B3748;

extern OSMesg gCrashMesgQueueBuffer[1];
extern OSMesgQueue gCrashMesgQueue;

extern s16 D_800B3770;

void crash_thread_entry(void *arg);
void stop_active_app_threads_2();
u32 func_80037678();

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
        /*pri*/     CRASH_THREAD_PRIORITY
    );

    osStartThread(&gCrashThread);
}

void crash_thread_entry(void *_) {
    OSScMsg *queueMesg = NULL;
    OSSched *scheduler;
    UnkCrashStruct2 threads;

    scheduler = get_ossched();

    threads = D_80091774;

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

    if (queueMesg->type == OS_SC_PRE_NMI_MSG) {
        stop_active_app_threads_2();
        func_80037678();

        // Halt
        while (TRUE) { }
    }

    osViBlack(FALSE);
    osViSwapBuffer(gFramebufferCurrent);

    stop_active_app_threads();
    check_video_mode_crash_and_clear_framebuffer();

    replace_loaded_dll_list(D_80091770->loadedDllList, D_80091770->loadedDllCount);
    some_crash_print(threads.threads, 2, 0);

    // Halt
    while (TRUE) { }
}

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

u32 func_80037678() {
    OSThread *thread;
    u32 *dllStart;
    u32 *dllEnd;

    // Clone asset thread and main thread
    thread = __osGetActiveQueue();

    while (thread->priority != -1) {
        if (thread->id == ASSET_THREAD_ID) {
            _bcopy(thread, CRASH_ASSET_THREAD_COPY, sizeof(OSThread));
        } else if (thread->id == MAIN_THREAD_ID) {
            _bcopy(thread, CRASH_MAIN_THREAD_COPY, sizeof(OSThread));
        }

        thread = thread->tlnext;
    }

    // Get current list of loaded DLLs
    D_80091770->loadedDllList = get_loaded_dlls(&D_80091770->loadedDllCount);
    D_80091770->unk0x8 = 1;

    // Return ID of the DLL that the main thread was executing (if any)
    return find_executing_dll(CRASH_MAIN_THREAD_COPY->context.pc, &dllStart, &dllEnd);
}
