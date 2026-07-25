#include "game/objects/object.h"
#include "sys/math.h"
#include "sys/objtype.h"
#include "constants.h"
#include "macros.h"

#define OBJECT_TYPE_LIST_LENGTH 256
#define OBJECT_MAX_TYPES 65

// -------- .bss start 800b28b0 -------- //
s16 gObjectTypeIndices[OBJECT_MAX_TYPES + 1];
s16 gObjectTypeListCount;
Object *gObjectTypeList[OBJECT_TYPE_LIST_LENGTH];
// -------- .bss end 800b2d40 -------- //

void objTypeInit(void) {
    bzero(gObjectTypeIndices, sizeof(gObjectTypeIndices));
    gObjectTypeListCount = 0;
}

// official name: objAddObjectType
void objAddObjectType(Object *obj, s32 type) {
    s32 start;
    s32 end;
    s32 i;

    if (type < 0 || type >= 65) {
        return;
    }

    STUBBED_PRINTF("objAddObjectType: obj romdefno %d, type %d\n", obj->id, type); // location unsure

    if (gObjectTypeListCount >= 256) {
        STUBBED_PRINTF("objAddObjectType: Reached MAXTYPES!!\n"); // location via default.dol
        return;
    }

    start = gObjectTypeIndices[type];
    end = gObjectTypeIndices[type + 1];

    for (; start < end; start++) {
        if (gObjectTypeList[start] == obj) {
            return;
        }
    }

    if (gObjectTypeIndices[type + 1] == gObjectTypeIndices[type]) {
        start = gObjectTypeIndices[type];
    } else {
        start = gObjectTypeIndices[type + 1] - 1;
    }

    gObjectTypeListCount++;

    end = gObjectTypeListCount - 1;
    for (; end > start; end--) {
        gObjectTypeList[end] = gObjectTypeList[end - 1];
    }

    gObjectTypeList[start] = obj;

    start = type + 1;
    while (start < 66) {
        gObjectTypeIndices[start] += 1;
        start++;
    }
}

void objFreeObjectType(Object* obj, s32 type) {
    s32 end;
    s32 start;
    s32 i;

    if (type < 0 || type >= 65) {
        return;
    }

    STUBBED_PRINTF("objFreeObjectType: obj romdefno %d, type %d\n", obj->id, type);

    start = gObjectTypeIndices[type];
    end = gObjectTypeIndices[type+1];
    while (start < end && gObjectTypeList[start] != obj) {
        start++;
    }

    if (start < end) {
        gObjectTypeListCount -= 1;

        while (start < gObjectTypeListCount) {
            gObjectTypeList[start] = gObjectTypeList[start + 1];
            start++;
        }

        i = type + 1;
        while (i < 66) {
            gObjectTypeIndices[i]--;
            i++;
        }
    }
}

static const char str_80099b84[] = "\nError Freeing Object\n";

Object **objGetAllOfType(s32 idx, s32 *count) {
    if (idx < 0 || idx >= OBJECT_MAX_TYPES) {
        *count = 0;
        return NULL;
    }

    *count = gObjectTypeIndices[idx + 1] - gObjectTypeIndices[idx];
    return &gObjectTypeList[gObjectTypeIndices[idx]];
}

static const char str_80099b9c[] = "objGetNearestType: type %d, from obj romdefno %d, maxdist %f, object found %x\n";
static const char str_80099bec[] = "objGetNearestType: type %d, from obj romdefno %d, maxdist %f, object found %x\n";

Object *objGetNearestTypeTo(s32 type, Object *object, float *distance) {
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
        f32 distSquared = vec3DistanceSquared(&object->globalPosition, &gObjectTypeList[i]->globalPosition);
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

Object *objGetNearestTypeToExcludingSelf(s32 type, Object *object, float *distance) {
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
            f32 distSquared = vec3DistanceSquared(&object->globalPosition, &gObjectTypeList[i]->globalPosition);
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

Object* objGetNearestType(s32 type, Vec3f* location, f32* distance) {
    Object *result;
    f32 minDistSquared;
    f32 distSquared;
    Vec3f d;
    s32 i;
    s32 iend;

    result = NULL;
    minDistSquared = (*distance) * (*distance);
    
    if (type < 0 || type >= OBJECT_MAX_TYPES) {
        return NULL;
    }
    
    i = gObjectTypeIndices[type];
    iend = gObjectTypeIndices[type + 1];
    while (i < iend) {
        if (gObjectTypeList[i] == NULL) {
            continue;
        }
        VECTOR_SUBTRACT(*location, gObjectTypeList[i]->globalPosition, d);
        
        distSquared = VECTOR_MAGNITUDE_SQUARED(d);
        if (distSquared < minDistSquared) {
            minDistSquared = distSquared;
            result = gObjectTypeList[i];
        }
        i++;
    }

    if (result != NULL) {
        *distance = sqrtf(minDistSquared);
    }
    
    return result;
}

s32 objIsObjectType(Object *obj, s32 type) {
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
