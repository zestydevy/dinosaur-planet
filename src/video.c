#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005C780.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005C998.s")
void func_8005C998();

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

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005CA88.s")

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
    video_mode = mode;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/get_video_mode.s")
#else
u32 get_video_mode() {
    return video_mode;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/get_addr_of_OSMesgQueue_8005D670.s")
#else
OSMesgQueue* get_addr_of_OSMesgQueue_8005D670() {
    return &OSMesgQueue_800bcce0;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/set_current_resolution_from_video_mode.s")
#else
// Note: framebufferIndex was decided on because another function calls this
//       with framebufferChoice (which is either 0 or 1 presumably) and
//       CurrentResolutionH and CurrentResolutionV both conveniently contain
//       2 integers, which are likely for each framebuffer.
void set_current_resolution_from_video_mode(int framebufferIndex) {
    CurrentResolutionH[framebufferIndex] = resolutionArray[video_mode & 7].h;
    CurrentResolutionV[framebufferIndex] = resolutionArray[video_mode & 7].v;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005D6C8.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/get_current_resolution_encoded.s")
#else
/**
 * Returns the current framebuffer's resolution encoded as 0xVVVV_HHHH.
 */
u32 get_current_resolution_encoded() {
    return (CurrentResolutionV[framebufferChoice < 1] << 0x10) | 
            CurrentResolutionH[framebufferChoice < 1];
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/set_custom_vi_mode.s")
#else
void set_custom_vi_mode() {
    u8 viLpn;
    OSViMode* viMode;

    // Determine VI LPN from TV type
    viLpn = OS_VI_NTSC_LPN1;
    
    if (osTvType == OS_TV_PAL) {
        viLpn = OS_VI_PAL_LPN1;
    } else if (osTvType == OS_TV_MPAL) {
        viLpn = OS_VI_MPAL_LPN1;
    }

    // Determine VI mode from video mode and VI LPN
    switch (video_mode & 0x7) {
        case 0x1:
        default:
            viMode = &osViModeTable[2 + viLpn];
            break;
        case 0x6:
            viMode = &osViModeTable[11 + viLpn];
            break;
    }

    // Set OSViMode_custom to the chosen VI mode
    _bcopy(
        viMode,
        &OSViMode_custom,
        0x50 // OSViMode size = 0x50 (80 bytes)
    );

    // Make PAL-specific vStart adjustments
    if (osTvType == OS_TV_PAL) {
        OSViMode_custom.fldRegs[0].vStart -= 0x180000;
        OSViMode_custom.fldRegs[1].vStart -= 0x180000;
        OSViMode_custom.fldRegs[0].vStart += 0x10;
        OSViMode_custom.fldRegs[1].vStart += 0x10;
    }

    OSViMode_custom.fldRegs[0].vStart += vscale_mod * 0x20000;
    OSViMode_custom.fldRegs[1].vStart += vscale_mod * 0x20000;
    OSViMode_custom.fldRegs[0].vStart += vscale_mod * 0x2;
    OSViMode_custom.fldRegs[1].vStart += vscale_mod * 0x2;
    OSViMode_custom.comRegs.hStart += hstartMod * 0x20000;
    OSViMode_custom.comRegs.hStart += hstartMod * 0x2;

    // Use the custom VI mode and set some special features
    osViSetMode(&OSViMode_custom);
    osViSetSpecialFeatures(OS_VI_DIVOT_ON);
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/video/set_framebuffer_pointers.s")
#else
#define framebufferAddress_NoExpPak_addr 0x802d4000
#define framebufferAddress_ExpPak_addr 0x80119000

void set_framebuffer_pointers(u32 param1, u32 param2, u32 param3) {
    u32 hRes;
    u32 vRes;
    u32 *resPtr;
    u32 temp;
    u32 temp2;

    vRes = (video_mode & 0x7) * 2;
    temp2 = vRes;
    resPtr = (&resolutionArray) + temp2;

    hRes = resPtr[0];
    resPtr = resPtr;
    vRes = resPtr[1];

    CurrentResolutionH[0] = hRes;
    CurrentResolutionH[1] = hRes;
    CurrentResolutionV[0] = vRes;
    CurrentResolutionV[1] = vRes;
    
    if (osMemSize != 0x800000)
    {
        framebufferPointers[0] = 0x802d4000;
        framebufferPointers[1] = 0x802d4000 + ((param2 * param3) * 2);
        framebufferStart = 0x802d4000;
        return;
    }

    if (param3 == 0x1e0)
    {
        framebufferPointers[0] = 0x80119000;
        framebufferPointers[1] = 0x80119000 + ((param2 * param3) * 2);
        framebufferStart = 0x80119000;
        return;
    }

    temp = (param2 * param3) * 2;
    framebufferPointers[0] = 0x80119000;
    framebufferPointers[1] = 0x80119000 + temp;
    frameBufferEnd = ((int) (temp + 0x80119000)) + temp;
    framebufferStart = 0x80200000;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005D9D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DA00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/video_func_returning_delay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DC68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DC70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DC7C.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/swap_framebuffer_pointers.s")
#else
/**
 * Swaps framebufferCurrent and framebufferNext.
 * 
 * Uses framebufferChoice to keep track of the index for the next framebuffer to swap to.
 */
void swap_framebuffer_pointers() {
    // Set the current framebuffer to the one last chosen for framebufferNext (see below)
    framebufferCurrent = framebufferPointers[framebufferChoice];
    
    // TODO: what is this doing?
    D_800bccb4 = framebufferStart; // D_800bccb4 = &framebufferCurrent+8
    
    // Swap choice to the other framebuffer index
    //
    // Assuming there are 2 framebuffers, this will flip between 0 and 1:
    //   0 ^ 1 = 1
    //   1 ^ 1 = 0
    framebufferChoice = framebufferChoice ^ 1;

    // Set next framebuffer to the swapped index
    framebufferNext = framebufferPointers[framebufferChoice];

    // TODO: what is this doing?
    D_800bccb0 = framebufferStart; // D_800bccb0 = &framebufferCurrent+4
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DD2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DD3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DD4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DE94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DEE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/some_video_setup.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/modify_viMode.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/osCreateViManager.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/viMgrMain.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005E550.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005E770.s")
