#include "PR/os.h"
#include "sys/generic_stack.h"
#include "sys/memory.h"

GenericStack *genericStackInit(GenericStack *stack, void *data, s32 capacity, s32 elementSize) {
    stack->data = data;
    stack->count = 0;
    stack->capacity = capacity;
    stack->elementSize = elementSize;
    stack->top = 0;

    return stack;
}

GenericStack *genericStackNew(s32 capacity, s32 elementSize) {
    GenericStack *stack;

    stack = mmAlloc(
        /*elementSize*/(elementSize * capacity) + (sizeof(GenericStack)),
        /*tag*/ALLOC_TAG_TEST_COL,
        /*name*/ALLOC_NAME("dtype:stack"));
    
    stack->data = stack + 1;
    stack->count = 0;
    stack->capacity = capacity;
    stack->elementSize = elementSize;
    stack->top = 0;

    return stack;
}

void genericStackFree(GenericStack *stack) {
    mmFree(stack);
}

void genericStackPush(GenericStack *stack, void *element) {
    bcopy(
        /*src*/element, 
        /*dst*/(void*)((u32)stack->data + stack->top * stack->elementSize), 
        /*len*/stack->elementSize);

    stack->top += 1;

    if (stack->top == stack->capacity) {
        stack->top = 0;
    }
    
    stack->count += 1;
}

void genericStackPop(GenericStack *stack, void *outElement) {
    stack->top -= 1;

    if (stack->top < 0) {
        stack->top = stack->capacity - 1;
    }

    bcopy(
        /*src*/(void*)((u32)stack->data + stack->top * stack->elementSize), 
        /*dst*/outElement, 
        /*len*/stack->elementSize);
    
    stack->count -= 1;
}

void genericStackDeleteElement(GenericStack *stack, s32 idx) {
    u8 *end;
    u8 *src;
    u8 *dst;

    end = (u8*)((u32)stack->data + stack->count * stack->elementSize);

    src = (u8*)((u32)stack->data + (idx + 1) * stack->elementSize);
    dst = src - stack->elementSize;

    while (src < end) {
        bcopy(src, dst, stack->elementSize);

        src += stack->elementSize;
        dst += stack->elementSize;
    }

    stack->count -= 1;
}

void generic_stack_func_8000B23C(GenericStack *stack) {
    stack->unk6 = 0;
}

s32 generic_stack_func_8000B248(GenericStack *stack) {
    return stack->unk6 == stack->top;
}

void *generic_stack_func_8000B260(GenericStack *stack) {
    void *element;

    element = (void*)((u32)stack->data + stack->unk6 * stack->elementSize);
    
    stack->unk6 += 1;

    if (stack->unk6 == stack->capacity) {
        stack->unk6 = 0;
    }

    return element;
}

s32 genericStackIsFull(GenericStack *stack) {
    return (stack->count + 1) == stack->capacity;
}

s32 genericStackIsEmpty(GenericStack *stack) {
    return stack->count == 0;
}
