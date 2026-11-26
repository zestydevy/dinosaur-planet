#ifndef _SYS_OBJMSG_H
#define _SYS_OBJMSG_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef void* ObjMesg;
#define OBJMESG_NONE ((ObjMesg)0)

enum ObjMesgID {
    MSG_1 = 0x1,
    MSG_3 = 0x3,
    MSG_4 = 0x4,
    MSG_5 = 0x5,
    MSG_AnimMesg8 = 0x8,
    MSG_AnimMesg9 = 0x9,
    MSG_A = 0xA,
    MSG_B = 0xB,
    MSG_C = 0xC,
    MSG_D = 0xD,
    MSG_E = 0xE,
    MSG_Enter_Windlift = 0xF,
    MSG_Exit_Windlift = 0x10,
    MSG_11 = 0x11,
    MSG_80 = 0x80,
    MSG_81 = 0x81,
    MSG_AnimMesg0 = 0x50001,
    MSG_AnimMesg10 = 0x30002,
    MSG_AnimMesg11 = 0x30003,
    MSG_30005 = 0x30005,
    MSG_30006 = 0x30006,
    MSG_40001 = 0x40001,
    MSG_AnimMesg1 = 0x50002,
    MSG_AnimMesg2 = 0x50003,
    MSG_AnimMesg3 = 0x60001,
    MSG_AnimMesg4 = 0x60002,
    MSG_Hurt_Player_Knock_Down = 0x60003,
    MSG_Hurt_Player_Knock_Down_Reverse = 0x60004,
    MSG_Hurt_Player_DFP_FloorTile_Zap = 0x60005,
    MSG_70004 = 0x70004,
    MSG_70005 = 0x70005,
    MSG_70008 = 0x70008,
    MSG_Collect_Item = 0x7000A,
    MSG_Item_Collected = 0x7000B,
    MSG_Cmdmenu_Item = 0x80001,
    MSG_Cmdmenu_Spell = 0x80002,
    MSG_Cmdmenu_Side = 0x80009,
    MSG_AnimMesg5 = 0xA0001,
    MSG_AnimMesg6 = 0xA0002,
    MSG_AnimMesg7 = 0xA0003,
    MSG_AnimMesg12 = 0xA0004,
    MSG_AnimMesg13 = 0xA0005,
    MSG_AnimMesg14 = 0xA0006,
    MSG_D0004 = 0xD0004,
    MSG_Clear_Target = 0xE0000,
    MSG_E0001 = 0xE0001,
    MSG_RobotPatrol_GetNextCurve = 0xF0001,
    MSG_RobotPatrol_SetCurve = 0xF0002,
    MSG_F0003 = 0xF0003,
    MSG_F0004 = 0xF0004,
    MSG_F0005 = 0xF0005,
    MSG_F0006 = 0xF0006,
    MSG_F0007 = 0xF0007,
    MSG_F0008 = 0xF0008,
    MSG_F0009 = 0xF0009,
    MSG_F000A = 0xF000A,
    MSG_AnimMesg15 = 0xF000B,
    MSG_AnimMesg16 = 0xF000C,
    MSG_AnimMesg17 = 0xF000D,
    MSG_AnimMesg18 = 0xF000E,
    MSG_AnimMesg19 = 0xF000F,
    MSG_AnimMesg20 = 0xF0010,
    MSG_Carrying_Object = 0x100008,
    MSG_Carrying_Basket = 0x100010,
    MSG_110001 = 0x110001,
    MSG_110002 = 0x110002,
    MSG_110003 = 0x110003,
    MSG_110004 = 0x110004,
    MSG_AnimMesg21 = 0x130001,
    MSG_AnimMesg22 = 0x130002,
    MSG_130003 = 0x130003,
    MSG_140001 = 0x140001
};

typedef enum {
    // Don't send the message to the sender object.
    OBJMSG_SEND_IGNORE_SENDER = 1,
    // Don't apply a filter.
    OBJMSG_SEND_ALL = 2,
    // Only valid for objSendMesgMany.
    // Whether to filter by ID instead of by controlNo.
    OBJMSG_SEND_FILTER_ID = 4
} ObjMsgSendFilter;

void objInitMesgQueue(Object* obj, u32 count);
/**
 * Sends the message to the single given receiver object.
 */
u32 objSendMesg(Object* receiver, u32 mesgID, Object* sender, ObjMesg mesgArg);
/**
 * Sends the message to all matching objects.
 */
void objSendMesgMany(s32 filter, u16 flags, Object* sender, u32 mesgID, ObjMesg mesgArg);
/**
 * Sends the message to all matching objects closer than the given distance to the sender.
 */
void objSendMesgManyNearby(s16 objId, f32 distance, u16 flags, Object* sender, u32 mesgID, ObjMesg mesgArg);
/**
 * Dequeues and returns the next message in the queue, if any exist.
 * Returns TRUE if a message was dequeued, FALSE if the queue is empty.
 */
s32 objRecvMesg(Object* obj, u32* outMesgID, Object** outSender, ObjMesg* outMesgArg);
/**
 * Returns the next message in the queue without removing it.
 * Returns TRUE if a message was peeked, FALSE if the queue is empty.
 */
s32 objPeekMesg(Object* obj, u32* outMesgID, Object** outSender, ObjMesg* outMesgArg);

#endif
