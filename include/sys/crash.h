/** Crash handler system
 */
#ifndef _SYS_CRASH_H
#define _SYS_CRASH_H

#include "PR/ultratypes.h"
#include "PR/os.h"
#include "PR/sched.h"
#include "sys/dll.h"

// used for crash screens
typedef struct ErrString {
	u32 code1;
	u32 code2;
	char *text;
} ErrString;

// Length of gCrashMesgQueueBuffer
#define CRASH_MESG_QUEUE_BUFFER_LENGTH 1

#define CRASH_ASSET_THREAD_COPY ((OSThread *)0x807FF000)
#define CRASH_MAIN_THREAD_COPY ((OSThread *)0x807FF230)
#define CRASH_DLL_LIST_COPY ((CrashedDlls *)0x807FF460)

typedef struct {
    /*0x0*/  DLLState *loadedDllList;
    /*0x4*/  s32 loadedDllCount;
    /*0x8*/  u8 loaded; // Whether any DLLs are actually stored here.
} CrashedDlls;

typedef struct {
    /*0x0*/  OSThread *threads[2];
} CrashThreadCopies;

// Note: Unsure of actual stack size
extern u8 gCrashThreadStack[OS_MIN_STACKSIZE];
extern OSThread gCrashThread;

extern OSScClient gCrashScClient;

extern OSMesg gCrashMesgQueueBuffer[CRASH_MESG_QUEUE_BUFFER_LENGTH];
extern OSMesgQueue gCrashMesgQueue;

extern OSScMsg gCrashScMsg;

void start_crash_thread(OSSched *scheduler);

void crash_thread_entry(void *arg);

/**
 * Stops all active application threads (those with priorities between 1 and OS_PRIORITY_APPMAX).
 *
 * Identical to stop_active_app_threads.
 */
void stop_active_app_threads_2();

/**
 * Runs from the crash thread when pre-NMI occurs.
 * 
 * - Copies the main thread and asset thread.
 * - Copies the list of currently loaded DLLs.
 * - Returns the ID of the DLL currently being executed by the main thread,
 *   or -1 if the main thread is not executing a DLL.
 */
u32 crash_nmi_handler();

#endif //_SYS_CRASH_H
