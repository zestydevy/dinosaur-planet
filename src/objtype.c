#include "game/objects/object.h"
#include "sys/math.h"
#include "constants.h"

#define OBJECT_TYPE_LIST_LENGTH 256
#define OBJECT_MAX_TYPES 65

extern s16 gObjectTypeIndices[OBJECT_MAX_TYPES + 1];
extern s16 gObjectTypeListCount;
extern Object *gObjectTypeList[OBJECT_TYPE_LIST_LENGTH];

static const char str_80099b00[] = "objAddObjectType: obj romdefno %d, type %d\n";
static const char str_80099b2c[] = "objAddObjectType: Reached MAXTYPES!!\n";
static const char str_80099b54[] = "objFreeObjectType: obj romdefno %d, type %d\n";
static const char str_80099b84[] = "\nError Freeing Object\n";
static const char str_80099b9c[] = "objGetNearestType: type %d, from obj romdefno %d, maxdist %f, object found %x\n";
static const char str_80099bec[] = "objGetNearestType: type %d, from obj romdefno %d, maxdist %f, object found %x\n";

void obj_object_type_init() {
    bzero(gObjectTypeIndices, sizeof(gObjectTypeIndices));
    gObjectTypeListCount = 0;
}

// regalloc
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/objtype/obj_add_object_type.s")
#else
void obj_add_object_type(Object *obj, s32 type) {
    s32 listStart;
    s32 listEnd;
    s32 listEnd2;
    s32 i;
    s32 b;

    if (type < 0 || type >= 65) {
        return;
    }

    if (gObjectTypeListCount >= 256) {
        return;
    }

    listStart = gObjectTypeIndices[type];
    listEnd = gObjectTypeIndices[type + 1];
    listEnd2 = listEnd;

    for (i = listStart; i < listEnd2; i++) {
        if (gObjectTypeList[i] == obj) {
            return;
        }
    }

    if (listEnd == listStart) {
        i = listStart;
    } else {
        i = listEnd - 1;
    }

    gObjectTypeListCount++;

    listEnd2 = gObjectTypeListCount - 1;
    for (b = listEnd2; b > i; b--) {
        gObjectTypeList[b] = gObjectTypeList[b - 1];
    }

    gObjectTypeList[i] = obj;

    i = type + 1;
    for (b = i; b < 66; b++) {
        gObjectTypeIndices[b] += 1;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/objtype/obj_free_object_type.s")

Object **obj_get_all_of_type(s32 idx, s32 *count) {
    if (idx < 0 || idx >= OBJECT_MAX_TYPES) {
        *count = 0;
        return NULL;
    }

    *count = gObjectTypeIndices[idx + 1] - gObjectTypeIndices[idx];
    return &gObjectTypeList[gObjectTypeIndices[idx]];
}

Object *obj_get_nearest_type_to(s32 type, Object *object, float *distance) {
    f32 minDistSquared;
    Object *result;
    s32 i;
    s32 iend;

    result = NULL;

    if (type < 0 || type >= OBJECT_MAX_TYPES) {
        return NULL;
    }

    if (distance != NULL) {
        minDistSquared = *distance * *distance;
    } else {
        minDistSquared = F32_MAX;
    }

    i = gObjectTypeIndices[type];
    iend = gObjectTypeIndices[type + 1];
    while (i < iend)
    {
        f32 distSquared = vec3_distance_squared(&object->positionMirror, &gObjectTypeList[i]->positionMirror);
        if (distSquared < minDistSquared) {
            minDistSquared = distSquared;
            result = gObjectTypeList[i];
        }

        i++;
    }

    if (result != NULL && distance != NULL) {
        *distance = sqrtf(minDistSquared);
    }

    return result;
}

Object *obj_get_nearest_type_to_excluding_self(s32 type, Object *object, float *distance) {
    f32 minDistSquared;
    s32 i;
    s32 iend;
    Object *result;

    result = NULL;

    if (type < 0 || type >= OBJECT_MAX_TYPES) {
        return NULL;
    }

    if (distance != NULL) {
        minDistSquared = *distance * *distance;
    } else {
        minDistSquared = F32_MAX;
    }

    i = gObjectTypeIndices[type];
    iend = gObjectTypeIndices[type + 1];
    while (i < iend)
    {
        if (object != gObjectTypeList[i])
        {
            f32 distSquared = vec3_distance_squared(&object->positionMirror, &gObjectTypeList[i]->positionMirror);
            if (distSquared < minDistSquared) {
                minDistSquared = distSquared;
                result = gObjectTypeList[i];
            }
        }

        i++;
    }

    if (result != NULL && distance != NULL) {
        *distance = sqrtf(minDistSquared);
    }

    return result;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/objtype/obj_get_nearest_type.s")
#else
Object* obj_get_nearest_type(s32 type, Vec3f* location, f32* distance) {
    Object *result;
    s32 i;
    s32 iend;
    Vec3f position;
    f32 minDistSquared;
    f32 distSquared;

    result = NULL;

    minDistSquared = (*distance) * (*distance);
    
    if (type < 0 || type >= OBJECT_MAX_TYPES) {
        return NULL;
    }
    
    i = gObjectTypeIndices[type];
    iend = gObjectTypeIndices[type + 1];

    while (i < iend)
    {
        if (gObjectTypeList[i] != NULL){
            f32 dx = location->x - gObjectTypeList[i]->positionMirror.x;
            f32 dy = location->y - gObjectTypeList[i]->positionMirror.y;
            f32 dz = location->z - gObjectTypeList[i]->positionMirror.z;
            
            distSquared = dx*dx + dy*dy + dz*dz;
            
            if (distSquared < minDistSquared) {
                minDistSquared = distSquared;
                result = gObjectTypeList[i];
            }
        }
        i++;
    }

    if (result != NULL) {
        *distance = sqrtf(minDistSquared);
    }
    
    return result;
}
#endif


s32 obj_is_object_type(Object *obj, s32 type) {
    s32 i;
    s32 iend;
    s32 ret;
    
    if (type < 0 || type >= OBJECT_MAX_TYPES) {
        return FALSE;
    }

    i = gObjectTypeIndices[type];
    iend = gObjectTypeIndices[type + 1];

    ret = i < iend;

    if (ret) {
        while (obj != gObjectTypeList[i])
        {
            i++;

            ret = i < iend;
            if (!ret) {
                break;
            }
        }
    }

    return ret;
}
