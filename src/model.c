#include "common.h"

#define ALIGN8(a) (((u32) (a) & ~0x7) + 0x8)
#define ALIGN16(a) (((u32) (a) & ~0xF) + 0x10)

typedef struct
{
/*0000*/    s32 id;
/*0004*/    Model *model;
} ModelSlot;

typedef struct
{
    // TODO
/*0000*/    u32 unk_0x0;
/*0004*/    u32 unk_0x4;
/*0008*/    u32 unk_0x8;
/*000C*/    u32 unk_0xc;
/*0010*/    u32 unk_0x10;
/*0014*/    u32 unk_0x14;
/*0018*/    u32 unk_0x18;
} ModelStats;

typedef struct
{
/*0000*/    s16 animCount;
/*0002*/    s16 unk_0x2;
/*0004*/    s16 unk_0x4;
/*0006*/    s16 unk_0x6;
/*0008*/    u32 uncompressedSize; // CAUTION: little-endian
} ModelHeader;

extern s16 *SHORT_ARRAY_800b17d0;

#pragma GLOBAL_ASM("asm/nonmatchings/model/init_models.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80017D2C.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/model_load_create_instance.s")
#else
#define MAX_LOADED_MODELS 70
extern void *gAuxBuffer;
extern s32 gNumLoadedModels;
extern s32 gNumModelsTabEntries;
extern s32 *gFile_MODELS_TAB;
extern s32 gNumFreeModelSlots;
extern s32 *gFreeModelSlots;
extern ModelSlot *gLoadedModels;
void model_destroy(Model *model);
u32 read_le32(u32 *p);
s32 model_load_anim_remap_table(s32 id, s32 param_2, s32 param_3);
ModelInstance *createModelInstance(Model *model, u32 flags, s32 initial);
u32 modanim_load(Model *model, u32 id, void *modanim);
Texture *texture_load(s32 id);
void func_800186CC(Model *model);
void model_setup_anim_playback(ModelInstance *modelInst, void *param_2);
u32 align_8(u32 a0);
void inflate(void *src, void *dest);
ModelInstance *_model_load_create_instance(s32 id, u32 flags)
{
    s32 slot;
    u32 offset;
    u32 loadSize;
    u32 modelSize;
    u32 uncompressedSize;
    u8* compressedData;
    s8 fail;
    ModelHeader *header;
    s16 animCount;
    s16 unk_0x2_aligned;
    s16 unk_0x4;
    s32 i;
    s8 isOldSlot;
    s8 isNewSlot;
    Model *model;
    ModelInstance *modelInst;
    void *modanim;
    s16 unk_0x68;

    if (id < 0) {
        id = -id;
    } else {
        s16 *idbuf = gAuxBuffer;
        read_file_region(MODELIND_BIN, idbuf, id * sizeof(s16), 8);
        id = *idbuf;
    }

    // Check to see if model is already loaded
    for (i = 0; i < gNumLoadedModels; i++)
    {
        ModelSlot *modelSlot = &gLoadedModels[i];
        if (id == modelSlot->id)
        {
            model = modelSlot->model;
            modelInst = createModelInstance(model, flags, 0);
            if (modelInst != NULL)
            {
                model->refCount++;

                model_setup_anim_playback(modelInst, modelInst->unk_0x28);
                if (modelInst->unk_0x2c != 0) {
                    model_setup_anim_playback(modelInst, modelInst->unk_0x2c);
                }
            }

            return modelInst;
        }
    }

    if (id >= gNumModelsTabEntries) {
        id = 0;
    }

    isNewSlot = FALSE;
    isOldSlot = FALSE;
    if (gNumFreeModelSlots > 0) {
        gNumFreeModelSlots--;
        slot = gFreeModelSlots[gNumFreeModelSlots];
        isOldSlot = TRUE;
    } else {
        slot = gNumLoadedModels;
        gNumLoadedModels++;
        isNewSlot = TRUE;
    }

    offset = gFile_MODELS_TAB[id];
    loadSize = gFile_MODELS_TAB[id + 1] - offset;
    read_file_region(MODELS_BIN, gAuxBuffer, offset, 0x10);

    header = gAuxBuffer;
    animCount = header->animCount;
    unk_0x4 = header->unk_0x4;
    unk_0x2_aligned = align_8(header->unk_0x2);
    unk_0x68 = unk_0x2_aligned + 0x90;
    uncompressedSize = read_le32(&header->uncompressedSize);
    modelSize = model_load_anim_remap_table(id, unk_0x4, animCount);
    modelSize += uncompressedSize + 500;

    model = malloc(modelSize, 9, 0);
    if (!model) {
        if (isOldSlot) {
            gNumFreeModelSlots++;
        }

        if (isNewSlot) {
            gNumLoadedModels--;
        }
        
        return NULL;
    }

    // In order to save memory, load compressed data into the latter portion of the output buffer,
    // then decompress it in-place.
    // We must pray that inflate does not overrun its input stream.
    compressedData = (u8*)model + modelSize - loadSize - 0x10;
    if ((s32)compressedData < 0) {
        // Align to 16 bytes
        u32 align = (u32)compressedData & 0xf;
        if (align != 0) {
            align -= 0x10;
        }
        compressedData -= align;
    }

    read_file_region(MODELS_BIN, compressedData, offset, loadSize);
    inflate(compressedData + 8, model);

    // Convert offsets to pointers
    model->textures = (ModelTexture*)((u32)model->textures + (u32)model);
    model->unk_0x4 = (void*)((u32)model->unk_0x4 + (u32)model);
    model->unk_0x8 = (void*)((u32)model->unk_0x8 + (u32)model);
    model->unk_0x28 = (void*)((u32)model->unk_0x28 + (u32)model);
    model->unk_0x18 = (void*)((u32)model->unk_0x18 + (u32)model);
    model->unk_0x14 = (void*)((u32)model->unk_0x14 + (u32)model);
    model->unk_0xc = (void*)((u32)model->unk_0xc + (u32)model);
    if (model->unk_0x3c != NULL) {
        model->unk_0x3c = (void*)((u32)model + (u32)model->unk_0x3c);
    }
    if (model->unk_0x38 != NULL) {
        model->unk_0x38 = (void*)((u32)model + (u32)model->unk_0x38);
    }
    if (model->unk_0x1c != NULL) {
        model->unk_0x1c = (void*)((u32)model + (u32)model->unk_0x1c);
    }

    model->anims = NULL;

    model->unk_0x24 = 0;

    if (model->unk_0x20 != NULL) {
        model->unk_0x20 = (void*)((u32)model + (u32)model->unk_0x20);
        if (model->unk_0x50 != NULL) {
            model->unk_0x50 = (void*)((u32)model + (u32)model->unk_0x50);
        }
        if (model->unk_0x54 != NULL) {
            model->unk_0x54 = (void*)((u32)model + (u32)model->unk_0x54);
        }
    } else {
        model->unk_0x50 = NULL;
        model->unk_0x54 = NULL;
    }

    if (model->unk_0x2c != NULL) {
        model->unk_0x2c = (void*)((u32)model + (u32)model->unk_0x2c);
    }
    if (model->unk_0x34 != NULL) {
        model->unk_0x34 = (void*)((u32)model + (u32)model->unk_0x34);
    }

    model->unk_0x68 = unk_0x68;
    model->modelId = id;
    model->refCount = 1;
    model->animCount = animCount;

    model->unk_0x71 &= ~0x40;
    if (unk_0x4 != 0) {
        model->unk_0x71 |= 0x40;
    }

    fail = FALSE;
    for (i = 0; i < model->textureCount; i++)
    {
        model->textures[i].texture = texture_load(-((u32)model->textures[i].texture | 0x8000));
        if (!model->textures[i].texture) {
            fail = TRUE;
        }
    }

    if (fail) {
        goto bail;
    }

    for (i = 0; i < model->unk_0x70; i++)
    {
        if (model->unk_0x8[i].unk_0x0 != 0xff && model->unk_0x8[i].unk_0x0 >= model->textureCount) {
            goto bail;
        }
    }

    func_800186CC(model);

    modanim = (void*)align_8((u32)model + uncompressedSize);

    if (modanim_load(model, id, modanim) != 0) {
        goto bail;
    }

    modelInst = createModelInstance(model, flags, 1);
    if (!modelInst) {
        goto bail;
    }

    model_setup_anim_playback(modelInst, modelInst->unk_0x28);
    if (modelInst->unk_0x2c != 0) {
        model_setup_anim_playback(modelInst, modelInst->unk_0x2c);
    }

    gLoadedModels[slot].id = id;
    gLoadedModels[slot].model = model;

    // How strange to perform this check after the model has already been loaded.
    if (gNumLoadedModels >= MAX_LOADED_MODELS) {
        goto bail;
    }

    return modelInst;

bail:
    if (isNewSlot) {
        gNumLoadedModels--;
    }

    if (isOldSlot) {
        gNumFreeModelSlots++;
    }

    model_destroy(model);
    return NULL;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/model/createModelInstance.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_800186CC.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/model_get_stats.s")
#else
// Write information about model in stats argument, then return total size of ModelInstance
u32 _model_get_stats(Model *model, u32 flags, ModelStats *stats, u32 param_4)
{
    u32 totalSize;

    stats->unk_0x18 = (model->unk_0x66 != 0) ? (model->unk_0x6f * 0x80) : 0x80;

    stats->unk_0x0 = model->unk_0x62 * 0x20;
    stats->unk_0x4 = model->unk_0x6e * 0x20;
    stats->unk_0xc = 0;

    if (model->unk_0x71 & 0x40)
    {
        stats->unk_0x14 = model->unk_0x68;
        while (stats->unk_0x14 & 0x7) {
            stats->unk_0x14++;
        }

        stats->unk_0xc = stats->unk_0x14 * 4;
    }

    stats->unk_0x10 = 0x68;

    if (flags & 0x80)
    {
        stats->unk_0x10 = 0xd0;
        stats->unk_0xc *= 2;
    }

    if ((flags & 0x1) || model->unk_0x1c != 0 || param_4 != 0) {
        totalSize = stats->unk_0xc + stats->unk_0x0 + stats->unk_0x10 + stats->unk_0x18 + 0x30 + stats->unk_0x4;
        stats->unk_0x10 += 0x30;
    } else {
        totalSize = stats->unk_0x10 + stats->unk_0xc + stats->unk_0x18 + stats->unk_0x4;
    }

    totalSize += 0x40;

    if (model->unk_0x20 != 0 && model->unk_0x6f != 0)
    {
        if (model->unk_0x50 != 0) {
            totalSize += model->unk_0x6f * 0x1c + model->unk_0x6f * 2 + 0x1c;
        }

        totalSize += 0x2f;
    }
    else
    {
        totalSize += 0x6f;
    }

    return ALIGN16(totalSize);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/model/load_model_display_list.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_800189A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80018A00.s")

// very close
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/model_destroy.s")
#else
void texture_destroy(Texture *texture);
void anim_destroy(Animation *anim);
void _model_destroy(Model *model)
{
    s32 i;

    for (i = 0; i < model->textureCount; i++)
    {
        if (model->textures[i].texture != NULL) {
            texture_destroy(model->textures[i].texture);
        }
    }

    if (model->anims != NULL)
    {
        for (i = 0; i < model->animCount; i++) {
            anim_destroy(model->anims[i]);
        }
    }

    free(model);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/model/model_load_anim_remap_table.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/modanim_load.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/model_setup_anim_playback.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80019118.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/anim_load.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/anim_destroy.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001943C.s")
#else
extern f32 gWorldX;
extern f32 gWorldZ;
void matrix_prescale_y(MtxF *mf, f32 scale);
void func_80015BB8(s16 theta, MtxF *mf);
typedef s16 (*ActorDLLFunc0x20)(TActor *actor);
void _func_8001943C(TActor *actor, MtxF *mf, f32 yPrescale)
{
    if (actor->linkedActor == NULL) {
        actor->srt.tx -= gWorldX;
        actor->srt.tz -= gWorldZ;
    }

    if (actor->srt.tx > *(f32*)0x800994c0 /* 32767.0f */ || actor->srt.tx < *(f32*)0x800994c4 /* -32767.0f */ ||
        actor->srt.tz > *(f32*)0x800994c0 /* 32767.0f */ || actor->srt.tz < *(f32*)0x800994c4 /* -32767.0f */)
    {
        bzero(mf, sizeof(MtxF));
        mf->m[0][0] = 1.0f;
        mf->m[1][1] = 1.0f;
        mf->m[2][2] = 1.0f;
        mf->m[3][3] = 1.0f;

        if (actor->linkedActor == NULL) {
            actor->srt.tx += gWorldX;
            actor->srt.tz += gWorldZ;
        }
    }
    else
    {
        matrix_from_srt(mf, &actor->srt);

        if (actor->unk0x46 == 0x427) {
            ActorDLLFunc0x20 func = *(ActorDLLFunc0x20*)((u8*)*actor->dll + 0x20);
            s16 theta = func(actor);
            func_80015BB8(theta, mf);
        }

        if (yPrescale != 1.0f) {
            matrix_prescale_y(mf, yPrescale);
        }

        if (actor->linkedActor == NULL) {
            actor->srt.tx += gWorldX;
            actor->srt.tz += gWorldZ;
        }
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_800195F8.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80019730.s")
#else
void func_800199A8(MtxF *param_1, ModelInstance *modelInst, AnimState *animState, f32 param_4, s32 param_5);
void func_80019FC0(MtxF *param_1, ModelInstance *modelInst, AnimState *animState, f32 param_4, u32 param_5, u8 animIdx0, u8 param_7, u8 animIdx1, u32 flags, u16 param_10);
void func_8001A3FC(ModelInstance *modelInst, u32 selector, s32 idx, f32 param_4, f32 scale, Vec3f *param_6, s16 *param_7);
void func_8001CAA4(AnimState *animState, s16 *out0, s16 *out1);
void _func_80019730(ModelInstance *modelInst, Model *model, TActor *actor, MtxF *param_4)
{
    s32 mtxSelector;
    AnimState *animState0;

    func_8001A640(actor, modelInst, model);

    modelInst->unk_0x34 ^= 0x1;
    mtxSelector = modelInst->unk_0x34 & 0x1;

    animState0 = modelInst->animState0;

    if (animState0->unk_0x63 & 0x4)
    {
        short xyz[3];
        Vec3f v;
        func_8001A3FC(modelInst, 0, 0, actor->unk0x98, actor->srt.scale, &v, xyz);
        *(s16*)0x800903dc = xyz[0];
        *(s16*)0x800903de = xyz[1];
        *(s16*)0x800903e0 = xyz[2];
    }

    if (modelInst->model->unk_0x71 & 0x8)
    {
        func_800199A8(param_4, modelInst, animState0, actor->unk0x98, 0x7f);
    }
    else
    {
        if (modelInst->animState0->unk_0x63 & 0x8)
        {
            AnimState *animState1 = modelInst->animState1;

            func_80019FC0(param_4, modelInst, animState0, actor->unk0x98, 0x7f, 0, 0, 2, 0x14, animState0->unk_0x5a);
            func_80019FC0(param_4, modelInst, animState1, actor->unk0x9c, 0x7f, 0, 0, 2, 0x18, animState0->unk_0x5a);
            func_80019FC0(param_4, modelInst, animState0, actor->unk0x98, 0x7f, 0, 0, 0, 7, animState1->unk_0x58);
            func_80019FC0(param_4, modelInst, animState0, actor->unk0x98, 0x7f, 0, 1, 1, 1, animState0->unk_0x58);
        }
        else
        {
            func_800199A8(param_4, modelInst, animState0, actor->unk0x98, 0x7f);
            if (modelInst->animState1 != NULL && actor->unk_0xa2 >= 0) {
                func_800199A8(param_4, modelInst, modelInst->animState1, actor->unk0x9c, -1);
            }
        }
    }

    pointerIntArray2_func(modelInst->unk_0x4.matrices[mtxSelector], model->unk_0x6f);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/func_800199A8.s")
#else
void _func_800199A8(MtxF *param_1, ModelInstance *modelInst, AnimState *animState, f32 param_4, u32 param_5)
{
    Model *model;
    MtxF *modelMtx;
    AnimState myAnimState;
    s32 i;
    u32 flags = 0;
    s32 j;

    model = modelInst->model;
    modelMtx = modelInst->unk_0x4.matrices[modelInst->unk_0x34 & 0x1];

    animState->unk_0x4[0] = animState->unk_0x14[0] * param_4;

    if (model->unk_0x71 & 0x8)
    {
        myAnimState.anims[0] = animState->anims[0];
        myAnimState.anims[1] = animState->anims[1];
        myAnimState.anims2[0] = animState->anims2[0];
        myAnimState.anims2[1] = animState->anims2[1];

        for (i = 0; i < 2; i++)
        {
            s32 srci = 0;

            if (animState->unk_0x58 != 0) {
                srci = i;
            }

            myAnimState.animIndexes[i] = animState->animIndexes[srci];
            myAnimState.unk_0x60[i] = animState->unk_0x60[srci];
            myAnimState.unk_0x4[i] = animState->unk_0x4[srci];
        }

        myAnimState.unk_0x58 = animState->unk_0x58;

        func_8001A1D4(model, &myAnimState, 2);

        if (animState->unk_0x63 & 0x1) {
            flags |= 0x10;
        }
        if (animState->unk_0x63 & 0x4) {
            flags |= 0x20;
        }

        func_8001B4F0(&modelMtx, param_1, &myAnimState, model->unk_0x20, model->unk_0x6f, SHORT_ARRAY_800b17d0, param_5, flags | 0x40);

        return;
    }

    for (i = 0; i < 2; i++)
    {
        if (i == 0) {
            myAnimState.unk_0x58 = animState->unk_0x5a;
        } else {
            myAnimState.unk_0x58 = animState->unk_0x5c;
        }

        if (myAnimState.unk_0x58 != 0)
        {
            u32 bit;

            if (animState->unk_0x58 == 0) {
                bit = 0;
            } else {
                bit = 4 << i;
            }

            myAnimState.unk_0x60[0] = animState->unk_0x60[i];
            myAnimState.unk_0x14[0] = animState->unk_0x14[i];
            myAnimState.unk_0x4[0] = animState->unk_0x4[i];
            myAnimState.unk_0x34[0] = animState->unk_0x34[i];
            myAnimState.unk_0x60[1] = animState->unk_0x60[i];
            myAnimState.unk_0x14[1] = animState->unk_0x14[i];
            myAnimState.unk_0x4[1] = animState->unk_0x4[i];
            myAnimState.unk_0x34[1] = animState->unk_0x3c[i];

            if (model->unk_0x71 & 0x40) {
                myAnimState.animIndexes[0] = 0;
                myAnimState.animIndexes[1] = 1;
                myAnimState.anims[0] = animState->anims[animState->animIndexes[i]];
                myAnimState.anims[1] = animState->anims2[animState->animIndexes[i + 2]];
            } else {
                myAnimState.animIndexes[0] = animState->animIndexes[i];
                myAnimState.animIndexes[1] = animState->animIndexes[i + 2];
            }

            func_8001A1D4(model, &myAnimState, 2);
            func_8001B4F0(&modelMtx, param_1, &myAnimState, model->unk_0x20, model->unk_0x6f, SHORT_ARRAY_800b17d0, param_5, bit);

            if (bit != 0) {
                bit |= 1 << i;
            }
        }
    }

    if (animState->unk_0x5a != 0 || animState->unk_0x5c != 0) {
        if (flags == 0) {
            return;
        }
    }

    j = 1;
    if (animState->unk_0x58) {
        j = 2;
    }

    myAnimState.anims[0] = animState->anims[0];
    myAnimState.anims[1] = animState->anims[1];
    myAnimState.anims2[0] = animState->anims2[0];
    myAnimState.anims2[1] = animState->anims2[1];

    if (j != 0)
    {
        if (j != 1)
        {
            for (i = 0; i < j; i++)
            {
                myAnimState.animIndexes[i] = animState->animIndexes[i];
                myAnimState.unk_0x60[i] = animState->unk_0x60[i];
                myAnimState.unk_0x14[i] = animState->unk_0x14[i];
                myAnimState.unk_0x4[i] = animState->unk_0x4[i];
                myAnimState.unk_0x34[i] = animState->unk_0x34[i];
            }
            if (i == 1)
                goto lab_80019f2c;
        }

        // TODO
    }

    lab_80019f2c:
    myAnimState.unk_0x58 = animState->unk_0x58;
    func_8001A1D4(model, &myAnimState, 1);
    if (animState->unk_0x63 & 0x1) {
        flags |= 0x10;
    }
    if (animState->unk_0x63 & 0x4) {
        flags |= 0x20;
    }
    func_8001B4F0(&modelMtx, param_1, &myAnimState, model->unk_0x20, model->unk_0x6f, SHORT_ARRAY_800b17d0, param_5, flags);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80019FC0.s")
#else
// Blend between animations?
void _func_80019FC0(MtxF *param_1, ModelInstance *modelInst, AnimState *animState, f32 param_4, u32 param_5, u8 animIdx0, u8 param_7, u8 animIdx1, u32 flags, u16 param_10)
{
    Model *model = modelInst->model;
    MtxF *modelMtx = modelInst->unk_0x4.matrices[modelInst->unk_0x34 & 1];
    AnimState myAnimState;
    u8 flags2;

    if (flags & 0x10) {
        animState->unk_0x4[0] = animState->unk_0x14[0] * param_4;
    }

    myAnimState.unk_0x60[0] = animState->unk_0x60[animIdx0];
    myAnimState.unk_0x14[0] = animState->unk_0x14[animIdx0];
    myAnimState.unk_0x4[0] = animState->unk_0x4[animIdx0];
    myAnimState.unk_0x34[0] = animState->unk_0x34[animIdx0];
    myAnimState.unk_0x60[1] = animState->unk_0x60[param_7];
    myAnimState.unk_0x14[1] = animState->unk_0x14[param_7];
    myAnimState.unk_0x4[1] = animState->unk_0x4[param_7];
    myAnimState.unk_0x34[1] = animState->unk_0x34[animIdx1];

    if (model->unk_0x71 & 0x40)
    {
        myAnimState.animIndexes[0] = 0;
        myAnimState.animIndexes[1] = 1;
        myAnimState.anims[0] = animState->anims[animState->animIndexes[animIdx0]];
        if (animIdx1 < 2) {
            myAnimState.anims[1] = animState->anims[animState->animIndexes[animIdx1]];
        } else {
            myAnimState.anims[1] = animState->anims2[animState->animIndexes[animIdx1]];
        }
    }
    else
    {
        myAnimState.animIndexes[0] = animState->animIndexes[animIdx0];
        myAnimState.animIndexes[1] = animState->animIndexes[animIdx1];
    }

    if (param_10 == 0) {
        myAnimState.unk_0x58 = 1;
    } else {
        myAnimState.unk_0x58 = param_10;
    }

    func_8001A1D4(model, &myAnimState, 2);

    flags2 = flags & 0xf;
    if (!(flags & 0xc))
    {
        if (animState->unk_0x63 & 0x1) {
            flags2 |= 0x10;
        }
        if (animState->unk_0x63 & 0x4) {
            flags2 |= 0x20;
        }
    }

    func_8001B4F0(&modelMtx, param_1, &myAnimState, model->unk_0x20, model->unk_0x6f, 0x800b17d0, param_5, flags2);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001A1D4.s")
#else
void _func_8001A1D4(Model *model, AnimState *animState, s32 count)
{
    s8* amap;
    Animation *anim;
    s32 i;
    s32 j;
    s32 k;
    s16 m;
    u8 n;

    for (i = 0; i < count; i++)
    {
        if (model->unk_0x71 & 0x40) {
            amap = animState->anims[animState->animIndexes[i]];
            anim = (Animation*)(amap + 0x80);
        } else {
            amap = model->amap + animState->animIndexes[i] * ALIGN8(model->unk_0x6f);
            anim = model->anims[animState->animIndexes[i]];
        }

        for (j = 0; j < model->unk_0x6f; j++) {
            *(u8*)((u8*)model->unk_0x20 + j + i * 0x10 + 2) = amap[j];
        }

        n = animState->unk_0x34[i][2];

        k = (int)animState->unk_0x4[i] - 1;
        if (k < 0 && animState->unk_0x60[i] != 0) {
            k += (int)animState->unk_0x14[i];
        }

        animState->unk_0x4c[i][0] = n;
        animState->unk_0x4c[i][1] = n * 2;
        animState->unk_0x4c[i][2] = n * 3;

        if (k < 0) {
            k = 0;
        }

        m = (s16)animState->unk_0x4[i];
        n = animState->unk_0x34[i][2];

        animState->unk_0x2c[i] = (u8*)anim + n * k + anim->unk_0x2;

        if (m == animState->unk_0x4[i]) {
            animState->unk_0x4c[i][0] = n;
        } else {
            animState->unk_0x4c[i][0] = 0;
        }

        if (animState->unk_0x60[i] != 0 && animState->unk_0x4[i] == m - 1.0f) {
            animState->unk_0x4c[i][0] = -n * m;
        }

        animState->unk_0x2c[i] = (u8*)anim + n * m + anim->unk_0x2;
    }
}
#endif

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001A3FC.s")
#else
void _func_8001A3FC(ModelInstance *modelInst, u32 selector, s32 idx, f32 param_4, f32 scale, Vec3f *param_6, short *param_7)
{
    AnimState *animState;
    Animation *anim;
    u8 *old_unk_0x34;
    f32 unk0x14;
    s32 iunk0x14;
    u8 unk0x34;
    s16 xyz[3];

    if (selector != 0) {
        animState = modelInst->animState1;
    } else {
        animState = modelInst->animState0;
    }

    old_unk_0x34 = animState->unk_0x34[0];
    animState->unk_0x34[0] = animState->unk_0x34[idx];

    if (modelInst->model->unk_0x71 & 0x40)
    {
        if (idx >= 2) {
            anim = animState->anims2[animState->animIndexes[idx]] + 0x80;
        } else {
            anim = animState->anims[animState->animIndexes[idx]] + 0x80;
        }
    } else {
        anim = modelInst->model->anims[animState->animIndexes[idx]];
    }

    unk0x14 = animState->unk_0x14[0] * param_4;
    iunk0x14 = (int)unk0x14;
    animState->unk_0x4[0] = unk0x14;
    unk0x34 = animState->unk_0x34[0][2];
    if (iunk0x14 != unk0x14) {
        animState->unk_0x4c[0][0] = unk0x34;
    } else {
        animState->unk_0x4c[0][0] = 0;
    }

    if (animState->unk_0x60[0] != 0 && iunk0x14 == animState->unk_0x14[0] - 1.0f) {
        animState->unk_0x4c[0][0] = -unk0x34 * iunk0x14;
    }

    animState->unk_0x2c[0] = (u8*)anim + anim->unk_0x2 + unk0x34 * iunk0x14;

    func_8001CAA4(animState, xyz, param_7);

    animState->unk_0x34[0] = old_unk_0x34;

    param_6->x = xyz[0] / 32.0f;
    param_6->y = xyz[1] / 32.0f;
    param_6->z = xyz[2] / 32.0f;
    param_6->x += modelInst->model->unk_0x20->x;
    param_6->y += modelInst->model->unk_0x20->y;
    param_6->z += modelInst->model->unk_0x20->z;
    param_6->x *= scale;
    param_6->y *= scale;
    param_6->z *= scale;

}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001A640.s")
#else
void _func_8001A640(TActor *actor, ModelInstance *modelInst, Model *model)
{
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 n;
    s8* amap;

    if (model->unk_0x71 & 0x40) {
        amap = modelInst->animState0->anims[modelInst->animState0->animIndexes[0]];
    } else {
        amap = model->amap + modelInst->animState0->animIndexes[0] * ALIGN8(model->unk_0x6f);
    }

    j = 0;
    m = 0;
    for (i = 0; i < actor->ptr0x50->unk_0x72; i++)
    {
        if (actor->ptr0x50->unk_0x10[actor->modelInstIdx + j + 1] != 0xff)
        {
            s16 *src = &actor->ptr0x6c[i * 9];
            u32 code = amap[actor->ptr0x50->unk_0x10[actor->modelInstIdx + j + 1]] << 6;

            for (k = 0; k < 3; k++)
            {
                if (src[3 * k] != 0) {
                    SHORT_ARRAY_800b17d0[m++] = 12 * k + code;
                    SHORT_ARRAY_800b17d0[m++] = src[3 * k];
                }
                if (src[3 * k + 1] != 0) {
                    SHORT_ARRAY_800b17d0[m++] = 12 * k + code + 2;
                    SHORT_ARRAY_800b17d0[m++] = src[3 * k + 1];
                }
                if (src[3 * k + 2] != 0) {
                    SHORT_ARRAY_800b17d0[m++] = 12 * k + code + 4;
                    SHORT_ARRAY_800b17d0[m++] = src[3 * k + 2];
                }
            }
        }

        j += actor->ptr0x50->unk_0x5d + 1;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001A8EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001AC44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001AE74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001AF04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001AFCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B010.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B084.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B100.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B49C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B4E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B4F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B6F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001BC00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001BCF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001BF70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001C8D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001CAA4.s")
