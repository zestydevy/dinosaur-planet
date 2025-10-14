#include "common.h"

extern s32 PTR_DAT_800b2e1c;

extern s8 BYTE_80091754;
extern s8 BYTE_80091758;
extern s16 SHORT_800b2e14;
extern s16 SHORT_800b2e16;
extern s16 SHORT_800b2e18;
extern s8 BYTE_800b2e20;
extern s8 BYTE_800b2e21;
extern s8 BYTE_800b2e22;
extern s8 BYTE_800b2e23;

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
    ObjectStruct50 *unk0x50;
    u8 r = 0xff, g = 0xff, b = 0xff, a, r_, g_, b_;
    Gfx *mygdl;
    Mtx *myrspMtxs;
    f32 tx, ty, tz;
    s16 yaw;
    ObjectStruct64* unk0x64;
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
    unk0x50 = obj->ptr0x50;
    a = obj->unk_0x37;

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

    if ((unk0x50->unk_0x44 & 0x10000) || !func_8001EBE0())
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

    if (unk0x50->unk_0x71 != 0) {
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

    unk0x64 = obj->ptr0x64;
    if (unk0x64 != NULL)
    {
        if (unk0x64->gdl != NULL)
        {
            if (unk0x64->unk_0x30 & 0x20)
            {
                srt.tx = unk0x64->tx;
                srt.ty = unk0x64->ty;
                srt.tz = unk0x64->tz;
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

            gSPDisplayList(mygdl++, OS_K0_TO_PHYSICAL(obj->ptr0x64->gdl));
            dl_set_all_dirty();
            func_8003DB5C();
        }
    }

    if (!(obj->srt.flags & 0x1000))
    {
        if (!(modelInst->unk_0x34 & 0x8))
        {
            if (model->animCount != 0 && !(model->unk_0x71 & 0x2))
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
                
                modelInst->unk_0x34 ^= 0x1;
                mf = modelInst->unk_0x4.matrices[modelInst->unk_0x34 & 0x1];
                if (*(MtxF**)0x800b2e1c == NULL) {
                    func_8001943C(obj, mf, yPrescale);
                } else {
                    bcopy(*PTR_DAT_800b2e1c, mf, sizeof(MtxF));
                }

                pointerIntArray2_func(mf, 1);
            }

            modelInst->unk_0x34 ^= 0x2;

            unk0x50 = obj->ptr0x50;
            if ((unk0x50->unk_0x44 & 0x10) || model->unk_0x1c != NULL)
            {
                if (model->unk_0x1c != NULL) {
                    func_8001B100(modelInst);
                }
                if (unk0x50->unk_0x44 & 0x10) {
                    func_8001DF60(obj, modelInst);
                }
            }

            if (unk0x50->unk_0x71) {
                func_8001E818(obj, model, modelInst);
            }

            if (model->unk_0x74) {
                func_8001F094(modelInst);
            }

            if (obj->unk0x74) {
                func_80036438(obj);
            }

            if (model->unk_0x6e != 0) {
                func_8001A8EC(modelInst, model, obj, NULL, obj);
            } else {
                if (obj->objhitInfo != NULL && (obj->objhitInfo->unk_0x5a & 0x20) && obj->objhitInfo->unk_0x9f != 0) {
                    obj->objhitInfo->unk_0x9f--;
                }
            }
        }
    }

    if (model->unk_0x71 & 0x4) {
        dl_set_env_color(&mygdl, r, g, b, BYTE_800b2e23);
    }
    dl_set_prim_color(&mygdl, r, g, b, a);

    if (!(obj->srt.flags & 0x200))
    {
        gMoveWd(mygdl++, 0x0c, 0x06, modelInst->unk_0x4.matrices[modelInst->unk_0x34 & 0x1]);
        gMoveWd(mygdl++, 0x14, 0x6, modelInst->unk_0x4.unk_0x0[(modelInst->unk_0x34 >> 1) & 0x1]);

        if (a == 0xff)
        {
            if (modelInst->unk_0x34 & 0x10) {
                load_model_display_list(model, modelInst);
                modelInst->unk_0x34 ^= 0x10;
            }
        }
        else
        {
            if (!(modelInst->unk_0x34 & 0x10)) {
                func_800189A8(model, modelInst);
                modelInst->unk_0x34 ^= 0x10;
            }
        }

        gSPDisplayList(mygdl++, OS_K0_TO_PHYSICAL(model->displayList));
        dl_set_all_dirty();
        func_8003DB5C();
    }

    if (obj->linkedObject != NULL) {
        func_80035AF4(&mygdl, &myrspMtxs, &out_param_4, &out_param_5, obj, modelInst, &mtxf, 0, obj->linkedObject, obj->unk0xb0 & 0x3, a);
    }

    if (obj->objhitInfo != NULL && (obj->objhitInfo->unk_0x5a & 0x20) && modelInst->unk_0x14) {
        func_800357B4(obj, modelInst, modelInst->model);
    }

    modelInst->unk_0x34 |= 0x8;

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

    d = modelInst->matrices[modelInst->unk_0x34 & 1];
    gSPSegment(mygdl++, 3, d);
    gSPSegment(mygdl++, 5, modelInst2->unk_0x4[(modelInst2->unk_0x34 >> 1) & 0x1]);
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

    if (object->ptr0x64->gdl) {
        if (object->ptr0x64->flags & 0x20) {
            shadowTransform.transl.x = object->ptr0x64->tr.x;
            shadowTransform.transl.y = object->ptr0x64->tr.y;
            shadowTransform.transl.z = object->ptr0x64->tr.z;
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
        gSPDisplayList((*gdl)++, OS_PHYSICAL_TO_K0(object->ptr0x64->gdl));

        if (object->parent) {
            setup_rsp_matrices_for_object(gdl, mtx, object->parent);
        }
        dl_set_all_dirty();
        func_8003DB5C();
    }
}

void func_80036F6C(s16 arg0, s16 arg1, s16 arg2) {
    SHORT_800b2e14 = arg0;
    SHORT_800b2e16 = arg1;
    SHORT_800b2e18 = arg2;
    BYTE_80091754 = 1;
}

void func_80036FBC(s16 arg0, s16 arg1, s16 arg2, u8 arg3) {
    BYTE_800b2e20 = arg0;
    BYTE_800b2e21 = arg1;
    BYTE_800b2e22 = arg2;
    BYTE_80091758 = 1;
    BYTE_800b2e23 = arg3 & 0xFF;
}

void func_80037020(f32 aX, f32 aY, f32 aZ, f32 bX, f32 bY, f32 bZ, f32* outX, f32* outY, f32* outZ) {
    f32 productSum;

    productSum = (aX * bX) + (aY * bY) + (aZ * bZ);
    *outX = bX - (productSum * aX);
    *outY = bY - (productSum * aY);
    *outZ = bZ - (productSum * aZ);
}
