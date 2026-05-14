#include "PR/gbi.h"
#include "sys/camera.h"
#include "sys/map.h"
#include "sys/rcp.h"
#include "sys/rsp_segment.h"
#include "sys/scheduler.h"
#include "sys/vi.h"
#include "macros.h"

// .data

s16 D_80091780[16] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
u8 sBGPrimColourR = 0;
u8 sBGPrimColourG = 0;
u8 sBGPrimColourB = 0;
u32 sBackgroundFillColour = GPACK_RGBA5551(0, 0, 0, 1) | (GPACK_RGBA5551(0, 0, 0, 1) << 16);
u32 D_800917B0 = 64;
s32 D_800917B4 = 0;
s32 D_800917B8 = 0;
s32 D_800917BC = 0;
s32 gGfxBufCounter = 0;
s32 D_800917C4 = 0;
s32 gGfxTaskIsRunning = 0;
u8 D_800917CC = 1;
Gfx D_800917D0[] = {
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPClipRatio(FRUSTRATIO_3),
    gsSPEndDisplayList()
};
Gfx D_80091800[] = {
    gsDPPipeSync(),
    gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_FILL | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_OPA_SURF | G_RM_OPA_SURF2),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE),
    gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_OPA_SURF | G_RM_OPA_SURF2)
};
Gfx D_80091828[] = {
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_AA_OPA_SURF | G_RM_AA_OPA_SURF2),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_OPA_SURF | G_RM_OPA_SURF2),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_AA_OPA_SURF | G_RM_AA_OPA_SURF2),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_OPA_SURF | G_RM_OPA_SURF2)
};
Gfx D_80091868[] = {
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_AA_XLU_SURF | G_RM_AA_XLU_SURF2),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_AA_XLU_SURF | G_RM_AA_XLU_SURF2),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2)
};

// -------- .bss start 800b3790 -------- //
u8 gGfxDramStack[SP_DRAM_STACK_SIZE8];
OSMesgQueue gRCPUnusedMesgQueue1;
OSMesg gRCPUnusedMesgBuf1[1];
OSMesgQueue gRCPUnusedMesgQueue2;
OSMesgQueue gGfxTaskMesgQueue;
OSMesg gRCPUnusedMesgBuf2[8];
OSMesg gGfxTaskMesgBuf[8];
OSScTask gGfxTasks[2];
OSMesgQueue *gScInterruptQ;
u8 _bss_800b3d08[0x18];
u8 gGfxYieldData[OS_YIELD_DATA_SIZE];
// -------- .bss end 800b4920 -------- //

/**
 * Prepare the gfx task for the F3DEX2 XBus microcode.
 * Sends a message to the scheduler to start processing an RSP task once set up.
 * Official Name: rcpFast3d
 */
s32 gfxtask_run_xbus(Gfx *dlStart, Gfx *dlEnd, s32 param3) {
    OSScTask *task;
    
    gGfxTaskIsRunning = TRUE;

    task = &gGfxTasks[gGfxBufCounter];

    gGfxBufCounter++;
    if (gGfxBufCounter == ARRAYCOUNT(gGfxTasks)) {
        gGfxBufCounter = 0;
    }

    task->flags = OS_SC_NEEDS_RDP | OS_SC_NEEDS_RSP | OS_SC_LAST_TASK;
    task->msgQ = &gGfxTaskMesgQueue;
    task->unk58 = 0xff0000ff;
    task->unk5C = 0xff0000ff;
    task->taskType = OS_SC_TASK_GAME;
    task->list.t.data_ptr = (u64*)dlStart;
    task->list.t.data_size = ((s32)dlEnd - (s32)dlStart) >> 3;
    task->list.t.type = M_GFXTASK;
    task->list.t.flags = OS_TASK_DP_WAIT | OS_TASK_LOADABLE;
    task->list.t.ucode_boot = (u64*)rspbootTextStart;
    task->list.t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart; 
    if (D_800917CC != 0) {
        task->list.t.ucode = (u64*)gspF3DEX2_xbusTextStart;
        task->list.t.ucode_data = (u64*)gspF3DEX2_xbusDataStart;
    }
    task->list.t.ucode_data_size = SP_UCODE_DATA_SIZE;
    task->list.t.dram_stack = (u64*)gGfxDramStack;
    task->list.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
    if (D_800917CC != 0) {
        task->list.t.output_buff = NULL;
        task->list.t.output_buff_size = NULL;
    }
    task->list.t.yield_data_ptr = (u64*)gGfxYieldData;
    task->list.t.yield_data_size = sizeof(gGfxYieldData);
    task->next = NULL;
    task->framebuffer = (void*)gBackFramebuffer;
    task->unk60 = 0xff;
    task->unk64 = 0xff;

    osWritebackDCacheAll();

    osSendMesg(gScInterruptQ, (OSMesg)task, OS_MESG_BLOCK);

    return 0;
}

