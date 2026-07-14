#include "dlls/objects/common/group48.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/camera.h"
#include "sys/di_rcp.h"
#include "sys/exception.h"
#include "sys/map.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/rsp_segment.h"
#include "sys/segment_1D900.h"
#include "sys/objprint.h"

// -------- .data start 80091750 -------- //
s32 D_80091750 = 0; // unused bytes
/** toggles Models' multiplier colour */
u8 BYTE_80091754 = 0;
/** toggles Models' blend colour */
u8 BYTE_80091758 = 0;
u8 _data_8009175c[0x14] = {0}; // unused bytes
// -------- .data end 80091770 -------- //

// -------- .bss start 800b2e10 -------- //
MtxF *D_800B2E10;
/** gModelColourMultiplyR? */
s16 SHORT_800b2e14;
/** gModelColourMultiplyG? */
s16 SHORT_800b2e16;
/** gModelColourMultiplyB? */
s16 SHORT_800b2e18;
MtxF *PTR_DAT_800b2e1c;
/** gModelColourBlendR? */
u8 BYTE_800b2e20;
/** gModelColourBlendG? */
u8 BYTE_800b2e21;
/** gModelColourBlendB? */
u8 BYTE_800b2e22;
/** gModelColourBlendA? */
u8 BYTE_800b2e23;
// -------- .bss end 800b2e30 -------- //

void objprint_func_800357B4(Object*, ModelInstance*, Model*);
ModelInstance *objprintDrawChildModel(Gfx**, Mtx**, Vertex**, Triangle**, Object*, ModelInstance*, MtxF*, MtxF*, Object*, s32, s32);
void objprint_func_80036890(Object*, s32);
void objprint_func_80036058(Object*, Object*, ModelInstance*, Gfx**, Mtx**, Vertex**);

void objprintDrawObject(Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** tris, Object* obj, s8 visibility) {
    if (obj->stateFlags & OBJSTATE_DESTROYED) {
        return;
    }

    if (visibility == 0) {
        if (obj->objhitInfo != NULL && (obj->objhitInfo->unk5A & 0x30)) {
            obj->objhitInfo->unk9F = 2;
        }
    }

    if (obj->srt.flags & OBJFLAG_INVISIBLE) {
        return;
    }

    if (obj->parent != NULL && (obj->parent->srt.flags & OBJFLAG_INVISIBLE)) {
        return;
    }

    update_pi_manager_array(2, obj->id);
    diRcpTrace(*gdl, obj->id, "objects/objprint.c", 426);
    if (obj->dll != NULL) {
        if (!(obj->stateFlags & OBJSTATE_PRINT_DISABLED)) {
            obj->dll->vtbl->print(obj, gdl, mtxs, vtxs, tris, visibility);
        } else if (visibility != 0) {
            objprintDrawModel(obj, gdl, mtxs, vtxs, tris, 1.0f);
        }
    } else if (visibility != 0) {
        objprintDrawModel(obj, gdl, mtxs, vtxs, tris, 1.0f);
    }
    if (obj->stateFlags & OBJSTATE_PENDING_MODEL_SWITCH) {
        objHandleModelSwitch(obj, obj->modelInsts[obj->modelInstIdx], obj->modelInsts[obj->modelInstIdx]->model);
    }
    if (obj->linkedObject != NULL && (obj->linkedObject->stateFlags & OBJSTATE_PENDING_MODEL_SWITCH)) {
        objHandleModelSwitch(obj->linkedObject, obj->linkedObject->modelInsts[obj->modelInstIdx], obj->linkedObject->modelInsts[obj->modelInstIdx]->model);
    }
    diRcpTrace(*gdl, (u32) -obj->id, "objects/objprint.c", 489);
    update_pi_manager_array(2, -1);
}

void objprintSetModelMatrixOverride(MtxF* arg0) {
    PTR_DAT_800b2e1c = arg0;
}

