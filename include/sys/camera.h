#ifndef _SYS_CAMERA_H
#define _SYS_CAMERA_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/math.h"

#define CAMERA_COUNT 12

typedef struct {
/*0000*/    SRT srt;
/*0018*/    f32 fov;
/*001C*/    u32 unk1C;
/*0020*/    f32 velX;
/*0024*/    f32 velY;
/*0028*/    f32 velZ;
/*002C*/    f32 dty;           // vertical shake offset
/*0030*/    f32 shakeAmplitude;
/*0034*/    f32 shakeFrequency;
/*0038*/    f32 shakeTime;
/*003C*/    f32 shakeDamping;
/*0040*/    Object *object;    // attached object
/*0044*/    f32 tx;            // resolved world X
/*0048*/    f32 ty;            // resolved world Y
/*004C*/    f32 tz;            // resolved world Z
/*0050*/    s16 yaw;           // resolved yaw
/*0052*/    s16 pitch;         // resolved pitch
/*0054*/    s16 roll;          // resolved roll
/*0056*/    u16 blockIndex;
/*0058*/    u16 unk58;
/*005A*/    s16 dpitch;        // pitch delta (shake)
/*005C*/    s8 shakeCooldown;
/*005D*/    s8 shakeMode;      // 0 = bounce, 1 = dampened oscillation, -1 = inactive
} Camera;

typedef union
{
    Mtx m;
    MtxF mf;
} Mtx_MtxF;

typedef struct {
/*0000*/    Mtx_MtxF *mtx;
/*0004*/    s16 count;
/*0006*/    u8 _pad[2];
} MatrixSlot;

typedef struct {
/*0000*/    s32 x1;
/*0004*/    s32 y1;
/*0008*/    s32 x2;
/*000C*/    s32 y2;
/*0010*/    s32 posX;
/*0014*/    s32 posY;
/*0018*/    s32 width;
/*001C*/    s32 height;
/*0020*/    s32 ulx;
/*0024*/    s32 uly;
/*0028*/    s32 lrx;
/*002C*/    s32 lry;
/*0030*/    u32 flags;
} Viewport;


