#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include "ultra64.h"

#define THREAD_ID_IDLE 1
#define THREAD_ID_MAIN 3

#define HW_REG2(reg, type) *(volatile type *)(u32)(reg | 0xa0000000)
#define ACCESS_1 *(s16 *)(u32)(0x1C000000 | 0xa0000000)
#define ACCESS_2 *(s16 *)(u32)(0x1C000002 | 0xa0000000)
#define EXPANSION_RAM_START 0x80400000
#define EXPANSION_SIZE 0x00800000

#define HEAP_AREA_00 0x8042C000
#define HEAP_AREA_01 0x80245000

#define IDLE_THREAD_SIZE (0x800 / sizeof(u64))
#define MAIN_THREAD_SIZE 1024

#define DEBUG_LOOP1 do {/* omitted debug code */} while(0)

#define PLAYER_POSBUF_SIZE 0x3C

#endif
