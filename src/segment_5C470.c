#include "common.h"

/** 
 * D_80092E9C could be one of the dimensions (height) for rendering dynamic shadow texture?
 * gPossiblyScreenWidthOrHeight might be the width!
 */
extern u32  D_80092E90[];
extern s32  D_80092E9C; // Usually 0x40 (64)
extern u32* D_800BB540[4];
extern u32* D_800BB550[];
extern u32  D_800BB558[];
extern u32* D_800BB560[4];
extern u32  D_800BB570[];
extern u32* D_800BB578[4];

void func_8005B870(void) {
    D_800BB540[0] = malloc((gPossiblyScreenWidthOrHeight * D_80092E9C << 1) + 0x30, 0x18, 0);
    D_800BB540[0] = (u32*)((u32)(D_800BB540[0]) + 0x3f & ~0x3f); //Makes sure framebuffer addresses are 64-byte-aligned
    D_800BB540[1] = malloc((gPossiblyScreenWidthOrHeight * D_80092E9C << 1) + 0x30, 0x18, 0);
    D_800BB540[1] = (u32*)((u32)(D_800BB540[1]) + 0x3f & ~0x3f);
    D_800BB540[2] = malloc((gPossiblyScreenWidthOrHeight * D_80092E9C << 1) + 0x30, 0x18, 0);
    D_800BB540[2] = (u32*)((u32)(D_800BB540[2]) + 0x3f & ~0x3f);
    D_800BB540[3] = malloc((gPossiblyScreenWidthOrHeight * D_80092E9C << 1) + 0x30, 0x18, 0);
    D_800BB540[3] = (u32*)((u32)(D_800BB540[3]) + 0x3f & ~0x3f);

    D_800BB560[0] = malloc(gPossiblyScreenWidthOrHeight * D_80092E9C << 1, 0x18, 0);
    D_800BB560[1] = malloc(gPossiblyScreenWidthOrHeight * D_80092E9C << 1, 0x18, 0);
    D_800BB560[2] = malloc(gPossiblyScreenWidthOrHeight * D_80092E9C << 1, 0x18, 0);
    D_800BB560[3] = malloc(gPossiblyScreenWidthOrHeight * D_80092E9C << 1, 0x18, 0);

    D_800BB578[0] = malloc(gPossiblyScreenWidthOrHeight * D_80092E9C >> 1, 0x18, 0);
    D_800BB578[1] = malloc(gPossiblyScreenWidthOrHeight * D_80092E9C >> 1, 0x18, 0);
    D_800BB578[2] = malloc(gPossiblyScreenWidthOrHeight * D_80092E9C >> 1, 0x18, 0);
    D_800BB578[3] = malloc(gPossiblyScreenWidthOrHeight * D_80092E9C >> 1, 0x18, 0);

    bzero(D_800BB560[0], gPossiblyScreenWidthOrHeight * D_80092E9C << 1);
    bzero(D_800BB560[1], gPossiblyScreenWidthOrHeight * D_80092E9C << 1);
    bzero(D_800BB560[2], gPossiblyScreenWidthOrHeight * D_80092E9C << 1);
    bzero(D_800BB560[3], gPossiblyScreenWidthOrHeight * D_80092E9C << 1);

    bzero(D_800BB578[0], gPossiblyScreenWidthOrHeight * D_80092E9C >> 1);
    bzero(D_800BB578[1], gPossiblyScreenWidthOrHeight * D_80092E9C >> 1);
    bzero(D_800BB578[2], gPossiblyScreenWidthOrHeight * D_80092E9C >> 1);
    bzero(D_800BB578[3], gPossiblyScreenWidthOrHeight * D_80092E9C >> 1);

    func_8005BBF0(0);
    func_8005BBF0(1);
}

void func_8005BBF0(u32 arg0) {
    D_80092E90[arg0] ^= 1;
    D_800BB550[arg0] = D_800BB540[2*arg0 + D_80092E90[arg0]];
}

/**
 * If param1 isn't null, sets it to gPossiblyScreenWidthOrHeight.
 *
 * Returns gSomeVideoFlag.
 */
int func_8005BC38(s32 *param1) {
    if (param1 != NULL) {
        *param1 = gPossiblyScreenWidthOrHeight;
    }

    return gSomeVideoFlag;
}

/*
 *   arg0 is an Object instance: Sabre/Krystal for example, because they create dynamic shadows
 *   arg1 is usually 0, but setting it to 1 affects the shadow rendering
 *   arg2 usually 0
 */
void func_8005BC58(Object* arg0, s32 arg1, s32 arg2) {
    if (arg1 != 0) {
        func_80052DC0(D_800BB550[arg2], D_800BB558[arg2], D_800BB570[arg2], arg2);
    } else {
        func_800529C0(D_800BB550[arg2], D_800BB558[arg2]);
    }
    arg0->ptr0x64->unk_0x41 ^= 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5C470/func_8005BCE0.s")

/*
 * May have something to do with the dynamic shadows,
 * since editing this function to immediately return will affect the appearance of Krystal/Sabre's appearance!
 * 
 * called once per frame
 * arg0 alternates between 0 and 1 on different frames
 * arg1 and arg2 are pointers
 * arg3 = 0
 */
void func_8005C740(s32 arg0, u32* arg1, u32* arg2, u32 arg3) {
    *arg1 = (u32)D_800BB560[arg0 + (arg3 * 2)];
    *arg2 = (u32)D_800BB578[arg0 + (arg3 * 2)];
}