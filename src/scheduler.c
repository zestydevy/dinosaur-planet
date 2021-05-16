#include "common.h"
#include "scheduler.h"

/*
 * private typedefs and defines
 */
#define VIDEO_MSG       666
#define RSP_DONE_MSG    667
#define RDP_DONE_MSG    668
#define PRE_NMI_MSG     669

/*
 * OSScTask state
 */
#define OS_SC_DP                0x0001  /* set if still needs dp        */
#define OS_SC_SP                0x0002  /* set if still needs sp        */
#define OS_SC_YIELD             0x0010  /* set if yield requested       */
#define OS_SC_YIELDED           0x0020  /* set if yield completed       */

OSTime D_800B4988;

void __scMain(void *arg);

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/osCreateScheduler.s")
#else
void osCreateScheduler(OSSched *s, void *stack, OSPri priority, u8 mode, u8 retraceCount) {
    // Initialize scheduler structure
    s->curRSPTask       = NULL;
    s->curRDPTask       = NULL;
    s->clientList       = NULL;
    s->audioListHead    = NULL;
    s->gfxListHead      = NULL;
    s->audioListTail    = NULL;
    s->gfxListTail      = NULL;
    s->doAudio          = 0;
    s->frameCount       = 0;
    s->retraceMsg.type  = OS_SC_RETRACE_MSG;
    s->prenmiMsg.type   = OS_SC_PRE_NMI_MSG;

    // Set up video manager
    osCreateViManager(OS_PRIORITY_VIMGR);
    osViSetMode(&osViModeTable[mode]);
    osViBlack(TRUE);

    // Set up scheduler message queues
    osCreateMesgQueue(&s->interruptQ, &s->intBuf[0], OS_SC_MAX_MESGS);
    osCreateMesgQueue(&s->cmdQ, &s->cmdMsgBuf[0], OS_SC_MAX_MESGS);

    // Listen for Video, RSP, and RDP interrupts
    osSetEventMesg(OS_EVENT_SP, &s->interruptQ, (OSMesg)RSP_DONE_MSG);
    osSetEventMesg(OS_EVENT_DP, &s->interruptQ, (OSMesg)RDP_DONE_MSG);
    osSetEventMesg(OS_EVENT_PRENMI, &s->interruptQ, (OSMesg)PRE_NMI_MSG);

    osViSetEvent(&s->interruptQ, (OSMesg)VIDEO_MSG, retraceCount);

    // Create and start scheduler thread
    osCreateThread(&s->thread, OS_SCHEDULER_THREAD_ID, &__scMain, (void*)s, stack, priority);
    osStartThread(&s->thread);
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/osScAddClient.s")
#else
void osScAddClient(OSSched *s, OSScClient *c, OSMesgQueue *msgQ, u8 param4) {
    OSIntMask mask;

    mask = osSetIntMask(OS_IM_NONE);

    c->msgQ = msgQ;
    c->next = s->clientList;
    c->unk0x0 = param4;
    s->clientList = c;

    osSetIntMask(mask);
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/osScRemoveClient.s")
#else
void osScRemoveClient(OSSched *s, OSScClient *c) {
    OSScClient *client = s->clientList;
    OSScClient *prev = NULL;
    OSIntMask mask;

    mask = osSetIntMask(OS_IM_NONE);

    while (client != NULL) {
        if (client == c) {
            if (prev) {
                prev->next = c->next;
            } else {
                s->clientList = c->next;
            }

            break;
        }

        prev = client;
        client = client->next;
    }

    osSetIntMask(mask);
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/osScGetCmdQ.s")
#else
OSMesgQueue *osScGetCmdQ(OSSched *s) {
    return &s->cmdQ;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/get_sched_interrupt_queue.s")
#else
OSMesgQueue *get_sched_interrupt_queue(OSSched *s) {
    return &s->interruptQ;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/get_float_timers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/__scMain.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_8003B9C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/get_task_type_string.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/some_dummied_task_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_8003BAD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/__scHandleRetrace.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/__scHandleRSP.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/__scHandleRDP.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/__scTaskReady.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/__scTaskComplete.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/__scAppendList.s")
#else
void __scAppendList(OSSched *s, OSScTask *t) {
    u32 type = t->list.t.type;

    if (type == M_AUDTASK) {
        if (s->audioListTail) {
            s->audioListTail->next = t;
        } else {
            s->audioListHead = t;
        }

        s->audioListTail = t;
    } else {
        if (s->gfxListTail) {
            s->gfxListTail->next = t;
        } else {
            s->gfxListHead = t;
        }

        s->gfxListTail = t;
    }

    t->next = NULL;
    t->state = t->flags & OS_SC_RCP_MASK;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/__scExec.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/__scYield.s")
#else
void __scYield(OSSched *s) {
    if (s->curRSPTask->list.t.type == M_GFXTASK) {
        s->curRSPTask->state |= OS_SC_YIELD;

        D_800B4988 = osGetTime();

        osSpTaskYield();
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/__scSchedule.s")
