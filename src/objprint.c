#include "common.h"
#include "dlls/objects/common/group48.h"

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

void func_8001DF60(Object*, ModelInstance*);
void func_8001E818(Object*, Model*, ModelInstance*);
void func_8001F094(ModelInstance*);
void func_800357B4(Object*, ModelInstance*, Model*);
ModelInstance *func_80035AF4(Gfx**, Mtx**, Vertex**, Triangle**, Object*, ModelInstance*, MtxF*, MtxF*, Object*, s32, s32);
void func_80036890(Object*, s32);
void func_80036058(Object*, Object*, ModelInstance*, Gfx**, Mtx**, Vertex**);
s32 func_80003278(Gfx**, Mtx**, s32, Object*, ModelInstance*, s32, s32);
void func_8001F848(Gfx**);
// should be in model.h
void func_80019730(ModelInstance* arg0, Model* arg1, Object* arg2, MtxF* arg3);
void func_8001A8EC(ModelInstance* modelInst, Model* model, Object* obj, MtxF* arg3, Object* obj2);

void objprint_func(Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** tris, Object* arg4, s8 arg5) {
    if (arg4->unkB0 & 0x40) {
        return;
    }

    if (arg5 == 0) {
        if (arg4->objhitInfo != NULL && (arg4->objhitInfo->unk5A & 0x30)) {
            arg4->objhitInfo->unk9F = 2;
        }
    }

    if (arg4->srt.flags & 0x4000) {
        return;
    }

    if (arg4->parent != NULL && (arg4->parent->srt.flags & 0x4000)) {
        return;
    }

    update_pi_manager_array(2, arg4->id);
    dl_add_debug_info(*gdl, arg4->id, "objects/objprint.c", 0x1AAU);
    if (arg4->dll != NULL) {
        if (!(arg4->unkB0 & 0x4000)) {
            arg4->dll->vtbl->print(arg4, gdl, mtxs, vtxs, tris, arg5);
        } else if (arg5 != 0) {
            draw_object(arg4, gdl, mtxs, vtxs, tris, 1.0f);
        }
    } else if (arg5 != 0) {
        draw_object(arg4, gdl, mtxs, vtxs, tris, 1.0f);
    }
    if (arg4->unkB0 & 0x800) {
        func_80023A78(arg4, arg4->modelInsts[arg4->modelInstIdx], arg4->modelInsts[arg4->modelInstIdx]->model);
    }
    if (arg4->linkedObject != NULL && (arg4->linkedObject->unkB0 & 0x800)) {
        func_80023A78(arg4->linkedObject, arg4->linkedObject->modelInsts[arg4->modelInstIdx], arg4->linkedObject->modelInsts[arg4->modelInstIdx]->model);
    }
    dl_add_debug_info(*gdl, (u32) -arg4->id, "objects/objprint.c", 0x1E9U);
    update_pi_manager_array(2, -1);
}

void func_80034FF0(MtxF* arg0) {
    PTR_DAT_800b2e1c = arg0;
}

