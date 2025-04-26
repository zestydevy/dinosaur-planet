#include "sys/fs.h"
#include "sys/memory.h"
#include "sys/rarezip.h"
#include "sys/asset_thread.h"

huft *gHuftTable = NULL;
u8 *gPackedHeader = NULL;
u8 *rarezip_inflate_input = NULL;
u8 *rarezip_inflate_output = NULL;
void *D_800918C0 = NULL;

extern u32 rarezip_bit_buffer;
extern u32 rarezip_num_bits;
extern s32 gHuftTablePos;

extern s32 rarezip_inflate_block();

void rarezip_init() {
    D_800918C0 = (void*)malloc(0x4000, 0x7F7F7FFF, NULL);
    gHuftTable = (void*)malloc(sizeof(huft) * 1280, 0x7F7F7FFF, NULL);
    gPackedHeader = (void*)malloc(0x20, 0x000000FF, NULL);
}

s32 rarezip_uncompress_size(u8 *b) {
    s32 result = *b++;
    result |= *b++ << 8;
    result |= *b++ << 16;
    result |= *b++ << 24;
    return result;
}

s32 rarezip_uncompress_size_rom(s32 arg0, s32 arg1, s32 arg2) {
    if (arg2 != 0) {
        read_file_region(arg0, gPackedHeader, arg1, 8);
    } else {
        queue_load_file_region_to_ptr((void**)gPackedHeader, arg0, arg1, 8);
    }
    
    return rarezip_uncompress_size(gPackedHeader);
}

static const char str_80099eb0[] = "WARNING: rzip buffer overflow (%d)\n";
static const char str_80099ed4[] = "rzipUncompress:overflow i:%08x o:%08x %d\n";
static const char str_80099f00[] = "rzipUncompress(%08x,%08x,...) overflow %d/%d\n";

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/rarezip/rarezip_uncompress.s")
#else
u8 *rarezip_uncompress(u8 *compressedInput, u8 *decompressedOutput, s32 outputSize) {
    rarezip_inflate_input = compressedInput + 5;
    rarezip_inflate_output = decompressedOutput;
    rarezip_num_bits = 0;
    rarezip_bit_buffer = 0;

    if (rarezip_inflate_input){} // fake match
    
    while (rarezip_inflate_block() != 0) {
        
    }

    if (outputSize != -1) {
        if (((!decompressedOutput) && (!decompressedOutput)) && (!decompressedOutput)){} // fake match
        if (compressedInput){} // fake match
    }
    
    return decompressedOutput;
}
#endif

