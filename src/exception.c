#include <PR/os_internal.h>
#include "common.h"
#include "exception.h"
#include "video.h"

void clear_framebuffer_current();
void pi_manager_entry(void *arg);
void stop_active_app_threads();
void crash_controller_getter();
void check_video_mode_crash_and_clear_framebuffer();
void some_crash_print(OSThread**, int, int);

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/exception/update_pi_manager_array.s")
#else
/**
 * Sets `gPiManagerArray[index] = value`, unless index is out of range
 * in which case this function does nothing.
 */
void update_pi_manager_array(s32 index, s32 value) {
    if (index >= 0 && index < PI_MANAGER_ARRAY_LENGTH) {
        gPiManagerArray[index] = value;
    }
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/exception/start_pi_manager_thread.s")
#else
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
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/exception/pi_manager_entry.s")
#else
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
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/exception/stop_active_app_threads.s")
#else
/**
 * Stops all active application threads (those with priorities between 1 and OS_PRIORITY_APPMAX).
 */
void stop_active_app_threads() {
    OSThread *thread;

    thread = __osGetActiveQueue();

    while (thread->priority != -1) {
        if (thread->priority > OS_PRIORITY_IDLE && 
            thread->priority <= OS_PRIORITY_APPMAX) {
            osStopThread(thread);
        }

        thread = thread->tlnext;
    };
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/exception/crash_controller_getter.s")
#else
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
    _bcopy(
        &gCrashContPadArray1[0], 
        &gCrashContPadArray2[0], 
        sizeof(OSContPad) * MAXCONTROLLERS
    );

    // Print some crash info
    some_crash_print(&thread, 1, 0);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/exception/some_crash_print.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/print_stack_trace.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/other_crash_print.s")

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

    _bcopy(
        &gCrashContPadArray1[0], 
        &gCrashContPadArray2[0], 
        sizeof(OSContPad) * MAXCONTROLLERS
    );
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/exception/check_video_mode_crash_and_clear_framebuffer.s")
#else
/**
 * - Sets D_800937F0 to 0
 * - Sets gSomeCrashVideoFlag if video mode is between 4-6
 * - Clears the current framebuffer 100 times
 */
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
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/exception/func_80062D38.s")
#else
// Super super close, pretty sure is functionally equiv
void _func_80062D38(s32 col, s32 row, u8 *param3) {
    int k;
    u16 *fbTemp;
    u32 res = get_some_resolution_encoded();
    s32 resWidth = res & 0xffff;
    int i = 4;
    u16 *fb = &gFramebufferCurrent[(row * resWidth) + col];
    u16 *someArray = &D_800933C4[D_800937F0 << 2];
    u8 temp;
    u16 *pixelPtr;

    do {
        k = 1;
        //v0 = k;

        if (gSomeCrashVideoFlag != 0) {
            k = 2;
        }

        while (k--) {
            temp = *param3;

            fbTemp = fb;
            fb = fb + resWidth;

            while (temp != 0) {
                pixelPtr = &someArray[temp & 3];
                temp >>= 2;

                fbTemp[0] = *pixelPtr;
                fbTemp[1] = *pixelPtr;
                fbTemp = fbTemp + 2;
            }

            //v0 = k;
        }

        //v0 = i;
        param3 = param3 + 1;
    } while (i--);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/exception/func_80062E38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/crash_print_line.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/get_err_string.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/exception/clear_framebuffer_current.s")
#else
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
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/exception/write_c_file_label_pointers.s")
#else
void write_c_file_label_pointers(char *cFileLabel, s32 a1) {
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
#endif

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