void objprintDrawModel(Object* obj, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** tris, f32 yPrescale) {
    s32 opacity;
    ModelInstance* modelInst;
    Model* model;
    SRT spDC;
    Object* parentObj;
    u8 blendR;
    u8 blendG;
    u8 blendB;
    s32 var_v0;
    s32 var_v1;
    s32 var_a0;
    f32 spC4;
    f32 spC0;
    f32 spBC;
    s16 spBA;
    MtxF sp78;
    s32 pad;
    MtxF* sp70;
    u8 sp6F;
    u8 sp6E;
    u8 sp6D;
    Gfx* tempGdl;
    Mtx* tempMtxs;
    Vertex* tempVtxs;
    Triangle* tempTris;

    func_8001F81C(&sp6F, &sp6E, &sp6D);
    tempGdl = *gdl;
    tempMtxs = *mtxs;
    tempVtxs = *vtxs;
    tempTris = *tris;
    modelInst = obj->modelInsts[obj->modelInstIdx];
    if (modelInst == NULL) {
        return;
    }

    model = modelInst->model;
    opacity = obj->opacityWithFade;
    if (opacity > 0xFF) {
        opacity = 0xFF;
    }
    if (obj->def->flags & OBJDEF_SKY_LIT) {
        if (func_8001EBE0() != 0) {
            blendB = blendG = blendR = 0xFF;
        } else {
            blendR = sp6F;
            blendG = sp6E;
            blendB = sp6D;
        }
    } else {
        blendB = blendG = blendR = 0xFF;
    }
    if (obj->def->numAnimatedFrames > 0) {
        objprint_func_80036890(obj, 2);
    }
    if (BYTE_80091754 != 0) {
        var_v0 = (blendR * SHORT_800b2e14) >> 8;
        var_v1 = (blendG * SHORT_800b2e16) >> 8;
        var_a0 = (blendB * SHORT_800b2e18) >> 8;
        if (var_v0 > 0xFF) {
            var_v0 = 0xFF;
        }
        if (var_v1 > 0xFF) {
            var_v1 = 0xFF;
        }
        if (var_a0 > 0xFF) {
            var_a0 = 0xFF;
        }
        blendR = var_v0;
        blendG = var_v1;
        blendB = var_a0;
        BYTE_80091754 = 0;
    }
    if (BYTE_80091758 != 0) {
        blendR = BYTE_800b2e20;
        blendG = BYTE_800b2e21;
        blendB = BYTE_800b2e22;
        BYTE_80091758 = 0;
    } else {
        BYTE_800b2e23 = 0;
    }
    parentObj = obj->parent;
    if (parentObj != NULL) {
        camSetupRSPMatricesForObject(&tempGdl, &tempMtxs, parentObj);
        spC4 = obj->srt.transl.f[0];
        spC0 = obj->srt.transl.f[1];
        spBC = obj->srt.transl.f[2];
        spBA = obj->srt.yaw;
    }
    if ((obj->shadow != NULL) && (obj->shadow->gdl != NULL)) {
        if (obj->shadow->flags & OBJ_SHADOW_FLAG_CUSTOM_OBJ_POS) {
            spDC.transl.f[0] = obj->shadow->tr.f[0];
            spDC.transl.f[1] = obj->shadow->tr.f[1];
            spDC.transl.f[2] = obj->shadow->tr.f[2];
        } else {
            spDC.transl.f[0] = obj->srt.transl.f[0];
            spDC.transl.f[1] = obj->srt.transl.f[1];
            spDC.transl.f[2] = obj->srt.transl.f[2];
        }
        spDC.yaw = 0;
        spDC.roll = 0;
        spDC.pitch = 0;
        spDC.scale = 0.05f;
        if (parentObj != NULL && model->animCount != 0) {
            spDC.transl.f[0] += gWorldX;
            spDC.transl.f[2] += gWorldZ;
        }
        camSetupObjectSRTMatrix(&tempGdl, &tempMtxs, &spDC, 1.0f, 0.0f, NULL);
        gSPDisplayList(tempGdl++, OS_PHYSICAL_TO_K0(obj->shadow->gdl));
        dlSetAllDirty();
        texRenderReset();
    }
    if (!(obj->srt.flags & OBJFLAG_SHADOW_ONLY)) {
        if (!(modelInst->unk34 & 8)) {
            if ((model->animCount != 0) && !(model->unk71 & 2)) {
                if (PTR_DAT_800b2e1c == 0) {
                    mod_func_8001943C(obj, &sp78, yPrescale, 0.0f);
                    mod_func_80019730(modelInst, model, obj, &sp78);
                } else {
                    mod_func_80019730(modelInst, model, obj, PTR_DAT_800b2e1c);
                }
            } else {
                modelInst->unk34 ^= 1;
                sp70 = modelInst->matrices[modelInst->unk34 & 1];
                if (PTR_DAT_800b2e1c == 0) {
                    mod_func_8001943C(obj, sp70, yPrescale, 0.0f);
                } else {
                    bcopy((void* ) PTR_DAT_800b2e1c, sp70, 0x40);
                }
                camAddMatrixToPool(sp70, 1);
            }
            modelInst->unk34 ^= 2;
            if ((obj->def->flags & OBJDEF_FLAG10) || (model->blendshapes != NULL)) {
                if (model->blendshapes != NULL) {
                    mod_func_8001B100(modelInst);
                }
                if (obj->def->flags & OBJDEF_FLAG10) {
                    func_8001DF60(obj, modelInst);
                }
            }
            if (obj->def->numAnimatedFrames != 0) {
                func_8001E818(obj, model, modelInst);
            }
            if (model->envMapCount != 0) {
                func_8001F094(modelInst);
            }
            if (obj->unk74 != NULL) {
                objprintUpdateLockIconCoords(obj);
            }
            if (model->hitSphereCount != 0) {
                mod_func_8001A8EC(modelInst, model, obj, 0, obj);
            } else if ((obj->objhitInfo != NULL) && (obj->objhitInfo->unk5A & 0x20)) {
                if (obj->objhitInfo->unk9F != 0) {
                    obj->objhitInfo->unk9F--;
                }
            }
        }
        if (model->unk71 & 4) {
            dlSetEnvColor(&tempGdl, blendR, blendG, blendB, BYTE_800b2e23);
        }
        dlSetPrimColor(&tempGdl, blendR, blendG, blendB, opacity);
        if (!(obj->srt.flags & OBJFLAG_SKIP_MODEL_DL)) {
            gSPSegment(tempGdl++, SEGMENT_3, modelInst->matrices[modelInst->unk34 & 1]);
            gSPSegment(tempGdl++, SEGMENT_5, modelInst->vertices[((s32) modelInst->unk34 >> 1) & 1]);
            if (opacity == 0xFF) {
                if (modelInst->unk34 & 0x10) {
                    modLoadModelDisplayList(model, modelInst);
                    modelInst->unk34 ^= 0x10;
                }
            } else {
                if (!(modelInst->unk34 & 0x10)) {
                    modLoadModelDisplayList2(model, modelInst);
                    modelInst->unk34 ^= 0x10;
                }
            }
            gSPDisplayList(tempGdl++, OS_PHYSICAL_TO_K0(modelInst->displayList));
            dlSetAllDirty();
            texRenderReset();
        }
        if (obj->linkedObject != NULL) {
            objprintDrawChildModel(&tempGdl, &tempMtxs, &tempVtxs, &tempTris, obj, modelInst, &sp78, 0, 
                obj->linkedObject, obj->stateFlags & OBJSTATE_UNK_ATTACH_INDEX_MASK, (u8)opacity);
        }
    }
    if ((obj->objhitInfo != NULL) && (obj->objhitInfo->unk5A & 0x20) && (modelInst->unk14 != NULL)) {
        objprint_func_800357B4(obj, modelInst, modelInst->model);
    }
    modelInst->unk34 |= 8;
    if (parentObj != NULL) {
        obj->srt.transl.f[0] = spC4;
        obj->srt.transl.f[1] = spC0;
        obj->srt.transl.f[2] = spBC;
        obj->srt.yaw = spBA;
        camLoadParentProjection(&tempGdl);
    }
    *gdl = tempGdl;
    *mtxs = tempMtxs;
    *vtxs = tempVtxs;
    *tris = tempTris;
}

