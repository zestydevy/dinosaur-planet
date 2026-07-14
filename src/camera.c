#include "sys/camera.h"

#include "PR/gbi.h"
#include "libultra/io/piint.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/vi.h"
#include "sys/gfx/model_asm.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/map.h"

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

/* -------- .bss start 800a6270 -------- */
f32 gFarPlaneStart;
f32 gFarPlaneTarget;
Camera gCameras[CAMERA_COUNT];
u32 gViewportMode;
s32 gCameraSelector;
s32 gMaxCameras;
SRT gCameraSRT;
f32 gFovY;
f32 gAspect;
s8 gTriggerUseAlternateCamera;
s8 gUseAlternateCamera;
MatrixSlot gMatrixPool[100];
u32 gMatrixCount;
u32 gCameraYOffsetEnabled;
u16 gPerspNorm;
MtxF MtxF_800a6a60;
MtxF gViewProjMtx;
Mtx *gRSPMtxList; // FIXME: not a list
MtxF gProjectionMtx;
MtxF gViewMtx;
MtxF gViewMtx2;
Mtx gRSPProjectionMtx;
Mtx gRSPViewMtx2;
MtxF gAuxMtx;
MtxF gAuxMtx2;
MtxF gObjectMatrices[30];
MtxF gInverseObjectMatrices[30];
Mtx *gRSPMatrices[30];
s8 gMatrixIndex;
/* -------- .bss end 800a7c30 -------- */

// The viewport z-range below is half of the max (511)
#define G_HALFZ (G_MAXZ / 2) /* 9 bits of integer screen-Z precision */

// x1, y1, x2, y2
// posX, posY, width, height
// scissorX1, scissorY1, scissorX2, scissorY2
// flags
#define DEFAULT_VIEWPORT \
    0, 0, 320, 240, \
    160, 120, 320, 240, \
    0, 0, 320 - 1, 240 - 1, \
    0

/* -------- .data start -------- */
f32 gNearPlane = 4.0f;
f32 gFarPlane = 10000.0f;
s16 gFarPlaneTimer = 0;
s16 gFarPlaneDuration = 0;
s8 gAntiPiracyFlag = 0; // gAntiPiracyViewport?
s16 gLetterboxSize = 0;
s16 gLetterboxTarget = 0;
f32 gProjectionScaleX = 1.0f;
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
SRT gStaticCameraSRT[2] = {
    { 0, 0, 0, 0, 1.0f, VEC3F(0.0f, 0.0f, -281.0f) },
    { 0, 0, 0, 0, 1.0f, VEC3F(0.0f, 0.0f, 0.0f) }
};
MtxF g2DProjectionMtx = {
    {
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 160.0f },
    }
};
/* -------- .data end -------- */

// ROM address
extern u32 D_B0000578;

f32 cam_fexp(f32 x, u32 iterations);

// official name: camInit
void camInit(void) {
    s32 i;
    u32 stat;

    for (i = 0; i < 12; i++) {
        gCameraSelector = i;
        camReset(200, 200, 200, 0, 0, 180);
    }

    gTriggerUseAlternateCamera = 0;
    gUseAlternateCamera = 0;
    gCameraSelector = 0;
    gViewportMode = 0;
    gCameraYOffsetEnabled = 0;
    gMatrixCount = 0;
    gMatrixIndex = 0;
    gFarPlane = 10000.0f;
    gFarPlaneTimer = 0;
    gAntiPiracyFlag = 0;

    WAIT_ON_IOBUSY(stat);

#ifndef NON_MATCHING
    // 0xB0000578 is a direct read from the ROM as opposed to RAM
    if (((D_B0000578 & 0xFFFF) & 0xFFFF) != 0x8965) {
        gAntiPiracyFlag = TRUE;
    }
#endif

    gAspect = 1.3333334f;
    gFovY = 60.0f;

    guPerspectiveF(gProjectionMtx.m, &gPerspNorm, gFovY, gAspect, gNearPlane, gFarPlane, 1.0f);
    matrix_f2l(&gProjectionMtx, &gRSPProjectionMtx);

    gProjectionScaleX = gProjectionMtx.m[0][0];
    gLetterboxSize = 0;
    gLetterboxTarget = 0;
}

// official name: camUseShake
void camUseShake(void) {
    gCameraYOffsetEnabled = 1;
}

// official name: camIgnoreShake
void camIgnoreShake(void) {
    gCameraYOffsetEnabled = 0;
}

u32 camIsUsingShake(void) {
    return gCameraYOffsetEnabled;
}

f32 camGetFOV(void) {
    return gFovY;
}

