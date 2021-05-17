#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "ultra64.h"

#define OS_SCHEDULER_THREAD_ID 5

/**
 * @returns The address of s->interruptQ.
 */
OSMesgQueue *get_sched_interrupt_queue(OSSched *s);

#endif
