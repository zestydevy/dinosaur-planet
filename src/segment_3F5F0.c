#include "common.h"

static const char str_8009a480[] = "Freak a0 %x  a1 %x \n";
static const char str_8009a498[] = " size %x ";

void func_8005324C(u16 *fb1, u16 *fb2, s32);
void weird_resize_copy(u16 *src, s32 srcWidth, s32 destWidth, u16 *dest);
void func_8003EC8C(u16*, u16 *, s32);
void func_8003F2C4(u16*, u16, u16, s32);
u16 func_8003FD48(u16, u16, u16, u16);
u16 func_8003FB88(u16, u16, u16, u16);

extern s32 D_80092A50;
extern s32 D_80092A54[3];

// -------- .bss start 800b49e0 -------- //
s32 D_800B49E0;
// -------- .bss end 800b49f0 -------- //

void func_8003E9F0(Gfx** gdl, s32 updateRate) {
    s32 temp_v0;
    s32 *v0;

    if (D_80092A50 > 0) {
        // yeah I don't even know what this is
        if ((D_80092A50 == 0xA) && ((temp_v0 = D_800B49E0) == 0)) {
            D_800B49E0 = temp_v0 = *D_80092A54;
        }
        func_8003FE70(gdl, *D_80092A54, D_80092A50, func_80004A4C());
        v0 = &D_800B49E0;
        if (D_80092A50 == 0xA) {
            temp_v0 = D_800B49E0;
            temp_v0 -= updateRate;
            *v0 = temp_v0;
            if (*v0 <= 0) {
                D_80092A50 = 0;
                *v0 = 0;
            }
        } else {
            D_80092A50 = 0;
        }
        return;
    } else {
        D_800B49E0 = 0;
    }
}

void func_8003EAC0(u16 *arg0, s32 arg1, s32 arg2, u16* arg3) {
    s32 var_a3;

    if (arg2 > 0) {
        func_8003EC8C(&arg3[arg2], arg0, arg1 - arg2);
        bzero(arg3, (arg2 + 1) << 1);
        var_a3 = arg2 >= 0xB ? 0xA : arg2;
        func_8003F2C4(&arg3[arg2] - var_a3, 0, arg3[arg2 + 1], var_a3);
        return;
    }

    arg2 = -arg2;
    func_8003EC8C(arg3, &arg0[arg2], arg1 - arg2);
    bzero(&arg3[arg1 - arg2], arg2 << 1);
    var_a3 = arg2 >= 0xB ? 0xA : arg2;
    func_8003F2C4(&arg3[arg1] - arg2, (&arg3[arg1] - arg2)[-2], 0, var_a3);
}

void func_8003EBD4(s32 hOffset) {
    s32 height;
    s32 resolution;
    s32 width;
    u16* nextFB;
    u16* currentFB;

    resolution = vi_get_current_size();
    width = GET_VIDEO_WIDTH(resolution);
    height = GET_VIDEO_HEIGHT(resolution);
    nextFB = gFramebufferNext;
    currentFB = gFramebufferCurrent;
    hOffset = width - hOffset;
    while (height--) {
        weird_resize_copy(&nextFB[hOffset], width - (hOffset << 1), width, currentFB);
        nextFB += width;
        currentFB += width;
    }
}

void func_8003EC8C(u16* arg0, u16* arg1, s32 arg2) {
    if (arg0 < arg1) {
        while (arg2--) {
            arg0[0] = arg1[0];
            arg0++;
            arg1++;
        }
    } else {
        arg0 += arg2;
        arg1 += arg2;
        while (arg2--) {
            arg0[0] = arg1[0];
            arg0--;
            arg1--;
        }
    }
}

