#include "PR/os.h"
#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "sys/asset_thread.h"
#include "sys/fs.h"
#include "sys/gfx/map.h"
#include "sys/memory.h"
#include "functions.h"
#include "macros.h"

static s32 sLoadedScreenNo;
static s32 sRenderScreen;
static s32 sLoadedScreenByteLength;
static u16 *sLoadedScreen;

void screens_ctor(void *self) {
    sLoadedScreenNo = -1;
    sLoadedScreen = NULL;
    sLoadedScreenByteLength = 0;
    sRenderScreen = FALSE;
}

void screens_dtor(void *self) {
    // This DLL is never unloaded but it really should free sLoadedScreen here...
}

void screens_show_screen(s32 screenNo) {
    u32 *screensTab;
    s32 offset;
    s32 length;
    s32 numScreens;

    screensTab = NULL;
    
    if (screenNo != sLoadedScreenNo) {
        queue_alloc_load_file((void**)&screensTab, SCREENS_TAB);

        numScreens = 0;
        while (screensTab[numScreens] != 0xFFFFFFFF) {
            numScreens++;
        }
        numScreens--;

        if (screenNo < 0 || screenNo >= numScreens) {
            screenNo = 0;
            STUBBED_PRINTF(("SCREEN: No out of range!!\n"));
        }

        offset = screensTab[screenNo];
        length = screensTab[screenNo + 1] - offset;

        if (length != sLoadedScreenByteLength) {
            if (sLoadedScreen != NULL) {
                mmFree(sLoadedScreen);
            }

            sLoadedScreen = mmAlloc(length, ALLOC_TAG_SCREEN_COL, NULL);
        }

        sLoadedScreenByteLength = length;
        queue_load_file_region_to_ptr((void*)sLoadedScreen, SCREENS_BIN, offset, length);

        mmFree(screensTab);
        sLoadedScreenNo = screenNo;
    }

    sRenderScreen = TRUE;
}

void screens_hide_screen() {
    if (sLoadedScreen != NULL) {
        mmFree(sLoadedScreen);
        sLoadedScreen = NULL;
        sLoadedScreenNo = -1;
        sLoadedScreenByteLength = 0;
        sRenderScreen = FALSE;
    }
}

void screens_draw(Gfx **gdl) {
    s32 width;
    s32 yPos;
    s32 height;
    s32 chunkSize;
    u16 *ptr;

    chunkSize = 2;

    if (!sRenderScreen) {
        if (sLoadedScreen != NULL) {
            mmFree(sLoadedScreen);
            sLoadedScreen = NULL;
            sLoadedScreenByteLength = 0;
            sLoadedScreenNo = -1;
        }
    } else if (sLoadedScreen != NULL) {
        sRenderScreen = FALSE;

        width = sLoadedScreen[0];
        height = sLoadedScreen[1];

        ptr = sLoadedScreen;
        ptr += 8;

        gDPSetCombineLERP((*gdl), TEXEL0, 0, SCALE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, SCALE, 0, 0, 0, 0, TEXEL0);
        dl_apply_combine(gdl);

        gDPSetOtherMode((*gdl), 
            G_AD_PATTERN | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_COPY | G_PM_NPRIMITIVE, 
            G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_NOOP2);
        dl_apply_other_mode(gdl);

        yPos = 0;
        do {
            gDPLoadTextureBlockS((*gdl)++,
                /*timg*/    OS_K0_TO_PHYSICAL(ptr),
                /*fmt*/     G_IM_FMT_RGBA,
                /*siz*/     G_IM_SIZ_16b,
                /*width*/   width,
                /*height*/  chunkSize,
                /*pal*/     0,
                /*cms*/     G_TX_CLAMP,
                /*cmt*/     G_TX_CLAMP,
                /*masks*/   G_TX_NOMASK,
                /*maskt*/   G_TX_NOMASK,
                /*shifts*/  G_TX_NOLOD,
                /*shiftt*/  G_TX_NOLOD);

            gSPTextureRectangle((*gdl)++,
                /*ulx*/ 0,
                /*uly*/ yPos << 2,
                /*lrx*/ width << 2,
                /*lry*/ (yPos + 1) << 2,
                /*tile*/G_TX_RENDERTILE,
                /*s*/   0,
                /*t*/   0,
                /*dsdx*/1 << 12,
                /*dtdy*/1 << 10);
        
            gDLBuilder->needsPipeSync = TRUE;

            ptr += width * chunkSize;
            yPos += chunkSize;
        } while (yPos < height);
    }
}
