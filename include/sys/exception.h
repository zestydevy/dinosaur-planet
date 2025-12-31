#ifndef _SYS_EXCEPTION_H_
#define _SYS_EXCEPTION_H_

#include "PR/os.h"
#include "PR/ultratypes.h"

void update_pi_manager_array(s32 index, s32 value);

void start_pi_manager_thread();

/**
 * Stops all active application threads (those with priorities between 1 and OS_PRIORITY_APPMAX).
 */
void stop_active_app_threads();

void some_crash_print(OSThread **threads, s32 count, s32 offset);

/**
 * - Sets D_800937F0 to 0
 * - Sets gSomeCrashVideoFlag if video mode is between 4-6
 * - Clears the current framebuffer 100 times
 */
void check_video_mode_crash_and_clear_framebuffer();

void clear_framebuffer_current();
void pi_manager_entry(void *arg);
void crash_controller_getter();
void check_video_mode_crash_and_clear_framebuffer();

void write_c_file_label_pointers(const char *cFileLabel, s32 a1);

#endif //_SYS_EXCEPTION_H_
