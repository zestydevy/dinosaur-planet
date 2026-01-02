#include "PR/os_internal.h"
#include "sys/audio.h"
#include "sys/dl_debug.h"
#include "sys/gfx/gx.h"
#include "sys/main.h"
#include "sys/print.h"
#include "sys/rsp_segment.h"
#include "sys/thread.h"
#include "sys/scheduler.h"
#include "functions.h"

GfxTaskMesg D_800918D0 = {0, 0};
GfxTaskMesg D_800918D8 = {0, 8};
f32 gAudTaskTimer0 = 0.0f;
f32 gAudTaskTimer1 = 0.0f;
f32 gAudTaskTimer2 = 0.0f;
f32 gAudTaskTimer3 = 0.0f;
s32 gRetraceCounter32 = 0;
s32 gCurRSPTaskCounter = 0;
s32 gCurRDPTaskCounter = 0;
u64 gRetraceCounter64 = 0;

// -------- .bss start 800b4980 -------- //
s32 gCurRSPTaskIsSet;
s32 gCurRDPTaskIsSet;
OSTime gLastGfxYield;
u32 gRSPAudTaskFlushTime;
u32 gRSPAudTaskDoneTime;
// -------- .bss end 800b49a0 -------- //

/***********************************************************************
 * Scheduler API
 **********************************************************************/
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
    s->retraceMsg.type  = OS_SC_RETRACE_MSG; // sent to apps
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

/*
 * Add a client to the scheduler.  Clients receive messages at retrace time
 */
