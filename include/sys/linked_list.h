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

LinkedList *linked_list_init(LinkedList *list, s16 nextFieldOffset);
void linked_list_prepend(LinkedList *list, void *node);
void linked_list_append(LinkedList *list, void *node);
void linked_list_insert(LinkedList *list, void *after, void *node);
void linked_list_remove(LinkedList *list, void *node);
void linked_list_remove_fast(LinkedList *list, void *before, void *node);

#endif
