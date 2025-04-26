#ifndef _SYS_EXCEPTION_H_
#define _SYS_EXCEPTION_H_

#include "PR/os.h"
#include "PR/ultratypes.h"

extern int gSomeCrashVideoFlag;
extern s32 D_800937F0;
extern u16 D_800933C4[];

// Length of gPiManagerArray
#define PI_MANAGER_ARRAY_LENGTH 5

extern s32 gPiManagerArray[PI_MANAGER_ARRAY_LENGTH];
extern u8 gPiManagerThreadStack[OS_PIM_STACKSIZE];
extern OSThread gPiManagerThread;

// Length of gPiManagerEventQueueBuffer
#define PI_MANAGER_EVENT_QUEUE_LENGTH 8
// Length of gPiManagerCmdQueueBuffer
#define PI_MANAGER_CMD_QUEUE_LENGTH 8

extern OSMesgQueue gPiManagerEventQueue;
extern OSMesg gPiManagerEventQueueBuffer[PI_MANAGER_EVENT_QUEUE_LENGTH];
extern OSMesg gPiManagerCmdQueueBuffer[PI_MANAGER_CMD_QUEUE_LENGTH];
extern OSMesgQueue gPiManagerCmdQueue;

extern OSContPad gCrashContPadArray1[MAXCONTROLLERS];
extern OSContPad gCrashContPadArray2[MAXCONTROLLERS];
extern OSMesgQueue gCrashControllerMesgQueue;

extern u16 gCrashButtons[MAXCONTROLLERS];

// Length of gCFileLabels and gSomeCFileInts
#define C_FILE_LABELS_LENGTH 10

extern u8 gCFileLabelIndex;
extern const char *gCFileLabels[C_FILE_LABELS_LENGTH];
extern u8 gCFileLabelFlag;
extern s32 gSomeCFileInts[C_FILE_LABELS_LENGTH];

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
