#include "sys/camera.h"

#include "PR/gbi.h"
#include "libultra/io/piint.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/gfx/gx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "functions.h"

static const char str_80098290[] = "Camera Error: Illegal mode!\n";
static const char str_800982b0[] = "Camera Error: Illegal player no!\n";
static const char str_800982d4[] = "CAMCONTROL!!!\n";
static const char str_800982e4[] = "Camera out of range: %d, (%.1f,%.1f,%.1f) (%.1f,%.1f)\n";
static const char str_8009831c[] = "%x.";
static const char str_80098320[] = "%x  ";
static const char str_80098328[] = "\n";
static const char str_8009832c[] = "\n";
static const char str_80098330[] = "%f  ";
static const char str_80098338[] = "\n";
static const char str_8009833c[] = "\n";
static const char str_80098340[] = "CAM Error!! Convlist overflow.\n";

// .bss
// pad to 0x800a7c20
static u8 _bss_pad[0x3E38]; // TODO: remove bss padding

extern f32 D_800A6270;
extern f32 D_800A6274;
extern Camera gCameras[CAMERA_COUNT];
extern u32 UINT_800a66f8;
extern s32 gCameraSelector;
extern s32 D_800A6700;
extern SRT gCameraSRT;
extern f32 gFovY;
extern f32 gAspect;
extern s8 gTriggerUseAlternateCamera;
extern s8 gUseAlternateCamera;
extern MatrixSlot gMatrixPool[100];
extern u32 gMatrixCount;
extern u32 UINT_800a6a54;
extern u16 gPerspNorm;
extern MtxF MtxF_800a6a60;
extern MtxF gViewProjMtx;
extern Mtx *gRSPMtxList; // FIXME: not a list
extern MtxF gProjectionMtx;
extern MtxF gViewMtx;
extern MtxF gViewMtx2;
extern Mtx gRSPProjectionMtx;
extern Mtx gRSPViewMtx2;
extern MtxF gAuxMtx;
extern MtxF gAuxMtx2;
extern MtxF gObjectMatrices[30];
extern MtxF gInverseObjectMatrices[30];
static Mtx *gRSPMatrices[30];                        // <----- NOTE: static!
extern s8 gMatrixIndex;


// .data

// The viewport z-range below is half of the max (511)
#define G_HALFZ (G_MAXZ / 2) /* 9 bits of integer screen-Z precision */

// x1, y1, x2, y2
// posX, posY, width, height
// scissorX1, scissorY1, scissorX2, scissorY2
// flags
#define DEFAULT_VIEWPORT                                                                                         \
    0, 0, 320, 240, \
    160, 120, 320, 240, \
    0, 0, 320 - 1, 240 - 1, \
    0

f32 gNearPlane = 4.0f;
f32 gFarPlane = 10000.0f;
s16 D_8008C518 = 0;
s16 D_8008C51C = 0;
s8 D_8008C520 = 0; // gAntiPiracyViewport?
s16 SHORT_8008c524 = 0;
s16 SHORT_8008c528 = 0;
f32 FLOAT_8008c52c = 1.0f;
Viewport gViewports[4] = {
    { DEFAULT_VIEWPORT },
    { DEFAULT_VIEWPORT },
    { DEFAULT_VIEWPORT },
    { DEFAULT_VIEWPORT },
};
u32 gCameraGroupSelector = 0;
u8 data_8008c604[] = { 
    0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xff, 0xff, 
    0xff, 0xff
};
Vp gRSPViewports[20] = {
    { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } }, { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } },
    { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } }, { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } },
    { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } }, { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } },
    { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } }, { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } },
    { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } }, { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } },
    { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } }, { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } },
    { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } }, { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } },
    { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } }, { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } },
    { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } }, { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } },
    { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } }, { { { 0, 0, G_HALFZ, 0 }, { 0, 0, G_HALFZ, 0 } } },
};
SRT D_8008C758[2] = {
    { 0, 0, 0, 0, 1.0f, { 0.0f, 0.0f, -281.0f }},
    { 0, 0, 0, 0, 1.0f, { 0.0f, 0.0f, 0.0f }}
};
MtxF D_8008C788 = {
    {
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 160.0f },
    }
};

// ROM address
extern u32 D_B0000578;

f32 fexp(f32 x, u32 iterations);

void camera_init() {
    s32 i;
    u32 stat;

    for (i = 0; i < 12; i++) {
        gCameraSelector = i;
        func_80001914(200, 200, 200, 0, 0, 180);
    }

    gTriggerUseAlternateCamera = 0;
    gUseAlternateCamera = 0;
    gCameraSelector = 0;
    UINT_800a66f8 = 0;
    UINT_800a6a54 = 0;
    gMatrixCount = 0;
    gMatrixIndex = 0;
    gFarPlane = 10000.0f;
    D_8008C518 = 0;
    D_8008C520 = 0;

    WAIT_ON_IOBUSY(stat);

    // 0xB0000578 is a direct read from the ROM as opposed to RAM
    if (((D_B0000578 & 0xFFFF) & 0xFFFF) != 0x8965) {
        D_8008C520 = TRUE;
    }

    gAspect = 1.3333334f;
    gFovY = 60.0f;

    guPerspectiveF(gProjectionMtx.m, &gPerspNorm, gFovY, gAspect, gNearPlane, gFarPlane, 1.0f);
    matrix_f2l(&gProjectionMtx, &gRSPProjectionMtx);

    FLOAT_8008c52c = gProjectionMtx.m[0][0];
    SHORT_8008c524 = 0;
    SHORT_8008c528 = 0;
}

void func_800013BC() {
    UINT_800a6a54 = 1;
}

void func_800013D0() {
    UINT_800a6a54 = 0;
}

u32 func_800013E0() {
    return UINT_800a6a54;
}

f32 camera_get_fov(void)
{
    return gFovY;
}

void camera_set_fov(f32 fovY)
{
    if (fovY > 90.0f) {
        fovY = 90.0f;
    } else if (fovY < 40.0f) {
        fovY = 40.0f;
    }

    gFovY = fovY;

    guPerspectiveF(gProjectionMtx.m, &gPerspNorm, gFovY, gAspect, gNearPlane, gFarPlane, 1.0f);
    matrix_f2l(&gProjectionMtx, &gRSPProjectionMtx);
    FLOAT_8008c52c = gProjectionMtx.m[0][0];
}

