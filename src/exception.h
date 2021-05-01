#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include "ultra64.h"

#include "constants.h"
#include "common_structs.h"
#include "variables.h"

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
extern char *gCFileLabels[C_FILE_LABELS_LENGTH];
extern u8 gCFileLabelFlag;
extern s32 gSomeCFileInts[C_FILE_LABELS_LENGTH];

#endif
