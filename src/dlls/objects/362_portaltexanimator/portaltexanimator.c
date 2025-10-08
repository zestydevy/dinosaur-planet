#include "PR/ultratypes.h"
#include "functions.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/memory.h"
#include "sys/objects.h"

Block* func_80044BB0(s32 blockIndex);
s32 func_8004A528(Object* obj, u8 animatorID);

typedef struct {
/*00*/ ObjCreateInfo base;
/*18*/ s16 flagID;
/*1A*/ s16 unk1A;
/*1C*/ u8 maxOpacity;
/*1D*/ u8 minOpacity;
/*1E*/ s8 animatorID;
/*1E*/ s8 unk1F;
/*20*/ s8 unk20;
/*21*/ u8 unk21;
/*22*/ u16 minDistance;
} PortalTexAnimatorCreateInfo;

typedef struct {
/*00*/ s32 animatedVertexCount;
/*04*/ f32 minDistance;
/*08*/ f32 maxDistance;
/*0C*/ f32 unkC;
/*10*/ s32 unk10;
/*14*/ s16 vertexOpacity;
/*16*/ s8 animatorID;
/*17*/ s8 enabled; //unfinished, doesn't affect behaviour
/*18*/ s8 blockFound;
/*19*/ s8 unk19;
/*1A*/ s8 unk1A;
/*1B*/ s8 unk1B;
} PortalTexAnimatorState;

static void portaltexanimator_animate_vertices(PortalTexAnimatorState* state, PortalTexAnimatorCreateInfo* createInfo, Block* block);

// offset: 0x0 | ctor
void portaltexanimator_ctor(void* dll){
}

// offset: 0xC | dtor
void portaltexanimator_dtor(void* dll){
}

// offset: 0x18 | func: 0 | export: 0
void portaltexanimator_create(Object* self, PortalTexAnimatorCreateInfo* arg1, s32 arg2) {
    PortalTexAnimatorState* state;

    state = self->state;

    state->unk19 = -1;
    state->minDistance = arg1->minDistance;
    state->maxDistance = arg1->base.loadDistance * 8;
}

// offset: 0x78 | func: 1 | export: 1
void portaltexanimator_update(Object* self) {
    PortalTexAnimatorState* state;
    PortalTexAnimatorCreateInfo* createInfo;
    Block* block;
    f32 distance;
    f32 blendValue;

    createInfo = (PortalTexAnimatorCreateInfo*)self->createInfo;
    state = self->state;

    //Get the object's local BLOCKS model
    block = func_80044BB0(func_8004454C(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if (block == NULL) {
        state->blockFound = FALSE;
        return;
    }

    //Bail if the BLOCKS model isn't animatable
    if (!(block->vtxFlags & 8)) {
        return;
    }

    //Animate tagged shapes' vertex opacity based on player distance
    if (state->animatedVertexCount == 0) {
        state->animatorID = createInfo->animatorID;
        state->animatedVertexCount = func_8004A528(self, state->animatorID);

        if (state->animatedVertexCount == 0) {
            state->animatorID = 0;
        }

        //Bail if no shape animation tag is specified
        if (!state->animatorID){
            return;
        }

        state->minDistance = createInfo->minDistance;
        state->maxDistance = createInfo->base.loadDistance * 8;
        state->unkC = createInfo->minDistance;

        if (createInfo->flagID == -1) {
            state->enabled = 1;
        } else {
            state->enabled = get_gplay_bitstring(createInfo->flagID);
        }

        //Set both vertex animation buffers' animated vertices to max opacity
        state->vertexOpacity = createInfo->maxOpacity;
        portaltexanimator_animate_vertices(state, createInfo, block); 
        block->vtxFlags ^= 1;
        portaltexanimator_animate_vertices(state, createInfo, block);
        block->vtxFlags ^= 1;
    } 

    if (state->animatorID) {
        distance = vec3_distance(&self->positionMirror, &get_player()->positionMirror) - state->minDistance;
        if (distance < 0.0f) {
            distance = 0.0f;
        }

        blendValue = distance / (state->maxDistance - state->minDistance);
        if (blendValue < 0.0f) {
            blendValue = 0.0f;
        }

        state->vertexOpacity = createInfo->minOpacity + (createInfo->maxOpacity - createInfo->minOpacity) * blendValue;
        portaltexanimator_animate_vertices(state, createInfo, block);
    }
}

// offset: 0x310 | func: 2
void portaltexanimator_animate_vertices(PortalTexAnimatorState* state, PortalTexAnimatorCreateInfo* createInfo, Block* block) {
    BlockShape *shapes;
    Vtx_t *vertices;
    s32 shapeIndex;
    s32 vertexIndex;

    vertices = block->vertices2[block->vtxFlags & 1];

    shapeIndex = 0;
    shapes = block->shapes;

    //Iterate over shapes, and update all vertices' alpha on shapes with matching animatorID tag
    while (shapeIndex < block->shapeCount){

        if (state->animatorID == shapes[shapeIndex].unk_0x14){
            for (vertexIndex = shapes[shapeIndex].vtxBase; vertexIndex < shapes[shapeIndex + 1].vtxBase; vertexIndex++){
                vertices[vertexIndex].cn[3] = state->vertexOpacity; //@bug: setting a 16-bit value on an 8-bit colour field
            }

            //Switch shape's draw flags when opacity is zero
            if (state->vertexOpacity == 0){
                shapes[shapeIndex].flags |= 0x200000;
                if (createInfo->unk21 != 0){
                    shapes[shapeIndex].flags |= 0x800;
                }
            } else {
                shapes[shapeIndex].flags &= 0xFFDFFFFF;
                if (createInfo->unk21 != 0){
                    shapes[shapeIndex].flags &= ~0x800;
                }
            }
        }

        shapeIndex++;
    }

}

// offset: 0x414 | func: 3 | export: 2
void dll_362_func_414(Object *self) { }

// offset: 0x420 | func: 4 | export: 3
void portaltexanimator_draw(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gfx, mtx, vtx, pols, 1.0f);
    }
}

// offset: 0x474 | func: 5 | export: 4
void portaltexanimator_destroy(Object* self, s32 arg1) {
    PortalTexAnimatorState* state = self->state;
    s32 ptr;

    ptr = state->unk10;
    if (ptr) {
        mmFree((void*)ptr);
    }
}

// offset: 0x4C4 | func: 6 | export: 5
s32 portaltexanimator_func_4C4(Object* self){
    return 0;
}

// offset: 0x4D4 | func: 7 | export: 6
s32 portaltexanimator_get_state_size(Object* self, s32 arg1){
    return sizeof(PortalTexAnimatorState);
}

/*0x0*/ static const char str_0[] = "%d:%d,%d %d:%d\n";
