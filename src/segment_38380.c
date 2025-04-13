#include "PR/gbi.h"
#include "common.h"
#include "functions.h"
#include "sys/gfx/map.h"

extern u8 D_800917A0;
extern u8 D_800917A4;
extern u8 D_800917A8;
extern u32 D_800917AC;
extern s32 D_800917BC;
extern s32 D_800917C0;
extern s32 D_800917C8;
extern u8 D_800917CC;

extern u8 gGfxDramStack[1024];
extern OSMesgQueue D_800B3B90;
extern OSMesg D_800B3BA8[1];
extern OSMesgQueue D_800B3BB0;
extern OSMesgQueue D_800B3BC8;
extern OSMesg D_800B3BE0[8];
extern OSMesg D_800B3C00[8];
extern OSScTask gGfxTasks[2];
extern OSMesgQueue *sched_intQ_ptr;

extern u8 gGfxYieldData[OS_YIELD_DATA_SIZE];

extern Gfx D_800917D0[];

s32 schedule_gfx_task(Gfx *dlStart, Gfx *dlEnd, s32 param3) {
    OSScTask *task;
    
    D_800917C8 = 1;

    task = &gGfxTasks[D_800917C0];

    D_800917C0++;
    if (D_800917C0 == ARRAYCOUNT(gGfxTasks)) {
        D_800917C0 = 0;
    }

    task->flags = OS_SC_NEEDS_RDP | OS_SC_NEEDS_RSP | OS_SC_LAST_TASK;
    task->msgQ = &D_800B3BC8;
    task->unk0x58 = 0xff0000ff;
    task->unk0x5c = 0xff0000ff;
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
    task->list.t.ucode_data_size = 0x800;
    task->list.t.dram_stack = (u64*)gGfxDramStack;
    task->list.t.dram_stack_size = sizeof(gGfxDramStack);
    if (D_800917CC != 0) {
        task->list.t.output_buff = NULL;
        task->list.t.output_buff_size = NULL;
    }
    task->list.t.yield_data_ptr = (u64*)gGfxYieldData;
    task->list.t.yield_data_size = sizeof(gGfxYieldData);
    task->next = NULL;
    task->framebuffer = (void*)gFramebufferNext;
    task->unk0x60 = 0xff;
    task->unk0x64 = 0xff;

    osWritebackDCacheAll();

    osSendMesg(sched_intQ_ptr, (OSMesg)task, OS_MESG_BLOCK);

    return 0;
}

void func_800378E8(s32 a0, s32 a1, s32 a2) {

}

void func_800378FC(s32 a0, s32 a1, s32 a2) {

}

void func_80037910(s32 a0, s32 a1, s32 a2) {

}

s32 func_80037924() {
    UnkSchedStruct *mesg;

    mesg = 0;

    if (D_800917C8 == 0) {
        return 0;
    }

    osRecvMesg(&D_800B3BC8, (OSMesg)&mesg, OS_MESG_BLOCK);
    D_800917C8 = 0;

    return mesg->unk4;
}

void func_80037978(s32 a0, s32 a1, s32 a2) {

}

void func_8003798C(u8 param1, u8 param2, u8 param3) {
    D_800917A0 = param1;
    D_800917A4 = param2;
    D_800917A8 = param3;
}

void func_800379D0(u32 red, u32 green, s32 blue) {
    D_800917AC = GPACK_RGBA5551(red, green, blue, 1);
    D_800917AC |= (D_800917AC << 16);
}

