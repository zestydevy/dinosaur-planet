#include "common.h"

#define OBJECT_LIST_LENGTH 256
#define OBJECT_LIST_INDICES_LENGTH 65

// Appears to be a list of objects that are grouped into "categories" via gObjectListIndices.
extern Object *gObjectList2[OBJECT_LIST_LENGTH];
extern s16 gObjectCount;
extern s16 gObjectListIndices[OBJECT_LIST_INDICES_LENGTH + 1];

void func_80030EC0()
{
    bzero(gObjectListIndices, sizeof(gObjectListIndices));
    gObjectCount = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/objlib/add_object_to_array.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objlib/func_80031080.s")

Object **object_getter(s32 idx, s32 *count)
{
    if (idx < 0 || idx >= OBJECT_LIST_INDICES_LENGTH) {
        *count = 0;
        return NULL;
    }

    *count = gObjectListIndices[idx + 1] - gObjectListIndices[idx];
    return &gObjectList2[gObjectListIndices[idx]];
}

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/objlib/func_80031238.s")
#else
#define F32_MAX_FINITE (*(f32*)0x80099c3c)
Object *_func_80031238(s32 idx, Object *object, float *distance)
{
    f32 minDistSquared;
    Object *result;
    s32 i;
    s32 iend;

    result = NULL;

    if (idx < 0 || idx >= OBJECT_LIST_INDICES_LENGTH) {
        return NULL;
    }

    if (distance != NULL) {
        minDistSquared = *distance * *distance;
    } else {
        minDistSquared = F32_MAX_FINITE;
    }

    i = gObjectListIndices[idx];
    iend = gObjectListIndices[idx + 1];
    while (i < iend)
    {
        f32 distSquared = vec3_distance_squared(&object->positionMirror, &gObjectList2[i]->positionMirror);
        if (distSquared < minDistSquared) {
            minDistSquared = distSquared;
            result = gObjectList2[i];
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
Object *_func_80031350(s32 idx, Object *object, float *distance)
{
    f32 minDistSquared;
    s32 i;
    s32 iend;
    Object *result;

    result = NULL;

    if (idx < 0 || idx >= OBJECT_LIST_INDICES_LENGTH) {
        return NULL;
    }

    if (distance != NULL) {
        minDistSquared = *distance * *distance;
    } else {
        minDistSquared = F32_MAX_FINITE;
    }

    i = gObjectListIndices[idx];
    iend = gObjectListIndices[idx + 1];
    while (i < iend)
    {
        if (object != gObjectList2[i])
        {
            f32 distSquared = vec3_distance_squared(&object->positionMirror, &gObjectList2[i]->positionMirror);
            if (distSquared < minDistSquared) {
                minDistSquared = distSquared;
                result = gObjectList2[i];
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
