#include "common.h"

#define REFRESH_50HZ 50
#define REFRESH_60HZ 60

// TODO: from DKR decomp, double check
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define HIGH_RES_SCREEN_WIDTH 640
#define HIGH_RES_SCREEN_HEIGHT 480

#define LOW_RES_WIDTH 320.0f
#define LOW_RES_PAL_HEIGHT 264.0f
#define LOW_RES_NTSC_HEIGHT 240.0f
#define LOW_RES_MPAL_HEIGHT LOW_RES_NTSC_HEIGHT

#define PAL_HEIGHT_DIFFERENCE 24

#define ASPECT_RATIO_PAL  (LOW_RES_WIDTH / LOW_RES_PAL_HEIGHT)
#define ASPECT_RATIO_NTSC (LOW_RES_WIDTH / LOW_RES_NTSC_HEIGHT)
#define ASPECT_RATIO_MPAL (LOW_RES_WIDTH / LOW_RES_MPAL_HEIGHT)

#define HEIGHT_RATIO_PAL  (LOW_RES_PAL_HEIGHT / LOW_RES_NTSC_HEIGHT)
#define HEIGHT_RATIO_NTSC (LOW_RES_NTSC_HEIGHT / LOW_RES_NTSC_HEIGHT)
#define HEIGHT_RATIO_MPAL (LOW_RES_MPAL_HEIGHT / LOW_RES_NTSC_HEIGHT)

static const char str_8009ace0[] = "320 by 240 Anti-aliased, Non interlaced.\n";
static const char str_8009ad0c[] = "640 by 480 Anti-aliased, Interlaced, De-flickered.\n";
static const char str_8009ad40[] = "vi sizes %d %d %d %d  w %d h %d\n";

void func_8005D9D8();
void set_video_mode(s32 mode);
void initialize_framebuffers(int someBool, s32 width, s32 height);
void swap_framebuffer_pointers();
void set_custom_vi_mode();
void func_8005DEE8();
int is_size_smaller_than_resolution(s32 width, s32 height);

extern OSIoMesg D_800BCC90;
extern OSDevMgr __osViDevMgr;
extern s8 D_80093064;

// viInit (see https://github.com/DavidSM64/Diddy-Kong-Racing/blob/aacc45612e4235da67137355bd8d6e1f345d1e04/src/video.c#L59)
void func_8005D410(s32 videoMode, OSSched* scheduler, s32 someBool) {
    int i;
    u32 width;
    u32 height;
    
    if (osTvType == OS_TV_PAL) {
        gDisplayHertz = REFRESH_50HZ;
        aspectRatioFloat = ASPECT_RATIO_PAL;
        D_800BCCB8 = HEIGHT_RATIO_PAL;
    } else if (osTvType == OS_TV_MPAL) {
        gDisplayHertz = REFRESH_60HZ;
        aspectRatioFloat = ASPECT_RATIO_MPAL;
        D_800BCCB8 = HEIGHT_RATIO_MPAL;
    } else {
        gDisplayHertz = REFRESH_60HZ;
        aspectRatioFloat = ASPECT_RATIO_NTSC;
        D_800BCCB8 = HEIGHT_RATIO_NTSC;
    }

    if (someBool && osTvType == OS_TV_PAL) {
        for (i = 0; i < VIDEO_RESOLUTIONS_COUNT; i++) {
            gResolutionArray[i].v += 20;
        }
    }

    func_8005D9D8();

    set_video_mode(videoMode);

    gFramebufferPointers[0] = NULL;
    gFramebufferPointers[1] = NULL;

    if (someBool) {
        osCreateMesgQueue(&OSMesgQueue_800bcce0, &D_800bccc0[0], 8);
        osScAddClient(scheduler, &D_800bce60, &OSMesgQueue_800bcce0, 2);
    }

    width = 320;
    height = 260;

    if (videoMode == OS_VI_PAL_LPN1) {
        width = 640;
        height = 480;
    }

    initialize_framebuffers(someBool, width, height);

    gFramebufferChoice = 1;

    swap_framebuffer_pointers();
    set_custom_vi_mode();

    D_800BCE14 = 0xc;

    osViBlack(TRUE);

    D_800BCE58 = 0;
    D_800BCE2C = 5;

    if (someBool) {
        D_800BCE18[0] = mmAlloc(960, ALLOC_TAG_SCREEN_COL, NULL);
        D_800BCE18[1] = &D_800BCE18[0][480];
    }

    bzero(D_800BCE18[0], 960);

    D_800BCE20 = 0;
    D_800BCE22[0] = 0;
    D_800BCE22[1] = 0;
    D_800BCE34 = 1;
}

