/** OS threads
 */
#ifndef _SYS_THREAD_H
#define _SYS_THREAD_H

#include "ultra64.h"
#include "PR/sched.h"

#define IDLE_THREAD_SIZE 0x40
#define IDLE_THREAD_ID 1
#define IDLE_THREAD_PRIORITY OS_PRIORITY_IDLE

#define MAIN_THREAD_SIZE 0x2000
#define MAIN_THREAD_ID 3
#define MAIN_THREAD_PRIORITY 10

#define OS_SCHEDULER_THREAD_ID 5
#define CONTROLLER_THREAD_ID 98

#define CRASH_THREAD_ID 100
#define CRASH_THREAD_PRIORITY 0x80

extern OSThread* __osRunningThread;
extern OSThread* __osRunQueue;
// this needs double checking. its address is within gMainThreadStack....
extern u64 gIdleThreadStack[];
extern u64 gMainThreadStack[];
extern OSThread gIdleThread;
extern OSThread gMainThread;

/**
 * @returns The address of s->interruptQ.
 */
OSMesgQueue *osScGetInterruptQ(OSSched *s);

#endif //_SYS_THREAD_H
