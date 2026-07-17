#include "dlls/objects/210_player.h"
#include "dlls/objects/281_area.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object_id.h"
#include "sys/joypad.h"
#include "sys/map.h"
#include "sys/objlib.h"
#include "sys/objects.h"
#include "dll.h"

// objlib.c (default.dol)

typedef struct ObjectPairCallback {
    Object *primary;
    Object *secondary;
    ObjectTouchCallbackFunc callback; // callback that takes in the primary and secondary object
} ObjectPairCallback;

// -------- .data start 80091710 -------- //
s32 sCallbackPairIndex = 0;
// -------- .data end 80091720 -------- //

// -------- .bss start 800b2d40 -------- //
ObjectPairCallback sObjectPairCallbacks[16];
// -------- .bss end 800b2e00 -------- //

/** Transforms a mapSpace point into an objectSpace point, relative to an Area object*/
/*static*/ void objAreaWorld2LocalSpace(Area_Setup* area, f32* x, f32* y, f32* z) {
    f32 sinYaw;
    f32 cosYaw;
    f32 sinPitch;
    f32 cosPitch;
    f32 dx;
    f32 dy;
    f32 dz;

    sinYaw = mathSinfInterp(-(area->yaw << 0x8));
    cosYaw = mathCosfInterp(-(area->yaw << 0x8));
    sinPitch = mathSinfInterp(-(area->pitch << 0x8));
    cosPitch = mathCosfInterp(-(area->pitch << 0x8));

    dx = *x - area->base.x;
    dy = *y - area->base.y;
    dz = *z - area->base.z;

    *x = (dx * cosYaw) - (dz * sinYaw);
    dz = (dx * sinYaw) + (dz * cosYaw);
    *y = (dy * cosPitch) - (dz * sinPitch);
    *z = (dy * sinPitch) + (dz * cosPitch);
}

