// Thanks to the libreultra project for most of the matching code here.
// See https://github.com/n64decomp/libreultra/blob/master/src/sched/sched.c

#include "common.h"
#include "PR/os_internal.h"

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

/*
 * OSScTask->flags type identifier
 */
#define OS_SC_XBUS      (OS_SC_SP | OS_SC_DP)
#define OS_SC_DRAM      (OS_SC_SP | OS_SC_DP | OS_SC_DRAM_DLIST)
#define OS_SC_DP_XBUS   (OS_SC_SP)
#define OS_SC_DP_DRAM   (OS_SC_SP | OS_SC_DRAM_DLIST)
#define OS_SC_SP_XBUS   (OS_SC_DP)
#define OS_SC_SP_DRAM   (OS_SC_DP | OS_SC_DRAM_DLIST)

/*
 * OSScTask taskType
 */
#define OS_SC_TASK_AUDIO 1
#define OS_SC_TASK_GAME 2
#define OS_SC_TASK_DI 3
#define OS_SC_TASK_DI_BENCHMARK_TEST 4

extern char gStrAudioTask[];        // "(Audio task)"
extern char gStrGameTask[];         // "(Game task)"
extern char gStrDITask[];           // "(DI task)\n"
extern char gStrDIBenchmarkTest[];  // "(DI benchmark test)\n"
extern char gStrUnknownTaskType[];  // "(Unknown task type)\n"

extern void *D_800918D0;

extern f32 floatTimer0;
extern f32 floatTimer1;
extern f32 floatTimer2;
extern f32 floatTimer3;

extern OSMesg D_800918D8;

extern s32 intTimer0;
extern s32 D_800918F4;
extern s32 D_800918F8;
extern u64 intTimer1;

extern f32 D_8009A340;
extern f32 D_8009A344;

extern s32 D_800B4980;
extern s32 D_800B4984;

OSTime D_800B4988;

extern u32 countRegA;
extern u32 countRegB;

extern char D_8009A280[]; // "SP CRASHED, gfx=%x\n"
extern char D_8009A294[]; // "TRACE:  %s:%d    gfx=%x\n"
extern char D_8009A2B0[]; // "TRACE:  %s:%d    gfx=%x\n"
extern char D_8009A2CC[]; // "DP CRASHED, gfx=%x\n"
extern char D_8009A2E0[]; // "TRACE:  %s:%5d    gfx=%x\n"
extern char D_8009A2FC[]; // "TRACE:  %s:%5d    gfx=%x\n"
extern char D_8009A318[]; // "CODE: Version %s  %s  %s\n" TODO: add symbol
extern char **D_8008C8F0; // ptr to "1.3623"
extern char **D_8008C8F4; // ptr to "01/12/00 09:19"
extern char **D_8008C8F8; // ptr to "dragon1"
extern char D_8009A334[]; // "DB:   %s\n"
extern char D_8008C8FC[]; // "Version 2.8 14/12/98 15.30 L.Schuneman"

void __scMain(void *arg);
void func_8003B9C0(OSSched *sc);
void __scHandleRetrace(OSSched *sc);
void __scHandleRSP(OSSched *sc);
void __scHandleRDP(OSSched *sc);
s32 __scTaskComplete(OSSched *sc, OSScTask *t);
void __scAppendList(OSSched *s, OSScTask *t);
void __scExec(OSSched *sc, OSScTask *sp, OSScTask *dp);
void __scYield(OSSched *sc);
s32 __scSchedule(OSSched *sc, OSScTask **sp, OSScTask **dp, s32 availRCP);

void func_80060EB8(u8, u8, u8, u8);
void func_80060FD0(u32, u32);

