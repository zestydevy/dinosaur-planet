#ifndef _SYS_RAREZIP_H
#define _SYS_RAREZIP_H

#include "PR/ultratypes.h"

typedef struct huft {
    u8 e;                /* number of extra bits or operation */
    u8 b;                /* number of bits in this code or subcode */
    union {
        u16 n;              /* literal, length base, or distance base */
        struct huft *t;     /* pointer to next level of table */
    } v;
} huft;

/* If BMAX needs to be larger than 16, then h and x[] should be ulg. */
#define BMAX 16         /* maximum bit length of any code (16 for explode) */
#define N_MAX 288       /* maximum number of codes in any set */

void rarezip_init();
s32 rarezip_uncompress_size(u8 *b);
s32 rarezip_uncompress_size_rom(s32 arg0, s32 arg1, s32 arg2);
u8 *rarezip_uncompress(u8 *compressedInput, u8 *decompressedOutput, s32 outputSize);
void rarezip_huft_build(u32 *b, u32 n, u32 s, u16 *d, u16 *e, huft **t, s32 *m);

#endif