void objprint_func_800357B4(Object* arg0, ModelInstance* arg1, Model* arg2) {
    f32* temp_v0_2;
    Vec3f* var_s0;
    MtxF* var_v1;
    MtxF sp54;
    s32 i;

    if (!(arg1->unk34 & 0x20)) {
        var_v1 = (MtxF*)arg1->matrices[arg1->unk34 & 1]->m[3];
        var_s0 = arg1->unk14->unk0;
        for (i = 0; i < arg2->jointCount; i++) {
            var_s0->f[0] = var_v1->m[0][0];
            var_s0->f[1] = var_v1->m[0][1];
            var_s0->f[2] = var_v1->m[0][2];
            if ( arg2->collisionB[i] > 1.0f) {
                temp_v0_2 = &((f32*)arg1->matrices[arg1->unk34 & 1])[arg2->joints[i].parentJointID << 4] + 12;
                var_s0->f[0] += (var_s0->f[0] - temp_v0_2[0]) * (arg2->collisionB[i] - 1.0f);
                var_s0->f[1] += (var_s0->f[1] - temp_v0_2[1]) * (arg2->collisionB[i] - 1.0f);
                var_s0->f[2] += (var_s0->f[2] - temp_v0_2[2]) * (arg2->collisionB[i] - 1.0f);
            }
            var_s0 += 1;
            var_v1 += 1;
        }
    }
    if (arg0->parent == NULL) {
        return;
    }

    matrix_from_srt(&sp54, &arg0->parent->srt);
    var_s0 = arg1->unk14->unk0;
    for (i = 0; i < arg2->jointCount; i++) {
        vec3_transform(&sp54, var_s0->f[0], var_s0->f[1], var_s0->f[2], &var_s0->f[0], &var_s0->f[1], &var_s0->f[2]);
        var_s0->f[0] -= gWorldX;
        var_s0->f[2] -= gWorldZ;
        var_s0 += 1;
    }
}