void osScAddClient(OSSched *s, OSScClient *c, OSMesgQueue *msgQ, u8 id) {
    OSIntMask mask;

    mask = osSetIntMask(OS_IM_NONE);

    c->msgQ = msgQ;
    c->next = s->clientList;
    c->id = id;
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

OSMesgQueue *osScGetInterruptQ(OSSched *s) {
    return &s->interruptQ;
}

void osScGetAudioSPStats(f32 *timer0, f32 *timer1, f32 *timer2) {
    f32 temp = 1.15f;

    *timer0 = gAudTaskTimer0 * temp;
    *timer1 = gAudTaskTimer2 * temp;
    *timer2 = gAudTaskTimer3 * temp;
}

/***********************************************************************
 * Scheduler implementation
 **********************************************************************/
void __scMain(void *arg) {
    OSMesg msg = NULL;
    s32 state;
    OSSched *sc = (OSSched*)arg;
    OSScClient *client;
    OSScTask *sp = NULL, *dp = NULL;

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

            case UNK_MSG:
                sc_signal_do_audio(sc);
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

void sc_signal_do_audio(OSSched *sc) {
    s32 state;
    OSScTask *sp = NULL, *dp = NULL;

    if (sc->audioListHead) {
        sc->doAudio = TRUE;
    }

    if (sc->doAudio && sc->curRSPTask != NULL) {
        __scYield(sc);
        return;
    }

    state = ((sc->curRSPTask == 0) << 1) | (sc->curRDPTask == 0);
    if ((__scSchedule(sc, &sp, &dp, state)) != state) {
        __scExec(sc, sp, dp);
    }
}

char *sc_get_task_type_string(u32 taskType) {
    switch (taskType) {
        case OS_SC_TASK_AUDIO:
            return "(Audio task)";
        case OS_SC_TASK_GAME:
            return "(Game task)";
        case OS_SC_TASK_DI:
            return "(DI task)\n";
        case OS_SC_TASK_DI_BENCHMARK_TEST:
            return "(DI benchmark test)\n";
        default:
            return "(Unknown task type)\n";
    }
}

// the following are probably part of some_dummied_task_func
static const char str_8009a078[] = "\nRCP TASK INFO\n";
static const char str_8009a088[] = "-------------\n";
static const char str_8009a098[] = "\ttype\t\t= %u\n";
static const char str_8009a0a8[] = "\tflags\t\t= %u\n";
static const char str_8009a0b8[] = "\tucode_boot\t\t= %p\n";
static const char str_8009a0cc[] = "\tucode_boot_size\t\t= %u\n";
static const char str_8009a0e4[] = "\tucode\t\t= %p\n";
static const char str_8009a0f4[] = "\tucode_size\t\t= %u\n";
static const char str_8009a108[] = "\tucode_data\t\t= %p\n";
static const char str_8009a11c[] = "\tucode_data_size\t\t= %u\n";
static const char str_8009a134[] = "\toutput_buff\t\t= %p\n";
static const char str_8009a148[] = "\toutput_buff_size\t\t= %u\n";
static const char str_8009a164[] = "\tdata_ptr\t\t= %p\n";
static const char str_8009a178[] = "\tdata_size\t\t= %u\n";
static const char str_8009a18c[] = "running task: %08x %08x\n";
static const char str_8009a1a8[] = "Crash gfx ptr = %x\n";
static const char str_8009a1bc[] = "Surrounding traces:\n";
static const char str_8009a1d4[] = "Previous:\t%s:%5d [%d] gfx=%x\n";
static const char str_8009a1f4[] = "Next:\t\t%s:%5d [%d] gfx=%x\n";
static const char str_8009a210[] = "No traces available\n";
static const char str_8009a228[] = "scheduler: Looks like the SP has crashed %s";
static const char str_8009a254[] = "scheduler: Looks like the DP has crashed %s";

void some_dummied_task_func(OSScTask *task) { }

Gfx *sc_func_8003BAD0(OSSched *sc, 
    char **retFile, u32 *retunkC, s32 *retunk10,
    char **retFile_2, u32 *retunkC_2, s32 *retunk10_2) {

    OSMesg queueBuffer[8];
    OSMesg queueMsg;
    OSMesgQueue queue;
    s64 tempCmds[2];
    Gfx *displayListPtr;
    OSTask *task;
    s32 dldi_unk10_2;
    s32 dldi_unk10;
    char *dldi_file_2;
    char *dldi_file;
    s32 done;
    s32 numRetraces;
    s32 gotRdpDone;
    s32 cmdIndex;
    u32 dldi_unkC_2;
    u32 dldi_unkC;
    u32 dldi_unk4_2;
    u32 dldi_unk4;

    queueMsg = 0;
    done = FALSE;

    // Stop audio thread
    stop_audio_thread();

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
            &dldi_unk4, &dldi_file, &dldi_unkC, &dldi_unk10,
            &dldi_unk4_2, &dldi_file_2, &dldi_unkC_2, &dldi_unk10_2);
        
        if (dldi_file_2 != NULL || dldi_file != NULL) {
            if (dldi_file != NULL) {
                *retFile = dldi_file;
                *retunkC = dldi_unkC;
                *retunk10 = dldi_unk10;
            }
            if (dldi_file_2 != NULL) {
                *retFile_2 = dldi_file_2;
                *retunkC_2 = dldi_unkC_2;
                *retunk10_2 = dldi_unk10_2;
            }
        }
    }

    return displayListPtr;
}