void camera_set_ortho_projection_matrix(f32 l, f32 r, f32 b, f32 t)
{
    guOrthoF(gProjectionMtx.m, l, r, b, t, 0.0f, 400.0f, 20.0f);
}

void camera_set_aspect(f32 aspect)
{
    gAspect = aspect;
    guPerspectiveF(gProjectionMtx.m, &gPerspNorm, gFovY, gAspect, gNearPlane, gFarPlane, 1.0f);
    matrix_f2l(&gProjectionMtx, &gRSPProjectionMtx);
    FLOAT_8008c52c = gProjectionMtx.m[0][0];
    gProjectionMtx.m[0][3] *= 0.5f;
}

void camera_set_near_plane(f32 near)
{
    gNearPlane = near;
    guPerspectiveF(gProjectionMtx.m, &gPerspNorm, gFovY, gAspect, gNearPlane, gFarPlane, 1.0f);
    matrix_f2l(&gProjectionMtx, &gRSPProjectionMtx);
    FLOAT_8008c52c = gProjectionMtx.m[0][0];
    gProjectionMtx.m[0][3] *= 0.5f;
}

void func_80001660(f32 farPlane, s32 param2) {
    if (param2 != 0) {
        D_8008C51C = param2;
        D_8008C518 = D_8008C51C;
        D_800A6270 = gFarPlane;
        D_800A6274 = farPlane;
    } else {
        gFarPlane = farPlane;
    }
}

f32 camera_get_far_plane()
{
    return gFarPlane;
}

void func_800016B8()
{
    camera_set_near_plane(4.0f);
}

void func_800016E0() {
    func_80001660(10000.0f, 60);
}

void func_80001708()
{
    guPerspectiveF(gProjectionMtx.m, &gPerspNorm, 60.0f, 4.0f / 3.0f, gNearPlane, gFarPlane, 1.0f);
    matrix_f2l(&gProjectionMtx, &gRSPProjectionMtx);
}

MtxF *camera_get_aux_mtx() {
    return &gAuxMtx2;
}

u32 func_80001788() {
    return UINT_800a66f8;
}

u32 get_camera_selector()
{
    return gCameraSelector;
}

void func_800017A8(Gfx **gdl, Mtx **rspMtxs) {
    Camera *camera;
    s16 yaw;
    s16 pitch;
    s16 roll;
    s16 dpitch;
    f32 posX;
    f32 posY;
    f32 posZ;

    func_80001A5C(0);
    set_camera_selector(0);

    camera = get_camera();
    yaw = camera->srt.yaw;
    pitch = camera->srt.pitch;
    roll = camera->srt.roll;
    posX = camera->srt.transl.x;
    posY = camera->srt.transl.y;
    posZ = camera->srt.transl.z;
    dpitch = camera->dpitch;

    camera->srt.roll = 0;
    camera->srt.pitch = 0;
    camera->srt.yaw = -0x8000;
    camera->dpitch = 0;
    camera->srt.transl.x = 0.0f;
    camera->srt.transl.y = 0.0f;
    camera->srt.transl.z = 0.0f;

    func_800021A0(gdl, rspMtxs);

    camera->dpitch = dpitch;
    camera->srt.yaw = yaw;
    camera->srt.pitch = pitch;
    camera->srt.roll = roll;
    camera->srt.transl.x = posX;
    camera->srt.transl.y = posY;
    camera->srt.transl.z = posZ;
}

f32 func_80001884(f32 x, f32 y, f32 z)
{
    s32 cameraSel;
    f32 dz;
    f32 dx;
    f32 dy;

    cameraSel = gCameraSelector;

    if (gUseAlternateCamera) {
        cameraSel += 4;
    }

    dz = (z - gCameras[cameraSel].srt.transl.z);\
    dy = (x - gCameras[cameraSel].srt.transl.x);\
    dx = (y - gCameras[cameraSel].srt.transl.y);
    
    return sqrtf((dz * dz) + ((dy * dy) + (dx * dx)));
}

// camera_reset
void func_80001914(s32 tx, s32 ty, s32 tz, s32 roll, s32 pitch, s32 yaw)
{
    gCameras[gCameraSelector].srt.transl.x = tx;
    gCameras[gCameraSelector].srt.transl.y = ty;
    gCameras[gCameraSelector].srt.transl.z = tz;
    gCameras[gCameraSelector].unk18 = 60.0f;
    gCameras[gCameraSelector].srt.roll = roll * 0xb6;
    gCameras[gCameraSelector].srt.pitch = pitch * 0xb6;
    gCameras[gCameraSelector].srt.yaw = yaw * 0xb6;
    gCameras[gCameraSelector].unk20 = 0.0f;
    gCameras[gCameraSelector].unk24 = 0.0f;
    gCameras[gCameraSelector].unk28 = 0.0f;
    gCameras[gCameraSelector].dty = 0.0f;
    gCameras[gCameraSelector].object = NULL;
    gCameras[gCameraSelector].dpitch = 0;
}

void func_800019BC(s32 selector, f32 x, f32 y, f32 z, s16 yaw, s16 pitch, s16 roll)
{
    selector += 4;

    gCameras[selector].dpitch = 0;
    gCameras[selector].srt.transl.x = x;
    gCameras[selector].srt.transl.y = y;
    gCameras[selector].srt.transl.z = z;
    gCameras[selector].srt.yaw = yaw;
    gCameras[selector].srt.pitch = pitch;
    gCameras[selector].srt.roll = roll;

    if (selector < 8) {
        gTriggerUseAlternateCamera = TRUE;
    }
}

s8 func_80001A2C()
{
    return gUseAlternateCamera;
}

void func_80001A3C()
{
    gUseAlternateCamera = gTriggerUseAlternateCamera;
    gTriggerUseAlternateCamera = 0;
}

// set_active_viewports_and_max?
s32 func_80001A5C(s32 num) {
    if (num >= 0 && num < 4) {
        UINT_800a66f8 = num;
    } else {
        UINT_800a66f8 = 0;
    }

    switch (UINT_800a66f8) {
        case 0:
            D_800A6700 = 1;
            break;
        case 1:
            D_800A6700 = 2;
            break;
        case 2:
            D_800A6700 = 3;
            break;
        case 3:
            D_800A6700 = 4;
            break;
    }

    if (D_800A6700 <= gCameraSelector) {
        gCameraSelector = 0;
    }

    return D_800A6700;
}

