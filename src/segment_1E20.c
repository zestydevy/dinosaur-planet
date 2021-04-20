#include "common.h"

#define CAMERA_COUNT 12

typedef struct
{
/*0000*/    SRT srt;
/*0018*/    u8 unk_0x18[0x2c - 0x18];
/*002C*/    f32 dty;
/*0030*/    f32 unk_0x30;
/*0034*/    f32 unk_0x34;
/*0038*/    f32 unk_0x38;
/*003C*/    f32 unk_0x3c;
/*0040*/    TActor *actor;
/*0044*/    f32 tx;
/*0048*/    f32 ty;
/*004C*/    f32 tz;
/*0050*/    s16 yaw;
/*0052*/    s16 pitch;
/*0054*/    s16 roll;
/*0056*/    u16 unk_0x56;
/*0058*/    u16 unk_0x58;
/*005A*/    s16 dpitch;
/*005C*/    s8 unk_0x5c;
/*005D*/    s8 unk_0x5d;
} Camera;

typedef union
{
    Mtx m;
    MtxF mf;
} Mtx_MtxF;

typedef struct
{
/*0000*/    Mtx_MtxF *mtx;
/*0004*/    s16 count;
} MatrixSlot;

typedef struct
{
/*0000*/    s32 unk_0x0[8];
/*0020*/    s32 ulx;
/*0024*/    s32 uly;
/*0028*/    s32 lrx;
/*002C*/    s32 lry;
/*0030*/    u32 flags;
} Viewport;

extern SRT gCameraSRT;
extern MtxF gViewMtx;
extern MtxF gViewProjMtx;
extern Mtx *gRSPMtxList;
extern f32 gWorldX;
extern f32 gWorldZ;
extern Mtx *gRSPMatrices[2];
extern MtxF gViewMtx2;
extern Mtx gRSPViewMtx2;
extern Viewport gViewports[2]; // FIXME: how many?
extern Vp gRSPViewports[2]; // FIXME: how many?
extern u32 gCameraGroupSelector;
extern MatrixSlot gMatrixPool[2]; // FIXME: how many?
extern u32 gMatrixCount;
extern Camera gCameras[CAMERA_COUNT];
extern s8 gUseAlternateCamera;
extern u32 gCameraSelector;
extern MtxF gActorMatrices[2]; // FIXME: how many items are there?
extern MtxF gInverseActorMatrices[2]; // FIXME: how many items are there?
extern f32 FLOAT_8008c52c;
extern f32 gFovY;
extern f32 gAspect;
extern f32 gNearPlane;
extern f32 gFarPlane;
extern MtxF gProjectionMtx;
extern Mtx gRSPProjectionMtx;
extern u16 gPerspNorm;

