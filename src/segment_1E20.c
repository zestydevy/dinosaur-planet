#include "common.h"

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

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800036A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_8000378C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800037D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003800.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003860.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_8000388C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_8000389C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800038AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800038BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800038CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_800038DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003A60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003AA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003B70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003BB0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003C48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003C68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/pointerIntArray2_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003CBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1E20/func_80003F0C.s")

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

    tempsrt.tx = -b->tx;
    tempsrt.ty = -b->ty;
    tempsrt.tz = -b->tz;
    tempsrt.scale = 1.0f;
    tempsrt.yaw = -b->yaw;
    tempsrt.pitch = -b->pitch;
    tempsrt.roll = -b->roll;

    matrix_from_srt_reversed(&mf, &tempsrt);

    guMtxXFMF(&mf, a->tx, a->ty, a->tz, &out->tx, &out->ty, &out->tz);

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
