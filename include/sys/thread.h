/** OS threads
 */
#ifndef _SYS_THREAD_H
#define _SYS_THREAD_H

#include "ultra64.h"
#include "PR/sched.h"

#define IDLE_THREAD_SIZE (0x800 / sizeof(u64))
#define IDLE_THREAD_ID 1
#define IDLE_THREAD_PRIORITY OS_PRIORITY_IDLE

#define MAIN_THREAD_SIZE 1024
#define MAIN_THREAD_ID 3
#define MAIN_THREAD_PRIORITY 10

#define OS_SCHEDULER_THREAD_ID 5
#define CONTROLLER_THREAD_ID 98

#define CRASH_THREAD_ID 100
#define CRASH_THREAD_PRIORITY 0x80

extern OSThread* __osRunningThread;
extern OSThread* __osRunQueue;
// this needs double checking. its address is within gMainThreadStack....
extern u8 gIdleThreadStack[IDLE_THREAD_SIZE];
extern OSThread gIdleThread;
extern OSThread gMainThread;
extern u64 gMainThreadStack[];        // some sort of data

/**
 * @returns The address of s->interruptQ.
 */
OSMesgQueue *get_sched_interrupt_queue(OSSched *s);

#endif //_SYS_THREAD_H