void draw_object(Object* obj, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** tris, f32 yPrescale) {
    s32 spFC;
    ModelInstance* modelInst;
    Model* model;
    SRT spDC;
    Object* parentObj;
    u8 spD7;
    u8 spD6;
    u8 spD5;
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
    spFC = obj->opacityWithFade;
    if (spFC > 0xFF) {
        spFC = 0xFF;
    }
    if (obj->def->flags & 0x10000) {
        if (func_8001EBE0() != 0) {
            spD5 = spD6 = spD7 = 0xFF;
        } else {
            spD7 = sp6F;
            spD6 = sp6E;
            spD5 = sp6D;
        }
    } else {
        spD5 = spD6 = spD7 = 0xFF;
    }
    if (obj->def->numAnimatedFrames > 0) {
        func_80036890(obj, 2);
    }
    if (BYTE_80091754 != 0) {
        var_v0 = (spD7 * SHORT_800b2e14) >> 8;
        var_v1 = (spD6 * SHORT_800b2e16) >> 8;
        var_a0 = (spD5 * SHORT_800b2e18) >> 8;
        if (var_v0 > 0xFF) {
            var_v0 = 0xFF;
        }
        if (var_v1 > 0xFF) {
            var_v1 = 0xFF;
        }
        if (var_a0 > 0xFF) {
            var_a0 = 0xFF;
        }
        spD7 = var_v0;
        spD6 = var_v1;
        spD5 = var_a0;
        BYTE_80091754 = 0;
    }
    if (BYTE_80091758 != 0) {
        spD7 = BYTE_800b2e20;
        spD6 = BYTE_800b2e21;
        spD5 = BYTE_800b2e22;
        BYTE_80091758 = 0;
    } else {
        BYTE_800b2e23 = 0;
    }
    parentObj = obj->parent;
    if (parentObj != NULL) {
        setup_rsp_matrices_for_object(&tempGdl, &tempMtxs, parentObj);
        spC4 = obj->srt.transl.f[0];
        spC0 = obj->srt.transl.f[1];
        spBC = obj->srt.transl.f[2];
        spBA = obj->srt.yaw;
    }
    if ((obj->unk64 != NULL) && (obj->unk64->gdl != NULL)) {
        if (obj->unk64->flags & 0x20) {
            spDC.transl.f[0] = obj->unk64->tr.f[0];
            spDC.transl.f[1] = obj->unk64->tr.f[1];
            spDC.transl.f[2] = obj->unk64->tr.f[2];
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
        func_800032C4(&tempGdl, &tempMtxs, (SRT* ) &spDC, 1.0f, 0.0f, NULL);
        gSPDisplayList(tempGdl++, OS_PHYSICAL_TO_K0(obj->unk64->gdl));
        dl_set_all_dirty();
        func_8003DB5C();
    }
    if (!(obj->srt.flags & 0x1000)) {
        if (!(modelInst->unk34 & 8)) {
            if ((model->animCount != 0) && !(model->unk71 & 2)) {
                if (PTR_DAT_800b2e1c == 0) {
                    func_8001943C(obj, &sp78, yPrescale, 0.0f);
                    func_80019730(modelInst, model, obj, &sp78);
                } else {
                    func_80019730(modelInst, model, obj, PTR_DAT_800b2e1c);
                }
            } else {
                modelInst->unk34 ^= 1;
                sp70 = modelInst->matrices[modelInst->unk34 & 1];
                if (PTR_DAT_800b2e1c == 0) {
                    func_8001943C(obj, sp70, yPrescale, 0.0f);
                } else {
                    bcopy((void* ) PTR_DAT_800b2e1c, sp70, 0x40);
                }
                add_matrix_to_pool(sp70, 1);
            }
            modelInst->unk34 ^= 2;
            if ((obj->def->flags & 0x10) || (model->blendshapes != NULL)) {
                if (model->blendshapes != NULL) {
                    func_8001B100(modelInst);
                }
                if (obj->def->flags & 0x10) {
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
                func_80036438(obj);
            }
            if (model->hitSphereCount != 0) {
                func_8001A8EC(modelInst, model, obj, 0, obj);
            } else if ((obj->objhitInfo != NULL) && (obj->objhitInfo->unk5A & 0x20)) {
                if (obj->objhitInfo->unk9F != 0) {
                    obj->objhitInfo->unk9F--;
                }
            }
        }
        if (model->unk71 & 4) {
            dl_set_env_color(&tempGdl, spD7, spD6, spD5, BYTE_800b2e23);
        }
        dl_set_prim_color(&tempGdl, spD7, spD6, spD5, spFC);
        if (!(obj->srt.flags & 0x200)) {
            gMoveWd(tempGdl++, 6, 0xC, modelInst->matrices[modelInst->unk34 & 1]);
            gMoveWd(tempGdl++, 6, 0x14, modelInst->vertices[((s32) modelInst->unk34 >> 1) & 1]);
            if (spFC == 0xFF) {
                if (modelInst->unk34 & 0x10) {
                    load_model_display_list(model, modelInst);
                    modelInst->unk34 ^= 0x10;
                }
            } else if (!(modelInst->unk34 & 0x10)) {
                load_model_display_list2(model, modelInst);
                modelInst->unk34 ^= 0x10;
            }
            gSPDisplayList(tempGdl++, OS_PHYSICAL_TO_K0(modelInst->displayList));
            dl_set_all_dirty();
            func_8003DB5C();
        }
        if (obj->linkedObject != NULL) {
            func_80035AF4(&tempGdl, &tempMtxs, &tempVtxs, &tempTris, obj, modelInst, &sp78, 0, obj->linkedObject, obj->unkB0 & 3, (u8)spFC);
        }
    }
    if ((obj->objhitInfo != NULL) && (obj->objhitInfo->unk5A & 0x20) && (modelInst->unk14 != NULL)) {
        func_800357B4(obj, modelInst, modelInst->model);
    }
    modelInst->unk34 |= 8;
    if (parentObj != NULL) {
        obj->srt.transl.f[0] = spC4;
        obj->srt.transl.f[1] = spC0;
        obj->srt.transl.f[2] = spBC;
        obj->srt.yaw = spBA;
        func_80004224(&tempGdl);
    }
    *gdl = tempGdl;
    *mtxs = tempMtxs;
    *vtxs = tempVtxs;
    *tris = tempTris;
}

void func_800357B4(Object* arg0, ModelInstance* arg1, Model* arg2) {
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

void func_800359D0(Object *obj, Gfx **gdl, Mtx **rspMtxs, u32 param_4, u32 param_5, u32 param_6)
{
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
    gSPSegment(mygdl++, 3, d);
    gSPSegment(mygdl++, 5, modelInst2->vertices[(modelInst2->unk34 >> 1) & 0x1]);
    gSPDisplayList(mygdl++, OS_K0_TO_PHYSICAL(modelInst2->displayList));

    dl_set_all_dirty();
    func_8003DB5C();

    *gdl = mygdl;
    *rspMtxs = outRspMtxs;
}

ModelInstance *func_80035AF4(Gfx** arg0, Mtx** arg1, Vertex** arg2, Triangle** arg3, Object* arg4, ModelInstance* arg5, MtxF* arg6, MtxF* arg7, Object* arg8, s32 arg9, s32 arg10) {
    MtxF* sp74;
    s32 sp70;
    ModelInstance* modelInst;
    MtxF* var_v1;
    Model* sp64;
    SRT sp4C;
    s32 i;

    D_800B2E10 = 0;

    if (arg8->def->numAnimatedFrames > 0) {
        func_80036890(arg8, 2);
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
            func_80019730(modelInst, sp64, arg8, arg6);
            sp74 = modelInst->matrices[modelInst->unk34 & 1];
            func_80036058(arg8, arg4, modelInst, arg0, arg1, arg2);
        } else {
            modelInst->unk34 ^= 1;
            arg7 = modelInst->matrices[modelInst->unk34 & 1];
            for (i = 0; i < 16; i++) {
                ((f32*)arg7->m)[i] = ((f32*)arg6->m)[i];
            }
            func_80036058(arg8, arg4, modelInst, arg0, arg1, arg2);
            add_matrix_to_pool(arg7, 1);
            D_800B2E10 = sp74 = arg7;
        }
        modelInst->unk34 ^= 2;
        if ((arg8->def->flags & 0x10) || (sp64->blendshapes != NULL)) {
            if (sp64->blendshapes != NULL) {
                func_8001B100(modelInst);
            }
            func_8001DF60(arg8, modelInst);
        }
        if (sp64->envMapCount != 0) {
            func_8001F094(modelInst);
        }
        if (sp64->hitSphereCount != 0) {
            func_8001A8EC(modelInst, sp64, arg8, arg7, arg4);
        }
        if (!(arg4->srt.flags & 0x200)) {
            gMoveWd((*arg0)++, 6, 0xC, sp74);
            gMoveWd((*arg0)++, 6, 0x14, modelInst->vertices[(modelInst->unk34 >> 1) & 1]);
            if ((u8) arg10 == 0xFF) {
                if (modelInst->unk34 & 0x10) {
                    load_model_display_list(sp64, modelInst);
                    modelInst->unk34 ^= 0x10;
                }
            } else if (!(modelInst->unk34 & 0x10)) {
                load_model_display_list2(sp64, modelInst);
                modelInst->unk34 ^= 0x10;
            }
            gSPDisplayList((*arg0)++, OS_PHYSICAL_TO_K0(modelInst->displayList));
            dl_set_all_dirty();
            func_8003DB5C();
            // @fake
            if (D_800B2E10) {}
        }
        arg8->srt.transl.f[0] = D_800B2E10->m[3][0];
        arg8->srt.transl.f[1] = D_800B2E10->m[3][1];
        arg8->srt.transl.f[2] = D_800B2E10->m[3][2];
        if (arg8->parent != NULL) {
            transform_point_by_object(arg8->srt.transl.f[0], arg8->srt.transl.f[1], arg8->srt.transl.f[2], arg8->positionMirror.f, &arg8->positionMirror.f[1], &arg8->positionMirror.f[2], arg8->parent);
        } else {
            arg8->srt.transl.f[0] += gWorldX;
            arg8->srt.transl.f[2] += gWorldZ;
            arg8->positionMirror.f[0] = arg8->srt.transl.f[0];
            arg8->positionMirror.f[1] = arg8->srt.transl.f[1];
            arg8->positionMirror.f[2] = arg8->srt.transl.f[2];
        }
        if (arg8->def->numAttachPoints >= 2 && arg8->group == GROUP_UNK48) {
            if (arg8->parent != NULL) {
                func_80004224(arg0);
            }
            ((DLL_IGROUP_48 *)arg8->dll)->vtbl->func10(arg8, arg0, arg1, arg2, arg3);
            if (arg8->parent != NULL) {
                setup_rsp_matrices_for_object(arg0, arg1, arg8->parent);
            }
        }
    }

    return modelInst;
}

void func_80036058(Object* obj, Object* otherObj, ModelInstance* modelInst, Gfx** gdl, Mtx** mtxs, Vertex** vtxs) {
    MtxF* tempMtx;
    s32 j;
    s32 boneId;
    s32 i;
    Vec3f sp7C;
    Vec3f sp70;
    ObjGroup48_Data* objData;

    if (obj->def->numAttachPoints < 2 || obj->group != GROUP_UNK48) {
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
                transform_point_by_object(sp7C.f[0], sp7C.f[1], sp7C.f[2], &sp7C.f[0], &sp7C.f[1], &sp7C.f[2], otherObj->parent);
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
                transform_point_by_object(sp70.f[0], sp70.f[1], sp70.f[2], &sp70.f[0], &sp70.f[1], &sp70.f[2], otherObj->parent);
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

void func_80036438(Object* arg0) {
    ObjDefStruct40* sp11C;
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

    sp11C = arg0->def->unk40;
    sp118 = arg0->unk74;
    if (arg0->unkAF & 0x28) {
        return;
    }
    spA4 = arg0->modelInsts[arg0->modelInstIdx];
    for (i = 0; i < arg0->def->unk9b; i++) {
        mtxId = sp11C[i].unk11[arg0->modelInstIdx];
        if (mtxId >= 0) {
            temp_s4 = (MtxF *)&((f32*)spA4->matrices[spA4->unk34 & 1])[mtxId << 4];
            temp_fs0 = sp11C[i].unk00 * 2;
            temp_fs1 = sp11C[i].unk02 * 2;
            temp_fs2 = sp11C[i].unk04 * 2;
            if (sp11C->unk10 & 0x10) {
                vec3_transform(temp_s4, 0.0f, 0.0f, 0.0f, &sp118[i].unk0.f[0], &sp118[i].unk0.f[1], &sp118[i].unk0.f[2]);
                sp118[i].unkC.f[0] += temp_fs0;
                sp118[i].unkC.f[1] += temp_fs1;
                sp118[i].unkC.f[2] += temp_fs2;
            } else {
                vec3_transform(temp_s4, temp_fs0, temp_fs1, temp_fs2, &sp118[i].unk0.f[0], &sp118[i].unk0.f[1], &sp118[i].unk0.f[2]);
            }
            if (arg0->parent == NULL) {
                sp118[i].unk0.f[0] += gWorldX;
                sp118[i].unk0.f[2] += gWorldZ;
            } else {
                transform_point_by_object_matrix(&sp118[i].unk0, &sp118[i].unk0, arg0->parent->matrixIdx);
            }
            temp_fs0 = sp11C[i].unk06 * 2;
            temp_fs1 = sp11C[i].unk08 * 2;
            temp_fs2 = sp11C[i].unk0a * 2;
            vec3_transform(temp_s4, temp_fs0, temp_fs1, temp_fs2, &sp118[i].unkC.f[0], &sp118[i].unkC.f[1], &sp118[i].unkC.f[2]);
            if (arg0->parent == NULL) {
                sp118[i].unkC.f[0] += gWorldX;
                sp118[i].unkC.f[2] += gWorldZ;
            } else {
                transform_point_by_object_matrix(&sp118[i].unkC, &sp118[i].unkC, arg0->parent->matrixIdx);
            }
        } else {
            spA8.transl.f[0] = arg0->positionMirror.f[0];
            spA8.transl.f[1] = arg0->positionMirror.f[1];
            spA8.transl.f[2] = arg0->positionMirror.f[2];
            if (sp11C->unk10 & 0x10) {
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
            vec3_transform(&spC0, temp_fs0, temp_fs1, temp_fs2, &sp118[i].unk0.f[0], &sp118[i].unk0.f[1], &sp118[i].unk0.f[2]);
            temp_fs0 = sp11C[i].unk06 * 2;
            temp_fs1 = sp11C[i].unk08 * 2;
            temp_fs2 = sp11C[i].unk0a * 2;
            vec3_transform(&spC0, temp_fs0, temp_fs1, temp_fs2, &sp118[i].unkC.f[0], &sp118[i].unkC.f[1], &sp118[i].unkC.f[2]);
        }
    }
}

#ifndef NON_EQUIVALENT
void func_80036890(Object* arg0, s32 arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/objprint/func_80036890.s")
#else
// https://decomp.me/scratch/cjUgO
// size: 0xC
typedef struct {
    u32 pad0;
    s32 unk4; // display list index?
    u8 pad8[0xB-8];
    s8 unkB;
} TextureAnimation;

// size: 0x10
typedef struct {
    s32 unk0;
    s32 pad4;
    s16 unk8;
    s16 unkA;
    s32 padC;
} UnkS6;

void func_80036890(Object* arg0, s32 arg1) {
    s32 i; // sp6C
    Gfx* temp_a1_2;
    s32 sp64;
    Gfx* temp_t0;
    Model* temp_t5;
    ModelFacebatch* temp_t4;
    ModelInstance* modelInst;
    Texture* temp_a2;
    Texture* var_v1;
    Vtx* temp_a1_4;
    Vtx* temp_v0_2;
    Vtx* var_a2;
    Vtx* var_v1_3;
    s32 temp_s3;
    u16 temp_t6_2;
    u16 *sp34;
    s32 var_a0;
    s32 j;
    s32 k;
    TextureAnimation* temp_v1_2;
    UnkS6* var_s6;

    modelInst = arg0->modelInsts[arg0->modelInstIdx];
    temp_t5 = modelInst->model;
    sp34 = arg0->def->pTextures;
    sp64 = arg0->def->numAnimatedFrames;
    var_s6 = arg0->unk70;
    for (i = 0; i < sp64; i++) {
        for (j = 0; j < temp_t5->textureAnimationCount; j++) {
            temp_v1_2 = &((TextureAnimation*)temp_t5->textureAnimations)[j];
            temp_t4 = &temp_t5->faces[temp_v1_2->unkB];
            if (temp_t4->tagB == (u8)sp34[i]) {
                if (temp_t4->materialID != 0xFF) {
                    temp_s3 = var_s6->unk0;
                    temp_a2 = temp_t5->materials[temp_t4->materialID].texture;
                    var_a0 = (temp_s3 >> 8) & 0xFF;
                    temp_s3 &= 0xFF;
                    temp_t6_2 = temp_a2->levels >> 8;
                    if (temp_v1_2->unk4 >= 0) {
                        if (var_a0 >= temp_t6_2) {
                            var_a0 = temp_t6_2 - 1;
                        }
                        var_v1 = temp_a2;
                        k = 0;
                        while (k < var_a0 && var_v1 != NULL) {
                            k++;
                            var_v1 = var_v1->next;
                        }
                        temp_t0 = var_v1->gdl;
                        temp_a1_2 = &modelInst->displayList[temp_v1_2->unk4];
                        gSPDisplayList(temp_a1_2, OS_PHYSICAL_TO_K0(temp_t0));
                        temp_t0 += var_v1->gdlIdx + 1;
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
                                k = 0;
                                while (k < var_a0 && var_v1 != NULL) {
                                    k++;
                                    var_v1 = var_v1->next;
                                }
                                (temp_a1_2 + 1)[0].words.w1 = (u32)OS_PHYSICAL_TO_K0(var_v1 + 1);
                                temp_a1_2 += 2;
                                gSPDisplayList(temp_a1_2, OS_PHYSICAL_TO_K0(temp_t0));
                                temp_a1_2++;
                                gDPSetEnvColor(temp_a1_2, temp_s3, temp_s3, temp_s3, 0);
                            }
                        }
                    }
                    temp_v0_2 = modelInst->vertices[(((s32) modelInst->unk34 >> 1) & 1) ^ 1];
                    temp_a1_4 = &temp_v0_2[temp_t4[1].baseVertexID];
                    var_a2 = &temp_v0_2[temp_t4->baseVertexID];
                    var_v1_3 = &temp_t5->vertices[temp_t4->baseVertexID];
                    // while current vertex ID < next vertex ID
                    while ((u32) var_a2 < (u32) temp_a1_4) {
                        var_a2->v.tc[0] = var_v1_3->v.tc[0] + var_s6->unk8;
                        var_a2->v.tc[1] = var_v1_3->v.tc[1] + var_s6->unkA;
                        var_a2 += 1;
                        var_v1_3 += 1;
                    }
                }
            }
        }
        var_s6 += 1;
    }
}
#endif

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/objprint/func_80036B78.s")
#else
// https://decomp.me/scratch/9dRNa
void func_80036B78(Object* arg0, Gfx** arg1, Mtx** arg2, s32 arg3) {
    s32 sp6C;
    s32 sp68;
    s32 sp64;
    f32 sp60;
    Gfx* temp_v1;
    SRT sp44;
    ModelInstance* temp_t0;

    sp6C = arg0->srt.flags | 0x88;
    sp68 = arg0->opacityWithFade & 0xFF;
    sp64 = 0;
    if (sp68 > 0xFF) {
        sp68 = 0xFF;
    }
    if (sp68 < 0xFF) {
        sp6C |= 4;
        sp64 = 1;
    }
    if ((arg0->unk64 != NULL) && (arg0->unk64->gdl != NULL)) {
        sp44.transl.f[0] = arg0->srt.transl.f[0];
        sp44.transl.f[1] = 0.0f;
        sp44.transl.f[2] = arg0->srt.transl.f[2];
        sp44.yaw = 0;
        sp44.roll = 0;
        sp44.pitch = 0;
        sp44.scale = 1.0f;
        func_800032C4(arg1, arg2, &sp44, 1.0f, 0.0f, NULL);
        gSPDisplayList((*arg1)++, OS_PHYSICAL_TO_K0(arg0->unk64->gdl));
        dl_set_all_dirty();
        func_8003DB5C();
        sp64 = 1;
        if (sp68 == 0xFF) {
            sp64 = 2;
        }
    }
    if (arg0->def->flags & 0x1000) {
        if ((sp64 == 1) || (arg0->def->flags & 0x20)) {
            func_8001F848(arg1);
        }
    } else {
        if (arg0->def->flags & 0x20) {
            dl_set_prim_color(arg1, 0xFFU, 0xFFU, 0xFFU, sp68);
        } else if (sp64 != 0) {
            func_8001F848(arg1);
        }
        temp_t0 = arg0->modelInsts[arg0->modelInstIdx];
        if (arg0->setup != NULL) {
            switch (arg0->setup->objId) {                    /* irregular */
            case 0x4B:
            case 0x4C:
            case 0x4D:
            case 0x4E:
            case 0x4F:
            case 0x50:
            case 0x54:
            case 0xF4:
            case 0x230:
                sp60 = arg0->srt.scale;
                arg0->srt.scale = 2.0f;
                func_80003278(arg1, arg2, arg3, arg0, temp_t0, sp6C, 1);
                arg0->srt.scale = sp60;
                break;
            default:
                func_80003278(arg1, arg2, arg3, arg0, temp_t0, sp6C, 1);
                break;
            }
        } else {
            func_80003278(arg1, arg2, arg3, arg0, temp_t0, sp6C, 1);
        }
        if ((sp64 == 1) || (arg0->def->flags & 0x20)) {
            func_8001F848(arg1);
        }
    }
}
#endif

void func_80036E5C(Object* object, Gfx** gdl, Mtx** mtx) {
    SRT shadowTransform;

    if (object->unk64->gdl) {
        if (object->unk64->flags & 0x20) {
            shadowTransform.transl.x = object->unk64->tr.x;
            shadowTransform.transl.y = object->unk64->tr.y;
            shadowTransform.transl.z = object->unk64->tr.z;
        } else {
            shadowTransform.transl.x = object->srt.transl.x;
            shadowTransform.transl.y = object->srt.transl.y;
            shadowTransform.transl.z = object->srt.transl.z;
        }
        shadowTransform.yaw = 0;
        shadowTransform.roll = 0;
        shadowTransform.pitch = 0;
        shadowTransform.scale = 0.05f;

        func_800032C4(gdl, mtx, (SRT* ) &shadowTransform, 1.0f, 0.0f, NULL);
        gSPDisplayList((*gdl)++, OS_PHYSICAL_TO_K0(object->unk64->gdl));

        if (object->parent) {
            setup_rsp_matrices_for_object(gdl, mtx, object->parent);
        }
        dl_set_all_dirty();
        func_8003DB5C();
    }
}

/** Sets a multiplier colour (generally lowers model's brightness) */
void func_80036F6C(s16 r, s16 g, s16 b) {
    SHORT_800b2e14 = r;
    SHORT_800b2e16 = g;
    SHORT_800b2e18 = b;
    BYTE_80091754 = TRUE;
}

/** Sets a blend colour (can increase a model's brightness) */
void func_80036FBC(s16 r, s16 g, s16 b, u8 a) {
    BYTE_800b2e20 = r;
    BYTE_800b2e21 = g;
    BYTE_800b2e22 = b;
    BYTE_80091758 = TRUE;
    BYTE_800b2e23 = a & 0xFF;
}

void func_80037020(f32 aX, f32 aY, f32 aZ, f32 bX, f32 bY, f32 bZ, f32* outX, f32* outY, f32* outZ) {
    f32 dotProduct;

    dotProduct = (aX * bX) + (aY * bY) + (aZ * bZ);
    *outX = bX - (dotProduct * aX);
    *outY = bY - (dotProduct * aY);
    *outZ = bZ - (dotProduct * aZ);
}