// official name: camSetFOV
void camSetFOV(f32 fovY) {
    if (fovY > 90.0f) {
        fovY = 90.0f;
    } else if (fovY < 40.0f) {
        fovY = 40.0f;
    }

    gFovY = fovY;

    guPerspectiveF(gProjectionMtx.m, &gPerspNorm, gFovY, gAspect, gNearPlane, gFarPlane, 1.0f);
    matrix_f2l(&gProjectionMtx, &gRSPProjectionMtx);
    gProjectionScaleX = gProjectionMtx.m[0][0];
}

void camSetOrthoProjectionMatrix(f32 l, f32 r, f32 b, f32 t) {
    guOrthoF(gProjectionMtx.m, l, r, b, t, 0.0f, 400.0f, 20.0f);
}

void camSetAspect(f32 aspect) {
    gAspect = aspect;
    guPerspectiveF(gProjectionMtx.m, &gPerspNorm, gFovY, gAspect, gNearPlane, gFarPlane, 1.0f);
    matrix_f2l(&gProjectionMtx, &gRSPProjectionMtx);
    gProjectionScaleX = gProjectionMtx.m[0][0];
    gProjectionMtx.m[0][3] *= 0.5f;
}

void camSetNearPlane(f32 near) {
    gNearPlane = near;
    guPerspectiveF(gProjectionMtx.m, &gPerspNorm, gFovY, gAspect, gNearPlane, gFarPlane, 1.0f);
    matrix_f2l(&gProjectionMtx, &gRSPProjectionMtx);
    gProjectionScaleX = gProjectionMtx.m[0][0];
    gProjectionMtx.m[0][3] *= 0.5f;
}

void camSetFarPlaneLerp(f32 farPlane, s32 duration) {
    if (duration != 0) {
        gFarPlaneDuration = duration;
        gFarPlaneTimer = gFarPlaneDuration;
        gFarPlaneStart = gFarPlane;
        gFarPlaneTarget = farPlane;
    } else {
        gFarPlane = farPlane;
    }
}

f32 camGetFarPlane(void) {
    return gFarPlane;
}

void camResetNearPlane(void) {
    camSetNearPlane(4.0f);
}

void camResetFarPlane(void) {
    camSetFarPlaneLerp(10000.0f, 60);
}

void camResetProjection(void) {
    guPerspectiveF(gProjectionMtx.m, &gPerspNorm, 60.0f, 4.0f / 3.0f, gNearPlane, gFarPlane, 1.0f);
    matrix_f2l(&gProjectionMtx, &gRSPProjectionMtx);
}

MtxF *camGetAuxMtx(void) {
    return &gAuxMtx2;
}

u32 camGetViewportMode(void) {
    return gViewportMode;
}

u32 camGetCameraSelector(void) {
    return gCameraSelector;
}

void camSetupSkybox(Gfx **gdl, Mtx **rspMtxs) {
    Camera *camera;
    s16 yaw;
    s16 pitch;
    s16 roll;
    s16 dpitch;
    f32 posX;
    f32 posY;
    f32 posZ;

    camSetViewportMode(0);
    camSetCameraSelector(0);

    camera = camGet();
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

    camSetupViewportAndMatrices(gdl, rspMtxs);

    camera->dpitch = dpitch;
    camera->srt.yaw = yaw;
    camera->srt.pitch = pitch;
    camera->srt.roll = roll;
    camera->srt.transl.x = posX;
    camera->srt.transl.y = posY;
    camera->srt.transl.z = posZ;
}

// official name: camDistance
f32 camDistance(f32 x, f32 y, f32 z) {
    s32 cameraIdx;
    f32 dz;
    f32 dx;
    f32 dy;

    cameraIdx = gCameraSelector;

    if (gUseAlternateCamera) {
        cameraIdx += 4;
    }

    dz = (z - gCameras[cameraIdx].srt.transl.z);\
    dy = (x - gCameras[cameraIdx].srt.transl.x);\
    dx = (y - gCameras[cameraIdx].srt.transl.y);
    
    return sqrtf((dz * dz) + ((dy * dy) + (dx * dx)));
}

void camReset(s32 tx, s32 ty, s32 tz, s32 roll, s32 pitch, s32 yaw) {
    gCameras[gCameraSelector].srt.transl.x = tx;
    gCameras[gCameraSelector].srt.transl.y = ty;
    gCameras[gCameraSelector].srt.transl.z = tz;
    gCameras[gCameraSelector].fov = 60.0f;
    gCameras[gCameraSelector].srt.roll = roll * 0xb6;
    gCameras[gCameraSelector].srt.pitch = pitch * 0xb6;
    gCameras[gCameraSelector].srt.yaw = yaw * 0xb6;
    gCameras[gCameraSelector].velX = 0.0f;
    gCameras[gCameraSelector].velY = 0.0f;
    gCameras[gCameraSelector].velZ = 0.0f;
    gCameras[gCameraSelector].dty = 0.0f;
    gCameras[gCameraSelector].object = NULL;
    gCameras[gCameraSelector].dpitch = 0;
}

