#include "game/objects/object.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "macros.h"

void obj_init_mesg_queue(Object *obj, u32 count) {
    ObjectMesgQueue *mesgQueue;
    s32 size;
    
    if (count != 0) {
        if (obj != NULL && obj->mesgQueue == NULL) {
            size = count * 3;

            mesgQueue = (ObjectMesgQueue*)mmAlloc(
                size * 4 + (sizeof(ObjectMesgQueue) - sizeof(ObjMesgQueueMessage)), 
                ALLOC_TAG_OBJECTS_COL, 
                ALLOC_NAME("obj:msgs"));
            mesgQueue->count = 0;
            mesgQueue->capacity = count;

            obj->mesgQueue = mesgQueue;
        }
    }
}

u32 obj_send_mesg(Object *receiver, u32 mesgID, Object *sender, void *mesgArg) {
    ObjectMesgQueue *mesgQueue;
    u32 *temp_a2;
    int new_var;

    if (receiver == NULL){
        return 0;
    }

    // TODO: this is an awful match
    mesgQueue = receiver->mesgQueue;
    if (mesgQueue != NULL) {
        if (mesgQueue->count < mesgQueue->capacity) {
            new_var = (mesgQueue->count + mesgQueue->count) + mesgQueue->count;
            temp_a2 = ((u32 *) mesgQueue) + new_var;
            temp_a2[2] = mesgID;
            temp_a2[3] = (u32)sender;
            temp_a2[4] = (u32)mesgArg;
            mesgQueue->count += 1;
            return mesgQueue->count;
        }
        STUBBED_PRINTF("objmsg (%x): overflow in object %d defno=%d FROM: defno %d\n",
            mesgID, receiver->group, receiver->id, sender->id);
    }

    return 0;
}

void obj_send_mesg_many(s32 filter, u16 flags, Object *sender, u32 mesgID, void *mesgArg) {
    Object** objects;
    s32 i;
    s32 numObjs;
    Object* obj;

    objects = get_world_objects(&i, &numObjs);
    if (flags & OBJMSG_SEND_FILTER_ID) {
        for (; i < numObjs; i++) {
            obj = objects[i];
            
            if ((sender != obj || !(flags & OBJMSG_SEND_IGNORE_SENDER)) && ((flags & OBJMSG_SEND_ALL) || filter == obj->id)) {
                obj_send_mesg(obj, mesgID, sender, mesgArg);
            }
        }
    } else {
        for (; i < numObjs; i++) {
            obj = objects[i];
            
            if ((sender != obj || !(flags & OBJMSG_SEND_IGNORE_SENDER)) && ((flags & OBJMSG_SEND_ALL) || filter == obj->group)) {
                obj_send_mesg(obj, mesgID, sender, mesgArg);
            }
        }
    }
}

void obj_send_mesg_many_nearby(s16 objId, f32 distance, u16 flags, Object *sender, u32 mesgID, void *mesgArg) {
    Object *obj;
    Object **objects;
    s32 i;
    s32 numObjs;

    objects = get_world_objects(&i, &numObjs);
    for (; i < numObjs; i++) {
        obj = objects[i];

        if ((obj != sender || !(flags & OBJMSG_SEND_IGNORE_SENDER)) && (objId == obj->id || (flags & OBJMSG_SEND_ALL))) {
            if (vec3_distance(&sender->positionMirror, &obj->positionMirror) < distance) {
                if (obj_send_mesg(obj, mesgID, sender, mesgArg) == 0) {
                    
                }
            }
        }
    }
}

s32 obj_recv_mesg(Object *obj, u32 *outMesgID, Object **outSender, void **outMesgArg) {
    u32 i;
    ObjectMesgQueue* mesgQueue;
    
    if (obj == NULL) {
        return FALSE;
    }
    
    mesgQueue = obj->mesgQueue;
    if (mesgQueue != NULL && mesgQueue->count != 0) {
        mesgQueue->count--;
        
        if (outMesgID != NULL) {
            *outMesgID = mesgQueue->queue[0].id;
        }
        if (outSender != NULL) {
            *outSender = mesgQueue->queue[0].sender;
        }
        if (outMesgArg != NULL) {
            *outMesgArg = mesgQueue->queue[0].arg;
        }

        for (i = 0; i < mesgQueue->count; i++) {
            mesgQueue->queue[i].id     = mesgQueue->queue[i+1].id;
            mesgQueue->queue[i].sender = mesgQueue->queue[i+1].sender;
            mesgQueue->queue[i].arg    = mesgQueue->queue[i+1].arg;
        }
        
        return TRUE;
    }
    
    return FALSE;
}

s32 obj_peek_mesg(Object *obj, u32 *outMesgID, Object **outSender, void **outMesgArg) {
    ObjectMesgQueue* temp_v0;

    if (obj == NULL) {
        return FALSE;
    }

    temp_v0 = obj->mesgQueue;
    if (temp_v0 != NULL && temp_v0->count != 0) {
        if (outMesgID != NULL) {
            *outMesgID = temp_v0->queue[0].id;
        }
        if (outSender != NULL) {
            *outSender = temp_v0->queue[0].sender;
        }
        if (outMesgArg != NULL) {
            *outMesgArg = temp_v0->queue[0].arg;
        }

        return TRUE;
    }

    return FALSE;
}
