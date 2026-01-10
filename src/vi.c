#include "PR/os.h"
#include "PR/sched.h"
#include "PR/ultratypes.h"
#include "sys/gfx/gx.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "functions.h"
#include "macros.h"

#define REFRESH_50HZ 50
#define REFRESH_60HZ 60

// TODO: from DKR decomp, double check
#define SCREEN_WIDTH 320
// DKR defines this as 240
#define SCREEN_HEIGHT 260
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

void vi_reset_update_rate(void);
void vi_set_mode(s32 mode);
void vi_init_framebuffers(int someBool, s32 width, s32 height);
void vi_swap_buffers(void);
void vi_update_mode(void);
void vi_func_8005DEE8(void);
int vi_contains_point(s32 x, s32 y);

extern OSDevMgr __osViDevMgr;

/* -------- .data start 80093010 -------- */
u16 *gFramebufferStart = NULL;
s32 D_80093014 = 0; // unused
s32 D_80093018 = 0x02000000; // unused
s32 D_8009301C = 0; // unused
VideoResolution gResolutionArray[VIDEO_RESOLUTIONS_COUNT] = {
    {320, 240},
    {320, 240},
    {512, 240},
    {512, 240},
    {640, 480},
    {640, 480},
    {640, 480},
    {384, 192}
};
s8 D_80093060 = 0;
s8 D_80093064 = 0;
UnkVidStruct3 D_80093068[8] = {
    {6, 0x00000},
    {5, 0x20000},
    {4, 0x30000},
    {3, 0x38000},
    {2, 0x3C000},
    {1, 0x3E000},
    {0, 0x3F000},
    {0, 0x3F800}
};
/* -------- .data end 800930B0 -------- */

/* -------- .bss start 800bcc90 -------- */
OSIoMesg D_800BCC90;
u16 *gFramebufferNext;
u16 *gFramebufferCurrent;
u16 *D_800BCCB0;
u16 *D_800BCCB4;
f32 gViHeightRatio;
OSMesg D_800bccc0[8];
OSMesgQueue gVideoMesgQueue;
u8 _bss_800bccf8[0xA8];
OSViMode gTvViMode;
u32 gCurrentResolutionH[2];
u32 gCurrentResolutionV[2];
u16 *gFramebufferPointers[2];
u16 *gFramebufferEnd;
u32 gFramebufferChoice;
s32 gVideoMode;
u32 gViBlackTimer;
u8 *D_800BCE18[2];
u8 D_800BCE20; // index of D_800BCE22?
u8 D_800BCE22[2];
s8 gHStartMod;
s8 gVScaleMod;
s32 gDisplayHertz;
u8 D_800BCE2C;
f32 gAspectRatio; //1.121212 for PAL, 1.333 for NTSC/MPAL.
u8 gViUpdateRateTarget;
u8 _bss_800bce38[0x20];
u8 D_800BCE58;
u8 gViUpdateRate;
OSScClient gVideoSched;
static u8 _bss_pad[0x10];
/* -------- .bss end 800bce70 -------- */

void vi_init(s32 videoMode, OSSched* scheduler, s32 someBool) {
    int i;
    u32 width;
    u32 height;
    
    if (osTvType == OS_TV_PAL) {
        gDisplayHertz = REFRESH_50HZ;
        gAspectRatio = ASPECT_RATIO_PAL;
        gViHeightRatio = HEIGHT_RATIO_PAL;
    } else if (osTvType == OS_TV_MPAL) {
        gDisplayHertz = REFRESH_60HZ;
        gAspectRatio = ASPECT_RATIO_MPAL;
        gViHeightRatio = HEIGHT_RATIO_MPAL;
    } else {
        gDisplayHertz = REFRESH_60HZ;
        gAspectRatio = ASPECT_RATIO_NTSC;
        gViHeightRatio = HEIGHT_RATIO_NTSC;
    }

    if (someBool && osTvType == OS_TV_PAL) {
        for (i = 0; i < VIDEO_RESOLUTIONS_COUNT; i++) {
            gResolutionArray[i].v += 20;
        }
    }

    vi_reset_update_rate();

    vi_set_mode(videoMode);

    gFramebufferPointers[0] = NULL;
    gFramebufferPointers[1] = NULL;

    if (someBool) {
        osCreateMesgQueue(&gVideoMesgQueue, D_800bccc0, ARRAYCOUNT(D_800bccc0));
        osScAddClient(scheduler, &gVideoSched, &gVideoMesgQueue, OS_SC_ID_VIDEO);
    }

    width = SCREEN_WIDTH;
    height = SCREEN_HEIGHT;

    if (videoMode == OS_VI_PAL_LPN1) {
        width = HIGH_RES_SCREEN_WIDTH;
        height = HIGH_RES_SCREEN_HEIGHT;
    }

    vi_init_framebuffers(someBool, width, height);

    gFramebufferChoice = 1;

    vi_swap_buffers();
    vi_update_mode();

    gViBlackTimer = 0xc;

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
    gViUpdateRateTarget = 1;
}

