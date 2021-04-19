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
/*0040*/    u8 unk_0x40[0x5c - 0x40];
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

extern MatrixSlot gMatrixPool[2]; // FIXME: how many?
extern u32 gMatrixCount;
extern Camera gCameras[CAMERA_COUNT];
extern s8 gUseAlternateCamera;
extern u32 gCameraSelector;
extern MtxF gActorMatrices[2]; // FIXME: how many items are there?
extern MtxF gInverseActorMatrices[2]; // FIXME: how many items are there?

void vec3_transform_no_translate(MtxF *mf, Vec3f *v, Vec3f *ov);
void matrix_from_srt_reversed(MtxF *mf, SRT *srt);

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001220.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800013BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800013D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/MusPtrBankGetCurrent.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800013F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001400.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/ortho_some_matrix.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001520.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800015C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001660.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800016A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800016B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800016E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001708.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/__CallBackDmaNew.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001788.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001798.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800017A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001884.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001914.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800019BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001A2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001A3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001A5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80001B18.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800026C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800029C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800029D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80002B2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80002C0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80002D14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80002DE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80002E94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80002F88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_8000302C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003168.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003278.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003294.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800032AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800032C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800034CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_8000356C.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_8000388C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_8000389C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800038AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800038BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800038CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800038DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003A60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003AA0.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80004224.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80004258.s")

void func_800042C8(TActor *actor, int matrixIdx);
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

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800048D4.s")

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