void rarezip_huft_build(u32 *b, u32 n, u32 s, u16 *d, u16 *e, huft **t, s32 *m) {
    u32 a;                   /* counter for codes of length k */
    u32 c[BMAX + 1];         /* bit length count table */
    u32 f;                   /* i repeats in table every f entries */
    s32 g;                   /* maximum code length */
    s32 h;                   /* table level */
    register u32 i;          /* counter, current code */
    register u32 j;          /* counter */
    register s32 k;          /* number of bits in current code */
    s32 l;                   /* bits per table (returned in m) */
    register u32 *p;         /* pointer into c[], b[], or v[] */
    register struct huft *q; /* points to current table */
    struct huft r;           /* table entry for structure assignment */
    struct huft *u[BMAX];    /* table stack */
    u32 v[N_MAX];            /* values in order of bit length */
    register s32 w;          /* bits before this table == (l * h) */
    u32 x[BMAX + 1];         /* bit offsets, then code stack */
    u32 *xp;                 /* pointer into x */
    s32 y;                   /* number of dummy codes added */
    u32 z;                   /* number of entries in current table */

    /* Generate counts for each bit length */
    bzero(c, sizeof(c));
    p = b;
    i = n;
    do {
        c[*p]++; /* assume all entries <= BMAX */
        p++;     /* Can't combine with above line (Solaris bug) */
    } while (--i);
    if (c[0] == n) /* null input--all zero length codes */
    {
        *t = NULL;
        *m = 0;
        return;
    }

    /* Find minimum and maximum length, bound *m by those */
    l = *m;
    for (j = 1; j <= BMAX; j++) {
        if (c[j]) {
            break;
        }
    }
    k = j; /* minimum code length */
    if ((u32) l < j) {
        l = j;
    }
    for (i = BMAX; i; i--) {
        if (c[i]) {
            break;
        }
    }
    g = i; /* maximum code length */
    if ((u32) l > i) {
        l = i;
    }
    *m = l;

    // Something is missing here.
    /* Adjust last length count to fill out codes, if needed */
    y = 1 << j;
    while (j < i) {
        y -= c[j];
        j++;
        y <<= 1;
    }
    y -= c[i];
    c[i] += y;

    /* Generate starting offsets into the value table for each length */
    x[1] = j = 0;
    p = c + 1;
    xp = x + 2;
    while (--i) { /* note that i == g from above */
        *xp++ = (j += *p++);
    }

    /* Make a table of values in order of bit lengths */
    p = b;
    i = 0;
    do {
        if ((j = *p++) != 0) {
            v[x[j]++] = i;
        }
    } while (++i < n);
    // n = x[g];              /* set n to length of v */

    /* Generate the Huffman codes and for each, make the table entries */
    x[0] = i = 0; /* first Huffman code is zero */
    p = v;        /* grab values in bit order */
    h = -1;       /* no tables yet--level -1 */
    w = -l;       /* bits decoded == (l * h) */
    u[0] = NULL;  /* just to keep compilers happy */
    q = NULL;     /* ditto */
    z = 0;        /* ditto */

    // gHuftTable and gHuftTablePos go in here somewhere.

    /* go through the bit lengths (k already is bits in shortest code) */
    for (; k <= g; k++) {
        a = c[k];
        while (a--) {
            /* here i is the Huffman code of length k bits for value *p */
            /* make tables up to required level */
            while (k > w + l) {
                h++;
                w += l; /* previous table always l bits */

                /* compute minimum size table less than or equal to l bits */
                z = (z = g - w) > (u32) l ? (u32) l : z; /* upper limit on table size */
                if ((f = 1 << (j = k - w)) > a + 1)      /* try a k-w bit table */
                {                                        /* too few codes for k-w bit table */
                    f -= a + 1;                          /* deduct codes from patterns left */
                    xp = c + k;
                    while (++j < z) /* try smaller tables up to z bits */
                    {
                        if ((f <<= 1) <= *++xp) {
                            break; /* enough codes to use up j bits */
                        }
                        f -= *xp; /* else deduct codes from patterns */
                    }
                }
                z = 1 << j; /* table entries for j-bit table */

                q = &gHuftTable[gHuftTablePos];
                gHuftTablePos += z + 1;

                *t = q + 1; /* link to list for huft_free() */
                *(t = &(q->v.t)) = NULL;
                u[h] = ++q; /* table starts after link */

                /* connect to last table, if there is one */
                if (h) {
                    x[h] = i;         /* save pattern for backing up */
                    r.b = l;          /* bits to dump before this table */
                    r.e = 16 + j;     /* bits in this table */
                    r.v.t = q;        /* pointer to this table */
                    j = i >> (w - l); /* (get around Turbo C bug) */
                    u[h - 1][j] = r;  /* connect to last table */
                }
            }

            /* set up table entry in r */
            r.b = k - w;
            if (p >= v + n) {
                r.e = 99; /* out of values--invalid code */
            } else if (*p < s) {
                r.e = *p < 256 ? 16 : 15; /* 256 is end-of-block code */
                r.v.n = *p;               /* simple code is just the value */
                p++;                      /* one compiler does not like *p++ */
            } else {
                r.e = ((u8 *) e)[*p - s]; /* non-simple--look up in lists */
                r.v.n = d[*p++ - s];
            }

            /* fill code-like entries with r */
            f = 1 << (k - w);
            for (j = i >> w; j < z; j += f) {
                q[j] = r;
            }

            /* backwards increment the k-bit code i */
            for (j = 1 << (k - 1); i & j; j >>= 1) {
                i ^= j;
            }
            i ^= j;

            /* backup over finished tables */
            while ((i & ((1 << w) - 1)) != x[h]) {
                h--; /* don't need to update q */
                w -= l;
            }
        }
    }
    return;
}
