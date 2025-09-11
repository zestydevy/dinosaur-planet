#include "PR/os.h"
#include "sys/generic_queue.h"
#include "sys/memory.h"

GenericQueue *generic_queue_init(GenericQueue *queue, void *data, s32 capacity, s32 elementSize) {
    queue->data = data;
    queue->count = 0;
    queue->capacity = capacity;
    queue->elementSize = elementSize;
    queue->top = 0;
    queue->bottom = 0;

    return queue;
}

GenericQueue *generic_queue_new(s32 capacity, s32 elementSize) {
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

void generic_queue_free(GenericQueue *queue) {
    mmFree(queue);
}

void generic_queue_enqueue(GenericQueue *queue, void *element) {
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

void generic_queue_dequeue(GenericQueue *queue, void *outElement) {
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

void generic_queue_func_8000af78(GenericQueue *queue) {
    queue->unk0x6 = queue->bottom;
}

s32 generic_queue_func_8000af88(GenericQueue *queue) {
    return queue->unk0x6 == queue->top;
}

void *generic_queue_func_8000afa0(GenericQueue *queue) {
    void *element;

    element = (void*)((u32)queue->data + queue->unk0x6 * queue->elementSize);
    
    queue->unk0x6 += 1;

    if (queue->unk0x6 == queue->capacity) {
        queue->unk0x6 = 0;
    }

    return element;
}

s32 generic_queue_is_full(GenericQueue *queue) {
    return (queue->count + 1) == queue->capacity;
}

s32 generic_queue_is_empty(GenericQueue *queue) {
    return queue->count == 0;
}
