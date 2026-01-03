#include "PR/os_internal.h"
#include "macros.h"
#include "sys/crash.h"
#include "libc/stdarg.h"
#include "common.h"

typedef struct {
    u32 code1;
    u32 code2;
    char *text;
} CrashErrString;

char D_80093200[] = { 
    0x0c, 0x0c, 0x0c, 0x00, 0x0c, 0x33, 0x33, 0x00, 0x00, 0x00, 0x26, 0x3f, 0x26, 0x3f, 0x26, 0x2c, 
    0x0e, 0x2e, 0x2c, 0x0e, 0x33, 0x28, 0x1d, 0x0a, 0x33, 0x1d, 0x33, 0x2d, 0x33, 0x3e, 0x0c, 0x0c, 
    0x00, 0x00, 0x00, 0x0e, 0x03, 0x03, 0x03, 0x0e, 0x2c, 0x30, 0x30, 0x30, 0x2c, 0x00, 0x0c, 0x3f, 
    0x1d, 0x37, 0x00, 0x0c, 0x3f, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x03, 0x00, 0x00, 0x3f, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x04, 0x0c, 0x30, 0x38, 0x19, 0x0b, 0x03, 0x2e, 0x33, 0x33, 0x33, 0x2e, 
    0x0c, 0x0f, 0x0c, 0x0c, 0x3f, 0x2f, 0x30, 0x2e, 0x03, 0x3f, 0x3f, 0x30, 0x3f, 0x30, 0x3f, 0x33, 
    0x33, 0x3f, 0x30, 0x30, 0x3f, 0x03, 0x2e, 0x30, 0x2f, 0x3e, 0x03, 0x2f, 0x33, 0x2f, 0x3f, 0x30, 
    0x30, 0x30, 0x30, 0x2e, 0x33, 0x2e, 0x33, 0x2e, 0x3e, 0x33, 0x3e, 0x30, 0x2f, 0x00, 0x0c, 0x00, 
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x1d, 0x33, 0x3f, 0x33, 0x33, 0x0f, 0x33, 0x0f, 0x33, 0x0f, 0x1d, 0x33, 0x03, 0x33, 0x1d, 0x0f, 
    0x33, 0x33, 0x33, 0x0f, 0x3f, 0x03, 0x1f, 0x03, 0x3f, 0x3f, 0x03, 0x1f, 0x03, 0x03, 0x2e, 0x03, 
    0x33, 0x33, 0x2e, 0x33, 0x33, 0x3f, 0x33, 0x33, 0x3f, 0x0c, 0x0c, 0x0c, 0x3f, 0x3f, 0x30, 0x33, 
    0x33, 0x2e, 0x33, 0x33, 0x1f, 0x33, 0x33, 0x03, 0x03, 0x03, 0x03, 0x3f, 0x37, 0x3f, 0x37, 0x33, 
    0x33, 0x37, 0x3b, 0x3f, 0x3b, 0x37, 0x0c, 0x33, 0x33, 0x33, 0x0c, 0x1f, 0x33, 0x1f, 0x03, 0x03, 
    0x2e, 0x33, 0x33, 0x37, 0x3e, 0x0f, 0x33, 0x1f, 0x33, 0x33, 0x2e, 0x07, 0x2e, 0x34, 0x2e, 0x3f, 
    0x0c, 0x0c, 0x0c, 0x0c, 0x33, 0x33, 0x33, 0x33, 0x2e, 0x33, 0x33, 0x37, 0x2a, 0x0c, 0x33, 0x33, 
    0x37, 0x3f, 0x37, 0x33, 0x33, 0x1d, 0x33, 0x33, 0x33, 0x33, 0x2e, 0x0c, 0x0c, 0x3f, 0x24, 0x19, 
    0x06, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x1d, 0x33, 0x3f, 0x33, 0x33, 0x0f, 0x33, 0x0f, 0x33, 0x0f, 0x1d, 0x33, 0x03, 0x33, 0x1d, 0x0f, 
    0x33, 0x33, 0x33, 0x0f, 0x3f, 0x03, 0x1f, 0x03, 0x3f, 0x3f, 0x03, 0x1f, 0x03, 0x03, 0x2e, 0x03, 
    0x33, 0x33, 0x2e, 0x33, 0x33, 0x3f, 0x33, 0x33, 0x3f, 0x0c, 0x0c, 0x0c, 0x3f, 0x3f, 0x30, 0x33, 
    0x33, 0x2e, 0x33, 0x33, 0x1f, 0x33, 0x33, 0x03, 0x03, 0x03, 0x03, 0x3f, 0x37, 0x3f, 0x37, 0x33, 
    0x33, 0x37, 0x3b, 0x3f, 0x3b, 0x37, 0x0c, 0x33, 0x33, 0x33, 0x0c, 0x1f, 0x33, 0x1f, 0x03, 0x03, 
    0x2e, 0x33, 0x33, 0x37, 0x3e, 0x0f, 0x33, 0x1f, 0x33, 0x33, 0x2e, 0x07, 0x2e, 0x34, 0x2e, 0x3f, 
    0x0c, 0x0c, 0x0c, 0x0c, 0x33, 0x33, 0x33, 0x33, 0x2e, 0x33, 0x33, 0x37, 0x2a, 0x0c, 0x33, 0x33, 
    0x37, 0x3f, 0x37, 0x33, 0x33, 0x1d, 0x33, 0x33, 0x33, 0x33, 0x2e, 0x0c, 0x0c, 0x3f, 0x24, 0x19, 
    0x06, 0x3f
};
u16 D_800933C4[] = { 
    0x0000, 0x5294, 0xa528, 0xfffe, 
    0x0000, 0x0014, 0x0028, 0x003e, 
    0x0000, 0x0294, 0x0528, 0x07fe, 
    0x0000, 0x0280, 0x0500, 0x07c0, 
    0x0000, 0x5280, 0xa500, 0xffc0, 
    0x0000, 0x5000, 0xa000, 0xf800, 
    0x0000, 0x5014, 0xa028, 0xf83e, 
    0x0000, 0x0000, 0x0000, 0x0000
};
CrashErrString errStringArray_cause[] = {
    { 0x80000000, 0x80000000, "BD" },
    { 0x00008000, 0x00008000, "IP8" },
    { 0x00004000, 0x00004000, "IP7" },
    { 0x00002000, 0x00002000, "IP6" },
    { 0x00001000, 0x00001000, "IP5" },
    { 0x00000800, 0x00000800, "IP4" },
    { 0x00000400, 0x00000400, "IP3" },
    { 0x00000200, 0x00000200, "IP2" },
    { 0x00000100, 0x00000100, "IP1" },
    { 0x0000007C, 0x00000000, "Interrupt" },
    { 0x0000007C, 0x00000004, "TLB modify ex" },
    { 0x0000007C, 0x00000008, "TLB load/fetch ex" },
    { 0x0000007C, 0x0000000C, "TLB store ex" },
    { 0x0000007C, 0x00000010, "Addr load/fetch err" },
    { 0x0000007C, 0x00000014, "Addr store err" },
    { 0x0000007C, 0x00000018, "Bus error fetch ex" },
    { 0x0000007C, 0x0000001C, "Bus error ref ex" },
    { 0x0000007C, 0x00000020, "Syscall ex" },
    { 0x0000007C, 0x00000024, "Breakpoint ex" },
    { 0x0000007C, 0x00000028, "Reserved inst ex" },
    { 0x0000007C, 0x0000002C, "Co-pro unusable ex" },
    { 0x0000007C, 0x00000030, "Arith overflow ex" },
    { 0x0000007C, 0x00000034, "Trap ex" },
    { 0x0000007C, 0x00000038, "Virtual coherency ex" },
    { 0x0000007C, 0x0000003C, "Floating point ex" },
    { 0x0000007C, 0x0000005C, "Watchpoint ex" },
    { 0x0000007C, 0x0000007C, "Virtual coherency ex2" },
    { 0x00000000, 0x00000000, "" },
};
CrashErrString errStringArray_cause2[] = {
    { 0x80000000, 0x80000000, "CU3" },
    { 0x40000000, 0x40000000, "CU2" },
    { 0x20000000, 0x20000000, "CU1" },
    { 0x10000000, 0x10000000, "CU0" },
    { 0x08000000, 0x08000000, "RP" },
    { 0x04000000, 0x04000000, "FR" },
    { 0x02000000, 0x02000000, "RE" },
    { 0x00400000, 0x00400000, "BEV" },
    { 0x00200000, 0x00200000, "TS" },
    { 0x00100000, 0x00100000, "SR" },
    { 0x00040000, 0x00040000, "CH" },
    { 0x00020000, 0x00020000, "CE" },
    { 0x00010000, 0x00010000, "DE" },
    { 0x00008000, 0x00008000, "IM8" },
    { 0x00004000, 0x00004000, "IM7" },
    { 0x00002000, 0x00002000, "IM6" },
    { 0x00001000, 0x00001000, "IM5" },
    { 0x00000800, 0x00000800, "IM4" },
    { 0x00000400, 0x00000400, "IM3" },
    { 0x00000200, 0x00000200, "IM2" },
    { 0x00000100, 0x00000100, "IM1" },
    { 0x00000080, 0x00000080, "KX" },
    { 0x00000040, 0x00000040, "SX" },
    { 0x00000020, 0x00000020, "UX" },
    { 0x00000018, 0x00000010, "USR" },
    { 0x00000018, 0x00000008, "SUP" },
    { 0x00000018, 0x00000000, "KER" },
    { 0x00000004, 0x00000004, "ERL" },
    { 0x00000002, 0x00000002, "EXL" },
    { 0x00000001, 0x00000001, "IE" },
    { 0x00000000, 0x00000000, "" },
};
CrashErrString errStringArray_fpsr[] = {
    { 0x01000000, 0x01000000, "FS" },
    { 0x00800000, 0x00800000, "C" },
    { 0x00020000, 0x00020000, "Unimplemented operation" },
    { 0x00010000, 0x00010000, "Invalid operation" },
    { 0x00008000, 0x00008000, "Division by zero" },
    { 0x00004000, 0x00004000, "Overflow" },
    { 0x00002000, 0x00002000, "Underflow" },
    { 0x00001000, 0x00001000, "Inexact operation" },
    { 0x00000800, 0x00000800, "EV" },
    { 0x00000400, 0x00000400, "EZ" },
    { 0x00000200, 0x00000200, "EO" },
    { 0x00000100, 0x00000100, "EU" },
    { 0x00000080, 0x00000080, "EI" },
    { 0x00000040, 0x00000040, "FV" },
    { 0x00000020, 0x00000020, "FZ" },
    { 0x00000010, 0x00000010, "FO" },
    { 0x00000008, 0x00000008, "FU" },
    { 0x00000004, 0x00000004, "FI" },
    { 0x00000003, 0x00000000, "RN" },
    { 0x00000003, 0x00000001, "RZ" },
    { 0x00000003, 0x00000002, "RP" },
    { 0x00000003, 0x00000003, "RM" },
    { 0x00000000, 0x00000000, "" },
};
char *crash_screen_strings[] = {
    "setup",
    "control",
    "print",
    "update",
    "free"
};
s32 D_800937F0 = 0;
s32 gSomeCrashVideoFlag = 0;
s32 D_800937f8[3] = {0};
u8 gCFileLabelIndex = 0;
u8 gCFileLabelFlag = 0;