/**
 * Sets gVideoMode.
 */
void set_video_mode(s32 mode) {
    gVideoMode = mode;
}

/**
 * Returns gVideoMode.
 */
s32 get_video_mode() {
    return gVideoMode;
}

OSMesgQueue *get_addr_of_OSMesgQueue_8005D670() {
    return &OSMesgQueue_800bcce0;
}

/**
 * Sets gCurrentResolution*[framebufferIndex] to the resolution
 * specified by gVideoMode from gResolutionArray.
 */
void set_current_resolution_from_video_mode(int framebufferIndex) {
    // Note: framebufferIndex was decided on because another function calls this
    //       with gFramebufferChoice (which is either 0 or 1 presumably) and
    //       gCurrentResolutionH and gCurrentResolutionV both conveniently contain
    //       2 integers, which are likely for each framebuffer.
    gCurrentResolutionH[framebufferIndex] = gResolutionArray[gVideoMode & 7].h;
    gCurrentResolutionV[framebufferIndex] = gResolutionArray[gVideoMode & 7].v;
}

/**
 * Returns a video resolution encoded as 0xVVVV_HHHH.
 *
 * If the result of func_8005BC38 is 0, then it will be the current framebuffer's resolution.
 */
u32 get_some_resolution_encoded(void) {
    s32 var1;
    int flag;

    flag = func_8005BC38(&var1);

    if (flag == FALSE) {
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

/**
 * Returns the resolution of the framebuffer not currently in use encoded as 0xVVVV_HHHH.
 *
 * Note: The resolution is found by gCurrentResolution*[gFramebufferChoice < 1]
 */
u32 get_other_resolution_encoded() {
    return (gCurrentResolutionV[gFramebufferChoice < 1] << 0x10) |
            gCurrentResolutionH[gFramebufferChoice < 1];
}

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
    bcopy(
        viMode,
        &gOSViModeCustom,
        sizeof(OSViMode)
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

void initialize_framebuffers(int someBool, s32 width, s32 height) {
    VideoResolution *resPtr;
    u32 hRes;
    u32 vRes;
    
    // Get resolution by current video mode
    resPtr = &gResolutionArray[gVideoMode & 0x7];
    hRes = resPtr->h;
    vRes = resPtr->v;

    // Set current resolution
    gCurrentResolutionH[0] = hRes;
    gCurrentResolutionH[1] = hRes;
    gCurrentResolutionV[0] = vRes;
    gCurrentResolutionV[1] = vRes;

    if (osMemSize != 0x800000) {
        // No expansion pack detected
        gFramebufferPointers[0] = (u16*)(FRAMEBUFFER_ADDRESS_NO_EXP_PAK);
        gFramebufferPointers[1] = (u16*)(FRAMEBUFFER_ADDRESS_NO_EXP_PAK + ((width * height) * 2));
        
        gFramebufferStart = (u16*)(FRAMEBUFFER_ADDRESS_NO_EXP_PAK);
        return;
    }
    
    if (height == 480) {
        // PAL framebuffer height
        gFramebufferPointers[0] = (u16*)(FRAMEBUFFER_ADDRESS_EXP_PAK);
        gFramebufferPointers[1] = (u16*)(FRAMEBUFFER_ADDRESS_EXP_PAK + ((width * height) * 2));
        
        gFramebufferStart = (u16*)(FRAMEBUFFER_ADDRESS_EXP_PAK);
    } else {
        // NTSC/M-PAL framebuffer height
        gFramebufferPointers[0] = (u16*)(FRAMEBUFFER_ADDRESS_EXP_PAK);
        gFramebufferPointers[1] = (u16*)(FRAMEBUFFER_ADDRESS_EXP_PAK + ((width * height) * 2));
        
        gFramebufferEnd = (u16*)(((int) (FRAMEBUFFER_ADDRESS_EXP_PAK + ((width * height) * 2))) + ((width * height) * 2));
        gFramebufferStart = (u16*)0x80200000;
    }
}

void func_8005D9D8() {
    D_800BCE58 = 0;
    D_800BCE59 = 2;
    D_800BCE34 = 1;
}

void func_8005DA00(u32 param1) {
    D_800BCE34 = param1;
}

s32 video_func_returning_delay(s32 param1) {
    s32 ret;
    s32 vidMode;

    ret = 1;

    if (D_800BCE14 != 0) {
        D_800BCE14 -= 1;

        if (D_800BCE14 == 0) {
            osViBlack(FALSE);
        }
    }

    if (param1 != 8) {
        swap_framebuffer_pointers();
    }

    while (osRecvMesg(&OSMesgQueue_800bcce0, NULL, OS_MESG_NOBLOCK) != -1) {
        ret += 1;
    }

    D_800BCE59 = ret;

    if (D_800BCE59 < D_800BCE34) {
        D_800BCE59 = D_800BCE34;
    }

    while (ret < D_800BCE59) {
        osRecvMesg(&OSMesgQueue_800bcce0, NULL, OS_MESG_BLOCK);
        ret++;
    }

    if (D_80093060 != 0) {
        vidMode = get_video_mode();

        if (D_80093060 == 3) {
            set_video_mode(vidMode);
            set_current_resolution_from_video_mode(gFramebufferChoice);
            osViSwapBuffer(gFramebufferCurrent);
        } else if (D_80093060 == 2) {
            set_current_resolution_from_video_mode(gFramebufferChoice);
            osViSwapBuffer(gFramebufferCurrent);
        } else {
            D_800BCC90.hdr.type = 0x11;
            D_800BCC90.hdr.retQueue = (OSMesgQueue*)&gOSViModeCustom;
            osSendMesg(__osViDevMgr.evtQueue, &D_800BCC90, OS_MESG_BLOCK);
            osViSwapBuffer(gFramebufferCurrent);
            D_80093064 ^= 1;
        }

        D_80093060 -= 1;
    } else {
        if (get_pause_state() == 1) {
            // Create pause screen screenshot
            set_pause_state(2);
            bcopy(gFramebufferNext, gFramebufferEnd, 0x25800);
        } else {
            osViSwapBuffer(gFramebufferCurrent);
        }
    }

    signal_apply_controller_inputs();
    func_8005DEE8();
    osRecvMesg(&OSMesgQueue_800bcce0, NULL, OS_MESG_BLOCK);

    return ret;
}

void func_8005DC68() {}

void func_8005DC70(int _) {}

s32 func_8005DC7C() {
    return (s32)((f32)gDisplayHertz / (f32)D_800BCE59);
}

/**
 * Swaps gFramebufferCurrent and gFramebufferNext.
 *
 * Uses gFramebufferChoice to keep track of the index for the next framebuffer to swap to.
 */
void swap_framebuffer_pointers() {
    // Set the current framebuffer to the one last chosen for framebufferNext (see below)
    gFramebufferCurrent = gFramebufferPointers[gFramebufferChoice];

    // TODO: what is this doing?
    D_800BCCB4 = gFramebufferStart; // D_800BCCB4 = &framebufferCurrent+8

    // Swap choice to the other framebuffer index
    //
    // Assuming there are 2 framebuffers, this will flip between 0 and 1:
    //   0 ^ 1 = 1
    //   1 ^ 1 = 0
    gFramebufferChoice = gFramebufferChoice ^ 1;

    // Set next framebuffer to the swapped index
    gFramebufferNext = gFramebufferPointers[gFramebufferChoice];

    // TODO: what is this doing?
    D_800BCCB0 = gFramebufferStart; // D_800BCCB0 = &framebufferCurrent+4
}

/**
 * Returns gFramebufferStart.
 */
u16 *get_framebuffer_start() {
    return gFramebufferStart;
}

/**
 * Returns gFramebufferEnd.
 */
u16 *get_framebuffer_end() {
    return gFramebufferEnd;
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/vi/func_8005DD4C.s")
#else
s32 func_8005DD4C(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp2C;
    s32 var_v1;
    Vec3s32* sp24;
    s32 temp_t6;
    u8* sp1C;

    temp_t6 = (D_800BCE20 ^ 1) & 0xFF;
    sp24 = D_800BCE18[temp_t6];
    for (sp2C = 0, var_v1 = 0; var_v1 < D_800BCE22[temp_t6]; var_v1++) {
        if (arg2 == sp24[var_v1].z) {
            sp2C = sp24[var_v1].x;
            break;
        }
    }

    sp1C = &D_800BCE22[D_800BCE20];
    sp24 = D_800BCE18[D_800BCE20];
    sp24[*sp1C].z = arg2;
    sp24[*sp1C].x = 0;
    if (is_size_smaller_than_resolution(arg0, arg1) == 0) {
        temp_t6 = -1;
    } else {
        temp_t6 = (gCurrentResolutionH[gFramebufferChoice] * arg1) + arg0;
    }

    sp24[*sp1C].y = temp_t6;
    *sp1C += 1;
    return sp2C;
}
#endif

/**
 * Returns whether the given width and height is smaller than the current framebuffer's resolution.
 *
 * Note: Both width AND height must be smaller for this to return true.
 */
int is_size_smaller_than_resolution(s32 width, s32 height) {
    return width >= 0
        && (u32)width < gCurrentResolutionH[gFramebufferChoice]
        && height >= 0
        && (u32)height < gCurrentResolutionV[gFramebufferChoice];
}

void func_8005DEE8(void) {
    s32 i;
    u8 new_var;
    s32 new_var2;
    u32 temp_t5;
    u8 *temp;
    UnkVidStruct3* temp_a3;
    Vec3s32 *temp2;

    new_var = D_800BCE20;
    temp2 = D_800BCE18[new_var];
    i = 0;
    temp = &D_800BCE22[D_800BCE20];
    for (; *temp > i; i++) {
        new_var2 = temp2[i].y;
        temp_t5 = new_var2;
        if (new_var2 >= 0) {
            temp_t5 = gFramebufferStart[new_var2] >> 2;
            // temp_t5 >> 0xB loads the first 21 bits
            temp_a3 = &D_80093068[((temp_t5 >> 0xB) & 7)];
            // temp_t5 & 0x7FF loads the last 11 bits
            temp2[i].x = (temp_a3->unk4 + ((temp_t5 & 0x7FF) << temp_a3->unk0)) >> 3;
        } else {
            temp2[i].x = 0;
        }
    }

    D_800BCE20 ^= 1;
    D_800BCE22[D_800BCE20] = 0;
}

/**
 * Note: param1 is most likely a boolean
 */
void some_video_setup(s32 param1) {
    if (param1) {
        set_video_mode(7);
        initialize_framebuffers(1, gResolutionArray[7].h, gResolutionArray[7].v);

        set_custom_vi_mode();
        D_800BCE14 = 0xc;
        osViBlack(TRUE);
        D_800BCE58 = 0;
        D_800BCE2C = 0x5;
    } else {
        set_video_mode(1);
        initialize_framebuffers(1, gResolutionArray[0].h, gResolutionArray[0].v);

        set_custom_vi_mode();
        D_800BCE14 = 0xc;
        osViBlack(TRUE);
        D_800BCE58 = 0;
        D_800BCE2C = 0x5;
    }
}

void modify_vi_mode(u8 a0, s8 hStartMod, s8 vScaleMod) {
    u8 viLpn;
    OSViMode *viMode;

    gHStartMod = hStartMod;
    gVScaleMod = vScaleMod;

    // If a0 is 0, don't set custom vi mode
    if (a0 == 0) {
        return;
    }

    // Determine VI LPN from TV type
    if (osTvType == OS_TV_PAL) {
        viLpn = OS_VI_PAL_LPN1;
    } else if (osTvType == OS_TV_MPAL) {
        viLpn = OS_VI_MPAL_LPN1;
    } else {
        viLpn = OS_VI_NTSC_LPN1;
    }

    // Determine VI mode from video mode and VI LPN
    if (gVideoMode == OS_VI_PAL_LPN1 /*0xe*/) {
        viMode = &osViModeTable[11 + viLpn];
    } else {
        viMode = &osViModeTable[2 + viLpn];
    }

    // Set gOSViModeCustom to the chosen VI mode
    bcopy(
        viMode,
        &gOSViModeCustom,
        sizeof(OSViMode)
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

    D_80093060 = 3;
}
