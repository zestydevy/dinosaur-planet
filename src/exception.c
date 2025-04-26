#include "PR/os_internal.h"
#include "common.h"
#include "sys/crash.h"
#include "libc/stdarg.h"

void get_err_string(s32 x, s32 y, u32 param3, ErrString *param4);
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
        /*sp*/      &gPiManagerThreadStack[OS_PIM_STACKSIZE],
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

#pragma GLOBAL_ASM("asm/nonmatchings/exception/some_crash_print.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/print_stack_trace.s")

extern char D_8009B5D4[]; // "FPU INFO"
extern char D_8009B5E0[]; // "Fault in thread %d"
extern char D_8009B5F4[]; // "fpcsr : %08x"
extern char D_8009B604[]; // "FPU Register dump diabled"
extern char D_8009B620[]; // "press button for CPU registers"

void other_crash_print(OSThread **threads, s32 count, s32 threadIdx) {
    OSThread *thread;
    __OSThreadContext *context;

    thread = threads[threadIdx];
    context = &thread->context;

    clear_framebuffer_current();

    D_800937F0 = 6;
    crash_print_line(244, 214, D_8009B5D4);

    D_800937F0 = 0;
    crash_print_line(16, 24, D_8009B5E0, thread->id);

    D_800937F0 = 3;
    crash_print_line(16, 34, D_8009B5F4, context->fpcsr);
    get_err_string(16, 40, context->fpcsr, &errStringArray_cause[59]);

    D_800937F0 = 2;
    crash_print_line(16, 50, D_8009B604);

    D_800937F0 = 0;
    crash_print_line(80, 220, D_8009B620);

    while (1) {
        do {
            crash_copy_control_inputs();
        } while (gCrashButtons[0] == 0);

        some_crash_print(threads, count, threadIdx);
    }
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/exception/crash_copy_control_inputs.s")
#else
// Functionally equivalent
void _crash_copy_control_inputs() {
    int i;

    osContGetReadData(&gCrashContPadArray1[0]);
    osContStartReadData(&gCrashControllerMesgQueue);

    // TODO: This loop unrolls differently than it should
    for (i = 0; i < MAXCONTROLLERS; ++i) {
        gCrashButtons[i] = gCrashContPadArray1[i].button &
            (u16)(gCrashContPadArray1[i].button ^ gCrashContPadArray2[i].button);
    }

    bcopy(
        &gCrashContPadArray1[0],
        &gCrashContPadArray2[0],
        sizeof(OSContPad) * MAXCONTROLLERS
    );
}
#endif

void check_video_mode_crash_and_clear_framebuffer() {
    int i;

    D_800937F0 = 0;

    // Set a video crash flag if video mode is between 4-6
    if (get_video_mode() > 3 && get_video_mode() < 7) {
        gSomeCrashVideoFlag = 1;
    } else {
        gSomeCrashVideoFlag = 0;
    }

    // Clear framebuffer 100 times
    for (i = 0; i != 100; ++i) {
        clear_framebuffer_current();
    }
}

void func_80062D38(s32 col, s32 row, char *param3);
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/exception/func_80062D38.s")
#else
// Super super close, pretty sure is functionally equiv
void func_80062D38(s32 col, s32 row, char *param3) {
    int k;
    u16 *fbTemp;
    u32 res;
    s32 resWidth;
    int i;
    u16 *fb;
    u16 *someArray;
    char temp;
    u16 *pixelPtr;
    char c;

    res = get_some_resolution_encoded();
    resWidth = res & 0xffff;
    fb = &gFramebufferCurrent[(row * resWidth) + col];
    someArray = &D_800933C4[D_800937F0 << 2];

    i = 4;
    do {
        k = 1;
        //v0 = k;

        if (gSomeCrashVideoFlag != 0) {
            k = 2;
        }

        while (k--) {
            fbTemp = fb;
            temp = *param3;
            while (temp != 0) {
                //pixelPtr = &someArray[temp & 3];
                //temp >>= 2;

                *fbTemp++ = someArray[temp & 3];
                *fbTemp++ = someArray[temp & 3];

                temp >>= 2;
            }
            
            fb += resWidth;

            //v0 = k;
        }

        //v0 = i;
        param3 = param3 + 1;
    } while (i--);
}
#endif

extern char D_80093200[]; // TODO: is this really the start of the array?

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

extern char D_8009B640[]; // "("
extern char D_8009B644[]; // ","
extern char D_8009B648[]; // "%s"
extern char D_8009B64C[]; // ")"

void get_err_string(s32 x, s32 y, u32 param3, ErrString *param4) {
    s32 bvar;
    char str[260]; // length is anywhere between 253-260
    s32 len;

    bvar = 1;

    sprintf(str, D_8009B640);
    len = strlen(str);

    while (param4->code1 != 0) {
        if ((param4->code1 & param3) == param4->code2) {
            if (bvar) {
                bvar = 0;
            } else {
                sprintf(str + len, D_8009B644);
            }

            len = strlen(str);
            sprintf(str + len, D_8009B648, param4->text);
            len = strlen(str);
        }

        param4++;
    }

    sprintf(str + len, D_8009B64C);
    len = strlen(str);
    crash_print_line(x, y, str);
}

/**
 * Sets all values of gFramebufferCurrent to 0.
 */
void clear_framebuffer_current() {
    u32 resEncoded = get_some_resolution_encoded();
    s32 valuesLeft = (/*hRes*/resEncoded & 0xffff) * (/*vRes*/(resEncoded >> 0x10) & 0xffff);
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
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/exception/func_800631E0.s")
#else
// Functionally equivalent
char **_func_800631E0() {
    // TODO: Every time gCFileLabels is referenced in the original assembly, it's address is
    // fully recomputed. But the code below calculates it once and reuses a register holding the addr

    s32 labelsIndex;
    char **label;
    s32 i;

    labelsIndex = gCFileLabelIndex + 1;

    if (labelsIndex == C_FILE_LABELS_LENGTH) {
        labelsIndex = 0;
    }

    label = &gCFileLabels[labelsIndex];

    // TODO: it looks like this loop could be for i 0-10 instead of 1-10 merging the above code,
    //       but doing so seems to result in the loop not getting unrolled...
    for (i = 1; i != C_FILE_LABELS_LENGTH; ++i) {
        ++labelsIndex;
        ++label;

        if (labelsIndex == C_FILE_LABELS_LENGTH) {
            label = &gCFileLabels[0];
            labelsIndex = 0;
        }
    }

    return label;
}
#endif