// Length of gPiManagerArray
#define PI_MANAGER_ARRAY_LENGTH 5
// Length of gPiManagerEventQueueBuffer
#define PI_MANAGER_EVENT_QUEUE_LENGTH 8
// Length of gPiManagerCmdQueueBuffer
#define PI_MANAGER_CMD_QUEUE_LENGTH 8
// Length of gCFileLabels and gSomeCFileInts
#define C_FILE_LABELS_LENGTH 10

/* -------- .bss start 800beb10 -------- */
u64 gPiManagerThreadStack[STACKSIZE(OS_PIM_STACKSIZE)];
OSThread gPiManagerThread;
s32 gPiManagerArray[PI_MANAGER_ARRAY_LENGTH];
OSMesgQueue gPiManagerEventQueue;
OSMesg gPiManagerEventQueueBuffer[PI_MANAGER_EVENT_QUEUE_LENGTH];
OSMesg gPiManagerCmdQueueBuffer[PI_MANAGER_CMD_QUEUE_LENGTH];
OSMesgQueue gPiManagerCmdQueue;
OSMesgQueue gCrashControllerMesgQueue;
OSContPad gCrashContPadArray1[MAXCONTROLLERS];
OSContPad gCrashContPadArray2[MAXCONTROLLERS];
u16 gCrashButtons[MAXCONTROLLERS];
const char *gCFileLabels[C_FILE_LABELS_LENGTH];
s32 gSomeCFileInts[C_FILE_LABELS_LENGTH];
/* -------- .bss end 800bfe70 -------- */

