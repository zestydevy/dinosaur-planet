#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include "ultra64.h"

#define HW_REG2(reg, type) *(volatile type *)(u32)(reg | 0xa0000000)
#define ACCESS_1 *(s16 *)(u32)(0x1C000000 | 0xa0000000)
#define ACCESS_2 *(s16 *)(u32)(0x1C000002 | 0xa0000000)

#define PLAYER_POSBUF_SIZE 60

#endif