/**
 * Sets gVideoMode.
 */
void vi_set_mode(s32 mode) {
    gVideoMode = mode;
}

/**
 * Returns gVideoMode.
 */
s32 vi_get_mode() {
    return gVideoMode;
}

OSMesgQueue *vi_get_mesg_queue() {
    return &gVideoMesgQueue;
}

/**
 * Sets gCurrentResolution*[framebufferIndex] to the resolution
 * specified by gVideoMode from gResolutionArray.
 */
void vi_update_fb_size_from_current_mode(int framebufferIndex) {
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
u32 vi_get_current_size(void) {
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
u32 vi_get_backbuffer_size(void) {
    return (gCurrentResolutionV[gFramebufferChoice < 1] << 0x10) |
            gCurrentResolutionH[gFramebufferChoice < 1];
}

void vi_update_mode(void) {
    u8 viModeTableIndex;
    OSViMode *viMode;

    // Determine VI mode from video mode and TV type
    viModeTableIndex = OS_VI_NTSC_LPN1;
    if (osTvType == OS_TV_PAL) {
        viModeTableIndex = OS_VI_PAL_LPN1;
    } else if (osTvType == OS_TV_MPAL) {
        viModeTableIndex = OS_VI_MPAL_LPN1;
    }

    switch (gVideoMode & 0x7) {
        case 0x1:
        default:
            viMode = &osViModeTable[2 + viModeTableIndex];
            break;
        case 0x6:
            viMode = &osViModeTable[11 + viModeTableIndex];
            break;
    }

    // Copy VI mode so we can make adjustments
    bcopy(viMode, &gTvViMode, sizeof(OSViMode));

    // Make PAL-specific vStart adjustments
    if (osTvType == OS_TV_PAL) {
        gTvViMode.fldRegs[0].vStart -= (PAL_HEIGHT_DIFFERENCE << 16);
        gTvViMode.fldRegs[1].vStart -= (PAL_HEIGHT_DIFFERENCE << 16);
        gTvViMode.fldRegs[0].vStart += 16;
        gTvViMode.fldRegs[1].vStart += 16;
    }

    gTvViMode.fldRegs[0].vStart += gVScaleMod * 0x20000;
    gTvViMode.fldRegs[1].vStart += gVScaleMod * 0x20000;
    gTvViMode.fldRegs[0].vStart += gVScaleMod * 0x2;
    gTvViMode.fldRegs[1].vStart += gVScaleMod * 0x2;
    gTvViMode.comRegs.hStart += gHStartMod * 0x20000;
    gTvViMode.comRegs.hStart += gHStartMod * 0x2;

    // Use the custom VI mode and set some special features
    osViSetMode(&gTvViMode);
    osViSetSpecialFeatures(OS_VI_DIVOT_ON);
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
}

void vi_init_framebuffers(int someBool, s32 width, s32 height) {
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

void vi_reset_update_rate(void) {
    D_800BCE58 = 0;
    gViUpdateRate = 2;
    gViUpdateRateTarget = 1;
}

void vi_set_update_rate_target(u32 target) {
    gViUpdateRateTarget = target;
}

s32 vi_frame_sync(s32 param1) {
    s32 updateRate;
    s32 vidMode;

    updateRate = 1;

    if (gViBlackTimer != 0) {
        gViBlackTimer -= 1;

        if (gViBlackTimer == 0) {
            osViBlack(FALSE);
        }
    }

    if (param1 != 8) {
        vi_swap_buffers();
    }

    while (osRecvMesg(&gVideoMesgQueue, NULL, OS_MESG_NOBLOCK) != -1) {
        updateRate += 1;
    }

    gViUpdateRate = updateRate;

    if (gViUpdateRate < gViUpdateRateTarget) {
        gViUpdateRate = gViUpdateRateTarget;
    }

    while (updateRate < gViUpdateRate) {
        osRecvMesg(&gVideoMesgQueue, NULL, OS_MESG_BLOCK);
        updateRate++;
    }

    if (D_80093060 != 0) {
        vidMode = vi_get_mode();

        if (D_80093060 == 3) {
            vi_set_mode(vidMode);
            vi_update_fb_size_from_current_mode(gFramebufferChoice);
            osViSwapBuffer(gFramebufferCurrent);
        } else if (D_80093060 == 2) {
            vi_update_fb_size_from_current_mode(gFramebufferChoice);
            osViSwapBuffer(gFramebufferCurrent);
        } else {
            D_800BCC90.hdr.type = 0x11;
            D_800BCC90.hdr.retQueue = (OSMesgQueue*)&gTvViMode;
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

    joy_read_nonblocking();
    vi_func_8005DEE8();
    osRecvMesg(&gVideoMesgQueue, NULL, OS_MESG_BLOCK);

    return updateRate;
}

void vi_func_8005DC68(void) {}

void vi_func_8005DC70(int _) {}

s32 vi_get_current_frame_rate(void) {
    return (s32)((f32)gDisplayHertz / (f32)gViUpdateRate);
}

/**
 * Swaps gFramebufferCurrent and gFramebufferNext.
 *
 * Uses gFramebufferChoice to keep track of the index for the next framebuffer to swap to.
 */
void vi_swap_buffers(void) {
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

// TODO: these function names are not right
/**
 * Returns gFramebufferStart.
 */
u16 *vi_get_framebuffer_start() {
    return gFramebufferStart;
}

/**
 * Returns gFramebufferEnd.
 */
u16 *vi_get_framebuffer_end() {
    return gFramebufferEnd;
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/vi/vi_func_8005DD4C.s")
#else
s32 vi_func_8005DD4C(s32 x, s32 y, s32 arg2) {
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
    if (vi_contains_point(x, y) == 0) {
        temp_t6 = -1;
    } else {
        temp_t6 = (gCurrentResolutionH[gFramebufferChoice] * y) + x;
    }

    sp24[*sp1C].y = temp_t6;
    *sp1C += 1;
    return sp2C;
}
#endif

/**
 * Whether a 2D point is within the current framebuffer's bounds.
 */
int vi_contains_point(s32 x, s32 y) {
    return x >= 0
        && (u32)x < gCurrentResolutionH[gFramebufferChoice]
        && y >= 0
        && (u32)y < gCurrentResolutionV[gFramebufferChoice];
}

void vi_func_8005DEE8(void) {
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
void vi_some_video_setup(s32 param1) {
    if (param1) {
        vi_set_mode(7);
        vi_init_framebuffers(1, gResolutionArray[7].h, gResolutionArray[7].v);

        vi_update_mode();
        gViBlackTimer = 12;
        osViBlack(TRUE);
        D_800BCE58 = 0;
        D_800BCE2C = 0x5;
    } else {
        vi_set_mode(1);
        vi_init_framebuffers(1, gResolutionArray[0].h, gResolutionArray[0].v);

        vi_update_mode();
        gViBlackTimer = 12;
        osViBlack(TRUE);
        D_800BCE58 = 0;
        D_800BCE2C = 0x5;
    }
}

void vi_set_modifiers(u8 updateViMode, s8 hStartMod, s8 vScaleMod) {
    u8 viModeTableIndex;
    OSViMode *viMode;

    gHStartMod = hStartMod;
    gVScaleMod = vScaleMod;

    // Only update modifiers if not set
    if (!updateViMode) {
        return;
    }

    // Determine VI mode from video mode and TV type
    if (osTvType == OS_TV_PAL) {
        viModeTableIndex = OS_VI_PAL_LPN1;
    } else if (osTvType == OS_TV_MPAL) {
        viModeTableIndex = OS_VI_MPAL_LPN1;
    } else {
        viModeTableIndex = OS_VI_NTSC_LPN1;
    }

    if (gVideoMode == 14) {
        viMode = &osViModeTable[11 + viModeTableIndex];
    } else {
        viMode = &osViModeTable[2 + viModeTableIndex];
    }

    // Copy VI mode so we can make adjustments
    bcopy(viMode, &gTvViMode, sizeof(OSViMode));

    // Make PAL-specific vStart adjustments
    if (osTvType == OS_TV_PAL) {
        gTvViMode.fldRegs[0].vStart -= (PAL_HEIGHT_DIFFERENCE << 16);
        gTvViMode.fldRegs[1].vStart -= (PAL_HEIGHT_DIFFERENCE << 16);
        gTvViMode.fldRegs[0].vStart += 16;
        gTvViMode.fldRegs[1].vStart += 16;
    }

    gTvViMode.fldRegs[0].vStart += gVScaleMod * 0x20000;
    gTvViMode.fldRegs[1].vStart += gVScaleMod * 0x20000;
    gTvViMode.fldRegs[0].vStart += gVScaleMod * 0x2;
    gTvViMode.fldRegs[1].vStart += gVScaleMod * 0x2;
    gTvViMode.comRegs.hStart += gHStartMod * 0x20000;
    gTvViMode.comRegs.hStart += gHStartMod * 0x2;

    D_80093060 = 3;
}
