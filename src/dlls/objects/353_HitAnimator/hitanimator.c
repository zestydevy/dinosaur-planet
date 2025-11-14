#include "common.h"

u32 func_80058F50(void);

typedef struct {
    s8 active;
    u8 flags;
    u8 gamebitState;
    u8 gamebitStatePrev;
} HitAnimator_Data;

typedef struct {
    ObjSetup base;
    s16 gamebitActivate; //gamebitID to check before animating
    u8 enableFlag1;      //unknown purpose
    u8 blocksAnimatorID; //when animating Blocks model Shapes
    u8 mode;             //decides which behaviour to use: removing/adding HITS lines, removing/adding Blocks Shapes, fading in/out Blocks Shapes
    u8 hitsAnimatorID;   //when animating HITS lines
} HitAnimator_Setup;

typedef enum {
    HitAnimator_Mode_Invert = 1,  //Remove the target when gamebit set (instead of enabling it when gamebit set)
    HitAnimator_Mode_No_Fade = 2, //When targetting BLOCKS Shapes: show/hide immediately
    HitAnimator_Mode_BLOCKS = 4,  //Targetting BLOCKS Shapes
    HitAnimator_Mode_HITS = 8     //Targetting HITS Lines
} HitAnimator_Modes;

typedef enum {
    HitAnimator_No_Flags = 0,
    HitAnimator_Flag_1 = 1, //unknown purpose
    HitAnimator_Flag_HITS_Animate_Needed = 2,
    HitAnimator_Flag_BLOCK_Animate_Needed = 4
} HitAnimator_Flags;

static void HitAnimator_animate_blocks_shapes(Block* block, Object* self, HitAnimator_Data* objData, HitAnimator_Setup* objSetup);

// offset: 0x0 | ctor
void HitAnimator_ctor(void *dll) { }

// offset: 0xC | dtor
void HitAnimator_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void HitAnimator_setup(Object* self, HitAnimator_Setup* objSetup, s32 arg2) {
    Block* block;
    s8 gamebitValue;
    HitAnimator_Data* objData;

    objData = self->data;

    objData->active = objSetup->mode & HitAnimator_Mode_Invert;
    objData->flags = HitAnimator_No_Flags;

    if (main_get_bits(objSetup->gamebitActivate)) {
        objData->active ^= 1;
        if (objSetup->enableFlag1 == TRUE) {
            objData->flags |= HitAnimator_Flag_1;
        }
    }

    block = func_80044BB0(func_8004454C(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if (block && (objSetup->mode & HitAnimator_Mode_BLOCKS) && objSetup->blocksAnimatorID) {
        HitAnimator_animate_blocks_shapes(block, self, objData, objSetup);
    }

    objData->flags |= HitAnimator_Flag_HITS_Animate_Needed;
    if (objSetup->mode & HitAnimator_Mode_BLOCKS) {
        objData->flags |= HitAnimator_Flag_BLOCK_Animate_Needed;
    }

    gamebitValue = main_get_bits(objSetup->gamebitActivate);
    objData->gamebitStatePrev = gamebitValue;
    objData->gamebitState = gamebitValue;
}

// offset: 0x160 | func: 1 | export: 1
void HitAnimator_control(Object* self) {
    HitAnimator_Data* objData;
    HitAnimator_Setup* objSetup;
    Block* block;
    u8 gamebitValue;

    objSetup = (HitAnimator_Setup*)self->setup;
    objData = self->data;

    //Get object's local Blocks model
    block = func_80044BB0(func_8004454C(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if (block == NULL) {
        objData->flags &= ~HitAnimator_Flag_1;
        objData->flags |= HitAnimator_Flag_BLOCK_Animate_Needed;
        return;
    }

    //Check if gamebit's value changed
    objData->gamebitState = main_get_bits(objSetup->gamebitActivate);
    if (objData->gamebitStatePrev != objData->gamebitState) {
        objData->active ^= 1;
        if (objSetup->enableFlag1 == TRUE) {
            objData->flags |= HitAnimator_Flag_1;
        }
        if (objSetup->mode & HitAnimator_Mode_HITS) {
            objData->flags |= HitAnimator_Flag_HITS_Animate_Needed;
        }
        if (objSetup->mode & HitAnimator_Mode_BLOCKS) {
            objData->flags |= HitAnimator_Flag_BLOCK_Animate_Needed;
        }
    }
    objData->gamebitStatePrev = objData->gamebitState;

    //Animate any HITS lines with the target animatorID
    if (objSetup->mode & HitAnimator_Mode_HITS) {
        if (func_80058F50()) {
            objData->flags |= HitAnimator_Flag_HITS_Animate_Needed;
        }
        if ((objData->flags & HitAnimator_Flag_HITS_Animate_Needed) && !func_80058F50()) {
            func_80059038(objSetup->hitsAnimatorID, self->parent, objData->active);
            objData->flags &= ~HitAnimator_Flag_HITS_Animate_Needed;
        }
    }

    //Animate any Block Shapes with the target animatorID
    if ((objSetup->mode & HitAnimator_Mode_BLOCKS) && objSetup->blocksAnimatorID && (objData->flags & HitAnimator_Flag_BLOCK_Animate_Needed)) {
        HitAnimator_animate_blocks_shapes(block, self, objData, objSetup);
        objData->flags &= ~HitAnimator_Flag_BLOCK_Animate_Needed;
    }
}

// offset: 0x354 | func: 2 | export: 2
void HitAnimator_update(Object *self) { }

// offset: 0x360 | func: 3 | export: 3
void HitAnimator_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x3B4 | func: 4 | export: 4
void HitAnimator_free(Object *self, s32 a1) { }

// offset: 0x3C4 | func: 5 | export: 5
u32 HitAnimator_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x3D4 | func: 6 | export: 6
u32 HitAnimator_get_data_size(Object *self, u32 a1) {
    return sizeof(HitAnimator_Data);
}

// offset: 0x3E8 | func: 7
void HitAnimator_animate_blocks_shapes(Block* block, Object* self, HitAnimator_Data* objData, HitAnimator_Setup* objSetup){
    s32 shapeIndex;
    BlockShape* shapes;
    BlockShape* shape;

    shapeIndex = 0;
    shapes = block->shapes;
    //Iterate through Shapes and animate those with matching animatorIDs
    while (shapeIndex < block->shapeCount){
        shape = &shapes[shapeIndex];
        if (shape->animatorID == objSetup->blocksAnimatorID) {
            //Show/hide (or fade in/out) Shapes
            if (objData->active) {
                shape->flags &= ~0x800;
                if (objSetup->mode & HitAnimator_Mode_No_Fade) {
                    shape->flags &= 0xFFDFFFFF;
                }
            } else {
                shape->flags |= 0x800;
                if (objSetup->mode & HitAnimator_Mode_No_Fade) {
                    shape->flags |= 0x200000;
                }
            }
        }
        shapeIndex++;
    }
}
