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



// Has an extra or'ing of _SHIFTL(G_TRI2, 24, 8) in w1
// Otherwise it's the same as calling gSP2Triangles
#define gSP2Triangles2(pkt, v00, v01, v02, flag0, v10, v11, v12, flag1)	\
{ \
	Gfx *_g = (Gfx *)(pkt); \
	_g->words.w0 = (_SHIFTL(G_TRI2, 24, 8)| __gsSP1Triangle_w1f(v00, v01, v02, flag0)); \
    _g->words.w1 = (_SHIFTL(G_TRI2, 24, 8)| __gsSP1Triangle_w1f(v10, v11, v12, flag1)); \
}

// same as gSPVertex but has the second and third shiftl swapped for w0
# define gSPVertex2(pkt, v, n, v0)				\
{									\
	Gfx *_g = (Gfx *)(pkt);						\
	_g->words.w0 = _SHIFTL(G_VTX,24,8)|_SHIFTL((v0)+(n),1,7)|_SHIFTL((n),12,8);	\
	_g->words.w1 = (unsigned int)(v);				\
}

// similar to gSP1Triangle except it works with an entire word instead of chars
#define gSP1TriangleBlock(pkt, v) \
    (pkt)->words.w0 = _SHIFTL(G_TRI1, 24, 8) | \
        ((v & 0x3F000) << 4) | ((v & 0xFC0) << 2) | ((v & 0x3F) << 0);

// similar to gSP2Triangles except it works with an entire word instead of chars
#define gSP2TrianglesBlock(pkt, v0, v1) \
    (pkt)->words.w0 = _SHIFTL(G_TRI2, 24, 8) | \
        ((v0 & 0x3F000) << 4) | ((v0 & 0xFC0) << 2) | ((v0 & 0x3F) << 0); \
    (pkt)->words.w1 = ((v1 & 0x3F000) << 4) | ((v1 & 0xFC0) << 2) | ((v1 & 0x3F) << 0);

#endif
