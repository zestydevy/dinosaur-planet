#include "PR/ultratypes.h"
#include "sys/camera.h"
#include "sys/framebuffer_fx.h"
#include "sys/gfx/gx.h"
#include "sys/rand.h"
#include "sys/rcp.h"
#include "sys/rsp_segment.h"
#include "sys/segment_535C0.h"

static const char str_8009a480[] = "Freak a0 %x  a1 %x \n";
static const char str_8009a498[] = " size %x ";

// -------- .data start 80092a50 -------- //
s32 gFbfxEffectID = 0;
s32 gFbfxEffectDuration = 0;
// -------- .data end 80092a60 -------- //

// .bss 800b49e0-800b49f0

void fbfx_do_effect(Gfx **gdl, s32 duration, s32 effectID, s32);
void fbfx_motion_blur(u16 *fb1, u16 *fb2, s32 width, s32 height);
void fbfx_func_800403AC(u16 *fb1, u16 *fb2, s32 width, s32 height, s32 arg4);
void fbfx_sine_waves(s32 arg0, s32 arg1);
void fbfx_func_80040754(u16 *fb, s32 width, s32 height, s32 arg3);
void fbfx_func_80040870(u16 *fb1, u16 *fb2, s32 width, s32 height, s32 arg4);
void fbfx_burn_paper_random_setup(u16* fb, s32 fbWidth, s32 fbHeight, s32 count);
void fbfx_burn_paper_center_setup(u16 *fb, s32 width, s32 height);
void fbfx_burn_paper_right_setup(u16* fb, s32 width, s32 height, s32 count);
void fbfx_burn_paper_corner_setup(u16 *framebuffer, s32 width, s32 height);
void fbfx_burn_paper(u16* fb1, u16* fb2, s32 width, s32 height);
void fbfx_fill_full_alpha(u16 *fb1, u16 *fb2, s32 width, s32 height);
u16 fbfx_func_8003FD48(u16, u16, u16, u16);
u16 fbfx_func_8003FB88(u16, u16, u16, u16);
void fbfx_func_8003F2C4(u16*, u16, u16, s32);
void fbfx_func_8003EC8C(u16*, u16 *, s32);
void weird_resize_copy(u16 *src, s32 srcWidth, s32 destWidth, u16 *dest);

void fbfx_tick(Gfx **gdl, s32 updateRate) {
    static s32 gFbfxTimer;

    if (gFbfxEffectID > FBFX_NONE) {
        if (gFbfxEffectID == FBFX_MOTION_BLUR && gFbfxTimer == 0) {
            gFbfxTimer = gFbfxEffectDuration;
        }
        fbfx_do_effect(gdl, gFbfxEffectDuration, gFbfxEffectID, camera_get_letterbox());
        if (gFbfxEffectID == FBFX_MOTION_BLUR) {
            gFbfxTimer -= updateRate;
            if (gFbfxTimer <= 0) {
                gFbfxEffectID = FBFX_NONE;
                gFbfxTimer = 0;
            }
        } else {
            gFbfxEffectID = FBFX_NONE;
        }
        return;
    } else {
        gFbfxTimer = 0;
    }
}

void fbfx_func_8003EAC0(u16 *arg0, s32 arg1, s32 arg2, u16* arg3) {
    s32 var_a3;

    if (arg2 > 0) {
        fbfx_func_8003EC8C(&arg3[arg2], arg0, arg1 - arg2);
        bzero(arg3, (arg2 + 1) << 1);
        var_a3 = arg2 >= 0xB ? 0xA : arg2;
        fbfx_func_8003F2C4(&arg3[arg2] - var_a3, 0, arg3[arg2 + 1], var_a3);
        return;
    }

    arg2 = -arg2;
    fbfx_func_8003EC8C(arg3, &arg0[arg2], arg1 - arg2);
    bzero(&arg3[arg1 - arg2], arg2 << 1);
    var_a3 = arg2 >= 0xB ? 0xA : arg2;
    fbfx_func_8003F2C4(&arg3[arg1] - arg2, (&arg3[arg1] - arg2)[-2], 0, var_a3);
}

void fbfx_func_8003EBD4(s32 hOffset) {
    s32 height;
    s32 resolution;
    s32 width;
    u16* nextFB;
    u16* currentFB;

    resolution = vi_get_current_size();
    width = GET_VIDEO_WIDTH(resolution);
    height = GET_VIDEO_HEIGHT(resolution);
    nextFB = gBackFramebuffer;
    currentFB = gFrontFramebuffer;
    hOffset = width - hOffset;
    while (height--) {
        weird_resize_copy(&nextFB[hOffset], width - (hOffset << 1), width, currentFB);
        nextFB += width;
        currentFB += width;
    }
}

