#include "PR/os.h"
#include "sys/generic_queue.h"
#include "sys/memory.h"

GenericQueue *genericQueueInit(GenericQueue *queue, void *data, s32 capacity, s32 elementSize) {
    queue->data = data;
    queue->count = 0;
    queue->capacity = capacity;
    queue->elementSize = elementSize;
    queue->top = 0;
    queue->bottom = 0;

    return queue;
}

GenericQueue *genericQueueNew(s32 capacity, s32 elementSize) {
    GenericQueue *queue;

    queue = mmAlloc(
        /*elementSize*/(elementSize * capacity) + (sizeof(GenericQueue)),
        /*tag*/ALLOC_TAG_TEST_COL,
        /*name*/NULL);
    
    queue->data = queue + 1;
    queue->count = 0;
    queue->capacity = capacity;
    queue->elementSize = elementSize;
    queue->top = 0;
    queue->bottom = 0;

    return queue;
}

void genericQueueFree(GenericQueue *queue) {
    mmFree(queue);
}

void genericQueueEnqueue(GenericQueue *queue, void *element) {
    bcopy(
        /*src*/element, 
        /*dst*/(void*)((u32)queue->data + queue->top * queue->elementSize), 
        /*len*/queue->elementSize);

    queue->top += 1;

    if (queue->top == queue->capacity) {
        queue->top = 0;
    }
    
    queue->count += 1;
}

void genericQueueDequeue(GenericQueue *queue, void *outElement) {
    bcopy(
        /*src*/(void*)((u32)queue->data + queue->bottom * queue->elementSize), 
        /*dst*/outElement, 
        /*len*/queue->elementSize);

    queue->bottom += 1;

    if (queue->bottom == queue->capacity) {
        queue->bottom = 0;
    }
    
    queue->count -= 1;
}

void genericQueue_func_8000af78(GenericQueue *queue) {
    queue->unk6 = queue->bottom;
}

s32 genericQueue_func_8000af88(GenericQueue *queue) {
    return queue->unk6 == queue->top;
}

void *genericQueue_func_8000afa0(GenericQueue *queue) {
    void *element;

    element = (void*)((u32)queue->data + queue->unk6 * queue->elementSize);
    
    queue->unk6 += 1;

    if (queue->unk6 == queue->capacity) {
        queue->unk6 = 0;
    }

    return element;
}

s32 genericQueueIsFull(GenericQueue *queue) {
    return (queue->count + 1) == queue->capacity;
}

s32 genericQueueIsEmpty(GenericQueue *queue) {
    return queue->count == 0;
}
