#include "common.h"

#define ALIGN16(a) (((u32) (a) & ~0xF) + 0x10)

typedef struct
{
/*0000*/    void *texture;
/*0004*/    u32 unk_0x4;
} ModelTexture;

typedef struct
{
/*0000*/    u8 unk_0x0;
/*0001*/    u8 unk_0x1[0x10 - 0x1];
} ModelUnk0x8;

typedef struct
{
    // TODO
/*0000*/    ModelTexture *textures;
/*0004*/    void *unk_0x4;
/*0008*/    ModelUnk0x8 *unk_0x8;
/*000C*/    void *unk_0xc;
/*0010*/    void **anims;
/*0014*/    void *unk_0x14;
/*0018*/    void *unk_0x18;
/*001C*/    u32 unk_0x1c;
/*0020*/    void *unk_0x20;
/*0024*/    u32 unk_0x24;
/*0028*/    void *unk_0x28;
/*002C*/    void *unk_0x2c;
/*0030*/    u32 unk_0x30;
/*0034*/    void *unk_0x34;
/*0038*/    void *unk_0x38;
/*003C*/    void *unk_0x3c;
/*0040*/    u32 unk_0x40;
/*0044*/    u32 unk_0x44;
/*0048*/    u32 unk_0x48;
/*004C*/    u32 unk_0x4c;
/*0050*/    void *unk_0x50;
/*0054*/    void *unk_0x54;
/*0058*/    u32 unk_0x58;
/*005C*/    u32 unk_0x5c;
/*0060*/    u16 unk_0x60;
/*0062*/    s16 unk_0x62;
/*0064*/    u16 unk_0x64;
/*0066*/    s16 unk_0x66;
/*0068*/    s16 unk_0x68;
/*006A*/    s16 modelId;
/*006C*/    u8 unk_0x6c;
/*006D*/    u8 unk_0x6d;
/*006E*/    u8 unk_0x6e;
/*006F*/    u8 unk_0x6f;
/*0070*/    u8 unk_0x70;
/*0071*/    u8 unk_0x71;
/*0072*/    u8 refCount;
/*0073*/    u8 textureCount;
} Model;

typedef struct
{
    // TODO
/*0000*/    Model *model;
/*0004*/    u8 unk_0x4[0x28 - 0x4];
/*0028*/    u32 unk_0x28;
/*002C*/    u32 unk_0x2c;
} ModelInstance;

typedef struct
{
/*0000*/    s32 id;
/*0004*/    Model *model;
} LoadedModel;

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
/*0000*/    s16 unk_0x0;
/*0002*/    s16 unk_0x2;
/*0004*/    s16 unk_0x4;
/*0006*/    s16 unk_0x6;
/*0008*/    u32 uncompressedSize; // CAUTION: little-endian
} ModelHeader;