void camSetAlternate(s32 selector, f32 x, f32 y, f32 z, s16 yaw, s16 pitch, s16 roll) {
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

s8 camIsAlternateActive(void) {
    return gUseAlternateCamera;
}

void camApplyAlternateTrigger(void) {
    gUseAlternateCamera = gTriggerUseAlternateCamera;
    gTriggerUseAlternateCamera = 0;
}

// set_active_viewports_and_max?
s32 camSetViewportMode(s32 num) {
    if (num >= 0 && num < 4) {
        gViewportMode = num;
    } else {
        gViewportMode = 0;
    }

    switch (gViewportMode) {
        case 0:
            gMaxCameras = 1;
            break;
        case 1:
            gMaxCameras = 2;
            break;
        case 2:
            gMaxCameras = 3;
            break;
        case 3:
            gMaxCameras = 4;
            break;
    }

    if (gMaxCameras <= gCameraSelector) {
        gCameraSelector = 0;
    }

    return gMaxCameras;
}

void camSetCameraSelector(s32 selector) {
    if (selector >= 0 && selector < 4) {
        gCameraSelector = selector;
    } else {
        // "Camera Error: Illegal player no!\n"
        gCameraSelector = 0;
    }
}

void camUpdateRSPViewports(void) {
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

void camViewportEnable(s32 selector, u32 immediate) {
    if (immediate != 0) {
        gViewports[selector].flags |= 0x1;
    } else {
        gViewports[selector].flags |= 0x2;
    }
    gViewports[selector].flags &= ~0x4;
}

void camViewportDisable(s32 selector, u32 immediate) {
    if (immediate != 0) {
        gViewports[selector].flags &= ~0x1;
    } else {
        gViewports[selector].flags |= 0x4;
    }
    gViewports[selector].flags &= ~0x2;
}

u32 camViewportIsActive(s32 selector) {
    return gViewports[selector].flags & 0x1;
}

void camViewportSetScissorRect(s32 viewPortIndex, s32 x1, s32 y1, s32 x2, s32 y2) {
    s32 resolution;
    s32 width;
    s32 height;
    s32 temp;

    resolution = viGetCurrentSize();
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

void camViewportSetCustomTransform(s32 viewPortIndex, s32 posX, s32 posY, s32 width, s32 height) {
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

u32 camViewportGetScissorRect(s32 selector, s32 *ulx, s32 *uly, s32 *lrx, s32 *lry) {
    *ulx = gViewports[selector].ulx;
    *lrx = gViewports[selector].lrx;
    *uly = gViewports[selector].uly;
    *lry = gViewports[selector].lry;

    if ((*ulx | *lrx | *uly | *lry) == 0) {
        return 0;
    }

    return 1;
}

void camViewportGetRect(s32 selector, s32 *x1, s32 *y1, s32 *x2, s32 *y2) {
    *x1 = gViewports[selector].x1;
    *y1 = gViewports[selector].y1;
    *x2 = gViewports[selector].x2;
    *y2 = gViewports[selector].y2;
}

void camViewportGetFullRect(s32 *ulx, s32 *uly, s32 *lrx, s32 *lry) {
    u32 wh = viGetCurrentSize();
    u32 width = GET_VIDEO_WIDTH(wh);
    u32 height = GET_VIDEO_HEIGHT(wh);

    *ulx = 0;
    *lrx = width;
    *uly = gLetterboxSize + 6;
    *lry = height - gLetterboxSize - 6;
}

void camSetupViewportAndMatrices(Gfx **gdl, Mtx **rspMtxs) {
    u32 wh;
    u32 mode;
    u32 x;
    u32 y;
    u32 scaleX;
    u32 posY;
    u32 posX;
    u32 width;
    u32 height;
    u32 scaleY;
    s32 origCamSelector;
    s32 savedCamSelector;
    s32 tempCamSelector;

    origCamSelector = gCameraSelector;
    savedCamSelector = gCameraSelector;
    wh = viGetCurrentSize();
    height = GET_VIDEO_HEIGHT(wh);
    width = GET_VIDEO_WIDTH(wh);

    if (gViewports[savedCamSelector].flags & 0x1)
    {
        tempCamSelector = gCameraSelector;
        gCameraSelector = savedCamSelector;
        gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, gViewports[gCameraSelector].ulx, gViewports[gCameraSelector].uly, gViewports[gCameraSelector].lrx, gViewports[gCameraSelector].lry);
        camApplyRSPViewport(gdl, 0, 0, 0, 0);
        gCameraSelector = tempCamSelector;
        if (rspMtxs != NULL) {
            camSetupRSPMatrices(gdl, rspMtxs);
        }
        gCameraSelector = origCamSelector;
        return;
    }

    mode = gViewportMode;
    if (mode == 2) {
        mode = 3;
    }

    scaleX = x = width >> 1;
    scaleY = y = height >> 1;
    if (osTvType == OS_TV_PAL) {
        scaleY = 145;
    }

    switch (mode)
    {
    case 0:
        posY = scaleY;
        posX = x;
        if (osTvType == OS_TV_PAL) {
            posY -= 18;
        }
        break;
    case 1:
        posX = x;
        posY = gCameraSelector;
        if (posY == 0) {
            posY = height >> 2;
            if (osTvType == OS_TV_PAL) {
                posY -= 12;
            }
        } else {
            posY = y;
            posY += height >> 2;
        }
        break;
    case 2:
        posY = scaleY;
        if (gCameraSelector == 0) {
            posX = width >> 2;
        }  else {
            posX = x;
            posX += width >> 2;
        }
        break;
    case 3:
        scaleY >>= 1;
        scaleX >>= 1;
        posX = scaleX;
        posY = scaleY;
        if (osTvType == OS_TV_PAL) {
            if (gCameraSelector < 2) {
                posY -= 20;
            } else {
                posY -= 6;
            }
        }
        break;
    }

    if (osTvType == OS_TV_PAL) {
        posX -= 4;
    }

    camApplyRSPViewport(gdl, scaleX, scaleY, posX, posY);
    if (rspMtxs != NULL) {
        camSetupRSPMatrices(gdl, rspMtxs);
    }
    camApplyScissor(gdl);

    gCameraSelector = origCamSelector;
}

void camApplyScissor(Gfx **gdl) {
    s32 ulx, uly, lrx, lry;
    s32 wh;
    s32 centerX;
    s32 centerY;
    s32 width;
    s32 height;
    s32 padX;
    s32 padY;
    s32 mode;
    
    wh = viGetCurrentSize();
    height = GET_VIDEO_HEIGHT(wh) & 0xFFFF;
    width = GET_VIDEO_WIDTH(wh);
    
    mode = gViewportMode;
    
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

        uly += gLetterboxSize + 6;
        lry -= gLetterboxSize + 6;
    }

    gDPSetScissor((*gdl)++, 0, ulx, uly, lrx, lry);
}

void camSetupRSPMatrices(Gfx **gdl, Mtx **rspMtxs) {
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

    camUpdateCameraForObject(camera);

    if (gCameraSelector == 4) {
        map_func_80046B58(camera->tx, camera->ty, camera->tz);
    }

    x = camera->tx - gWorldX;
    y = camera->ty;
    z = camera->tz - gWorldZ;

    if (x > 32767.0f || -32767.0f > x || z > 32767.0f || -32767.0f > z) {
        // "Camera out of range: %d, (%.1f,%.1f,%.1f) (%.1f,%.1f)\n"
        return;
    }
    
    gCameraSRT.yaw = 0x8000 + camera->yaw;
    gCameraSRT.pitch = camera->pitch + camera->dpitch;
    gCameraSRT.roll = camera->roll;
    gCameraSRT.transl.x = -x;
    gCameraSRT.transl.y = -y;
    if (gCameraYOffsetEnabled != 0) {
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
    if (gCameraYOffsetEnabled != 0) {
        gCameraSRT.transl.y += camera->dty;
    }
    gCameraSRT.transl.z = z;
    
    matrix_from_srt(&gViewMtx2, &gCameraSRT);
    matrix_f2l(&gViewMtx2, &gRSPViewMtx2);

    gCameraSelector = prevCameraSel;

    i = 0;
    while (i < 30) {
        gRSPMatrices[i++] = NULL;
    }
}

void camSet2DScaleY(f32 scaleY) {
    g2DProjectionMtx.m[1][1] = scaleY;
}

void camSetup2DProjection(Gfx** gdl, Mtx** rspMtxs) {
    u32 wh;
    s32 i;
    s32 width;
    s32 height;

    wh = viGetCurrentSize();
    width = GET_VIDEO_WIDTH(wh);
    height = GET_VIDEO_HEIGHT(wh);
    gRSPViewports[gCameraSelector + 5].vp.vscale[0] = width * 2;
    gRSPViewports[gCameraSelector + 5].vp.vscale[1] = width * 2;
    gRSPViewports[gCameraSelector + 5].vp.vtrans[0] = width * 2;
    gRSPViewports[gCameraSelector + 5].vp.vtrans[1] = height * 2;
    gSPViewport((*gdl)++, OS_PHYSICAL_TO_K0(&gRSPViewports[gCameraSelector] + 5));
    for (i = 0; i < 16; i++) { ((f32*)&gViewProjMtx)[i] = ((f32*)&g2DProjectionMtx)[i]; }
    matrix_f2l(&gViewProjMtx, *rspMtxs);
    gRSPMtxList = *rspMtxs;
    gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL((*rspMtxs)++), G_MTX_PROJECTION | G_MTX_LOAD);
    for (i = 0; i < 30; i++) { gRSPMatrices[i] = NULL; }
}

void camSetupStaticProjection(Gfx** gdl, Mtx** rspMtxs) {
    s32 i;

    matrix_from_srt_reversed(&gAuxMtx, gStaticCameraSRT);
    matrix_concat(&gAuxMtx, &gProjectionMtx, &gViewProjMtx);
    matrix_f2l(&gViewProjMtx, *rspMtxs);
    gRSPMtxList = *rspMtxs;
    gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL((*rspMtxs)++), G_MTX_PROJECTION | G_MTX_LOAD);
    for (i = 0; i < 30; i++) { gRSPMatrices[i] = NULL; }
}

void camApplyRSPViewport(Gfx **gdl, s32 scaleX, s32 scaleY, s32 transX, s32 transY) {
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

void camProjectPoint(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz) {
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

void camUnprojectPoint(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz) {
    f32 dot = x * gViewProjMtx.m[0][3] + y * gViewProjMtx.m[1][3] + z * gViewProjMtx.m[2][3] + gViewProjMtx.m[3][3];
    if (dot != 0.0f)
    {
        x *= dot;
        y *= dot;
        z *= dot;
    }
    vec3_transform(&gViewMtx2, x, y, z, ox, oy, oz);
}

void camClipToScreen(f32 x, f32 y, f32 z, s32 *ox, s32 *oy, s32 *oz) {
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

void camScreenToClip(s32 x, s32 y, s32 z, f32 *ox, f32 *oy, f32 *oz) {
    x -= (gRSPViewports[0].vp.vtrans[0] >> 2);
    *ox = (f32)(x) / (gRSPViewports[0].vp.vscale[0] >> 2);

    y = 240 - y;
    y -= (gRSPViewports[0].vp.vtrans[1] >> 2);
    *oy = (f32)(y) / (gRSPViewports[0].vp.vscale[1] >> 2);

    z -= (gRSPViewports[0].vp.vtrans[2] << 5);
    *oz = (f32)(z) / (gRSPViewports[0].vp.vscale[2] << 5);
}

void camSetupFullscreenViewport(Gfx **gdl) {
    u32 wh;
    u32 width;
    u32 height;

    gCameraSelector = 4;

    wh = viGetCurrentSize();
    height = GET_VIDEO_HEIGHT(wh);
    width = GET_VIDEO_WIDTH(wh);

    if (!(gViewports[gCameraSelector].flags & 0x1))
    {
        gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, 0, 0, width - 1, height - 1);
        camApplyRSPViewport(gdl, width / 2, height / 2, width / 2, height / 2);
    }
    else
    {
        camApplyScissor(gdl);
        camApplyRSPViewport(gdl, 0, 0, 0, 0);
    }

    gCameraSelector = 0;
}

void camSetupWorldMatrix(Gfx **gdl, Mtx **rspMtxs, s32 x, s32 y, s32 z, f32 scale) {
    MtxF mf;

    matrix_translation(&MtxF_800a6a60, x * 640.0f, y, z * 640.0f);
    if (scale != 1.0f) {
        matrix_scaling(&mf, scale, scale, scale);
        matrix_concat(&mf, &MtxF_800a6a60, &MtxF_800a6a60);
    }
    matrix_f2l_4x3(&MtxF_800a6a60, *rspMtxs);

    gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL((*rspMtxs)++), G_MTX_MODELVIEW | G_MTX_LOAD);
}

// TODO: where did this function name come from?
s32 camCheckConvexHull(Gfx **gdl, Mtx **rspMtxs, s32 a2, Object *obj, ModelInstance *modelInst, s32 a5, s32 a6) {
    return 1;
}

void camStub3294(u32 a0, u32 a1, u32 a2, u32 a4) { }

void camStub32AC(u32 a0, u32 a1, u32 a2, u32 a4) { }

void camSetupObjectSRTMatrix(Gfx **gdl, Mtx **rspMtxs, SRT *srt, f32 yScale, f32 unused, MtxF *outMtx) {
    MtxF *mtx;
    Mtx *fallbackMtx;

    mtx = outMtx == NULL ? &MtxF_800a6a60 : outMtx;

    srt->transl.x -= gWorldX;
    srt->transl.z -= gWorldZ;

    if (srt->transl.x > 32767.0f || -32767.0f > srt->transl.x || srt->transl.z > 32767.0f || -32767.0f > srt->transl.z) {
        fallbackMtx = get_some_model_view_mtx();
        gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL(fallbackMtx), G_MTX_LOAD | G_MTX_MODELVIEW);

        srt->transl.x += gWorldX;
        srt->transl.z += gWorldZ;
    } else {
        matrix_from_srt(mtx, srt);
        if (yScale != 1.0f) {
            matrix_prescale_y(mtx, yScale);
        }

        if (outMtx == NULL) {
            matrix_f2l_4x3(mtx, *rspMtxs);
            gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL((*rspMtxs)++), G_MTX_LOAD | G_MTX_MODELVIEW);
        } else {
            // Output matrix was given, so don't add matrix to the global rsp matrix list but add to
            // the matrix pool so it still gets converted to the long format later before the display
            // list is processed.
            gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL(outMtx), G_MTX_LOAD | G_MTX_MODELVIEW);
            camAddMatrixToPool(outMtx, 1);
        }

        srt->transl.x += gWorldX;
        srt->transl.z += gWorldZ;
    }
}

