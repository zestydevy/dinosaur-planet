#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object_id.h"
#include "sys/gfx/gx.h"
#include "sys/camera.h"
#include "sys/map.h"
#include "sys/memory.h"
#include "sys/rsp_segment.h"
#include "sys/objprint.h"
#include "sys/shadowtex.h"
#include "sys/segment_535C0.h"
#include "functions.h"
#include "macros.h"

static const char str_8009ac70[] = "maketex: buffer out of range\n";
static const char str_8009ac90[] = "maketex: buffer out of range\n";

// -------- .bss start 800bb540 -------- //
void* gShadowTexFramebuffers[SHADOWTEX_NUM_SLOTS * 2]; // shadow framebuffers
void* gShadowTexCurrFb[SHADOWTEX_NUM_SLOTS]; // current framebuffer for each slot
void* D_800BB558[SHADOWTEX_NUM_SLOTS];
void* D_800BB560[SHADOWTEX_NUM_SLOTS * 2]; // finalized shadow textures, 16-bit RGBA
void* D_800BB570[SHADOWTEX_NUM_SLOTS];
void* D_800BB578[SHADOWTEX_NUM_SLOTS * 2]; // ? texture data, 4-bit I (intensity)
// -------- .bss end 800bb590 -------- //

// -------- .data start 80092E90 -------- //
u32 gShadowTexCurrFbIdx[SHADOWTEX_NUM_SLOTS] = {0}; // active buffer index (front/back) for each shadow texture
s32 gShadowTexWidth = 64;
s32 gShadowTexHeight = 64;
s32 gIsShadowTexActive = FALSE; // whether a shadow framebuffer is currently being drawn to
s32 D_80092EA4 = 0; // unused
s32 D_80092EA8 = 0; // unused
// function statics start here
// -------- .data start 80092EC0 -------- //

void shadowtex_swap_buffer(s32 slot);

void shadowtex_init(void) {
    gShadowTexFramebuffers[0] = mmAlloc((gShadowTexWidth * gShadowTexHeight << 1) + 0x30, ALLOC_TAG_SHAD_COL, 0);
    gShadowTexFramebuffers[0] = (void*)((u32)(gShadowTexFramebuffers[0]) + 0x3f & ~0x3f); //Makes sure framebuffer addresses are 64-byte-aligned
    gShadowTexFramebuffers[1] = mmAlloc((gShadowTexWidth * gShadowTexHeight << 1) + 0x30, ALLOC_TAG_SHAD_COL, 0);
    gShadowTexFramebuffers[1] = (void*)((u32)(gShadowTexFramebuffers[1]) + 0x3f & ~0x3f);
    gShadowTexFramebuffers[2] = mmAlloc((gShadowTexWidth * gShadowTexHeight << 1) + 0x30, ALLOC_TAG_SHAD_COL, 0);
    gShadowTexFramebuffers[2] = (void*)((u32)(gShadowTexFramebuffers[2]) + 0x3f & ~0x3f);
    gShadowTexFramebuffers[3] = mmAlloc((gShadowTexWidth * gShadowTexHeight << 1) + 0x30, ALLOC_TAG_SHAD_COL, 0);
    gShadowTexFramebuffers[3] = (void*)((u32)(gShadowTexFramebuffers[3]) + 0x3f & ~0x3f);

    D_800BB560[0] = mmAlloc(gShadowTexWidth * gShadowTexHeight << 1, ALLOC_TAG_SHAD_COL, 0);
    D_800BB560[1] = mmAlloc(gShadowTexWidth * gShadowTexHeight << 1, ALLOC_TAG_SHAD_COL, 0);
    D_800BB560[2] = mmAlloc(gShadowTexWidth * gShadowTexHeight << 1, ALLOC_TAG_SHAD_COL, 0);
    D_800BB560[3] = mmAlloc(gShadowTexWidth * gShadowTexHeight << 1, ALLOC_TAG_SHAD_COL, 0);

    D_800BB578[0] = mmAlloc(gShadowTexWidth * gShadowTexHeight >> 1, ALLOC_TAG_SHAD_COL, 0);
    D_800BB578[1] = mmAlloc(gShadowTexWidth * gShadowTexHeight >> 1, ALLOC_TAG_SHAD_COL, 0);
    D_800BB578[2] = mmAlloc(gShadowTexWidth * gShadowTexHeight >> 1, ALLOC_TAG_SHAD_COL, 0);
    D_800BB578[3] = mmAlloc(gShadowTexWidth * gShadowTexHeight >> 1, ALLOC_TAG_SHAD_COL, 0);

    bzero(D_800BB560[0], gShadowTexWidth * gShadowTexHeight << 1);
    bzero(D_800BB560[1], gShadowTexWidth * gShadowTexHeight << 1);
    bzero(D_800BB560[2], gShadowTexWidth * gShadowTexHeight << 1);
    bzero(D_800BB560[3], gShadowTexWidth * gShadowTexHeight << 1);

    bzero(D_800BB578[0], gShadowTexWidth * gShadowTexHeight >> 1);
    bzero(D_800BB578[1], gShadowTexWidth * gShadowTexHeight >> 1);
    bzero(D_800BB578[2], gShadowTexWidth * gShadowTexHeight >> 1);
    bzero(D_800BB578[3], gShadowTexWidth * gShadowTexHeight >> 1);

    shadowtex_swap_buffer(0);
    shadowtex_swap_buffer(1);
}

