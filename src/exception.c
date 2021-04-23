#include <PR/os_internal.h>
#include "common.h"
#include "video.h"

void clear_framebuffer_current();

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/exception/update_pi_man_array.s")
#else
void update_pi_man_array(s32 index, s32 value) {
    if (index > -1 && index < 5) {
        pi_man_array[index] = value;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/exception/PiManager_thread_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/PiManagerStarter.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/exception/crash_controller_getter.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/some_crash_print.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/print_stack_trace.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/other_crash_print.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/crash_copy_control_inputs.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/exception/func_80062D38.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/exception/write_cFile_label_pointers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/func_800631E0.s")