void __scHandleRetrace(OSSched *sc) {
    OSScTask *rspTask = NULL;
    OSScClient *client;
    s32 state;
    OSScTask *sp = NULL;
    OSScTask *dp = NULL;
    u8 setRSPTaskToNull = FALSE;
    u8 setRDPTaskToNull = FALSE;
    Gfx *displayListPtr1;
    Gfx *displayListPtr2;
    s32 sp_dldi_unk10;
    s32 sp_dldi_unk10_2;
    s32 dp_dldi_unk10;
    s32 dp_dldi_unk10_2;
    char *sp_dldi_file;
    char *sp_dldi_file_2;
    char *dp_dldi_file;
    char *dp_dldi_file_2;
    u32 sp_dldi_unkC;
    u32 sp_dldi_unkC_2;
    u32 dp_dldi_unkC;
    u32 dp_dldi_unkC_2;
    OSMesg queueBuffer[8];
    OSMesgQueue queue;
    OSScTask *unkTask2;
    u64 *taskDataPtr;
    OSScTask *unkTask;

    if (sc->curRSPTask) {
        gCurRSPTaskCounter++;
    }

    if (sc->curRDPTask) {
        gCurRDPTaskCounter++;
    }

    displayListPtr1 = NULL;
    displayListPtr2 = NULL;

    if ((gCurRSPTaskCounter > 10) && (sc->curRSPTask)) {
        if (gCurRSPTaskIsSet) {
            sc_get_task_type_string(sc->curRSPTask->taskType);
            some_dummied_task_func(sc->curRSPTask);

            if (sc->curRSPTask->list.t.type == OS_SC_TASK_AUDIO) {
                displayListPtr1 = sc_func_8003BAD0(sc, 
                    &sp_dldi_file, &sp_dldi_unkC, &sp_dldi_unk10, 
                    &sp_dldi_file_2, &sp_dldi_unkC_2, &sp_dldi_unk10_2);
            }

            gCurRSPTaskIsSet = 0;
        }

        gCurRSPTaskCounter = 0;
        setRSPTaskToNull = TRUE;

        __osSpSetStatus(SP_SET_HALT | SP_CLR_INTR_BREAK | SP_CLR_SIG0 |
            SP_CLR_SIG1 | SP_CLR_SIG2 | SP_CLR_SIG3 | SP_CLR_SIG4 |
            SP_CLR_SIG5 | SP_CLR_SIG6 | SP_CLR_SIG7);
    } else if (sc->curRSPTask) {
        gCurRSPTaskIsSet = TRUE;
    }

    if ((gCurRDPTaskCounter > 10) && (sc->curRDPTask)) {
        if (sc->curRDPTask->unk68 == 0) {
            osSendMesg(sc->curRDPTask->msgQ, (OSMesg)&D_800918D8, OS_MESG_BLOCK);
        }

        if (gCurRDPTaskIsSet) {
            sc_get_task_type_string(sc->curRDPTask->taskType);
            some_dummied_task_func(sc->curRDPTask);

            if (sc->curRDPTask->list.t.type == OS_SC_TASK_AUDIO) {
                displayListPtr2 = sc_func_8003BAD0(sc, 
                    &dp_dldi_file, &dp_dldi_unkC, &dp_dldi_unk10, 
                    &dp_dldi_file_2, &dp_dldi_unkC_2, &dp_dldi_unk10_2);
            }

            gCurRDPTaskIsSet = FALSE;
        }

        setRDPTaskToNull = TRUE;
        sc->frameCount = 0;
        gCurRDPTaskCounter = 0;

        __osSpSetStatus(SP_SET_HALT | SP_CLR_INTR_BREAK | SP_CLR_SIG0 |
            SP_CLR_SIG1 | SP_CLR_SIG2 | SP_CLR_SIG3 | SP_CLR_SIG4 |
            SP_CLR_SIG5 | SP_CLR_SIG6 | SP_CLR_SIG7);

        osDpSetStatus(DPC_SET_XBUS_DMEM_DMA | DPC_CLR_FREEZE | DPC_CLR_FLUSH |
            DPC_CLR_TMEM_CTR | DPC_CLR_PIPE_CTR | DPC_CLR_CMD_CTR);
    } else if (sc->curRDPTask) {
        gCurRDPTaskIsSet = TRUE;
    }

    if (displayListPtr1 != NULL || displayListPtr2 != NULL) {
        osCreateMesgQueue(&queue, queueBuffer, 8);
        
        osSetEventMesg(OS_EVENT_SP, &queue, (OSMesg)RSP_DONE_MSG);
        osSetEventMesg(OS_EVENT_DP, &queue, (OSMesg)RDP_DONE_MSG);
        osViSetEvent(&queue, (OSMesg)VIDEO_MSG, /*retraceCount*/1);

        unkTask2 = sc->curRSPTask != NULL ? sc->curRSPTask : sc->curRDPTask;
        taskDataPtr = unkTask2->list.t.data_ptr;

        rsp_segment((Gfx**)&taskDataPtr, SEGMENT_MAIN, (void*)0);
        rsp_segment((Gfx**)&taskDataPtr, SEGMENT_FRAMEBUFFER, gFramebufferCurrent);
        rsp_segment((Gfx**)&taskDataPtr, SEGMENT_ZBUFFER, D_800BCCB4);
        rsp_segment((Gfx**)&taskDataPtr, SEGMENT_4, gFramebufferNext - 0x280);

        diPrintfSetBG(0, 0, 0, 128);

        if (displayListPtr1 != NULL) {
            diPrintfSetXY(30, 30);
            diPrintf("SP CRASHED, gfx=%x\n", displayListPtr1);

            if (sp_dldi_file != NULL || sp_dldi_file_2 != NULL) {
                if (sp_dldi_file != NULL) {
                    diPrintfSetXY(30, 40);
                    diPrintf("TRACE:  %s:%d    gfx=%x\n", sp_dldi_file, sp_dldi_unkC, sp_dldi_unk10);
                }

                if (sp_dldi_file_2 != NULL) {
                    diPrintfSetXY(30, 50);
                    diPrintf("TRACE:  %s:%d    gfx=%x\n", sp_dldi_file_2, sp_dldi_unkC_2, sp_dldi_unk10_2);
                }
            }
        }

        if (displayListPtr2 != NULL) {
            diPrintfSetXY(30, 70);
            diPrintf("DP CRASHED, gfx=%x\n", displayListPtr2);

            if (sp_dldi_file != NULL || sp_dldi_file_2 != NULL) {
                if (sp_dldi_file != NULL) {
                    diPrintfSetXY(30, 80);
                    diPrintf("TRACE:  %s:%5d    gfx=%x\n", sp_dldi_file, sp_dldi_unkC, sp_dldi_unk10);
                }

                if (sp_dldi_file_2 != NULL) {
                    diPrintfSetXY(30, 90);
                    diPrintf("TRACE:  %s:%5d    gfx=%x\n", sp_dldi_file_2, sp_dldi_unkC_2, sp_dldi_unk10_2);
                }
            }
        }

        displayListPtr1 = NULL;
        displayListPtr2 = NULL;

        diPrintfSetXY(30, 110);
        diPrintf("CODE: Version %s  %s  %s\n", 
            gGameBuildVersion, gGameBuildDateTime, gGameBuildName);

        diPrintfSetXY(30, 120);
        diPrintf("DB:   %s\n", gDBVersion);

        diPrintfAll((Gfx**)&taskDataPtr);

        __osSpSetStatus(SP_SET_HALT | SP_CLR_INTR_BREAK | 
            SP_CLR_SIG0 | SP_CLR_SIG1 | SP_CLR_SIG2 | SP_CLR_SIG3 |
            SP_CLR_SIG4 |  SP_CLR_SIG5 | SP_CLR_SIG6 | SP_CLR_SIG7);
        osDpSetStatus(DPC_SET_XBUS_DMEM_DMA | 
            DPC_CLR_FREEZE |  DPC_CLR_FLUSH | DPC_CLR_TMEM_CTR | 
            DPC_CLR_PIPE_CTR | DPC_CLR_CMD_CTR);

        gDPFullSync(taskDataPtr++);
        gSPEndDisplayList(taskDataPtr++);

        osWritebackDCacheAll();

        osSpTaskLoad(&unkTask2->list);
        osSpTaskStartGo(&unkTask2->list);

        while (TRUE) {}
    }

    if (setRSPTaskToNull) {
        sc->curRSPTask = NULL;
    }

    if (setRDPTaskToNull) {
        sc->curRDPTask = NULL;
    }

    // Read the task command queue and schedule tasks
    while (osRecvMesg(&sc->cmdQ, (OSMesg *)&rspTask, OS_MESG_NOBLOCK) != -1)
        __scAppendList(sc, rspTask);

    state = ((sc->curRSPTask == NULL) << 1) | (sc->curRDPTask == NULL);
    if (__scSchedule(sc, &sp, &dp, state) != state)
        __scExec(sc, sp, dp);

    gRetraceCounter64++;
    gRetraceCounter32++;
    sc->frameCount++;

    if ((sc->unkTask) && (sc->frameCount >= 2)) {
        unkTask = sc->unkTask;
        if (unkTask->msgQ) {
            if ((unkTask->unk68) || (unkTask->msg)) {
                osSendMesg(unkTask->msgQ, unkTask->msg, OS_MESG_BLOCK);
            } else {
                osSendMesg(unkTask->msgQ, (OSMesg)&D_800918D0, OS_MESG_BLOCK);
            }
        }
        sc->frameCount = 0;
        sc->unkTask = NULL;
    }

    for (client = sc->clientList; client != NULL; client = client->next) {
        if (client->id == OS_SC_ID_AUDIO) {
            // Only run this on even calls to this function
            if (gRetraceCounter64 % 2 == 0) {
                osSendMesg(client->msgQ, sc, OS_MESG_NOBLOCK);
                if (sc->audioListHead) {
                    sc_signal_do_audio(sc);
                }
            }
        } else if (client->id == OS_SC_ID_VIDEO) {
            osSendMesg(client->msgQ, sc, OS_MESG_NOBLOCK);
        }
    }
}

