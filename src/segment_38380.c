#include "PR/gbi.h"
#include "common.h"
#include "functions.h"
#include "sys/map.h"
#include "sys/gfx/map.h"

extern u8 D_800917A0;
extern u8 D_800917A4;
extern u8 D_800917A8;
extern u32 D_800917AC;
extern s32 D_800917BC;
extern s32 D_800917C0;
extern s32 D_800917C8;
extern u8 D_800917CC;
extern Gfx D_80091828[6];
extern Gfx D_80091868[6];

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
void func_8003833C(Gfx** gdl, Texture* tex, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8); 
Texture* func_8003E904(Texture* arg0, s32 arg1);

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

void func_80037A14(Gfx **gdl, Mtx **mtx, s32 param3) {
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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80037F9C.s")
#else
typedef struct Unk {
    Texture *unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
} Unk;

// https://decomp.me/scratch/MUNuy
// Type for arg1 is incorrect, current definition (Func_80037F9C_Struct) is also not fully correct
void func_80037F9C(Gfx** gdl, Unk* arg1, s32 arg2, s32 arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7) {
    Texture* var_v0;
    s32 temp_s5;
    s32 temp_s6;
    s32 temp_t8;
    s32 var_s1;
    s32 var_s2;
    s32 var_s3;
    s32 var_s4;
    s32 i;
    s32 j;
    Gfx* sp68;

    j = 0;
    sp68 = *gdl;
    gSPLoadGeometryMode(sp68, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    dl_apply_geometry_mode(&sp68);
    gDPSetCombineMode(sp68, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    dl_apply_combine(&sp68);
    gDPSetOtherMode(
        sp68, 
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2
    );
    dl_apply_other_mode(&sp68);
    while (arg1[j].unk0 != NULL) {
        var_s1 = (arg1[j].unk8 * 4) + (arg2 * 4);
        var_s2 = (arg1[j].unkA * 4) + (arg3 * 4);
        temp_s5 = (arg1[j].unk0->width * 4) + var_s1;
        temp_s6 = (arg1[j].unk0->height * 4) + var_s2;
        if (temp_s5 > 0 && temp_s6 > 0) {
            var_s3 = 0;
            var_s4 = 0;
            if (var_s1 < 0) {
                var_s3 = -(var_s1 * 8);
                var_s1 = 0;
            }
            if (var_s2 < 0) {
                var_s4 = -(var_s2 * 8);
                var_s2 = 0;
            }
            temp_t8 = (s32) arg1[j].unk4 >> 8;
            for (var_v0 = arg1[j].unk0, i = 0; i < temp_t8 && var_v0 != NULL; i++) {
                var_v0 = var_v0->next;
            }
            sp68->words.w0 = var_v0->gdl->words.w0;
            sp68->words.w1 = OS_PHYSICAL_TO_K0(var_v0 + 1);
            
            sp68++;
            gSPDisplayList(sp68++, OS_PHYSICAL_TO_K0(var_v0->gdl + 1));
            dl_set_prim_color(&sp68, arg4, arg5, arg6, arg7);
            gSPTextureRectangle(sp68++, 
            /* xl */ var_s1,
            /* xl */ var_s2,
            /* xh */ temp_s5,
            /* yh */ temp_s6,
            /* tile */ 0,
            /* s */ var_s3,
            /* t */ var_s4,
            /* dsdx */ 1 << 10,
            /* dsdy */ 1 << 10
            );
            gDLBuilder->needsPipeSync = 1;
        }
        j++;
    }
    func_8003DB5C();
    *gdl = sp68;
}
#endif

void func_8003825C(Gfx** gdl, Texture* tex, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 temp = tex->height | ((tex->unk_0x1b & 0xF) << 8);
    func_8003833C(gdl, tex, arg2, arg3, 0, temp, arg5, arg6, arg7);
}

void func_800382AC(Gfx** gdl, Texture* tex, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 temp_v1;
    s32 var_s0;
    s32 var_v0;

    arg4 -= arg3;
    temp_v1 = tex->height | ((tex->unk_0x1b & 0xF) << 8);
    if (arg4 < 0) {
        arg4 = 0;
    }
    arg5 -= arg3;
    if (temp_v1 < arg5) {
        arg5 = temp_v1;
    }
    if (arg4 < temp_v1 && arg5 >= 0) {
        func_8003833C(gdl, tex, arg2, arg3, arg4, arg5, 0, arg6, arg7);
    }
}

void func_8003833C(Gfx** gdl, Texture* tex, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 width;
    s32 sp148;
    s32 temp_v0_9;
    s32 temp_v1;
    s32 height;
    s32 var_t3;
    s32 texFormat;
    s32 sp130;
    s32 sp12C;
    s32 sp128;
    s32 var_v0;
    Texture* var_s6;
    Texture* sp11C;

    sp12C = func_80041DBC();
    sp128 = func_80041E08();
    
    if (tex->levels != 0) {
        var_v0 = (s32) tex->levels >> 8;
    } else {
        var_v0 = 0;    
    }
    sp11C = tex;
    if ((var_v0 >= 2) && (arg6 < var_v0)) {
        for (var_v0 = 0; var_v0 < arg6 && sp11C != NULL; var_v0++) {
            sp11C = sp11C->next;
        }
    }
    gSPGeometryMode(*gdl, 0xFFFFFF, 0);
    dl_apply_geometry_mode(gdl);
    width = tex->width | ((tex->unk_0x1b & 0xF0) * 16);
    if (sp12C != 0) {
        sp148 = (s32) ((f32)tex->width * 1) | ((tex->unk_0x1b & 0xF0) * 16);
    } else {
        sp148 = width;
    }
    texFormat = TEX_FORMAT(tex->format);
    if (texFormat == TEX_FORMAT_RGBA16) {
        sp130 = 2;
        height = (0x800 / width) & ~1;
    } else if (texFormat == TEX_FORMAT_IA8) {
        sp130 = 1;
        height = (0x1000 / width) & ~1;
    } else {
        sp130 = 4;
        height = (0x400 / width) & ~1;
    }
    if (height == 0) {
        return;
    }

    var_s6 = (Texture*)((u8*)sp11C + (width * arg4 * sp130) + sizeof(Texture));
    if (arg8 & 2) {
        gDPSetCombineMode(*gdl, G_CC_DECALRGBA, G_CC_DECALRGBA);
        dl_apply_combine(gdl);
        gDPSetOtherMode(
            *gdl, 
            G_AD_PATTERN | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_COPY | G_PM_NPRIMITIVE, 
            G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_NOOP2
        );
        dl_apply_other_mode(gdl);
    } else if ((arg7 == 0xFF) && (arg8 & 1)) {
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
    dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, arg7);
    var_t3 = arg4;
    do {
        temp_v0_9 = arg5 - var_t3;
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
            gDPSetTextureImage((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, OS_PHYSICAL_TO_K0(var_s6));
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
                gDPSetTextureImage((*gdl)++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, OS_PHYSICAL_TO_K0(var_s6));
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
            gDPSetTextureImage((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, OS_PHYSICAL_TO_K0(var_s6));
            gDPSetTile((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
            gDPLoadSync((*gdl)++);
            gDPLoadBlock((*gdl)++, G_TX_LOADTILE, 0, 0, width * height - 1, 0);
            gDPPipeSync((*gdl)++);
            gDPSetTile((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_32b, ((width << 1) + 7) >> 3, G_TX_RENDERTILE, 0, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
            gDPSetTileSize((*gdl)++, 0, 0, 0, (width - 1) << 2, ((height - 1) << 2));
        }

        temp_v1 = arg3 + var_t3;
        if (arg8 & 2) {
            gSPTextureRectangle(
            /* pkt */ (*gdl)++,
            /* xl */ arg2 << 2,
            /* yl */ temp_v1 << 2,
            /* xh */ (arg2 + width) << 2,
            /* yh */ (temp_v1 + height - 1) << 2,
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
            /* xl */ arg2 << 2,
            /* yl */ temp_v1 << 2,
            /* xh */ (arg2 + sp148) << 2,
            /* yh */ (temp_v1 + height) << 2,
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
            /* xl */ arg2 << 2,
            /* yl */ temp_v1 << 2,
            /* xh */ (arg2 + sp148) << 2,
            /* yh */ (temp_v1 + height) << 2,
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
            /* xl */ arg2 << 2,
            /* yl */ temp_v1 << 2,
            /* xh */ (arg2 + sp148) << 2,
            /* yh */ (temp_v1 + height) << 2,
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
            /* xl */ arg2 << 2,
            /* yl */ temp_v1 << 2,
            /* xh */ (arg2 + width) << 2,
            /* yh */ (temp_v1 + height) << 2,
            /* tile */ 0,
            /* s */ 0,
            /* t */ 0,
            /* dsdx */ 1 << 10,
            /* dsdy */ 1 << 10
            );
            gDLBuilder->needsPipeSync = 1;
        }
        var_t3 += height;
        var_s6 = (Texture *) ((u8*)var_s6 + width * height * sp130);
    } while (var_t3 < arg5);
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

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_800390A4.s")
#else
// https://decomp.me/scratch/vcSVM
typedef struct Unk2 {
    void *unk0;
    s32 unk4;
    union {
        struct {
            s16 unk8;
            s16 unkA;
        } shorts;
        Gfx *gdl;
    };
} Unk2;
void func_800390A4(Gfx** arg0, Unk2* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7, f32 arg8, f32 arg9, s32 arg10, s32 arg11) {
    Unk2* var_s4;
    s32 temp_a0;
    s32 resolution;
    s32 width; // cpCC
    s32 height; // spC8
    s32 minWidth;
    s32 minHeight;
    s32 var_s3;
    s32 spB8;
    s32 spB4;
    s32 var_s5;
    Gfx* temp_s0;
    Gfx* spA8;
    Unk2* var_s7;

    spA8 = *arg0;
    resolution = get_some_resolution_encoded();
    height = ((u16)RESOLUTION_HEIGHT(resolution)) * 4;
    width = RESOLUTION_WIDTH(resolution) * 4;
    if (arg11 & 0x4000) {
        temp_s0 = &D_80091868[(arg11 & 0xFF) * 3];
    } else {
        temp_s0 = &D_80091828[(arg11 & 0xFF) * 3];
    }
    gSPGeometryMode(spA8, 0xFFFFFF, G_SHADING_SMOOTH | G_SHADE);
    dl_apply_geometry_mode(&spA8);
    bcopy(temp_s0, spA8, 8);
    dl_apply_combine(&spA8);
    bcopy(temp_s0 + 1, spA8, 8);
    dl_apply_other_mode(&spA8);
    dl_set_prim_color(&spA8, ((u32) arg10 >> 0x18) & 0xFF, ((u32) arg10 >> 0x10) & 0xFF, ((u32) arg10 >> 8) & 0xFF, (u8) (arg10 & 0xFF));
    var_s7 = arg1->unk0;
    arg8 *= 4.0f;
    arg9 *= 4.0f;
    if (var_s7 != NULL) {
        var_s4 = arg1;
        do {
            minWidth = (s32) ((f32) var_s4->shorts.unk8 * arg8) + (s32) (arg2 * 4.0f);
            minHeight = (s32) ((f32) var_s4->shorts.unkA * arg9) + (s32) (arg3 * 4.0f);
            if ((minWidth < width) && (minHeight < height)) {
                spB8 = (s32) (arg4 * arg8) + minWidth;
                spB4 = (s32) (arg5 * arg9) + minHeight;
                if ((spB8 > 0) && (spB4 > 0) && (minWidth < spB8) && (minHeight < spB4)) {
                    temp_a0 = (s32) arg4 - 1;
                    var_s5 = (temp_a0 << 12);
                    var_s5 /= (spB8 - minWidth);
                    if (arg11 & 0x1000) {
                        arg6 += temp_a0 << 5;
                        var_s5 = -var_s5;
                    }
                    temp_a0 = (s32) arg5 - 1;
                    var_s3 = temp_a0 << 12;
                    var_s3 /= spB4 - minHeight;
                    if (arg11 & 0x2000) {
                        arg7 = temp_a0 << 5;
                        var_s3 = -var_s3;
                    }
                    if (minWidth < 0) {
                        arg6 += (-minWidth * var_s5) >> 7;
                        minWidth = 0;
                    }
                    if (minHeight < 0) {
                        arg7 += (-minHeight * var_s3) >> 7;
                        minHeight = 0;
                    }
                    temp_s0 = (Gfx *)var_s7->gdl;
                    spA8->words.w0 = temp_s0->words.w0;
                    spA8->words.w1 = OS_PHYSICAL_TO_K0(func_8003E904((Texture* ) var_s7, var_s4->unk4));
                    spA8++;
                    temp_s0++;
                    gSPDisplayList(spA8++, OS_PHYSICAL_TO_K0(temp_s0));
                    gSPTextureRectangle(spA8++, 
                    /* xl */ minWidth,
                    /* xl */ minHeight,
                    /* xh */ spB8,
                    /* yh */ spB4,
                    /* tile */ 0,
                    /* s */ arg6,
                    /* t */ arg7,
                    /* dsdx */ var_s5, 
                    /* dsdy */ var_s3
                    );
                    gDLBuilder->needsPipeSync = 1;
                }
            }
            var_s7 = var_s4[1].unk0;
            var_s4 += 1;
        } while (var_s7 != NULL);
    }
    func_8003DB5C();
    *arg0 = spA8;
}
#endif

void func_80039560(s32 a0, s32 a1, s32 a2, s32 a3) {

}

void func_80039578(s32 a0, s32 a1, s32 a2, s32 a3) {

}
