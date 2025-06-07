#ifndef _SYS_OBJMSG_H
#define _SYS_OBJMSG_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef enum {
    // Don't send the message to the sender object.
    OBJMSG_SEND_IGNORE_SENDER = 1,
    // Don't apply a filter.
    OBJMSG_SEND_ALL = 2,
    // Only valid for obj_send_mesg_many_nearby.
    // Whether to filter by ID instead of by group.
    OBJMSG_SEND_FILTER_ID = 4
} ObjMsgSendFilter;

void obj_init_mesg_queue(Object *obj, u32 count);
/**
 * Sends the message to the single given receiver object.
 */
u32 obj_send_mesg(Object *receiver, u32 mesgID, Object *sender, void *mesgArg);
/**
 * Sends the message to all matching objects.
 */
void obj_send_mesg_many(s32 filter, u16 flags, Object *sender, u32 mesgID, void *mesgArg);
/**
 * Sends the message to all matching objects closer than the given distance to the sender.
 */
void obj_send_mesg_many_nearby(s16 objId, f32 distance, u16 flags, Object *sender, u32 mesgID, void *mesgArg);
/**
 * Dequeues and returns the next message in the queue, if any exist.
 * Returns TRUE if a message was dequeued, FALSE if the queue is empty.
 */
s32 obj_recv_mesg(Object *obj, u32 *outMesgID, Object **outSender, void **outMesgArg);
/**
 * Returns the next message in the queue without removing it.
 * Returns TRUE if a message was peeked, FALSE if the queue is empty.
 */
s32 obj_peek_mesg(Object *obj, u32 *outMesgID, Object **outSender, void **outMesgArg);

#endif
