#include "common.h"

typedef struct {
    u8 flags;
    s8 visibility;       //Visibility of the affected Block Shapes
    u8 gamebitState;     //Whether the queried gamebit is set
    u8 gamebitStatePrev; //The queried gamebit's value on the previous tick
    u8 bitMask;          //A mask used to query a specific bit in the gamebit's bitfield
} VisAnimator_Data;

typedef struct {
    ObjSetup base;
    s16 gamebitID;          //gamebit controlling visibility (usually multi-bit, used as a bitfield)
    u8 animatorID1;         //the animatorID tag of the Block Shapes that will be shown/hidden
    u8 initialVisibility;   //Boolean
    u8 bitIndex;            //the index to check on the gamebit bitfield
    u8 animatorID2;         //up to 3 different animatorIDs can be specified
    u8 animatorID3;         //up to 3 different animatorIDs can be specified
} VisAnimator_Setup;

typedef enum {
    VisAnimator_FLAG_Block_Animate_Needed = 1
} VisAnimator_Flags;

static void VisAnimator_animateBlockShapes(Block* block, Object* self, VisAnimator_Data* objData, VisAnimator_Setup* objSetup);

// offset: 0x0 | ctor
void VisAnimator_ctor(void* dll) { }

// offset: 0xC | dtor
void VisAnimator_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void VisAnimator_obj_Setup(Object* self, VisAnimator_Setup* objSetup, s32 reset) {
    VisAnimator_Data* objData;
    Block* block;
    u16 gamebitSet;

    objData = self->data;

    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);

    //Set initial visibility, inverting if the relevant bit is set in the multi-bit flag 
    objData->visibility = objSetup->initialVisibility;
    objData->bitMask = 1 << objSetup->bitIndex;
    if (mainGetBits(objSetup->gamebitID) & objData->bitMask) {
        objData->visibility ^= VisAnimator_FLAG_Block_Animate_Needed;
    }

    //Get local Blocks model and animate tagged Shapes' visibility
    block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(
        self->srt.transl.x,self->srt.transl.y, self->srt.transl.z));
    if (block) {
        VisAnimator_animateBlockShapes(block, self, objData, objSetup);
    }

    //Check a specific bit from the multi-bit gamebit's current value
    gamebitSet = mainGetBits(objSetup->gamebitID) & objData->bitMask;
    objData->gamebitState = gamebitSet;
    objData->gamebitStatePrev = gamebitSet;

    objData->flags |= VisAnimator_FLAG_Block_Animate_Needed;
}

// offset: 0x138 | func: 1 | export: 1
void VisAnimator_obj_Control(Object* self) {
    VisAnimator_Setup* objSetup;
    VisAnimator_Data* objData;
    Block* block;

    objSetup = (VisAnimator_Setup*)self->setup;
    objData = self->data;

    //Get local Block model
    block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if (block == NULL) {
        objData->flags |= VisAnimator_FLAG_Block_Animate_Needed;
        return;
    }

    //Check a specific bit from the multi-bit gamebit's current value
    objData->gamebitState = mainGetBits(objSetup->gamebitID) & objData->bitMask;
    if (objData->gamebitStatePrev != objData->gamebitState) {
        objData->visibility ^= 1;
        objData->flags |= VisAnimator_FLAG_Block_Animate_Needed;
    }
    objData->gamebitStatePrev = objData->gamebitState;

    //Animate Block Shapes if needed
    if (objData->flags & VisAnimator_FLAG_Block_Animate_Needed) {
        VisAnimator_animateBlockShapes(block, self, objData, objSetup);
        objData->flags &= ~VisAnimator_FLAG_Block_Animate_Needed;
    }
}

// offset: 0x254 | func: 2 | export: 2
void VisAnimator_obj_Update(Object* self) { }

// offset: 0x260 | func: 3 | export: 3
void VisAnimator_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x278 | func: 4 | export: 4
void VisAnimator_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x288 | func: 5 | export: 5
u32 VisAnimator_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x298 | func: 6 | export: 6
u32 VisAnimator_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(VisAnimator_Data);
}

// offset: 0x2AC | func: 7
void VisAnimator_animateBlockShapes(Block* block, Object* self, VisAnimator_Data* objData, VisAnimator_Setup* objSetup) {
    BlockShape* shapes;
    s32 shapeIndex;

    shapeIndex = 0;
    shapes = block->shapes;
    while (shapeIndex < block->shapeCount){
        if (objSetup->animatorID1 == shapes[shapeIndex].animatorID || 
            (objSetup->animatorID2 && objSetup->animatorID2 == shapes[shapeIndex].animatorID) || 
            (objSetup->animatorID3 && objSetup->animatorID3 == shapes[shapeIndex].animatorID)
        ){
            if (objData->visibility) {
                shapes[shapeIndex].flags &= ~RENDER_SHAPE_HIDE;
            } else {
                shapes[shapeIndex].flags |= RENDER_SHAPE_HIDE;
            }
        }
        shapeIndex++;
    }
}