void camTranslate(f32 x, f32 y, f32 z) {
    gCameras[gCameraSelector].srt.transl.x += x;
    gCameras[gCameraSelector].srt.transl.y += y;
    gCameras[gCameraSelector].srt.transl.z += z;
    gCameras[gCameraSelector].blockIndex = mapWorldCoordsToBlockIndex(
        gCameras[gCameraSelector].srt.transl.x, 
        gCameras[gCameraSelector].srt.transl.y, 
        gCameras[gCameraSelector].srt.transl.z);
}

void camTranslateRelative(f32 forward, f32 b, f32 strafe) {
    gCameras[gCameraSelector].srt.transl.x -= forward * fcos16_precise(gCameras[gCameraSelector].srt.yaw);
    gCameras[gCameraSelector].srt.transl.z -= forward * fsin16_precise(gCameras[gCameraSelector].srt.yaw);
    gCameras[gCameraSelector].srt.transl.x -= strafe * fsin16_precise(gCameras[gCameraSelector].srt.yaw);
    gCameras[gCameraSelector].srt.transl.z += strafe * fcos16_precise(gCameras[gCameraSelector].srt.yaw);
    gCameras[gCameraSelector].blockIndex = mapWorldCoordsToBlockIndex(
        gCameras[gCameraSelector].srt.transl.x, 
        gCameras[gCameraSelector].srt.transl.y, 
        gCameras[gCameraSelector].srt.transl.z);
}

