#include "common.h"
#include "dll.h"
#include "dlls/objects/210_player.h"
#include "sys/map.h"

extern s32 sCallbackPairIndex;
typedef struct ObjectPairCallback {
    Object *primary;
    Object *secondary;
    void (*callback)(Object*, Object*); // callback that takes in the primary and secondary object
} ObjectPairCallback;
extern ObjectPairCallback sObjectPairCallbacks[16];

typedef struct SomeObjSetup {
    ObjSetup base;
    u16 unk18;
    u16 unk1A;
    u16 unk1C;
    u16 unk1E;
    u8 unk20;
    u8 unk21;
} SomeObjSetup;

void func_80031AA0(SomeObjSetup* arg0, f32* ox, f32* oy, f32* oz) {
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 zDiff;
    f32 yDiff;
    f32 temp_fv0;
    f32 xDiff;

    sp2C = fsin16_precise(-(arg0->unk20 << 0x8));
    sp28 = fcos16_precise(-(arg0->unk20 << 0x8));
    sp24 = fsin16_precise(-(arg0->unk21 << 0x8));
    temp_fv0 = fcos16_precise(-(arg0->unk21 << 0x8));
    xDiff = *ox - arg0->base.x;
    yDiff = *oy - arg0->base.y;
    zDiff = *oz - arg0->base.z;
    *ox = (xDiff * sp28) - ((zDiff) * sp2C);
    zDiff = (xDiff * sp2C) + ((zDiff) * sp28);
    *oy = (yDiff * temp_fv0) - (zDiff * sp24);
    *oz = (yDiff * sp24) + (zDiff * temp_fv0);
}

u16 func_80031BBC(f32 arg0, f32 arg1, f32 arg2) {
    s32 pad;
    s32 out;
    u8* objsetup;
    s32 i;
    s32 j;
    MapHeader** sp68;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    MapHeader *current;

    out = -1U;
    sp68 = func_80044A10();
    for (i = 0; i < MAP_ID_MAX; i++) {
        if (sp68[i] == NULL) {
            continue;
        }

        current = sp68[i];
        objsetup = current->objectInstanceFile_ptr;
        j = 0;
        while (j < current->objectInstancesFileLength) {
            if (((SomeObjSetup *)objsetup)->base.objId == OBJ_Area) {
                sp7C = arg0;
                sp78 = arg1;
                sp74 = arg2;
                func_80031AA0(((SomeObjSetup *)objsetup), &sp7C, &sp78, &sp74);
                if (sp7C < 0.0f) {
                    sp7C = -sp7C;
                }
                if (sp78 < 0.0f) {
                    sp78 = -sp78;
                }
                if (sp74 < 0.0f) {
                    sp74 = -sp74;
                }
                if (sp7C <= ((SomeObjSetup *)objsetup)->unk1A) {
                    if (sp78 <= ((SomeObjSetup *)objsetup)->unk1C) {
                        if (sp74 <= ((SomeObjSetup *)objsetup)->unk1E) {
                            out = ((SomeObjSetup *)objsetup)->unk18;
                        }
                    }
                }
            }
            j += ((SomeObjSetup *)objsetup)->base.quarterSize << 2;
            objsetup += ((SomeObjSetup *)objsetup)->base.quarterSize << 2;
        }
    }
    return out;
}

s16 func_80031DD8(Object* arg0, Object* arg1, f32* arg2) {
    s16 sp26;
    s32 var_v1;
    f32 sp1C;
    f32 sp18;

    if ((arg0 == NULL) || (arg1 == NULL)) {
        return 0;
    }
    sp1C = arg0->srt.transl.x - arg1->srt.transl.x;
    sp18 = arg0->srt.transl.z - arg1->srt.transl.z;
    sp26 = arctan2_f(sp1C, sp18);
    if (arg2 != NULL) {
        *arg2 = sqrtf(SQ(sp1C) + SQ(sp18));
    }
    var_v1 = sp26 - (arg0->srt.yaw & 0xFFFF);
    if (var_v1 >= 0x8001) {
        var_v1 += 0xFFFF0001;
    }
    if (var_v1 < -0x8000) {
        var_v1 += 0xFFFF;
    }
    return var_v1;
}

void func_80031EBC(Vec4f* arg0, Vec4f* arg1) {
    arg1->x = (arg0->w * 0.5f) + ((-0.5f * arg0->x) + (1.5f * arg0->y) + (-1.5f * arg0->z));
    arg1->y = (arg0->w * -0.5f) + (arg0->x + (-2.5f * arg0->y) + (2.0f * arg0->z));
    arg1->z = (arg0->z * 0.5f) + (-0.5f * arg0->x);
    arg1->w = arg0->y;
}

s32 func_80031F6C(Object* obj, s32 attachIdx, f32* ox, f32* oy, f32* oz, s32 arg5) {
    s32 boneIdx;
    ModelInstance* temp_v1;
    MtxF* sp9C;
    MtxF sp5C;
    SRT srt;

    temp_v1 = obj->modelInsts[obj->modelInstIdx];
    if (!(temp_v1->unk34 & 8)) {
        *ox = obj->srt.transl.x;
        *oy = obj->srt.transl.y;
        *oz = obj->srt.transl.z;
        return 1;
    }
    boneIdx = obj->def->pAttachPoints[attachIdx].bones[obj->modelInstIdx];
    sp9C = (MtxF *) &(((f32 **)temp_v1->matrices)[(temp_v1->unk34 & 1)][boneIdx << 4]);
    if (arg5 != 0) {
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
        srt.yaw = obj->def->pAttachPoints[attachIdx].rot[0];
        srt.pitch = obj->def->pAttachPoints[attachIdx].rot[1];
        srt.roll = obj->def->pAttachPoints[attachIdx].rot[2];
    }
    matrix_from_srt_reversed(&sp5C, (SRT* ) &srt);
    matrix_concat_4x3(&sp5C, sp9C, &sp5C);
    *ox = sp5C.m[3][0];
    *oy = sp5C.m[3][1];
    *oz = sp5C.m[3][2];
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

void func_80032238(Object* obj, s32 arg1, s32 arg2, Vec3f* arg3) {
    ModelInstance* modelInst;
    MtxF* mtx;
    ObjDef* objDef;
    s32 i;
    s32 boneIdx;

    objDef = obj->def;
    if (objDef->numAttachPoints < (arg1 + arg2)) {
        return;
    }

    modelInst = obj->modelInsts[obj->modelInstIdx];
    for (i = 0; i < arg2; i++) {
        if (modelInst->unk34 & 8) {
            boneIdx = objDef->pAttachPoints[arg1 + i].bones[obj->modelInstIdx];
            mtx = (MtxF *) &((f32 **)modelInst->matrices)[modelInst->unk34 & 1][boneIdx << 4];
            arg3[i].x = mtx->m[3][0];
            arg3[i].y = mtx->m[3][1];
            arg3[i].z = mtx->m[3][2];
            if (obj->parent == NULL) {
                arg3[i].x += gWorldX;
                arg3[i].z += gWorldZ;
            }
        } else {
            arg3[i].x = obj->srt.transl.x;
            arg3[i].y = obj->srt.transl.y;
            arg3[i].z = obj->srt.transl.z;
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

    if (arg0->unkAF & 1 && !gDLL_1_UI->vtbl->func6()) {
        player = get_player();
        if (((DLL_210_Player*)player->dll)->vtbl->func50(player) == -1) {
            set_button_mask(0, 0x8000);
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
