#include "common.h"
#include "video.h"

bool func_8005BC38(u32 *param1);
void _bcopy(void*, void*, u32);

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005C780.s")
#else
void _func_8005C780() {
    UnknownVideoStruct2 *vidStruct2;
    UnknownVideoStruct *vidStruct;
    UnknownVideoStruct *vidStructsEnd;
    UnknownHeapVideoStruct *vidHeapStruct;

    vidHeapStruct = D_800bcc10;

    if (vidHeapStruct != NULL) {
        free(vidHeapStruct);
    }

    D_800bcc10 = malloc(UNKNOWN_HEAP_VIDEO_STRUCT_SIZE, 0x13, 0);

    if (1) {
        vidStructsEnd = &gUnknownVideoStructs[UNKNOWN_VIDEO_STRUCTS_COUNT];
        vidStruct = &gUnknownVideoStructs[0];

        do {
            vidStruct->ptrToUnknown1 = (UnknownHeapVideoStruct*)0xdf000000;
            vidStruct->ptrToUnknown2 = NULL; 
            goto label_1; label_1: // TODO: This probably shouldn't be necessary
            vidStruct->viMode = NULL;
            vidStruct->unk0x88 = 0;
        } while (++vidStruct < vidStructsEnd);

        // TODO: Figure out how to get this to match in a way that it weaves in `lui   at,0x800c`
        //       The if (1) stmts get pretty close, but there's probably something missing here
        //if (1) {
            vidStruct2 = &D_800bcc18[0];
            vidStruct2->unk0x10 = 0;
        //}
        // lui   at,0x800c
        //if (1) {
            vidStruct2->unk0x0 = 0.0f;
            vidStruct2->unk0x4 = 0.0f;
        //}
        // lui   at,0x800c
        //if (1) {
            vidStruct2->unk0xc = (u8)0;
            vidStruct2->unk0x8 = 0.0f;
        //}
        // lui   at,0x800c
        //if (1) {
            vidStruct2 = &D_800bcc18[1];
            vidStruct2->unk0x10 = 0;
        //}
        // lui   at,0x800c
        //if (1) {
            vidStruct2->unk0x0 = 0.0f;
        //}
        // lui   at,0x800c
        //if (1) {
            vidStruct2->unk0x4 = 0.0f;
            vidStruct2->unk0x8 = 0.0f;
        //}
        // lui   at,0x800c
        //if (1) {
            vidStruct2->unk0xc = (u8)0;
        //}
        // lui   at,0x800c
        //if (1) {
            vidStruct2 = &D_800bcc18[2];
            vidStruct2->unk0x10 = 0;
        //}
        // lui   at,0x800c
        //if (1) {
            vidStruct2->unk0x0 = 0.0f;
            vidStruct2->unk0x4 = 0.0f;
        //}
        // lui   at,0x800c
        //if (1) {
            vidStruct2->unk0xc = (u8)0;
            vidStruct2->unk0x8 = 0.0f;
        //}
        // lui   at,0x800c
        //if (1) {
            vidStruct2 = &D_800bcc18[3];
            vidStruct2->unk0x10 = 0;
        //}
        // lui   at,0x800c
        //if (1) {
            vidStruct2->unk0x0 = 0.0f;
        //}
        // lui   at,0x800c
        //if (1) {
            vidStruct2->unk0x4 = 0.0f;
            vidStruct2->unk0x8 = 0.0f;
        //}
        // lui   at,0x800c
        //if (1) {
            vidStruct2->unk0xc = (u8)0;
        //}
    }

    D_800bcc10->unk0x0 = -3.0f;
    D_800bcc10->unk0x4 = 0.0f;
    D_800bcc10->unk0x8 = -3.0f;
    D_800bcc10->unk0xc = -3.0f;
    D_800bcc10->unk0x10 = 0.0f;
    D_800bcc10->unk0x14 = 3.0f;
    D_800bcc10->unk0x18 = 3.0f;
    D_800bcc10->unk0x1c = 0.0f;
    D_800bcc10->unk0x20 = 3.0f;
    D_800bcc10->unk0x24 = 3.0f;
    D_800bcc10->unk0x28 = 0.0f;
    D_800bcc10->unk0x2c = -3.0f;
    D_800bcc10->unk0x30 = -3.0f;
    D_800bcc10->unk0x34 = 0.0f;
    D_800bcc10->unk0x38 = -3.0f;
    D_800bcc10->unk0x3c = -3.0f;
    D_800bcc10->unk0x40 = 0.0f;
    D_800bcc10->unk0x44 = 3.0f;
    D_800bcc10->unk0x48 = 3.0f;
    D_800bcc10->unk0x4c = 0.0f;
    D_800bcc10->unk0x50 = 3.0f;
    D_800bcc10->unk0x54 = 3.0f;
    D_800bcc10->unk0x58 = 0.0f;
    D_800bcc10->unk0x5c = -3.0f;

    D_800bcc68 = func_8003cd7c(0x18);
    D_800bcc6c = func_8003cd7c(0x19);
    D_800bcc70 = func_8003cd7c(0x1a);

    D_800bcc78 = (u8)0;
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005C998.s")
void func_8005C998();
#else
void _func_8005C998() {
    UnknownVideoStruct2 *vidStruct2;
    UnknownVideoStruct *end = &gUnknownVideoStructs[UNKNOWN_VIDEO_STRUCTS_COUNT];
    UnknownVideoStruct *vidStruct = &gUnknownVideoStructs[0];

    do {
        vidStruct->ptrToUnknown1 = (UnknownHeapVideoStruct*)0xdf000000;
        vidStruct->ptrToUnknown2 = NULL; 
        goto label_1; label_1: // TODO: This probably shouldn't be necessary
        vidStruct->viMode = NULL;
        vidStruct->unk0x88 = 0;
    } while (++vidStruct < end);

    // TODO: Figure out how to get this to match in a way that it weaves in `lui   at,0x800c`
    //       The if (1) stmts get pretty close, but there's probably something missing here
    //if (1) {
        vidStruct2 = &D_800bcc18[0];
        vidStruct2->unk0x10 = 0;
    //}
    // lui   at,0x800c
    //if (1) {
        vidStruct2->unk0x0 = 0.0f;
        vidStruct2->unk0x4 = 0.0f;
    //}
    // lui   at,0x800c
    //if (1) {
        vidStruct2->unk0xc = (u8)0;
        vidStruct2->unk0x8 = 0.0f;
    //}
    // lui   at,0x800c
    //if (1) {
        vidStruct2 = &D_800bcc18[1];
        vidStruct2->unk0x10 = 0;
    //}
    // lui   at,0x800c
    //if (1) {
        vidStruct2->unk0x0 = 0.0f;
    //}
    // lui   at,0x800c
    //if (1) {
        vidStruct2->unk0x4 = 0.0f;
        vidStruct2->unk0x8 = 0.0f;
    //}
    // lui   at,0x800c
    //if (1) {
        vidStruct2->unk0xc = (u8)0;
    //}
    // lui   at,0x800c
    //if (1) {
        vidStruct2 = &D_800bcc18[2];
        vidStruct2->unk0x10 = 0;
    //}
    // lui   at,0x800c
    //if (1) {
        vidStruct2->unk0x0 = 0.0f;
        vidStruct2->unk0x4 = 0.0f;
    //}
    // lui   at,0x800c
    //if (1) {
        vidStruct2->unk0xc = (u8)0;
        vidStruct2->unk0x8 = 0.0f;
    //}
    // lui   at,0x800c
    //if (1) {
        vidStruct2 = &D_800bcc18[3];
        vidStruct2->unk0x10 = 0;
    //}
    // lui   at,0x800c
    //if (1) {
        vidStruct2->unk0x0 = 0.0f;
    //}
    // lui   at,0x800c
    //if (1) {
        vidStruct2->unk0x4 = 0.0f;
        vidStruct2->unk0x8 = 0.0f;
    //}
    // lui   at,0x800c
    //if (1) {
        vidStruct2->unk0xc = (u8)0;
    //}
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005CA5C.s")
#else
void func_8005CA5C(u32 param1) {
    D_800bcc78 = param1;

    if (param1 == 0) {
        func_8005C998();
    }
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005CA88.s")
#else
// TODO: This one is really close
void func_8005CA88(f32 *a0, f32 *a1, u8 a2)
{
    s32 temp1;
    s32 temp2;
    UnknownVideoStruct2 *v1;

    if (D_800bcc78 != 0)
    {
        temp1 = D_80092FF8;

        D_800bcc18[temp1].unk0x10 = a0;
        D_800bcc18[temp1].unk0x0 = a1[0];

        temp2 = temp1 + 1;
        if (1);

        D_800bcc18[temp1].unk0x4 = a0[4] + a1[1];
        D_800bcc18[temp1].unk0x8 = a1[2];
        D_800bcc18[temp1].unk0xc = a2;

        D_80092FF8 = temp2;

        if (D_80092FF8 == 4)
        {
            D_80092FF8 = 0;
        }
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005CB10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005CC74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005CD80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005CDFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005CF4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005D3A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005D410.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/set_video_mode.s")
#else
void set_video_mode(u32 mode) {
    gVideoMode = mode;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/get_video_mode.s")
#else
u32 get_video_mode() {
    return gVideoMode;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/get_addr_of_OSMesgQueue_8005D670.s")
#else
OSMesgQueue *get_addr_of_OSMesgQueue_8005D670() {
    return &OSMesgQueue_800bcce0;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/set_current_resolution_from_video_mode.s")
#else
// Note: framebufferIndex was decided on because another function calls this
//       with gFramebufferChoice (which is either 0 or 1 presumably) and
//       gCurrentResolutionH and gCurrentResolutionV both conveniently contain
//       2 integers, which are likely for each framebuffer.
void set_current_resolution_from_video_mode(int framebufferIndex) {
    gCurrentResolutionH[framebufferIndex] = gResolutionArray[gVideoMode & 7].h;
    gCurrentResolutionV[framebufferIndex] = gResolutionArray[gVideoMode & 7].v;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005D6C8.s")
#else
/**
 * Returns a video resolution encoded as 0xVVVV_HHHH.
 * 
 * If the result of func_8005BC38 is 0, then it will be the current framebuffer's resolution.
 */
bool func_8005D6C8() {
    u32 var1;
    bool var2;

    var2 = func_8005BC38(&var1);

    if (var2 == FALSE) {
        return (gCurrentResolutionV[gFramebufferChoice] << 0x10) | 
                gCurrentResolutionH[gFramebufferChoice];
    } else {
        // Turns 0x0000_XXXX into 0xXXXX_XXXX
        //
        // Maybe this is to default to a square resolution?
        // Say, var1 is 480, this encodes the res as 480x480
        return (var1 << 0x10) | var1;
    }
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/get_current_resolution_encoded.s")
#else
// TODO: Rename this! This is not getting the current res, it's getting the res of the OTHER framebuffer!
/**
 * Returns the current framebuffer's resolution encoded as 0xVVVV_HHHH.
 */
u32 get_current_resolution_encoded() {
    return (gCurrentResolutionV[gFramebufferChoice < 1] << 0x10) | 
            gCurrentResolutionH[gFramebufferChoice < 1];
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/set_custom_vi_mode.s")
#else
void set_custom_vi_mode() {
    u8 viLpn;
    OSViMode *viMode;

    // Determine VI LPN from TV type
    viLpn = OS_VI_NTSC_LPN1;
    
    if (osTvType == OS_TV_PAL) {
        viLpn = OS_VI_PAL_LPN1;
    } else if (osTvType == OS_TV_MPAL) {
        viLpn = OS_VI_MPAL_LPN1;
    }

    // Determine VI mode from video mode and VI LPN
    switch (gVideoMode & 0x7) {
        case 0x1:
        default:
            viMode = &osViModeTable[2 + viLpn];
            break;
        case 0x6:
            viMode = &osViModeTable[11 + viLpn];
            break;
    }

    // Set gOSViModeCustom to the chosen VI mode
    _bcopy(
        viMode,
        &gOSViModeCustom,
        0x50 // OSViMode size = 0x50 (80 bytes)
    );

    // Make PAL-specific vStart adjustments
    if (osTvType == OS_TV_PAL) {
        gOSViModeCustom.fldRegs[0].vStart -= 0x180000;
        gOSViModeCustom.fldRegs[1].vStart -= 0x180000;
        gOSViModeCustom.fldRegs[0].vStart += 0x10;
        gOSViModeCustom.fldRegs[1].vStart += 0x10;
    }

    gOSViModeCustom.fldRegs[0].vStart += gVScaleMod * 0x20000;
    gOSViModeCustom.fldRegs[1].vStart += gVScaleMod * 0x20000;
    gOSViModeCustom.fldRegs[0].vStart += gVScaleMod * 0x2;
    gOSViModeCustom.fldRegs[1].vStart += gVScaleMod * 0x2;
    gOSViModeCustom.comRegs.hStart += gHStartMod * 0x20000;
    gOSViModeCustom.comRegs.hStart += gHStartMod * 0x2;

    // Use the custom VI mode and set some special features
    osViSetMode(&gOSViModeCustom);
    osViSetSpecialFeatures(OS_VI_DIVOT_ON);
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/video/set_framebuffer_pointers.s")
void set_framebuffer_pointers(u32, u32, u32);
#else
#define framebufferAddress_NoExpPak_addr 0x802d4000
#define framebufferAddress_ExpPak_addr 0x80119000

// initialize_framebuffers?
void _set_framebuffer_pointers(u32 param1, u32 param2, u32 param3) {
    u32 hRes;
    u32 vRes;
    u32 *resPtr;
    u32 temp;
    u32 temp2;

    vRes = (gVideoMode & 0x7) * 2;
    temp2 = vRes;
    resPtr = (&gResolutionArray) + temp2;

    hRes = resPtr[0];
    resPtr = resPtr;
    vRes = resPtr[1];

    gCurrentResolutionH[0] = hRes;
    gCurrentResolutionH[1] = hRes;
    gCurrentResolutionV[0] = vRes;
    gCurrentResolutionV[1] = vRes;
    
    if (osMemSize != 0x800000)
    {
        gFramebufferPointers[0] = 0x802d4000;
        gFramebufferPointers[1] = 0x802d4000 + ((param2 * param3) * 2);
        gFramebufferStart = 0x802d4000;
        return;
    }

    if (param3 == 0x1e0)
    {
        gFramebufferPointers[0] = 0x80119000;
        gFramebufferPointers[1] = 0x80119000 + ((param2 * param3) * 2);
        gFramebufferStart = 0x80119000;
        return;
    }

    temp = (param2 * param3) * 2;
    gFramebufferPointers[0] = 0x80119000;
    gFramebufferPointers[1] = 0x80119000 + temp;
    gFramebufferEnd = ((int) (temp + 0x80119000)) + temp;
    gFramebufferStart = 0x80200000;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005D9D8.s")
#else
void func_8005D9D8() {
    D_800bce58 = 0;
    D_800bce59 = 2;
    D_800bce34 = 1;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DA00.s")
#else
void func_8005DA00(u32 param1) {
    D_800bce34 = param1;   
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/video/video_func_returning_delay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DC68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DC70.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DC7C.s")
#else
// Returns an s32 / u8 (floating-point division) casted to an s32
s32 func_8005DC7C() {
    return (s32)((f32)D_800bce28 / (f32)D_800bce59);
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/swap_framebuffer_pointers.s")
#else
/**
 * Swaps gFramebufferCurrent and gFramebufferNext.
 * 
 * Uses gFramebufferChoice to keep track of the index for the next framebuffer to swap to.
 */
void swap_framebuffer_pointers() {
    // Set the current framebuffer to the one last chosen for framebufferNext (see below)
    gFramebufferCurrent = gFramebufferPointers[gFramebufferChoice];
    
    // TODO: what is this doing?
    D_800bccb4 = gFramebufferStart; // D_800bccb4 = &framebufferCurrent+8
    
    // Swap choice to the other framebuffer index
    //
    // Assuming there are 2 framebuffers, this will flip between 0 and 1:
    //   0 ^ 1 = 1
    //   1 ^ 1 = 0
    gFramebufferChoice = gFramebufferChoice ^ 1;

    // Set next framebuffer to the swapped index
    gFramebufferNext = gFramebufferPointers[gFramebufferChoice];

    // TODO: what is this doing?
    D_800bccb0 = gFramebufferStart; // D_800bccb0 = &framebufferCurrent+4
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/get_framebuffer_start.s")
#else
u32 *get_framebuffer_start() {
    return gFramebufferStart;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/get_framebuffer_end.s")
#else
u32 *get_framebuffer_end() {
    return gFramebufferEnd;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DD4C.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/vid_is_size_smaller_than_resolution.s")
#else
/**
 * Returns whether the given width and height is smaller than the current framebuffer's resolution.
 * 
 * Note: Both width AND height must be smaller for this to return true.
 */
bool vid_is_size_smaller_than_resolution(s32 width, s32 height) {
    return width >= 0 
        && (u32)width < gCurrentResolutionH[gFramebufferChoice]
        && height >= 0 
        && (u32)height < gCurrentResolutionV[gFramebufferChoice];
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DEE8.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/some_video_setup.s")
#else
// TODO: param1 seems likely to be a boolean
void some_video_setup(u32 param1) {
    if (param1 != 0) {
        set_video_mode(7);
        set_framebuffer_pointers(1, gResolutionArray[7].h, gResolutionArray[7].v);

        set_custom_vi_mode();
        D_800bce14 = 0xc;
        osViBlack(1);
        D_800bce58 = 0;
        D_800bce2c = 0x5;
    } else {
        set_video_mode(1);
        set_framebuffer_pointers(1, gResolutionArray[0].h, gResolutionArray[0].v);

        set_custom_vi_mode();
        D_800bce14 = 0xc;
        osViBlack(1);
        D_800bce58 = 0;
        D_800bce2c = 0x5;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/video/modify_viMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/osCreateViManager.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/viMgrMain.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005E550.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005E770.s")
