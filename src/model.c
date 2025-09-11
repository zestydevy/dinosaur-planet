#include "common.h"
#include "sys/rarezip.h"

#define ALIGN8(a)  (((u32) (a) & ~0x7) + 0x8)
#define PAD16(a) while (a & 7){a++;}

extern s16 *SHORT_ARRAY_800b17d0;

void func_8001AF04(ModelInstance* modelInstance, s32 arg1, s32 shapeId, f32 arg3, s32 layer, s32 arg5);
Animation* anim_load(s16 animId, s16 modanimId, AmapPlusAnimation* anim, Model* model);
void anim_destroy(Animation*);


extern u32* D_800B17BC;
extern AnimSlot* gLoadedAnims;
extern void* gBuffer_ANIM_TAB;
extern s32 gNumLoadedAnims;
extern void* gAuxBuffer;
extern s32* gFile_MODELS_TAB;
extern void* gFreeModelSlots;
extern ModelSlot* gLoadedModels;
extern s32 gNumFreeModelSlots;
extern s32 gNumLoadedModels;
extern s32 gNumModelsTabEntries;

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
u32 modanim_load(Model *model, u32 id, void *modanim);
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
    unk_0x2_aligned = mmAlign8(header->unk_0x2);
    unk_0x68 = unk_0x2_aligned + 0x90;
    uncompressedSize = rarezip_uncompress_size(&header->uncompressedSize);
    modelSize = model_load_anim_remap_table(id, unk_0x4, animCount);
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

    modanim = (void*)mmAlign8((u32)model + uncompressedSize);

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

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/patch_model_display_list_for_textures.s")
#else
// void patch_model_display_list_for_textures(Model* model) {
//     u8 commandID;
//     s32 var_a1;
//     s32 var_a1_2;
//     s32 materialIndex;
//     s32 materialIndex_2;
//     u32 *var_t0;
//     s32 index;
//     u32* var_t0_2;
//     ModelTexture* material;
//     void* temp_a3_2;
//     Gfx* gfx;

//     gfx = model->displayList;
//     index = 0;
//     while (index < model->displayListLength){        
//         commandID = gfx->words.w0 >> 24;
        
//         if (commandID != G_NOOP) {
//             if (commandID == G_DL) { //Texture load commands!
//                 materialIndex = gfx->words.w1;
//                 var_a1 = 0;
//                 if (gfx->words.w1 & 0x80000000) {
//                     materialIndex = gfx->words.w1 & 0x7FFFFFFF;
//                     var_a1 = 1;
//                 }
//                 material = &model->materials[materialIndex];
//                 var_t0 = material->unk8;
//                 if (var_a1 != 0) {
//                     var_t0 += material->unk12 << 1;
//                 }
//                 gfx->words.w1 = (s32)(0x80000000 + (u32)var_t0);
//                 gfx->words.w0 = 0xDE000000;
//             }
//         } else {
//             materialIndex = gfx->words.w1;
//             var_a1 = 0;
//             if (materialIndex & 0x80000000) {
//                 materialIndex &= 0x7FFFFFFF;
//                 var_a1 = 1;
//             }
//             material = &model->materials[materialIndex];
//             var_t0 = material->unk8;
//             if (var_a1 != 0) {
//                 var_t0 += material->unk12 << 1;
//             }
//             gfx->words.w1 = (s32)(0x80000020 + (u32)var_t0);
//             gfx->words.w0 = *var_t0;
//         }

//         gfx++;
//         index++;        
//     }
// }

