#include "PR/os.h"
#include "sys/main.h"
#include "sys/thread.h"
#include "macros.h"
#include "bss.h"

// .bss

BSS_GLOBAL u64 gIdleThreadStack[IDLE_THREAD_SIZE + 1];
BSS_GLOBAL u64 gMainThreadStack[MAIN_THREAD_SIZE + 1];
BSS_GLOBAL OSThread gIdleThread;
BSS_GLOBAL OSThread gMainThread;

void idle(void * arg);

// official name: boot
void bootproc(void) {
    osInitialize();
    // @bug: The idle thread stack size was shrunk but the call to osCreateThread was not adjusted!
    osCreateThread(&gIdleThread, IDLE_THREAD_ID, &idle, NULL, 
        &gIdleThreadStack[IDLE_THREAD_SIZE + 0x18], IDLE_THREAD_PRIORITY);
    osStartThread(&gIdleThread);
}

void idle(void * arg)
{
    osCreateThread(&gMainThread, MAIN_THREAD_ID, &mainproc, NULL, 
        &gMainThreadStack[MAIN_THREAD_SIZE], MAIN_THREAD_PRIORITY);

    gMainThreadStack[MAIN_THREAD_SIZE] = 0;
    gMainThreadStack[0] = 0;

    osStartThread(&gMainThread);
    osSetThreadPri(NULL, OS_PRIORITY_IDLE);

    while (TRUE) {}
}

// official name: bootCheckStack
void thread_timer_tick(void) {
    gMainThreadStack[MAIN_THREAD_SIZE]++;
    gMainThreadStack[0]++;
    if (gMainThreadStack[MAIN_THREAD_SIZE] != gMainThreadStack[0]) {
        STUBBED_PRINTF("WARNING: Stack overflow/underflow!!!\n");
    }
}

void func_80001178(s32 a0, s32 *a1)
{
    u8  *ptr1;
    s32 *ptr2;
    s32  count;
    u8  *tmp;

    count = 0;
    tmp = (u8 *)a1[2];
    ptr1 = tmp;
    ptr2 = &a1[2];

    while (ptr1) {
        while ((u32)ptr1 < (u32)ptr2[1])
            count += *ptr1++;

        ptr1 = (u8 *)ptr2[2];
        ptr2 += 2;
    }

    if (a1[0] == 1) {
        a1[0] = 2;
        a1[1] = count;
    } else if (a1[0] == 2) {
        // matches with any local var, not just a1
        if (a1)
            ;
    }
}

void func_800011F4(s32 a0, s32 *a1)
{
    *a1 = 1;
    func_80001178(a0, a1);
}