void objprintDrawShadowModel(Object *obj, Gfx **gdl, Mtx **rspMtxs, Vertex **vtxs, Triangle **pols, u32 param_6) {
    Gfx *mygdl;
    Mtx *outRspMtxs;
    void *d;
    u32 mainIdx;
    u32 otherIdx;
    ModelInstance *modelInst;
    ModelInstance *modelInst2;

    mygdl = *gdl;
    outRspMtxs = *rspMtxs;

    mainIdx = obj->modelInstIdx;
    if (param_6) {
        otherIdx = 0;
    } else {
        otherIdx = obj->def->numModels - 1;
    }

    modelInst = obj->modelInsts[mainIdx];
    modelInst2 = obj->modelInsts[otherIdx];

    d = modelInst->matrices[modelInst->unk34 & 1];
    gSPSegment(mygdl++, SEGMENT_3, d);
    gSPSegment(mygdl++, SEGMENT_5, modelInst2->vertices[(modelInst2->unk34 >> 1) & 0x1]);
    gSPDisplayList(mygdl++, OS_K0_TO_PHYSICAL(modelInst2->displayList));

    dlSetAllDirty();
    texRenderReset();

    *gdl = mygdl;
    *rspMtxs = outRspMtxs;
}

ModelInstance *objprintDrawChildModel(Gfx** arg0, Mtx** arg1, Vertex** arg2, Triangle** arg3, Object* arg4, ModelInstance* arg5, MtxF* arg6, MtxF* arg7, Object* arg8, s32 arg9, s32 arg10) {
    MtxF* sp74;
    s32 sp70;
    ModelInstance* modelInst;
    MtxF* var_v1;
    Model* sp64;
    SRT sp4C;
    s32 i;

    D_800B2E10 = 0;

    if (arg8->def->numAnimatedFrames > 0) {
        objprint_func_80036890(arg8, 2);
    }
    modelInst = arg8->modelInsts[arg8->modelInstIdx];
    if (modelInst != NULL && !(arg5->unk34 & 8)) {
        sp64 = modelInst->model;
        if (arg4->def->numAttachPoints > 0) {
            sp70 = arg4->def->pAttachPoints[arg9].bones[arg4->modelInstIdx];
            sp4C.transl.f[0] = arg4->def->pAttachPoints[arg9].pos.f[0];
            sp4C.transl.f[1] = arg4->def->pAttachPoints[arg9].pos.f[1];
            sp4C.transl.f[2] = arg4->def->pAttachPoints[arg9].pos.f[2];
            sp4C.scale = 1.0f;
            sp4C.yaw = arg4->def->pAttachPoints[arg9].rot.s[0];
            sp4C.pitch = arg4->def->pAttachPoints[arg9].rot.s[1];
            sp4C.roll = arg4->def->pAttachPoints[arg9].rot.s[2];
            matrix_from_srt(arg6, &sp4C);
            matrix_concat_4x3(arg6, (MtxF*)&((f32*)arg5->matrices[arg5->unk34 & 1])[sp70 << 4], arg6);
        } else {
            // required to match
        }
        if (sp64->animCount != 0) {
            D_800B2E10 = arg6;
            mod_func_80019730(modelInst, sp64, arg8, arg6);
            sp74 = modelInst->matrices[modelInst->unk34 & 1];
            objprint_func_80036058(arg8, arg4, modelInst, arg0, arg1, arg2);
        } else {
            modelInst->unk34 ^= 1;
            arg7 = modelInst->matrices[modelInst->unk34 & 1];
            for (i = 0; i < 16; i++) {
                ((f32*)arg7->m)[i] = ((f32*)arg6->m)[i];
            }
            objprint_func_80036058(arg8, arg4, modelInst, arg0, arg1, arg2);
            camAddMatrixToPool(arg7, 1);
            D_800B2E10 = sp74 = arg7;
        }
        modelInst->unk34 ^= 2;
        if ((arg8->def->flags & OBJDEF_FLAG10) || (sp64->blendshapes != NULL)) {
            if (sp64->blendshapes != NULL) {
                mod_func_8001B100(modelInst);
            }
            func_8001DF60(arg8, modelInst);
        }
        if (sp64->envMapCount != 0) {
            func_8001F094(modelInst);
        }
        if (sp64->hitSphereCount != 0) {
            mod_func_8001A8EC(modelInst, sp64, arg8, arg7, arg4);
        }
        if (!(arg4->srt.flags & OBJFLAG_SKIP_MODEL_DL)) {
            gSPSegment((*arg0)++, SEGMENT_3, sp74);
            gSPSegment((*arg0)++, SEGMENT_5, modelInst->vertices[(modelInst->unk34 >> 1) & 1]);
            if ((u8) arg10 == 0xFF) {
                if (modelInst->unk34 & 0x10) {
                    modLoadModelDisplayList(sp64, modelInst);
                    modelInst->unk34 ^= 0x10;
                }
            } else if (!(modelInst->unk34 & 0x10)) {
                modLoadModelDisplayList2(sp64, modelInst);
                modelInst->unk34 ^= 0x10;
            }
            gSPDisplayList((*arg0)++, OS_PHYSICAL_TO_K0(modelInst->displayList));
            dlSetAllDirty();
            texRenderReset();
            // @fake
            if (D_800B2E10) {}
        }
        arg8->srt.transl.f[0] = D_800B2E10->m[3][0];
        arg8->srt.transl.f[1] = D_800B2E10->m[3][1];
        arg8->srt.transl.f[2] = D_800B2E10->m[3][2];
        if (arg8->parent != NULL) {
            camTransformPointByObject(arg8->srt.transl.f[0], arg8->srt.transl.f[1], arg8->srt.transl.f[2], arg8->globalPosition.f, &arg8->globalPosition.f[1], &arg8->globalPosition.f[2], arg8->parent);
        } else {
            arg8->srt.transl.f[0] += gWorldX;
            arg8->srt.transl.f[2] += gWorldZ;
            arg8->globalPosition.f[0] = arg8->srt.transl.f[0];
            arg8->globalPosition.f[1] = arg8->srt.transl.f[1];
            arg8->globalPosition.f[2] = arg8->srt.transl.f[2];
        }
        if (arg8->def->numAttachPoints >= 2 && arg8->controlNo == OBJCONTROL_Weapon) {
            if (arg8->parent != NULL) {
                camLoadParentProjection(arg0);
            }
            ((DLL_IGROUP_48 *)arg8->dll)->vtbl->func10(arg8, arg0, arg1, arg2, arg3);
            if (arg8->parent != NULL) {
                camSetupRSPMatricesForObject(arg0, arg1, arg8->parent);
            }
        }
    }

    return modelInst;
}