void get_err_string(s32 x, s32 y, u32 param3, CrashErrString *param4);
void crash_copy_control_inputs();
void crash_print_line(s32 x, s32 y, char *fmt, ...);

/**
 * Sets `gPiManagerArray[index] = value`, unless index is out of range
 * in which case this function does nothing.
 */
void update_pi_manager_array(s32 index, s32 value) {
    if (index >= 0 && index < PI_MANAGER_ARRAY_LENGTH) {
        gPiManagerArray[index] = value;
    }
}

void start_pi_manager_thread() {
    int i;

    osCreateThread(
        /*t*/       &gPiManagerThread,
        /*id*/      -1,
        /*entry*/   &pi_manager_entry,
        /*arg*/     NULL,
        /*sp*/      &gPiManagerThreadStack[STACKSIZE(OS_PIM_STACKSIZE)],
        /*pri*/     OS_PRIORITY_MAX
    );

    osStartThread(&gPiManagerThread);

    for (i = 0; i < PI_MANAGER_ARRAY_LENGTH; ++i) {
        gPiManagerArray[i] = -1;
    }
}

#define PI_OS_EVENT_FAULT 8
#define PI_OS_EVENT_CPU_BREAK 2

void pi_manager_entry(void *arg) {
    OSMesg msg;
    s32 evtFlags;

    evtFlags = 0;

    // Listen for OS_EVENT_FAULT and OS_EVENT_CPU_BREAK
    osCreateMesgQueue(
        /*mq*/      &gPiManagerEventQueue,
        /*msg*/     &gPiManagerEventQueueBuffer[0],
        /*count*/   PI_MANAGER_EVENT_QUEUE_LENGTH
    );

    osSetEventMesg(OS_EVENT_FAULT, &gPiManagerEventQueue, (OSMesg)PI_OS_EVENT_FAULT);
    osSetEventMesg(OS_EVENT_CPU_BREAK, &gPiManagerEventQueue, (OSMesg)PI_OS_EVENT_CPU_BREAK);

    // Start system PI manager thread
    osCreatePiManager(
        /*pri*/         OS_PRIORITY_PIMGR,
        /*cmdQ*/        &gPiManagerCmdQueue,
        /*cmdBuf*/      &gPiManagerCmdQueueBuffer[0],
        /*cmdMsgCnt*/   PI_MANAGER_CMD_QUEUE_LENGTH
    );

    while (TRUE) {
        // Wait for event
        osRecvMesg(&gPiManagerEventQueue, &msg, OS_MESG_BLOCK);

        evtFlags |= (s32)msg;

        // If a CPU break or CPU fault occurred...
        if ((evtFlags & PI_OS_EVENT_FAULT) || (evtFlags & PI_OS_EVENT_CPU_BREAK)) {
            // Only handle a fault once per message, but if a break is received, this should run
            // every time a fault or break is received again regardless of which
            evtFlags &= ~PI_OS_EVENT_FAULT;

            // Stop app threads and do some crash stuff
            stop_active_app_threads();
            check_video_mode_crash_and_clear_framebuffer();
            crash_controller_getter();
        }
    }
}

