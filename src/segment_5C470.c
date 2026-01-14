#include "common.h"
#include "sys/rsp_segment.h"
#include "sys/objprint.h"

static const char str_8009ac70[] = "maketex: buffer out of range\n";
static const char str_8009ac90[] = "maketex: buffer out of range\n";

// -------- .bss start 800bb540 -------- //
u32* D_800BB540[4];
u32* D_800BB550[2];
u32* D_800BB558[2];
u32* D_800BB560[4];
u32* D_800BB570[2];
u32* D_800BB578[4];
// -------- .bss end 800bb590 -------- //

// -------- .data start 80092E90 -------- //
/** 
 * D_80092E9C could be one of the dimensions (height) for rendering dynamic shadow texture?
 * gPossiblyScreenWidthOrHeight might be the width!
 */
u32 D_80092E90[2] = {0, 0};
s32 gPossiblyScreenWidthOrHeight = 64;
s32 D_80092E9C = 64; // Usually 0x40 (64)
s32 gSomeVideoFlag = 0;
s32 D_80092EA4 = 0; // unused
s32 D_80092EA8 = 0; // unused
// -------- .data start 80092EC0 -------- //

void func_8005B870(void) {
    D_800BB540[0] = mmAlloc((gPossiblyScreenWidthOrHeight * D_80092E9C << 1) + 0x30, ALLOC_TAG_SHAD_COL, 0);
    D_800BB540[0] = (u32*)((u32)(D_800BB540[0]) + 0x3f & ~0x3f); //Makes sure framebuffer addresses are 64-byte-aligned
    D_800BB540[1] = mmAlloc((gPossiblyScreenWidthOrHeight * D_80092E9C << 1) + 0x30, ALLOC_TAG_SHAD_COL, 0);
    D_800BB540[1] = (u32*)((u32)(D_800BB540[1]) + 0x3f & ~0x3f);
    D_800BB540[2] = mmAlloc((gPossiblyScreenWidthOrHeight * D_80092E9C << 1) + 0x30, ALLOC_TAG_SHAD_COL, 0);
    D_800BB540[2] = (u32*)((u32)(D_800BB540[2]) + 0x3f & ~0x3f);
    D_800BB540[3] = mmAlloc((gPossiblyScreenWidthOrHeight * D_80092E9C << 1) + 0x30, ALLOC_TAG_SHAD_COL, 0);
    D_800BB540[3] = (u32*)((u32)(D_800BB540[3]) + 0x3f & ~0x3f);

    D_800BB560[0] = mmAlloc(gPossiblyScreenWidthOrHeight * D_80092E9C << 1, ALLOC_TAG_SHAD_COL, 0);
    D_800BB560[1] = mmAlloc(gPossiblyScreenWidthOrHeight * D_80092E9C << 1, ALLOC_TAG_SHAD_COL, 0);
    D_800BB560[2] = mmAlloc(gPossiblyScreenWidthOrHeight * D_80092E9C << 1, ALLOC_TAG_SHAD_COL, 0);
    D_800BB560[3] = mmAlloc(gPossiblyScreenWidthOrHeight * D_80092E9C << 1, ALLOC_TAG_SHAD_COL, 0);

    D_800BB578[0] = mmAlloc(gPossiblyScreenWidthOrHeight * D_80092E9C >> 1, ALLOC_TAG_SHAD_COL, 0);
    D_800BB578[1] = mmAlloc(gPossiblyScreenWidthOrHeight * D_80092E9C >> 1, ALLOC_TAG_SHAD_COL, 0);
    D_800BB578[2] = mmAlloc(gPossiblyScreenWidthOrHeight * D_80092E9C >> 1, ALLOC_TAG_SHAD_COL, 0);
    D_800BB578[3] = mmAlloc(gPossiblyScreenWidthOrHeight * D_80092E9C >> 1, ALLOC_TAG_SHAD_COL, 0);

    bzero(D_800BB560[0], gPossiblyScreenWidthOrHeight * D_80092E9C << 1);
    bzero(D_800BB560[1], gPossiblyScreenWidthOrHeight * D_80092E9C << 1);
    bzero(D_800BB560[2], gPossiblyScreenWidthOrHeight * D_80092E9C << 1);
    bzero(D_800BB560[3], gPossiblyScreenWidthOrHeight * D_80092E9C << 1);

    bzero(D_800BB578[0], gPossiblyScreenWidthOrHeight * D_80092E9C >> 1);
    bzero(D_800BB578[1], gPossiblyScreenWidthOrHeight * D_80092E9C >> 1);
    bzero(D_800BB578[2], gPossiblyScreenWidthOrHeight * D_80092E9C >> 1);
    bzero(D_800BB578[3], gPossiblyScreenWidthOrHeight * D_80092E9C >> 1);

    func_8005BBF0(0);
    func_8005BBF0(1);
}