void osCreateScheduler(OSSched *s, void *stack, OSPri priority, u8 mode, u8 retraceCount) {
    // Initialize scheduler structure
    s->curRSPTask       = NULL;
    s->curRDPTask       = NULL;
    s->clientList       = NULL;
    s->audioListHead    = NULL;
    s->gfxListHead      = NULL;
    s->audioListTail    = NULL;
    s->gfxListTail      = NULL;
    s->frameCount       = 0;
    s->unkTask          = NULL;
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

/**
 * Add a client to the scheduler. Clients receive messages at retrace time.
 */
void osScAddClient(OSSched *s, OSScClient *c, OSMesgQueue *msgQ, u8 param4) {
    OSIntMask mask;

    mask = osSetIntMask(OS_IM_NONE);

    c->msgQ = msgQ;
    c->next = s->clientList;
    c->unk0x0 = param4;
    s->clientList = c;

    osSetIntMask(mask);
}

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

OSMesgQueue *osScGetCmdQ(OSSched *s) {
    return &s->cmdQ;
}

OSMesgQueue *get_sched_interrupt_queue(OSSched *s) {
    return &s->interruptQ;
}

void get_float_timers(f32 *timer0, f32 *timer1, f32 *timer2) {
    f32 temp = D_8009A340;

    *timer0 = floatTimer0 * temp;
    *timer1 = floatTimer2 * temp;
    *timer2 = floatTimer3 * temp;
}

void __scMain(void *arg) {
    OSMesg msg = 0;
    s32 state;
    OSSched *sc = (OSSched*)arg;
    OSScClient *client;
    OSScTask *sp = 0, *dp = 0;

    while (TRUE) {
        osRecvMesg(&sc->interruptQ, &msg, OS_MESG_BLOCK);

        switch ((int)msg) {

            case VIDEO_MSG:
                __scHandleRetrace(sc);
                break;

            case RSP_DONE_MSG:
                __scHandleRSP(sc);
                break;

            case RDP_DONE_MSG:
                __scHandleRDP(sc);
                break;

            case 0x63:
                func_8003B9C0(sc);
                break;

            case PRE_NMI_MSG:
                // notify audio and graphics threads to fade out
                for (client = sc->clientList; client != 0; client = client->next) {
                    osSendMesg(client->msgQ, &sc->prenmiMsg, OS_MESG_NOBLOCK);
                }
                break;

            default:
                __scAppendList(sc, (OSScTask *)msg);

                state = ((sc->curRSPTask == 0) << 1) | (sc->curRDPTask == 0);
                if ((__scSchedule(sc, &sp, &dp, state)) != state) {
                    __scExec(sc, sp, dp);
                }
                break;
        }
    }
}

void func_8003B9C0(OSSched *sc) {
    s32 state;
    OSScTask *sp = 0, *dp = 0;

    if (sc->audioListHead) {
        sc->doAudio = 1;
    }

    if (sc->doAudio != 0 && sc->curRSPTask) {
        __scYield(sc);
        return;
    }

    state = ((sc->curRSPTask == 0) << 1) | (sc->curRDPTask == 0);
    if ((__scSchedule(sc, &sp, &dp, state)) != state) {
        __scExec(sc, sp, dp);
    }
}

/**
 * Gets a string for a OSScTask task type.
 */
char *get_task_type_string(u32 taskType) {
    switch (taskType) {
        case OS_SC_TASK_AUDIO:
            return gStrAudioTask;
        case OS_SC_TASK_GAME:
            return gStrGameTask;
        case OS_SC_TASK_DI:
            return gStrDITask;
        case OS_SC_TASK_DI_BENCHMARK_TEST:
            return gStrDIBenchmarkTest;
        default:
            return gStrUnknownTaskType;
    }
}

void some_dummied_task_func(int _) { }

Gfx *func_8003BAD0(OSSched *sc, 
    char **retFile, u32 *retUnk0xc, s32 *retUnk0x10,
    char **retFile_2, u32 *retUnk0xc_2, s32 *retUnk0x10_2) {

    OSMesg queueBuffer[8];
    OSMesg queueMsg;
    OSMesgQueue queue;
    s64 tempCmds[2];
    Gfx *displayListPtr;
    OSTask *task;
    s32 dldi_unk0x10_2;
    s32 dldi_unk0x10;
    char *dldi_file_2;
    char *dldi_file;
    s32 done;
    s32 numRetraces;
    s32 gotRdpDone;
    s32 cmdIndex;
    u32 dldi_unk0xc_2;
    u32 dldi_unk0xc;
    u32 dldi_unk0x4_2;
    u32 dldi_unk0x4;

    queueMsg = 0;
    done = FALSE;

    // Stop audio syn thread
    stop_alSyn_thread();

    task = &sc->curRSPTask->list;
    cmdIndex = task->t.data_size / 2;
    displayListPtr = (Gfx*)&task->t.data_ptr[cmdIndex];

    // Temporarily steal events for SP completion, DP fullsync, and vertical retraces
    osCreateMesgQueue(&queue, queueBuffer, 8);
    osSetEventMesg(OS_EVENT_SP, &queue, (OSMesg)RSP_DONE_MSG);
    osSetEventMesg(OS_EVENT_DP, &queue, (OSMesg)RDP_DONE_MSG);
    osViSetEvent(&queue, (OSMesg)VIDEO_MSG, /*retraceCount*/1);

    do {
        // Halt the SP, disable interrupt on break, and clear all signals
        __osSpSetStatus(
            SP_SET_HALT | 
            SP_CLR_INTR_BREAK | 
            SP_CLR_SIG0 | 
            SP_CLR_SIG1 | 
            SP_CLR_SIG2 | 
            SP_CLR_SIG3 | 
            SP_CLR_SIG4 | 
            SP_CLR_SIG5 | 
            SP_CLR_SIG6 | 
            SP_CLR_SIG7);
        
        gotRdpDone = FALSE;
        numRetraces = 0;
        
        // Reset DP?
        osDpSetStatus(
            DPC_SET_XBUS_DMEM_DMA | 
            DPC_CLR_FREEZE | 
            DPC_CLR_FLUSH | 
            DPC_CLR_TMEM_CTR | 
            DPC_CLR_PIPE_CTR | 
            DPC_CLR_CMD_CTR);

        // Temporarily patch an early end command into the display list
        tempCmds[1] = ((s64*)displayListPtr)[0];
        tempCmds[0] = ((s64*)displayListPtr)[1];

        gDPFullSync(&displayListPtr[0]);
        gSPEndDisplayList(&displayListPtr[1]);

        // Flush CPU cache
        osWritebackDCacheAll();

        // Run the task
        osSpTaskLoad(&sc->curRSPTask->list);
        osSpTaskStartGo(&sc->curRSPTask->list);

        // Wait until 10 vertical retraces occur or we receive RDP done
        do {
            osRecvMesg(&queue, &queueMsg, OS_MESG_BLOCK);

            switch ((int)queueMsg) {
                case VIDEO_MSG:
                    numRetraces++;
                    break;
                case RDP_DONE_MSG:
                    gotRdpDone = TRUE;
                    break;
                case RSP_DONE_MSG:
                    break;
            }
        } while (numRetraces < 10 && !gotRdpDone);

        // Restore the original display list commands
        ((s64*)displayListPtr)[0] = tempCmds[1];
        ((s64*)displayListPtr)[1] = tempCmds[0];

        if (cmdIndex < 2) {
            done = TRUE;
        }

        if ((cmdIndex % 2) != 0) {
            cmdIndex = (cmdIndex / 2) + 1;
        } else {
            cmdIndex = cmdIndex / 2;
        }

        if (gotRdpDone) {
            displayListPtr += cmdIndex;
        } else {
            displayListPtr -= cmdIndex;
        }
    } while (!done);

    // Restore SP, DP, and vertical retrace event handling to the scheduler
    osSetEventMesg(OS_EVENT_SP, &sc->interruptQ, (OSMesg)RSP_DONE_MSG);
    osSetEventMesg(OS_EVENT_DP, &sc->interruptQ, (OSMesg)RDP_DONE_MSG);
    osViSetEvent(&sc->interruptQ, (OSMesg)VIDEO_MSG, /*retraceCount*/1);

    if (gotRdpDone) {
        *retFile_2 = NULL;
        *retFile = NULL;

        dl_get_debug_info2(displayListPtr,
            &dldi_unk0x4, &dldi_file, &dldi_unk0xc, &dldi_unk0x10,
            &dldi_unk0x4_2, &dldi_file_2, &dldi_unk0xc_2, &dldi_unk0x10_2);
        
        if (dldi_file_2 != NULL || dldi_file != NULL) {
            if (dldi_file != NULL) {
                *retFile = dldi_file;
                *retUnk0xc = dldi_unk0xc;
                *retUnk0x10 = dldi_unk0x10;
            }
            if (dldi_file_2 != NULL) {
                *retFile_2 = dldi_file_2;
                *retUnk0xc_2 = dldi_unk0xc_2;
                *retUnk0x10_2 = dldi_unk0x10_2;
            }
        }
    }

    return displayListPtr;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/__scHandleRetrace.s")
#else
// A single %hi isn't being reused, everything else matches, even regalloc
void a__scHandleRetrace(OSSched *sc) {
    OSScTask *rspTask = NULL;   // sp+0xd4
    OSScClient *client;
    s32 state;
    OSScTask *sp = 0;       // sp+0xc8
    OSScTask *dp = 0;       // sp+0xc4
    u8 set_curRSPTask_NULL = FALSE; // sp+0xc2
    u8 set_curRDPTask_NULL = FALSE;
    Gfx *displayListPtr1;   // sp+0xbc
    Gfx *displayListPtr2;   // sp+0xb8
    s32 sp_dldi_unk0x10;    // sp+0xb4
    s32 sp_dldi_unk0x10_2;  // sp+0xb0
    s32 dp_dldi_unk0x10;    // sp+0xac
    s32 dp_dldi_unk0x10_2;  // sp+0xa8
    char *sp_dldi_file;     // sp+0xa4
    char *sp_dldi_file_2;   // sp+0xa0 
    char *dp_dldi_file;     // sp+0x9c 
    char *dp_dldi_file_2;   // sp+0x98
    u32 sp_dldi_unk0xc;     // sp+0x94
    u32 sp_dldi_unk0xc_2;   // sp+0x90
    u32 dp_dldi_unk0xc;     // sp+0x8c
    u32 dp_dldi_unk0xc_2;   // sp+0x88
    OSMesg queueBuffer[8];  // sp+0x68
    OSMesgQueue queue;      // sp+0x50
    OSScTask *unkTask2;     // sp+0x4c
    u64 *taskDataPtr;       // sp+0x48
    OSScTask *unkTask;

    if (sc->curRSPTask) {
        D_800918F4++; // gCurRSPTaskCounter
    }

    if (sc->curRDPTask) {
        D_800918F8++; // gCurRDPTaskCounter
    }

    displayListPtr1 = NULL;
    displayListPtr2 = NULL;

    if ((D_800918F4 > 10) && (sc->curRSPTask)) {
        if (D_800B4980) {
            get_task_type_string(sc->curRSPTask->taskType);
            some_dummied_task_func(sc->curRSPTask);

            if (sc->curRSPTask->list.t.type == OS_SC_TASK_AUDIO) {
                displayListPtr1 = func_8003BAD0(sc, 
                    &sp_dldi_file, &sp_dldi_unk0xc, &sp_dldi_unk0x10, 
                    &sp_dldi_file_2, &sp_dldi_unk0xc_2, &sp_dldi_unk0x10_2);
            }

            D_800B4980 = 0;
        }

        D_800918F4 = 0;
        set_curRSPTask_NULL = TRUE;

        __osSpSetStatus(SP_SET_HALT | SP_CLR_INTR_BREAK | SP_CLR_SIG0 |
            SP_CLR_SIG1 | SP_CLR_SIG2 | SP_CLR_SIG3 | SP_CLR_SIG4 |
            SP_CLR_SIG5 | SP_CLR_SIG6 | SP_CLR_SIG7);
    } else if (sc->curRSPTask) {
        D_800B4980 = TRUE; // gCurRSPTaskIsSet
    }

    if ((D_800918F8 > 10) && (sc->curRDPTask)) {
        if (sc->curRDPTask->unk0x68 == 0) {
            osSendMesg(sc->curRDPTask->msgQ, &D_800918D8, OS_MESG_BLOCK);
        }

        if (D_800B4984) {
            get_task_type_string(sc->curRDPTask->taskType);
            some_dummied_task_func(sc->curRDPTask);

            if (sc->curRDPTask->list.t.type == OS_SC_TASK_AUDIO) {
                displayListPtr2 = func_8003BAD0(sc, 
                    &dp_dldi_file, &dp_dldi_unk0xc, &dp_dldi_unk0x10, 
                    &dp_dldi_file_2, &dp_dldi_unk0xc_2, &dp_dldi_unk0x10_2);
            }

            D_800B4984 = FALSE;
        }

        set_curRDPTask_NULL = TRUE;
        sc->frameCount = 0;
        D_800918F8 = 0;

        __osSpSetStatus(SP_SET_HALT | SP_CLR_INTR_BREAK | SP_CLR_SIG0 |
            SP_CLR_SIG1 | SP_CLR_SIG2 | SP_CLR_SIG3 | SP_CLR_SIG4 |
            SP_CLR_SIG5 | SP_CLR_SIG6 | SP_CLR_SIG7);

        osDpSetStatus(DPC_SET_XBUS_DMEM_DMA | DPC_CLR_FREEZE | DPC_CLR_FLUSH |
            DPC_CLR_TMEM_CTR | DPC_CLR_PIPE_CTR | DPC_CLR_CMD_CTR);
    } else if (sc->curRDPTask) {
        D_800B4984 = TRUE; // gCurRDPTaskIsSet 
    }

    if (displayListPtr1 != NULL || displayListPtr2 != NULL) {
        osCreateMesgQueue(&queue, queueBuffer, 8);
        
        osSetEventMesg(OS_EVENT_SP, &queue, (OSMesg)RSP_DONE_MSG);
        osSetEventMesg(OS_EVENT_DP, &queue, (OSMesg)RDP_DONE_MSG);
        osViSetEvent(&queue, (OSMesg)VIDEO_MSG, /*retraceCount*/1);

        unkTask2 = sc->curRSPTask != NULL ? sc->curRSPTask : sc->curRDPTask;
        taskDataPtr = unkTask2->list.t.data_ptr;

        dl_segment((Gfx**)&taskDataPtr, 0, NULL);
        dl_segment((Gfx**)&taskDataPtr, 1, gFramebufferCurrent);
        dl_segment((Gfx**)&taskDataPtr, 2, D_800bccb4);
        dl_segment((Gfx**)&taskDataPtr, 4, gFramebufferNext - 0x280);

        func_80060EB8(0, 0, 0, 0x80);

        if (displayListPtr1 != NULL) {
            func_80060FD0(0x1e, 0x1e);
            dummied_print_func(&D_8009A280, displayListPtr1);

            if (sp_dldi_file != NULL || sp_dldi_file_2 != NULL) {
                if (sp_dldi_file != NULL) {
                    func_80060FD0(0x1e, 0x28);
                    dummied_print_func(D_8009A294, sp_dldi_file, sp_dldi_unk0xc, sp_dldi_unk0x10);
                }

                if (sp_dldi_file_2 != NULL) {
                    func_80060FD0(0x1e, 0x32);
                    dummied_print_func(D_8009A2B0, sp_dldi_file_2, sp_dldi_unk0xc_2, sp_dldi_unk0x10_2);
                }
            }
        }

        if (displayListPtr2 != NULL) {
            func_80060FD0(0x1e, 0x46);
            dummied_print_func(&D_8009A2CC, displayListPtr2);

            if (sp_dldi_file != NULL || sp_dldi_file_2 != NULL) {
                if (sp_dldi_file != NULL) {
                    func_80060FD0(0x1e, 0x50);
                    dummied_print_func(D_8009A2E0, sp_dldi_file, sp_dldi_unk0xc, sp_dldi_unk0x10);
                }

                if (sp_dldi_file_2 != NULL) {
                    func_80060FD0(0x1e, 0x5a);
                    dummied_print_func(D_8009A2FC, sp_dldi_file_2, sp_dldi_unk0xc_2, sp_dldi_unk0x10_2);
                }
            }
        }

        displayListPtr1 = NULL;
        displayListPtr2 = NULL;

        func_80060FD0(0x1e, 0x6e);
        dummied_print_func(&D_8009A318, D_8008C8F0, D_8008C8F4, D_8008C8F8);

        func_80060FD0(0x1e, 0x78);
        dummied_print_func(&D_8009A334, &D_8008C8FC);

        func_80060B94((Gfx**)&taskDataPtr);

        __osSpSetStatus(0xaaaa82);
        osDpSetStatus(0x1d6);

        gDPFullSync(taskDataPtr++);
        gSPEndDisplayList(taskDataPtr++);

        osWritebackDCacheAll();

        osSpTaskLoad(&unkTask2->list);
        osSpTaskStartGo(&unkTask2->list);

        while (TRUE) {}
    }

    if (set_curRSPTask_NULL) {
        sc->curRSPTask = NULL;
    }

    if (set_curRDPTask_NULL) {
        sc->curRDPTask = NULL;
    }

    while (osRecvMesg(&sc->cmdQ, (OSMesg *)&rspTask, OS_MESG_NOBLOCK) != -1)
        __scAppendList(sc, rspTask);

    state = ((sc->curRSPTask == NULL) << 1) | (sc->curRDPTask == NULL);
    if (__scSchedule(sc, &sp, &dp, state) != state)
        __scExec(sc, sp, dp);

    intTimer1++;
    intTimer0++;
    sc->frameCount++;

    if ((sc->unkTask) && (sc->frameCount >= 2)) {
        unkTask = sc->unkTask;
        if (unkTask->msgQ) {
            if ((unkTask->unk0x68) || (unkTask->msg)) {
                osSendMesg(unkTask->msgQ, unkTask->msg, OS_MESG_BLOCK);
            } else {
                osSendMesg(unkTask->msgQ, &D_800918D0, OS_MESG_BLOCK);
            }
        }
        sc->frameCount = 0;
        sc->unkTask = 0;
    }

    for (client = sc->clientList; client != 0; client = client->next) {
        if (client->unk0x0 == 1) {
            //Only run this on even calls to this function
            if (intTimer1 % 2 == 0) {
                osSendMesg(client->msgQ, sc, OS_MESG_NOBLOCK);
                if (sc->audioListHead) {
                    func_8003B9C0(sc);
                }
            }
        } else if (client->unk0x0 == 2) {
            osSendMesg(client->msgQ, sc, OS_MESG_NOBLOCK);
        }
    }
}
#endif

/**
 * __scHandleRSP is called when an RSP task signals that it has
 * finished or yielded (at the hosts request)
 */
void __scHandleRSP(OSSched *sc) {
    OSScTask *t, *sp = 0, *dp = 0;
    s32 state;

    t = sc->curRSPTask;
    sc->curRSPTask = 0;

    if (t->list.t.type == M_AUDTASK) {
        countRegB = osGetCount();

        floatTimer3 = ((countRegB - countRegA) * 60.0f) / D_8009A344;
        floatTimer1 += floatTimer3;

        if (floatTimer3 > floatTimer0) {
            floatTimer0 = floatTimer3;
        }

        if ((intTimer0 % 1000 == 1) || (intTimer0 % 1000 == 2)) {
            floatTimer2 = floatTimer1 / 500.0f;
            floatTimer1 = 0.0f;
            floatTimer0 = 0.0f;
        }
    }

    if (t->state & OS_SC_YIELD) {
        if (osSpTaskYielded(&t->list)) {
            t->state |= OS_SC_YIELDED;

            if ((t->flags & OS_SC_TYPE_MASK) == OS_SC_XBUS) {
                // push the task back on the list
                t->next = sc->gfxListHead;
                sc->gfxListHead = t;

                if (sc->gfxListTail == 0) {
                    sc->gfxListTail = t;
                }

                if (t->unk0x60 != 0) { } // ??
            }
        } else {
            t->state &= ~OS_SC_NEEDS_RSP;

            // NOTE: added by permuter, doesn't match without it.
            //       maybe this and the other empty ifs are some kind of asserts
            //       that didn't get fully compiled out?
            if (!sc) { }
        }

        if ((t->flags & OS_SC_TYPE_MASK) != OS_SC_XBUS) { } // ??
    } else {
        t->state &= ~OS_SC_NEEDS_RSP;
        __scTaskComplete(sc, t);
    }

    state = ((sc->curRSPTask == 0) << 1) | (sc->curRDPTask == 0);
    if ((__scSchedule(sc, &sp, &dp, state)) != state) {
        __scExec(sc, sp, dp);
    }
}

/**
 * __scHandleRDP is called when an RDP task signals that it has finished.
 */
void __scHandleRDP(OSSched *sc) {
    OSScTask *t, *sp = 0, *dp = 0;
    s32 state;

    t = sc->curRDPTask;
    sc->curRDPTask = 0;

    t->state &= ~OS_SC_NEEDS_RDP;

    __scTaskComplete(sc, t);

    state = ((sc->curRSPTask == 0) << 1) | (sc->curRDPTask == 0);
    if ((__scSchedule(sc, &sp, &dp, state)) != state) {
        __scExec(sc, sp, dp);
    }
}

/**
 * Checks to see if the graphics task is able to run based on the current state of the RCP.
 */
OSScTask *__scTaskReady(OSScTask *t) {
    if (t) {
        // If there is a pending swap bail out til later (next retrace).
        if (osViGetCurrentFramebuffer() != osViGetNextFramebuffer()) {
            return NULL;
        }

        return t;
    }

    return NULL;
}

s32 __scTaskComplete(OSSched *sc, OSScTask *t) {
    if ((t->state & OS_SC_RCP_MASK) == 0) { /* none of the needs bits set */
        if (t->msgQ) {
            if (t->flags & OS_SC_LAST_TASK) {
                if ((u32)sc->frameCount < 2u) {
                    sc->unkTask = t;
                    return 1;
                }

                if (t->unk0x68 || t->msg) {
                    osSendMesg(t->msgQ, t->msg, OS_MESG_BLOCK);
                } else {
                    osSendMesg(t->msgQ, (OSMesg)&D_800918D0, OS_MESG_BLOCK);
                }

                sc->frameCount = 0;
                return 1;
            }

            if (t->unk0x68 || t->msg) {
                osSendMesg(t->msgQ, t->msg, OS_MESG_BLOCK);
                return 1;
            }

            osSendMesg(t->msgQ, (OSMesg)&D_800918D0, OS_MESG_BLOCK);
        }

        return 1;
    }

    return 0;
}

/**
 * Place task on either the audio or graphics queue.
 */
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

void __scExec(OSSched *sc, OSScTask *sp, OSScTask *dp) {
    if (sp) {
        if (sp->list.t.type == M_AUDTASK) {
            osWritebackDCacheAll(); // flush the cache
            countRegA = osGetCount();
        }

        sp->state &= ~(OS_SC_YIELD | OS_SC_YIELDED);

        osSpTaskLoad(&sp->list);
        osSpTaskStartGo(&sp->list);

        D_800918F4 = 0;
        D_800918F8 = 0;

        sc->curRSPTask = sp;

        if (sp == dp) {
            sc->curRDPTask = dp;
        }
    }

    if (dp && (dp != sp)) {
        osDpSetNextBuffer(dp->list.t.output_buff, *dp->list.t.output_buff_size);

        sc->curRDPTask = dp;
    }
}

void __scYield(OSSched *sc) {
    if (sc->curRSPTask->list.t.type == M_GFXTASK) {
        sc->curRSPTask->state |= OS_SC_YIELD;

        D_800B4988 = osGetTime();

        osSpTaskYield();
    }
}

/*
 * Schedules the tasks to be run on the RCP.
 */
s32 __scSchedule(OSSched *sc, OSScTask **sp, OSScTask **dp, s32 availRCP) {
    s32 avail = availRCP;
    OSScTask *gfx = sc->gfxListHead;
    OSScTask *audio = sc->audioListHead;

    if (sc->doAudio && (avail & OS_SC_SP)) {
        if (gfx && (gfx->flags & OS_SC_PARALLEL_TASK)) {
            *sp = gfx;
            avail &= ~OS_SC_SP;
        } else {
            *sp = audio;
            avail &= ~OS_SC_SP;
            sc->doAudio = 0;
            sc->audioListHead = sc->audioListHead->next;

            if (sc->audioListHead == NULL) {
                sc->audioListTail = NULL;
            }
        }
    } else {
        if (__scTaskReady(gfx)) {
            switch (gfx->flags & OS_SC_TYPE_MASK) {
                case (OS_SC_XBUS):
                    if (gfx->state & OS_SC_YIELDED) {
                        /* can hit this if RDP finishes at yield req */
                        /* assert(gfx->state & OS_SC_DP); */

                        if (avail & OS_SC_SP) {   /* if SP is available */
                            *sp = gfx;
                            avail &= ~OS_SC_SP;

                            if (gfx->state & OS_SC_DP) {  /* if it needs DP */
                                *dp = gfx;
                                avail &= ~OS_SC_DP;

                                if ((avail & OS_SC_DP) == 0) { }
                            }

                            sc->gfxListHead = sc->gfxListHead->next;
                            if (sc->gfxListHead == NULL) {
                                sc->gfxListTail = NULL;
                            }
                        }
                    } else {
                        if (avail == (OS_SC_SP | OS_SC_DP)) {
                            *sp = *dp = gfx;
                            avail &= ~(OS_SC_SP | OS_SC_DP);
                            sc->gfxListHead = sc->gfxListHead->next;

                            if (sc->gfxListHead == NULL) {
                                sc->gfxListTail = NULL;
                            }
                        }
                    }

                    break;

                case (OS_SC_DRAM):
                case (OS_SC_DP_DRAM):
                case (OS_SC_DP_XBUS):
                    if (gfx->state & OS_SC_SP) {  /* if needs SP */
                        if (avail & OS_SC_SP) {   /* if SP is available */
                            *sp = gfx;
                            avail &= ~OS_SC_SP;
                        }
                    }

                    if (gfx->state & OS_SC_DP) {   /* if needs DP */
                        if (avail & OS_SC_DP) {        /* if DP available */
                            *dp = gfx;
                            avail &= ~OS_SC_DP;
                            sc->gfxListHead = sc->gfxListHead->next;

                            if (sc->gfxListHead == NULL) {
                                sc->gfxListTail = NULL;
                            }
                        }
                    }
                    break;

                case (OS_SC_SP_DRAM):
                case (OS_SC_SP_XBUS):
                default:
                    break;
            }
        }
    }

    if (avail != availRCP) {
        avail = __scSchedule(sc, sp, dp, avail);
    }

    return avail;
}
