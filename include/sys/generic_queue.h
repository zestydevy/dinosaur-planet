#ifndef _SYS_GENERIC_QUEUE_H
#define _SYS_GENERIC_QUEUE_H

#include "PR/ultratypes.h"

typedef struct {
    /*00*/ s16 count;
    /*02*/ s16 capacity;
    /*04*/ s16 elementSize; // in bytes
    /*06*/ s16 unk6; // unused and uninitialized (ref'd by unused funcs)
    /*08*/ s16 top;
    /*0A*/ s16 bottom;
    /*0C*/ void *data;
} GenericQueue;

GenericQueue *genericQueueInit(GenericQueue *queue, void *data, s32 capacity, s32 elementSize);
GenericQueue *genericQueueNew(s32 capacity, s32 elementSize);
void genericQueueFree(GenericQueue *queue);
/**
 * Enqueues a copy of the element.
 */
void genericQueueEnqueue(GenericQueue *queue, void *element);
/**
 * Dequeues a copy of the next element in the queue.
 */
void genericQueueDequeue(GenericQueue *queue, void *outElement);
s32 genericQueueIsFull(GenericQueue *queue);
s32 genericQueueIsEmpty(GenericQueue *queue);

#endif //_SYS_GENERIC_QUEUE_H
