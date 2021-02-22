#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "ultra64.h"

extern OSThread* __osRunningThread;
extern OSThread* __osRunQueue;
extern u8 gMainThreadStack[0x1F50];
extern OSThread gMainThread;

#endif