void patch_model_display_list_for_textures(Model* model) {
    Gfx *gfx;
    s32 i;

    for (gfx = model->displayList, i = 0; i < model->displayListLength; i++, gfx++){
        u8 cmd = gfx->words.w0 >> 24;

        if (cmd != G_NOOP)
        {
            if (cmd == G_DL)
            {
                u32 idx = gfx->words.w1;
                Gfx *texGdl;
                u8 isRelative = 0;

                if (idx & 0x80000000) {
                    idx &= 0x7fffffff;
                    isRelative = 1;
                }

                texGdl = model->materials[idx].texture->gdl;
                if (isRelative) {
                    texGdl += model->materials[idx].texture->gdlIdx;
                }

                gSPDisplayList(gfx, OS_K0_TO_PHYSICAL(texGdl));
            }
        } else {
            u32 idx = gfx->words.w1;
            Texture *tex;
            Gfx *texGdl;
            u8 isRelative = 0;

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
            gfx->words.w1 = (u32)tex + 0x80000020;
        }
    }
}
#endif

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

    if (model->unk_0x71 & 0x40){
        stats->unk14 = model->unk_0x68;
        while (stats->unk14 & 7) {
            stats->unk14++;
        }
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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/destroy_model_instance.s")
#else
extern s32 *gFreeModelSlots;
extern s32 gNumFreeModelSlots;
extern ModelSlot *gLoadedModels;
extern s32 gNumLoadedModels;
void _destroy_model_instance(ModelInstance *modelInst)
{
    Model *model;

    if (modelInst == NULL) {
        return;
    }

    model = modelInst->model;

    if (model->displayList != modelInst->displayList) {
        mmFree(modelInst->displayList);
    }
    mmFree(modelInst);

    if (--model->refCount <= 0)
    {
        s32 slot;

        for (slot = 0; slot < gNumLoadedModels; slot++) {
            if (gLoadedModels[slot].model == model) {
                break;
            }
        }

        if (slot == gNumLoadedModels)
        {
            *(u8*)0x0 = 0; // CRASH!
        }
        else
        {
            gFreeModelSlots[gNumFreeModelSlots++] = slot;
            gLoadedModels[slot].id = gLoadedModels[slot].model = -1;
            model_destroy(model);
        }
    }
}
#endif

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

#pragma GLOBAL_ASM("asm/nonmatchings/model/modanim_load.s")

void model_setup_anim_playback(ModelInstance* arg0, AnimState* animState) {
    Model* model;
    AnimationHeader* animHeader;
    Animation* anim;

    animState->animIndexes[0] = 0;
    animState->unk_0x5c[1] = 0;
    animState->unk_0x58[0] = 0;
    animState->unk_0x58[1] = 0;
    animState->unk_0x5c[0] = 0;
    animState->unk_0xc[0] = 0.0f;
    animState->curAnimationFrame[0] = 0.0f;
    animState->totalAnimationFrames[0] = 0.0f;
    animState->unk_0x60[0] = 0;
    model = arg0->model;
    if (model->animCount != 0) {
        if (model->unk_0x71 & 0x40) {
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
            STUBBED_PRINTF("makeModelAnimation() size mismatch!! (%d,%d)");
        }
        animState->unk_0x34[0] = animHeader;
        animState->unk_0x60[0] = anim->unk_0x1 & 0xF0;

        animState->totalAnimationFrames[0] = animHeader->totalKeyframes;
        //If the anim doesn't loop, maybe?
        //(i.e. no need to blend from last key to first key, so finished as soon as it reaches last key)
        if (animState->unk_0x60[0] == 0) {
            animState->totalAnimationFrames[0] -= 1.0f;
        }
        
        animState->unk_0x60[1] = animState->unk_0x60[0];
        animState->unk_0x34[1] = animState->unk_0x34[0];
        animState->unk_0x3c[0] = animState->unk_0x34[0];
        animState->unk_0x3c[1] = animState->unk_0x34[0];
        animState->animIndexes[1] = animState->animIndexes[0];
        animState->unk_0x48[0] = animState->animIndexes[0];
        animState->unk_0x48[1] = animState->animIndexes[0];
        animState->curAnimationFrame[1] = animState->curAnimationFrame[0];
        animState->totalAnimationFrames[1] = animState->totalAnimationFrames[0];
        animState->unk_0xc[1] = animState->unk_0xc[0];
    }
}

Animation* func_80019118(s16 animId, s16 modAnimId, s32 amap, s32 model) {
    Animation* anim;

    anim = NULL;
    queue_load_anim((void*)&anim, animId, modAnimId, amap, model);
    return anim;
}

#pragma GLOBAL_ASM("asm/nonmatchings/model/anim_load.s")

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
        if (anim == ((AnimSlot *)((s32*)gLoadedAnims + (index << 1)))->animation) {
            matchIndex = index;
            //@bug?: continues interating through animations after animation found
        }
    }
    
    if (matchIndex != -1) {
        ((AnimSlot *)((u8*)gLoadedAnims + (matchIndex << 1 << 2)))->referenceCount = clear;
        ((AnimSlot *)((u8*)gLoadedAnims + (matchIndex << 1 << 2)))->animation = clear;
        mmFree(anim);
    }        
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001943C.s")
#else
extern f32 gWorldX;
extern f32 gWorldZ;
typedef s16 (*ObjectDLLFunc0x20)(Object *object);
void _func_8001943C(Object *object, MtxF *mf, f32 yPrescale)
{
    if (object->parent == NULL) {
        object->srt.tx -= gWorldX;
        object->srt.tz -= gWorldZ;
    }

    if (object->srt.tx > *(f32*)0x800994c0 /* 32767.0f */ || object->srt.tx < *(f32*)0x800994c4 /* -32767.0f */ ||
        object->srt.tz > *(f32*)0x800994c0 /* 32767.0f */ || object->srt.tz < *(f32*)0x800994c4 /* -32767.0f */)
    {
        bzero(mf, sizeof(MtxF));
        mf->m[0][0] = 1.0f;
        mf->m[1][1] = 1.0f;
        mf->m[2][2] = 1.0f;
        mf->m[3][3] = 1.0f;

        if (object->parent == NULL) {
            object->srt.tx += gWorldX;
            object->srt.tz += gWorldZ;
        }
    }
    else
    {
        matrix_from_srt(mf, &object->srt);

        if (object->id == 0x427) { // if == OBJ_ScorpionRobot
            ObjectDLLFunc0x20 func = *(ObjectDLLFunc0x20*)((u8*)*object->dll + 0x20);
            s16 theta = func(object);
            matrix_from_yaw(theta, mf);
        }

        if (yPrescale != 1.0f) {
            matrix_prescale_y(mf, yPrescale);
        }

        if (object->parent == NULL) {
            object->srt.tx += gWorldX;
            object->srt.tz += gWorldZ;
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
void _func_80019730(ModelInstance *modelInst, Model *model, Object *object, MtxF *param_4)
{
    s32 mtxSelector;
    AnimState *animState0;

    func_8001A640(object, modelInst, model);

    modelInst->unk_0x34 ^= 0x1;
    mtxSelector = modelInst->unk_0x34 & 0x1;

    animState0 = modelInst->animState0;

    if (animState0->unk_0x63 & 0x4)
    {
        short xyz[3];
        Vec3f v;
        func_8001A3FC(modelInst, 0, 0, object->unk0x98, object->srt.scale, &v, xyz);
        *(s16*)0x800903dc = xyz[0];
        *(s16*)0x800903de = xyz[1];
        *(s16*)0x800903e0 = xyz[2];
    }

    if (modelInst->model->unk_0x71 & 0x8)
    {
        func_800199A8(param_4, modelInst, animState0, object->unk0x98, 0x7f);
    }
    else
    {
        if (modelInst->animState0->unk_0x63 & 0x8)
        {
            AnimState *animState1 = modelInst->animState1;

            func_80019FC0(param_4, modelInst, animState0, object->unk0x98, 0x7f, 0, 0, 2, 0x14, animState0->unk_0x5a);
            func_80019FC0(param_4, modelInst, animState1, object->unk0x9c, 0x7f, 0, 0, 2, 0x18, animState0->unk_0x5a);
            func_80019FC0(param_4, modelInst, animState0, object->unk0x98, 0x7f, 0, 0, 0, 7, animState1->unk_0x58);
            func_80019FC0(param_4, modelInst, animState0, object->unk0x98, 0x7f, 0, 1, 1, 1, animState0->unk_0x58);
        }
        else
        {
            func_800199A8(param_4, modelInst, animState0, object->unk0x98, 0x7f);
            if (modelInst->animState1 != NULL && object->curModAnimIdLayered >= 0) {
                func_800199A8(param_4, modelInst, modelInst->animState1, object->unk0x9c, -1);
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
void _func_8001A640(Object *object, ModelInstance *modelInst, Model *model)
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
    for (i = 0; i < object->ptr0x50->unk_0x72; i++)
    {
        if (object->ptr0x50->unk_0x10[object->modelInstIdx + j + 1] != 0xff)
        {
            s16 *src = &object->ptr0x6c[i * 9];
            u32 code = amap[object->ptr0x50->unk_0x10[object->modelInstIdx + j + 1]] << 6;

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

        j += object->ptr0x50->unk_0x5d + 1;
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
            if (blendshape->unk0xC == -1 && blendshape->id == -1)
                return;
            arg5 |= 6;
        }
        blendshape->unk0xC = arg1;
        blendshape->id = shapeId;
        if (!(arg5 & 0x10)) {
            blendshape->strength = 0.0f;
        }
        blendshape->unk0x4 = -1.0f;
        blendshape->unk0x8 = arg3;
        blendshape->unk0xE = arg5 | 4;
    }
}

void func_8001AFCC(ModelInstance *modelInst, s32 param2, f32 param3) {
    ModelInstanceBlendshape *var1;

    if (param2 < 3 && modelInst->model->blendshapes != NULL) {
        var1 = &modelInst->blendshapes[param2];
        var1->strength = param3;
        var1->unk0xE |= 4;
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

        if (var1->strength != var1->unk0x4 || (var1->unk0xE & 0xE) != 0) {
            return 1;
        }
    }

    return 0;
}

void func_8001B084(ModelInstance *modelInst, f32 param2) {
    int i;
    ModelInstanceBlendshape *var1;
    
    if (modelInst->model->blendshapes == NULL) {
        return;
    }

    for (i = 0; i < 3; i++) {
        var1 = &modelInst->blendshapes[i];

        if (var1->unk0xC != -1 || var1->id != -1) {
            if ((var1->unk0xE & 1) == 0) {
                var1->strength += var1->unk0x8 * param2;
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B100.s")

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B49C.s")
#else
extern s32 gNumLoadedModels;

void func_8001B49C(void) {
    ModelSlot* modelSlot;
    s32 index;
    s32 modelID;

    for (index = 0; index < gNumLoadedModels; index++){
        modelSlot = &gLoadedModels[index];
        modelID = modelSlot->id;

        if (modelID < 0){
            continue;
        }
        if (index && modelSlot && modelID)
        {
        }
    }
}
#endif

void doNothing_8001B4E4(s32 arg) {}
