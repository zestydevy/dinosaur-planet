#include "common.h"
#include "dll.h"
#include "dlls/objects/210_player.h"
#include "sys/map.h"

#include "dlls/objects/281_area.h"

typedef struct ObjectPairCallback {
    Object *primary;
    Object *secondary;
    void (*callback)(Object*, Object*); // callback that takes in the primary and secondary object
} ObjectPairCallback;

// -------- .data start 80091710 -------- //
s32 sCallbackPairIndex = 0;
// -------- .data end 80091720 -------- //

// -------- .bss start 800b2d40 -------- //
ObjectPairCallback sObjectPairCallbacks[16];
// -------- .bss end 800b2e00 -------- //

/** Transforms a mapSpace point into an objectSpace point, relative to an Area object*/
void func_80031AA0(Area_Setup* area, f32* ox, f32* oy, f32* oz) {
    f32 sinYaw;
    f32 cosYaw;
    f32 sinPitch;
    f32 cosPitch;
    f32 dx;
    f32 dy;
    f32 dz;

    sinYaw = fsin16_precise(-(area->yaw << 0x8));
    cosYaw = fcos16_precise(-(area->yaw << 0x8));
    sinPitch = fsin16_precise(-(area->pitch << 0x8));
    cosPitch = fcos16_precise(-(area->pitch << 0x8));

    dx = *ox - area->base.x;
    dy = *oy - area->base.y;
    dz = *oz - area->base.z;

    *ox = (dx * cosYaw) - (dz * sinYaw);
    dz = (dx * sinYaw) + (dz * cosYaw);
    *oy = (dy * cosPitch) - (dz * sinPitch);
    *oz = (dy * sinPitch) + (dz * cosPitch);
}

u16 func_80031BBC(f32 x, f32 y, f32 z) {
    s32 pad;
    s32 out;
    Area_Setup* objsetup;
    s32 i;
    s32 j;
    MapHeader** maps;
    f32 ox;
    f32 oy;
    f32 oz;
    MapHeader *current;

    out = -1U;
    maps = func_80044A10();
    for (i = 0; i < MAP_ID_MAX; i++) {
        if (maps[i] == NULL) {
            continue;
        }

        current = maps[i];
        objsetup = current->objectInstanceFile_ptr;
        j = 0;
        while (j < current->objectInstancesFileLength) {
            if (objsetup->base.objId == OBJ_Area) {
                ox = x;
                oy = y;
                oz = z;
                func_80031AA0(objsetup, &ox, &oy, &oz);

                if (ox < 0.0f) {
                    ox = -ox;
                }
                if (oy < 0.0f) {
                    oy = -oy;
                }
                if (oz < 0.0f) {
                    oz = -oz;
                }

                //Check if in bounds of Area
                if (ox <= objsetup->halfWidth && 
                    oy <= objsetup->halfHeight && 
                    oz <= objsetup->halfDepth
                ) {
                    out = objsetup->value;
                }
            }
            j += objsetup->base.quarterSize << 2;
            objsetup = (Area_Setup*)((u32)objsetup + (objsetup->base.quarterSize << 2));
        }
    }
    return out;
}