#pragma GLOBAL_ASM("asm/nonmatchings/model/init_models.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80017D2C.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/model_load_create_instance.s")
#else
#define MAX_LOADED_MODELS 70
extern void *gAuxBuffer;
extern s32 INT_800b17b4;
extern s32 gNumLoadedModels;
extern s32 *PTR_DAT_800b17a8;
extern s32 gNumModelsTabEntries;
extern s32 *gFile_MODELS_TAB;
extern LoadedModel *gLoadedModels;
void func_80018B14(Model *model);
u32 read_le32(u32 *p);
ModelInstance *createModelInstance(Model *model, u32 flags, s32 initial);
u32 modanim_load(Model *model, u32 id, void *modanim);
ModelInstance *_model_load_create_instance(s32 id, u32 flags)
{
    s32 slot;
    u32 offset;
    u32 *tab;
    u32 loadSize;
    u32 animRemapSize;
    u32 modelSize;
    u32 uncompressedSize;
    u8* compressedData;
    int isNewSlot;
    u32 uVar8;
    ModelHeader *header;
    s16 unk_0x0;
    s16 unk_0x2_aligned;
    s16 unk_0x4;
    s32 i;
    int fail;
    Model *model;
    ModelInstance *modelInst;
    void *modanim;

    if (id < 0) {
        id = -id;
    } else {
        read_file_region(MODELIND_BIN, gAuxBuffer, id * sizeof(s16), 8);
        id = *(s16*)gAuxBuffer;
    }

    // Check to see if model is already loaded
    for (i = 0; i < gNumLoadedModels; i++)
    {
        if (id == gLoadedModels[i].id)
        {
            model = gLoadedModels[i].model;
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

    if (gNumModelsTabEntries <= id) {
        id = 0;
    }

    isNewSlot = INT_800b17b4 < 1;
    if (isNewSlot) {
        slot = gNumLoadedModels;
        gNumLoadedModels++;
    } else {
        INT_800b17b4--;
        slot = PTR_DAT_800b17a8[INT_800b17b4];
    }

    tab = &gFile_MODELS_TAB[id];
    offset = *tab;
    loadSize = tab[1] - offset;
    read_file_region(MODELS_BIN, gAuxBuffer, offset, 0x10);

    header = gAuxBuffer;
    unk_0x0 = header->unk_0x0;
    unk_0x4 = header->unk_0x4;
    unk_0x2_aligned = align_8(header->unk_0x2);
    uncompressedSize = read_le32(&header->uncompressedSize);
    animRemapSize = model_load_anim_remap_table(id, unk_0x4, unk_0x0);
    modelSize = animRemapSize + uncompressedSize + 500;

    model = malloc(modelSize, 9, 0);
    if (!model) {
        if (isNewSlot) {
            gNumLoadedModels--;
        } else {
            INT_800b17b4++;
        }

        return NULL;
    }

    compressedData = (u8*)model + modelSize - loadSize - 0x10;
    uVar8 = (u32)compressedData & 0xf;
    if ((s32)compressedData < 0 && uVar8 != 0) {
        uVar8 -= 0x10;
    }
    compressedData -= uVar8;

    read_file_region(MODELS_BIN, compressedData, offset, loadSize);
    inflate(compressedData + 8, model);

    // Convert offsets to pointers
    *(u32*)&model->textures += (u32)model;
    *(u32*)&model->unk_0x4 += (u32)model;
    *(u32*)&model->unk_0x8 += (u32)model;
    *(u32*)&model->unk_0x28 += (u32)model;
    *(u32*)&model->unk_0x18 += (u32)model;
    *(u32*)&model->unk_0x14 += (u32)model;
    *(u32*)&model->unk_0xc += (u32)model;
    if (model->unk_0x3c != NULL) {
        *(u32*)&model->unk_0x3c += (u32)model;
    }
    if (model->unk_0x38 != NULL) {
        *(u32*)&model->unk_0x38 += (u32)model;
    }
    if (model->unk_0x1c != NULL) {
        *(u32*)&model->unk_0x1c += (u32)model;
    }

    model->anims = NULL;

    model->unk_0x24 = 0;

    if (model->unk_0x20 != NULL) {
        *(u32*)&model->unk_0x20 += (u32)model;
        if (model->unk_0x50 != NULL) {
            *(u32*)&model->unk_0x50 += (u32)model;
        }
        if (model->unk_0x54 != NULL) {
            *(u32*)&model->unk_0x54 += (u32)model;
        }
    } else {
        model->unk_0x50 = NULL;
        model->unk_0x54 = NULL;
    }

    if (model->unk_0x2c != NULL) {
        *(u32*)&model->unk_0x2c += (u32)model;
    }
    if (model->unk_0x34 != NULL) {
        *(u32*)&model->unk_0x34 += (u32)model;
    }

    model->unk_0x68 = unk_0x2_aligned + 0x90;
    model->refCount = 1;
    model->modelId = id;
    model->unk_0x71 &= 0xbf;
    model->unk_0x66 = unk_0x0;

    if (unk_0x4 != 0) {
        model->unk_0x71 |= 0x40;
    }

    fail = FALSE;
    for (i = 0; i < model->textureCount; i++)
    {
        void *texture = texture_load(-((u32)model->textures[i].texture | 0x8000));
        model->textures[i].texture = texture;
        if (!model->textures[i].texture) {
            fail = TRUE;
        }
    }

    if (fail) {
        goto bail;
    }

    for (i = 0; i < model->unk_0x70; i++)
    {
        if (model->unk_0x8[i].unk_0x0 != 0xff && model->textureCount <= model->unk_0x8[i].unk_0x0) {
            goto bail;
        }
    }

    func_800186CC(model);

    modanim = align_8((u32)model + uncompressedSize);

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

    if (gNumLoadedModels >= MAX_LOADED_MODELS) {
        goto bail;
    }

    return modelInst;

bail:
    if (isNewSlot) {
        gNumLoadedModels--;
    } else {
        INT_800b17b4++;
    }

    func_80018B14(model);
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

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80018950.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_800189A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80018A00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80018B14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/model_load_anim_remap_table.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/modanim_load.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/model_setup_anim_playback.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80019118.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/anim_load.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80019388.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001943C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_800195F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80019730.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_800199A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80019FC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001A1D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001A3FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001A640.s")

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
