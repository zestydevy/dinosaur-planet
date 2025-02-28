#ifndef _SYS_SCHEDULER_H
#define _SYS_SCHEDULER_H

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

extern char **D_8008C8F0; // ptr to "1.3623"
extern char **D_8008C8F4; // ptr to "01/12/00 09:19"
extern char **D_8008C8F8; // ptr to "dragon1"
extern char D_8008C8FC[]; // "Version 2.8 14/12/98 15.30 L.Schuneman"

extern s32 gCurRSPTaskIsSet;
extern s32 gCurRDPTaskIsSet;

extern u32 countRegA;
extern u32 countRegB;

void osCreateScheduler(OSSched *s, void *stack, OSPri priority, u8 mode, u8 retraceCount);

/**
 * Add a client to the scheduler. Clients receive messages at retrace time.
 */
void osScAddClient(OSSched *s, OSScClient *c, OSMesgQueue *msgQ, u8 param4);

void osScRemoveClient(OSSched *s, OSScClient *c);

OSMesgQueue *osScGetCmdQ(OSSched *s);

OSMesgQueue *get_sched_interrupt_queue(OSSched *s);

void get_float_timers(f32 *timer0, f32 *timer1, f32 *timer2);

void __scMain(void *arg);

void func_8003B9C0(OSSched *sc);

/**
 * Gets a string for a OSScTask task type.
 */
char *get_task_type_string(u32 taskType);

void some_dummied_task_func(OSScTask *task);

Gfx *func_8003BAD0(OSSched *sc, 
    char **retFile, u32 *retUnk0xc, s32 *retUnk0x10,
    char **retFile_2, u32 *retUnk0xc_2, s32 *retUnk0x10_2);

void __scHandleRetrace(OSSched *sc);

/**
 * __scHandleRSP is called when an RSP task signals that it has
 * finished or yielded (at the hosts request)
 */
void __scHandleRSP(OSSched *sc);

/**
 * __scHandleRDP is called when an RDP task signals that it has finished.
 */
void __scHandleRDP(OSSched *sc);

/**
 * Checks to see if the graphics task is able to run based on the current state of the RCP.
 */
OSScTask *__scTaskReady(OSScTask *t);

s32 __scTaskComplete(OSSched *sc, OSScTask *t);

/**
 * Place task on either the audio or graphics queue.
 */
void __scAppendList(OSSched *s, OSScTask *t);

void __scExec(OSSched *sc, OSScTask *sp, OSScTask *dp);

void __scYield(OSSched *sc);

/*
 * Schedules the tasks to be run on the RCP.
 */
s32 __scSchedule(OSSched *sc, OSScTask **sp, OSScTask **dp, s32 availRCP);

#endif // _SYS_SCHEDULER_H