void shadowtex_swap_buffer(s32 slot) {
    gShadowTexCurrFbIdx[slot] ^= 1;
    gShadowTexCurrFb[slot] = gShadowTexFramebuffers[SHADOWTEX_NUM_SLOTS*slot + gShadowTexCurrFbIdx[slot]];
}

s32 shadowtex_get_status(s32 *outTexWidth) {
    if (outTexWidth != NULL) {
        *outTexWidth = gShadowTexWidth;
    }

    return gIsShadowTexActive;
}

// post-processing?
void shadowtex_func_8005BC58(Object *obj, s32 arg1, s32 slot) {
    if (arg1 != 0) {
        func_80052DC0(gShadowTexCurrFb[slot], D_800BB558[slot], D_800BB570[slot], slot);
    } else {
        func_800529C0(gShadowTexCurrFb[slot], D_800BB558[slot]);
    }
    obj->shadow->bufferIdx ^= 1;
}

void shadowtex_draw(Object *obj, s16 arg1, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s32 arg6, f32 arg7, s32 slot) {
    Camera* temp_v0_2;
    s32 _pad;
    f32 spC4;
    f32 spC0;
    f32 spBC;
    Camera* temp_s0;
    f32 var_fv0;
    s32 spB0;
    s32 spAC;
    s32 spA8;
    u32 spA4;
    f32 someZ;
    f32 someY;
    f32 someX;
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    Mtx* sp84;
    static s32 D_80092EAC = 0;
    static s32 D_80092EB0 = 0;

    sp84 = *mtxs;
    gIsShadowTexActive = TRUE;
    if (slot >= (s32)ARRAYCOUNT(D_800BB558)) {
        // STUBBED_PRINTF("maketex: buffer out of range\n");
    }
    D_800BB558[slot] = D_800BB560[(obj->shadow->bufferIdx ^ 1)+(slot*SHADOWTEX_NUM_SLOTS)];
    D_800BB570[slot] = D_800BB578[(obj->shadow->bufferIdx ^ 1)+(slot*SHADOWTEX_NUM_SLOTS)];
    set_camera_selector(1);
    temp_s0 = get_camera();
    sp88 = camera_get_fov();
    camera_set_fov(70.0f);
    camera_set_aspect(1.0f);
    spC4 = temp_s0->srt.transl.f[0];
    spC0 = temp_s0->srt.transl.f[1];
    spBC = temp_s0->srt.transl.f[2];
    spB0 = temp_s0->srt.pitch;
    spAC = temp_s0->srt.yaw;
    spA8 = temp_s0->srt.roll;
    if (arg6 != 0) {
        set_camera_selector(0);
        temp_v0_2 = get_camera();
        someX = temp_v0_2->tx - obj->positionMirror.f[0];
        someY = temp_v0_2->ty - obj->positionMirror.f[1];
        someZ = temp_v0_2->tz - obj->positionMirror.f[2];
        var_fv0 = sqrtf(SQ(someX) + SQ(someY) + SQ(someZ));
        if (var_fv0 > 0.0f) {
            someX *= 15.0f / var_fv0;
            someY *= 15.0f / var_fv0;
            someZ *= 15.0f / var_fv0;
        }
        temp_s0->srt.yaw = 0x8000 - arctan2_f(someX, someZ);
        temp_s0->srt.pitch = arctan2_f(someY, sqrtf(SQ(someX) + SQ(someZ)));
        var_fv0 = temp_s0->srt.pitch / 200.0f;
        if (var_fv0 < 35.0f) {
            var_fv0 = 35.0f;
        } else if (var_fv0 > 100.0f) {
            var_fv0 = 100.0f;
        }
        temp_s0->srt.pitch = 0;
        temp_s0->srt.transl.f[0] = someX + obj->srt.transl.f[0];
        temp_s0->srt.transl.f[1] = arg7  + obj->srt.transl.f[1];
        temp_s0->srt.transl.f[2] = someZ + obj->srt.transl.f[2];
        set_camera_selector(1);
        camera_set_ortho_projection_matrix(-19.5f, 19.5f, 0.0f, var_fv0);
    } else {
        temp_s0->srt.pitch = 0;
        someX = obj->shadow->unk14.f[0];
        someY = obj->shadow->unk14.f[1];
        someZ = obj->shadow->unk14.f[2];
        sp8C = -obj->shadow->unk14.f[0];
        sp90 = -obj->shadow->unk14.f[1];
        sp94 = -obj->shadow->unk14.f[2];
        D_80092EAC = arctan2_f(-sp8C, sp94);
        sp8C *= sp8C;
        sp94 *= sp94;
        D_80092EB0 = sqrtf(sp8C + sp94);
        D_80092EB0 = arctan2_f(D_80092EB0, sp90) - 0x3FC8;
        temp_s0->srt.pitch = D_80092EB0;
        if (obj->shadow->flags & OBJ_SHADOW_FLAG_200) {
            temp_s0->srt.yaw = 0x10000 - obj->srt.yaw;
        } else {
            temp_s0->srt.yaw = D_80092EAC;
        }
        var_fv0 = sqrtf(SQ(someX) + SQ(someY) + SQ(someZ));
        if (var_fv0 > 0.0f) {
            someX *= 35.0f / var_fv0;
            someY *= 35.0f / var_fv0;
            someZ *= 35.0f / var_fv0;
        }
        if (obj->shadow->flags & OBJ_SHADOW_FLAG_800) {
            someY += 200.0f;
        } else {
            someY += 20.0f;
        }
        temp_s0->object = NULL;
        temp_s0->srt.transl.f[0] = obj->srt.transl.f[0] + someX;
        temp_s0->srt.transl.f[1] = obj->srt.transl.f[1] + someY;
        temp_s0->srt.transl.f[2] = obj->srt.transl.f[2] + someZ;
        if (obj->id == OBJ_DIMSnowHorn1) {
            camera_set_ortho_projection_matrix(-55.0f, 55.0f, -50.0f, 60.0f);
        } else {
            camera_set_ortho_projection_matrix(-26.0f, 26.0f, -29.0f, 23.0f);
        }
    }
    if (obj->parent != NULL) {
        temp_s0->srt.transl.f[0] += gWorldX;
        temp_s0->srt.transl.f[2] += gWorldZ;
    }
    spA4 = func_800013E0();
    if (spA4 != 0) {
        func_800013D0();
    }
    func_800021A0(gdl, &sp84);
    if (spA4 != 0) {
        func_800013BC();
    }
    gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, 0, 0, gShadowTexWidth, gShadowTexHeight);
    rsp_segment(gdl, SEGMENT_FRAMEBUFFER, gShadowTexCurrFb[slot]);
    rsp_segment(gdl, SEGMENT_4, (u8*)gShadowTexCurrFb[slot] - 0x280);
    gDPSetOtherMode(*gdl, 
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_FILL | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_OPA_SURF | G_RM_OPA_SURF2);
    dl_apply_other_mode(gdl);
    gSPClipRatio((*gdl)++, FRUSTRATIO_3);
    if (arg6 != 0) {
        if (gDLBuilder->needsPipeSync != 0) {
            gDLBuilder->needsPipeSync = 0;
            gDPPipeSync((*gdl)++);
        }
        gDPSetColorImage((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, gShadowTexWidth, SEGMENT_ADDR(SEGMENT_FRAMEBUFFER, 0x0));
        dl_set_fill_color(gdl, 0);
    } else {
        if (gDLBuilder->needsPipeSync != 0) {
            gDLBuilder->needsPipeSync = 0;
            gDPPipeSync((*gdl)++);
        }
        gDPSetColorImage((*gdl)++, G_IM_FMT_I, G_IM_SIZ_8b, gShadowTexWidth, SEGMENT_ADDR(SEGMENT_FRAMEBUFFER, 0x0));
        dl_set_fill_color(gdl, -1);
        gSPTexture((*gdl)++, 0, 0, 0, G_TX_RENDERTILE, G_OFF);
    }
    gDPFillRectangle((*gdl)++, 0, 0, gShadowTexWidth - 1, gShadowTexHeight - 1);
    gDLBuilder->needsPipeSync = 1;
    if (arg6 == 0) {
        gDPSetCombineMode(*gdl, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
        dl_apply_combine(gdl);
        gDPSetOtherMode(
            *gdl,
            G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
            G_AC_NONE | G_ZS_PIXEL | G_RM_OPA_SURF | G_RM_OPA_SURF2);
        dl_apply_other_mode(gdl);
        dl_set_prim_color(gdl, 0, 0, 0xFF, 0x7D);
    }
    func_800359D0(obj, gdl, &sp84, vtxs, pols, arg6);
    gSPTexture((*gdl)++, -1, -1, 3, G_TX_RENDERTILE, G_ON);
    gIsShadowTexActive = FALSE;
    temp_s0->srt.transl.f[0] = spC4;
    temp_s0->srt.transl.f[1] = spC0;
    temp_s0->srt.transl.f[2] = spBC;
    temp_s0->srt.pitch = (s16) spB0;
    temp_s0->srt.yaw = (s16) spAC;
    temp_s0->srt.roll = (s16) spA8;
    if (func_80041E08() != 0) {
        set_camera_selector(0);
        camera_set_fov(sp88);
        if (func_80041DBC() != 0) {
            camera_set_aspect(2.25f);
        } else {
            camera_set_aspect(1.66f);
        }
        func_800021A0(gdl, &sp84);
    } else if (func_80041DBC() != 0) {
        set_camera_selector(0);
        camera_set_fov(sp88);
        camera_set_aspect(1.7777778f);
        func_800021A0(gdl, &sp84);
    } else {
        set_camera_selector(0);
        camera_set_fov(sp88);
        camera_set_aspect(gAspectRatio);
        func_800021A0(gdl, &sp84);
    }
    rsp_segment(gdl, SEGMENT_FRAMEBUFFER, gFrontFramebuffer);
    rsp_segment(gdl, SEGMENT_4, gFrontFramebuffer - (0x500 / sizeof(u16)));
    if (gDLBuilder->needsPipeSync != 0) {
        gDLBuilder->needsPipeSync = 0;
        gDPPipeSync((*gdl)++);
    }
    gDPSetColorImage((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, GET_VIDEO_WIDTH(vi_get_current_size()), 
        SEGMENT_ADDR(SEGMENT_FRAMEBUFFER, 0x0));
    shadowtex_func_8005BC58(obj, arg6, slot);
    shadowtex_swap_buffer(slot);
    *mtxs = sp84;
}

void shadowtex_get_textures(s32 bufferIdx, void **arg1, void **arg2, s32 slot) {
    *arg1 = D_800BB560[bufferIdx + (slot * SHADOWTEX_NUM_SLOTS)];
    *arg2 = D_800BB578[bufferIdx + (slot * SHADOWTEX_NUM_SLOTS)];
}
