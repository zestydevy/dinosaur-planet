#include "common.h"
#include <PR/os_internal.h>

void start_crash_thread(OSSched *scheduler) {
    s32 videoMode = OS_VI_PAL_LPN1;

    if (osResetType == 1 && gCrashDllListCopy->loaded == TRUE) {
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

void crash_thread_entry(void *arg) {
    OSScMsg *queueMesg = NULL;
    OSSched *scheduler;
    CrashThreadCopies threads;

    // Get the OS scheduler
    scheduler = get_ossched();

    // Get pointers to copies of the main thread and asset thread
    // from before the last NMI reset
    threads = gCrashThreadCopies;

    // Register our client with the scheduler
    osCreateMesgQueue(
        /*mq*/      &gCrashMesgQueue,
        /*msg*/     gCrashMesgQueueBuffer,
        /*count*/   CRASH_MESG_QUEUE_BUFFER_LENGTH
    );

    osScAddClient(scheduler, &gCrashScClient, &gCrashMesgQueue, 3);

    if (osResetType == 1 && gCrashDllListCopy->loaded == TRUE) {
        // System was previously reset via NMI and we have a copy of state from then
        
        // Send our own queue a message indicating this
        gCrashScMsg.type = 5;
        osSendMesg(&gCrashMesgQueue, &gCrashScMsg, OS_MESG_NOBLOCK);
    }

    // Reset loaded flag of captured pre NMI state (lets next reset continue as normal)
    gCrashDllListCopy->loaded = FALSE;

    // Await a scheduler message
    osRecvMesg(&gCrashMesgQueue, (OSMesg)&queueMesg, OS_MESG_BLOCK);

    if (queueMesg->type == OS_SC_PRE_NMI_MSG) {
        // Pre-NMI, stop all threads and enter the handler
        stop_active_app_threads_2();
        crash_nmi_handler();

        // Halt
        while (TRUE) { }
    }

    // Show screen
    osViBlack(FALSE);
    osViSwapBuffer(gFramebufferCurrent);

    // Stop all threads and do video crash handling stuff
    stop_active_app_threads();
    check_video_mode_crash_and_clear_framebuffer();

    // Display state from before the last NMI reset
    replace_loaded_dll_list(gCrashDllListCopy->loadedDllList, gCrashDllListCopy->loadedDllCount);
    some_crash_print(threads.threads, 2, 0);

    // Halt
    while (TRUE) { }
}

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

u32 crash_nmi_handler() {
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
    gCrashDllListCopy->loadedDllList = get_loaded_dlls(&gCrashDllListCopy->loadedDllCount);
    gCrashDllListCopy->loaded = TRUE;

    // Return ID of the DLL that the main thread was executing (if any)
    return find_executing_dll(CRASH_MAIN_THREAD_COPY->context.pc, &dllStart, &dllEnd);
}
