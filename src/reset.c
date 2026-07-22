#include "sys/reset.h"

#include "PR/os_internal.h"
#include "sys/asset.h"
#include "sys/di_cpu.h"
#include "sys/dll.h"
#include "sys/main.h"
#include "sys/thread.h"
#include "sys/vi.h"
#include "macros.h"
#include "memory_regions.h"

typedef struct {
    /*0x0*/  DLLState *loadedDllList;
    /*0x4*/  s32 loadedDllCount;
    /*0x8*/  u8 loaded; // Whether any DLLs are actually stored here.
} ResetSavedDlls;

typedef struct {
    /*0x0*/  OSThread *threads[2];
} ResetThreadCopies;

/**
 * Holds a list of DLLs that were previously running before the last NMI reset.
 */
ResetSavedDlls *gResetDllListCopy = RESET_DLL_LIST_COPY;
/**
 * Holds copies of the main thread and asset thread respectively from before
 * the last NMI reset.
 */
ResetThreadCopies gResetThreadCopies = { { RESET_MAIN_THREAD_COPY, RESET_ASSET_THREAD_COPY } };

// -------- .bss start 800b2ed0 -------- //
u64 gResetThreadStack[STACKSIZE(0x640) + 1];
OSThread gResetThread;
OSScClient gResetScClient;
OSMesg gResetScMesgQueueBuffer[1];
OSMesgQueue gResetScMesgQueue;
OSScMsg gResetScMsg;
// -------- .bss end 800b3790 -------- //

void resetThreadMain(void *arg);
void resetStopActiveAppThreads(void);
s32 resetSaveState(void);

void resetInit(OSSched *scheduler) {
    s32 videoMode = OS_VI_PAL_LPN1;

    if (osResetType == 1 && gResetDllListCopy->loaded == TRUE) {
        videoMode = OS_VI_NTSC_LPF1;
    }

    // Why is the real initial viInit call done here??
    viInit(videoMode, scheduler, TRUE);

    osCreateThread(
        /*t*/       &gResetThread,
        /*id*/      RESET_THREAD_ID,
        /*entry*/   &resetThreadMain,
        /*arg*/     NULL,
        /*sp*/      &gResetThreadStack[STACKSIZE(0x640)],
        /*pri*/     RESET_THREAD_PRIORITY
    );

    osStartThread(&gResetThread);
}

void resetThreadMain(void *arg) {
    OSScMsg *queueMesg = NULL;
    OSSched *scheduler;
    ResetThreadCopies threads;

    // Get the OS scheduler
    scheduler = mainGetScheduler();

    // Get pointers to copies of the main thread and asset thread
    // from before the last NMI reset
    threads = gResetThreadCopies;

    // Register our client with the scheduler
    osCreateMesgQueue(
        /*mq*/      &gResetScMesgQueue,
        /*msg*/     gResetScMesgQueueBuffer,
        /*count*/   ARRAYCOUNT(gResetScMesgQueueBuffer)
    );

    osScAddClient(scheduler, &gResetScClient, &gResetScMesgQueue, OS_SC_ID_PRENMI);

    if (osResetType == 1 && gResetDllListCopy->loaded == TRUE) {
        // System was previously reset via NMI and we have a copy of state from then
        
        // Send our own queue a message indicating this
        gResetScMsg.type = 5;
        osSendMesg(&gResetScMesgQueue, &gResetScMsg, OS_MESG_NOBLOCK);
    }

    // Reset loaded flag of captured pre NMI state (lets next reset continue as normal)
    gResetDllListCopy->loaded = FALSE;

    // Await a scheduler message
    osRecvMesg(&gResetScMesgQueue, (OSMesg)&queueMesg, OS_MESG_BLOCK);

    if (queueMesg->type == OS_SC_PRE_NMI_MSG) {
        // Pre-NMI, stop all threads and enter the handler
        resetStopActiveAppThreads();
        resetSaveState();

        // Halt
        while (TRUE) { }
    }

    // Otherwise, this is after a reset and we have DLL and thread state to display (for debugging)

    // Show screen
    osViBlack(FALSE);
    osViSwapBuffer(gFrontFramebuffer);

    // Stop all threads and display crash screen with the saved DLL and thread state from before the reset
    diCpuStopActiveAppThreads();
    diCpuCrashScreenInit();
    dllReplaceLoadedDLLs(gResetDllListCopy->loadedDllList, gResetDllListCopy->loadedDllCount);
    diCpuDrawCpuInfo(threads.threads, 2, 0);

    // Halt
    while (TRUE) { }
}

void resetStopActiveAppThreads(void) {
    OSThread *thread = __osGetActiveQueue();

    while (thread->priority != -1) {
        if (thread->priority > OS_PRIORITY_IDLE &&
            thread->priority <= OS_PRIORITY_APPMAX) {
            osStopThread(thread);
        }

        thread = thread->tlnext;
    }
}

s32 resetSaveState(void) {
    OSThread *thread;
    void *dllStart;
    void *dllEnd;

    // Clone asset thread and main thread
    thread = __osGetActiveQueue();

    while (thread->priority != -1) {
        if (thread->id == ASSET_THREAD_ID) {
            bcopy(thread, RESET_ASSET_THREAD_COPY, sizeof(OSThread));
        } else if (thread->id == MAIN_THREAD_ID) {
            bcopy(thread, RESET_MAIN_THREAD_COPY, sizeof(OSThread));
        }

        thread = thread->tlnext;
    }

    // Get current list of loaded DLLs
    gResetDllListCopy->loadedDllList = dllGetLoadedDLLs(&gResetDllListCopy->loadedDllCount);
    gResetDllListCopy->loaded = TRUE;

    // Return ID of the DLL that the main thread was executing (if any)
    return dllFindExecutingDLL(RESET_MAIN_THREAD_COPY->context.pc, &dllStart, &dllEnd);
}
