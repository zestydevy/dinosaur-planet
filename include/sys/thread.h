/** OS threads
 */
#ifndef _SYS_THREAD_H
#define _SYS_THREAD_H

#include "ultra64.h"
#include <PR/sched.h>

#define IDLE_THREAD_SIZE (0x800 / sizeof(u64))
#define IDLE_THREAD_ID 1
#define IDLE_THREAD_PRIORITY OS_PRIORITY_IDLE

#define MAIN_THREAD_SIZE 1024
#define MAIN_THREAD_ID 3
#define MAIN_THREAD_PRIORITY 10

#define OS_SCHEDULER_THREAD_ID 5
#define CONTROLLER_THREAD_ID 98

#define CRASH_THREAD_ID 100
#define CRASH_THREAD_PRIORITY 0x80

//why are these not part of libultra?
typedef struct {
    /* 0x000 */ s32 regs[50];
    /* 0x0C8 */ char unk_0C8[4];
    /* 0x0CC */ s32 gp;
    /* 0x0D0 */ char unk_0D0[4];
    /* 0x0D4 */ s32 sp_upper;
    /* 0x0D8 */ void* sp;
    /* 0x0DC */ s32 s8_up;
    /* 0x0E0 */ s32 s8;
    /* 0x0E4 */ u32 ra_up;
    /* 0x0E8 */ void* ra;
    /* 0x0EC */ s32 lo_up;
    /* 0x0F0 */ s32 lo;
    /* 0x0F4 */ s32 hi_up;
    /* 0x0F8 */ u32 hi;
    /* 0x0FC */ u32 sr;
    /* 0x100 */ void* pc;
    /* 0x104 */ u32 cause;
    /* 0x108 */ u32 badvaddr;
    /* 0x10C */ u32 rcp;
    /* 0x110 */ u32 fpcsr;
    /* 0x114 */ f32 fregs[32];
} OSThreadContext; // size = 0x194

struct OSThread {
    /* 0x000 */ OSThread* next;
    /* 0x004 */ s32 priority;
    /* 0x008 */ OSThread** queue;
    /* 0x00C */ OSThread* tlnext;
    /* 0x010 */ u16 state;
    /* 0x012 */ u16 flags;
    /* 0x014 */ s32 id;
    /* 0x018 */ s32 fp;
    /* 0x01C */ OSThreadContext context;
    /* 0x1B0 */ char unk_1B0[0x8];
    /* 0x1B8 */ void* stackInfo;
};

extern OSThread* __osRunningThread;
extern OSThread* __osRunQueue;
// this needs double checking. its address is within gMainThreadStack....
extern u8 gIdleThreadStack[IDLE_THREAD_SIZE];
extern OSThread gIdleThread;
extern OSThread gMainThread;
extern u64 gMainThreadStack[];        // some sort of data
extern OSSched *osscheduler_;
extern s8 ossceduler_stack;

/**
 * @returns The address of s->interruptQ.
 */
OSMesgQueue *get_sched_interrupt_queue(OSSched *s);

#endif //_SYS_THREAD_H
