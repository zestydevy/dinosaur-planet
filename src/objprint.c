#include "common.h"

/** toggles Models' multiplier colour */
extern s8 BYTE_80091754;
/** toggles Models' blend colour */
extern s8 BYTE_80091758;

// -------- .bss start 800b2e10 -------- //
s32 D_800B2E10;
/** gModelColourMultiplyR? */
s16 SHORT_800b2e14;
/** gModelColourMultiplyG? */
s16 SHORT_800b2e16;
/** gModelColourMultiplyB? */
s16 SHORT_800b2e18;
s32 PTR_DAT_800b2e1c;
/** gModelColourBlendR? */
s8 BYTE_800b2e20;
/** gModelColourBlendG? */
s8 BYTE_800b2e21;
/** gModelColourBlendB? */
s8 BYTE_800b2e22;
/** gModelColourBlendA? */
s8 BYTE_800b2e23;
// -------- .bss end 800b2e30 -------- //

#pragma GLOBAL_ASM("asm/nonmatchings/objprint/objprint_func.s")

void func_80034FF0(s32 arg0) {
    PTR_DAT_800b2e1c = arg0;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/objprint/draw_object.s")
#else
extern f32 gWorldX;
extern f32 gWorldZ;
extern u8 BYTE_80091754;
extern u8 BYTE_80091758;
extern s16 SHORT_800b2e14;
extern s16 SHORT_800b2e16;
extern s16 SHORT_800b2e18;
// wtf, why don't these externs work
//extern void **PTR_DAT_800b2e1c;
#define PTR_DAT_800b2e1c (*(void***)0x800b2e1c) 
extern u8 BYTE_800b2e20;
extern u8 BYTE_800b2e21;
extern u8 BYTE_800b2e22;
//extern u8 BYTE_800b2e23;
#define BYTE_800b2e23 (*(u8*)0x800b2e23)
void func_8001F81C(u8*, u8*, u8*);
void func_8001943C(Object *obj, MtxF *mf, f32 yPrescale);
void _draw_object(Object *obj, Gfx **gdl, Mtx **rspMtxs, u32 *param_4, u32 *param_5, f32 yPrescale)
{
    ModelInstance *modelInst;
    Model *model;
    ObjectStruct50 *unk50;
    u8 r = 0xff, g = 0xff, b = 0xff, a, r_, g_, b_;
    Gfx *mygdl;
    Mtx *myrspMtxs;
    f32 tx, ty, tz;
    s16 yaw;
    ObjectStruct64* unk64;
    SRT srt;
    MtxF mtxf;
    u32 out_param_4;
    u32 out_param_5;
    u32 xxx;

    func_8001F81C(&r_, &g_, &b_);

    mygdl = *gdl;
    myrspMtxs = *rspMtxs;

    modelInst = obj->modelInsts[obj->modelInstIdx];
    if (modelInst == NULL) {
        return;
    }

    model = modelInst->model;
    unk50 = obj->ptr0x50;
    a = obj->unk37;

    // A piece of very strange code occurs here:
    //
    // 35c8c:    lbu     t9,0x37(s0) ; t9 == a
    // 35c90:    slti    at,t9,0x100
    // 35c94:    bnez    at,0x35ca0 ~>
    // 35c98:    sw      t9,0xfc(sp)
    // 35c9c:    sw      t1,0xfc(sp)
    // 35ca0:    ....
    //
    // Note the condition tested by slti/bnez, a >= 256, will never be true.

    if ((unk50->unk44 & 0x10000) || !func_8001EBE0())
    {
        r = 0xff;
        g = 0xff;
        b = 0xff;
    }
    else
    {
        r = r_;
        g = g_;
        b = b_;
    }

    if (unk50->unk71 != 0) {
        func_80036890(obj);
    }

    if (BYTE_80091754)
    {
        s32 r2 = ((s32)r * SHORT_800b2e14) >> 8;
        s32 g2 = ((s32)g * SHORT_800b2e16) >> 8;
        s32 b2 = ((s32)b * SHORT_800b2e18) >> 8;
        if (r2 > 0xff) {
            r2 = 0xff;
        }
        if (g2 > 0xff) {
            g2 = 0xff;
        }
        if (b2 > 0xff) {
            b2 = 0xff;
        }
        r = r2;
        g = g2;
        b = b2;

        BYTE_80091754 = 0;
    }

    if (BYTE_80091758)
    {
        BYTE_80091758 = 0;
        r = BYTE_800b2e20;
        g = BYTE_800b2e21;
        b = BYTE_800b2e22;
    }
    else
    {
        BYTE_800b2e23 = 0;
    }

    if (obj->parent != NULL)
    {
        setup_rsp_matrices_for_object(&mygdl, &myrspMtxs, obj->parent);
        tx = obj->srt.tx;
        ty = obj->srt.ty;
        tz = obj->srt.tz;
        yaw = obj->srt.yaw;
    }

    unk64 = obj->unk64;
    if (unk64 != NULL)
    {
        if (unk64->gdl != NULL)
        {
            if (unk64->unk30 & 0x20)
            {
                srt.tx = unk64->tx;
                srt.ty = unk64->ty;
                srt.tz = unk64->tz;
            }
            else
            {
                srt.tx = obj->srt.tx;
                srt.ty = obj->srt.ty;
                srt.tz = obj->srt.tz;
            }

            srt.yaw = 0;
            srt.roll = 0;
            srt.pitch = 0;
            srt.scale = *(f32*)0x80099d78; // 0.05f

            if (obj->parent != NULL && model->animCount != 0)
            {
                srt.tx += gWorldX;
                srt.tz += gWorldZ;
            }

            func_800032C4(&mygdl, &myrspMtxs, &srt, 1.0f, 0, NULL);

            gSPDisplayList(mygdl++, OS_K0_TO_PHYSICAL(obj->unk64->gdl));
            dl_set_all_dirty();
            func_8003DB5C();
        }
    }

    if (!(obj->srt.flags & 0x1000))
    {
        if (!(modelInst->unk34 & 0x8))
        {
            if (model->animCount != 0 && !(model->unk71 & 0x2))
            {
                if (*(MtxF**)0x800b2e1c == NULL) {
                    func_8001943C(obj, &mtxf, yPrescale);
                    func_80019730(modelInst, model, obj, &mtxf);
                } else {
                    func_80019730(modelInst, model, obj, *PTR_DAT_800b2e1c);
                }
            }
            else
            {
                MtxF *mf;
                
                modelInst->unk34 ^= 0x1;
                mf = modelInst->unk4.matrices[modelInst->unk34 & 0x1];
                if (*(MtxF**)0x800b2e1c == NULL) {
                    func_8001943C(obj, mf, yPrescale);
                } else {
                    bcopy(*PTR_DAT_800b2e1c, mf, sizeof(MtxF));
                }

                pointerIntArray2_func(mf, 1);
            }

            modelInst->unk34 ^= 0x2;

            unk50 = obj->ptr0x50;
            if ((unk50->unk44 & 0x10) || model->unk1C != NULL)
            {
                if (model->unk1C != NULL) {
                    func_8001B100(modelInst);
                }
                if (unk50->unk44 & 0x10) {
                    func_8001DF60(obj, modelInst);
                }
            }

            if (unk50->unk71) {
                func_8001E818(obj, model, modelInst);
            }

            if (model->unk74) {
                func_8001F094(modelInst);
            }

            if (obj->unk74) {
                func_80036438(obj);
            }

            if (model->unk6E != 0) {
                func_8001A8EC(modelInst, model, obj, NULL, obj);
            } else {
                if (obj->objhitInfo != NULL && (obj->objhitInfo->unk5A & 0x20) && obj->objhitInfo->unk9F != 0) {
                    obj->objhitInfo->unk9F--;
                }
            }
        }
    }

    if (model->unk71 & 0x4) {
        dl_set_env_color(&mygdl, r, g, b, BYTE_800b2e23);
    }
    dl_set_prim_color(&mygdl, r, g, b, a);

    if (!(obj->srt.flags & 0x200))
    {
        gMoveWd(mygdl++, 0x0c, 0x06, modelInst->unk4.matrices[modelInst->unk34 & 0x1]);
        gMoveWd(mygdl++, 0x14, 0x6, modelInst->unk4.unk0[(modelInst->unk34 >> 1) & 0x1]);

        if (a == 0xff)
        {
            if (modelInst->unk34 & 0x10) {
                load_model_display_list(model, modelInst);
                modelInst->unk34 ^= 0x10;
            }
        }
        else
        {
            if (!(modelInst->unk34 & 0x10)) {
                func_800189A8(model, modelInst);
                modelInst->unk34 ^= 0x10;
            }
        }

        gSPDisplayList(mygdl++, OS_K0_TO_PHYSICAL(model->displayList));
        dl_set_all_dirty();
        func_8003DB5C();
    }

    if (obj->linkedObject != NULL) {
        func_80035AF4(&mygdl, &myrspMtxs, &out_param_4, &out_param_5, obj, modelInst, &mtxf, 0, obj->linkedObject, obj->unkB0 & 0x3, a);
    }

    if (obj->objhitInfo != NULL && (obj->objhitInfo->unk5A & 0x20) && modelInst->unk14) {
        func_800357B4(obj, modelInst, modelInst->model);
    }

    modelInst->unk34 |= 0x8;

    if (obj->parent != NULL)
    {
        obj->srt.tx = tx;
        obj->srt.ty = ty;
        obj->srt.tz = tz;
        obj->srt.yaw = yaw;
        func_80004224(&mygdl);
    }

    *gdl = mygdl;
    *rspMtxs = myrspMtxs;
    *param_4 = out_param_4;
    *param_5 = out_param_5;
}
#endif

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
    gSPSegment(mygdl++, 5, modelInst2->unk4[(modelInst2->unk34 >> 1) & 0x1]);
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