void stop_active_app_threads() {
    OSThread *thread;

    thread = __osGetActiveQueue();

    while (thread->priority != -1) {
        if (thread->priority > OS_PRIORITY_IDLE &&
            thread->priority <= OS_PRIORITY_APPMAX) {
            osStopThread(thread);
        }

        thread = thread->tlnext;
    }
}

void crash_controller_getter() {
    // Find first faulted/broke active thread
    OSThread *thread = __osGetActiveQueue();

    while (thread->priority != -1) {
        if (thread->priority > OS_PRIORITY_IDLE &&
            ((thread->flags & OS_FLAG_FAULT) || (thread->flags & OS_FLAG_CPU_BREAK))) {
            break;
        }

        thread = thread->tlnext;
    }

    // Get current controller state
    osContGetReadData(&gCrashContPadArray1[0]);

    // TODO: Why isn't this called before osContGetReadData?
    osContStartReadData(&gCrashControllerMesgQueue);

    // Copy gCrashContPadArray1 -> gCrashContPadArray2
    bcopy(
        &gCrashContPadArray1[0],
        &gCrashContPadArray2[0],
        sizeof(OSContPad) * MAXCONTROLLERS
    );

    // Print some crash info
    some_crash_print(&thread, 1, 0);
}

void some_crash_print(OSThread** threads, s32 count, s32 offset) {
    OSThread* thread;
    __OSThreadContext* ctx;
    s32 temp_s1;
    s32 j;
    s32 var_s2;
    u32 raDllStart;
    u32 raDllEnd;
    s32* var_s0;
    u32 pcDllStart;
    u32 pcDllEnd;
    u32 temp_a3;
    u32 var_a3;

    while (1) {
        thread = threads[offset];
        var_s2 = find_executing_dll((u32)thread->context.ra, (void **) &raDllStart, (void **) &raDllEnd);
        temp_s1 = find_executing_dll(thread->context.pc, (void **) &pcDllStart, (void **) &pcDllEnd);
        clear_framebuffer_current();
        D_800937F0 = 6;
        crash_print_line(0xCC, 0xD6, "CPU + DLL INFO");
        D_800937F0 = 0;
        ctx = &thread->context;
        crash_print_line(0x10, 0x18, "fault in thread %d", thread->id);
        D_800937F0 = 3;
        crash_print_line(0x10, 0x22, "epc\t\t\t\t%08x", ctx->pc);
        if (temp_s1 != -1) {
            D_800937F0 = 5;
            var_a3 = thread->context.pc;
            if ((var_a3 >= (u32) pcDllStart) && ((u32) pcDllEnd >= var_a3)) {
                var_a3 -= (u32)pcDllStart;
            }
            crash_print_line(0x10, 0x28, "dll epc\t\t%08x  (in DLL #%d)", var_a3, temp_s1);
            D_800937F0 = 3;
        }
        temp_a3 = ctx->cause;
        if (temp_a3 == -1U) {
            crash_print_line(0x10, 0x2E, "cause\t\tmmAlloc(%d,%8x)\n",  (u32)ctx->a0, (u32)ctx->a1);
        } else {
            crash_print_line(0x10, 0x2E, "cause\t\t\t%08x", temp_a3);
            get_err_string(0xA8, 0x2E, ctx->cause, errStringArray_cause);
        }
        crash_print_line(0x10, 0x34, "sr\t\t\t\t%08x", ctx->sr);
        crash_print_line(0x10, 0x3A, "badvaddr\t%08x", ctx->badvaddr);
        D_800937F0 = 5;
        if (var_s2 != -1) {
            var_a3 = (u32)thread->context.ra;
            if ((var_a3 >= (u32) raDllStart) && ((u32) raDllEnd >= var_a3)) {
                var_a3 -= (u32)raDllStart;
            }
            crash_print_line(0x10, 0x40, "ra\t\t\t\t%08x  (in DLL #%d)", var_a3, var_s2);
            crash_print_line(0x10, 0x4C, "dll start\t%08x", raDllStart);
            crash_print_line(0x10, 0x52, "dll end\t\t%08x", raDllEnd);
        } else {
            D_800937F0 = 3;
            crash_print_line(0x10, 0x40, "ra\t\t\t\t%08x", (u32)ctx->ra);
        }
        D_800937F0 = 4;
        var_s2 = 0x5E;
        for (j = 0; j < 5; j++) {
            if (gPiManagerArray[j] != -1) {
                crash_print_line(0x10, var_s2, "Fault in object: (%s) (%d) ", crash_screen_strings[j], gPiManagerArray[j]);
                var_s2 += 6;
            }
        }
        D_800937F0 = 2;
        var_s2 += 6;
        crash_print_line(0x10, var_s2, "at %08x v0 %08x v1 %08x", (u32)ctx->at, (u32)ctx->v0, (u32)ctx->v1);
        var_s2 += 6;
        crash_print_line(0x10, var_s2, "a0 %08x a1 %08x a2 %08x", (u32)ctx->a0, (u32)ctx->a1, (u32)ctx->a2);
        var_s2 += 6;
        crash_print_line(0x10, var_s2, "a3 %08x t0 %08x t1 %08x", (u32)ctx->a3, (u32)ctx->t0, (u32)ctx->t1);
        var_s2 += 6;
        crash_print_line(0x10, var_s2, "t2 %08x t3 %08x t4 %08x", (u32)ctx->t2, (u32)ctx->t3, (u32)ctx->t4);
        var_s2 += 6;
        crash_print_line(0x10, var_s2, "t5 %08x t6 %08x t7 %08x", (u32)ctx->t5, (u32)ctx->t6, (u32)ctx->t7);
        var_s2 += 6;
        crash_print_line(0x10, var_s2, "s0 %08x s1 %08x s2 %08x", (u32)ctx->s0, (u32)ctx->s1, (u32)ctx->s2);
        var_s2 += 6;
        crash_print_line(0x10, var_s2, "s3 %08x s4 %08x s5 %08x", (u32)ctx->s3, (u32)ctx->s4, (u32)ctx->s5);
        var_s2 += 6;
        crash_print_line(0x10, var_s2, "s6 %08x s7 %08x t8 %08x", (u32)ctx->s6, (u32)ctx->s7, (u32)ctx->t8);
        var_s2 += 6;
        crash_print_line(0x10, var_s2, "t9 %08x gp %08x sp %08x", (u32)ctx->t9, (u32)ctx->gp, (u32)ctx->sp);
        var_s2 += 6;
        crash_print_line(0x10, var_s2, "s8 %08x", (u32)ctx->s8);
        D_800937F0 = 0;
        crash_print_line(0x60, 0xDC, "press button for stack trace");
        while (1) {
            crash_copy_control_inputs();
            if (gCrashButtons[0] == 0) {
                continue;
            }
            if (count == 1) {
                print_stack_trace(threads, count, offset);
                continue;
            }

            break;
        }

        offset++;
        if (offset >= count) {
            offset = 0;
        }
    }
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/exception/print_stack_trace.s")
#else
// https://decomp.me/scratch/EZR1D
void print_stack_trace(OSThread** threads, s32 arg1, s32 arg2) {
    OSThread* thread;
    __OSThreadContext* ctx;
    s32 temp_v0_2;
    u32 *var_s2;
    s32 var_s3;
    s32 sp60;
    s32 var_s4;
    u32 *var_s0;
    void* sp54;
    void* sp50;
    u32 *var_s1;

    sp60 = 0x1C;
    thread = threads[arg2];
    clear_framebuffer_current();
    D_800937F0 = 6;
    crash_print_line(0xDC, 0xD6, "STACK TRACE");
    D_800937F0 = 0;
    crash_print_line(0x10, 0x18, "Fault in thread %d", thread->id);
    D_800937F0 = 0;
    ctx = &thread->context;
    crash_print_line(0x50, 0xDC, "press button for FPU registers");
    var_s1 = (u32*) ctx->ra;
    var_s0 = (u32*) ctx->pc;
    var_s2 = (u32*) ctx->sp;
    D_800937F0 = 3;
    crash_print_line(0x10, 0x24, "J#\tPC\t\t\t\tSP");
    crash_print_line(0xC8, 0x24, "DLL#\tDLL ADDR");
    D_800937F0 = 2;
    crash_print_line(0x10, 0x2A, "%02d\t%08x\t%08x", 0, var_s0, var_s2);
    temp_v0_2 = find_executing_dll(var_s1, &sp54, &sp50);
    if (temp_v0_2 != -1) {
        D_800937F0 = 5;
        crash_print_line(0xC8, 0x2A, "%d", temp_v0_2);
        crash_print_line(0xF0, 0x2A, "%08x", sp54);
        D_800937F0 = 2;
    }
    var_s3 = 0x30;
    var_s4 = 1;
    while (var_s0 != NULL && sp60 > var_s4) {
        if ((var_s0[0] & 0xFC1FFFFF) == 0xF809) {
            var_s1 = var_s0[1];
            var_s0 += 1;
            continue;
        }

        switch (var_s0[0] >> 0x10) {
            case 0xDFBF:
                var_s1 = (var_s2 + (((u32) (var_s0[0] & 0xFFFF) >> 2)))[1];
                break;
            case 0x8FBF:
                // @fake *0
                var_s1 = (var_s2 + (((u32) (var_s0[0] & 0xFFFF)) >> 2))[var_s0[0] * 0];
                break;
            case 0x3E0:
                if ((var_s0[0] & 0xFFFF) != 8) {
                    break;
                }

                // ????????
                if (&var_s0[1]) {
                    switch (var_s0[1] >> 0x10) {
                        case 0x27BD:
                            var_s2 += ((u32) (var_s0[1] & 0xFFFF) >> 2);
                            break;
                    }
                }
                var_s0 = var_s1;
                if (var_s1) {
                    crash_print_line(0x10, var_s3, "%02d\t%08x\t%08x", var_s4, var_s1, var_s2);
                    temp_v0_2 = find_executing_dll(var_s1, &sp54, &sp50);
                    if (temp_v0_2 != -1) {
                        D_800937F0 = 5;
                        crash_print_line(0xC8, var_s3, "%d", temp_v0_2);
                        crash_print_line(0xF0, var_s3, "%08x", sp54);
                        D_800937F0 = 2;
                    }
                    var_s3 += 6;
                    var_s4 += 1;
                }
                continue;
            case 0x27BD:
                var_s2 += ((u32) (var_s0[0] & 0xFFFF) >> 2);
                break;
        }
        var_s0 += 1;
    }
    while (1) {
        crash_copy_control_inputs();
        if (gCrashButtons[0] != 0) {
            other_crash_print(threads, arg1, arg2);
        }
    }
}
#endif

void other_crash_print(OSThread **threads, s32 count, s32 threadIdx) {
    OSThread *thread;
    __OSThreadContext *context;

    thread = threads[threadIdx];
    context = &thread->context;

    clear_framebuffer_current();

    D_800937F0 = 6;
    crash_print_line(244, 214, "FPU INFO");

    D_800937F0 = 0;
    crash_print_line(16, 24, "Fault in thread %d", thread->id);

    D_800937F0 = 3;
    crash_print_line(16, 34, "fpcsr : %08x", context->fpcsr);
    get_err_string(16, 40, context->fpcsr, &errStringArray_cause[59]);

    D_800937F0 = 2;
    crash_print_line(16, 50, "FPU Register dump diabled");

    D_800937F0 = 0;
    crash_print_line(80, 220, "press button for CPU registers");

    while (1) {
        do {
            crash_copy_control_inputs();
        } while (gCrashButtons[0] == 0);

        some_crash_print(threads, count, threadIdx);
    }
}

void crash_copy_control_inputs(void) {
    s32 i;

    osContGetReadData(gCrashContPadArray1);
    osContStartReadData(&gCrashControllerMesgQueue);
    for (i = 0; i < MAXCONTROLLERS; i++) {
        gCrashButtons[i] = (gCrashContPadArray1[i].button ^ gCrashContPadArray2[i].button) & gCrashContPadArray1[i].button & 0xFFFF;
    }
    bcopy(gCrashContPadArray1, gCrashContPadArray2, sizeof(OSContPad) * MAXCONTROLLERS);
}

void check_video_mode_crash_and_clear_framebuffer() {
    int i;

    D_800937F0 = 0;

    // Set a video crash flag if video mode is between 4-6
    if (vi_get_mode() > 3 && vi_get_mode() < 7) {
        gSomeCrashVideoFlag = 1;
    } else {
        gSomeCrashVideoFlag = 0;
    }

    // Clear framebuffer 100 times
    for (i = 0; i != 100; ++i) {
        clear_framebuffer_current();
    }
}

void func_80062D38(s32 col, s32 row, u8* arg2) {
    u32 width;
    s32 var_a3;
    s32 i;
    u16* fb;
    u16* tempFb;
    u8 var_a1;
    u16 *a2;

    width = vi_get_current_size() & 0xFFFF;
    fb = &gFramebufferCurrent[(row * width) + col];
    a2 = (u16*)&((u8*)D_800933C4)[D_800937F0 * 8];
    i = MAXCONTROLLERS + 1;
    while (i--) {
        var_a3 = 1;
        if (gSomeCrashVideoFlag != 0) {
            var_a3 = 2;
        }
        while (var_a3--) {
            tempFb = fb;
            fb += width;
            var_a1 = arg2[0];
            while (var_a1) {
                (tempFb++)[0] = a2[var_a1 & 3];
                (tempFb++)[0] = a2[var_a1 & 3];
                var_a1 >>= 2;
            }
        }
        arg2++;
    }
}

void func_80062E38(s32 x, s32 y, char *str) {
    char c;

    while (*str != '\0') {
        c = *str++;

        if (c == '\n') {
            y += 6;
            x = 32;
        } else if (c == '\t') {
            x = (x - (x & 0xf)) + 16;
        } else if (c <= ' ') {
            x += 4;
        } else if (c > ' ' && c <= 'z') {
            func_80062D38(x, y, &D_80093200[c * 5 - 165]);
            x += 8;
        }
    }

    osWritebackDCacheAll();
}

void crash_print_line(s32 x, s32 y, char *fmt, ...) {
    s32 var;
    va_list ap;
    char str[252]; // exact length could vary between 249-252

    va_start(ap, fmt);
    
    vsprintf(str, fmt, ap);

    va_end(ap);

    if (gSomeCrashVideoFlag) {
        if (gSomeCrashVideoFlag == 1) {
            y -= 8;
        } else {
            y -= 104;
        }

        if (y < 0 || y > 115) {
            return;
        }

        y *= 2;
    }

    func_80062E38(x, y, str);
}

void get_err_string(s32 x, s32 y, u32 param3, CrashErrString *param4) {
    s32 bvar;
    char str[260]; // length is anywhere between 253-260
    s32 len;

    bvar = 1;

    sprintf(str, "(");
    len = strlen(str);

    while (param4->code1 != 0) {
        if ((param4->code1 & param3) == param4->code2) {
            if (bvar) {
                bvar = 0;
            } else {
                sprintf(str + len, ",");
            }

            len = strlen(str);
            sprintf(str + len, "%s", param4->text);
            len = strlen(str);
        }

        param4++;
    }

    sprintf(str + len, ")");
    len = strlen(str);
    crash_print_line(x, y, str);
}

static const char str_8009b650[] = "recent ";
static const char str_8009b658[] = "       ";
static const char str_8009b660[] = "%s:%d\n";

/**
 * Sets all values of gFramebufferCurrent to 0.
 */
void clear_framebuffer_current() {
    u32 resEncoded = vi_get_current_size();
    s32 valuesLeft = GET_VIDEO_WIDTH(resEncoded) * (GET_VIDEO_HEIGHT(resEncoded) & 0xffff);
    u16 *framebufferPtr = gFramebufferCurrent;

    while (valuesLeft--) {
        *framebufferPtr = 0;
        ++framebufferPtr;
    }
}

void write_c_file_label_pointers(const char *cFileLabel, s32 a1) {
    // If gCFileLabelFlag is zero, then zero out gCFileLabels and gSomeCFileInts
    if (gCFileLabelFlag == 0) {
        for (gCFileLabelFlag = 0; gCFileLabelFlag < C_FILE_LABELS_LENGTH; ++gCFileLabelFlag) {
            gCFileLabels[gCFileLabelFlag] = NULL;
            gSomeCFileInts[gCFileLabelFlag] = 0;
        }

        gCFileLabelFlag = 1;
    }

    // Store C file label at next position in gCFileLabels
    gCFileLabelIndex = gCFileLabelIndex + 1;

    if (gCFileLabelIndex == C_FILE_LABELS_LENGTH) {
        gCFileLabelIndex = 0;
    }

    gCFileLabels[gCFileLabelIndex] = cFileLabel;
    gSomeCFileInts[gCFileLabelIndex] = a1;
}

// diRcpTrace?
s32 func_800631E0(void) {
    s32 labelsIndex;
    s32 i;
    s32 max;

    labelsIndex = gCFileLabelIndex;
    for (i = 0; i < C_FILE_LABELS_LENGTH; i++) {
        u8 *label;

        labelsIndex++;
        if ((labelsIndex ^ 0) == (max = C_FILE_LABELS_LENGTH)) {
            labelsIndex = 0;
        }
        label = gCFileLabels[labelsIndex];
        if (gCFileLabelIndex){}
        if (label) {
            // maybe?
            STUBBED_PRINTF(label);
        }
    }

    return labelsIndex ^ 0;
}
