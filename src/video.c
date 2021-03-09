#include "common.h"

bool func_8005BC38(u32 *param1);

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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005CD80.s")
#else
void _func_8005CD80()
{
    UnknownVideoStruct *vStruct;
    u8 *vStructArray;
    u8 *vStructArray0x10;
    u8 *vStructArray0x20;
    short zero;
    zero = 0;
    vStruct = &gUnknownVideoStructs[zero];
    vStructArray = &vStruct->unk0x18[zero];
    
    do
    {
        vStructArray0x10 = vStructArray + 0x10;
        vStructArray0x20 = vStructArray0x10 + 0x10;

        if (vStruct->viMode != NULL)
        {
            vStruct->unk0x88 -= 5;
            
            if (vStruct->unk0x88 < zero)
            {
                vStruct->unk0x88 = zero;
            }

            vStructArray[0xf] = vStruct->unk0x88;
            vStructArray0x10[0xf] = vStruct->unk0x88;
            vStructArray0x10[0x1f] = vStruct->unk0x88;
            vStructArray0x20[0xf] = vStruct->unk0x88;
        }

        vStructArray += 0x90;
        vStruct++;
    }
    while ((vStructArray != ((&gMaybeCurrentUnknownVideoStruct.unk0x18[zero])) != 0));
}
#endif

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
OSMesgQueue* get_addr_of_OSMesgQueue_8005D670() {
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

    gOSViModeCustom.fldRegs[0].vStart += vscale_mod * 0x20000;
    gOSViModeCustom.fldRegs[1].vStart += vscale_mod * 0x20000;
    gOSViModeCustom.fldRegs[0].vStart += vscale_mod * 0x2;
    gOSViModeCustom.fldRegs[1].vStart += vscale_mod * 0x2;
    gOSViModeCustom.comRegs.hStart += hstartMod * 0x20000;
    gOSViModeCustom.comRegs.hStart += hstartMod * 0x2;

    // Use the custom VI mode and set some special features
    osViSetMode(&gOSViModeCustom);
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

// initialize_framebuffers?
void set_framebuffer_pointers(u32 param1, u32 param2, u32 param3) {
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
u32* get_framebuffer_start() {
    return gFramebufferStart;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/video/get_framebuffer_end.s")
#else
u32* get_framebuffer_end() {
    return gFramebufferEnd;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DD4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DE94.s")

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
