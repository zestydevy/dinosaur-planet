#include "PR/os.h"
#include "PR/sched.h"
#include "PR/ultratypes.h"
#include "sys/vi.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/shadowtex.h"
#include "constants.h"
#include "macros.h"
#include "memory_regions.h"

#define REFRESH_50HZ 50
#define REFRESH_60HZ 60

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

/**
 * Holds the horizontal and vertical resolution for video related functions.
 */
typedef struct VideoResolution {
    u32 h;
    u32 v;
} VideoResolution;

typedef struct {
    s32 zValue;
    s32 zPixelIdx;
    Object *obj;
} ViObjDepth;

static const char str_8009ace0[] = "320 by 240 Anti-aliased, Non interlaced.\n";
static const char str_8009ad0c[] = "640 by 480 Anti-aliased, Interlaced, De-flickered.\n";
static const char str_8009ad40[] = "vi sizes %d %d %d %d  w %d h %d\n";

void viResetUpdateRate(void);
void viSetMode(s32 mode);
void viInitFramebuffers(int someBool, s32 width, s32 height);
void viSwapBuffers(void);
void viUpdateMode(void);
void viCalcObjDepths(void);
int viContainsPoint(s32 x, s32 y);

extern OSDevMgr __osViDevMgr;

/* -------- .data start 80093010 -------- */
u16 *gDepthBuffer = NULL;
s32 D_80093014 = 0; // unused
s32 D_80093018 = 0x02000000; // unused
s32 D_8009301C = 0; // unused
VideoResolution gResolutionArray[8] = {
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
// From N64 Programming Manual Chapter 15.5.9
/*
 * Convert 11 bit mantissa and 3 bit exponent
 * to 0,15.3 number
 */
struct {
    int shift;
    long add;
} gZConvTable[8] = {
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
u16 *gBackFramebuffer;
u16 *gFrontFramebuffer;
u16 *gBackDepthBuffer;
u16 *gFrontDepthBuffer;
f32 gViHeightRatio;
OSMesg D_800bccc0[8];
OSMesgQueue gVideoMesgQueue;
u8 _bss_800bccf8[0xA8];
OSViMode gTvViMode;
u32 gCurrentResolutionH[2];
u32 gCurrentResolutionV[2];
u16 *gFramebufferPointers[2];
u16 *gFramebufferEnd;
u32 gCurrFramebufferIdx;
s32 gVideoMode;
u32 gViBlackTimer;
ViObjDepth *gViObjDepthLists[2];
u8 gViObjDepthListIdx;
u8 gViObjDepthListLens[2];
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
//static u8 _bss_pad[0x10];
/* -------- .bss end 800bce70 -------- */

void viInit(s32 videoMode, OSSched* scheduler, s32 doOneTimeInit) {
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

    if (doOneTimeInit && osTvType == OS_TV_PAL) {
        for (i = 0; i < ARRAYCOUNT_S(gResolutionArray); i++) {
            gResolutionArray[i].v += 20;
        }
    }

    viResetUpdateRate();

    viSetMode(videoMode);

    gFramebufferPointers[0] = NULL;
    gFramebufferPointers[1] = NULL;

    if (doOneTimeInit) {
        osCreateMesgQueue(&gVideoMesgQueue, D_800bccc0, ARRAYCOUNT(D_800bccc0));
        osScAddClient(scheduler, &gVideoSched, &gVideoMesgQueue, OS_SC_ID_VIDEO);
    }

    width = SCREEN_WIDTH;
    height = SCREEN_HEIGHT + 20;

    if (videoMode == OS_VI_PAL_LPN1) {
        width = HIGH_RES_SCREEN_WIDTH;
        height = HIGH_RES_SCREEN_HEIGHT;
    }

    viInitFramebuffers(doOneTimeInit, width, height);

    gCurrFramebufferIdx = 1;

    viSwapBuffers();
    viUpdateMode();

    gViBlackTimer = 0xc;

    osViBlack(TRUE);

    D_800BCE58 = 0;
    D_800BCE2C = 5;

    if (doOneTimeInit) {
        gViObjDepthLists[0] = mmAlloc(sizeof(ViObjDepth) * 80, ALLOC_TAG_SCREEN_COL, NULL);
        gViObjDepthLists[1] = &gViObjDepthLists[0][80 / 2];
    }

    bzero(gViObjDepthLists[0], sizeof(ViObjDepth) * 80);
    gViObjDepthListIdx = 0;
    gViObjDepthListLens[0] = 0;
    gViObjDepthListLens[1] = 0;

    gViUpdateRateTarget = 1;
}

/**
 * Sets gVideoMode.
 */
void viSetMode(s32 mode) {
    gVideoMode = mode;
}

/**
 * Returns gVideoMode.
 */
s32 viGetMode(void) {
    return gVideoMode;
}

OSMesgQueue *viGetMesgQueue(void) {
    return &gVideoMesgQueue;
}

/**
 * Sets gCurrentResolution*[framebufferIndex] to the resolution
 * specified by gVideoMode from gResolutionArray.
 */
void viUpdateFbSizeFromCurrentMode(int framebufferIndex) {
    // Note: framebufferIndex was decided on because another function calls this
    //       with gCurrFramebufferIdx (which is either 0 or 1 presumably) and
    //       gCurrentResolutionH and gCurrentResolutionV both conveniently contain
    //       2 integers, which are likely for each framebuffer.
    gCurrentResolutionH[framebufferIndex] = gResolutionArray[gVideoMode & 7].h;
    gCurrentResolutionV[framebufferIndex] = gResolutionArray[gVideoMode & 7].v;
}

u32 viGetCurrentSize(void) {
    s32 shadowTexWidth;
    s32 shadowTexActive;

    shadowTexActive = shadowtexGetStatus(&shadowTexWidth);

    if (shadowTexActive == FALSE) {
        return (gCurrentResolutionV[gCurrFramebufferIdx] << 0x10) |
                gCurrentResolutionH[gCurrFramebufferIdx];
    } else {
        return (shadowTexWidth << 0x10) | shadowTexWidth;
    }
}

/**
 * Returns the resolution of the framebuffer not currently in use encoded as 0xVVVV_HHHH.
 *
 * Note: The resolution is found by gCurrentResolution*[gCurrFramebufferIdx < 1]
 */
u32 viGetBackbufferSize(void) {
    return (gCurrentResolutionV[gCurrFramebufferIdx < 1] << 0x10) |
            gCurrentResolutionH[gCurrFramebufferIdx < 1];
}

void viUpdateMode(void) {
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

void viInitFramebuffers(int someBool, s32 width, s32 height) {
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

    if (osMemSize != EXPANSION_RAM_SIZE) {
        // No expansion pack detected
        gFramebufferPointers[0] = (u16*)(COLOR_BUFFERS_ADDR_NO_EXP_PAK);
        gFramebufferPointers[1] = (u16*)(COLOR_BUFFERS_ADDR_NO_EXP_PAK + ((width * height) * 2));
        
        gDepthBuffer = (u16*)(COLOR_BUFFERS_ADDR_NO_EXP_PAK); // dummy depth buffer
        return;
    }
    
    if (height == 480) {
        // High resolution (menus, 640x480)
        gFramebufferPointers[0] = (u16*)(COLOR_BUFFERS_ADDR_EXP_PAK);
        gFramebufferPointers[1] = (u16*)(COLOR_BUFFERS_ADDR_EXP_PAK + ((width * height) * 2));
        
        gDepthBuffer = (u16*)(COLOR_BUFFERS_ADDR_EXP_PAK); // dummy depth buffer
    } else {
        // Normal resolution (gameplay, 320x240)
        gFramebufferPointers[0] = (u16*)(COLOR_BUFFERS_ADDR_EXP_PAK);
        gFramebufferPointers[1] = (u16*)(COLOR_BUFFERS_ADDR_EXP_PAK + ((width * height) * 2));
        
        gFramebufferEnd = (u16*)(((int) (COLOR_BUFFERS_ADDR_EXP_PAK + ((width * height) * 2))) + ((width * height) * 2));
        gDepthBuffer = (u16*)DEPTH_BUFFER_ADDR;
    }
}

void viResetUpdateRate(void) {
    D_800BCE58 = 0;
    gViUpdateRate = 2;
    gViUpdateRateTarget = 1;
}

void viSetUpdateRateTarget(u32 target) {
    gViUpdateRateTarget = target;
}

s32 viFrameSync(s32 param1) {
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
        viSwapBuffers();
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
        vidMode = viGetMode();

        if (D_80093060 == 3) {
            viSetMode(vidMode);
            viUpdateFbSizeFromCurrentMode(gCurrFramebufferIdx);
            osViSwapBuffer(gFrontFramebuffer);
        } else if (D_80093060 == 2) {
            viUpdateFbSizeFromCurrentMode(gCurrFramebufferIdx);
            osViSwapBuffer(gFrontFramebuffer);
        } else {
            D_800BCC90.hdr.type = 0x11;
            D_800BCC90.hdr.retQueue = (OSMesgQueue*)&gTvViMode;
            osSendMesg(__osViDevMgr.evtQueue, &D_800BCC90, OS_MESG_BLOCK);
            osViSwapBuffer(gFrontFramebuffer);
            D_80093064 ^= 1;
        }

        D_80093060 -= 1;
    } else {
        if (mainGetPauseState() == 1) {
            // Create pause screen screenshot
            mainSetPauseState(2);
            bcopy(gBackFramebuffer, gFramebufferEnd, (320 * 240 * sizeof(u16)));
        } else {
            osViSwapBuffer(gFrontFramebuffer);
        }
    }

    joyReadNonblocking();
    viCalcObjDepths();
    osRecvMesg(&gVideoMesgQueue, NULL, OS_MESG_BLOCK);

    return updateRate;
}

void vi_func_8005DC68(void) {}

void vi_func_8005DC70(int _) {}

s32 viGetCurrentFrameRate(void) {
    return (s32)((f32)gDisplayHertz / (f32)gViUpdateRate);
}

void viSwapBuffers(void) {
    gFrontFramebuffer = gFramebufferPointers[gCurrFramebufferIdx];
    gFrontDepthBuffer = gDepthBuffer;

    gCurrFramebufferIdx ^= 1;

    gBackFramebuffer = gFramebufferPointers[gCurrFramebufferIdx];
    gBackDepthBuffer = gDepthBuffer;
}

u16 *viGetDepthBuffer(void) {
    return gDepthBuffer;
}

/**
 * Returns gFramebufferEnd.
 */
u16 *viGetFramebufferEnd(void) {
    return gFramebufferEnd;
}

s32 viObjDepth(s32 x, s32 y, Object *obj) {
    s32 zvalue;
    s32 i;
    ViObjDepth* list;
    u8 prevListIdx;
    u8 currListIdx;
    
    currListIdx = gViObjDepthListIdx;
    prevListIdx = gViObjDepthListIdx ^ 1;
    list = gViObjDepthLists[prevListIdx];
    
    // Find screen Z of object from previous frame
    zvalue = 0;
    for (i = 0; i < gViObjDepthListLens[prevListIdx]; i++) {
        if (obj == list[i].obj) {
            zvalue = list[i].zValue;
            break;
        }
    }

    list = gViObjDepthLists[currListIdx];
    list[gViObjDepthListLens[currListIdx]].obj = obj;
    list[gViObjDepthListLens[currListIdx]].zValue = 0;
    if (viContainsPoint(x, y) == 0) {
        i = -1;
    } else {
        i = (gCurrentResolutionH[gCurrFramebufferIdx] * y) + x;
    }

    list[gViObjDepthListLens[currListIdx]].zPixelIdx = i;
    gViObjDepthListLens[currListIdx] += 1;
    return zvalue;
}

/**
 * Whether a 2D point is within the current framebuffer's bounds.
 */
int viContainsPoint(s32 x, s32 y) {
    return x >= 0
        && (u32)x < gCurrentResolutionH[gCurrFramebufferIdx]
        && y >= 0
        && (u32)y < gCurrentResolutionV[gCurrFramebufferIdx];
}

void viCalcObjDepths(void) {
    s32 i;
    u8 listIdx;
    s32 idx;
    u32 zfloat;
    ViObjDepth *list;
    u32 exponent;
    u32 mantissa;

    listIdx = gViObjDepthListIdx;
    list = gViObjDepthLists[listIdx];
    for (i = 0; i < gViObjDepthListLens[listIdx]; i++) {
        idx = list[i].zPixelIdx;
        if (idx >= 0) {
            // 16-bit depth buffer bit format: 15-13 (Zexp), 12-2 (Zmantissa), 1-0 (hidden)
            zfloat = gDepthBuffer[idx] >> 2; // skip hidden bits
            exponent = (zfloat >> 11) & 7;
            mantissa = zfloat & 0x7FF;
            // Convert to 15.3 fixed point, then extract the integer portion
            list[i].zValue = ((mantissa << gZConvTable[exponent].shift) + gZConvTable[exponent].add) >> 3;
        } else {
            list[i].zValue = 0;
        }
    }

    gViObjDepthListIdx ^= 1;
    gViObjDepthListLens[gViObjDepthListIdx] = 0;
}

/**
 * Note: param1 is most likely a boolean
 */
void viSomeVideoSetup(s32 param1) {
    if (param1) {
        viSetMode(7);
        viInitFramebuffers(1, gResolutionArray[7].h, gResolutionArray[7].v);

        viUpdateMode();
        gViBlackTimer = 12;
        osViBlack(TRUE);
        D_800BCE58 = 0;
        D_800BCE2C = 0x5;
    } else {
        viSetMode(1);
        viInitFramebuffers(1, gResolutionArray[0].h, gResolutionArray[0].v);

        viUpdateMode();
        gViBlackTimer = 12;
        osViBlack(TRUE);
        D_800BCE58 = 0;
        D_800BCE2C = 0x5;
    }
}

void viSetModifiers(u8 updateViMode, s8 hStartMod, s8 vScaleMod) {
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