void set_camera_selector(s32 selector)
{
    if (selector >= 0 && selector < 4) {
        gCameraSelector = selector;
    } else {
        gCameraSelector = 0;
    }
}

void func_80001B40() {
    s32 width;
    s32 height;
    s32 port;
    s32 yPos;
    s32 xPos;
    s32 i;
    
    gCameraGroupSelector = 1 - gCameraGroupSelector;

    for (i = 0; i < 4; i++) {
        if (gViewports[i].flags & 0x4) {
            gViewports[i].flags &= ~1;
        } else if (gViewports[i].flags & 0x2) {
            gViewports[i].flags |= 1;
        }

        gViewports[i].flags &= ~(2 | 4);

        if (gViewports[i].flags & 1) {
            if (!(gViewports[i].flags & 8)) {
                xPos = (((gViewports[i].x2 - gViewports[i].x1) + 1) << 1) + (gViewports[i].x1 * 4);
            } else {
                xPos = gViewports[i].posX;
                xPos *= 4;
            }

            if (!(gViewports[i].flags & 16)) {
                yPos = (((gViewports[i].y2 - gViewports[i].y1) + 1) << 1) + (gViewports[i].y1 * 4);
            } else {
                yPos = gViewports[i].posY;
                yPos *= 4;
            }

            if (!(gViewports[i].flags & 32)) {
                width = gViewports[i].x2 - gViewports[i].x1;
                width += 1;
                width *= 2;
            } else {
                width = gViewports[i].width;
                width *= 2;
            }

            if (!(gViewports[i].flags & 64)) {
                height = gViewports[i].y2 - gViewports[i].y1;
                height += 1;
                height *= 2;
            } else {
                height = gViewports[i].height;
                height *= 2;
            }

            port = i + (gCameraGroupSelector * 5) + 10;

            gRSPViewports[port].vp.vtrans[0] = xPos;
            gRSPViewports[port].vp.vtrans[1] = yPos;
            gRSPViewports[port].vp.vscale[0] = width;
            gRSPViewports[port].vp.vscale[1] = height;
        }
    }
}

void func_80001CE0(s32 selector, u32 param_2)
{
    if (param_2 != 0) {
        gViewports[selector].flags |= 0x1;
    } else {
        gViewports[selector].flags |= 0x2;
    }
    gViewports[selector].flags &= ~0x4;
}

void func_80001D58(s32 selector, u32 param_2)
{
    if (param_2 != 0) {
        gViewports[selector].flags &= ~0x1;
    } else {
        gViewports[selector].flags |= 0x4;
    }
    gViewports[selector].flags &= ~0x2;
}

u32 func_80001DD4(s32 selector)
{
    return gViewports[selector].flags & 0x1;
}

void func_80001E04(s32 viewPortIndex, s32 x1, s32 y1, s32 x2, s32 y2) {
    s32 resolution;
    s32 width;
    s32 height;
    s32 temp;

    resolution = vi_get_current_size();
    height = GET_VIDEO_HEIGHT(resolution) & 0xFFFF;
    width = GET_VIDEO_WIDTH(resolution);

    if (x2 < x1) {
        temp = x1;
        x1 = x2;
        x2 = temp;
    }

    if (y2 < y1) {
        temp = y1;
        y1 = y2;
        y2 = temp;
    }

    if (x1 >= width || x2 < 0 || y1 >= height || y2 < 0) {
        gViewports[viewPortIndex].ulx = 0;
        gViewports[viewPortIndex].uly = 0;
        gViewports[viewPortIndex].lrx = 0;
        gViewports[viewPortIndex].lry = 0;
    } else {
        if (x1 < 0) {
            gViewports[viewPortIndex].ulx = 0;
        } else {
            gViewports[viewPortIndex].ulx = x1;
        }

        if (y1 < 0) {
            gViewports[viewPortIndex].uly = 0;
        } else {
            gViewports[viewPortIndex].uly = y1;
        }

        if (x2 >= width) {
            gViewports[viewPortIndex].lrx = width - 1;
        } else {
            gViewports[viewPortIndex].lrx = x2;
        }

        if (y2 >= height) {
            gViewports[viewPortIndex].lry = height - 1;
        } else {
            gViewports[viewPortIndex].lry = y2;
        }
    }

    gViewports[viewPortIndex].x1 = x1;
    gViewports[viewPortIndex].x2 = x2;
    gViewports[viewPortIndex].y1 = y1;
    gViewports[viewPortIndex].y2 = y2;
}

void func_80001F70(s32 viewPortIndex, s32 posX, s32 posY, s32 width, s32 height) {
    if (posX != 0x8000) {
        gViewports[viewPortIndex].posX = posX;
        gViewports[viewPortIndex].flags |= 8;
    } else {
        gViewports[viewPortIndex].flags &= ~8;
    }

    if (posY != 0x8000) {
        //!@bug Viewport Y writes to the X value. Luckily, this function is unused.
        gViewports[viewPortIndex].posX = posY;
        gViewports[viewPortIndex].flags |= 16;
    } else {
        gViewports[viewPortIndex].flags &= ~16;
    }

    if (width != 0x8000) {
        gViewports[viewPortIndex].width = width;
        gViewports[viewPortIndex].flags |= 32;
    } else {
        gViewports[viewPortIndex].flags &= ~32;
    }

    if (height != 0x8000) {
        gViewports[viewPortIndex].height = height;
        gViewports[viewPortIndex].flags |= 64;
    } else {
        gViewports[viewPortIndex].flags &= ~64;
    }
}

u32 func_8000206C(s32 selector, s32 *ulx, s32 *uly, s32 *lrx, s32 *lry)
{
    *ulx = gViewports[selector].ulx;
    *lrx = gViewports[selector].lrx;
    *uly = gViewports[selector].uly;
    *lry = gViewports[selector].lry;

    if ((*ulx | *lrx | *uly | *lry) == 0) {
        return 0;
    }

    return 1;
}