void camInit(void);
void camUseShake(void);
void camIgnoreShake(void);
u32 camIsUsingShake(void);
f32 camGetFOV(void);
void camSetFOV(f32 fovY);
void camSetOrthoProjectionMatrix(f32 l, f32 r, f32 b, f32 t);
void camSetAspect(f32 aspect);
void camSetNearPlane(f32 near);
void camSetFarPlaneLerp(f32 farPlane, s32 duration);
f32 camGetFarPlane(void);
void camResetNearPlane(void);
void camResetFarPlane(void);
void camResetProjection(void);
MtxF *camGetAuxMtx(void);
u32 camGetViewportMode(void);
u32 camGetCameraSelector(void);
void camSetupSkybox(Gfx **gdl, Mtx **rspMtxs);
f32 camDistance(f32 x, f32 y, f32 z);
void camReset(s32 tx, s32 ty, s32 tz, s32 roll, s32 pitch, s32 yaw);
void camSetAlternate(s32 selector, f32 x, f32 y, f32 z, s16 yaw, s16 pitch, s16 roll);
s8 camIsAlternateActive(void);
void camApplyAlternateTrigger(void);
s32 camSetViewportMode(s32 num);
void camSetCameraSelector(s32 selector);
void camUpdateRSPViewports(void);
void camViewportEnable(s32 selector, u32 immediate);
void camViewportDisable(s32 selector, u32 immediate);
u32 camViewportIsActive(s32 selector);
void camViewportSetScissorRect(s32 viewPortIndex, s32 x1, s32 y1, s32 x2, s32 y2);
void camViewportSetCustomTransform(s32 viewPortIndex, s32 posX, s32 posY, s32 width, s32 height);
u32 camViewportGetScissorRect(s32 selector, s32 *ulx, s32 *uly, s32 *lrx, s32 *lry);
void camViewportGetRect(s32 selector, s32 *x1, s32 *y1, s32 *x2, s32 *y2);
void camViewportGetFullRect(s32 *ulx, s32 *uly, s32 *lrx, s32 *lry);
void camSetupViewportAndMatrices(Gfx **gdl, Mtx **rspMtxs);
void camApplyScissor(Gfx **gdl);
void camSetupRSPMatrices(Gfx **gdl, Mtx **rspMtxs);
void camSet2DScaleY(f32 scaleY);
void camSetup2DProjection(Gfx **gdl, Mtx **rspMtxs);
void camSetupStaticProjection(Gfx **gdl, Mtx **rspMtxs);
void camApplyRSPViewport(Gfx **gdl, s32 scaleX, s32 scaleY, s32 transX, s32 transY);
void camProjectPoint(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
void camUnprojectPoint(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
void camClipToScreen(f32 x, f32 y, f32 z, s32 *ox, s32 *oy, s32 *oz);
void camScreenToClip(s32 x, s32 y, s32 z, f32 *ox, f32 *oy, f32 *oz);
void camSetupFullscreenViewport(Gfx **gdl);
void camSetupWorldMatrix(Gfx **gdl, Mtx **rspMtxs, s32 x, s32 y, s32 z, f32 scale);
s32 camCheckConvexHull(Gfx **gdl, Mtx **rspMtxs, s32 a2, Object *obj, ModelInstance *modelInst, s32 a5, s32 a6);
void camSetupObjectSRTMatrix(Gfx **gdl, Mtx **rspMtxs, SRT *srt, f32 yScale, f32 unused, MtxF *outMtx);
void camTranslate(f32 x, f32 y, f32 z);
void camTranslateRelative(f32 forward, f32 b, f32 strafe);
void camGetVec3ToCameraNormalized(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
void camRotate(s32 yaw, s32 pitch, s32 roll);
Camera *camGetMain(void);
Camera *camGet(void);
Camera *camGetCameraArray(void);
MtxF *camGetViewMtx2(void);
MtxF *camGetViewMtx2(void);
MtxF *camGetViewprojMtx(void);
MtxF *camGetProjectionMtx(void);
Mtx *camGetRSPProjectionMtx(void);
MtxF *camGetViewMtx(void);
u32 camWorldToScreen(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, u8 useViewMtx);
f32 camGetAngleToPoint(f32, f32, f32);
void camApplyShakeRadial(f32 x, f32 y, f32 z, f32 radius, f32 intensity);
void camSetShakeOffset(f32 dty);
void camStartShakeDampened(f32 amplitude, f32 frequency, f32 damping);
void camAddMatrixToPool(MtxF *mf, s32 count);
void camTick(void);
void camSetupRSPMatricesForObject(Gfx **gdl, Mtx **rspMtxs, Object *object);
void camLoadParentProjection(Gfx **gdl);
s32 camAllocObjectMatrix(Object *object);
void camUpdateObjectMatrix(Object *object);
void camBuildObjectMatrix(Object *object, int matrixIdx);
void camGetObjectChildPosition(Object *object, float *ox, float *oy, float *oz);
void camTransformPointByObject(float x, float y, float z, float *ox, float *oy, float *oz, Object *object);
void camInverseTransformPointByObject(float x, float y, float z, float *ox, float *oy, float *oz, Object *object);
void camInverseRotatePointByObject(float x, float y, float z, float *ox, float *oy, float * oz, Object *object);
void camRotatePointByObject(float x, float y, float z, float *ox, float *oy, float *oz, Object *object);
void camTransformSRTByObject(SRT *srt, SRT *out, Object *object);
void camTransformSRTRelative(SRT *a, SRT *b, SRT *out);
void camUpdateCameraForObject(Camera *camera);
void camTransformPointByObjectMatrix(Vec3f *v, Vec3f *ov, s8 matrixIdx);
void camSetLetterbox(s16 size);
s16 camGetLetterbox(void);

#endif