void camGetVec3ToCameraNormalized(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz) {
    f32 nrm;
    Camera *camera = &gCameras[gCameraSelector];

    *ox = camera->srt.transl.x - x;
    *oy = camera->srt.transl.y - y;
    *oz = camera->srt.transl.z - z;

    nrm = sqrtf(SQ(*ox) + SQ(*oy) + SQ(*oz));
    if (nrm != 0.0f)
    {
        nrm = 1.0f / nrm;
        *ox *= nrm;
        *oy *= nrm;
        *oz *= nrm;
    }
}

// Unused
void camRotate(s32 yaw, s32 pitch, s32 roll) {
    gCameras[gCameraSelector].srt.yaw += yaw;
    gCameras[gCameraSelector].srt.pitch += pitch;
    gCameras[gCameraSelector].srt.roll += roll;
}

Camera *camGetMain(void) {
    return &gCameras[gCameraSelector];
}

Camera *camGet(void) {
    if (gUseAlternateCamera) {
        return &gCameras[gCameraSelector + 4];
    }

    return &gCameras[gCameraSelector];
}

// TODO: rename
Camera *camGetCameraArray(void) {
    if (gUseAlternateCamera) {
        return &gCameras[4];
    }

    return gCameras;
}

MtxF *camGetViewMtx2(void) {
    return &gViewMtx2;
}

