#include "common.h"

extern Block_0x28Struct* func_8004A284(Block*, s32);
extern BlockTexture* func_8004A2CC(s32);

typedef struct {
    s32 materialID;     //Index within object's local Blocks model's materials (the frame of the material's texture animation will be manipulated)
    u8 speed;           //Playback rate when advancing through the texture animation
    s32 endFrame;       //End frame of the animated texture
    s32 loopStartFrame; //Frame to loop back to after reaching endFrame (only when looping)
    s32 frame;          //Current frame of the animated texture
    u32 unused: 1;
    u32 finished: 1;
    u32 playing: 1;
} TexFrameAnimator_Data;

typedef struct {
    ObjSetup base;
    s8 loopStartFrame;   //Frame to loop back to after reaching endFrame (only when looping)
    s8 materialIndex;    //Index within object's local Blocks model's materials (the frame of the material's texture animation will be manipulated)
    s16 endFrame;        //End frame of texture animation
    s16 speed;           //Playback rate when advancing through the texture animation
    s16 gamebitFinished; //GamebitID to set when end of animation reached (also prevents looping)
    s16 gamebitPlay;     //GamebitID to query, starting animation
} TexFrameAnimator_Setup;

// offset: 0x0 | ctor
void TexFrameAnimator_ctor(void *dll) { }

// offset: 0xC | dtor
void TexFrameAnimator_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void TexFrameAnimator_setup(Object* self, TexFrameAnimator_Setup* objSetup, s32 arg2) {
    TexFrameAnimator_Data *objData;

    objData = self->data;

    objData->materialID = objSetup->materialIndex;
    objData->endFrame = objSetup->endFrame << 8;
    objData->speed = objSetup->speed;
    objData->loopStartFrame = objSetup->loopStartFrame << 8;

    if (0 && objSetup->speed == 0){
        diPrintf(" TFRAME ANIMATOR: no speed give \n");
    }

    //Check if already finished playing
    if ((objData->finished = main_get_bits(objSetup->gamebitFinished))){
        objData->frame = objData->endFrame;
        objData->playing = TRUE;
    }

    self->unkB0 |= 0x2000;
    self->unkB0 |= 0x4000;
}

// offset: 0xDC | func: 1 | export: 1
void TexFrameAnimator_control(Object* self) {
    TexFrameAnimator_Data* objData;
    Block* block;
    Block_0x28Struct* blockMaterialInfo;
    BlockTexture* blockTexture;
    TexFrameAnimator_Setup* objSetup;

    objData = self->data;
    objSetup = (TexFrameAnimator_Setup*)self->setup;

    //Check if texture animation should start
    if (objData->playing == FALSE && 
        main_get_bits(objSetup->gamebitPlay) && 
        objData->finished == FALSE) {

        objData->frame = 0;
        objData->playing = TRUE;
    }

    //Return if no playback needed
    if (objData->playing == FALSE || !objData->materialID) {
        return;
    }

    //Get object's local Blocks model
    block = func_80044BB0(func_8004454C(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if (!block || !(block->vtxFlags & 8)) {
        return;
    }

    //Get local Block's relevant Material
    blockMaterialInfo = func_8004A284(block, objData->materialID);
    if (!blockMaterialInfo) {
        return;
    }

    //Get Block Material's Texture
    blockTexture = func_8004A2CC(blockMaterialInfo->texIdx);

    //Advance texture animation frame
    objData->frame += objData->speed * gUpdateRate;
    diPrintf(" TEXFRAMEANIM %i ", objData->frame);

    //Handle ending/looping
    if (objData->frame < 0){
        objData->frame = 0;
    } else if (objData->frame > objData->endFrame) {
        if (objSetup->gamebitFinished != NO_GAMEBIT) {
            main_set_bits(objSetup->gamebitFinished, TRUE);
            if (0){
                diPrintf(" Set bit %i ", objSetup->gamebitFinished);
            }
            objData->playing = FALSE;
            objData->finished = TRUE;
            objData->frame = objData->endFrame;
        } else {
            objData->frame = objData->loopStartFrame;
        }
    }

    //Set Texture's updated frame index
    blockTexture->unk4 = objData->frame;
}

// offset: 0x2B8 | func: 2 | export: 2
void dll_361_update(Object *self) { }

// offset: 0x2C4 | func: 3 | export: 3
void TexFrameAnimator_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility){
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x318 | func: 4 | export: 4
void TexFrameAnimator_free(Object *self, s32 a1) { }

// offset: 0x328 | func: 5 | export: 5
u32 TexFrameAnimator_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x338 | func: 6 | export: 6
u32 TexFrameAnimator_get_data_size(Object *self, u32 a1){
    return sizeof(TexFrameAnimator_Data);
}