void objprint_func_80036058(Object* obj, Object* otherObj, ModelInstance* modelInst, Gfx** gdl, Mtx** mtxs, Vertex** vtxs) {
    MtxF* tempMtx;
    s32 j;
    s32 boneId;
    s32 i;
    Vec3f sp7C;
    Vec3f sp70;
    ObjGroup48_Data* objData;

    if (obj->def->numAttachPoints < 2 || obj->controlNo != OBJCONTROL_Weapon) {
        return;
    }

    objData = obj->data;
    for (i = 0; i < objData->unk8A; i++) {
        j = i << 1;
        if (j < obj->def->numAttachPoints) {
            boneId = obj->def->pAttachPoints[j + 1].bones[obj->modelInstIdx];
            tempMtx = (MtxF*) &((f32*)modelInst->matrices[modelInst->unk34 & 1])[boneId << 4];
            sp7C.f[0] = obj->def->pAttachPoints[j + 1].pos.f[0];
            sp7C.f[1] = obj->def->pAttachPoints[j + 1].pos.f[1];
            sp7C.f[2] = obj->def->pAttachPoints[j + 1].pos.f[2];
            vec3_transform(tempMtx, sp7C.f[0], sp7C.f[1], sp7C.f[2], &sp7C.f[0], &sp7C.f[1], &sp7C.f[2]);
            if (otherObj->parent != NULL) {
                camTransformPointByObject(sp7C.f[0], sp7C.f[1], sp7C.f[2], &sp7C.f[0], &sp7C.f[1], &sp7C.f[2], otherObj->parent);
            } else {
                sp7C.f[0] += gWorldX;
                sp7C.f[2] += gWorldZ;
            }
            objData->unk30[i] = sp7C.f[0];
            objData->unk38[i] = sp7C.f[1];
            objData->unk40[i] = sp7C.f[2];
        }
        if (j < obj->def->numAttachPoints) {
            boneId = obj->def->pAttachPoints[j].bones[obj->modelInstIdx];
            tempMtx = (MtxF*) &((f32*)modelInst->matrices[modelInst->unk34 & 1])[boneId << 4];
            sp70.f[0] = obj->def->pAttachPoints[j].pos.f[0];
            sp70.f[1] = obj->def->pAttachPoints[j].pos.f[1];
            sp70.f[2] = obj->def->pAttachPoints[j].pos.f[2];
            vec3_transform(tempMtx, sp70.f[0], sp70.f[1], sp70.f[2], &sp70.f[0], &sp70.f[1], &sp70.f[2]);
            if (otherObj->parent != NULL) {
                camTransformPointByObject(sp70.f[0], sp70.f[1], sp70.f[2], &sp70.f[0], &sp70.f[1], &sp70.f[2], otherObj->parent);
            } else {
                sp70.f[0] += gWorldX;
                sp70.f[2] += gWorldZ;
            }
            objData->unk18[i] = sp70.f[0];
            objData->unk20[i] = sp70.f[1];
            objData->unk28[i] = sp70.f[2];
        }
    }
    if (objData->unk8A != 0) {
        sp7C.f[0] = objData->unk30[objData->unk8C];
        sp7C.f[1] = objData->unk38[objData->unk8C];
        sp7C.f[2] = objData->unk40[objData->unk8C];
        ((DLL_IGROUP_48*)obj->dll)->vtbl->func9(obj, otherObj, &sp70);
        sp7C.f[0] -= sp70.f[0];
        sp7C.f[1] -= sp70.f[1];
        sp7C.f[2] -= sp70.f[2];
        obj->srt.yaw = arctan2_f(sp7C.x, sp7C.z);
        obj->srt.pitch = 0x4000 - arctan2_f(sp7C.y, sqrtf(SQ(sp7C.z) + SQ(sp7C.x)));
        obj->srt.roll = 0;
    }
}