void func_80037A14(Gfx **gdl, UNK_PTR **mtx, s32 param3) {
    s32 resolution;
    s32 resWidth, resHeight;
    s32 ulx, uly, lrx, lry;
    s32 var1;

    func_80002130(&ulx, &uly, &lrx, &lry);

    var1 = func_80004A4C();

    resolution = get_some_resolution_encoded();
    resWidth = RESOLUTION_WIDTH(resolution);
    resHeight = RESOLUTION_HEIGHT(resolution);

    gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, 0, 0, resWidth - 1, resHeight - 1);

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

    gDPSetColorImage((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, resWidth, 0x02000000);

    if ((param3 & 2) != 0) {
        dl_set_fill_color(gdl, (GPACK_ZDZ(G_MAXFBZ, 0) << 16) | GPACK_ZDZ(G_MAXFBZ, 0));

        gDPFillRectangle((*gdl)++, ulx, uly, lrx, lry);

        gDLBuilder->needsPipeSync = TRUE;
    }

    if (gDLBuilder->needsPipeSync) {
        gDLBuilder->needsPipeSync = FALSE;
        gDPPipeSync((*gdl)++);
    }

    gDPSetColorImage((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, resWidth, 0x01000000);

    if ((param3 & 1) != 0 || var1 != 0) {
        dl_set_fill_color(gdl, 
            (GPACK_RGBA5551(D_800917A0, D_800917A4, D_800917A8, 1) << 16) 
                | GPACK_RGBA5551(D_800917A0, D_800917A4, D_800917A8, 1));

        if ((param3 & 1) != 0) {
            gDPFillRectangle((*gdl)++, 0, 0, resWidth - 1, resHeight - 1);
            gDLBuilder->needsPipeSync = TRUE;
        } else if (var1 != 0) {
            gDPFillRectangle((*gdl)++, 0, 0, resWidth - 1, uly - 1);
            gDLBuilder->needsPipeSync = TRUE;

            gDPFillRectangle((*gdl)++, 0, lry, resWidth - 1, resHeight - 1);
            gDLBuilder->needsPipeSync = TRUE;
        }
    }

    func_80002490(gdl);
}

void func_80037DB8(Gfx **gdl) {
    s32 resolution;
    s32 resWidth;

    resolution = get_some_resolution_encoded();
    resWidth = RESOLUTION_WIDTH(resolution);

    if (gDLBuilder->needsPipeSync) {
        gDLBuilder->needsPipeSync = FALSE;
        gDPPipeSync((*gdl)++);
    }

    gDPSetColorImage((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, resWidth, 0x01000000);

    if (gDLBuilder->needsPipeSync) {
        gDLBuilder->needsPipeSync = FALSE;
        gDPPipeSync((*gdl)++);
    }

    gDPSetDepthImage((*gdl)++, 0x02000000);

    gDPSetCombineMode((*gdl), G_CC_DECALRGB, G_CC_DECALRGB);
    dl_apply_combine(gdl);

    gDPSetOtherMode((*gdl), 
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_1PRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_OPA_SURF | G_RM_OPA_SURF2);
    dl_apply_other_mode(gdl);
}

void func_80037EC8(Gfx **gdl) {
    // Clear all
    gSPClearGeometryMode((*gdl), 0xFFFFFF);
    dl_apply_geometry_mode(gdl);

    gSPDisplayList((*gdl)++, OS_K0_TO_PHYSICAL(D_800917D0));
}

void create_3_megs_quues(OSSched *sched) {
    sched_intQ_ptr = get_sched_interrupt_queue(sched);

    osCreateMesgQueue(&D_800B3B90, D_800B3BA8, ARRAYCOUNT(D_800B3BA8));
    osCreateMesgQueue(&D_800B3BB0, D_800B3BE0, ARRAYCOUNT(D_800B3BE0));
    osCreateMesgQueue(&D_800B3BC8, D_800B3C00, ARRAYCOUNT(D_800B3C00));
}

void func_80037F8C(s32 param1) {
    D_800917BC = param1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80037F9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_8003825C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_800382AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_8003833C.s")

void draw_pause_screen_freeze_frame(Gfx** gdl) {
    u16* fbPtr;
    s32 remainingY;
    s32 yPos;
    s32 height;
    s32 chunkSize;
    s32 width;

    width = 320;
    height = 240;
    
    fbPtr = get_framebuffer_end();
    
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

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_800390A4.s")

void func_80039560(s32 a0, s32 a1, s32 a2, s32 a3) {

}

void func_80039578(s32 a0, s32 a1, s32 a2, s32 a3) {

}