/*
 * __scHandleRSP is called when an RSP task signals that it has
 * finished or yielded (at the hosts request)
 */
void __scHandleRSP(OSSched *sc) {
    OSScTask *t, *sp = NULL, *dp = NULL;
    s32 state;

    t = sc->curRSPTask;
    sc->curRSPTask = NULL;

    if (t->list.t.type == M_AUDTASK) {
        gRSPAudTaskDoneTime = osGetCount();

        gAudTaskTimer3 = ((gRSPAudTaskDoneTime - gRSPAudTaskFlushTime) * 60.0f) / 468750.0f;
        gAudTaskTimer1 += gAudTaskTimer3;

        if (gAudTaskTimer3 > gAudTaskTimer0) {
            gAudTaskTimer0 = gAudTaskTimer3;
        }

        if ((gRetraceCounter32 % 1000 == 1) || (gRetraceCounter32 % 1000 == 2)) {
            gAudTaskTimer2 = gAudTaskTimer1 / 500.0f;
            gAudTaskTimer1 = 0.0f;
            gAudTaskTimer0 = 0.0f;
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

                if (t->unk60 != 0) { } // ??
            }
        } else {
            t->state &= ~OS_SC_NEEDS_RSP;

            if (!sc) { } // @fake
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

/*
 * __scHandleRDP is called when an RDP task signals that it has
 * finished
 */
void __scHandleRDP(OSSched *sc) {
    OSScTask *t, *sp = NULL, *dp = NULL;
    s32 state;

    t = sc->curRDPTask;
    sc->curRDPTask = NULL;

    t->state &= ~OS_SC_NEEDS_RDP;

    __scTaskComplete(sc, t);

    state = ((sc->curRSPTask == 0) << 1) | (sc->curRDPTask == 0);
    if ((__scSchedule(sc, &sp, &dp, state)) != state) {
        __scExec(sc, sp, dp);
    }
}


/*
 * __scTaskReady checks to see if the graphics task is able to run
 * based on the current state of the RCP.
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

/*
 * __scTaskComplete checks to see if the task is complete (all RCP
 * operations have been performed) and sends the done message to the
 * client if it is.
 */
s32 __scTaskComplete(OSSched *sc, OSScTask *t) {
    if ((t->state & OS_SC_RCP_MASK) == 0) { /* none of the needs bits set */
        if (t->msgQ) {
            if (t->flags & OS_SC_LAST_TASK) {
                if (sc->frameCount < 2) {
                    sc->unkTask = t;
                    return 1;
                }

                if (t->unk68 || t->msg) {
                    osSendMesg(t->msgQ, t->msg, OS_MESG_BLOCK);
                } else {
                    osSendMesg(t->msgQ, (OSMesg)&D_800918D0, OS_MESG_BLOCK);
                }

                sc->frameCount = 0;
                return 1;
            }

            if (t->unk68 || t->msg) {
                osSendMesg(t->msgQ, t->msg, OS_MESG_BLOCK);
                return 1;
            }

            osSendMesg(t->msgQ, (OSMesg)&D_800918D0, OS_MESG_BLOCK);
        }

        return 1;
    }

    return 0;
}

/*
 * Place task on either the audio or graphics queue
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
            gRSPAudTaskFlushTime = osGetCount();
        }

        sp->state &= ~(OS_SC_YIELD | OS_SC_YIELDED);

        osSpTaskLoad(&sp->list);
        osSpTaskStartGo(&sp->list);

        gCurRSPTaskCounter = 0;
        gCurRDPTaskCounter = 0;

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

        gLastGfxYield = osGetTime();

        osSpTaskYield();
    }
}

/*
 * Schedules the tasks to be run on the RCP
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
            sc->doAudio = FALSE;
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
