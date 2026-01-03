#include "common.h"

/** toggles Models' multiplier colour */
extern u8 BYTE_80091754;
/** toggles Models' blend colour */
extern u8 BYTE_80091758;

// -------- .bss start 800b2e10 -------- //
s32 D_800B2E10;
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
void func_80035AF4(Gfx**, Mtx**, Vertex**, Triangle**, Object*, ModelInstance*, MtxF*, s32, Object*, s32, s32);
void func_80036890(Object*, s32);
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

#pragma GLOBAL_ASM("asm/nonmatchings/objprint/func_800357B4.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/objprint/func_80035AF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objprint/func_80036058.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objprint/func_80036438.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objprint/func_80036890.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objprint/func_80036B78.s")

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
