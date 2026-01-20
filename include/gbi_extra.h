#ifndef _GBI_EXTRA_H
#define _GBI_EXTRA_H

#include "PR/ultratypes.h"

// modified from https://github.com/glankk/n64
// helps with compiling gfxdis output

/* data types and structures */
typedef u8   qu08_t;
typedef u16  qu016_t;
typedef s16   qs48_t;
typedef s16   qs510_t;
typedef u16  qu510_t;
typedef s16   qs102_t;
typedef u16  qu102_t;
typedef s16   qs105_t;
typedef u16  qu105_t;
typedef s16   qs132_t;
typedef s16   qs142_t;
typedef s32   qs1516_t;
typedef s32   qs1616_t;
typedef s32   qs205_t;

/* fixed-point conversion macros */
#define qu08(n)                       ((qu08_t)((n)*0x100))
#define qu016(n)                      ((qu016_t)((n)*0x10000))
#define qs48(n)                       ((qs48_t)((n)*0x0100))
#define qs510(n)                      ((qs510_t)((n)*0x0400))
#define qu510(n)                      ((qu510_t)((n)*0x0400))
#define qs102(n)                      ((qs102_t)((n)*0x0004))
#define qu102(n)                      ((qu102_t)((n)*0x0004))
#define qs105(n)                      ((qs105_t)((n)*0x0020))
#define qu105(n)                      ((qu105_t)((n)*0x0020))
#define qs132(n)                      ((qs132_t)((n)*0x0004))
#define qs142(n)                      ((qs142_t)((n)*0x0004))
#define qs1516(n)                     ((qs1516_t)((n)*0x00010000))
#define qs1616(n)                     ((qs1616_t)((n)*0x00010000))
#define qs205(n)                      ((qs205_t)((n)*0x00000020))

#endif