MtxF *camGetViewprojMtx(void) {
    return &gViewProjMtx;
}

MtxF *camGetProjectionMtx(void) {
    return &gProjectionMtx;
}

Mtx *camGetRSPProjectionMtx(void) {
    return &gRSPProjectionMtx;
}

MtxF *camGetViewMtx(void) {
    return &gViewMtx;
}

u32 camWorldToScreen(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, u8 useViewMtx) {
    u32 ret;
    Vp *rspvp;
    f32 vx;
    f32 vy;
    f32 vz;
    f32 x1;
    f32 y1;
    
    ret = 0;

    if (useViewMtx) {
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
f32 camGetAngleToPoint(f32 x, f32 y, f32 z) {
    return 
        x * gViewMtx.m[0][2] +
        y * gViewMtx.m[1][2] +
        z * gViewMtx.m[2][2] +
        gViewMtx.m[3][2];
}

void camApplyShakeRadial(f32 x, f32 y, f32 z, f32 radius, f32 intensity) {
    s32 i;

    for (i = 0; i < 8; i++)
    {
        f32 dx = x - gCameras[i].srt.transl.x;
        f32 dy = y - gCameras[i].srt.transl.y;
        f32 dz = z - gCameras[i].srt.transl.z;
        f32 d = sqrtf(dx * dx + dy * dy + dz * dz);
        if (d < radius) {
            gCameras[i].shakeMode = 0;
            gCameras[i].dty = (radius - d) * intensity / radius;
        }
    }
}

void camSetShakeOffset(f32 dty) {
    s32 i;

    for (i = 0; i < CAMERA_COUNT; i++)
    {
        gCameras[i].dty = dty;
        gCameras[i].shakeMode = 0;
    }
}

void camStartShakeDampened(f32 amplitude, f32 frequency, f32 damping) {
    s32 i;

    for (i = 0; i < CAMERA_COUNT; i++)
    {
        gCameras[i].dty = amplitude;
        gCameras[i].shakeAmplitude = amplitude;
        gCameras[i].shakeFrequency = frequency;
        gCameras[i].shakeTime = 0.0f;
        gCameras[i].shakeDamping = damping;
        gCameras[i].shakeMode = 1;
    }
}

// unused
void camStubLoop16(s32 a0) {
    s32 i = 0;
    while (i < 16) {
        i++;
    }
}

// unused
void camStubLoop16b(s32 a0) {
    s32 i = 0;
    while (i < 16) {
        i++;
    }
}

void camAddMatrixToPool(MtxF *mf, s32 count) {
    gMatrixPool[gMatrixCount].mtx = (Mtx_MtxF*)mf;
    gMatrixPool[gMatrixCount++].count = count;
}

// official name: camTick
void camTick(void) {
    s32 pad;
    f32 lerpFactor;
    Camera *camera;
    f32 dampFactor;

    gLetterboxSize = gLetterboxTarget;

    if (gFarPlaneTimer != 0) {
        gFarPlaneTimer -= gUpdateRate;

        if (gFarPlaneTimer < 0) {
            gFarPlaneTimer = 0;
        }

        lerpFactor = ((f32)gFarPlaneTimer / (f32)gFarPlaneDuration);
        gFarPlane = (gFarPlaneStart - gFarPlaneTarget) * lerpFactor + gFarPlaneTarget;
    }

    gMatrixPool[gMatrixCount].count = -1;

    convert_mtxf_to_mtx_in_pool(gMatrixPool);

    gMatrixCount = 0;
    gMatrixIndex = 0;

    if (gUseAlternateCamera) {
        gCameraSelector += 4;
    }
    
    camera = &gCameras[gCameraSelector];

    if (camera->shakeMode == 0) {
        camera->shakeCooldown--;

        while (camera->shakeCooldown < 0) {
            camera->dty = -camera->dty * 0.89999998f;

            camera->shakeCooldown++;
        }
    } else if (camera->shakeMode == 1) {
        dampFactor = cam_fexp(-camera->shakeDamping * camera->shakeTime, 20);
        lerpFactor = fcos16_precise(camera->shakeFrequency * 65535.0f * camera->shakeTime);
        lerpFactor *= camera->shakeAmplitude * dampFactor;

        camera->dty = lerpFactor;

        if (camera->dty < 0.1f && -0.1f < camera->dty) {
            camera->shakeMode = -1;
            camera->dty = 0.0f;
        }

        camera->shakeTime += gUpdateRateF / 60.0f;
    }
}

// Computes e to the power of x
f32 cam_fexp(f32 x, u32 iterations) {
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
void camSetupRSPMatricesForObject(Gfx **gdl, Mtx **rspMtxs, Object *object) {
    u8 ancestor;
    MtxF mtxf;
    Object *origObject;
    f32 oldScale;

    origObject = object;

    if (gRSPMatrices[object->matrixIdx] == NULL) {
        ancestor = FALSE;
        while (object != NULL) {
            if (object->parent == NULL) {
                object->srt.transl.x -= gWorldX;
                object->srt.transl.z -= gWorldZ;
            }

            oldScale = object->srt.scale;
            if (!(object->stateFlags & OBJSTATE_WORLD_MTX_IGNORE_SCALE)) {
                object->srt.scale = 1.0f;
            }

            if (!ancestor) {
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
            ancestor = TRUE;
        }

        matrix_concat(&MtxF_800a6a60, &gViewProjMtx, &gAuxMtx2);
        matrix_f2l(&gAuxMtx2, *rspMtxs);
        gRSPMatrices[origObject->matrixIdx] = *rspMtxs;
        (*rspMtxs)++;
    }

    gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL(gRSPMatrices[origObject->matrixIdx]), G_MTX_PROJECTION | G_MTX_LOAD);
}

// load parent projection matrix
void camLoadParentProjection(Gfx **gdl) {
    gSPMatrix((*gdl)++, OS_K0_TO_PHYSICAL(gRSPMtxList), G_MTX_PROJECTION | G_MTX_LOAD);
}

s32 camAllocObjectMatrix(Object *object) {
    camBuildObjectMatrix(object, gMatrixIndex);
    gRSPMatrices[gMatrixIndex] = NULL;
    gMatrixIndex++;

    if (gMatrixIndex > 30) {
        STUBBED_PRINTF("Cam: Worldmtx overflow!!!\n");
    }

    return gMatrixIndex - 1;
}

void camUpdateObjectMatrix(Object *object) {
    camBuildObjectMatrix(object, object->matrixIdx);
}

void camBuildObjectMatrix(Object *object, int matrixIdx) {
    s8 ancestor;
    f32 oldScale;
    s8 objectCount;
    SRT invsrt;
    Object* objectList[5]; // how many?

    ancestor = FALSE;
    // @fake
    if (ancestor){}
    objectCount = 0;

    while (object != NULL) {
        objectList[objectCount] = object;
        objectCount++;

        oldScale = object->srt.scale;
        object->srt.scale = 1.0f;

        if (!ancestor) {
            matrix_from_srt((MtxF *) (((f32 *) gObjectMatrices) + (matrixIdx << 4)), &object->srt);
        } else {
            matrix_from_srt(&gAuxMtx, &object->srt);
            matrix_concat((MtxF *)((f32 *)gObjectMatrices + (matrixIdx << 4)), &gAuxMtx, (MtxF *)((f32 *)gObjectMatrices + (matrixIdx << 4)));
        }

        object->srt.scale = oldScale;
        object = object->parent;

        ancestor = TRUE;
    }

    while (objectCount > 0)
    {
        objectCount--;
        object = objectList[objectCount];

        // @fake
        PRAGMA_IGNORE_PUSH("-Waddress")
        if (gObjectMatrices) {}
        PRAGMA_IGNORE_POP()
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

void camGetObjectChildPosition(Object *object, f32 *ox, f32 *oy, f32 *oz) {
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

void camTransformPointByObject(float x, float y, float z, float *ox, float *oy, float *oz, Object *object) {
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

void camInverseTransformPointByObject(float x, float y, float z, float *ox, float *oy, float *oz, Object *object) {
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

void camInverseRotatePointByObject(f32 x, f32 y, f32 z, f32* ox, f32* oy, f32* oz, Object* object) {
    Vec3f v;

    v.x = x;
    v.y = y;
    v.z = z;
    vec3_transform_no_translate((MtxF *) ((f32*)gInverseObjectMatrices + (object->matrixIdx << 4)), &v, &v);
    *ox = v.x;
    *oy = v.y;
    *oz = v.z;
}

void camRotatePointByObject(f32 x, f32 y, f32 z, f32* ox, f32* oy, f32* oz, Object* object) {
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

void camTransformSRTByObject(SRT* srt, SRT* out, Object* object) {
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

void camTransformSRTRelative(SRT *a, SRT *b, SRT *out) {
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

void camUpdateCameraForObject(Camera *camera) {
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

void camTransformPointByObjectMatrix(Vec3f *v, Vec3f *ov, s8 matrixIdx){
    u32 noTransformation = matrixIdx < 0; 

    if (matrixIdx) {} // @fake

    if (noTransformation) {
        ov->x = v->x; 
        ov->y = v->y; 
        ov->z = v->z; 
        return; 
    } 

    vec3_transform(&gObjectMatrices[matrixIdx], v->x, v->y, v->z, &ov->x, &ov->y, &ov->z);
}

void camSetLetterbox(s16 size) {
    gLetterboxTarget = size;
}

s16 camGetLetterbox(void) {
    return gLetterboxTarget;
}