void func_800378E8(s32 a0, s32 a1, s32 a2) {

}

void func_800378FC(s32 a0, s32 a1, s32 a2) {

}

void func_80037910(s32 a0, s32 a1, s32 a2) {

}

s32 gfxtask_wait(void) {
    GfxTaskMesg *mesg = NULL;

    if (!gGfxTaskIsRunning) {
        return 0;
    }

    osRecvMesg(&gGfxTaskMesgQueue, (OSMesg)&mesg, OS_MESG_BLOCK);
    gGfxTaskIsRunning = FALSE;

    return mesg->unk4;
}

void func_80037978(s32 a0, s32 a1, s32 a2) {

}

/**
 * Sets the primitive colour for the cyclemode fillrect background.
 * Official name: rcpSetScreenColour
 */
void rcp_set_screen_color(u8 red, u8 green, u8 blue) {
    sBGPrimColourR = red;
    sBGPrimColourG = green;
    sBGPrimColourB = blue;
}

/**
 * Sets the fill colour for the fillmode fillrect background.
 * Uses RGBA5551
 * Official name: rcpSetBorderColour
 */
void rcp_set_border_color(u32 red, u32 green, s32 blue) {
    sBackgroundFillColour = GPACK_RGBA5551(red, green, blue, 1);
    sBackgroundFillColour |= (sBackgroundFillColour << 16);
}