void func_800020E4(s32 selector, s32 *x1, s32 *y1, s32 *x2, s32 *y2) {
    *x1 = gViewports[selector].x1;
    *y1 = gViewports[selector].y1;
    *x2 = gViewports[selector].x2;
    *y2 = gViewports[selector].y2;
}

void func_80002130(s32 *ulx, s32 *uly, s32 *lrx, s32 *lry)
{
    u32 wh = vi_get_current_size();
    u32 width = GET_VIDEO_WIDTH(wh);
    u32 height = GET_VIDEO_HEIGHT(wh);

    *ulx = 0;
    *lrx = width;
    *uly = SHORT_8008c524 + 6;
    *lry = height - SHORT_8008c524 - 6;
}

#ifdef NON_EQUIVALENT
void func_800021A0(Gfx **gdl, Mtx **rspMtxs)
{
    s32 cameraSelPre;
    u32 wh;
    s32 cameraSelPost;
    u32 width;
    u32 height;
    s32 scaleX;
    s32 scaleY;
    s32 transX;
    s32 transY;
    u32 mode;

    cameraSelPre = gCameraSelector;
    wh = vi_get_current_size();
    cameraSelPost = gCameraSelector;
    width = GET_VIDEO_WIDTH(wh);
    height = GET_VIDEO_HEIGHT(wh);

    if (gViewports[cameraSelPre].flags & 0x1)
    {
        gCameraSelector = cameraSelPre;
        gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, gViewports[gCameraSelector].ulx, gViewports[gCameraSelector].uly, gViewports[gCameraSelector].lrx, gViewports[gCameraSelector].lry);
        func_80002C0C(gdl, 0, 0, 0, 0);
        if (rspMtxs != NULL) {
            gCameraSelector = cameraSelPost;
            setup_rsp_camera_matrices(gdl, rspMtxs);
        }
        gCameraSelector = cameraSelPost;
        return;
    }

    mode = UINT_800a66f8;
    if (mode == 2) {
        mode = 3;
    }

    scaleY = height / 2;
    if (osTvType == OS_TV_PAL) {
        scaleY = 145;
    }

    switch (mode)
    {
    case 0:
        transX = scaleX;
        transY = scaleY;
        if (osTvType == OS_TV_PAL) {
            transY = scaleY - 18;
        }
        break;
    case 1:
        if (gCameraSelector == 0) {
            transY = height / 4;
            transX = scaleX;
            if (osTvType == OS_TV_PAL) {
                transY -= 12;
            }
        } else {
            transY = height / 2 + height / 4;
            transX = scaleX;
        }
        break;
    case 2:
        transX = scaleX + width / 4;
        transY = scaleY;
        if (gCameraSelector == 0) {
            transX = width / 4;
        }
        break;
    case 3:
        scaleX = width / 4;
        transX = scaleX;
        scaleY = scaleY / 2;
        transY = height / 2;
        if (osTvType == OS_TV_PAL) {
            transY = scaleY - 6;
            if (gCameraSelector < 2) {
                transY = scaleY - 20;
            }
        }
        break;
    }

    if (osTvType == OS_TV_PAL) {
        transX -= 4;
    }

    func_80002C0C(gdl, scaleX, scaleY, transX, transY);
    if (rspMtxs != NULL) {
        setup_rsp_camera_matrices(gdl, rspMtxs);
    }
    func_80002490(gdl);

    gCameraSelector = cameraSelPre;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/camera/func_800021A0.s")
#endif

void func_80002490(Gfx **gdl)
{
    s32 ulx, uly, lrx, lry;
    s32 wh;
    s32 centerX;
    s32 centerY;
    s32 width;
    s32 height;
    s32 padX;
    s32 padY;
    s32 mode;
    
    wh = vi_get_current_size();
    height = GET_VIDEO_HEIGHT(wh) & 0xFFFF;
    width = GET_VIDEO_WIDTH(wh);
    
    mode = UINT_800a66f8;
    
    if (mode != 0)
    {
        if (mode == 2) {
            mode = 3;
        }

        lrx = ulx = 0;
        lry = uly = 0;
        lrx += width;\
        lry += height;

        centerX = width >> 1;
        centerY = height >> 1;
        padX = width >> 8;
        padY = height >> 7;

        switch (mode)
        {
        case 1:
            if (gCameraSelector == 0) {
                lry = centerY - padY;
            } else {
                lry = height - padY;
                uly = centerY + padY;
            }
            break;
        case 2:
            if (gCameraSelector == 0) {
                lrx = centerX - padX;
                ulx = 0;
            } else {
                lrx = width - padX;
                ulx = centerX + padX;
            }
            break;
        case 3:
            switch (gCameraSelector)
            {
            case 0:
                lrx = centerX - padX;\
                lry = centerY - padY;
                break;
            case 1:
                ulx = centerX + padX;
                lrx = width - padX;
                lry = centerY - padY;
                break;
            case 2:
                uly = centerY + padY;
                lrx = centerX - padX;
                lry = height - padY;
                break;
            case 3:
                ulx = centerX + padX;\
                uly = centerY + padY;
                lry = height - padY;
                lrx = width - padX;
                break;
            }
            break;
        }
    }
    else
    {
        ulx = 0;
        uly = 0;
        lrx = width;\
        lry = height;

        uly += SHORT_8008c524 + 6;
        lry -= SHORT_8008c524 + 6;
    }

    gDPSetScissor((*gdl)++, 0, ulx, uly, lrx, lry);
}