void fbfx_func_8003EC8C(u16* arg0, u16* arg1, s32 arg2) {
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

void fbfx_func_8003ED00(u16* arg0, u16* arg1, s32 arg2) {
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
#pragma GLOBAL_ASM("asm/nonmatchings/framebuffer_fx/fbfx_func_8003EF30.s")
#else
// https://decomp.me/scratch/KWLGK
void fbfx_func_8003EF30(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, void (*arg6)(u16*, s32, s32, u16*)) {
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
    currentFB = gFrontFramebuffer;
    nextFB = gBackFramebuffer;
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

void fbfx_func_8003F074(u16* arg0, s32 arg1, s32 arg2, u16* arg3) {
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
    fbfx_func_8003F2C4(&arg3[arg2] - var_a3, 0, arg3[arg2 + 1], var_a3);
    fbfx_func_8003F2C4(&arg3[arg1] - arg2, (&arg3[arg1] - arg2)[-2], 0, var_a3);
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

void fbfx_func_8003F2C4(u16* arg0, u16 arg1, u16 arg2, s32 arg3) {
    u16 temp;
    u16 temp2;
    u16 temp3;
    u16 temp4;
    u16 temp5;
    u16 temp6;

    if (arg3 == 0) {
        return;
    }

    temp4 = ((((arg2 & 0xF800) >> 0xB) - ((arg1 & 0xF800) >> 0xB)) << 8) / arg3;
    temp5 = ((((arg2 & 0x7C0) >> 6) - ((arg1 & 0x7C0) >> 6)) << 8) / arg3;
    temp6 = ((((arg2 & 0x3E) >> 1) - ((arg1 & 0x3E) >> 1)) << 8) / arg3;
    temp = ((arg1 & 0xF800) >> 0xB) << 8;
    temp2 = ((arg1 & 0x7C0) >> 6) << 8;
    temp3 = ((arg1 & 0x3E) >> 1) << 8;  
    while (arg3--) {
        temp += temp4, temp2 += temp5, temp3 += temp6;
        arg0[0] = ((temp >> 8) << 0xB) + ((temp2 >> 8) << 6) + ((temp3 >> 8) << 1);
        arg0++;
    }
}


void fbfx_play(s32 effectID, s32 duration) {
    gFbfxEffectID = effectID;
    gFbfxEffectDuration = duration;
}

void fbfx_func_8003F4C0(s32 arg0) {
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
        nextFB = gBackFramebuffer;
        nextFB[-1] = 0;
        currentFB = gFrontFramebuffer;
        for (i = 0; i < height - 1; i++) {
            for (j = 0; j < width; j++) {
                currentFB[0] = fbfx_func_8003FD48(currentFB[0], *(nextFB - width), nextFB[0], nextFB[width]);
                currentFB += 1;
                nextFB += 1;
            }
        }
    } else {
        nextFB = gBackFramebuffer;
        nextFB[-1] = 0;
        currentFB = gFrontFramebuffer;
        for (i = 0; i < height - 1; i++) {
            for (j = 0; j < width; j++) {
                v0 = 0xFFFF >> (0x10 - arg0);
                // apparently this is v0 * 0x842
                v0 = ~((v0 << 0xB) + (v0 << 6) + (v0 << 1));
                currentFB[0] = (fbfx_func_8003FD48(currentFB[0], *(nextFB - width), *nextFB, nextFB[width]) &  v0) >> arg0;
                currentFB += 1;
                nextFB += 1;
            }
        }
    }
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/framebuffer_fx/fbfx_func_8003F660.s")
#else
// https://decomp.me/scratch/wArrS
void fbfx_func_8003F660(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 width;
    s32 height;
    s32 resolution;
    s32 i;
    s32 j;
    s32 var_v0;
    s32 temp_a1;
    s32 temp_v1;
    u16* currentFB;
    u16* currentFB2;

    s32 otherTemp;
    u16 temp;
    u16 temp2;
    u16 temp3;
    u16 temp4;
    u16 temp5;
    u16 temp6;

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
        currentFB = &(&(&gFrontFramebuffer[width * var_v0])[arg1])[arg2 * width];
        currentFB[-1] = 0;
        currentFB2 = &gFrontFramebuffer[width * var_v0];
        for (i = var_v0; i < height; i++) {
            for (j = 0; j < width; j++) {
                temp_v1 = currentFB[width];
                temp_a1 = (currentFB - width)[0];
                if (1){}
                temp = (((u32)temp_v1 & 0xF800) + (temp_a1 & 0xF800) + (currentFB[1] & 0xF800) + (currentFB[-1] & 0xF800)) >> 2;
                temp2 = (((s32)temp_v1 & 0x7C0) + (temp_a1 & 0x7C0) + (currentFB[1] & 0x7C0) + (currentFB[-1] & 0x7C0)) >> 2;
                temp3 = (((s32)temp_v1 & 0x3E) + (temp_a1 & 0x3E) + (currentFB[1] & 0x3E) + (currentFB[-1] & 0x3E)) >> 2;
                currentFB2[0] = (temp & 0xF800) + (temp2 & 0x7C0) + (temp3 & 0x3E);
                currentFB2++;
                currentFB++;
            }
        }
    } else {
        currentFB = &(&(&gFrontFramebuffer[width * var_v0])[arg1])[arg2 * width];
        currentFB[-1] = 0;
        currentFB2 = &gFrontFramebuffer[width * var_v0];
        for (i = var_v0; i < height; i++) {
            for (j = 0; j < width; j++) {
                var_v0 = 0xFFFF >> (0x10 - arg0);
                // apparently this is var_v0 * 0x842
                var_v0 = ~((var_v0 << 0xB) + (var_v0 << 6) + (var_v0 << 1));
                otherTemp = fbfx_func_8003FB88(currentFB[-1], *(currentFB - width), currentFB[1], currentFB[width]) & var_v0;
                currentFB2[0] = otherTemp >> arg0;
                currentFB2++;
                currentFB++;
            }
        }
    }
}
#endif

u16 fbfx_func_8003F928(u16* arg0, s32 arg1) {
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

u16 fbfx_func_8003FA88(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4) {
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

u16 fbfx_func_8003FB88(u16 arg0, u16 arg1, u16 arg2, u16 arg3) {
    u16 r;
    u16 g;
    u16 b;

    r = (u32)((arg0 & 0xF800) + (arg1 & 0xF800) + (arg2 & 0xF800) + (arg3 & 0xF800)) >> 2;
    g = ((arg0 & 0x7C0) + (arg1 & 0x7C0) + (arg2 & 0x7C0) + (arg3 & 0x7C0)) >> 2;
    b = ((arg0 & 0x3E) + (arg1 & 0x3E) + (arg2 & 0x3E) + (arg3 & 0x3E)) >> 2;

    return (r & 0xF800) + (g & 0x7C0) + (b & 0x3E);
}

u16 fbfx_func_8003FC3C(u16 arg0, u16 arg1, u16 arg2, u16 arg3) {
    return (((((arg0 & ~0x842) >> 1) + ((arg1 & ~0x842) >> 1)) & ~0x842) >> 1) + (((((arg2 & ~0x842) >> 1) + ((arg3 & ~0x842) >> 1)) & ~0x842) >> 1);
}

u16 fbfx_func_8003FCBC(u16 arg0, u16 arg1) {
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

u16 fbfx_func_8003FD48(u16 arg0, u16 arg1, u16 arg2, u16 arg3) {
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

void fbfx_do_effect(Gfx **gdl, s32 duration, s32 effectID, s32 arg3) {
    s32 i;
    s32 fadeOutDuration;
    s32 viSize;
    s32 viWidth;
    s32 viHeight;
    s32 var_v1_2;
    s32 var_a0;
    s32 var_fp;
    s32 var_s7;

    var_s7 = 0;
    var_fp = 0;
    fadeOutDuration = (s32) ((f32) duration - 12.0f);
    if (fadeOutDuration < 0) {
        fadeOutDuration = 0;
    }
    viSize = vi_get_current_size();
    viWidth = GET_VIDEO_WIDTH(viSize);
    viHeight = GET_VIDEO_HEIGHT(viSize);
    gfxtask_wait();
    switch (effectID) {
    case FBFX_NOOP: // nop
        break;
    case FBFX_BURN_PAPER_RANDOM: // burn away (from random points) (setup)
        fbfx_fill_full_alpha(gFrontFramebuffer, gBackFramebuffer, viWidth, viHeight);
        fbfx_burn_paper_random_setup(gFrontFramebuffer, viWidth, viHeight, 10);
        break;
    case FBFX_BURN_PAPER_CENTER: // burn away (from center) (setup)
        fbfx_fill_full_alpha(gFrontFramebuffer, gBackFramebuffer, viWidth, viHeight);
        fbfx_burn_paper_center_setup(gFrontFramebuffer, viWidth, viHeight);
        break;
    case FBFX_BURN_PAPER_RIGHT: // burn away (from right side) (setup)
        fbfx_fill_full_alpha(gFrontFramebuffer, gBackFramebuffer, viWidth, viHeight);
        fbfx_burn_paper_right_setup(gFrontFramebuffer, viWidth, viHeight, 4);
        break;
    case FBFX_BURN_PAPER_CORNERS: // burn away (from corners) (setup)
        fbfx_fill_full_alpha(gFrontFramebuffer, gBackFramebuffer, viWidth, viHeight);
        fbfx_burn_paper_corner_setup(gFrontFramebuffer, viWidth, viHeight);
        break;
    case FBFX_FADE_OUT_RIGHT_FADE_IN:  // fade to black (smear right) -> then back in (setup)
        var_s7 = -1;
        break;
    case FBFX_FADE_OUT_LEFT_FADE_IN: // fade to black (smear left) -> then back in (setup)
        var_s7 = 1;
        break;
    case FBFX_FADE_OUT_DOWN_FADE_IN: // fade to black (smear down) -> then back in (setup)
        var_fp = -1;
        break;
    case FBFX_FADE_OUT_UP_FADE_IN: // fade to black (smear up) -> then back in (setup)
        var_fp = 1;
        break;
    case FBFX_MOTION_BLUR: // accumulation motion blur
        // - at this point, gfxtask_wait ensured that the rcp is done writing to gBackFramebuffer.
        // - gBackFramebuffer is the next frame to be displayed, gFrontFramebuffer is the current.
        // - average the pixels of both and write back to gBackFramebuffer (to be displayed next).
        fbfx_motion_blur(gBackFramebuffer, gFrontFramebuffer, viWidth, viHeight);
        return;
    }

    for (i = 0; i < duration; i++) {
        var_v1_2 = (duration - i) / 3;
        if (var_v1_2 >= 5) {
            var_v1_2 = -1;
        }
        // This segment command is unnecessary and can cause a display list buffer overflow if the
        // framebuffer effect has too many iterations!
        rsp_segment(gdl, SEGMENT_FRAMEBUFFER, gFrontFramebuffer);
        switch (effectID) {
        case FBFX_SINE_WAVES: // 2 vertical sine waves moving from the center outward
            fbfx_sine_waves((viWidth >> 1) - ((s32) ((s32) (viWidth * (i + 1)) / duration) >> 1), viWidth >> 4);
            break;
        case FBFX_LERP: // modulate to next frame (does nothing while arg1 - var_t0 >= 15)
            if (var_v1_2 != -1) {
                fbfx_func_800403AC(gFrontFramebuffer, gBackFramebuffer, viWidth, viHeight, var_v1_2);
            }
            break;
        case FBFX_SLIDE: // slide next frame in from the left
            if ((i % 2)) {
                fbfx_func_80040754(gFrontFramebuffer, viWidth, viHeight, (s32) (viWidth * 2) / duration);
            } else {
                fbfx_func_80040870(gFrontFramebuffer, gBackFramebuffer, viWidth, viHeight, (s32) (viWidth * i) / duration);
            }
            break;
        case FBFX_BURN_PAPER_RANDOM: // burn away effect
        case FBFX_BURN_PAPER_CENTER: // burn away effect
        case FBFX_BURN_PAPER_RIGHT: // burn away effect
        case FBFX_BURN_PAPER_CORNERS: // burn away effect
            fbfx_burn_paper(gFrontFramebuffer, gBackFramebuffer, viHeight, viWidth);
            break;
        case FBFX_FADE_OUT_FADE_IN:  // fade to black (blurry/smear) -> then back in
        case FBFX_FADE_OUT_RIGHT_FADE_IN: // fade to black (smear right) -> then back in
        case FBFX_FADE_OUT_LEFT_FADE_IN: // fade to black (smear left) -> then back in
        case FBFX_FADE_OUT_DOWN_FADE_IN: // fade to black (smear down) -> then back in
        case FBFX_FADE_OUT_UP_FADE_IN: // fade to black (smear up) -> then back in
            if (i < fadeOutDuration) {
                if (fadeOutDuration < i) { // what
                    var_a0 = 1;
                } else {
                    var_a0 = 0;
                }
                fbfx_func_8003F660(var_a0, var_s7, var_fp, arg3);
            } else {
                fbfx_func_800403AC(gFrontFramebuffer, gBackFramebuffer, viWidth, viHeight, 2);
            }
            break;
        case FBFX_FADE_OUT: // fade to black (blurry/smear)
            fbfx_func_8003F660(0, var_s7, var_fp, arg3);
            break;
        }
    }
    return;
}

void fbfx_func_800402F8(u16* arg0, u16* arg1, s32 arg2, s32 arg3, s32 arg4) {
    u16 i;
    u16 j;

    for (i = 0; i < arg3; i++) {
        for (j = 0; j < arg2; j++) {
            arg0[0] = (((arg1[0] & (1 << arg4)) | (arg0[0] & ~(1 << arg4))));
            arg0 += 1;
            arg1 += 1;
        }
    }
}

void fbfx_motion_blur(u16 *fb1, u16 *fb2, s32 width, s32 height) {
    func_8005324C(fb1, fb2, (width * height) >> 2);
}

// interpolating fb1 -> fb2: y = 1 / (2^x), where x is arg4 and y is the progress from fb1 to fb2
void fbfx_func_800403AC(u16 *fb1, u16 *fb2, s32 width, s32 height, s32 arg4) {
    s32 x;
    s32 y;
    u32 r;
    u32 g;
    u32 b;
    u32 px1;
    u32 px2;
    for (x = 0; x != height; x++) {
        for (y = 0; y != width; y++) {
          px1 = *fb1;
          px2 = *fb2;
          r = (((px1 & 0xF800) << arg4) + (px2 & 0xF800)) - (px1 & 0xF800);
          g = (((px1 & 0x7C0) << arg4) + (px2 & 0x7C0)) - (px1 & 0x7C0);
          b = (((px1 & 0x3E) << arg4) + (px2 & 0x3E)) - (px1 & 0x3E);
          r >>= arg4;
          g >>= arg4;
          b >>= arg4;
          *fb1 = (r & 0xF800) + (g & 0x7C0) + (b & 0x3E);
          fb1 += 1;
          fb2 += 1;
        }
    }
}

// cross blend
void fbfx_func_80040468(u16* arg0, u16* arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 i;
    s32 j;
    u32 temp_a0;
    u32 temp_a1;
    u32 temp;
    u32 temp2;
    u32 temp3;
    u32 temp4;
    u32 temp5;
    u32 temp6;

    for (i = 0; i != arg3; i++) {
        for (j = 0; j != arg2; j++) {
            temp_a0 = arg0[0];
            temp_a1 = arg1[0];
            temp = ((((temp_a0 & 0xF800) << arg4) + (temp_a1 & 0xF800)) - (temp_a0 & 0xF800));
            temp2 = ((((temp_a0 & 0x7C0) << arg4) + (temp_a1 & 0x7C0)) - (temp_a0 & 0x7C0));
            temp3 = ((((temp_a0 & 0x3E) << arg4) + (temp_a1 & 0x3E)) - (temp_a0 & 0x3E));
            temp >>= arg4;
            temp2 >>= arg4;
            temp3 >>= arg4;
            temp4 = ((((temp_a1 & 0xF800) << (arg4 - 1)) + (temp_a0 & 0xF800)) - (temp_a1 & 0xF800));
            temp5 = ((((temp_a1 & 0x7C0) << (arg4 - 1)) + (temp_a0 & 0x7C0)) - (temp_a1 & 0x7C0));
            temp6 = ((((temp_a1 & 0x3E) << (arg4 - 1)) + (temp_a0 & 0x3E)) - (temp_a1 & 0x3E));
            temp4 >>= (arg4 - 1);
            temp5 >>= (arg4 - 1);
            temp6 >>= (arg4 - 1);
            arg0[0] = (temp & 0xF800) + (temp2 & 0x7C0) + (temp3 & 0x3E);
            arg1[0] = (temp4 & 0xF800) + (temp5 & 0x7C0) + (temp6 & 0x3E);
            arg0 += 1;
            arg1 += 1;
        }
    }
}

void fbfx_sine_waves(s32 arg0, s32 arg1) {
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
    nextFB = gBackFramebuffer;
    currentFB = gFrontFramebuffer;
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
        fbfx_func_8003F2C4(&currentFB[resolution], currentFB[resolution], nextFB[var_s1], var_s2);
        fbfx_func_8003F2C4(&currentFB[width] - var_s1, *(&nextFB[width] - var_s1), *(&currentFB[width] - resolution), var_s2);
        bcopy(&nextFB[var_s1], &currentFB[var_s1], (width - (var_s1 << 1)) << 1);
        currentFB += width;
        nextFB += width;
    }
}

void fbfx_func_80040754(u16 *fb, s32 width, s32 height, s32 arg3) {
    s32 j;
    s32 i;
    s32 end;

    end = width - arg3;
    for (i = 0; i < height; i++) {
        fb += arg3;
        for (j = 0; j < end; j++) {
            fb[0] = ((fb[0] & 0xF7BD) >> 1) + ((*(fb - arg3) & 0xF7BD) >> 1);
            fb += 1;
        }
    }
}

void fbfx_func_80040870(u16 *fb1, u16 *fb2, s32 width, s32 height, s32 arg4) {
    s32 j;
    s32 i;

    fb2 += (width - arg4);
    for (i = 0; i < height; i++) {
        for (j = 0; j < arg4; j++) {
            fb1[0] = fb2[0];
            fb1++;
            fb2++;
        }
        fb2 += (width - arg4);
        fb1 += (width - arg4);
    }
}

void fbfx_burn_paper_random_setup(u16* fb, s32 fbWidth, s32 fbHeight, s32 count) {
    s32 x;
    s32 i;
    u16* pixelPtr;

    for (i = 0; i < count; i++) {
        x = rand_next(0, fbWidth);
        pixelPtr = (u16 *)((rand_next(0, fbHeight) * fbWidth * 2) + (s32)fb + (x * 2));
        *pixelPtr &= ~1;
    }
}

void fbfx_burn_paper_center_setup(u16 *fb, s32 width, s32 height) {
    // Sets center pixel alpha to 0
    fb[(width >> 1) + (height >> 1) * width] &= ~1;
}

void fbfx_burn_paper_right_setup(u16* fb, s32 width, s32 height, s32 count) {
    s32 i;
    u16* temp_v1;

    for (i = 0; i < count; i++) {
        temp_v1 = (u16 *)((rand_next(0, height) * width * 2) + (s32)fb);
        temp_v1[0] &= ~1;
    }
}

void fbfx_func_80040A94(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 i;
    u16* temp_v1;

    for (i = 0; i < arg3; i++) {
        temp_v1 = (u16 *)((rand_next(0, arg2) * arg1 * 2) + arg0 + (arg1 * 2));
        temp_v1[0] &= ~1;
    }
}

void fbfx_burn_paper_corner_setup(u16 *framebuffer, s32 width, s32 height) {
    // @bug? these are off by one?
    (framebuffer + width)[0] &= -2;
    framebuffer[0] &= -2;
    (framebuffer + width)[height * width] &= -2;
    framebuffer[height * width] &= -2;
}

void fbfx_func_80040B8C(u16* arg0, u16* arg1, u16* arg2, s32 arg3, s32 arg4) {
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

void fbfx_burn_paper(u16* frontFb, u16* backFb, s32 height, s32 width) {
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
    var_s5 = (rand_next(0, 3) - 1) * width;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if ((~*(frontFb + var_s7) & 1) + (~*(frontFb + var_s5) & 1) + (~*((frontFb - var_s5) + var_s7) & 1)) {
                if (~backFb[0] & 1) {
                    sp5C[var_fp][j] = backFb[0];
                } else {
                    sp5C[var_fp][j] = ((frontFb[0] & ~0x842) >> 1) + ((backFb[0] & ~0x842) >> 1);
                    backFb[0] &= ~1;
                }
                sp5C[var_fp][j] &= ~1;
            } else {
                sp5C[var_fp][j] = frontFb[0];
            }
            frontFb++;
            backFb++;
        }

        var_s7 = rand_next(0, 3) - 1;
        if (!var_s7) {
            var_s7 = 1;
        }
        temp_a2_2 = width << 1;
        var_s5 = (rand_next(0, 3) - 1) * width;
        var_fp ^= 1;
        if (i != 0) {
            bcopy(&sp5C[var_fp], frontFb - temp_a2_2, temp_a2_2);
        }
    }
    var_fp ^= 1;
    bcopy(&sp5C[var_fp], frontFb - width, width << 1);
}

void fbfx_fill_full_alpha(u16 *fb1, u16 *fb2, s32 width, s32 height) {
    s32 i;

    for (i = 0; i < (width * height); i++) {
        *fb1 |= 1;
        fb1++;
        *fb2 |= 1;
        fb2++;
    }
}
