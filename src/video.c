#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005C780.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005C998.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005CA5C.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005D670.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005D680.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005D6C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005D724.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/video/set_framebuffer_pointers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005D9D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DA00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/video_func_returning_delay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DC68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DC70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DC7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/video/swap_framebuffer_pointers.s")

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