void setup_rsp_camera_matrices(Gfx **gdl, Mtx **rspMtxs) {
    s32 prevCameraSel;
    f32 x,y,z;
    s32 i;
    Camera *camera;

    gSPPerspNormalize((*gdl)++, gPerspNorm);

    prevCameraSel = gCameraSelector;
    
    if (gUseAlternateCamera) {
        gCameraSelector += 4;
    }
 
    camera = &gCameras[gCameraSelector];

    update_camera_for_object(camera);

    if (gCameraSelector == 4) {
        map_func_80046B58(camera->tx, camera->ty, camera->tz);
    }

    x = camera->tx - gWorldX;
    y = camera->ty;
    z = camera->tz - gWorldZ;

    if (x > 32767.0f || -32767.0f > x || z > 32767.0f || -32767.0f > z) {
        return;
    }
    
    gCameraSRT.yaw = 0x8000 + camera->yaw;
    gCameraSRT.pitch = camera->pitch + camera->dpitch;
    gCameraSRT.roll = camera->roll;
    gCameraSRT.transl.x = -x;
    gCameraSRT.transl.y = -y;
    if (UINT_800a6a54 != 0) {
        gCameraSRT.transl.y -= camera->dty;
    }
    gCameraSRT.transl.z = -z;

    matrix_from_srt_reversed(&gViewMtx, &gCameraSRT);
    matrix_concat(&gViewMtx, &gProjectionMtx, &gViewProjMtx);
    matrix_f2l(&gViewProjMtx, *rspMtxs);

    gRSPMtxList = *rspMtxs;

    gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL((*rspMtxs)++), G_MTX_PROJECTION | G_MTX_LOAD);

    gCameraSRT.yaw = -0x8000 - camera->yaw;
    gCameraSRT.pitch = -(camera->pitch + camera->dpitch);
    gCameraSRT.roll = -camera->roll;
    gCameraSRT.scale = 1.0f;
    gCameraSRT.transl.x = x;
    gCameraSRT.transl.y = y;
    if (UINT_800a6a54 != 0) {
        gCameraSRT.transl.y += camera->dty;
    }
    gCameraSRT.transl.z = z;
    
    matrix_from_srt(&gViewMtx2, &gCameraSRT);
    matrix_f2l(&gViewMtx2, &gRSPViewMtx2);

    gCameraSelector = prevCameraSel;

    i = 0;
    while (i < 30) {
        gRSPMatrices[i++] = 0;
    }
}

void func_800029C4(f32 param1) {
    D_8008C788.m[1][1] = param1;
}

void func_800029D4(Gfx** gdl, Mtx** rspMtxs) {
    u32 wh;
    s32 i;
    s32 width;
    s32 height;

    wh = vi_get_current_size();
    width = GET_VIDEO_WIDTH(wh);
    height = GET_VIDEO_HEIGHT(wh);
    gRSPViewports[gCameraSelector + 5].vp.vscale[0] = width * 2;
    gRSPViewports[gCameraSelector + 5].vp.vscale[1] = width * 2;
    gRSPViewports[gCameraSelector + 5].vp.vtrans[0] = width * 2;
    gRSPViewports[gCameraSelector + 5].vp.vtrans[1] = height * 2;
    gSPViewport((*gdl)++, OS_PHYSICAL_TO_K0(&gRSPViewports[gCameraSelector] + 5));
    for (i = 0; i < 16; i++) { ((f32*)&gViewProjMtx)[i] = ((f32*)&D_8008C788)[i]; }
    matrix_f2l(&gViewProjMtx, *rspMtxs);
    gRSPMtxList = *rspMtxs;
    gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL((*rspMtxs)++), G_MTX_PROJECTION | G_MTX_LOAD);
    for (i = 0; i < 30; i++) { gRSPMatrices[i] = NULL; }
}

void func_80002B2C(Gfx** gdl, Mtx** rspMtxs) {
    s32 i;

    matrix_from_srt_reversed(&gAuxMtx, D_8008C758);
    matrix_concat(&gAuxMtx, &gProjectionMtx, &gViewProjMtx);
    matrix_f2l(&gViewProjMtx, *rspMtxs);
    gRSPMtxList = *rspMtxs;
    gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL((*rspMtxs)++), G_MTX_PROJECTION | G_MTX_LOAD);
    for (i = 0; i < 30; i++) { gRSPMatrices[i] = NULL; }
}

void func_80002C0C(Gfx **gdl, s32 scaleX, s32 scaleY, s32 transX, s32 transY)
{
    if (0) {} // fakematch

    if (!(gViewports[gCameraSelector].flags & 0x1))
    {
        gRSPViewports[gCameraSelector].vp.vtrans[0] = transX << 2;
        gRSPViewports[gCameraSelector].vp.vtrans[1] = transY << 2;
        gRSPViewports[gCameraSelector].vp.vscale[0] = scaleX << 2;
        gRSPViewports[gCameraSelector].vp.vscale[1] = scaleY << 2;
        gSPViewport((*gdl)++, OS_K0_TO_PHYSICAL(&gRSPViewports[gCameraSelector]));
    }
    else
    {
        gSPViewport((*gdl)++, OS_K0_TO_PHYSICAL(&gRSPViewports[gCameraSelector + 5 * gCameraGroupSelector + 10]));
    }
}

void func_80002D14(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz)
{
    f32 nrm = x * gViewProjMtx.m[0][3] + y * gViewProjMtx.m[1][3] + z * gViewProjMtx.m[2][3] + gViewProjMtx.m[3][3];
    vec3_transform(&gViewProjMtx, x, y, z, ox, oy, oz);
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
    vec3_transform(&gViewMtx2, x, y, z, ox, oy, oz);
}

void func_80002E94(f32 x, f32 y, f32 z, s32 *ox, s32 *oy, s32 *oz)
{
    if (ox != NULL) {
        x *= (gRSPViewports[0].vp.vscale[0] >> 2);
        x += (gRSPViewports[0].vp.vtrans[0] >> 2);
        *ox = x;
    }
    if (oy != NULL) {
        y *= (gRSPViewports[0].vp.vscale[1] >> 2);
        y += (gRSPViewports[0].vp.vtrans[1] >> 2);
        *oy = 240 - (s32)(y);
    }
    if (oz != NULL) {
        z *= (gRSPViewports[0].vp.vscale[2] << 5);
        z += (gRSPViewports[0].vp.vtrans[2] << 5);
        *oz = z;
    }
}

void func_80002F88(s32 x, s32 y, s32 z, f32 *ox, f32 *oy, f32 *oz)
{
    x -= (gRSPViewports[0].vp.vtrans[0] >> 2);
    *ox = (f32)(x) / (gRSPViewports[0].vp.vscale[0] >> 2);

    y = 240 - y;
    y -= (gRSPViewports[0].vp.vtrans[1] >> 2);
    *oy = (f32)(y) / (gRSPViewports[0].vp.vscale[1] >> 2);

    z -= (gRSPViewports[0].vp.vtrans[2] << 5);
    *oz = (f32)(z) / (gRSPViewports[0].vp.vscale[2] << 5);
}

