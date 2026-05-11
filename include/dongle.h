#ifndef _DONGLE_H
#define _DONGLE_H

#include "PR/ultratypes.h"

#define ACCESS_1 *(s16 *)(u32)(0x1C000000 | 0xa0000000)
#define ACCESS_2 *(s16 *)(u32)(0x1C000002 | 0xa0000000)
#define DONGLE_LSFS 0x4C534653
#define DONGLE_MPFS 0x4D504653

#endif
