#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/objprint/objprint_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objprint/func_80034FF0.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/objprint/draw_actor.s")
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
void func_800032C4(Gfx **gdl, Mtx **rspMtxs, SRT *srt, f32 param_4, u32 param_5, Mtx **overrideMtx);
void func_8001943C(TActor *actor, MtxF *mf, f32 yPrescale);
void _draw_actor(TActor *actor, Gfx **gdl, Mtx **rspMtxs, u32 *param_4, u32 *param_5, f32 yPrescale)
{
    ModelInstance *modelInst;
    Model *model;
    ActorUnk0x50 *unk0x50;
    u8 r = 0xff, g = 0xff, b = 0xff, a, r_, g_, b_;
    Gfx *mygdl;
    Mtx *myrspMtxs;
    f32 tx, ty, tz;
    s16 yaw;
    ActorUnk0x64* unk0x64;
    SRT srt;
    MtxF mtxf;
    u32 out_param_4;
    u32 out_param_5;
    u32 xxx;

    func_8001F81C(&r_, &g_, &b_);

    mygdl = *gdl;
    myrspMtxs = *rspMtxs;

    modelInst = actor->modelInsts[actor->modelInstIdx];
    if (modelInst == NULL) {
        return;
    }

    model = modelInst->model;
    unk0x50 = actor->ptr0x50;
    a = actor->unk_0x37;

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
        func_80036890(actor);
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

    if (actor->linkedActor != NULL)
    {
        setup_rsp_matrices_for_actor(&mygdl, &myrspMtxs, actor->linkedActor);
        tx = actor->srt.tx;
        ty = actor->srt.ty;
        tz = actor->srt.tz;
        yaw = actor->srt.yaw;
    }

    unk0x64 = actor->ptr0x64;
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
                srt.tx = actor->srt.tx;
                srt.ty = actor->srt.ty;
                srt.tz = actor->srt.tz;
            }

            srt.yaw = 0;
            srt.roll = 0;
            srt.pitch = 0;
            srt.scale = *(f32*)0x80099d78; // 0.05f

            if (actor->linkedActor != NULL && model->animCount != 0)
            {
                srt.tx += gWorldX;
                srt.tz += gWorldZ;
            }

            func_800032C4(&mygdl, &myrspMtxs, &srt, 1.0f, 0, NULL);

            gSPDisplayList(mygdl++, OS_K0_TO_PHYSICAL(actor->ptr0x64->gdl));
            dl_set_all_dirty();
            func_8003DB5C();
        }
    }

    if (!(actor->srt.flags & 0x1000))
    {
        if (!(modelInst->unk_0x34 & 0x8))
        {
            if (model->animCount != 0 && !(model->unk_0x71 & 0x2))
            {
                if (*(MtxF**)0x800b2e1c == NULL) {
                    func_8001943C(actor, &mtxf, yPrescale);
                    func_80019730(modelInst, model, actor, &mtxf);
                } else {
                    func_80019730(modelInst, model, actor, *PTR_DAT_800b2e1c);
                }
            }
            else
            {
                MtxF *mf;
                
                modelInst->unk_0x34 ^= 0x1;
                mf = modelInst->unk_0x4.matrices[modelInst->unk_0x34 & 0x1];
                if (*(MtxF**)0x800b2e1c == NULL) {
                    func_8001943C(actor, mf, yPrescale);
                } else {
                    bcopy(*PTR_DAT_800b2e1c, mf, sizeof(MtxF));
                }

                pointerIntArray2_func(mf, 1);
            }

            modelInst->unk_0x34 ^= 0x2;

            unk0x50 = actor->ptr0x50;
            if ((unk0x50->unk_0x44 & 0x10) || model->unk_0x1c != NULL)
            {
                if (model->unk_0x1c != NULL) {
                    func_8001B100(modelInst);
                }
                if (unk0x50->unk_0x44 & 0x10) {
                    func_8001DF60(actor, modelInst);
                }
            }

            if (unk0x50->unk_0x71) {
                func_8001E818(actor, model, modelInst);
            }

            if (model->unk_0x74) {
                func_8001F094(modelInst);
            }

            if (actor->unk0x74) {
                func_80036438(actor);
            }

            if (model->unk_0x6e != 0) {
                func_8001A8EC(modelInst, model, actor, NULL, actor);
            } else {
                if (actor->objhitInfo != NULL && (actor->objhitInfo->unk_0x5a & 0x20) && actor->objhitInfo->unk_0x9f != 0) {
                    actor->objhitInfo->unk_0x9f--;
                }
            }
        }
    }

    if (model->unk_0x71 & 0x4) {
        dl_set_env_color(&mygdl, r, g, b, BYTE_800b2e23);
    }
    dl_set_prim_color(&mygdl, r, g, b, a);

    if (!(actor->srt.flags & 0x200))
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

    if (actor->linkedActor2 != NULL) {
        func_80035AF4(&mygdl, &myrspMtxs, &out_param_4, &out_param_5, actor, modelInst, &mtxf, 0, actor->linkedActor2, actor->unk0xb0 & 0x3, a);
    }

    if (actor->objhitInfo != NULL && (actor->objhitInfo->unk_0x5a & 0x20) && modelInst->unk_0x14) {
        func_800357B4(actor, modelInst, modelInst->model);
    }

    modelInst->unk_0x34 |= 0x8;

    if (actor->linkedActor != NULL)
    {
        actor->srt.tx = tx;
        actor->srt.ty = ty;
        actor->srt.tz = tz;
        actor->srt.yaw = yaw;
        func_80004224(&mygdl);
    }

    *gdl = mygdl;
    *rspMtxs = myrspMtxs;
    *param_4 = out_param_4;
    *param_5 = out_param_5;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/objprint/func_800357B4.s")

void func_800359D0(TActor *actor, Gfx **gdl, Mtx **rspMtxs, u32 param_4, u32 param_5, u32 param_6)
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

    mainIdx = actor->modelInstIdx;
    if (param_6) {
        otherIdx = 0;
    } else {
        otherIdx = actor->def->numModels - 1;
    }

    modelInst = actor->modelInsts[mainIdx];
    modelInst2 = actor->modelInsts[otherIdx];

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

#pragma GLOBAL_ASM("asm/nonmatchings/objprint/func_80036E5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objprint/func_80036F6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objprint/func_80036FBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/objprint/func_80037020.s")
