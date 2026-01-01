#include "common.h"
#include "sys/rarezip.h"

#define ALIGN8(a)  (((u32) (a) & ~0x7) + 0x8)
#define ALIGN16(a)  ((u32) (a) & ~0xF)
#define PAD16(a) while (a & 7){a++;}

static const char str_80099240[] = "Error: Model no. %d out of range on load. !!\n";
static const char str_80099270[] = "TEXTURE ERROR!!\n%d,%d\n";
static const char str_80099288[] = "Error: Model table overflow!!\n";
static const char str_800992a8[] = "WARNING :: createModelInstance called with NULL pointer\n";
static const char str_800992e4[] = "DANGER: createModelInstance: Actual size exceeded totalsize!!!\n";
static const char str_80099324[] = "ModFreeModel : NULL mod_inst!!\n";
static const char str_80099344[] = "MOD Error: Tryed to deallocate non-existent model!!\n";


#define ANIM_SLOT_RC(animationRef, idx) ((((s32*)animationRef) + (idx << 1)))[0]
#define ANIM_SLOT_ANIM(animationRef, idx) ((((s32*)animationRef) + (idx << 1)))[1]

#define MODEL_SLOT_ID(modelRef, idx) ((((s32*)modelRef) + (idx << 1)))[0]
#define MODEL_SLOT_MODEL(modelRef, idx) ((((s32*)modelRef) + (idx << 1)))[1]

extern s16 *SHORT_ARRAY_800b17d0;

void func_8001AF04(ModelInstance* modelInstance, s32 arg1, s32 shapeId, f32 arg3, s32 layer, s32 arg5);
Animation* anim_load(s16 animId, s16 modanimId, AmapPlusAnimation* anim, Model* model);
void anim_destroy(Animation*);
void model_destroy(Model* model);

// model_asm funcs
void func_800202E8(s32, Vtx*, void *, s16 *, u8*, u8*, s32, s32, s32, s32);
void func_8001CAA4(AnimState*, s16*, s16*);

extern u32* D_800B17BC;
extern AnimSlot* gLoadedAnims;
extern s32* gBuffer_ANIM_TAB;
extern s32 gNumLoadedAnims;
extern s16* gAuxBuffer;
extern s32* gFile_MODELS_TAB;
extern s32* gFreeModelSlots;
extern ModelSlot* gLoadedModels;
extern s32 gNumFreeModelSlots;
extern s32 gNumLoadedModels;
extern s32 gNumModelsTabEntries;

extern s16 D_800903DC;
extern s16 D_800903DE;
extern s16 D_800903E0;

extern u8 D_8008EAC8[8]; // = {0};
extern s32 D_8008EAB0[3]; // = {0};
extern s32 D_8008EABC[3]; // = {0};

void init_models() {
    u32* temp_v0;    

    gLoadedModels = mmAlloc(0x230, ALLOC_TAG_MODELS_COL, NULL);
    gFreeModelSlots = mmAlloc(0x190, ALLOC_TAG_MODELS_COL, NULL);
    gNumLoadedModels = 0;
    gNumFreeModelSlots = 0;

    queue_alloc_load_file((void*)&gFile_MODELS_TAB, MODELS_TAB);
    
    gNumModelsTabEntries = 0;
    while (gFile_MODELS_TAB[gNumModelsTabEntries] != -1){
        gNumModelsTabEntries++;
    }
    gNumModelsTabEntries--;
    
    temp_v0 = mmAlloc(0x830, ALLOC_TAG_ANIMS_COL, NULL);
    gAuxBuffer = temp_v0;
    D_800B17BC = temp_v0 + 0x200;
    gBuffer_ANIM_TAB = temp_v0 + 0x204;
    gLoadedAnims = mmAlloc(0x400, ALLOC_TAG_ANIMS_COL, NULL);
    gNumLoadedAnims = 0;
}

