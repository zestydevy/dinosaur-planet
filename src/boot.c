#include "PR/os.h"
#include "sys/main.h"
#include "sys/thread.h"
#include "macros.h"

/* -------- .bss start 800a3d70 -------- */
u64 gIdleThreadStack[STACKSIZE(IDLE_THREAD_SIZE) + 1];
u64 gMainThreadStack[STACKSIZE(MAIN_THREAD_SIZE) + 1];
OSThread gIdleThread;
OSThread gMainThread;
u8 D_800A6220[0x50];
/* -------- .bss end 800a6270 -------- */

void idle(void * arg);

// official name: boot
void bootproc(void) {
    osInitialize();
    // @bug: The idle thread stack size was shrunk but the call to osCreateThread was not adjusted!
    osCreateThread(&gIdleThread, IDLE_THREAD_ID, &idle, NULL, 
        &gIdleThreadStack[STACKSIZE(IDLE_THREAD_SIZE) + 0x18], IDLE_THREAD_PRIORITY);
    osStartThread(&gIdleThread);
}

void idle(void * arg)
{
    osCreateThread(&gMainThread, MAIN_THREAD_ID, &mainproc, NULL, 
        &gMainThreadStack[STACKSIZE(MAIN_THREAD_SIZE)], MAIN_THREAD_PRIORITY);

    gMainThreadStack[STACKSIZE(MAIN_THREAD_SIZE)] = 0;
    gMainThreadStack[0] = 0;

    osStartThread(&gMainThread);
    osSetThreadPri(NULL, OS_PRIORITY_IDLE);

    while (TRUE) {}
}

// official name: bootCheckStack
void thread_timer_tick(void) {
    gMainThreadStack[STACKSIZE(MAIN_THREAD_SIZE)]++;
    gMainThreadStack[0]++;
    if (gMainThreadStack[STACKSIZE(MAIN_THREAD_SIZE)] != gMainThreadStack[0]) {
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