void objprintUpdateLockIconCoords(Object* arg0) {
    ObjDefLockData* sp11C;
    ObjectStruct74* sp118;
    MtxF* temp_s4;
    f32 temp_fs0;
    f32 temp_fs1;
    f32 temp_fs2;
    s32 i; // sp104
    s32 mtxId;
    MtxF spC0;
    SRT spA8;
    ModelInstance* spA4;

    sp11C = arg0->def->lockdata;
    sp118 = arg0->unk74;
    if (arg0->unkAF & 0x28) {
        return;
    }
    spA4 = arg0->modelInsts[arg0->modelInstIdx];
    for (i = 0; i < arg0->def->numLockdata; i++) {
        mtxId = sp11C[i].unk11[arg0->modelInstIdx];
        if (mtxId >= 0) {
            temp_s4 = (MtxF *)&((f32*)spA4->matrices[spA4->unk34 & 1])[mtxId << 4];
            temp_fs0 = sp11C[i].unk00 * 2;
            temp_fs1 = sp11C[i].unk02 * 2;
            temp_fs2 = sp11C[i].unk04 * 2;
            if (sp11C->flags & 0x10) {
                vec3_transform(temp_s4, 0.0f, 0.0f, 0.0f, &sp118[i].drawPoint.f[0], &sp118[i].drawPoint.f[1], &sp118[i].drawPoint.f[2]);
                sp118[i].refPoint.f[0] += temp_fs0;
                sp118[i].refPoint.f[1] += temp_fs1;
                sp118[i].refPoint.f[2] += temp_fs2;
            } else {
                vec3_transform(temp_s4, temp_fs0, temp_fs1, temp_fs2, &sp118[i].drawPoint.f[0], &sp118[i].drawPoint.f[1], &sp118[i].drawPoint.f[2]);
            }
            if (arg0->parent == NULL) {
                sp118[i].drawPoint.f[0] += gWorldX;
                sp118[i].drawPoint.f[2] += gWorldZ;
            } else {
                camTransformPointByObjectMatrix(&sp118[i].drawPoint, &sp118[i].drawPoint, arg0->parent->matrixIdx);
            }
            temp_fs0 = sp11C[i].unk06 * 2;
            temp_fs1 = sp11C[i].unk08 * 2;
            temp_fs2 = sp11C[i].unk0a * 2;
            vec3_transform(temp_s4, temp_fs0, temp_fs1, temp_fs2, &sp118[i].refPoint.f[0], &sp118[i].refPoint.f[1], &sp118[i].refPoint.f[2]);
            if (arg0->parent == NULL) {
                sp118[i].refPoint.f[0] += gWorldX;
                sp118[i].refPoint.f[2] += gWorldZ;
            } else {
                camTransformPointByObjectMatrix(&sp118[i].refPoint, &sp118[i].refPoint, arg0->parent->matrixIdx);
            }
        } else {
            spA8.transl.f[0] = arg0->globalPosition.f[0];
            spA8.transl.f[1] = arg0->globalPosition.f[1];
            spA8.transl.f[2] = arg0->globalPosition.f[2];
            if (sp11C->flags & 0x10) {
                spA8.yaw = 0;
                spA8.pitch = 0;
                spA8.roll = 0;
            } else {
                spA8.yaw = arg0->srt.yaw;
                spA8.pitch = arg0->srt.pitch;
                spA8.roll = arg0->srt.roll;
            }
            spA8.scale = 1.0f;
            matrix_from_srt(&spC0, &spA8);
            temp_fs0 = sp11C[i].unk00 * 2;
            temp_fs1 = sp11C[i].unk02 * 2;
            temp_fs2 = sp11C[i].unk04 * 2;
            vec3_transform(&spC0, temp_fs0, temp_fs1, temp_fs2, &sp118[i].drawPoint.f[0], &sp118[i].drawPoint.f[1], &sp118[i].drawPoint.f[2]);
            temp_fs0 = sp11C[i].unk06 * 2;
            temp_fs1 = sp11C[i].unk08 * 2;
            temp_fs2 = sp11C[i].unk0a * 2;
            vec3_transform(&spC0, temp_fs0, temp_fs1, temp_fs2, &sp118[i].refPoint.f[0], &sp118[i].refPoint.f[1], &sp118[i].refPoint.f[2]);
        }
    }
}