void func_8005BBF0(u32 arg0) {
    D_80092E90[arg0] ^= 1;
    D_800BB550[arg0] = D_800BB540[2*arg0 + D_80092E90[arg0]];
}

/**
 * If param1 isn't null, sets it to gPossiblyScreenWidthOrHeight.
 *
 * Returns gSomeVideoFlag.
 */
int func_8005BC38(s32 *param1) {
    if (param1 != NULL) {
        *param1 = gPossiblyScreenWidthOrHeight;
    }

    return gSomeVideoFlag;
}

/*
 *   arg0 is an Object instance: Sabre/Krystal for example, because they create dynamic shadows
 *   arg1 is usually 0, but setting it to 1 affects the shadow rendering
 *   arg2 usually 0
 */
void func_8005BC58(Object* arg0, s32 arg1, s32 arg2) {
    if (arg1 != 0) {
        // TODO: casts
        func_80052DC0(D_800BB550[arg2], (u32)D_800BB558[arg2], (u32)D_800BB570[arg2], arg2);
    } else {
        func_800529C0(D_800BB550[arg2], (u32)D_800BB558[arg2]);
    }
    arg0->unk64->unk41 ^= 1;
}

void func_8005BCE0(Object* arg0, s16 arg1, Gfx** gdl, Mtx** mtxs, s32 arg4, s32 arg5, s32 arg6, f32 arg7, s32 arg8) {
    Camera* temp_v0_2;
    s32 temp_v1;
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
    gSomeVideoFlag = 1;
    // @fake
    if (arg8) {}
    D_800BB558[arg8] = D_800BB560[(arg0->unk64->unk41 ^ 1)+(arg8*2)];
    D_800BB570[arg8] = D_800BB578[(arg0->unk64->unk41 ^ 1)+(arg8*2)];
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
        someX = temp_v0_2->tx - arg0->positionMirror.f[0];
        someY = temp_v0_2->ty - arg0->positionMirror.f[1];
        someZ = temp_v0_2->tz - arg0->positionMirror.f[2];
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
        temp_s0->srt.transl.f[0] = someX + arg0->srt.transl.f[0];
        temp_s0->srt.transl.f[1] = arg7  + arg0->srt.transl.f[1];
        temp_s0->srt.transl.f[2] = someZ + arg0->srt.transl.f[2];
        set_camera_selector(1);
        camera_set_ortho_projection_matrix(-19.5f, 19.5f, 0.0f, var_fv0);
    } else {
        temp_s0->srt.pitch = 0;
        someX = arg0->unk64->unk14.f[0];
        someY = arg0->unk64->unk14.f[1];
        someZ = arg0->unk64->unk14.f[2];
        sp8C = -arg0->unk64->unk14.f[0];
        sp90 = -arg0->unk64->unk14.f[1];
        sp94 = -arg0->unk64->unk14.f[2];
        D_80092EAC = arctan2_f(-sp8C, sp94);
        sp8C *= sp8C;
        sp94 *= sp94;
        D_80092EB0 = sqrtf(sp8C + sp94);
        D_80092EB0 = arctan2_f(D_80092EB0, sp90) - 0x3FC8;
        temp_s0->srt.pitch = D_80092EB0;
        if (arg0->unk64->flags & 0x200) {
            temp_s0->srt.yaw = 0x10000 - arg0->srt.yaw;
        } else {
            temp_s0->srt.yaw = D_80092EAC;
        }
        var_fv0 = sqrtf(SQ(someX) + SQ(someY) + SQ(someZ));
        if (var_fv0 > 0.0f) {
            someX *= 35.0f / var_fv0;
            someY *= 35.0f / var_fv0;
            someZ *= 35.0f / var_fv0;
        }
        if (arg0->unk64->flags & 0x800) {
            someY += 200.0f;
        } else {
            someY += 20.0f;
        }
        temp_s0->object = NULL;
        temp_s0->srt.transl.f[0] = arg0->srt.transl.f[0] + someX;
        temp_s0->srt.transl.f[1] = arg0->srt.transl.f[1] + someY;
        temp_s0->srt.transl.f[2] = arg0->srt.transl.f[2] + someZ;
        if (arg0->id == OBJ_DIMSnowHorn1) {
            camera_set_ortho_projection_matrix(-55.0f, 55.0f, -50.0f, 60.0f);
        } else {
            camera_set_ortho_projection_matrix(-26.0f, 26.0f, -29.0f, 23.0f);
        }
    }
    if (arg0->parent != NULL) {
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
    gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, 0, 0, gPossiblyScreenWidthOrHeight, D_80092E9C);
    rsp_segment(gdl, 1U, D_800BB550[arg8]);
    rsp_segment(gdl, 4U, D_800BB550[arg8] - (0x280 / 4));
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
        gDPSetColorImage((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, gPossiblyScreenWidthOrHeight, 0x01000000);
        dl_set_fill_color(gdl, 0U);
    } else {
        if (gDLBuilder->needsPipeSync != 0) {
            gDLBuilder->needsPipeSync = 0;
            gDPPipeSync((*gdl)++);
        }
        gDPSetColorImage((*gdl)++, G_IM_FMT_I, G_IM_SIZ_8b, gPossiblyScreenWidthOrHeight, 0x01000000);
        dl_set_fill_color(gdl, -1U);
        gSPTexture((*gdl)++, 0, 0, 0, G_TX_RENDERTILE, G_OFF);
    }
    gDPFillRectangle((*gdl)++, 0, 0, gPossiblyScreenWidthOrHeight - 1, D_80092E9C - 1);
    gDLBuilder->needsPipeSync = 1;
    if (arg6 == 0) {
        gDPSetCombineMode(*gdl, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
        dl_apply_combine(gdl);
        gDPSetOtherMode(
            *gdl,
            G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
            G_AC_NONE | G_ZS_PIXEL | G_RM_OPA_SURF | G_RM_OPA_SURF2);
        dl_apply_other_mode(gdl);
        dl_set_prim_color(gdl, 0U, 0U, 0xFFU, 0x7DU);
    }
    func_800359D0(arg0, gdl, &sp84, (u32) arg4, (u32) arg5, (u32) arg6);
    gSPTexture((*gdl)++, -1, -1, 3, G_TX_RENDERTILE, G_ON);
    gSomeVideoFlag = 0;
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
    rsp_segment(gdl, 1U, gFramebufferCurrent);
    rsp_segment(gdl, 4U, gFramebufferCurrent - (0x500 / sizeof(u16)));
    if (gDLBuilder->needsPipeSync != 0) {
        gDLBuilder->needsPipeSync = 0;
        gDPPipeSync((*gdl)++);
    }
    gDPSetColorImage((*gdl)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, GET_VIDEO_WIDTH(vi_get_current_size()), 0x01000000);
    func_8005BC58(arg0, arg6, arg8);
    func_8005BBF0((u32) arg8);
    *mtxs = sp84;
}

/*
 * May have something to do with the dynamic shadows,
 * since editing this function to immediately return will affect the appearance of Krystal/Sabre's appearance!
 * 
 * called once per frame
 * arg0 alternates between 0 and 1 on different frames
 * arg1 and arg2 are pointers
 * arg3 = 0
 */
void func_8005C740(s32 arg0, u32* arg1, u32* arg2, u32 arg3) {
    *arg1 = (u32)D_800BB560[arg0 + (arg3 * 2)];
    *arg2 = (u32)D_800BB578[arg0 + (arg3 * 2)];
}