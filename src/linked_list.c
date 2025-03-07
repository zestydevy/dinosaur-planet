#include "PR/ultratypes.h"
#include "sys/linked_list.h"

LinkedList *linked_list_init(LinkedList *list, s16 nextFieldOffset) {
    list->head = NULL;
    list->nextFieldOffset = nextFieldOffset;

    return list;
}

void linked_list_prepend(LinkedList *list, void *node) {
    void *prevHead;

    prevHead = list->head;
    list->head = node;

    *LINKED_LIST_NEXT_FIELD(list, node) = prevHead;

    list->count += 1;
}

void linked_list_append(LinkedList *list, void *node) {
    void *next;
    void *last;

    if (list->head == NULL) {
        list->head = node;
    } else {
        next = list->head;
        last = next;

        while (next != NULL) {
            last = next;
            next = *LINKED_LIST_NEXT_FIELD2(list, last);
        }

        *LINKED_LIST_NEXT_FIELD(list, last) = node;
    }

    *LINKED_LIST_NEXT_FIELD(list, node) = NULL;

    list->count += 1;
}

void linked_list_insert(LinkedList *list, void *after, void *node) {
    void *next;

    if (list->head == NULL) {
        list->head = node;
    } else {
        if (after == NULL) {
            // Prepend if after is null
            next = list->head;
            list->head = node;
        } else {
            // Insert between after and its next node
            next = *LINKED_LIST_NEXT_FIELD2(list, after);
            *LINKED_LIST_NEXT_FIELD(list, after) = node;
        }

        *LINKED_LIST_NEXT_FIELD(list, node) = next;
    }

    list->count += 1;
}

void linked_list_remove(LinkedList *list, void *node) {
    void *before;
    void *curr;
    
    if (node == list->head) {
        list->head = *LINKED_LIST_NEXT_FIELD(list, list->head);
        list->count -= 1;
        return;
    }

    // Find the node before the one we want to remove
    curr = list->head;
    before = curr;

    while (curr != NULL && curr != node) {
        before = curr;
        curr = *LINKED_LIST_NEXT_FIELD2(list, curr);
    }

    if (curr != NULL) {
        void *next = *LINKED_LIST_NEXT_FIELD2(list, curr);

        if (curr == list->head) {
            list->head = next;
        } else {
            *LINKED_LIST_NEXT_FIELD(list, before) = next;
        }

        list->count -= 1;
    }
}

void linked_list_remove_fast(LinkedList *list, void *before, void *node) {
    if (node != NULL) {
        void *next = *LINKED_LIST_NEXT_FIELD2(list, node);

        if (node == list->head) {
            list->head = next;
        } else {
            *LINKED_LIST_NEXT_FIELD(list, before) = next;
        }

        list->count -= 1;
    }
}
