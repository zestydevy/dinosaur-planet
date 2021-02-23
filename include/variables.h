#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "ultra64.h"

#include "constants.h"
#include "common_structs.h"

extern OSThread* __osRunningThread;
extern OSThread* __osRunQueue;
extern u8 gIdleThreadStack[IDLE_THREAD_SIZE];
extern OSThread gIdleThread;
extern OSThread gMainThread;
extern u64 gMainThreadStack[];        // some sort of data

#endif