ModelInstance* func_80017D2C(s32 arg0, s32 arg1) {
    ModelInstance* model;

    queue_load_model((void*)&model, arg0, arg1);
    return model;
}


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
s32 model_load_anim_remap_table(s32 id, s32 param_2, s32 param_3);
ModelInstance *createModelInstance(Model *model, u32 flags, s32 initial);
s32 modanim_load(Model* model, s32 id, u8* data);
void func_800186CC(Model *model);
void model_setup_anim_playback(ModelInstance *modelInst, void *param_2);
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
    s16 unk2_aligned;
    s16 unk4;
    s32 i;
    s8 isOldSlot;
    s8 isNewSlot;
    Model *model;
    ModelInstance *modelInst;
    void *modanim;
    s16 unk68;

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

                model_setup_anim_playback(modelInst, modelInst->unk28);
                if (modelInst->unk2C != 0) {
                    model_setup_anim_playback(modelInst, modelInst->unk2C);
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
    unk4 = header->unk4;
    unk2_aligned = mmAlign8(header->unk2);
    unk68 = unk2_aligned + 0x90;
    uncompressedSize = rarezip_uncompress_size(&header->uncompressedSize);
    modelSize = model_load_anim_remap_table(id, unk4, animCount);
    modelSize += uncompressedSize + 500;

    model = mmAlloc(modelSize, ALLOC_TAG_MODELS_COL, 0);
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
    // We must pray that rarezip_uncompress does not overrun its input stream.
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
    rarezip_uncompress(compressedData + 8, model);

    // Convert offsets to pointers
    model->textures = (ModelTexture*)((u32)model->textures + (u32)model);
    model->unk4 = (void*)((u32)model->unk4 + (u32)model);
    model->unk8 = (void*)((u32)model->unk8 + (u32)model);
    model->unk28 = (void*)((u32)model->unk28 + (u32)model);
    model->unk18 = (void*)((u32)model->unk18 + (u32)model);
    model->unk14 = (void*)((u32)model->unk14 + (u32)model);
    model->unkC = (void*)((u32)model->unkC + (u32)model);
    if (model->unk3C != NULL) {
        model->unk3C = (void*)((u32)model + (u32)model->unk3C);
    }
    if (model->unk38 != NULL) {
        model->unk38 = (void*)((u32)model + (u32)model->unk38);
    }
    if (model->unk1C != NULL) {
        model->unk1C = (void*)((u32)model + (u32)model->unk1C);
    }

    model->anims = NULL;

    model->unk24 = 0;

    if (model->unk20 != NULL) {
        model->unk20 = (void*)((u32)model + (u32)model->unk20);
        if (model->unk50 != NULL) {
            model->unk50 = (void*)((u32)model + (u32)model->unk50);
        }
        if (model->unk54 != NULL) {
            model->unk54 = (void*)((u32)model + (u32)model->unk54);
        }
    } else {
        model->unk50 = NULL;
        model->unk54 = NULL;
    }

    if (model->unk2C != NULL) {
        model->unk2C = (void*)((u32)model + (u32)model->unk2C);
    }
    if (model->unk34 != NULL) {
        model->unk34 = (void*)((u32)model + (u32)model->unk34);
    }

    model->unk68 = unk68;
    model->modelId = id;
    model->refCount = 1;
    model->animCount = animCount;

    model->unk71 &= ~0x40;
    if (unk4 != 0) {
        model->unk71 |= 0x40;
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

    for (i = 0; i < model->unk70; i++)
    {
        if (model->unk8[i].unk0 != 0xff && model->unk8[i].unk0 >= model->textureCount) {
            goto bail;
        }
    }

    func_800186CC(model);

    modanim = (void*)mmAlign8((u32)model + uncompressedSize);

    if (modanim_load(model, id, modanim) != 0) {
        goto bail;
    }

    modelInst = createModelInstance(model, flags, 1);
    if (!modelInst) {
        goto bail;
    }

    model_setup_anim_playback(modelInst, modelInst->unk28);
    if (modelInst->unk2C != 0) {
        model_setup_anim_playback(modelInst, modelInst->unk2C);
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

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/model/createModelInstance.s")
#else
// https://decomp.me/scratch/znF3e
ModelInstance* createModelInstance(Model* model, s32 flags, s32 arg2) {
    ModelInstance* temp_v0;
    s32 i;
    AnimState *temp_a1;
    s32 sp58;
    AnimState *temp_v1_4;
    ModelInstanceBlendshape* temp_v0_8;
    u8 *s0;
    ModelStats sp30;

    if (model == NULL) {
        STUBBED_PRINTF(str_800992a8);
        return NULL;
    }
    sp58 = model_get_stats(model, flags, &sp30, model->envMapCount != 0 || model->textureAnimationCount != 0);
    temp_v0 = mmAlloc(sp58, 0x89, NULL);
    // @fake
    if (&sp58) {}
    if (temp_v0 == NULL) {
        return NULL;
    }
    bzero(temp_v0, sp58);
    temp_v0->matrices[0] = (MtxF *) mmAlign16((u32) (temp_v0 + 1));
    s0 = (u8*)temp_v0->matrices[0] + (sp30.jointsMalloc >> 1);
    temp_v0->matrices[1] =  (MtxF *) s0;
    s0 += (sp30.jointsMalloc >> 1);
    if ((flags & 1) || (model->blendshapes != NULL) || (model->textureAnimationCount != 0) || (model->envMapCount != 0)) {
        temp_v0->vertices[0] = (Vtx *) mmAlign16((u32)s0);
        s0 = (u8*) &temp_v0->vertices[0][model->vertexCount];
        temp_v0->vertices[1] = (Vtx *) s0;
        s0 += model->vertexCount * sizeof(Vtx);
        bcopy(model->vertices, temp_v0->vertices[0], model->vertexCount * sizeof(Vtx));
        bcopy(model->vertices, temp_v0->vertices[1], model->vertexCount * sizeof(Vtx));
    } else {
        temp_v0->vertices[0] = model->vertices;
        temp_v0->vertices[1] = model->vertices;
    }
    temp_v0->animState0 = mmAlign4((u32)s0);
    s0 = (u8*)(temp_v0->animState0 + 1);
    if (flags & 0x80) {
        temp_v0->animState1 = (AnimState*)s0;
        s0 += sizeof(AnimState);
    }
    if (model->unk71 & 0x40) {
        s0 = (u8*)mmAlign8((u32)s0);
        temp_v1_4 = temp_v0->animState0;
        temp_v1_4->anims[0] = (AmapPlusAnimation *)s0;
        s0 += sp30.unk14;
        temp_v1_4->anims[1] = (AmapPlusAnimation *)s0;
        s0 += sp30.unk14;
        temp_v1_4->anims2[0] = (AmapPlusAnimation *)s0;
        s0 += sp30.unk14;
        temp_v1_4->anims2[1] = (AmapPlusAnimation *)s0;
        temp_a1 = temp_v0->animState1;
        s0 += sp30.unk14;
        if (temp_a1 != 0) {
            temp_a1->anims[0] = (AmapPlusAnimation *)s0;
            s0 += sp30.unk14;
            temp_a1->anims[1] = (AmapPlusAnimation *)s0;
            s0 += sp30.unk14;
            temp_a1->anims2[0] = (AmapPlusAnimation *)s0;
            s0 += sp30.unk14;
            temp_a1->anims2[1] = (AmapPlusAnimation *)s0;
            s0 += sp30.unk14;
        }
    }
    if (model->blendshapes != NULL) {
        temp_v0->blendshapes = mmAlign4((u32)s0);
        s0 = (u8*) (temp_v0->blendshapes + 3);
        for (i = 0; i < 3; i++) {
            temp_v0_8 = &temp_v0->blendshapes[i];
            temp_v0_8->unkC = -1;
            temp_v0_8->id = -1;
            temp_v0_8->strength = 0.0f;
            temp_v0_8->unk4 = 0.0f;
            temp_v0_8->unk8 = 0.0f;
        }
    }
    if (sp30.hitSphereMalloc > 0) {
        temp_v0->unk1C[0] = mmAlign4((u32)s0);
        s0 = (u8*)(temp_v0->unk1C[0] + (model->hitSphereCount * 0x10));
        temp_v0->unk1C[1] = s0;
        s0 += (model->hitSphereCount * 0x10);
        temp_v0->unk24 = temp_v0->unk1C[0];
    }
    if ((model->joints != NULL) && (model->jointCount != 0) && (model->collisionA != NULL) && (model->collisionB != NULL)) {
        s0 = (u8*) mmAlign4((u32)s0);
        temp_v0->unk14 = (ModelInstance_0x14*)s0;
        s0 += 0x1C;
        temp_v0->unk14->unk0 = (Vec3f*)s0;
        s0 += (model->jointCount * sizeof(Vec3f));
        temp_v0->unk14->unk4 = (f32*)s0;
        s0 += model->jointCount * sizeof(f32 *);
        temp_v0->unk14->unk8 = (f32*)s0;
        s0 += model->jointCount * sizeof(f32 *);
        temp_v0->unk14->unkC = (f32*)s0;
        s0 += model->jointCount * sizeof(f32 *);
        temp_v0->unk14->unk10 = (f32*)s0;
        s0 += model->jointCount * sizeof(f32 *);
        temp_v0->unk14->unk18 = (s8*)s0;
    } else {
        temp_v0->unk14 = NULL;
    }
    if (arg2 == 0) {
        temp_v0->displayList = mmAlloc(model->displayListLength * sizeof(Gfx), ALLOC_TAG_MODELS_COL, 0);
        bcopy(model->displayList, temp_v0->displayList, model->displayListLength * sizeof(Gfx));
    } else {
        temp_v0->displayList = model->displayList;
    }
    load_model_display_list(model, temp_v0);
    temp_v0->model = model;
    return temp_v0;
}
#endif

void patch_model_display_list_for_textures(Model* model) {
    Gfx *gfx;
    s32 i;
    u32 idx;
    Gfx *texGdl;
    Texture *tex;
    u8 isRelative;

    gfx = model->displayList;
    for (i = 0; i < model->displayListLength; i++, gfx++){
        u8 cmd = gfx->words.w0 >> 24;

        switch (cmd) {
            case G_DL:
                isRelative = 0;

                idx = gfx->words.w1;
                if (idx & 0x80000000) {
                    idx &= 0x7fffffff;
                    isRelative = 1;
                }

                tex = model->materials[idx].texture;
                texGdl = tex->gdl;
                if (isRelative) {
                    texGdl += tex->gdlIdx;
                }

                gSPDisplayList(gfx, OS_K0_TO_PHYSICAL(texGdl));
                break;
            case G_NOOP:
                isRelative = 0;
                idx = gfx->words.w1;

                if (idx & 0x80000000) {
                    idx &= 0x7fffffff;
                    isRelative = 1;
                }

                tex = model->materials[idx].texture;
                texGdl = tex->gdl;
                if (isRelative) {
                    texGdl += tex->gdlIdx;
                }

                gfx->words.w0 = texGdl->words.w0;
                gfx->words.w1 = (u32)OS_K0_TO_PHYSICAL(tex + 1);
                break;
        }
    }
}

/** 
  * Calculates various model malloc categories and returns the total size allocated to the model 
  */
u32 model_get_stats(Model* model, s32 settingsBitfield, ModelStats* stats, s32 blendshapeSetting) {
    s32 joints_calc;
    s32 total;
    
    if (model->animCount) {
        stats->jointsMalloc = model->jointCount * 0x80;
    } else {
        stats->jointsMalloc = 0x80;
    }
    
    stats->vertexMalloc = (model->vertexCount << 2) << 3;
    stats->hitSphereMalloc = (model->hitSphereCount << 2) << 3;
    stats->unkC = 0;

    if (model->unk71 & 0x40){
        stats->unk14 = model->unk68;
        PAD16(stats->unk14);
        stats->unkC = stats->unk14 * 4;
    }

    stats->blendShapeMalloc = 0x68;
    if (settingsBitfield & 0x80) {
        stats->blendShapeMalloc *= 2;
        stats->unkC *= 2;
    }

    if (settingsBitfield & 1 || model->blendshapes || blendshapeSetting) {
        stats->blendShapeMalloc += 0x30;
        total = stats->unkC + stats->vertexMalloc + stats->blendShapeMalloc + 0x38;
        total += (stats->jointsMalloc + stats->hitSphereMalloc) + 8;
    } else {
        total = stats->blendShapeMalloc + 0x38;
        total += stats->unkC + stats->jointsMalloc + stats->hitSphereMalloc + 8;
    }    
    
    if (model->joints != 0 && model->jointCount && model->collisionA) {
        joints_calc = model->jointCount;
        joints_calc *= 7;
        joints_calc <<= 2;
        total += joints_calc + (model->jointCount << 1) + 0x1c;
    }
    total += 0x2F;

    // align 16
    total = total & ~0xF;
    total += 0x10;

    return total;
}

//Draw mode when opaque?
void load_model_display_list(Model *model, ModelInstance *modelInst)
{
    ModelDLInfo *dlInfo = model->drawModes;
    ModelDLInfo *dlInfoEnd = dlInfo + model->drawModesCount;

    for (; dlInfo < dlInfoEnd; dlInfo++)
    {
        modelInst->displayList[dlInfo->idx] = dlInfo->gfx;
    }
}

//Draw mode when at semi-transparent opacity?
void load_model_display_list2(Model *model, ModelInstance *modelInst)
{
    ModelDLInfo *dlInfo = model->drawModes;
    ModelDLInfo *dlInfoEnd = dlInfo + model->drawModesCount;

    for (; dlInfo < dlInfoEnd; dlInfo++)
    {
        modelInst->displayList[dlInfo->idx] = dlInfo->gfx2;
    }
}

void destroy_model_instance(ModelInstance* modelInst) {
    Model* sp1C;
    s32 i;

    if (modelInst == NULL) {
        return;
    }

    sp1C = modelInst->model;
    if (modelInst->displayList != sp1C->displayList) {
        mmFree(modelInst->displayList);
    }

    mmFree(modelInst);
    sp1C->refCount--;
    if (sp1C->refCount > 0) {
        return;
    }

    for (i = 0; i < gNumLoadedModels; i++) {
        if (sp1C == (Model*)MODEL_SLOT_MODEL(gLoadedModels, i)) {
            break;
        }
    }

    if (i == gNumLoadedModels) {
        *(u8*)0x0 = 0; // CRASH!
    } else {
        gFreeModelSlots[gNumFreeModelSlots++] = i;

        MODEL_SLOT_ID(gLoadedModels, i) = -1;
        MODEL_SLOT_MODEL(gLoadedModels, i) = -1;
        model_destroy(sp1C);
    }
}


void model_destroy(Model* model) {
    s32 i;

    for (i = 0; i < model->textureCount; i++)
    {
        if ((&model->materials[i])->texture) {
            texture_destroy((&model->materials[i])->texture);
        }
    }

    if (model->anims != 0) {
        i = 0;
        if (model->animCount != 0) {
            do {
                anim_destroy(model->anims[i]);
                i++;
            } while (i < model->animCount);
        }
    }

    mmFree(model);
}

s32 model_load_anim_remap_table(s32 modelID, s32 arg1, s32 animCount){
    s32 *offset;
    s32 amap_size;
    s32 total_size;
    s32 index;
    
    total_size = animCount;
    if (arg1 != 0){
        total_size = (total_size << 1) + 8;
        PAD16(total_size);
    } else {
        total_size = total_size << 2;
        PAD16(total_size);
       
        index = modelID & 3;
        read_file_region(AMAP_TAB, D_800B17BC, (modelID & ~3) << 2, 0x20);

        amap_size = D_800B17BC[index+1] - D_800B17BC[index];
        
        total_size += amap_size;
    }
    return total_size;
}

s32 modanim_load(Model* model, s32 id, u8* data) {
    s32 temp_t0;
    s32 temp_t7;
    s32 temp_v0;
    s32 i;
    s32 var_a0;
    s32 var_s1;
    s32 var_v1;
    s32 sp40;
    s16* temp_s0;
    s32 pad;
    s32 var_s0;

    temp_s0 = (s16*)D_800B17BC;
    var_s1 = 0;
    read_file_region(MODANIM_TAB, temp_s0, id << 1, 0x10);
    temp_t0 = temp_s0[0];
    var_a0 = temp_s0[1];
    temp_t7 = (var_a0 - temp_t0) >> 1;
    if (temp_t7 != model->animCount) {
        model->animCount = temp_t7;
    }
    var_s0 = (model->animCount << 1);
    if (model->animCount == 0) {
        return 0;
    }
    var_s0 += 8;
    read_file_region(AMAP_TAB, D_800B17BC, (id & ~3) << 2, 0x20);
    model->unk5C = D_800B17BC[id & 3];
    sp40 = D_800B17BC[(id & 3) + 1] - D_800B17BC[(id & 3) + 0];
    if (model->unk71 & 0x40) {
        model->modAnim = (s16 *)data;
        PAD16(var_s0);
        // @fake
        if (var_s0) {}
        var_s1 = var_s0;
        data += var_s0;
        read_file_region(MODANIM_BIN, model->modAnim, temp_t0, var_s0);
    } else {
        read_file_region(MODANIM_BIN, gAuxBuffer, temp_t0, var_s0);
        model->modAnim = gAuxBuffer;
    }
    model->modAnimBankBases[0] = 0;
    var_v1 = 1;
    for (i = 0; i < model->animCount; i++) {
        if (model->modAnim[i] == -1) {
            model->modAnimBankBases[var_v1] = i + 1;
            var_v1 += 1;
        }
    }

    var_s1 += model->animCount * 4;
    if (!(model->unk71 & 0x40)) {
        model->anims = (Animation **)data;
        model->modAnim = NULL;
        data += model->animCount * 4;
        while (var_s1 & 7) {
            var_s1 += 1;
            data += 1;
        }
        model->amap = (u8*)data;
        read_file_region(AMAP_BIN, data, model->unk5C, sp40);
        var_s1 = 0;
        do {
            if (gAuxBuffer[var_s1] != -1) {
                model->anims[var_s1] = anim_load(gAuxBuffer[var_s1], var_s1, NULL, model);
                if (model->anims[var_s1] == 0) {
                    for (i = 0; i < var_s1; i++) {
                        anim_destroy(model->anims[i]);
                    }
                    model->anims = NULL;
                    return 1;
                }
            } else {
                model->anims[var_s1] = 0;
            }
            var_s1 += 1;
        } while (var_s1 < model->animCount);
    }
    return 0;
}

void model_setup_anim_playback(ModelInstance* arg0, AnimState* animState) {
    Model* model;
    AnimationHeader* animHeader;
    Animation* anim;

    animState->animIndexes[0] = 0;
    animState->unk5C[1] = 0;
    animState->unk58[0] = 0;
    animState->unk58[1] = 0;
    animState->unk5C[0] = 0;
    animState->unkC[0] = 0.0f;
    animState->curAnimationFrame[0] = 0.0f;
    animState->totalAnimationFrames[0] = 0.0f;
    animState->unk60[0] = 0;
    model = arg0->model;
    if (model->animCount != 0) {
        if (model->unk71 & 0x40) {
            anim_load(*model->modAnim, 0, animState->anims[0], model);
            anim_load(*model->modAnim, 0, animState->anims[1], model);
            anim_load(*model->modAnim, 0, animState->anims2[0], model);
            anim_load(*model->modAnim, 0, animState->anims2[1], model);
            animState->animIndexes[0] = 0U;
            anim = &animState->anims[animState->animIndexes[0]]->anim;
        } else {
            anim = model->anims[animState->animIndexes[0]];
        }
        
        animHeader = &anim->animHeader;
        if (animHeader->totalBones != model->jointCount) {
            STUBBED_PRINTF("makeModelAnimation() size mismatch!! (%d,%d)\n");
        }
        animState->unk34[0] = animHeader;
        animState->unk60[0] = anim->unk1 & 0xF0;

        animState->totalAnimationFrames[0] = animHeader->totalKeyframes;
        //If the anim doesn't loop, maybe?
        //(i.e. no need to blend from last key to first key, so finished as soon as it reaches last key)
        if (animState->unk60[0] == 0) {
            animState->totalAnimationFrames[0] -= 1.0f;
        }
        
        animState->unk60[1] = animState->unk60[0];
        animState->unk34[1] = animState->unk34[0];
        animState->unk3C[0] = animState->unk34[0];
        animState->unk3C[1] = animState->unk34[0];
        animState->animIndexes[1] = animState->animIndexes[0];
        animState->unk48[0] = animState->animIndexes[0];
        animState->unk48[1] = animState->animIndexes[0];
        animState->curAnimationFrame[1] = animState->curAnimationFrame[0];
        animState->totalAnimationFrames[1] = animState->totalAnimationFrames[0];
        animState->unkC[1] = animState->unkC[0];
    }
}

static const char str_800993ac[] = "Warning: Model animation buffer overflow!! size=%d\n";
static const char str_800993e0[] = "modLoadAnimActual: anim overflow %d/%d\n";

Animation* func_80019118(s16 animId, s16 modAnimId, s32 amap, s32 model) {
    Animation* anim;

    anim = NULL;
    queue_load_anim((void*)&anim, animId, modAnimId, amap, model);
    return anim;
}

#ifndef NON_MATCHING
Animation* anim_load(s16 animId, s16 modanimId, AmapPlusAnimation* anim, Model* model);
#pragma GLOBAL_ASM("asm/nonmatchings/model/anim_load.s")
#else
Animation* anim_load(s16 animId, s16 modanimId, AmapPlusAnimation* anim, Model* model) {
    Animation* temp_v0;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    Animation* var_s1;
    s32 i;

    if (anim == NULL) {
        sp28 = -1;
        for (i = 0; i < gNumLoadedAnims; i++) {
            if (animId == ANIM_SLOT_RC(gLoadedAnims, i)) {
                temp_v0 = (Animation*) ANIM_SLOT_ANIM(gLoadedAnims, i);
                temp_v0->referenceCount++;
                return temp_v0;
            }
            if (ANIM_SLOT_RC(gLoadedAnims, i) == -1) {
                sp28 = i;
            }
        }
    }

    read_file_region(ANIM_TAB, gBuffer_ANIM_TAB, (animId & ~1) << 2, 0x10);
    sp24 = gBuffer_ANIM_TAB[(animId & 1) + 0];
    sp20 = gBuffer_ANIM_TAB[(animId & 1) + 1] - sp24;
    if (anim == NULL) {
        var_s1 = mmAlloc(sp20, ALLOC_TAG_ANIMS_COL, NULL);
    } else {
        var_s1 = &anim->anim;
    }
    if (var_s1 == NULL) {
        return NULL;
    }
    read_file_region(ANIM_BIN, var_s1, sp24, sp20);
    if (anim != NULL) {
        sp20 = ALIGN8(model->jointCount - 1);
        read_file_region(AMAP_BIN, anim, model->unk5C + (modanimId * sp20), sp20);
        if (gNumLoadedAnims) {}
    }
    if (anim == NULL) {
        var_s1->referenceCount = 1;
        if (sp28 == -1) {
            sp28 = gNumLoadedAnims;
            gNumLoadedAnims++;
            if (gNumLoadedAnims == 0x80) {
                gNumLoadedAnims--;
            }
        }
        ANIM_SLOT_RC(gLoadedAnims, sp28) = animId;
        ANIM_SLOT_ANIM(gLoadedAnims, sp28) = var_s1;
    }
    return var_s1;
}
#endif

void anim_destroy(Animation* anim) {
    AnimSlot* slot;
    s32 index;
    s32 matchIndex;
    int clear = -1;

    if (!anim){
        STUBBED_PRINTF("Anim Error: Tryed to deallocate non-existent anim!!\n");
        return;
    }

    anim->referenceCount--;
    if (anim->referenceCount > 0)
        return;
            
    for (matchIndex = -1, index = 0; index < gNumLoadedAnims; index++){
        if (anim == (Animation*)ANIM_SLOT_ANIM(gLoadedAnims, index)) {
            matchIndex = index;
            //@bug?: continues iterating through animations after animation found
        }
    }
    
    if (matchIndex != -1) {
        ANIM_SLOT_RC(gLoadedAnims, matchIndex) = -1;
        ANIM_SLOT_ANIM(gLoadedAnims, matchIndex) = -1;
        mmFree(anim);
    }        
}

static const char str_80099440[] = "**Top matrix out of range: (%.1f,%.1f,%.1f) (%.1f,%.1f)\n";
static const char str_8009947c[] = "Warning! Tiltlist overflow!!\n";
static const char str_8009949c[] = "%d : %d\n";
static const char str_800994a8[] = "\n";
static const char str_800994ac[] = "blend_frames: %x\n";

void func_8001943C(Object* object, MtxF* mf, f32 yPrescale, f32 arg3) {
    if (object->parent == NULL) {
        object->srt.transl.f[0] -= gWorldX;
        object->srt.transl.f[2] -= gWorldZ;
    }

    if (object->srt.transl.f[0] > 32767.0f || object->srt.transl.f[0] < -32767.0f || object->srt.transl.f[2] > 32767.0f || object->srt.transl.f[2] < -32767.0f) {
        bzero(mf, 0x40);
        mf->m[0][0] = 1.0f;
        mf->m[1][1] = 1.0f;
        mf->m[2][2] = 1.0f;
        mf->m[3][3] = 1.0f;
        if (object->parent == NULL) {
            object->srt.transl.f[0] += gWorldX;
            object->srt.transl.f[2] += gWorldZ;
        }
    } else {
        matrix_from_srt(mf, &object->srt);
        if (object->id == OBJ_ScorpionRobot) {
            matrix_from_yaw(((DLL_Unknown *)object->dll)->vtbl->func[7].withOneArgS32((s32)object), mf);
        }
        if (yPrescale != 1.0f) {
            matrix_prescale_y(mf, yPrescale);
        }
        if (object->parent == NULL) {
            object->srt.transl.f[0] += gWorldX;
            object->srt.transl.f[2] += gWorldZ;
        }
    }
}

void func_800195F8(f32 arg0, UNK_TYPE_32 arg1, Model* arg2, MtxF* arg3, f32 *arg4) {
    s32 i;
    s32 j;
    MtxF* temp_a1;
    MtxF* temp_t1;
    ModelJoint* var_a3;

    var_a3 = arg2->joints;
    for (i = 0, j = 0; i < arg2->jointCount; j += 4, i++) {
        temp_a1 = (MtxF *)(arg4 + (j * 4));
        temp_a1->m[0][0] = arg0;
        temp_a1->m[0][1] = 0.0f;
        temp_a1->m[0][2] = 0.0f;
        temp_a1->m[0][3] = 0.0f;
        temp_a1->m[1][0] = 0.0f;
        temp_a1->m[1][1] = arg0;
        temp_a1->m[1][2] = 0.0f;
        temp_a1->m[1][3] = 0.0f;
        temp_a1->m[2][0] = 0.0f;
        temp_a1->m[2][1] = 0.0f;
        temp_a1->m[2][2] = arg0;
        temp_a1->m[2][3] = 0.0f;
        temp_a1->m[3][0] = 0.0f;
        temp_a1->m[3][1] = 0.0f;
        temp_a1->m[3][2] = 0.0f;
        temp_a1->m[3][3] = 1.0f;
        if (var_a3[i].parentJointID == -1) {
            temp_a1->m[3][0] = arg3->m[3][0] + (var_a3[i].x * arg0);
            temp_a1->m[3][1] = arg3->m[3][1] + (var_a3[i].y * arg0);
            temp_a1->m[3][2] = arg3->m[3][2] + (var_a3[i].z * arg0);
        } else {
            temp_t1 = (MtxF *) &arg4[var_a3[i].parentJointID << 4];
            temp_a1->m[3][0] = temp_t1->m[3][0] + (var_a3[i].x * arg0);
            temp_a1->m[3][1] = temp_t1->m[3][1] + (var_a3[i].y * arg0);
            temp_a1->m[3][2] = temp_t1->m[3][2] + (var_a3[i].z * arg0);
        }
    }
}
void func_80019730(ModelInstance* arg0, Model* arg1, Object* arg2, MtxF* arg3) {
    AnimState* temp_a2;
    s32 sp60;
    s32 pad;
    AnimState* sp58;
    Vec3f sp4C;
    s16 sp44[3];

    func_8001A640(arg2, arg0, arg1);
    arg0->unk34 ^= 1;
    sp60 = arg0->unk34 & 1;
    temp_a2 = arg0->animState0;
    if (temp_a2->unk62[1] & 4) {
        func_8001A3FC(arg0, 0, 0, arg2->animProgress, arg2->srt.scale, &sp4C, sp44);
        D_800903DC = sp44[0];
        D_800903DE = sp44[1];
        D_800903E0 = sp44[2];
    }
    if (arg0->model->unk71 & 8) {
        func_800199A8(arg3, arg0, arg0->animState0, arg2->animProgress, 0x7F);
    } else if (arg0->animState0->unk62[1] & 8) {
        sp58 = arg0->animState1;
        func_80019FC0(arg3, arg0, temp_a2, arg2->animProgress, 0x7F, 0, 0, 2, 0x14, temp_a2->unk58[1]);
        func_80019FC0(arg3, arg0, sp58, arg2->animProgressLayered, 0x7F, 0, 0, 2, 0x18, sp58->unk58[1]);
        func_80019FC0(arg3, arg0, temp_a2, arg2->animProgress, 0x7F, 0, 0, 0, 7, sp58->unk58[0]);
        func_80019FC0(arg3, arg0, temp_a2, arg2->animProgress, 0x7F, 0, 1, 1, 1, temp_a2->unk58[0]);
    } else {
        func_800199A8(arg3, arg0, arg0->animState0, arg2->animProgress, 0x7F);
        if ((arg0->animState1 != NULL) && (arg2->curModAnimIdLayered >= 0)) {
            func_800199A8(arg3, arg0, arg0->animState1, arg2->animProgressLayered, -1U);
        }
    }
    add_matrix_to_pool(arg0->matrices[sp60], arg1->jointCount);
}

void func_800199A8(MtxF* arg0, ModelInstance* modelInst, AnimState* animState, f32 arg3, u32 arg4) {
    MtxF* spEC;
    Model* model;
    s32 var_s0;
    s32 var_s1;
    s32 var_v1;
    s32 var_s4;
    s32 temp_v0;
    AnimState sp6C;

    model = modelInst->model;
    var_s4 = 0;
    spEC = modelInst->matrices[modelInst->unk34 & 1];
    animState->curAnimationFrame[0] = animState->totalAnimationFrames[0] * arg3;
    if (model->unk71 & 8) {
        sp6C.anims[0] = animState->anims[0];
        sp6C.anims[1] = animState->anims[1];
        sp6C.anims2[0] = animState->anims2[0];
        sp6C.anims2[1] = animState->anims2[1];
        var_s0 = 0;
        for (; var_s0 < 2; var_s0++) {
            if (animState->unk58[0] != 0) {
                var_s1 = var_s0;
            } else {
                var_s1 = 0;
            }
            sp6C.animIndexes[var_s0] = animState->animIndexes[var_s1];
            sp6C.unk60[var_s0] = animState->unk60[var_s1];
            sp6C.totalAnimationFrames[var_s0] = animState->totalAnimationFrames[var_s1];
            sp6C.curAnimationFrame[var_s0] = animState->curAnimationFrame[var_s1];
            sp6C.unk34[var_s0] = animState->unk34[var_s1];
        }
        sp6C.unk58[0] = animState->unk58[0];
        func_8001A1D4(model, &sp6C, 2);
        temp_v0 = animState->unk62[1];
        if (temp_v0 & 1) {
            var_s4 = 0x10;
        }
        if (temp_v0 & 4) {
            var_s4 |= 0x20;
        }
        func_8001B4F0(&spEC, arg0, &sp6C, model->joints, (s32) model->jointCount, (s16* ) &SHORT_ARRAY_800b17d0, (s32) arg4, var_s4 | 0x40);
        return;
    }
    for (var_s0 = 0; var_s0 < 2; var_s0++) {
        if (var_s0 != 0) {
            var_v1 = animState->unk5C[0];
        } else {
            var_v1 = animState->unk58[1];
        }
        if (var_v1 != 0) {
            if (animState->unk58[0] != 0) {
                var_s1 = 4 << var_s0;
            } else {
                var_s1 = 0;
            }
            sp6C.unk60[0] = animState->unk60[var_s0];
            sp6C.totalAnimationFrames[0] = animState->totalAnimationFrames[var_s0];
            sp6C.curAnimationFrame[0] = animState->curAnimationFrame[var_s0];
            sp6C.unk34[0] = animState->unk34[var_s0];
            sp6C.unk60[1] = animState->unk60[var_s0];
            sp6C.totalAnimationFrames[1] = animState->totalAnimationFrames[var_s0];
            sp6C.curAnimationFrame[1] = animState->curAnimationFrame[var_s0];
            sp6C.unk34[1] = animState->unk3C[var_s0];
            if (model->unk71 & 0x40) {
                sp6C.animIndexes[0] = 0;
                sp6C.animIndexes[1] = 1;
                sp6C.anims[0] = animState->anims[animState->animIndexes[var_s0]];
                sp6C.anims[1] = animState->anims2[animState->unk48[var_s0]];
            } else {
                sp6C.animIndexes[0] = animState->animIndexes[var_s0];
                sp6C.animIndexes[1] = animState->unk48[var_s0];
            }
            sp6C.unk58[0] = var_v1;
            func_8001A1D4(model, &sp6C, 2);
            func_8001B4F0(&spEC, arg0, &sp6C, model->joints, (s32) model->jointCount, (s16* ) &SHORT_ARRAY_800b17d0, (s32) arg4, var_s1);
            if (var_s1 != 0) {
                var_s4 |= 1 << var_s0;
            }
        }
    }
    if (((animState->unk58[1] == 0) && (animState->unk5C[0] == 0)) || (var_s4 != 0)) {
        var_s1 = 1;
        if (animState->unk58[0] != 0) {
            var_s1 = 2;
        }
        sp6C.anims[0] = animState->anims[0];
        sp6C.anims[1] = animState->anims[1];
        sp6C.anims2[0] = animState->anims2[0];
        sp6C.anims2[1] = animState->anims2[1];
        for (var_s0 = 0; var_s0 < var_s1; var_s0++) {
            sp6C.animIndexes[var_s0] = animState->animIndexes[var_s0];
            sp6C.unk60[var_s0] = animState->unk60[var_s0];
            sp6C.totalAnimationFrames[var_s0] = animState->totalAnimationFrames[var_s0];
            sp6C.curAnimationFrame[var_s0] = animState->curAnimationFrame[var_s0];
            sp6C.unk34[var_s0] = animState->unk34[var_s0];
        }
        sp6C.unk58[0] = animState->unk58[0];
        func_8001A1D4(model, &sp6C, var_s1);
        temp_v0 = animState->unk62[1];
        if (temp_v0 & 1) {
            var_s4 |= 0x10;
        }
        if (temp_v0 & 4) {
            var_s4 |= 0x20;
        }
        func_8001B4F0(&spEC, arg0, &sp6C, model->joints, (s32) model->jointCount, (s16* ) &SHORT_ARRAY_800b17d0, (s32) arg4, var_s4);
    }
}

void func_80019FC0(MtxF* arg0, ModelInstance* modelInst, AnimState* animState, f32 arg3, u32 arg4, u8 animIdx0, u8 arg6, u8 animIdx1, u8 flags, s16 arg9) {
    MtxF* spA4;
    Model* temp_s1;
    AnimState sp38;

    temp_s1 = modelInst->model;
    spA4 = modelInst->matrices[modelInst->unk34 & 1];
    if (flags & 0x10) {
        animState->curAnimationFrame[0] = animState->totalAnimationFrames[0] * arg3;
    }
    sp38.unk60[0] = animState->unk60[animIdx0];
    sp38.totalAnimationFrames[0] = animState->totalAnimationFrames[animIdx0];
    sp38.curAnimationFrame[0] = animState->curAnimationFrame[animIdx0];
    sp38.unk34[0] = animState->unk34[animIdx0];
    sp38.unk60[1] = animState->unk60[arg6];
    sp38.totalAnimationFrames[1] = animState->totalAnimationFrames[arg6];
    sp38.curAnimationFrame[1] = animState->curAnimationFrame[arg6];
    sp38.unk34[1] = animState->unk34[animIdx1];
    if (temp_s1->unk71 & 0x40) {
        sp38.animIndexes[0] = 0;
        sp38.animIndexes[1] = 1;
        sp38.anims[0] = animState->anims[animState->animIndexes[animIdx0]];
        if (animIdx1 < 2) {
            sp38.anims[1] = animState->anims[animState->animIndexes[animIdx1]];
        } else {
            sp38.anims[1] = animState->anims2[animState->animIndexes[animIdx1]];
        }
    } else {
        sp38.animIndexes[0] = animState->animIndexes[animIdx0];
        sp38.animIndexes[1] = animState->animIndexes[animIdx1];
    }
    if (arg9 == 0) {
        arg9 = 1;
    }
    sp38.unk58[0] = arg9;
    func_8001A1D4(temp_s1, &sp38, 2);
    flags &= 0xF;
    if (!(flags & 0xC)) {
        if (animState->unk62[1] & 1) {
            flags |= 0x10;
        }
        if (animState->unk62[1] & 4) {
            flags |= 0x20;
        }
    }
    func_8001B4F0(&spA4, arg0, &sp38, temp_s1->joints, (s32) temp_s1->jointCount, (s16* ) &SHORT_ARRAY_800b17d0, (s32) arg4, flags);
}

void func_8001A1D4(Model* model, AnimState* animState, s32 count) {
    AmapPlusAnimation* var_a1;
    Animation* var_t0;
    s32 var_t1;
    s32 temp_a1;
    s32 j;
    s32 i;
    s32 k;

    for (i = 0; i < count; i++) {
        k = i;
        if (model->unk71 & 0x40) {
            var_a1 = animState->anims[animState->animIndexes[k]];
            var_t0 = &var_a1->anim;
        } else {
            var_a1 = (AmapPlusAnimation* ) &model->amap[animState->animIndexes[k] * ALIGN8(model->jointCount - 1)];
            var_t0 = model->anims[animState->animIndexes[k]];
        }
        for (j = 0; j < model->jointCount; j++) {
            ((u8*)&model->joints[j])[k + 2] = var_a1->boneRemaps[j];
        }
        temp_a1 = animState->unk34[k]->keyframeStride & 0xFF;
        var_t1 = (s32) animState->curAnimationFrame[i] - 1;
        if ((var_t1 < 0) && (animState->unk60[i] != 0)) {
            var_t1 += (s32) animState->totalAnimationFrames[k];
        }
        animState->unk4C[i][0] = temp_a1;
        animState->unk4C[i][1] = temp_a1 * 2;
        animState->unk4C[i][2] = temp_a1 * 3;
        if (var_t1 < 0) {
            var_t1 = 0;
        }
        animState->unk2C[k] = (u8*)var_t0 + var_t0->unk2 + (temp_a1 * var_t1);
        temp_a1 = animState->unk34[k]->keyframeStride & 0xFF;
        var_t1 = animState->curAnimationFrame[i];
        if (var_t1 != animState->curAnimationFrame[i]) {
            animState->unk4C[0][k] = temp_a1;
        } else {
            animState->unk4C[0][k] = 0;
        }
        if ((animState->unk60[i] != 0) && (var_t1 == (animState->totalAnimationFrames[k] - 1.0f))) {
            animState->unk4C[0][k] = -temp_a1 * var_t1;
        }
        animState->unk2C[k] = (u8*)var_t0 + var_t0->unk2 + (temp_a1 * var_t1);
    }
}

void func_8001A3FC(ModelInstance* modelInst, u32 selector, s32 idx, f32 arg3, f32 scale, Vec3f* arg5, s16* arg6) {
    AnimState* sp44;
    Animation* var_a3;
    s32 temp;
    s32 pad;
    s32 temp_t1;
    s16 sp2C[3];
    AnimationHeader* sp28;

    if (selector != 0) {
        sp44 = modelInst->animState1;
    } else {
        sp44 = modelInst->animState0;
    }
    sp28 = sp44->unk34[0];
    sp44->unk34[0] = sp44->unk34[idx];
    if (modelInst->model->unk71 & 0x40) {
        if (idx >= 2) {
            var_a3 = &sp44->anims2[sp44->animIndexes[idx]]->anim;
        } else {
            var_a3 = &sp44->anims[sp44->animIndexes[idx]]->anim;
        }
    } else {
        var_a3 = modelInst->model->anims[sp44->animIndexes[idx]];
    }
    sp44->curAnimationFrame[0] = sp44->totalAnimationFrames[0] * arg3;
    temp_t1 = sp44->unk34[0]->keyframeStride & 0xFF;
    temp = sp44->curAnimationFrame[0];
    if (temp != sp44->curAnimationFrame[0]) {
        sp44->unk4C[0][0] = temp_t1;
    } else {
        sp44->unk4C[0][0] = 0;
    }
    if ((sp44->unk60[0] != 0) && (temp == (sp44->totalAnimationFrames[0] - 1.0f))) {
        sp44->unk4C[0][0] = -temp_t1 * temp;
    }
    sp44->unk2C[0] = (u8*)var_a3 + var_a3->unk2 + (temp_t1 * temp);
    func_8001CAA4(sp44, sp2C, arg6);
    sp44->unk34[0] = sp28;
    arg5->f[0] = sp2C[0] * 0.03125f;
    arg5->f[1] = sp2C[1] * 0.03125f;
    arg5->f[2] = sp2C[2] * 0.03125f;
    arg5->f[0] += modelInst->model->joints->x;
    arg5->f[1] += modelInst->model->joints->y;
    arg5->f[2] += modelInst->model->joints->z;
    arg5->f[0] *= scale;
    arg5->f[1] *= scale;
    arg5->f[2] *= scale;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001A640.s")
#else
void _func_8001A640(Object *object, ModelInstance *modelInst, Model *model)
{
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 n;
    s8* amap;

    if (model->unk71 & 0x40) {
        amap = modelInst->animState0->anims[modelInst->animState0->animIndexes[0]];
    } else {
        amap = model->amap + modelInst->animState0->animIndexes[0] * ALIGN8(model->unk6F);
    }

    j = 0;
    m = 0;
    for (i = 0; i < object->ptr0x50->unk72; i++)
    {
        if (object->ptr0x50->unk10[object->modelInstIdx + j + 1] != 0xff)
        {
            s16 *src = &object->unk6C[i * 9];
            u32 code = amap[object->ptr0x50->unk10[object->modelInstIdx + j + 1]] << 6;

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

        j += object->ptr0x50->unk5D + 1;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001A8EC.s")

void func_8001AC44(ModelInstance*, Model*, Object*, MtxF*, MtxF*, u32, f32);
#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001AC44.s")

void func_8001AE74(ModelInstance *modelInst) {
    if (modelInst->model->blendshapes != NULL) {
        func_8001AF04(modelInst, -1, -1, 0, 0, 7);
        func_8001AF04(modelInst, -1, -1, 0, 1, 7);
        func_8001AF04(modelInst, -1, -1, 0, 2, 7);
    }
}

void func_8001AF04(ModelInstance* modelInstance, s32 arg1, s32 shapeId, f32 arg3, s32 layer, s32 arg5) {
    BlendshapeHeader* blendshapes;
    ModelInstanceBlendshape* blendshape;
    s16 totalBlendshapes;

    if (layer >= BLENDSHAPE_LAYER_LIMIT)
        return;

    blendshapes = modelInstance->model->blendshapes;
    if (blendshapes == NULL)
        return;

    totalBlendshapes = blendshapes->totalBlendshapes;
    if (arg1 >= -1 && shapeId >= -1 && arg1 < totalBlendshapes && shapeId < totalBlendshapes) {
        blendshape = &modelInstance->blendshapes[layer];
        if (arg1 == -1 && shapeId == -1) {
            if (blendshape->unkC == -1 && blendshape->id == -1)
                return;
            arg5 |= 6;
        }
        blendshape->unkC = arg1;
        blendshape->id = shapeId;
        if (!(arg5 & 0x10)) {
            blendshape->strength = 0.0f;
        }
        blendshape->unk4 = -1.0f;
        blendshape->unk8 = arg3;
        blendshape->unkE = arg5 | 4;
    }
}

void func_8001AFCC(ModelInstance *modelInst, s32 param2, f32 param3) {
    ModelInstanceBlendshape *var1;

    if (param2 < 3 && modelInst->model->blendshapes != NULL) {
        var1 = &modelInst->blendshapes[param2];
        var1->strength = param3;
        var1->unkE |= 4;
    }
}

s32 func_8001B010(ModelInstance *modelInst) {
    int i;
    ModelInstanceBlendshape *var1;
    
    if (modelInst->model->blendshapes == NULL) {
        return 0;
    }

    for (i = 0; i < 3; i++) {
        var1 = &modelInst->blendshapes[i];

        if (var1->strength != var1->unk4 || (var1->unkE & 0xE) != 0) {
            return 1;
        }
    }

    return 0;
}

void func_8001B084(ModelInstance *modelInst, f32 updateRate) {
    int i;
    ModelInstanceBlendshape *var1;
    
    if (modelInst->model->blendshapes == NULL) {
        return;
    }

    for (i = 0; i < 3; i++) {
        var1 = &modelInst->blendshapes[i];

        if (var1->unkC != -1 || var1->id != -1) {
            if ((var1->unkE & 1) == 0) {
                var1->strength += var1->unk8 * updateRate;
            }
        }
    }
}

void func_8001B100(ModelInstance* modelInst) {
    BlendshapeHeader* temp_v0;
    Model* temp_s2;
    Vtx* var_a1;
    s32 temp_s4;
    s32 var_t3;
    ModelInstanceBlendshape* sp88;
    u8* var_t0;
    u8* var_t1;
    s32 pad2;
    s32 var_t2;
    s32 i;
    s32 sp68[3] = D_8008EAB0;
    s32 sp5C[3] = D_8008EABC;
    s32 pad;

    temp_s2 = modelInst->model;
    if (temp_s2->blendshapes == NULL) {
        return;
    }
    temp_v0 = temp_s2->blendshapes;
    temp_s4 = temp_v0->totalInfluences;
    for (i = 0; i < 3; i++) {
        sp88 = &modelInst->blendshapes[i];
        if (sp88->strength != sp88->unk4) {
            sp88->unkE &= ~0xC;
            sp88->unkE |= 4;
        }
        sp5C[i] = sp88->unkE & 0xC;
        if ((sp88->unkC != -1) || (sp88->id != -1) || (sp88->unkE & 0xC)) {
            sp68[i] = 1;
        }
        if (sp5C[i] & 4) {
            sp88->unkE &= ~4;
            sp88->unkE |= 8;
        } else if (sp5C[i] & 8) {
            sp88->unkE &= ~8;
        }
    }
    if (sp68[0] == 0 && sp68[1] == 0 && sp68[2] == 0) {
        return;
    }

    if (sp68[1] != 0) {
        sp68[0] = 0;
    }
    if (sp5C[2] != 0) {
        sp5C[0] = 1;
        sp5C[1] = 1;
    }

    for (i = 0; i < 3; i++, sp88->unk4 = sp88->strength) {
        sp88 = &modelInst->blendshapes[i];
        if (sp88->unkE & 2) {
            sp88->unkE &= ~2;
            sp88->strength = 0.0f;
        }

        if (sp68[i] == 0 || sp5C[i] == 0) {
            continue;
        }

        var_t0 = temp_v0->headerLength + ((u8*)temp_s2->blendshapes);
        if (sp88->unkC >= 0) {
            var_t1 = var_t0;
            var_t1 += (temp_s4 * 4 * sp88->unkC);
            var_t2 = 4;
        } else {
            var_t1 = D_8008EAC8;
            var_t2 = 0;
        }
        if (sp88->id >= 0) {
            var_t0 += temp_s4 * 4 * sp88->id;
            var_t3 = 4;
        } else {
            var_t0 = D_8008EAC8;
            var_t3 = 0;
        }
        if (i == 2) {
            if (sp68[0] == 0 && sp68[1] == 0) {
                var_a1 = temp_s2->vertices;
            } else {
                var_a1 = (Vtx*) modelInst->vertices[(modelInst->unk34 >> 1) & 1];
            }
        } else {
            var_a1 = temp_s2->vertices;
        }
        if (sp88->strength > 1.0f) {
            sp88->strength = 1.0f;
        } else if (sp88->strength < 0.0f) {
            sp88->strength = 0.0f;
        }
        func_800202E8(
            modelInst->vertices[(modelInst->unk34 >> 1) & 1],
            var_a1,
            temp_s2->vertexGroups,
            &temp_v0->influences,
            var_t1,
            var_t0,
            var_t2,
            var_t3,
            (sp88->strength * 255.0f),
            i < 2
        );
    }
}

void func_8001B49C(void) {
    s32 index;
    
    for (index = 0; index < gNumLoadedModels; index++){
        ModelSlot *slot;
        if (MODEL_SLOT_MODEL(gLoadedModels, index)) {
        }

        if (MODEL_SLOT_ID(gLoadedModels, index) < 0){
            continue;
        }
        index = index ^ 0;
    }
}

void doNothing_8001B4E4(s32 arg) {}