void func_8000302C(Gfx **gdl)
{
    u32 wh;
    u32 width;
    u32 height;

    gCameraSelector = 4;

    wh = vi_get_current_size();
    height = GET_VIDEO_HEIGHT(wh);
    width = GET_VIDEO_WIDTH(wh);

    if (!(gViewports[gCameraSelector].flags & 0x1))
    {
        gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, 0, 0, width - 1, height - 1);
        func_80002C0C(gdl, width / 2, height / 2, width / 2, height / 2);
    }
    else
    {
        func_80002490(gdl);
        func_80002C0C(gdl, 0, 0, 0, 0);
    }

    gCameraSelector = 0;
}

void func_80003168(Gfx **gdl, Mtx **rspMtxs, s32 x, s32 y, s32 z, f32 scale)
{
    MtxF mf;

    matrix_translation(&MtxF_800a6a60, x * 640.0f, y, z * 640.0f);
    if (scale != 1.0f) {
        matrix_scaling(&mf, scale, scale, scale);
        matrix_concat(&mf, &MtxF_800a6a60, &MtxF_800a6a60);
    }
    matrix_f2l_4x3(&MtxF_800a6a60, *rspMtxs);

    gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL((*rspMtxs)++), G_MTX_LOAD);
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

void func_800032C4(Gfx **gdl, Mtx **rspMtxs, SRT *param3, f32 param4, f32 param5, MtxF *param6) {
    MtxF *mtx;
    Mtx *mtx2;

    mtx = param6 == NULL ? &MtxF_800a6a60 : param6;

    param3->transl.x -= gWorldX;
    param3->transl.z -= gWorldZ;

    if (param3->transl.x > 32767.0f || -32767.0f > param3->transl.x || param3->transl.z > 32767.0f || -32767.0f > param3->transl.z) {
        mtx2 = get_some_model_view_mtx();
        gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL(mtx2), G_MTX_LOAD | G_MTX_MODELVIEW);

        param3->transl.x += gWorldX;
        param3->transl.z += gWorldZ;
    } else {
        matrix_from_srt(mtx, param3);
        if (param4 != 1.0f) {
            matrix_prescale_y(mtx, param4);
        }

        if (param6 == NULL) {
            matrix_f2l_4x3(mtx, *rspMtxs);
            gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL((*rspMtxs)++), G_MTX_LOAD | G_MTX_MODELVIEW);
        } else {
            // @bug: param6 is a float matrix? shouldn't this be converted to a long matrix?
            gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL(param6), G_MTX_LOAD | G_MTX_MODELVIEW);
            add_matrix_to_pool(param6, 1);
        }

        param3->transl.x += gWorldX;
        param3->transl.z += gWorldZ;
    }
}

void func_800034CC(f32 x, f32 y, f32 z)
{
    gCameras[gCameraSelector].srt.transl.x += x;
    gCameras[gCameraSelector].srt.transl.y += y;
    gCameras[gCameraSelector].srt.transl.z += z;
    gCameras[gCameraSelector].unk56 = func_8004454C(
        gCameras[gCameraSelector].srt.transl.x, 
        gCameras[gCameraSelector].srt.transl.y, 
        gCameras[gCameraSelector].srt.transl.z);
}

void func_8000356C(f32 a, f32 b, f32 c)
{
    gCameras[gCameraSelector].srt.transl.x -= a * fcos16_precise(gCameras[gCameraSelector].srt.yaw);
    gCameras[gCameraSelector].srt.transl.z -= a * fsin16_precise(gCameras[gCameraSelector].srt.yaw);
    gCameras[gCameraSelector].srt.transl.x -= c * fsin16_precise(gCameras[gCameraSelector].srt.yaw);
    gCameras[gCameraSelector].srt.transl.z += c * fcos16_precise(gCameras[gCameraSelector].srt.yaw);
    gCameras[gCameraSelector].unk56 = func_8004454C(
        gCameras[gCameraSelector].srt.transl.x, 
        gCameras[gCameraSelector].srt.transl.y, 
        gCameras[gCameraSelector].srt.transl.z);
}

void get_vec3_to_camera_normalized(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz)
{
    f32 nrm;
    Camera *camera = &gCameras[gCameraSelector];

    *ox = camera->srt.transl.x - x;
    *oy = camera->srt.transl.y - y;
    *oz = camera->srt.transl.z - z;

    nrm = sqrtf(*ox * *ox + *oy * *oy + *oz * *oz);
    if (nrm != 0.0f)
    {
        nrm = 1.0f / nrm;
        *ox *= nrm;
        *oy *= nrm;
        *oz *= nrm;
    }
}

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

MtxF *camera_get_view_mtx2()
{
    return &gViewMtx2;
}

MtxF *camera_get_viewproj_mtx()
{
    return &gViewProjMtx;
}

MtxF *camera_get_projection_mtx()
{
    return &gProjectionMtx;
}

Mtx *camera_get_rsp_projection_mtx()
{
    return &gRSPProjectionMtx;
}

MtxF *camera_get_view_mtx()
{
    return &gViewMtx;
}

u32 func_800038DC(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, u8 param_6)
{
    u32 ret;
    Vp *rspvp;
    f32 vx;
    f32 vy;
    f32 vz;
    f32 x1;
    f32 y1;
    
    ret = 0;

    if (param_6) {
        vec3_transform(&gViewMtx, x, y, z, &x, &y, &z);
    }

    vx = gProjectionMtx.m[0][0] * x;
    vy = gProjectionMtx.m[1][1] * y;
    vz = gProjectionMtx.m[2][3] * z;
    vz = -vz;

    if (vz < -2.0f)
    {

        rspvp = &gRSPViewports[gCameraSelector];
        if (gViewports[gCameraSelector].flags & 0x1) {
            rspvp += gCameraGroupSelector * 5 + 10;
        }

        x1 = rspvp->vp.vscale[0] >> 2;
        y1 = rspvp->vp.vscale[1] >> 2;

        *ox = (rspvp->vp.vtrans[0] >> 2) - ((vx * x1) / vz);
        *oy = (rspvp->vp.vtrans[1] >> 2) + ((vy * y1) / vz);

        ret = 1;
    }

    return ret;
}