u16 objGetAreaValueAtPoint(f32 x, f32 y, f32 z) {
    s32 pad;
    s32 out;
    Area_Setup* objsetup;
    s32 i;
    s32 j;
    MapHeader** maps;
    f32 lx;
    f32 ly;
    f32 lz;
    MapHeader *current;

    out = -1U;
    maps = mapGetLoadedMapsTable();
    for (i = 0; i < MAP_ID_MAX; i++) {
        if (maps[i] == NULL) {
            continue;
        }

        current = maps[i];
        objsetup = current->objectInstanceFile_ptr;
        j = 0;
        while (j < current->objectInstancesFileLength) {
            if (objsetup->base.objId == OBJ_Area) {
                lx = x;
                ly = y;
                lz = z;
                objAreaWorld2LocalSpace(objsetup, &lx, &ly, &lz);

                if (lx < 0.0f) {
                    lx = -lx;
                }
                if (ly < 0.0f) {
                    ly = -ly;
                }
                if (lz < 0.0f) {
                    lz = -lz;
                }

                //Check if in bounds of Area
                if (lx <= objsetup->halfWidth && 
                    ly <= objsetup->halfHeight && 
                    lz <= objsetup->halfDepth
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

s16 objAngleToObjectXZ(Object* objA, Object* objB, f32* distance) {
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

    angle = mathAtan2f(dx, dz);

    //(Optional) Store distance
    if (distance != NULL) {
        *distance = sqrtf(SQ(dx) + SQ(dz));
    }

    //Return angular difference needed for objA to point towards objB
    angle16 = angle - (objA->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(angle16)
    return angle16;
}

// catmull-rom converter with the * 0.5 factored in rather than done as a second step
void objLibCatmullRomConverter(f32* in, f32* out) {
    out[0] = (-0.5f * in[0]) + (1.5f * in[1]) + (-1.5f * in[2]) + (in[3] * 0.5f);
    out[1] = in[0] + (-2.5f * in[1]) + (2.0f * in[2]) + (in[3] * -0.5f);
    out[2] = (-0.5f * in[0]) + (in[2] * 0.5f);
    out[3] = in[1];
}

// 2nd param = placeNum in default.dol
s32 objGetAttachPointWorldSpace(Object* obj, s32 attachIdx, f32* ox, f32* oy, f32* oz, s32 useInputCoords) {
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
    mathRpyXyzMtx(&attachPointMtx, &srt);

    //Transform attach point by its bone's matrix
    mathMtxCat4x3F(&attachPointMtx, boneMtx, &attachPointMtx);

    //Store the transformed coords
    *ox = attachPointMtx.m[3][0];
    *oy = attachPointMtx.m[3][1];
    *oz = attachPointMtx.m[3][2];
    if (obj->parent != NULL) {
        camTransformPointByObject(*ox, *oy, *oz, ox, oy, oz, obj->parent);
    } else {
        *ox += gWorldX;
        *oz += gWorldZ;
    }
    return 0;
}

// 2nd param = "placeNum" in default.dol
MtxF *objGetAttachPointBoneMatrix(Object* obj, s32 attachIdx) {
    ModelInstance* temp_v1;
    s32 boneIdx;
    u16 new_var;

    temp_v1 = obj->modelInsts[obj->modelInstIdx];
    if (!(temp_v1->unk34 & 8)) {
        return NULL;
    }
    boneIdx = obj->def->pAttachPoints[attachIdx].bones[obj->modelInstIdx];
    new_var = temp_v1->unk34;
    return (MtxF *) &((f32 **)temp_v1->matrices)[new_var & 1][boneIdx << 4];
}

void objGetAttachPointLocalSpace(Object* obj, s32 attachIdx, f32* ox, f32* oy, f32* oz) {
    *ox = obj->def->pAttachPoints[attachIdx].pos.x;
    *oy = obj->def->pAttachPoints[attachIdx].pos.y;
    *oz = obj->def->pAttachPoints[attachIdx].pos.z;
}

// attachBaseIdx (2nd param) = "placeno" in default.dol
// arg2 (3rd param) = "nofeet" in default.dol
void objGetAttachPointBoneWorldPositions(Object* obj, s32 attachBaseIdx, s32 count, Vec3f* positions) {
    ModelInstance* modelInst;
    MtxF* mtx;
    ObjDef* objDef;
    s32 i;
    s32 boneIdx;

    objDef = obj->def;
    if (objDef->numAttachPoints < (attachBaseIdx + count)) {
        return;
    }

    modelInst = obj->modelInsts[obj->modelInstIdx];
    for (i = 0; i < count; i++) {
        if (modelInst->unk34 & 8) {
            boneIdx = objDef->pAttachPoints[attachBaseIdx + i].bones[obj->modelInstIdx];
            mtx = (MtxF *) &((f32 **)modelInst->matrices)[modelInst->unk34 & 1][boneIdx << 4];
            positions[i].x = mtx->m[3][0];
            positions[i].y = mtx->m[3][1];
            positions[i].z = mtx->m[3][2];
            if (obj->parent == NULL) {
                positions[i].x += gWorldX;
                positions[i].z += gWorldZ;
            }
        } else {
            positions[i].x = obj->srt.transl.x;
            positions[i].y = obj->srt.transl.y;
            positions[i].z = obj->srt.transl.z;
        }
    }
}

s32 objIsObjectStandalone(Object *obj) {
    Object **objects;
    s32 i;
    s32 objectCount;

    objects = objGetObjects(&i, &objectCount);
    for (i = 0; i < objectCount; i++) {
        if (obj == objects[i]) {
            return 1;
        }
    }

    return 0;
}

Object* objFindClosestObject(Object* obj, s32 objID, f32* distance) {
    Object* outObj;
    Object** objects;
    f32 distTemp;
    s32 start;
    s32 numObjs;
    s32 i;

    objects = objGetObjects(&start, &numObjs);
    *distance = SQ(*distance);
    outObj = NULL;
    if (objID != -1) {
        for (i = start; i < numObjs; i++) {
            if (objID == objects[i]->id && obj != objects[i]) {
                distTemp = vec3DistanceSquared(&obj->globalPosition, &objects[i]->globalPosition);
                if (distTemp < *distance) {
                    *distance = distTemp;
                    outObj = objects[i];
                }
            }
        }
    } else {
        for (i = start; i < numObjs; i++) {
            distTemp = vec3DistanceSquared(&obj->globalPosition, &(objects[i])->globalPosition);
            // @bug? why exclude a dist of 0 here?
            if (distTemp != 0.0f && distTemp < *distance) {
                *distance = distTemp;
                outObj = objects[i];
            }
        }
    }

    return outObj;
}

int objCheckPlayerInteract(Object* obj) {
    Object* player;

    if ((obj->unkAF & ARROW_FLAG_1_Interacted) && 
        (gDLL_1_cmdmenu->vtbl->was_any_item_used() == FALSE)
    ) {
        player = objGetPlayer();
        if (((DLL_210_Player*)player->dll)->vtbl->func50(player) == -1) { // ensure no spell is equipped
            joyDisableButtons(0, A_BUTTON);
            return TRUE;
        }
    }

    return FALSE;
}

s32 objRegisterTouchCallback(Object* obj, Object* otherObj, ObjectTouchCallbackFunc callback) {
    ObjectPairCallback* callbackPair;
    s32 i;

    if ((obj == NULL) || (otherObj == NULL)) {
        return FALSE;
    }

    callbackPair = sObjectPairCallbacks;
    i = sCallbackPairIndex;
    while (i--) {
        if (obj == callbackPair->primary && otherObj == callbackPair->secondary) {
            /* default.dol
            if (callbackPair->callback != callback) {
                STUBBED_PRINTF("WARNING:only one callback per collision is allowed\n");
            } else {
                STUBBED_PRINTF("WARNING:TCB already registered\n");
            }
            */
            return FALSE;
        }
        callbackPair++;
    }

    if (sCallbackPairIndex >= 16) {
        // STUBBED_PRINTF("WARNING: MAX_TOUCH_CALLBACKS too small\n"); // default.dol
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

void objRemoveTouchCallback(Object* obj, Object* otherObj) {
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

void objRemoveTouchCallbacksForObj(Object* obj) {
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

void objInvokeTouchCallbacks(Object* obj, Object* otherObj) {
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
