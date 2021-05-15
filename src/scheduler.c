#include "common.h"
#include "scheduler.h"

#define VIDEO_MSG       666
#define RSP_DONE_MSG    667
#define RDP_DONE_MSG    668
#define PRE_NMI_MSG     669

void __scMain(void *arg);

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/osCreateScheduler.s")
#else
void osCreateScheduler(OSSched *s, void *stack, OSPri priority, u8 mode, u8 retraceCount) {
    // Initialize scheduler structure
    s->curRSPTask = 0;
    s->curRDPTask = 0;
    s->clientList = 0;
    s->audioListHead = 0;
    s->gfxListHead = 0;
    s->audioListTail = 0;
    s->gfxListTail = 0;
    s->doAudio = 0;
    s->frameCount = 0;
    s->retraceMsg.type = OS_SC_RETRACE_MSG;
    s->prenmiMsg.type = OS_SC_PRE_NMI_MSG;

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

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_8003B73C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/get_sched_commandQueue.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/get_sched_interrupt_queue.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/get_float_timers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/__scMain.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_8003B9C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/get_task_type_string.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/some_dummied_task_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_8003BAD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_printing_compile_info.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_setting_float_timers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_8003C6FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_8003C784.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_8003C7DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_8003C8DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_8003C940.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_8003CA10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_8003CA5C.s")