void vec3_transform_no_translate(MtxF *mf, Vec3f *v, Vec3f *ov);
void matrix_from_srt_reversed(MtxF *mf, SRT *srt);
void matrix_perspective(MtxF *mf, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001220.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800013BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800013D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/MusPtrBankGetCurrent.s")

f32 get_fov_y()
{
    return gFovY;
}

void set_fov_y(f32 fovY)
{
    if (fovY > 90.0f) {
        fovY = 90.0f;
    } else if (fovY < 40.0f) {
        fovY = 40.0f;
    }

    gFovY = fovY;

    matrix_perspective(&gProjectionMtx, &gPerspNorm, gFovY, gAspect, gNearPlane, gFarPlane, 1.0f);
    matrix_f2l(&gProjectionMtx, &gRSPProjectionMtx);
    FLOAT_8008c52c = gProjectionMtx.m[0][0];
}

void set_ortho_projection_matrix(f32 l, f32 r, f32 b, f32 t)
{
    guOrthoF(&gProjectionMtx, l, r, b, t, 0.0f, 400.0f, 20.0f);
}

void set_aspect(f32 aspect)
{
    gAspect = aspect;
    matrix_perspective(&gProjectionMtx, &gPerspNorm, gFovY, gAspect, gNearPlane, gFarPlane, 1.0f);
    matrix_f2l(&gProjectionMtx, &gRSPProjectionMtx);
    FLOAT_8008c52c = gProjectionMtx.m[0][0];
    gProjectionMtx.m[0][3] *= 0.5f;
}

void set_near_plane(f32 near)
{
    gNearPlane = near;
    matrix_perspective(&gProjectionMtx, &gPerspNorm, gFovY, gAspect, gNearPlane, gFarPlane, 1.0f);
    matrix_f2l(&gProjectionMtx, &gRSPProjectionMtx);
    FLOAT_8008c52c = gProjectionMtx.m[0][0];
    gProjectionMtx.m[0][3] *= 0.5f;
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001660.s")

f32 get_far_plane()
{
    return gFarPlane;
}

void func_800016B8()
{
    set_near_plane(4.0f);
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800016E0.s")

void func_80001708()
{
    matrix_perspective(&gProjectionMtx, &gPerspNorm, 60.0f, 4.0f / 3.0f, gNearPlane, gFarPlane, 1.0f);
    matrix_f2l(&gProjectionMtx, &gRSPProjectionMtx);
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/__CallBackDmaNew.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001788.s")

u32 get_camera_selector()
{
    return gCameraSelector;
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800017A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001884.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001914.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800019BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001A2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001A3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001A5C.s")

void set_camera_selector(s32 selector)
{
    if (selector >= 0 && selector < 4) {
        gCameraSelector = selector;
    } else {
        gCameraSelector = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001B40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001CE0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001D58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001DD4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001E04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001F70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_8000206C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800020E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80002130.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800021A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80002490.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/setup_rsp_camera_matrices.s")
#else
void func_80046B58(f32 x, f32 y, f32 z);
void _setup_rsp_camera_matrices(Gfx **gdl, Mtx **rspMtxs)
{
    s32 cameraSel;
    f32 x;
    f32 y;
    f32 z;
    u32 *p;
    s32 i;

    rare_gSPPerspNormalize((*gdl)++, gPerspNorm);

    cameraSel = gCameraSelector;
    if (gUseAlternateCamera) {
        gCameraSelector += 4;
        cameraSel = gCameraSelector;
    }

    update_camera_for_actor(&gCameras[cameraSel]);

    if (gCameraSelector == 4) {
        func_80046B58(gCameras[cameraSel].tx, gCameras[cameraSel].ty, gCameras[cameraSel].tz);
    }

    x = gCameras[cameraSel].tx - gWorldX;
    y = gCameras[cameraSel].ty;
    z = gCameras[cameraSel].tz - gWorldZ;

    if (x > *(f32*)0x80098388 /*32767.0f */ || *(f32*)0x8009838c /* -32767.0f */ > x || z > *(f32*)0x80098388 /* 32767.0f */ || *(f32*)0x8009838c > z /* -32767.0f */) {
        return;
    }
    
    gCameraSRT.transl.x = -x;
    gCameraSRT.transl.y = -y;
    gCameraSRT.transl.z = -z;
    gCameraSRT.yaw = gCameras[cameraSel].yaw - 0x8000;
    gCameraSRT.pitch = gCameras[cameraSel].pitch + gCameras[cameraSel].dpitch;
    gCameraSRT.roll = gCameras[cameraSel].roll;
    if (*(u32*)0x800a6a54 != 0) {
        gCameraSRT.transl.y -= gCameras[cameraSel].dty;
    }

    matrix_from_srt_reversed(&gViewMtx, &gCameraSRT);
    matrix_concat(&gViewMtx, &gProjectionMtx, &gViewProjMtx);
    matrix_f2l(&gViewProjMtx, *rspMtxs);

    gRSPMtxList = *rspMtxs;

    gSPMatrix((*gdl)++, *rspMtxs, 0);
    *rspMtxs++;

    gCameraSRT.yaw = -0x8000 - gCameras[cameraSel].yaw;
    gCameraSRT.pitch = -(gCameras[cameraSel].pitch + gCameras[cameraSel].dpitch);
    gCameraSRT.roll = -gCameras[cameraSel].roll;
    gCameraSRT.scale = 1.0f;
    gCameraSRT.transl.x = x;
    gCameraSRT.transl.y = y;
    gCameraSRT.transl.z = z;
    if (*(u32*)0x800a6a54 != 0) {
        gCameraSRT.transl.y += gCameras[cameraSel].dty;
    }
    matrix_from_srt(&gViewMtx2, &gCameraSRT);
    matrix_f2l(&gViewMtx2, &gRSPViewMtx2);

    gRSPMatrices[0] = NULL;
    gRSPMatrices[1] = NULL;

    p = 0x800a7bb0;
    for (i = 0; i < 28; i++) {
        p[i] = 0;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800029C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800029D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80002B2C.s")

// extremely close
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80002C0C.s")
#else
void func_80002C0C(Gfx **gdl, s32 scaleX, s32 scaleY, s32 transX, s32 transY)
{
    if (!(gViewports[gCameraSelector].flags & 0x1))
    {
        gRSPViewports[gCameraSelector].vp.vtrans[0] = transX << 2;
        gRSPViewports[gCameraSelector].vp.vtrans[1] = transY << 2;
        gRSPViewports[gCameraSelector].vp.vscale[0] = scaleX << 2;
        gRSPViewports[gCameraSelector].vp.vscale[1] = scaleY << 2;
        rare_gSPViewport((*gdl)++, OS_K0_TO_PHYSICAL(&gRSPViewports[gCameraSelector]));
    }
    else
    {
        rare_gSPViewport((*gdl)++, OS_K0_TO_PHYSICAL(&gRSPViewports[gCameraSelector + 5 * gCameraGroupSelector + 10]));
    }
}
#endif

void func_80002D14(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz)
{
    f32 nrm = x * gViewProjMtx.m[0][3] + y * gViewProjMtx.m[1][3] + z * gViewProjMtx.m[2][3] + gViewProjMtx.m[3][3];
    guMtxXFMF(&gViewProjMtx, x, y, z, ox, oy, oz);
    if (nrm != 0.0f)
    {
        nrm = 1.0f / nrm;
        *ox *= nrm;
        *oy *= nrm;
        *oz *= nrm;
    }
}

void func_80002DE4(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz)
{
    f32 dot = x * gViewProjMtx.m[0][3] + y * gViewProjMtx.m[1][3] + z * gViewProjMtx.m[2][3] + gViewProjMtx.m[3][3];
    if (dot != 0.0f)
    {
        x *= dot;
        y *= dot;
        z *= dot;
    }
    guMtxXFMF(&gViewMtx2, x, y, z, ox, oy, oz);
}

// close
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80002E94.s")
#else
void func_80002E94(f32 x, f32 y, f32 z, s32 *ox, s32 *oy, s32 *oz)
{
    if (ox != NULL) {
        *ox = x * (gRSPViewports[0].vp.vscale[0] >> 2) + (gRSPViewports[0].vp.vtrans[0] >> 2);
    }
    if (oy != NULL) {
        *oy = 240 - (int)((y * (gRSPViewports[0].vp.vscale[1] >> 2) + (gRSPViewports[0].vp.vtrans[1] >> 2)));
    }
    if (oz != NULL) {
        *oz = z * (gRSPViewports[0].vp.vscale[2] << 5) + (gRSPViewports[0].vp.vtrans[2] << 5);
    }
}
#endif

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80002F88.s")
#else
void func_80002F88(s32 x, s32 y, s32 z, f32 *ox, f32 *oy, f32 *oz)
{
    y = 240 - y;
    *ox = (f32)(x - (gRSPViewports[0].vp.vtrans[0] >> 2)) / (gRSPViewports[0].vp.vscale[0] >> 2);
    *oy = (f32)(y - (gRSPViewports[0].vp.vtrans[1] >> 2)) / (gRSPViewports[0].vp.vscale[1] >> 2);
    *oz = (f32)(z - (gRSPViewports[0].vp.vtrans[2] << 5)) / (gRSPViewports[0].vp.vscale[2] << 5);
}
#endif

void func_8000302C(Gfx **gdl)
{
    u32 wh;
    u32 width;
    u32 height;

    gCameraSelector = 4;

    wh = get_some_resolution_encoded();
    height = wh >> 16;
    width = wh & 0xffff;

    if (!(gViewports[gCameraSelector].flags & 0x1))
    {
        gDPSetScissor((*gdl)++, 0, 0, 0, width - 1, height - 1);
        func_80002C0C(gdl, width / 2, height / 2, width / 2, height / 2);
    }
    else
    {
        func_80002490(gdl);
        func_80002C0C(gdl, 0, 0, 0, 0);
    }

    gCameraSelector = 0;
}

extern MtxF MtxF_800a6a60;
void matrix_scaling(MtxF *mf, f32 sx, f32 sy, f32 sz);
void mat4_ident_translate(MtxF *mf, f32 x, f32 y, f32 z);
void func_80003168(Gfx **gdl, Mtx **rspMtxs, s32 x, s32 y, s32 z, f32 scale)
{
    MtxF mf;

    mat4_ident_translate(&MtxF_800a6a60, x * 640.0f, y, z * 640.0f);
    if (scale != 1.0f) {
        matrix_scaling(&mf, scale, scale, scale);
        matrix_concat(&mf, &MtxF_800a6a60, &MtxF_800a6a60);
    }
    matrix_f2l_4x3(&MtxF_800a6a60, *rspMtxs);

    // rare_gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL(*rspMtxs), 0);
    // ?????? what did rare do to the matrix commands?
    gDma2p((*gdl)++, G_MTX, OS_K0_TO_PHYSICAL((*rspMtxs)++), 0, 3, 0x38 * 8);
}

u32 func_80003278(u32 a0, u32 a1, u32 a2, u32 a3)
{
    return 1;
}

void func_80003294(u32 a0, u32 a1, u32 a2, u32 a4)
{
}

void func_800032AC(u32 a0, u32 a1, u32 a2, u32 a4)
{
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800032C4.s")

s32 func_8004454C(f32 x, f32 y, f32 z);
void func_800034CC(f32 x, f32 y, f32 z)
{
    gCameras[gCameraSelector].srt.transl.x += x;
    gCameras[gCameraSelector].srt.transl.y += y;
    gCameras[gCameraSelector].srt.transl.z += z;
    gCameras[gCameraSelector].unk_0x56 = func_8004454C(gCameras[gCameraSelector].srt.transl.x, gCameras[gCameraSelector].srt.transl.y, gCameras[gCameraSelector].srt.transl.z);
}

// extremely close
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_8000356C.s")
#else
f32 fcos16_precise(s16 theta);
f32 fsin16_precise(s16 theta);
// Pushes the camera in a given direction on the XZ plane?
void _func_8000356C(f32 a, f32 b)
{
    gCameras[gCameraSelector].srt.transl.x -= a * fcos16_precise(gCameras[gCameraSelector].srt.yaw);
    gCameras[gCameraSelector].srt.transl.z -= a * fsin16_precise(gCameras[gCameraSelector].srt.yaw);
    gCameras[gCameraSelector].srt.transl.x -= b * fsin16_precise(gCameras[gCameraSelector].srt.yaw);
    gCameras[gCameraSelector].srt.transl.z += b * fcos16_precise(gCameras[gCameraSelector].srt.yaw);
    gCameras[gCameraSelector].unk_0x56 = func_8004454C(gCameras[gCameraSelector].srt.transl.x, gCameras[gCameraSelector].srt.transl.y, gCameras[gCameraSelector].srt.transl.z);
}
#endif

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/get_vec3_to_camera_normalized.s")
#else
f32 _sqrtf(f32 x);
void _get_vec3_to_camera_normalized(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz)
{
    f32 nrm;
    u32 cameraSel = gCameraSelector;

    *ox = gCameras[cameraSel].srt.translation.x - x;
    *oy = gCameras[cameraSel].srt.translation.y - y;
    *oz = gCameras[cameraSel].srt.translation.z - z;

    nrm = _sqrtf(*ox * *ox + *oy * *oy + *oz * *oz);
    if (nrm != 0.0f)
    {
        nrm = 1.0f / nrm;
        *ox *= nrm;
        *oy *= nrm;
        *oz *= nrm;
    }
}
#endif

// Unused
void rotate_camera(s32 yaw, s32 pitch, s32 roll)
{
    gCameras[gCameraSelector].srt.yaw += yaw;
    gCameras[gCameraSelector].srt.pitch += pitch;
    gCameras[gCameraSelector].srt.roll += roll;
}

Camera *get_main_camera()
{
    return &gCameras[gCameraSelector];
}

Camera *get_camera()
{
    if (gUseAlternateCamera) {
        return &gCameras[gCameraSelector + 4];
    }

    return &gCameras[gCameraSelector];
}

Camera *get_camera_array()
{
    if (gUseAlternateCamera) {
        return &gCameras[4];
    }

    return gCameras;
}

MtxF *func_8000388c()
{
    return &gViewMtx2;
}

MtxF *func_8000389c()
{
    return &gViewProjMtx;
}

MtxF *func_800038AC()
{
    return &gProjectionMtx;
}

Mtx *func_800038BC()
{
    return &gRSPProjectionMtx;
}

MtxF *func_800038CC()
{
    return &gViewMtx;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800038DC.s")
#else
u32 _func_800038DC(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, u8 param_6)
{
    f32 vx;
    f32 vy;
    f32 vz;

    if (param_6) {
        guMtxXFMF(gViewMtx.m, x, y, z, &vx, &vy, &vz);
    }

    vx = gProjectionMtx.m[0][0] * vx;
    vy = gProjectionMtx.m[1][1] * vy;
    vz = -gProjectionMtx.m[2][3] * vz;

    if (vz < -2.0f)
    {
        Vp *rspvp;

        rspvp = &gRSPViewports[gCameraSelector];
        if (gViewports[gCameraSelector].flags & 0x1) {
            rspvp += gCameraGroupSelector * 5 + 10;
        }

        *ox = (rspvp->vp.vtrans[0] >> 2) - (vx * (rspvp->vp.vscale[0] >> 2)) / vz;
        *oy = (rspvp->vp.vtrans[1] >> 2) + (vy * (rspvp->vp.vscale[1] >> 2)) / vz;

        return 1;
    }

    return 0;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003A60.s")

f32 _sqrtf(f32 x);
void func_80003AA0(f32 x, f32 y, f32 z, f32 distance, f32 param_5)
{
    s32 i;

    for (i = 0; i < 8; i++)
    {
        f32 dx = x - gCameras[i].srt.transl.x;
        f32 dy = y - gCameras[i].srt.transl.y;
        f32 dz = z - gCameras[i].srt.transl.z;
        f32 d = _sqrtf(dx * dx + dy * dy + dz * dz);
        if (d < distance) {
            gCameras[i].unk_0x5d = 0;
            gCameras[i].dty = (distance - d) * param_5 / distance;
        }
    }
}

void func_80003B70(f32 dty)
{
    s32 i;

    for (i = 0; i < CAMERA_COUNT; i++)
    {
        gCameras[i].dty = dty;
        gCameras[i].unk_0x5d = 0;
    }
}

void func_80003bb0(f32 param_1, f32 param_2, f32 param_3)
{
    s32 i;

    for (i = 0; i < CAMERA_COUNT; i++)
    {
        gCameras[i].dty = param_1;
        gCameras[i].unk_0x30 = param_1;
        gCameras[i].unk_0x34 = param_2;
        gCameras[i].unk_0x38 = 0.0f;
        gCameras[i].unk_0x3c = param_3;
        gCameras[i].unk_0x5d = 1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003C48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003C68.s")

void add_matrix_to_pool(MtxF *mf, s32 count)
{
    gMatrixPool[gMatrixCount].mtx = (Mtx_MtxF*)mf;
    gMatrixPool[gMatrixCount++].count = count;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/tick_cameras.s")
#else
extern s8 gMatrixIndex;
extern f32 gFarPlane;
f32 fexp(f32 x, u32 iterations);
f32 fcos16_precise(s16 theta);
void _tick_cameras()
{
    *(s16*)0x8008c524 = *(s16*)0x8008c528;
    if (*(s16*)0x8008c518 != 0)
    {
        *(s16*)0x8008c518 -= *(u8*)0x8008c950;
        if (*(s16*)0x8008c518 < 0) {
            *(s16*)0x8008c518 = 0;
        }

        gFarPlane = (*(f32*)0x800a6270 - *(f32*)0x800a6274) * ((f32)*(s16*)0x8008c518 / *(s16*)0x8008c51c) + *(f32*)0x800a6274;
    }

    gMatrixPool[gMatrixCount].count = -1;
    convert_mtxf_to_mtx_in_pool(gMatrixPool);
    gMatrixCount = 0;
    gMatrixIndex = 0;

    if (gUseAlternateCamera) {
        gCameraSelector += 4;
    }

    if (gCameras[gCameraSelector].unk_0x5d == 0)
    {
        gCameras[gCameraSelector].unk_0x5c--;
        while (gCameras[gCameraSelector].unk_0x5c < 0) {
            gCameras[gCameraSelector].unk_0x5c++;
            gCameras[gCameraSelector].dty = -gCameras[gCameraSelector].dty * *(f32*)0x80098398 /* 0.09f */;
        }
    }
    else
    {
        if (gCameras[gCameraSelector].unk_0x5d == 1)
        {
            f32 exp = fexp(-gCameras[gCameraSelector].unk_0x3c * gCameras[gCameraSelector].unk_0x38, 20);
            f32 c = fcos16_precise(gCameras[gCameraSelector].unk_0x34 * *(f32*)0x8009839c /* 65535.0f */ * gCameras[gCameraSelector].unk_0x38)
                    * gCameras[gCameraSelector].unk_0x30 * exp;
            gCameras[gCameraSelector].dty = c;
            if (c < *(f32*)0x800983a0 /* 0.1f */ && c > *(f32*)0x800983a4 /* -0.1f */) {
                gCameras[gCameraSelector].unk_0x5d = -1;
                gCameras[gCameraSelector].dty = 0.0f;
            }
            gCameras[gCameraSelector].unk_0x38 += *(f32*)0x8008c958 /* 1.0f */ / 60.0f;
        }
    }
}
#endif

// Returns e to the power of x
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/fexp.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/setup_rsp_matrices_for_actor.s")
#else
extern Mtx *gRSPMatrices[2]; // TODO: how many matrices are there?
extern f32 gWorldX;
extern f32 gWorldZ;
extern MtxF MtxF_800a6a60;
extern MtxF MtxF_800a6aa0;
extern MtxF gAuxMtx2;
void _setup_rsp_matrices_for_actor(Gfx **gdl, Mtx **matrices, TActor *actor)
{
    TActor *link = actor;
    u8 isChild;
    f32 oldScale;
    MtxF mtxf;

    if (gRSPMatrices[link->matrixIdx] == NULL)
    {
        isChild = FALSE;

        while (link != NULL)
        {
            if (link->linkedActor == NULL) {
                link->srt.tx -= gWorldX;
                link->srt.tz -= gWorldZ;
            }

            oldScale = link->srt.scale;
            if (!(link->unk0xb0 & 0x8)) {
                link->srt.scale = 1.0f;
            }

            if (!isChild) {
                matrix_from_srt(&MtxF_800a6a60, &link->srt);
            } else {
                matrix_from_srt(&mtxf, &link->srt);
                matrix_concat_4x3(&MtxF_800a6a60, &mtxf, &MtxF_800a6a60);
            }

            link->srt.scale = oldScale;

            if (link->linkedActor == NULL) {
                link->srt.tx += gWorldX;
                link->srt.tz += gWorldZ;
            }

            link = link->linkedActor;
            isChild = TRUE;
        }

        matrix_concat(&MtxF_800a6a60, &MtxF_800a6aa0, &gAuxMtx2);
        matrix_f2l(&gAuxMtx2, *matrices);
        gRSPMatrices[actor->matrixIdx] = *matrices;
        (*matrices)++;
    }

    // FIXME: Requires #define F3DEX_GBI_2x (???)
    // gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL(gRSPMatrices[actor->matrixIdx]), 7);
    {
        Gfx *g = (*gdl)++;

        g->words.w0 = 0xda380007;
        g->words.w1 = OS_K0_TO_PHYSICAL(gRSPMatrices[actor->matrixIdx]);
    }
}
#endif

void func_80004224(Gfx **gdl)
{
    // ???
    gDma1p((*gdl)++, G_MTX, OS_K0_TO_PHYSICAL(gRSPMtxList), 7, 0x38);
}

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80004258.s")
#else
extern s8 gMatrixIndex;
void func_800042C8(TActor *actor, int matrixIdx);
s32 func_80004258(TActor *actor)
{
    func_800042C8(actor, gMatrixIndex);
    gRSPMatrices[gMatrixIndex] = NULL;
    gMatrixIndex++;
    return gMatrixIndex - 1;
}
#endif

void func_800042A8(TActor *actor)
{
    func_800042C8(actor, actor->matrixIdx);
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800042C8.s")
#else
extern MtxF gAuxMtx2;
void _func_800042C8(TActor *actor, int matrixIdx)
{
    u8 isChild = FALSE;
    f32 oldScale;
    TActor* actorList[2]; // FIXME: really? only two?
    s32 actorCount = 0;
    s32 i;
    MtxF* pmtx;

    while (actor != NULL)
    {
        actorList[actorCount++] = actor;
        actorCount = (s8)actorCount;

        pmtx = &gActorMatrices[matrixIdx];

        oldScale = actor->srt.scale;
        actor->srt.scale = 1.0f;

        if (!isChild) {
            matrix_from_srt(pmtx, &actor->srt);
        } else {
            matrix_from_srt(&gAuxMtx2, &actor->srt);
            matrix_concat(pmtx, &gAuxMtx2, pmtx);
        }

        actor->srt.scale = oldScale;

        actor = actor->linkedActor;

        isChild = TRUE;
    }

    while (actorCount > 0)
    {
        TActor *pactor = actorList[(s8)--actorCount];
        SRT invsrt;

        invsrt.tx = -pactor->srt.tx;
        invsrt.ty = -pactor->srt.ty;
        invsrt.tz = -pactor->srt.tz;
        invsrt.scale = 1.0f;
        invsrt.yaw = -pactor->srt.yaw;
        invsrt.pitch = -pactor->srt.pitch;
        invsrt.roll = -pactor->srt.roll;
        matrix_from_srt_reversed(&gInverseActorMatrices[matrixIdx], &invsrt);
    }
}
#endif

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/get_actor_child_position.s")
#else
void _get_actor_child_position(TActor *actor, float *ox, float *oy, float *oz)
{
    if (actor->linkedActor == NULL)
    {
        *ox = actor->srt.tx;
        *oy = actor->srt.ty;
        *oz = actor->srt.tz;
    }
    else
    {
        guMtxXFMF(&gActorMatrices[actor->linkedActor->matrixIdx],
            actor->srt.tx, actor->srt.ty, actor->srt.tz, ox, oy, oz);
    }
}
#endif

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/transform_point_by_actor.s")
#else
void _transform_point_by_actor(float x, float y, float z, float *ox, float *oy, float *oz, TActor *actor)
{
    if (actor != NULL)
    {
        guMtxXFMF(&gActorMatrices[actor->matrixIdx], x, y, z, ox, oy, oz);
    }
    else
    {
        *ox = x;
        *oy = y;
        *oz = z;
    }
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/inverse_transform_point_by_actor.s")
#else
void _inverse_transform_point_by_actor(float x, float y, float z, float *ox, float *oy, float *oz, TActor *actor)
{
    if (actor != NULL)
    {
        guMtxXFMF(&gInverseActorMatrices[actor->matrixIdx], x, y, z, ox, oy, oz);
    }
    else
    {
        *ox = x;
        *oy = y;
        *oz = z;
    }
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/inverse_rotate_point_by_actor.s")
#else
void _inverse_rotate_point_by_actor(float x, float y, float z, float *ox, float *oy, float * oz, TActor *actor)
{
    Vec3f v;

    v.x = x;
    v.y = y;
    v.z = z;
    vec3_transform_no_translate(&gInverseActorMatrices[actor->matrixIdx], &v, &v);
    *ox = v.x;
    *oy = v.y;
    *oz = v.y;
}
#endif

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/rotate_point_by_actor.s")
#else
void _rotate_point_by_actor(float x, float y, float z, float *ox, float *oy, float *oz, TActor *actor)
{
    Vec3f v;

    v.x = x;
    v.y = y;
    v.z = z;
    vec3_transform_no_translate(&gActorMatrices[actor->matrixIdx], &v, &v);
    *ox = v.x;
    *oy = v.y;
    *oz = v.z;
}
#endif

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/transform_srt_by_actor.s")
#else
void _transform_srt_by_actor(SRT *srt, SRT *out, TActor *actor)
{
    if (actor == NULL)
    {
        out->tx = srt->tx;
        out->ty = srt->ty;
        out->tz = srt->tz;
        out->yaw = srt->yaw;
        out->pitch = srt->pitch;
        out->roll = srt->roll;
    }
    else
    {
        guMtxXFMF(&gActorMatrices[actor->matrixIdx], srt->tx, srt->ty, srt->tz, &out->tx, &out->ty, &out->tz);
        out->yaw = actor->srt.yaw + srt->yaw;
        out->pitch = srt->pitch;
        out->roll = srt->roll;
    }
}
#endif

void func_800047C8(SRT *a, SRT *b, SRT *out)
{
    MtxF mf;
    SRT tempsrt;
    s32 yaw;

    tempsrt.transl.x = -b->transl.x;
    tempsrt.transl.y = -b->transl.y;
    tempsrt.transl.z = -b->transl.z;
    tempsrt.scale = 1.0f;
    tempsrt.yaw = -b->yaw;
    tempsrt.pitch = -b->pitch;
    tempsrt.roll = -b->roll;

    matrix_from_srt_reversed(&mf, &tempsrt);

    guMtxXFMF(&mf, a->transl.x, a->transl.y, a->transl.z, &out->transl.x, &out->transl.y, &out->transl.z);

    yaw = a->yaw - b->yaw;
    if (yaw > 0x8000) {
        yaw -= 0xffff;
    }
    if (yaw < -0x8000) {
        yaw += 0xffff;
    }

    out->yaw = yaw;
    out->pitch = a->pitch;
    out->roll = a->roll;
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/update_camera_for_actor.s")

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/transform_point_by_actor_matrix.s")
#else
void _transform_point_by_actor_matrix(Vec3f *v, Vec3f *ov, s8 matrixIdx)
{
    if (matrixIdx < 0)
    {
        ov->x = v->x;
        ov->y = v->y;
        ov->z = v->z;
    }
    else
    {
        guMtxXFMF(gActorMatrices[matrixIdx], v->x, v->y, v->z, &ov->x, &ov->y, &ov->z);
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80004A30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/fbSetBg.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80004A4C.s")