// gets angle to camera z-vector
f32 camera_get_angle_to_point(f32 x, f32 y, f32 z) {
    return 
        x * gViewMtx.m[0][2] +
        y * gViewMtx.m[1][2] +
        z * gViewMtx.m[2][2] +
        gViewMtx.m[3][2];
}

void func_80003AA0(f32 x, f32 y, f32 z, f32 distance, f32 param_5)
{
    s32 i;

    for (i = 0; i < 8; i++)
    {
        f32 dx = x - gCameras[i].srt.transl.x;
        f32 dy = y - gCameras[i].srt.transl.y;
        f32 dz = z - gCameras[i].srt.transl.z;
        f32 d = sqrtf(dx * dx + dy * dy + dz * dz);
        if (d < distance) {
            gCameras[i].unk5D = 0;
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
        gCameras[i].unk5D = 0;
    }
}

void func_80003bb0(f32 param_1, f32 param_2, f32 param_3)
{
    s32 i;

    for (i = 0; i < CAMERA_COUNT; i++)
    {
        gCameras[i].dty = param_1;
        gCameras[i].unk30 = param_1;
        gCameras[i].unk34 = param_2;
        gCameras[i].unk38 = 0.0f;
        gCameras[i].unk3C = param_3;
        gCameras[i].unk5D = 1;
    }
}

// unused
void func_80003C48(s32 a0) {
    s32 i = 0;
    while (i < 16) {
        i++;
    }
}

// unused
void func_80003C68(s32 a0) {
    s32 i = 0;
    while (i < 16) {
        i++;
    }
}

void add_matrix_to_pool(MtxF *mf, s32 count)
{
    gMatrixPool[gMatrixCount].mtx = (Mtx_MtxF*)mf;
    gMatrixPool[gMatrixCount++].count = count;
}

void camera_tick() {
    s32 pad;
    f32 var5;
    Camera *camera;
    f32 var4;

    SHORT_8008c524 = SHORT_8008c528;

    if (D_8008C518 != 0) {
        D_8008C518 -= gUpdateRate;

        if (D_8008C518 < 0) {
            D_8008C518 = 0;
        }

        var5 = ((f32)D_8008C518 / (f32)D_8008C51C);
        gFarPlane = (D_800A6270 - D_800A6274) * var5 + D_800A6274;
    }

    gMatrixPool[gMatrixCount].count = -1;

    convert_mtxf_to_mtx_in_pool(gMatrixPool);

    gMatrixCount = 0;
    gMatrixIndex = 0;

    if (gUseAlternateCamera) {
        gCameraSelector += 4;
    }
    
    camera = &gCameras[gCameraSelector];

    if (camera->unk5D == 0) {
        camera->unk5C--;

        while (camera->unk5C < 0) {
            camera->dty = -camera->dty * 0.89999998f;

            camera->unk5C++;
        }
    } else if (camera->unk5D == 1) {
        var4 = fexp(-camera->unk3C * camera->unk38, 20);
        var5 = fcos16_precise(camera->unk34 * 65535.0f * camera->unk38);
        var5 *= camera->unk30 * var4;

        camera->dty = var5;

        if (camera->dty < 0.1f && -0.1f < camera->dty) {
            camera->unk5D = -1;
            camera->dty = 0.0f;
        }

        camera->unk38 += gUpdateRateF / 60.0f;
    }
}

// Computes e to the power of x
f32 fexp(f32 x, u32 iterations)
{
    f32 y = 1.0f;
    f32 n = 1.0f;
    f32 xp = x;
    f32 yp = 1.0f;

    for (; iterations != 0; iterations--)
    {
        y += xp / yp;
        n += 1.0f;
        xp *= x;
        yp *= n;
    }

    return y;
}

// compute viewProjection matrix for object's matrix idx
void setup_rsp_matrices_for_object(Gfx **gdl, Mtx **rspMtxs, Object *object)
{
    u8 isChild;
    MtxF mtxf;
    Object *origObject;
    f32 oldScale;

    origObject = object;

    if (gRSPMatrices[object->matrixIdx] == NULL)
    {
        isChild = FALSE;

        while (object != NULL)
        {
            if (object->parent == NULL) {
                object->srt.transl.x -= gWorldX;
                object->srt.transl.z -= gWorldZ;
            }

            oldScale = object->srt.scale;
            if (!(object->unkB0 & 0x8)) {
                object->srt.scale = 1.0f;
            }

            if (!isChild) {
                matrix_from_srt(&MtxF_800a6a60, &object->srt);
            } else {
                matrix_from_srt(&mtxf, &object->srt);
                matrix_concat_4x3(&MtxF_800a6a60, &mtxf, &MtxF_800a6a60);
            }

            object->srt.scale = oldScale;

            if (object->parent == NULL) {
                object->srt.transl.x += gWorldX;
                object->srt.transl.z += gWorldZ;
            }

            object = object->parent;
            isChild = TRUE;
        }

        matrix_concat(&MtxF_800a6a60, &gViewProjMtx, &gAuxMtx2);
        matrix_f2l(&gAuxMtx2, *rspMtxs);
        gRSPMatrices[origObject->matrixIdx] = *rspMtxs;
        (*rspMtxs)++;
    }

    gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL(gRSPMatrices[origObject->matrixIdx]), G_MTX_PROJECTION | G_MTX_LOAD);
}

// load parent projection matrix
void func_80004224(Gfx **gdl)
{
    gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL(gRSPMtxList), G_MTX_PROJECTION | G_MTX_LOAD);
}

s32 func_80004258(Object *object)
{
    func_800042C8(object, gMatrixIndex);
    gRSPMatrices[gMatrixIndex] = NULL;
    gMatrixIndex++;

    if (gMatrixIndex) {}

    return gMatrixIndex - 1;
}

void func_800042A8(Object *object)
{
    func_800042C8(object, object->matrixIdx);
}