// official name: rcpClearScreen
void rcp_clear_screen(Gfx **gdl, Mtx **mtx, s32 flags) {
    s32 viSize;
    s32 viWidth, viHeight;
    s32 ulx, uly, lrx, lry;
    s32 letterbox;

    viewport_get_full_rect(&ulx, &uly, &lrx, &lry);

    letterbox = camera_get_letterbox();

    viSize = vi_get_current_size();
    viWidth = GET_VIDEO_WIDTH(viSize);
    viHeight = GET_VIDEO_HEIGHT(viSize);

    gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, 0, 0, viWidth - 1, viHeight - 1);

    gDPSetCombineMode((*gdl), G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    dl_apply_combine(gdl);

    gDPSetOtherMode((*gdl), 
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_FILL |  G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_OPA_SURF | G_RM_OPA_SURF2);
    dl_apply_other_mode(gdl);

    if (gDLBuilder->needsPipeSync) {
        gDLBuilder->needsPipeSync = FALSE;
        gDPPipeSync((*gdl)++);
    }

    gDPSetColorImage((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, viWidth, SEGMENT_ADDR(SEGMENT_ZBUFFER, 0x0));

    if ((flags & CLEAR_ZBUFFER) != 0) {
        dl_set_fill_color(gdl, (GPACK_ZDZ(G_MAXFBZ, 0) << 16) | GPACK_ZDZ(G_MAXFBZ, 0));

        gDPFillRectangle((*gdl)++, ulx, uly, lrx, lry);

        gDLBuilder->needsPipeSync = TRUE;
    }

    if (gDLBuilder->needsPipeSync) {
        gDLBuilder->needsPipeSync = FALSE;
        gDPPipeSync((*gdl)++);
    }

    gDPSetColorImage((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, viWidth, SEGMENT_ADDR(SEGMENT_FRAMEBUFFER, 0x0));

    if ((flags & CLEAR_COLOR) != 0 || letterbox != 0) {
        dl_set_fill_color(gdl, 
            (GPACK_RGBA5551(sBGPrimColourR, sBGPrimColourG, sBGPrimColourB, 1) << 16) 
                | GPACK_RGBA5551(sBGPrimColourR, sBGPrimColourG, sBGPrimColourB, 1));

        if ((flags & CLEAR_COLOR) != 0) {
            gDPFillRectangle((*gdl)++, 0, 0, viWidth - 1, viHeight - 1);
            gDLBuilder->needsPipeSync = TRUE;
        } else if (letterbox != 0) {
            gDPFillRectangle((*gdl)++, 0, 0, viWidth - 1, uly - 1);
            gDLBuilder->needsPipeSync = TRUE;

            gDPFillRectangle((*gdl)++, 0, lry, viWidth - 1, viHeight - 1);
            gDLBuilder->needsPipeSync = TRUE;
        }
    }

    camera_apply_scissor(gdl);
}

// unused
void rdp_init(Gfx **gdl) {
    s32 resolution;
    s32 resWidth;

    resolution = vi_get_current_size();
    resWidth = GET_VIDEO_WIDTH(resolution);

    if (gDLBuilder->needsPipeSync) {
        gDLBuilder->needsPipeSync = FALSE;
        gDPPipeSync((*gdl)++);
    }

    gDPSetColorImage((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, resWidth, SEGMENT_ADDR(SEGMENT_FRAMEBUFFER, 0x0));

    if (gDLBuilder->needsPipeSync) {
        gDLBuilder->needsPipeSync = FALSE;
        gDPPipeSync((*gdl)++);
    }

    gDPSetDepthImage((*gdl)++, SEGMENT_ADDR(SEGMENT_ZBUFFER, 0x0));

    gDPSetCombineMode((*gdl), G_CC_DECALRGB, G_CC_DECALRGB);
    dl_apply_combine(gdl);

    gDPSetOtherMode((*gdl), 
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_1PRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_OPA_SURF | G_RM_OPA_SURF2);
    dl_apply_other_mode(gdl);
}

void rsp_init(Gfx **gdl) {
    // Clear all
    gSPClearGeometryMode((*gdl), 0xFFFFFF);
    dl_apply_geometry_mode(gdl);

    gSPDisplayList((*gdl)++, OS_K0_TO_PHYSICAL(D_800917D0));
}

void gfxtask_init(OSSched *sched) {
    gScInterruptQ = osScGetInterruptQ(sched);

    osCreateMesgQueue(&gRCPUnusedMesgQueue1, gRCPUnusedMesgBuf1, ARRAYCOUNT(gRCPUnusedMesgBuf1));
    osCreateMesgQueue(&gRCPUnusedMesgQueue2, gRCPUnusedMesgBuf2, ARRAYCOUNT(gRCPUnusedMesgBuf2));
    osCreateMesgQueue(&gGfxTaskMesgQueue, gGfxTaskMesgBuf, ARRAYCOUNT(gGfxTaskMesgBuf));
}

void func_80037F8C(s32 param1) {
    D_800917BC = param1;
}

// official name: rcpTileWrite ?
void rcp_tile_write(Gfx** gdl, TextureTile* tiles, s32 x, s32 y, u8 r, u8 g, u8 b, u8 a) {
    Texture* tile;
    Texture* tex;
    s32 lrx;
    s32 lry;
    s32 frameno;
    s32 ulx;
    s32 uly;
    s32 s;
    s32 t;
    s32 i;
    s32 j;
    Gfx* dl;

    j = 0;
    dl = *gdl;
    gSPLoadGeometryMode(dl, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    dl_apply_geometry_mode(&dl);
    gDPSetCombineMode(dl, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    dl_apply_combine(&dl);
    gDPSetOtherMode(
        dl, 
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2
    );
    dl_apply_other_mode(&dl);
    x *= 4;
    y *= 4;
    tex = tiles->tex;
    while (tex != NULL) {
        ulx = (tiles[j].x * 4) + x;
        uly = (tiles[j].y * 4) + y;
        lrx = (tex->width * 4) + ulx;
        lry = (tex->height * 4) + uly;
        if (lrx > 0 && lry > 0) {
            s = 0;
            t = 0;
            if (ulx < 0) {
                s = -(ulx * 8);
                ulx = 0;
            }
            if (uly < 0) {
                t = -(uly * 8);
                uly = 0;
            }
            frameno = (s32) tiles[j].animProgress >> 8;
            for (tile = tex, i = 0; i < frameno && tile != NULL; i++) {
                tile = tile->next;
            }
            dl->words.w0 = tile->gdl->words.w0;
            dl->words.w1 = (unsigned int) OS_PHYSICAL_TO_K0(tile + 1);
            dl++;
            gSPDisplayList(dl++, OS_PHYSICAL_TO_K0(tile->gdl + 1));
            dl_set_prim_color(&dl, r, g, b, a);
            gSPTextureRectangle(dl++, 
            /*ulx*/ ulx,
            /*uly*/ uly,
            /*lrx*/ lrx,
            /*lry*/ lry,
            /*tile*/ 0,
            /*s*/ s,
            /*t*/ t,
            /*dsdx*/ 1 << 10,
            /*dsdy*/ 1 << 10
            );
            gDLBuilder->needsPipeSync = 1;
        }
        j++;
        tex = tiles[j].tex;
    }
    tex_render_reset();
    *gdl = dl;
}

void rcp_screen_full_write(Gfx** gdl, Texture* tex, s32 x, s32 y, s32 arg4, s32 frameno, s32 alpha, s32 flags) {
    s32 texHeight = tex->height | ((tex->widthHeightHi & 0xF) << 8);
    rcp_screen_write(gdl, tex, x, y, 0, texHeight, frameno, alpha, flags);
}

void rcp_screen_scroll_write(Gfx** gdl, Texture* tex, s32 dstX, s32 dstY, s32 top, s32 bottom, s32 alpha, s32 flags) {
    s32 texHeight;

    top -= dstY;
    texHeight = tex->height | ((tex->widthHeightHi & 0xF) << 8);
    if (top < 0) {
        top = 0;
    }
    bottom -= dstY;
    if (texHeight < bottom) {
        bottom = texHeight;
    }
    if (top < texHeight && bottom >= 0) {
        rcp_screen_write(gdl, tex, dstX, dstY, top, bottom, 0, alpha, flags);
    }
}

// official name: rcpScreenWrite (default.dol)
void rcp_screen_write(Gfx** gdl, Texture* tex, s32 dstX, s32 dstY, s32 srcTop, s32 srcBottom, s32 frameno, s32 alpha, s32 flags) {
    s32 width;
    s32 sp148;
    s32 temp_v0_9;
    s32 y;
    s32 height;
    s32 srcY;
    s32 texFormat;
    s32 bytesPerPx;
    s32 sp12C;
    s32 sp128;
    s32 numFrames;
    u8* img;
    Texture* frameTex;

    sp12C = track_func_80041DBC();
    sp128 = track_func_80041E08();
    
    if (tex->animDuration != 0) {
        numFrames = (s32) tex->animDuration >> 8;
    } else {
        numFrames = 0;    
    }
    frameTex = tex;
    if ((numFrames > 1) && (frameno < numFrames)) {
        for (numFrames = 0; numFrames < frameno && frameTex != NULL; numFrames++) {
            frameTex = frameTex->next;
        }
    }
    gSPGeometryMode(*gdl, 0xFFFFFF, 0);
    dl_apply_geometry_mode(gdl);
    width = tex->width | ((tex->widthHeightHi & 0xF0) << 4);
    if (sp12C != 0) {
        sp148 = (s32) ((f32)tex->width * 1) | ((tex->widthHeightHi & 0xF0) << 4);
    } else {
        sp148 = width;
    }
    texFormat = TEX_FORMAT(tex->format);
    if (texFormat == TEX_FORMAT_RGBA16) {
        bytesPerPx = 2;
        height = (0x800 / width) & ~1;
    } else if (texFormat == TEX_FORMAT_IA8) {
        bytesPerPx = 1;
        height = (0x1000 / width) & ~1;
    } else {
        bytesPerPx = 4;
        height = (0x400 / width) & ~1;
    }
    if (height == 0) {
        return;
    }

    img = (u8*)frameTex + (width * srcTop * bytesPerPx) + sizeof(Texture);
    if (flags & SCREEN_WRITE_CYC_COPY) {
        gDPSetCombineMode(*gdl, G_CC_DECALRGBA, G_CC_DECALRGBA);
        dl_apply_combine(gdl);
        gDPSetOtherMode(
            *gdl, 
            G_AD_PATTERN | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_COPY | G_PM_NPRIMITIVE, 
            G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_NOOP2
        );
        dl_apply_other_mode(gdl);
    } else if ((alpha == 255) && (flags & SCREEN_WRITE_OPAQUE)) {
        gDPSetCombineMode(*gdl, G_CC_DECALRGBA, G_CC_DECALRGBA);
        dl_apply_combine(gdl);
        gDPSetOtherMode(
            *gdl, 
            G_AD_PATTERN | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
            G_AC_NONE | G_ZS_PIXEL | G_RM_OPA_SURF | G_RM_OPA_SURF2
        );
        dl_apply_other_mode(gdl);
    } else {
        gDPSetCombineLERP(*gdl, 0, 0, 0, TEXEL0, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, PRIMITIVE, 0);
        dl_apply_combine(gdl);
        gDPSetOtherMode(
            *gdl, 
            G_AD_PATTERN | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
            G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2
        );
        dl_apply_other_mode(gdl);
    }
    dl_set_prim_color(gdl, 255, 255, 255, alpha);
    srcY = srcTop;
    do {
        temp_v0_9 = srcBottom - srcY;
        if (temp_v0_9 < height) {
            height = temp_v0_9;
        }
        if (texFormat == TEX_FORMAT_RGBA16) {
            // Should be this but DP doesn't like using this macro?
            // causes an incorrect use of gDPSetTile where the tile is set to 2 and a regswap of t0 and t1
            // gDPLoadMultiBlockS(
            //     /* pkt */ (*gdl)++,
            //     /* timg */ OS_PHYSICAL_TO_K0(var_s6),
            //     /* tmem */ 0,
            //     /* rtile */ 0,
            //     /* fmt */ G_IM_FMT_RGBA,
            //     /* siz */ G_IM_SIZ_16b,
            //     /* width */ width,
            //     /* height */ height,
            //     /* pal */ G_TX_NOMIRROR | G_TX_CLAMP,
            //     /* cms */ G_TX_NOMIRROR | G_TX_CLAMP,
            //     /* cmt */ G_TX_NOMIRROR | G_TX_CLAMP,
            //     /* masks */ G_TX_NOMASK,
            //     /* maskt */ G_TX_NOMASK,
            //     /* shifts */ G_TX_NOLOD,
            //     /* shiftt */ G_TX_NOLOD
            // );
            gDPSetTextureImage((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, OS_PHYSICAL_TO_K0(img));
            gDPSetTile((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
            gDPLoadSync((*gdl)++);
            gDPLoadBlock((*gdl)++, G_TX_LOADTILE, 0, 0, width * height - 1, 0);
            gDPPipeSync((*gdl)++);
            gDPSetTile((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, ((width << 1) + 7) >> 3, G_TX_RENDERTILE, 0, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
            gDPSetTileSize((*gdl)++, 0, 0, 0, (width - 1) << 2, ((height - 1) << 2));
        } else if (texFormat == TEX_FORMAT_IA8) {
                // Should be this but DP doesn't like using this macro?
                // Instead they added a "but" when using gDPSetTile by passing in the incorrect format
                // gDPLoadMultiBlockS(
                //     /* pkt */ (*gdl)++,
                //     /* timg */ OS_PHYSICAL_TO_K0(var_s6),
                //     /* tmem */ 0,
                //     /* rtile */ 0,
                //     /* fmt */ G_IM_FMT_IA,
                //     /* siz */ G_IM_SIZ_16b,
                //     /* width */ width,
                //     /* height */ height, // may be incorrect
                //     /* pal */ G_TX_NOMIRROR | G_TX_CLAMP,
                //     /* cms */ G_TX_NOMIRROR | G_TX_CLAMP,
                //     /* cmt */ G_TX_NOMIRROR | G_TX_CLAMP,
                //     /* masks */ G_TX_NOMASK,
                //     /* maskt */ G_TX_NOMASK,
                //     /* shifts */ G_TX_NOLOD,
                //     /* shiftt */ G_TX_NOLOD
                // );
                gDPSetTextureImage((*gdl)++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, OS_PHYSICAL_TO_K0(img));
                gDPSetTile((*gdl)++, G_IM_FMT_IA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
                gDPLoadSync((*gdl)++);
                gDPLoadBlock((*gdl)++, G_TX_LOADTILE, 0, 0, ((width * height + 1) >> 1) - 1, 0);
                gDPPipeSync((*gdl)++);
                // ?????????????? why G_IM_SIZ_8b here instead of 16b?
                gDPSetTile((*gdl)++, G_IM_FMT_IA, G_IM_SIZ_8b, (width + 7) >> 3, G_TX_RENDERTILE, 0, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
                gDPSetTileSize((*gdl)++, 0, 0, 0, (width - 1) << 2, ((height - 1) << 2));
        } else {
            // Should be this but DP doesn't like using this macro?
            // causes an incorrect use of gDPSetTile where the tile is set to 2 and a regswap of t0 and t1
            // gDPLoadMultiBlockS(
            //     /* pkt */ (*gdl)++,
            //     /* timg */ OS_PHYSICAL_TO_K0(var_s6),
            //     /* tmem */ 0,
            //     /* rtile */ 0,
            //     /* fmt */ G_IM_FMT_RGBA,
            //     /* siz */ G_IM_SIZ_32b,
            //     /* width */ width,
            //     /* height */ height,
            //     /* pal */ G_TX_NOMIRROR | G_TX_CLAMP,
            //     /* cms */ G_TX_NOMIRROR | G_TX_CLAMP,
            //     /* cmt */ G_TX_NOMIRROR | G_TX_CLAMP,
            //     /* masks */ G_TX_NOMASK,
            //     /* maskt */ G_TX_NOMASK,
            //     /* shifts */ G_TX_NOLOD,
            //     /* shiftt */ G_TX_NOLOD
            // );
            gDPSetTextureImage((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, OS_PHYSICAL_TO_K0(img));
            gDPSetTile((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
            gDPLoadSync((*gdl)++);
            gDPLoadBlock((*gdl)++, G_TX_LOADTILE, 0, 0, width * height - 1, 0);
            gDPPipeSync((*gdl)++);
            gDPSetTile((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_32b, ((width << 1) + 7) >> 3, G_TX_RENDERTILE, 0, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
            gDPSetTileSize((*gdl)++, 0, 0, 0, (width - 1) << 2, ((height - 1) << 2));
        }

        y = dstY + srcY;
        if (flags & SCREEN_WRITE_CYC_COPY) {
            gSPTextureRectangle(
            /* pkt */ (*gdl)++,
            /* xl */ dstX << 2,
            /* yl */ y << 2,
            /* xh */ (dstX + width) << 2,
            /* yh */ (y + height - 1) << 2,
            /* tile */ 0,
            /* s */ 0,
            /* t */ 0,
            /* dsdx */ 1 << 12,
            /* dsdy */ 1 << 10
            );
            gDLBuilder->needsPipeSync = 1;
        } else if ((sp128 != 0) && (sp12C != 0)) {
            gSPTextureRectangle(
            /* pkt */ (*gdl)++,
            /* xl */ dstX << 2,
            /* yl */ y << 2,
            /* xh */ (dstX + sp148) << 2,
            /* yh */ (y + height) << 2,
            /* tile */ 0,
            /* s */ 0,
            /* t */ 0,
            /* dsdx */ 0x4FF,
            /* dsdy */ 0x4FF
            );
            gDLBuilder->needsPipeSync = 1;
        } else if (sp12C != 0) {
            gSPTextureRectangle(
            /* pkt */ (*gdl)++,
            /* xl */ dstX << 2,
            /* yl */ y << 2,
            /* xh */ (dstX + sp148) << 2,
            /* yh */ (y + height) << 2,
            /* tile */ 0,
            /* s */ 0,
            /* t */ 0,
            /* dsdx */ 0x4FF,
            /* dsdy */ 1 << 10
            );
            gDLBuilder->needsPipeSync = 1;
        } else if (sp128 != 0) {
            gSPTextureRectangle(
            /* pkt */ (*gdl)++,
            /* xl */ dstX << 2,
            /* yl */ y << 2,
            /* xh */ (dstX + sp148) << 2,
            /* yh */ (y + height) << 2,
            /* tile */ 0,
            /* s */ 0,
            /* t */ 0,
            /* dsdx */ 1 << 10,
            /* dsdy */ 1 << 10
            );
            gDLBuilder->needsPipeSync = 1;
        } else {
            gSPTextureRectangle(
            /* pkt */ (*gdl)++,
            /* xl */ dstX << 2,
            /* yl */ y << 2,
            /* xh */ (dstX + width) << 2,
            /* yh */ (y + height) << 2,
            /* tile */ 0,
            /* s */ 0,
            /* t */ 0,
            /* dsdx */ 1 << 10,
            /* dsdy */ 1 << 10
            );
            gDLBuilder->needsPipeSync = 1;
        }
        srcY += height;
        img = (u8*)img + width * height * bytesPerPx;
    } while (srcY < srcBottom);
}

void draw_pause_screen_freeze_frame(Gfx** gdl) {
    u16* fbPtr;
    s32 remainingY;
    s32 yPos;
    s32 height;
    s32 chunkSize;
    s32 width;

    width = 320;
    height = 240;
    
    fbPtr = vi_get_framebuffer_end();
    
    chunkSize = 6;
    
    gSPClearGeometryMode(*gdl, 0xFFFFFF);
    dl_apply_geometry_mode(gdl);
    
    gDPSetCombineMode(*gdl, G_CC_DECALRGBA, G_CC_DECALRGBA);
    dl_apply_combine(gdl);
   
    gDPSetOtherMode(*gdl, 
        G_AD_PATTERN | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | 
            G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_COPY | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_NOOP2);
    dl_apply_other_mode(gdl);
    
    yPos = 0;

    while (yPos < height) {
        remainingY = height - yPos;

        if (remainingY < chunkSize) {
            chunkSize = remainingY;
        }

        gDPLoadTextureBlock((*gdl)++,
            /*timg*/    OS_PHYSICAL_TO_K0(fbPtr),
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
            /*lrx*/ (0 + width) << 2,
            /*lry*/ ((yPos + chunkSize) - 1) << 2,
            /*tile*/G_TX_RENDERTILE,
            /*s*/   0,
            /*t*/   0,
            /*dsdx*/1 << 12,
            /*dtdy*/1 << 10);

        gDLBuilder->needsPipeSync = 1;
        
        fbPtr += chunkSize * width;
        yPos += chunkSize;
    }
}

// official name: rcpTileWriteX ?
void rcp_tile_write_x(Gfx** gdl, TextureTile* tiles, f32 x, f32 y, f32 width, f32 height, s32 s, s32 t, f32 scaleX, f32 scaleY, u32 color, s32 flags) {
    s32 _pad;
    s32 resolution;
    Texture* tile;
    Gfx* temp_s0;
    s32 viWidth; // cpCC
    s32 viHeight; // spC8
    s32 ulx;
    s32 uly;
    s32 dsdy;
    s32 lrx;
    s32 lry;
    s32 dsdx;
    s32 j;
    Gfx* dl;

    dl = *gdl;
    resolution = vi_get_current_size();
    viHeight = ((u16)GET_VIDEO_HEIGHT(resolution)) * 4;
    viWidth = GET_VIDEO_WIDTH(resolution) * 4;
    temp_s0 = (flags & TILE_WRITE_TRANSLUCENT)
        ? &D_80091868[(flags & 0xFF) * 3]
        : &D_80091828[(flags & 0xFF) * 3];
    gSPGeometryMode(dl, 0xFFFFFF, G_SHADING_SMOOTH | G_SHADE);
    dl_apply_geometry_mode(&dl);
    bcopy(temp_s0, dl, sizeof(Gfx));
    dl_apply_combine(&dl);
    bcopy(temp_s0 + 1, dl, sizeof(Gfx));
    dl_apply_other_mode(&dl);
    dl_set_prim_color(&dl, (color >> 0x18) & 0xFF, (color >> 0x10) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
    scaleX *= 4.0f;
    scaleY *= 4.0f;
    for (j = 0, tile = tiles[j].tex; tile != NULL; j++, tile = tiles[j].tex) {
        dsdy = (s32) (x * 4.0f); // @fake: reusing dsdx/dsdy vars here
        dsdx = (s32) (y * 4.0f);
        ulx = (s32) (tiles[j].x * scaleX) + dsdy;
        uly = (s32) (tiles[j].y * scaleY) + dsdx;
        if ((ulx < viWidth) && (uly < viHeight)) {
            lrx = (s32) (width * scaleX) + ulx;
            lry = (s32) (height * scaleY) + uly;
            if ((lrx > 0) && (lry > 0) && (ulx < lrx) && (uly < lry)) {
                dsdx = (((s32) width - 1) << 12) / (lrx - ulx);
                if (flags & TILE_WRITE_FLIP_X) {
                    s += ((s32) width - 1) << 5;
                    dsdx = -dsdx;
                }
                dsdy = (((s32) height - 1) << 12) / (lry - uly);
                if (flags & TILE_WRITE_FLIP_Y) {
                    t = ((s32) height - 1) << 5;
                    dsdy = -dsdy;
                }
                if (ulx < 0) {
                    s += (-ulx * dsdx) >> 7;
                    ulx = 0;
                }
                if (uly < 0) {
                    t += (-uly * dsdy) >> 7;
                    uly = 0;
                }
                temp_s0 = tile->gdl;
                dl->words.w0 = temp_s0->words.w0;
                dl->words.w1 = (unsigned int) OS_PHYSICAL_TO_K0(tex_get_frame_img(tile, tiles[j].animProgress));
                dl++;
                temp_s0++;
                gSPDisplayList(dl++, OS_PHYSICAL_TO_K0(temp_s0));
                gSPTextureRectangle(dl++, 
                /*ulx*/ ulx,
                /*uly*/ uly,
                /*lrx*/ lrx,
                /*lry*/ lry,
                /*tile*/ 0,
                /*s*/ s,
                /*t*/ t,
                /*dsdx*/ dsdx, 
                /*dsdy*/ dsdy
                );
                gDLBuilder->needsPipeSync = 1;
            }
        }
    }
    tex_render_reset();
    *gdl = dl;
}

void func_80039560(s32 a0, s32 a1, s32 a2, s32 a3) {

}

void func_80039578(s32 a0, s32 a1, s32 a2, s32 a3) {

}