//aim_object_at_object_get_yaw_diff?
s16 func_80031DD8(Object* objA, Object* objB, f32* distance) {
    s16 angle;
    s32 angle16;
    f32 dx;
    f32 dz;

    if ((objA == NULL) || (objB == NULL)) {
        return 0;
    }

    //Get 2D vector from objB to objA
    dx = objA->srt.transl.x - objB->srt.transl.x;
    dz = objA->srt.transl.z - objB->srt.transl.z;

    angle = arctan2_f(dx, dz);

    //(Optional) Store distance
    if (distance != NULL) {
        *distance = sqrtf(SQ(dx) + SQ(dz));
    }

    //Return angular difference needed for objA to point towards objB
    angle16 = angle - (objA->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(angle16)
    return angle16;
}

void func_80031EBC(Vec4f* arg0, Vec4f* arg1) {
    arg1->x = (arg0->w * 0.5f) + ((-0.5f * arg0->x) + (1.5f * arg0->y) + (-1.5f * arg0->z));
    arg1->y = (arg0->w * -0.5f) + (arg0->x + (-2.5f * arg0->y) + (2.0f * arg0->z));
    arg1->z = (arg0->z * 0.5f) + (-0.5f * arg0->x);
    arg1->w = arg0->y;
}

s32 func_80031F6C(Object* obj, s32 attachIdx, f32* ox, f32* oy, f32* oz, s32 useInputCoords) {
    s32 boneIdx;
    ModelInstance* modelInstance;
    MtxF* boneMtx;
    MtxF attachPointMtx;
    SRT srt;

    modelInstance = obj->modelInsts[obj->modelInstIdx];
    if (!(modelInstance->unk34 & 8)) {
        *ox = obj->srt.transl.x;
        *oy = obj->srt.transl.y;
        *oz = obj->srt.transl.z;
        return 1;
    }

    //Get the attach point's untransformed position (or use the argument coords' current location)
    boneIdx = obj->def->pAttachPoints[attachIdx].bones[obj->modelInstIdx];
    boneMtx = (MtxF *) &(((f32 **)modelInstance->matrices)[(modelInstance->unk34 & 1)][boneIdx << 4]);
    if (useInputCoords) {
        srt.transl.x = *ox;
        srt.transl.y = *oy;
        srt.transl.z = *oz;
        srt.yaw = 0;
        srt.pitch = 0;
        srt.roll = 0;
    } else {
        srt.transl.x = obj->def->pAttachPoints[attachIdx].pos.x;
        srt.transl.y = obj->def->pAttachPoints[attachIdx].pos.y;
        srt.transl.z = obj->def->pAttachPoints[attachIdx].pos.z;
        srt.yaw = obj->def->pAttachPoints[attachIdx].rot.x;
        srt.pitch = obj->def->pAttachPoints[attachIdx].rot.y;
        srt.roll = obj->def->pAttachPoints[attachIdx].rot.z;
    }
    matrix_from_srt_reversed(&attachPointMtx, (SRT* ) &srt);

    //Transform attach point by its bone's matrix
    matrix_concat_4x3(&attachPointMtx, boneMtx, &attachPointMtx);

    //Store the transformed coords
    *ox = attachPointMtx.m[3][0];
    *oy = attachPointMtx.m[3][1];
    *oz = attachPointMtx.m[3][2];
    if (obj->parent != NULL) {
        transform_point_by_object(*ox, *oy, *oz, ox, oy, oz, obj->parent);
    } else {
        *ox += gWorldX;
        *oz += gWorldZ;
    }
    return 0;
}

MtxF *func_80032170(Object* obj, s32 arg1) {
    ModelInstance* temp_v1;
    s32 boneIdx;
    u16 new_var;

    temp_v1 = obj->modelInsts[obj->modelInstIdx];
    if (!(temp_v1->unk34 & 8)) {
        return NULL;
    }
    boneIdx = obj->def->pAttachPoints[arg1].bones[obj->modelInstIdx];
    new_var = temp_v1->unk34;
    return (MtxF *) &((f32 **)temp_v1->matrices)[new_var & 1][boneIdx << 4];
}

void func_800321E4(Object* obj, s32 arg1, f32* ox, f32* oy, f32* oz) {
    *ox = obj->def->pAttachPoints[arg1].pos.x;
    *oy = obj->def->pAttachPoints[arg1].pos.y;
    *oz = obj->def->pAttachPoints[arg1].pos.z;
}

void func_80032238(Object* obj, s32 attachBaseIdx, s32 arg2, Vec3f* position) {
    ModelInstance* modelInst;
    MtxF* mtx;
    ObjDef* objDef;
    s32 i;
    s32 boneIdx;

    objDef = obj->def;
    if (objDef->numAttachPoints < (attachBaseIdx + arg2)) {
        return;
    }

    modelInst = obj->modelInsts[obj->modelInstIdx];
    for (i = 0; i < arg2; i++) {
        if (modelInst->unk34 & 8) {
            boneIdx = objDef->pAttachPoints[attachBaseIdx + i].bones[obj->modelInstIdx];
            mtx = (MtxF *) &((f32 **)modelInst->matrices)[modelInst->unk34 & 1][boneIdx << 4];
            position[i].x = mtx->m[3][0];
            position[i].y = mtx->m[3][1];
            position[i].z = mtx->m[3][2];
            if (obj->parent == NULL) {
                position[i].x += gWorldX;
                position[i].z += gWorldZ;
            }
        } else {
            position[i].x = obj->srt.transl.x;
            position[i].y = obj->srt.transl.y;
            position[i].z = obj->srt.transl.z;
        }
    }
}

s32 isObjectInWorld(Object *obj) {
    Object **objects;
    s32 i;
    s32 objectCount;

    objects = get_world_objects(&i, &objectCount);
    for (i = 0; i < objectCount; i++) {
        if (obj == objects[i]) {
            return 1;
        }
    }

    return 0;
}

Object* func_800323C4(Object* obj, s32 objID, f32* outDistance) {
    Object* outObj;
    Object** objects;
    f32 distance;
    s32 sp48;
    s32 sp44;
    s32 i;

    objects = get_world_objects(&sp48, &sp44);
    *outDistance = SQ(*outDistance);
    outObj = NULL;
    if (objID != -1) {
        for (i = sp48; i < sp44; i++) {
            if (objID == objects[i]->id && obj != objects[i]) {
                distance = vec3_distance_squared(&obj->positionMirror, &objects[i]->positionMirror);
                if (distance < *outDistance) {
                    *outDistance = distance;
                    outObj = objects[i];
                }
            }
        }
    } else {
        for (i = sp48; i < sp44; i++) {
            distance = vec3_distance_squared(&obj->positionMirror, &(objects[i])->positionMirror);
            if (distance != 0.0f && distance < *outDistance) {
                *outDistance = distance;
                outObj = objects[i];
            }
        }
    }

    return outObj;
}

s32 func_80032538(Object* arg0) {
    Object* player;

    if (arg0->unkAF & 1 && !gDLL_1_UI->vtbl->func_DC4()) {
        player = get_player();
        if (((DLL_210_Player*)player->dll)->vtbl->func50(player) == -1) {
            joy_set_button_mask(0, A_BUTTON);
            return 1;
        }
    }
    return 0;
}

s32 registerObjectCallback(Object* obj, Object* otherObj, void (*callback)(Object*, Object*)) {
    ObjectPairCallback* callbackPair;
    s32 i;

    if ((obj == NULL) || (otherObj == NULL)) {
        return FALSE;
    }

    callbackPair = sObjectPairCallbacks;
    i = sCallbackPairIndex;
    while (i--) {
        if (obj == callbackPair->primary && otherObj == callbackPair->secondary) {
            return FALSE;
        }
        callbackPair++;
    }

    if (sCallbackPairIndex >= 16) {
        return FALSE;
    }

    callbackPair = &sObjectPairCallbacks[sCallbackPairIndex];
    callbackPair->primary = obj;
    callbackPair->secondary = otherObj;
    callbackPair->callback = callback;
    obj->unkD9++;
    otherObj->unkD9++;
    sCallbackPairIndex++;
    return TRUE;
}

void func_80032690(Object* obj, Object* otherObj) {
    ObjectPairCallback* callbackPair;
    s32 i;

    callbackPair = sObjectPairCallbacks;
    i = sCallbackPairIndex;
    while (i--) {
        if (obj == callbackPair->primary && otherObj == callbackPair->secondary) {
            sCallbackPairIndex--;
            if (sCallbackPairIndex != 15 && sCallbackPairIndex != 0) {
                *callbackPair = sObjectPairCallbacks[sCallbackPairIndex];
            }
            obj->unkD9--;
            otherObj->unkD9--;
            return;
        }
        callbackPair++;
    }
}

void func_8003273C(Object* obj) {
    ObjectPairCallback* callbackPair;
    s32 i;

    callbackPair = sObjectPairCallbacks;
    i = sCallbackPairIndex;
    while ((i--) > 0) {
        if (obj == callbackPair->primary || obj == callbackPair->secondary) {
            i--;
            sCallbackPairIndex--;
            callbackPair->primary->unkD9 -= 1;
            callbackPair->secondary->unkD9 -= 1;
            if (sCallbackPairIndex != 15 && sCallbackPairIndex != 0) {
                *callbackPair = sObjectPairCallbacks[sCallbackPairIndex];
            }
        }
        callbackPair++;
    }
}

void func_80032804(Object* obj, Object* otherObj) {
    ObjectPairCallback* callbackPair;
    s32 i;
    s32 var_s4;
    s32 var_s5;

    var_s4 = obj->unkD9;
    var_s5 = otherObj->unkD9;
    callbackPair = sObjectPairCallbacks;
    i = sCallbackPairIndex;
    while (var_s4 != 0 && var_s5 != 0 && i--) {
        if (obj == callbackPair->primary && otherObj == callbackPair->secondary) {
            var_s4--;
            callbackPair->callback(obj, otherObj);
        }
        if (otherObj == callbackPair->primary && obj == callbackPair->secondary) {
            var_s5--;
            callbackPair->callback(otherObj, obj);
        }
        callbackPair++;
    }
}