void func_800042C8(Object *object, int matrixIdx)
{
    s8 isChild;
    f32 oldScale;
    s8 objectCount;
    SRT invsrt;
    Object* objectList[5]; // how many?

    isChild = FALSE;
    // @fake
    if (isChild){}
    objectCount = 0;

    while (object != NULL)
    {
        objectList[objectCount] = object;
        objectCount++;

        oldScale = object->srt.scale;
        object->srt.scale = 1.0f;

        if (!isChild) {
            matrix_from_srt((MtxF *) (((f32 *) gObjectMatrices) + (matrixIdx << 4)), &object->srt);
        } else {
            matrix_from_srt(&gAuxMtx, &object->srt);
            matrix_concat((MtxF *)((f32 *)gObjectMatrices + (matrixIdx << 4)), &gAuxMtx, (MtxF *)((f32 *)gObjectMatrices + (matrixIdx << 4)));
        }

        object->srt.scale = oldScale;
        object = object->parent;

        isChild = TRUE;
    }

    while (objectCount > 0)
    {
        objectCount--;
        object = objectList[objectCount];

        // @fake
        if (gObjectMatrices) {}
        invsrt.transl.x = -object->srt.transl.x;
        invsrt.transl.y = -object->srt.transl.y;
        invsrt.transl.z = -object->srt.transl.z;
        invsrt.scale = 1.0f;
        invsrt.yaw = -object->srt.yaw;
        invsrt.pitch = -object->srt.pitch;
        invsrt.roll = -object->srt.roll;
        matrix_from_srt_reversed((MtxF *) ((f32*)gInverseObjectMatrices + (matrixIdx << 4)), &invsrt);
    }
}

void get_object_child_position(Object *object, f32 *ox, f32 *oy, f32 *oz)
{
    Object *parent = object->parent;

    if (NULL == parent)
    {
        *ox = object->srt.transl.x;
        *oy = object->srt.transl.y;
        *oz = object->srt.transl.z;
        return;
    }

    vec3_transform((MtxF *)((f32 *)gObjectMatrices + (object->parent->matrixIdx << 4)), object->srt.transl.x, object->srt.transl.y, object->srt.transl.z, ox, oy, oz);
}

void transform_point_by_object(float x, float y, float z, float *ox, float *oy, float *oz, Object *object)
{
    if (object != NULL)
    {
        vec3_transform((MtxF *) ((f32*)gObjectMatrices + (object->matrixIdx << 4)), x, y, z, ox, oy, oz);
    }
    else
    {
        *ox = x;
        *oy = y;
        *oz = z;
    }
}

void inverse_transform_point_by_object(float x, float y, float z, float *ox, float *oy, float *oz, Object *object)
{
    if (object != NULL)
    {
        vec3_transform((MtxF *) ((f32*)gInverseObjectMatrices + (object->matrixIdx << 4)), x, y, z, ox, oy, oz);
    }
    else
    {
        *ox = x;
        *oy = y;
        *oz = z;
    }
}

void inverse_rotate_point_by_object(f32 x, f32 y, f32 z, f32* ox, f32* oy, f32* oz, Object* object) {
    Vec3f v;

    v.x = x;
    v.y = y;
    v.z = z;
    vec3_transform_no_translate((MtxF *) ((f32*)gInverseObjectMatrices + (object->matrixIdx << 4)), &v, &v);
    *ox = v.x;
    *oy = v.y;
    *oz = v.z;
}

void rotate_point_by_object(f32 x, f32 y, f32 z, f32* ox, f32* oy, f32* oz, Object* object) {
    Vec3f  point;
    u8 mtxId;

    point.x = x;
    point.y = y;
    point.z = z;
    vec3_transform_no_translate((MtxF *)((f32 *)gObjectMatrices + (object->matrixIdx << 4)), &point, &point);
    *ox = point.x;
    *oy = point.y;
    *oz = point.z;
}

void transform_srt_by_object(SRT* srt, SRT* out, Object* object) {
    if (object == NULL) {
        out->transl.x = srt->transl.x;
        out->transl.y = srt->transl.y;
        out->transl.z = srt->transl.z;
        out->yaw = srt->yaw;
        out->pitch = srt->pitch;
        out->roll = srt->roll;
        return;
    }
    vec3_transform((MtxF *) ((f32 *)gObjectMatrices + (object->matrixIdx << 4)), srt->transl.x, srt->transl.y, srt->transl.z, &out->transl.x, &out->transl.y, &out->transl.z);
    out->yaw = object->srt.yaw + srt->yaw;
    out->pitch = srt->pitch;
    out->roll = srt->roll;
}

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

    vec3_transform(&mf, 
        a->transl.x, a->transl.y, a->transl.z, 
        &out->transl.x, &out->transl.y, &out->transl.z);

    yaw = a->yaw - b->yaw;
    CIRCLE_WRAP(yaw)
    out->yaw = yaw;
    out->pitch = a->pitch;
    out->roll = a->roll;
}

void update_camera_for_object(Camera *camera)
{
    Object *obj;
    Object *obj2;
    f32 x,y,z;

    if (camera->object == NULL)
    {
        camera->tx = camera->srt.transl.x;
        camera->ty = camera->srt.transl.y;
        camera->tz = camera->srt.transl.z;
        camera->yaw = camera->srt.yaw;
        camera->pitch = camera->srt.pitch;
        camera->roll = camera->srt.roll;
    }
    else
    {
        x = camera->srt.transl.x;
        y = camera->srt.transl.y;
        z = camera->srt.transl.z;

        obj = camera->object;
        vec3_transform((MtxF*) ((f32 *)gObjectMatrices + (obj->matrixIdx << 4)), x,y,z, &camera->tx, &camera->ty, &camera->tz);
        
        camera->yaw = camera->srt.yaw - obj->srt.yaw;
        camera->pitch = camera->srt.pitch;
        camera->roll = camera->srt.roll;
    }
}

void transform_point_by_object_matrix(Vec3f *v, Vec3f *ov, s8 matrixIdx){
    u32 noTransformation = matrixIdx < 0; 

    if (matrixIdx >= 30) {
        STUBBED_PRINTF("Cam: Worldmtx overflow!!!\n");
    }; 

    if (noTransformation) {
        ov->x = v->x; 
        ov->y = v->y; 
        ov->z = v->z; 
        return; 
    } 

    vec3_transform(&gObjectMatrices[matrixIdx], v->x, v->y, v->z, &ov->x, &ov->y, &ov->z);
}

void func_80004A30(s16 param1) {
    SHORT_8008c528 = param1;
}

s16 func_80004A4C() {
    return SHORT_8008c528;
}