#ifndef NON_MATCHING
void objprint_func_80036890(Object* arg0, s32 arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/objprint/objprint_func_80036890.s")
#else
// https://decomp.me/scratch/cjUgO
void objprint_func_80036890(Object* arg0, s32 arg1) {
    s32 sp6C; // a1
    Gfx* temp_a1_2;
    s32 sp64;
    Gfx* temp_t0;
    Model* temp_t5;
    ModelFacebatch* temp_t4;
    ModelInstance* temp_s2;
    Texture* temp_a2;
    Texture* var_v1;
    Vtx* temp_a1_4;
    Vtx* temp_v0_2;
    Vtx* var_v0_2;
    Vtx* var_a2;
    Vtx* var_s6;
    u8 *sp34;
    Vtx* var_v1_3;
    s16 temp_t3;
    u8 temp_s3;
    u16 temp_t6_2;
    s32 temp_v0;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_s0;
    s32 var_s1;
    s32 var_v0;
    TextureAnimation* temp_v1_2;
    s16 var_t1;
    s16 var_t2;
    s32 new_var_2;
    u8 *var_s7;

    var_s6 = arg0->unk70;
    temp_t5 =  arg0->modelInsts[arg0->modelInstIdx]->model;
    sp34 = arg0->def->pTextures;
    sp64 = arg0->def->numAnimatedFrames;
    temp_s2 = arg0->modelInsts[arg0->modelInstIdx];
    for (sp6C = 0; sp6C < sp64; sp6C++, var_s6++) {
        var_t1 = var_s6->n.tc[0];
        var_t2 = var_s6->n.tc[1];
        var_s7 = sp34;
        var_s7 += sp6C << 1;
        var_s7++;
        for (var_s0 = 0; var_s0 < temp_t5->textureAnimationCount; var_s0++) {
            temp_t4 = &temp_t5->faces[temp_t5->textureAnimations[var_s0].unkB];
            if (temp_t4->tagB == var_s7[0]) {
                if (temp_t4->materialID != 0xFF) {
                    temp_t3 = temp_t5->textureAnimations[var_s0].unk4;
                    temp_a2 = temp_t5->materials[temp_t4->materialID].texture;
                    var_a0 = (((s32*)var_s6)[0] >> 8) & 0xFF;
                    temp_s3 = (((s32*)var_s6)[0]) & 0xFF;
                    temp_t6_2 = temp_a2->animDuration >> 8;
                    if (temp_t3 >= 0) {
                        if (var_a0 >= temp_t6_2) {
                            var_a0 = temp_t6_2 - 1;
                        }

                        for (var_v1 = temp_a2, var_v0 = 0; var_v0 < var_a0 && var_v1 != NULL; var_v0++, var_v1 = var_v1->next);

                        temp_a1_2 = &temp_s2->displayList[new_var_2 = temp_t3];
                        temp_t0 = var_v1->gdl;
                        gSPDisplayList(temp_a1_2, OS_PHYSICAL_TO_K0(temp_t0));
                        temp_t0 += var_v1->gdl2Offset + 1;
                        if (temp_t6_2 >= 2) {
                            var_a0++;
                            if (temp_a2->flags & 0x40) {
                                if (var_a0 >= temp_t6_2) {
                                    if (temp_a2->unk1A != 0) {
                                        var_a0 = 0;
                                    } else {
                                        var_a0 = temp_t6_2 - 1;
                                    }
                                }
                                var_v1 = temp_a2;
                                for (var_v0 = 0; var_v0 < var_a0 && var_v1 != NULL; var_v0++, var_v1 = var_v1->next);
                                temp_a1_2++;
                                temp_a1_2->words.w1 = (unsigned int)OS_PHYSICAL_TO_K0(var_v1 + 1);
                                temp_a1_2++;
                                gSPDisplayList(temp_a1_2, OS_PHYSICAL_TO_K0(temp_t0));
                                temp_a1_2++;
                                gDPSetEnvColor(temp_a1_2, temp_s3, temp_s3, temp_s3, 0);
                            }
                        }
                    }
                    temp_v0_2 = temp_s2->vertices[((temp_s2->unk34 >> 1) & 1) ^ 1];
                    var_v1_3 = &temp_t5->vertices[temp_t4->baseVertexID];
                    temp_a1_4 = &temp_v0_2[temp_t4[1].baseVertexID];
                    var_a2 = &temp_v0_2[temp_t4->baseVertexID];
                    while ((u32) var_a2 < (u32) temp_a1_4) {
                        var_a2->n.tc[0] = var_v1_3->v.tc[0] + var_t1;
                        var_a2->n.tc[1] = var_v1_3->v.tc[1] + var_t2;
                        var_v1_3 += 1;
                        var_a2 += 1;
                    }
                }
            }
        }
    }
}

#endif

void objprint_func_80036B78(Object* arg0, Gfx** arg1, Mtx** arg2, s32 arg3) {
    s32 sp6C;
    s32 sp68;
    s32 sp64;
    f32 sp60;
    Gfx* temp_v1;
    SRT sp44;
    ModelInstance* temp_t0;

    sp6C = arg0->srt.flags;
    sp6C |= 0x80;
    sp6C |= 8;
    sp64 = 0;
    sp68 = arg0->opacityWithFade;
    if (sp68 > 0xFF) {
        sp68 = 0xFF;
    }
    if (sp68 < 0xFF) {
        sp6C |= 4;
        sp64 = 1;
    }
    if ((arg0->shadow != NULL) && (arg0->shadow->gdl != NULL)) {
        sp44.transl.f[0] = arg0->srt.transl.f[0];
        sp44.transl.f[1] = 0.0f;
        sp44.transl.f[2] = arg0->srt.transl.f[2];
        sp44.yaw = 0;
        sp44.roll = 0;
        sp44.pitch = 0;
        sp44.scale = 1.0f;
        camSetupObjectSRTMatrix(arg1, arg2, &sp44, 1.0f, 0.0f, NULL);
        gSPDisplayList((*arg1)++, OS_PHYSICAL_TO_K0(arg0->shadow->gdl));
        dlSetAllDirty();
        texRenderReset();
        sp64 = 1;
        if (sp68 == 0xFF) {
            sp64 = 2;
        }
    }
    if (arg0->def->flags & OBJDEF_FLAG1000) {
        if ((sp64 == 1) || (arg0->def->flags & OBJDEF_FLAG20)) {
            func_8001F848(arg1);
        }
    } else {
        if (arg0->def->flags & OBJDEF_FLAG20) {
            dlSetPrimColor(arg1, 0xFFU, 0xFFU, 0xFFU, sp68);
        } else if (sp64 != 0) {
            func_8001F848(arg1);
        }
        temp_t0 = arg0->modelInsts[arg0->modelInstIdx];
        if (arg0->setup != NULL) {
            switch (arg0->setup->objId) {                    /* irregular */
            case OBJ_TriggerPoint:
            case OBJ_TriggerPlane:
            case OBJ_TriggerArea:
            case OBJ_TriggerTime:
            case OBJ_TriggerButton:
            case OBJ_TriggerSetup:
            case OBJ_TriggerBits:
            case OBJ_TriggerCurve:
            case OBJ_TriggerCylinder:
                sp60 = arg0->srt.scale;
                arg0->srt.scale = 2.0f;
                camCheckConvexHull(arg1, arg2, arg3, arg0, temp_t0, sp6C, 1);
                arg0->srt.scale = sp60;
                break;
            default:
                camCheckConvexHull(arg1, arg2, arg3, arg0, temp_t0, sp6C, 1);
                break;
            }
        } else {
            camCheckConvexHull(arg1, arg2, arg3, arg0, temp_t0, sp6C, 1);
        }
        if ((sp64 == 1) || (arg0->def->flags & OBJDEF_FLAG20)) {
            func_8001F848(arg1);
        }
    }
}

void objprint_func_80036E5C(Object* object, Gfx** gdl, Mtx** mtx) {
    SRT shadowTransform;

    if (object->shadow->gdl) {
        if (object->shadow->flags & OBJ_SHADOW_FLAG_CUSTOM_OBJ_POS) {
            shadowTransform.transl.x = object->shadow->tr.x;
            shadowTransform.transl.y = object->shadow->tr.y;
            shadowTransform.transl.z = object->shadow->tr.z;
        } else {
            shadowTransform.transl.x = object->srt.transl.x;
            shadowTransform.transl.y = object->srt.transl.y;
            shadowTransform.transl.z = object->srt.transl.z;
        }
        shadowTransform.yaw = 0;
        shadowTransform.roll = 0;
        shadowTransform.pitch = 0;
        shadowTransform.scale = 0.05f;

        camSetupObjectSRTMatrix(gdl, mtx, &shadowTransform, 1.0f, 0.0f, NULL);
        gSPDisplayList((*gdl)++, OS_PHYSICAL_TO_K0(object->shadow->gdl));

        if (object->parent) {
            camSetupRSPMatricesForObject(gdl, mtx, object->parent);
        }
        dlSetAllDirty();
        texRenderReset();
    }
}

/** Sets a multiplier colour (generally lowers model's brightness) */
void objprintSetMultiplierColor(s16 r, s16 g, s16 b) {
    SHORT_800b2e14 = r;
    SHORT_800b2e16 = g;
    SHORT_800b2e18 = b;
    BYTE_80091754 = TRUE;
}

/** Sets a blend colour (can increase a model's brightness) */
void objprintSetBlendColor(s16 r, s16 g, s16 b, u8 a) {
    BYTE_800b2e20 = r;
    BYTE_800b2e21 = g;
    BYTE_800b2e22 = b;
    BYTE_80091758 = TRUE;
    BYTE_800b2e23 = a & 0xFF;
}

void objprint_func_80037020(f32 aX, f32 aY, f32 aZ, f32 bX, f32 bY, f32 bZ, f32* outX, f32* outY, f32* outZ) {
    f32 dotProduct;

    dotProduct = (aX * bX) + (aY * bY) + (aZ * bZ);
    *outX = bX - (dotProduct * aX);
    *outY = bY - (dotProduct * aY);
    *outZ = bZ - (dotProduct * aZ);
}