void func_8003ED00(u16* arg0, u16* arg1, s32 arg2) {
    // 0x842 = 2114
    // 0x18C6 = 6342 (2114 * 3)
    // 0x39CE = 14798 (2114 * 7)
    if (arg0 < arg1) {
        (arg0++)[0] = ((arg1++)[0] & ~0x39CE) >> 3;
        (arg0++)[0] = ((arg1++)[0] & ~0x18C6) >> 2;
        (arg0++)[0] = ((arg1++)[0] & ~0x18C6) >> 2;
        (arg0++)[0] = ((arg1++)[0] & ~0x842) >> 1;
        (arg0++)[0] = ((arg1++)[0] & ~0x842) >> 1;
        (arg0++)[0] = ((arg1++)[0] & ~0x842) >> 1;
        arg2 -= 12;
        while (arg2--) {
            (arg0++)[0] = (arg1++)[0];
        }
        (arg0++)[0] = ((arg1++)[0] & ~0x842) >> 1;
        (arg0++)[0] = ((arg1++)[0] & ~0x842) >> 1;
        (arg0++)[0] = ((arg1++)[0] & ~0x842) >> 1;
        (arg0++)[0] = ((arg1++)[0] & ~0x18C6) >> 2;
        (arg0++)[0] = ((arg1++)[0] & ~0x18C6) >> 2;
        (arg0++)[0] = ((arg1++)[0] & ~0x39CE) >> 3;
        return;
    }

    arg1 += arg2;
    arg0 += arg2;
    (arg0--)[0] = ((arg1--)[0] & ~0x39CE) >> 3;
    (arg0--)[0] = ((arg1--)[0] & ~0x18C6) >> 2;
    (arg0--)[0] = ((arg1--)[0] & ~0x18C6) >> 2;
    (arg0--)[0] = ((arg1--)[0] & ~0x842) >> 1;
    (arg0--)[0] = ((arg1--)[0] & ~0x842) >> 1;
    (arg0--)[0] = ((arg1--)[0] & ~0x842) >> 1;
    arg2 -= 12;
    while (arg2--) {
        (arg0--)[0] = (arg1--)[0];
    }
    (arg0--)[0] = ((arg1--)[0] & ~0x842) >> 1;
    (arg0--)[0] = ((arg1--)[0] & ~0x842) >> 1;
    (arg0--)[0] = ((arg1--)[0] & ~0x842) >> 1;
    (arg0--)[0] = ((arg1--)[0] & ~0x18C6) >> 2;
    (arg0--)[0] = ((arg1--)[0] & ~0x18C6) >> 2;
    (arg0--)[0] = ((arg1--)[0] & ~0x39CE) >> 3;
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/segment_3F5F0/func_8003EF30.s")
#else
// https://decomp.me/scratch/KWLGK
void func_8003EF30(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, void (*arg6)(u16*, s32, s32, u16*)) {
    s32 width;
    s32 height;
    s32 temp_s0;
    s32 resolution;
    s32 i;
    u16* currentFB;
    u16* nextFB;
    s32 s1;
    s32 s2;

    resolution = vi_get_current_size();
    width = GET_VIDEO_WIDTH(resolution);
    height = GET_VIDEO_HEIGHT(resolution);
    currentFB = gFramebufferCurrent;
    nextFB = gFramebufferNext;
    i = 0;
    if (height > i) {
        s1 = arg0;
        s2 = arg3;

        do {
            temp_s0 = (sin16(s1) * arg1) / 32768;
            temp_s0 -= (sin16(s2) * arg4) / 32768;
            arg6(currentFB, width, temp_s0, nextFB);
            s1 += arg2;
            s2 += arg5 ^ 0;
            i++;
            currentFB += width;
            nextFB += width;
        } while (i < height);
    }
}
#endif

void func_8003F074(u16* arg0, s32 arg1, s32 arg2, u16* arg3) {
    u16 sp50[644];
    s32 max;
    s32 i;
    s32 var_v0;
    u16 *v1;
    u16 *a0;
    s32 var_a3;

    if (arg2 < 0) {
        arg2 = -arg2;
    }
    var_v0 = 0;
    a0 = sp50;
    v1 = arg0;
    var_a3 = arg1 - (arg2 * 2);
    max = arg1 - (arg2 * 2);
    for (i = 0; i < max; i++) {
        // We *should* re-use max / var_a3 here but that doesn't match
        var_v0 += arg1 % (arg1 - (arg2 * 2));
        a0[0] = v1[0];
        a0++;
        while (var_a3 < var_v0) {
            var_v0 -= var_a3;
            v1 += 1;
        }
        v1 += (arg1 / (var_a3));
    }
    bcopy(sp50, &arg3[arg2], var_a3 << 1);
    bzero(arg3, (arg2 - 1) << 1);
    bzero(&arg3[arg1 - arg2 - 1], arg2 << 1);
    var_a3 = arg2 >= 0xB ? 0xA : arg2;
    func_8003F2C4(&arg3[arg2] - var_a3, 0, arg3[arg2 + 1], var_a3);
    func_8003F2C4(&arg3[arg1] - arg2, (&arg3[arg1] - arg2)[-2], 0, var_a3);
}

/**
 * Copies data from src to dest+destWidth taking into account differences in width
 * (e.g. if destWidth is half srcWidth, every other value will be copied from src).
 *
 * Additionally, values copied to dest have the following applied first:
 *   (value & ~0x842) >> 1
 *
 * destWidth must be less than 644.
 *
 * NOTE: Please see the note in the implementation, this function also reads
 * undefined stack memory, which affects the copied values.
 */
void weird_resize_copy(u16* src, s32 srcWidth, s32 destWidth, u16* dest) {
    s32 i;
    u16 buffer[642];
    s32 var_v0;
    u16* bufferPtr;
    u16* srcPtr;

    var_v0 = 0;
    bufferPtr = buffer;
    srcPtr = src;
    i = 0;
    while (i < destWidth) {
        var_v0 += srcWidth % destWidth;
        // NOTE: This reads undefined stack memory!
        bufferPtr[0] = ((srcPtr[0] & ~0x842) >> 1) + ((bufferPtr[0] & ~0x842) >> 1);
        bufferPtr += 1;
        while (destWidth < var_v0) {
            var_v0 -= destWidth;
            srcPtr += 1;
        }
        i += 1;
        srcPtr += srcWidth / destWidth;
    }
    bcopy(buffer, &dest[destWidth], destWidth << 1);
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_3F5F0/func_8003F2C4.s")
#else
void func_8003F2C4(u16* arg0, u16 arg1, u16 arg2, s32 arg3) {
    u16* var_s3;
    s32 var_a3;
    u16 var_a1;
    u16 var_t0;
    u16 var_v1;
    u16 t1;
    u16 t2;
    u16 t3;

    if (arg3 == 0) {
        return;
    }
    var_s3 = arg0;
    var_a3 = arg3 - 1;
    if (arg3 != 0) {
        var_v1 = (((arg1 & 0xF800) >> 0xB) << 8);
        var_a1 = (((arg1 & 0x7C0) >> 6) << 8);
        var_t0 = (((arg1 & 0x3E) >> 1) << 8);
        do {
            t1 = ((((arg2 & 0xF800) >> 0xB) - ((arg1 & 0xF800) >> 0xB)) << 8) / arg3;
            var_v1 += t1;
            t2 = ((((arg2 & 0x7C0) >> 6) - ((arg1 & 0x7C0) >> 6)) << 8) / arg3;
            var_a1 += t2;
            t3 = ((((arg2 & 0x3E) >> 1) - ((arg1 & 0x3E) >> 1)) << 8) / arg3;
            var_t0 += t3;
            *var_s3 = ((var_v1 >> 8) << 0xB) + ((var_a1 >> 8) << 6) + ((var_t0 >> 8) << 1);
            var_s3 += 1;
        } while (var_a3--);
    }
}
#endif

void NOTosSetTime(s32 arg0, s32 arg1) {
    D_80092A50 = arg0;
    D_80092A54[0] = arg1;
}

void func_8003F4C0(s32 arg0) {
    s32 width;
    s32 resolution;
    s32 height;
    s32 i;
    s32 j;
    u16* nextFB;
    u16* currentFB;
    s32 v0;

    resolution = vi_get_current_size();
    width = GET_VIDEO_WIDTH(resolution);
    height = GET_VIDEO_HEIGHT(resolution);
    if (arg0 == 0) {
        nextFB = gFramebufferNext;
        nextFB[-1] = 0;
        currentFB = gFramebufferCurrent;
        for (i = 0; i < height - 1; i++) {
            for (j = 0; j < width; j++) {
                currentFB[0] = func_8003FD48(currentFB[0], *(nextFB - width), nextFB[0], nextFB[width]);
                currentFB += 1;
                nextFB += 1;
            }
        }
    } else {
        nextFB = gFramebufferNext;
        nextFB[-1] = 0;
        currentFB = gFramebufferCurrent;
        for (i = 0; i < height - 1; i++) {
            for (j = 0; j < width; j++) {
                v0 = 0xFFFF >> (0x10 - arg0);
                // apparently this is v0 * 0x842
                v0 = ~((v0 << 0xB) + (v0 << 6) + (v0 << 1));
                currentFB[0] = (func_8003FD48(currentFB[0], *(nextFB - width), *nextFB, nextFB[width]) &  v0) >> arg0;
                currentFB += 1;
                nextFB += 1;
            }
        }
    }
}

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/segment_3F5F0/func_8003F660.s")
#else
// https://decomp.me/scratch/4XcTd
void func_8003F660(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 height;
    s32 width;
    s32 resolution;
    s32 j;
    s32 var_v0;
    u16 temp_a1;
    u16 temp_v1;
    u16 t1;
    u16 t2;
    u16* currentFB;
    u16* currentFB2;
    u32 a2;
    s32 a3;
    s32 v0;
    s32 temp;
    s32 s8;

    resolution = vi_get_current_size();
    width = GET_VIDEO_WIDTH(resolution);
    height = GET_VIDEO_HEIGHT(resolution);
    if (arg3 != 0) {
        var_v0 = (height - ((height * 0xB6) / 240)) >> 1;
        height -= var_v0;
    } else {
        var_v0 = 0;
    }
    if (arg0 == 0) {
        s8 = var_v0;
        currentFB = &(&(&gFramebufferCurrent[width * var_v0])[arg1])[arg2 * width];
        currentFB[-1] = 0;
        currentFB2 = &gFramebufferCurrent[width * var_v0];
        if (s8 < height) {
            do {
                j = 0;
                while (j < width) {
                    t1 = currentFB[width];
                    t2 = currentFB[-1];
                    temp_a1 = *(currentFB - width);
                    temp_v1 = currentFB[1];
                    j += 1;
                    temp = ((u32)((currentFB[width] & 0xF800) + (currentFB[-1] & 0xF800) + (temp_a1 & 0xF800) + (temp_v1 & 0xF800)) >> 2) & 0xF800;
                    temp += (((currentFB[width] & 0x7C0) + (currentFB[-1] & 0x7C0) + (temp_a1 & 0x7C0) + (temp_v1 & 0x7C0)) >> 2) & 0x7C0;
                    temp += ((((currentFB[width] & 0x3E) + (currentFB[-1] & 0x3E) + (temp_a1 & 0x3E) + (temp_v1 & 0x3E)) >> 2) & 0x3E);
                    *currentFB2 = temp;
                    currentFB2 += 1;
                    currentFB += 1;
                }
                s8 += 1;
            } while (s8 != height);
        }
    } else {
        s8 = var_v0;
        currentFB = &(&(&gFramebufferCurrent[width * var_v0])[arg1])[arg2 * width];
        currentFB[-1] = 0;
        currentFB2 = &gFramebufferCurrent[width * var_v0];
        if (s8 < height) {
            do {
                j = 0;
                while (j < width) {
                    v0 = 0xFFFF >> (0x10 - arg0);
                    // apparently this is v0 * 0x842
                    v0 = ~((v0 << 0xB) + (v0 << 6) + (v0 << 1));
                    temp = (func_8003FB88(currentFB[-1], *(currentFB - width), currentFB[1], currentFB[width]) & v0);
                    temp >>= arg0;
                    j += 1;
                    currentFB2[0] = temp;
                    currentFB2 += 1;
                    currentFB += 1;
                }
                s8 += 1;
            } while (s8 != height);
        }
    }
}
#endif

u16 func_8003F928(u16* arg0, s32 arg1) {
    u32 temp;
    u32 temp2;
    u32 temp3;

    temp = (arg0[0] & 0xF800) + (arg0[1] & 0xF800) + (arg0[2] & 0xF800);
    temp2 = (arg0[0] & 0x7C0) + (arg0[1] & 0x7C0) + (arg0[2] & 0x7C0);
    temp3 = (arg0[0] & 0x3E) + (arg0[1] & 0x3E) + (arg0[2] & 0x3E);

    arg0 += arg1;
    temp += (arg0[0] & 0xF800) + (arg0[1] & 0xF800) + (arg0[2] & 0xF800);
    temp2 += (arg0[0] & 0x7C0) + (arg0[1] & 0x7C0) + (arg0[2] & 0x7C0);
    temp3 += (arg0[0] & 0x3E) + (arg0[1] & 0x3E) + (arg0[2] & 0x3E);

    arg0 += arg1;
    temp += (arg0[0] & 0xF800) + (arg0[1] & 0xF800) + (arg0[2] & 0xF800);
    temp2 += (arg0[0] & 0x7C0) + (arg0[1] & 0x7C0) + (arg0[2] & 0x7C0);
    temp3 += (arg0[0] & 0x3E) + (arg0[1] & 0x3E) + (arg0[2] & 0x3E);

    temp /= 9;
    temp2 /= 9;
    temp3 /= 9;
    return (temp & 0xF800) + (temp2 & 0x7C0) + (temp3 & 0x3E);
}

u16 func_8003FA88(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4) {
    u32 temp;
    u32 temp2;
    u32 temp3;

    temp = arg0 & 0xF800;
    temp += (((((arg1 & 0xF800) >> 0xB) - ((arg2 & 0xF800) >> 0xB)) + ((arg3 & 0xF800) >> 0xB)) - ((arg4 & 0xF800) >> 0xB)) << 0xB;

    temp2 = arg0 & 0x7C0;
    temp2 += (((((arg1 & 0x7C0) >> 6) - ((arg2 & 0x7C0) >> 6)) + ((arg3 & 0x7C0) >> 6)) - ((arg4 & 0x7C0) >> 6)) << 6;

    temp3 = arg0 & 0x3E;
    temp3 += (((((arg1 & 0x3E) >> 1) - ((arg2 & 0x3E) >> 1)) + ((arg3 & 0x3E) >> 1)) - ((arg4 & 0x3E) >> 1)) << 1;

    return (((u16)temp & 0xF800) + ((u16)temp2 & 0x7C0) + ((u16)temp3 & 0x3E));
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/segment_3F5F0/func_8003FB88.s")
#else
u16 func_8003FB88(u16 arg0, u16 arg1, u16 arg2, u16 arg3) {
    u16 temp;
    u16 temp2;
    u16 temp3;
    temp = (((u32)arg0 & 0xF800) + (arg1 & 0xF800) + (arg2 & 0xF800) + (arg3 & 0xF800) >> 2) & 0xF800;
    temp2 = ((arg0 & 0x7C0) + (arg1 & 0x7C0) + (arg2 & 0x7C0) + (arg3 & 0x7C0) >> 2) & 0x7C0;
    temp3 = ((arg0 & 0x3E) + (arg1 & 0x3E) + (arg2 & 0x3E) + (arg3 & 0x3E) >> 2) & 0x3E;
    return temp + temp2 + temp3;
}
#endif

u16 func_8003FC3C(u16 arg0, u16 arg1, u16 arg2, u16 arg3) {
    return (((((arg0 & ~0x842) >> 1) + ((arg1 & ~0x842) >> 1)) & ~0x842) >> 1) + (((((arg2 & ~0x842) >> 1) + ((arg3 & ~0x842) >> 1)) & ~0x842) >> 1);
}

u16 func_8003FCBC(u16 arg0, u16 arg1) {
    s32 temp_a3;
    s32 temp_v1;
    u16 var_a0;
    u16 var_a2;
    u16 var_a3;

    temp_v1 = arg1 & 0xF800;
    temp_a3 = arg0 & 0xF800;
    if (temp_v1 < temp_a3) {
        var_a0 = temp_a3;
    } else {
        var_a0 = temp_v1;
    }
    temp_v1 = arg1 & 0x7C0;
    temp_a3 = arg0 & 0x7C0;
    if (temp_v1 < temp_a3) {
        var_a3 = temp_a3;
    } else {
        var_a3 = temp_v1;
    }
    temp_v1 = arg1 & 0x3E;
    temp_a3 = arg0 & 0x3E;
    if (temp_v1 < temp_a3) {
        var_a2 = temp_a3;
    } else {
        var_a2 = temp_v1;
    }
    return (var_a0 + var_a3 + var_a2);
}

u16 func_8003FD48(u16 arg0, u16 arg1, u16 arg2, u16 arg3) {
    s32 temp_t2;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_a3;
    s32 var_t2;
    s32 var_v0;
    s32 var_v1;

    temp_v1 = arg0 & 0xF800;
    temp_t2 = arg1 & 0xF800;
    if (temp_v1 < temp_t2) {
        var_v0 = (u16)temp_v1;
    } else {
        var_v0 = (u16)temp_t2;
    }

    temp_v1 = arg0 & 0x7C0;
    temp_t2 = arg1 & 0x7C0;
    if (temp_v1 < temp_t2) {
        var_v1 = (u16)temp_v1;
    } else {
        var_v1 = (u16)temp_t2;
    }

    temp_v1 = arg0 & 0x3E;
    temp_t2 = arg1 & 0x3E;
    if (temp_v1 < temp_t2) {
        var_a2 = (u16)temp_v1;
    } else {
        var_a2 = (u16)temp_t2;
    }

    temp_v1 = arg2 & 0xF800;
    temp_t2 = arg3 & 0xF800;
    if (temp_v1 < temp_t2) {
        var_a3 = (u16)temp_v1;
    } else {
        var_a3 = (u16)temp_t2;
    }

    temp_v1 = arg2 & 0x7C0;
    temp_t2 = arg3 & 0x7C0;
    if (temp_v1 < temp_t2) {
        var_t2 = (u16)temp_v1;
    } else {
        var_t2 = (u16)temp_t2;
    }

    temp_v1 = arg2 & 0x3E;
    temp_t2 = arg3 & 0x3E;
    if (temp_v1 < temp_t2) {
        var_a0 = (u16)temp_v1;
    } else {
        var_a0 = (u16)temp_t2;
    }

    var_a1 = var_v0;
    if (var_a3 < var_v0) {
        var_a1 = (u16)var_a3;
    }

    var_a3 = var_v1;
    if (var_t2 < var_v1) {
        var_a3 = (u16)var_t2;
    }

    var_v1 = var_a2;
    if (var_a0 < var_a2) {
        var_v1 = (u16)var_a0;
    }
    return (var_a1 + var_a3 + var_v1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_3F5F0/func_8003FE70.s")

void func_800402F8(u16* arg0, u16* arg1, s32 arg2, s32 arg3, s32 arg4) {
    u16 i;
    u16 j;

    for (i = 0; i < arg3; i++) {
        for (j = 0; j < arg2; j++) {
            arg0[0] = ((arg1[0] & (1 << arg4) | arg0[0] & ~(1 << arg4)));
            arg0 += 1;
            arg1 += 1;
        }
    }
}

void func_80040378(u16 *fb1, u16 *fb2, s32 width, s32 height) {
    func_8005324C(fb1, fb2, width * height >> 2);
}

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/segment_3F5F0/func_800403AC.s")
#else
// https://decomp.me/scratch/0S8QE
void func_800403AC(u16* arg0, u16* arg1, u32 arg2, u32 arg3, s32 arg4) {
    s32 i;
    s32 j;
    u16 temp;
    u16 temp2;
    u16 temp3;
    u16 temp4;
    u16 temp5;

    for (i = 0; i != arg3; i++) {
        for (j = 0; j != arg2; j++) {
            temp4 = arg0[0];
            temp5 = arg1[0];
            temp = ((u32) ((((temp4 & 0xF800) << arg4) + (temp5 & 0xF800)) - (temp4 & 0xF800)) >> arg4);
            temp2 = ((u32) ((((temp4 & 0x7C0) << arg4) + (temp5 & 0x7C0)) - (temp4 & 0x7C0)) >> arg4);
            temp3 = ((u32) ((((temp4 & 0x3E) << arg4) + (temp5 & 0x3E)) - (temp4 & 0x3E)) >> arg4);
            arg0[0] = (temp & 0xF800) + (temp2 & 0x7C0) + (temp3 & 0x3E);
            arg0 += 1;
            arg1 += 1;
        }
    }
}
#endif

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/segment_3F5F0/func_80040468.s")
#else
// https://decomp.me/scratch/NGTZD
void func_80040468(u16* arg0, u16* arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp_s2;
    s32 temp_s4;
    s32 temp_t0;
    s32 temp_t2;
    s32 temp_t3;
    s32 temp_t4;
    s32 temp_t5;
    s32 i;
    s32 j;
    u16 temp_a0;
    u16 temp_a1;

    for (i = 0; i != arg3; i++) {
        for (j = 0; j != arg2; j++) {
            temp_s4 = arg4 - 1;
            temp_a0 = arg0[0];
            temp_a1 = arg1[0];
            temp_t0 = temp_a0 & 0xF800;
            temp_t2 = temp_a1 & 0xF800;
            temp_t3 = temp_a0 & 0x7C0;
            temp_t4 = temp_a1 & 0x7C0;
            temp_t5 = temp_a0 & 0x3E;
            temp_s2 = temp_a1 & 0x3E;
            arg0[0] = (((u32) (((temp_t0 << arg4) + temp_t2) - temp_t0) >> arg4) & 0xF800) +
                (((u32) (((temp_t3 << arg4) + temp_t4) - temp_t3) >> arg4) & 0x7C0) +
                (((u32) (((temp_t5 << arg4) + temp_s2) - temp_t5) >> arg4) & 0x3E);
            arg1[0] = (((u32) (((temp_t2 << temp_s4) + temp_t0) - temp_t2) >> temp_s4) & 0xF800) +
                (((u32) (((temp_t4 << temp_s4) + temp_t3) - temp_t4) >> temp_s4) & 0x7C0) +
                (((u32) (((temp_s2 << temp_s4) + temp_t5) - temp_s2) >> temp_s4) & 0x3E);
            arg0 += 1;
            arg1 += 1;
        }
    }
}
#endif

void func_80040590(s32 arg0, s32 arg1) {
    s32 width;
    s32 height;
    s32 resolution;
    u16* currentFB;
    u16* nextFB;
    s32 var_s1;
    s32 var_s2;
    s32 i;

    resolution = vi_get_current_size();
    width = GET_VIDEO_WIDTH(resolution);
    height = GET_VIDEO_HEIGHT(resolution);
    nextFB = gFramebufferNext;
    currentFB = gFramebufferCurrent;
    for (i = 0; i < height; i++) {
        var_s1 = arg0 - (cos16(((((arg0 >> 1) + i) << 0xF) / (u32)(height >> 2))) >> 0xC);
        if ((width >> 1) < var_s1) {
            var_s1 = (width >> 1);
        }
        if (var_s1 < 0) {
            var_s1 = -var_s1;
        }
        if (arg1 < var_s1) {
            resolution = var_s1 - arg1;
        } else {
            resolution = 0;
        }

        if (var_s1 < arg1) {
            // @fake
            if (1) {}
            var_s2 = var_s1;
        } else {
            var_s2 = arg1;
        }
        func_8003F2C4(&currentFB[resolution], currentFB[resolution], nextFB[var_s1], var_s2);
        func_8003F2C4(&currentFB[width] - var_s1, *(&nextFB[width] - var_s1), *(&currentFB[width] - resolution), var_s2);
        bcopy(&nextFB[var_s1], &currentFB[var_s1], (width - (var_s1 << 1)) << 1);
        currentFB += width;
        nextFB += width;
    }
}

void func_80040754(u16* arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 j;
    s32 i;
    s32 end;

    end = arg1 - arg3;
    for (i = 0; i < arg2; i++) {
        arg0 += arg3;
        for (j = 0; j < end; j++) {
            arg0[0] = ((arg0[0] & 0xF7BD) >> 1) + ((*(arg0 - arg3) & 0xF7BD) >> 1);
            arg0 += 1;
        }
    }
}

void func_80040870(u16* arg0, u16 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 j;
    s32 i;

    arg1 += (arg2 - arg4);
    for (i = 0; i < arg3; i++) {
        for (j = 0; j < arg4; j++) {
            arg0[0] = arg1[0];
            arg0++;
            arg1++;
        }
        arg1 += (arg2 - arg4);
        arg0 += (arg2 - arg4);
    }
}

void func_80040920(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_s2;
    s32 i;
    u16* temp_v1;

    for (i = 0; i < arg3; i++) {
        temp_s2 = rand_next(0, arg1);
        temp_v1 = (u16 *)((rand_next(0, arg2) * arg1 * 2) + arg0 + (temp_s2 * 2));
        temp_v1[0] &= ~1;
    }
}

void func_800409D0(u16 *fb, s32 width, s32 height) {
    // Sets center pixel alpha to 0
    fb[(width >> 1) + (height >> 1) * width] &= 0xFFFE;
}

void func_80040A04(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 i;
    u16* temp_v1;

    for (i = 0; i < arg3; i++) {
        temp_v1 = (u16 *)((rand_next(0, arg2) * arg1 * 2) + arg0);
        temp_v1[0] &= ~1;
    }
}

void func_80040A94(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 i;
    u16* temp_v1;

    for (i = 0; i < arg3; i++) {
        temp_v1 = (u16 *)((rand_next(0, arg2) * arg1 * 2) + arg0 + (arg1 * 2));
        temp_v1[0] &= ~1;
    }
}

/**
 * Sets the alpha channel of the following corners of the given buffer to 0:
 * X----------*X
 * |          |
 * |          |
 * |          |
 * *----------*
 * X           X
 *
 * Alternatively:
 * X----------*
 * X          |
 * |          |
 * |          |
 * *----------*
 * X
 * X
 */
void show_framebuffer_corners_kinda(u16 *framebuffer, s32 width, s32 height) {
    // Top right + 1???
    (framebuffer + width)[0] &= -2;

    // Top left
    framebuffer[0] &= -2;

    // Bottom right + extra row???
    (framebuffer + width)[height * width] &= -2;

    // Bottom right + 1???
    framebuffer[height * width] &= -2;
}

void func_80040B8C(u16* arg0, u16* arg1, u16* arg2, s32 arg3, s32 arg4) {
    s32 i;
    s32 j;

    for (i = 0; i < arg3; i++) {
        for (j = 0; j < arg4; j++, arg0++, arg1++, arg2++) {
            if (~arg2[0] & 1) {
                *arg0 = arg1[0];
                *arg0 &= ~1;
            }
        }
    }
}

void func_80040CD0(u16* arg0, u16* arg1, s32 arg2, s32 arg3) {
    s32 temp_a2_2;
    s32 j;
    s32 var_fp;
    s32 var_s5;
    s32 i;
    u16 sp5C[2][0x280];
    s32 var_s7;

    var_fp = 0;
    var_s7 = rand_next(0, 3) - 1;
    if (!var_s7) {
        var_s7 = 1;
    }
    var_s5 = (rand_next(0, 3) - 1) * arg3;
    for (i = 0; i < arg2; i++) {
        for (j = 0; j < arg3; j++) {
            if ((~*(arg0 + var_s7) & 1) + (~*(arg0 + var_s5) & 1) + (~*((arg0 - var_s5) + var_s7) & 1)) {
                if (~arg1[0] & 1) {
                    sp5C[var_fp][j] = arg1[0];
                } else {
                    sp5C[var_fp][j] = ((arg0[0] & ~0x842) >> 1) + ((arg1[0] & ~0x842) >> 1);
                    arg1[0] &= ~1;
                }
                sp5C[var_fp][j] &= ~1;
            } else {
                sp5C[var_fp][j] = arg0[0];
            }
            arg0++;
            arg1++;
        }

        var_s7 = rand_next(0, 3) - 1;
        if (!var_s7) {
            var_s7 = 1;
        }
        temp_a2_2 = arg3 << 1;
        var_s5 = (rand_next(0, 3) - 1) * arg3;
        var_fp ^= 1;
        if (i != 0) {
            bcopy(&sp5C[var_fp], arg0 - temp_a2_2, temp_a2_2);
        }
    }
    var_fp ^= 1;
    bcopy(&sp5C[var_fp], arg0 - arg3, arg3 << 1);
}

void func_80040EFC(u16* arg0, u16* arg1, s32 arg2, s32 arg3) {
    s32 i;

    for (i = 0; i < (arg2 * arg3); i++) {
        arg0[0] |= 1;
        arg0++;
        arg1[0] |= 1;
        arg1++;
    }
}
