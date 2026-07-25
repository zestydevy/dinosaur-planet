#ifndef _SYS_LINKED_LIST_H
#define _SYS_LINKED_LIST_H

#include "PR/ultratypes.h"

typedef struct {
    s16 count;
    s16 nextFieldOffset;
    void *head;
} LinkedList;

// Pointer to the "next" field of a node
#define LINKED_LIST_NEXT_FIELD(list, node) ((void**)((u32)node + list->nextFieldOffset))
// Same as LINKED_LIST_NEXT_FIELD but the add operands are reversed... just exists for matching code 
#define LINKED_LIST_NEXT_FIELD2(list, node) ((void**)(list->nextFieldOffset + (u32)node))
// Also same as above but list is not a pointer
#define LINKED_LIST_NEXT_FIELD3(list, node) ((void**)((u32)node + list.nextFieldOffset))

LinkedList *linkedListInit(LinkedList *list, s16 nextFieldOffset);
void linkedListPrepend(LinkedList *list, void *node);
void linkedListAppend(LinkedList *list, void *node);
void linkedListInsert(LinkedList *list, void *after, void *node);
void linkedListRemove(LinkedList *list, void *node);
void linkedListRemoveFast(LinkedList *list, void *before, void *node);

#endif
