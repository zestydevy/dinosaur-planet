#ifndef _SYS_SCHEDULER_H
#define _SYS_SCHEDULER_H

#include "PR/ultratypes.h"
#include "PR/sched.h"
#include "PR/os.h"

/*
 * private typedefs and defines
 */
#define UNK_MSG         99
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

typedef struct {
    s32 unk0;
    s32 unk4;
} GfxTaskMesg;

void osCreateScheduler(OSSched *s, void *stack, OSPri priority, u8 mode, u8 retraceCount);

void osScRemoveClient(OSSched *s, OSScClient *c);

OSMesgQueue *osScGetCmdQ(OSSched *s);

OSMesgQueue *osScGetInterruptQ(OSSched *s);

void osScGetAudioSPStats(f32 *timer0, f32 *timer1, f32 *timer2);

void __scMain(void *arg);

void sc_signal_do_audio(OSSched *sc);

/**
 * Gets a string for a OSScTask task type.
 */
char *sc_get_task_type_string(u32 taskType);

void some_dummied_task_func(OSScTask *task);

Gfx *sc_func_8003BAD0(OSSched *sc, 
    char **retFile, u32 *retunkC, s32 *retunk10,
    char **retFile_2, u32 *retunkC_2, s32 *retunk10_2);

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
