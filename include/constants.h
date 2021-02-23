#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include "ultra64.h"

#define HW_REG2(reg, type) *(volatile type *)(u32)(reg | 0xa0000000)

#define IDLE_THREAD_SIZE (0x800 / sizeof(u64))
#define MAIN_THREAD_SIZE 1024

#endif
