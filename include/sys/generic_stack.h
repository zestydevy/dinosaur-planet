#ifndef _SYS_GENERIC_STACK_H
#define _SYS_GENERIC_STACK_H

#include "PR/ultratypes.h"

typedef struct {
    /*00*/ s16 count;
    /*02*/ s16 capacity;
    /*04*/ s16 elementSize; // in bytes
    /*06*/ s16 unk6; // unused and uninitialized (ref'd by unused funcs)
    /*08*/ s16 top;
    /*0C*/ void *data;
} GenericStack;

GenericStack *genericStackInit(GenericStack *stack, void *data, s32 capacity, s32 elementSize);
GenericStack *genericStackNew(s32 capacity, s32 elementSize);
void genericStackFree(GenericStack *stack);
/**
 * Pushes a copy of the element onto the stack.
 */
void genericStackPush(GenericStack *stack, void *element);
/**
 * Pops a copy of the top element from the stack.
 */
void genericStackPop(GenericStack *stack, void *outElement);
s32 genericStackIsFull(GenericStack *stack);
s32 genericStackIsEmpty(GenericStack *stack);

#endif //_SYS_GENERIC_STACK_H
