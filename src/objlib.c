#include "common.h"

#define ACTOR_LIST_LENGTH 256
#define ACTOR_LIST_INDICES_LENGTH 65

// Appears to be a list of actors that are grouped into "categories" via gActorListIndices.
extern TActor *gActorList[ACTOR_LIST_LENGTH];
extern s16 gActorCount;
extern s16 gActorListIndices[ACTOR_LIST_INDICES_LENGTH + 1];

void func_80030EC0()
{
    bzero(gActorListIndices, sizeof(gActorListIndices));
    gActorCount = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/objlib/add_object_to_array.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objlib/func_80031080.s")

TActor **TActor_getter(s32 idx, s32 *count)
{
    if (idx < 0 || idx >= ACTOR_LIST_INDICES_LENGTH) {
        *count = 0;
        return NULL;
    }

    *count = gActorListIndices[idx + 1] - gActorListIndices[idx];
    return &gActorList[gActorListIndices[idx]];
}

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/objlib/func_80031238.s")
#else
#define F32_MAX_FINITE (*(f32*)0x80099c3c)
float vec3_distance_squared(Vec3f *v0, Vec3f *v1);
TActor *_func_80031238(s32 idx, TActor *actor, float *distance)
{
    f32 minDistSquared;
    TActor *result;
    s32 i;
    s32 iend;

    result = NULL;

    if (idx < 0 || idx >= ACTOR_LIST_INDICES_LENGTH) {
        return NULL;
    }

    if (distance != NULL) {
        minDistSquared = *distance * *distance;
    } else {
        minDistSquared = F32_MAX_FINITE;
    }

    i = gActorListIndices[idx];
    iend = gActorListIndices[idx + 1];
    while (i < iend)
    {
        f32 distSquared = vec3_distance_squared(&actor->positionMirror, &gActorList[i]->positionMirror);
        if (distSquared < minDistSquared) {
            minDistSquared = distSquared;
            result = gActorList[i];
        }

        i++;
    }

    if (result != NULL && distance != NULL) {
        *distance = sqrtf(minDistSquared);
    }

    return result;
}
#endif

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/objlib/func_80031350.s")
#else
#define F32_MAX_FINITE (*(f32*)0x80099c40)
float vec3_distance_squared(Vec3f *v0, Vec3f *v1);
TActor *_func_80031350(s32 idx, TActor *actor, float *distance)
{
    f32 minDistSquared;
    s32 i;
    s32 iend;
    TActor *result;

    result = NULL;

    if (idx < 0 || idx >= ACTOR_LIST_INDICES_LENGTH) {
        return NULL;
    }

    if (distance != NULL) {
        minDistSquared = *distance * *distance;
    } else {
        minDistSquared = F32_MAX_FINITE;
    }

    i = gActorListIndices[idx];
    iend = gActorListIndices[idx + 1];
    while (i < iend)
    {
        if (actor != gActorList[i])
        {
            f32 distSquared = vec3_distance_squared(&actor->positionMirror, &gActorList[i]->positionMirror);
            if (distSquared < minDistSquared) {
                minDistSquared = distSquared;
                result = gActorList[i];
            }
        }

        i++;
    }

    if (result != NULL && distance != NULL) {
        *distance = sqrtf(minDistSquared);
    }

    return result;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/objlib/func_80031470.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objlib/func_8003159C.s")
