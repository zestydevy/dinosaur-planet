// Thanks to the libreultra project for most of the matching code here.
// See https://github.com/n64decomp/libreultra/blob/master/src/sched/sched.c

#include "common.h"

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

extern s32 intTimer0;

extern u32 D_800918F4;
extern u32 D_800918F8;

extern f32 D_8009A340;
extern f32 D_8009A344;

OSTime D_800B4988;

extern u32 countRegA;
extern u32 countRegB;

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
        sc->unk0x304 = 1;
    }

    if (sc->unk0x304 != 0 && sc->curRSPTask) {
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

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/func_8003BAD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/scheduler/__scHandleRetrace.s")

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

// TODO: This matches, but has some illogical oddities
s32 __scTaskComplete(OSSched *sc, OSScTask *t) {
    if ((t->state & OS_SC_RCP_MASK) == 0) { /* none of the needs bits set */
        if (t->msgQ) {
            if (t->flags & OS_SC_LAST_TASK) {
                if ((u32)sc->doAudio < 2u) {
                    sc->frameCount = (u32)t; // ??
                    return 1;
                }

                if (t->unk0x68 || t->msg) {
                    osSendMesg(t->msgQ, t->msg, 1);
                } else {
                    osSendMesg(t->msgQ, (OSMesg)&D_800918D0, OS_MESG_BLOCK);
                }

                sc->doAudio = 0;
                return 1;
            }

            if (t->unk0x68 || t->msg) {
                osSendMesg(t->msgQ, t->msg, 1);
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

    if (sc->unk0x304 && (avail & OS_SC_SP)) {
        if (gfx && (gfx->flags & OS_SC_PARALLEL_TASK)) {
            *sp = gfx;
            avail &= ~OS_SC_SP;
        } else {
            *sp = audio;
            avail &= ~OS_SC_SP;
            sc->unk0x304 = 0;
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
