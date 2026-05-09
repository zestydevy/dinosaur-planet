#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include "ultra64.h"

#define HW_REG2(reg, type) *(volatile type *)(u32)(reg | 0xa0000000)

#define PLAYER_POSBUF_SIZE 60

#define F32_MAX 3.4028235E+38

#endif //_CONSTANTS_H
