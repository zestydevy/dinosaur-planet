#include "common.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/355_WallAnimator.h"
#include "sys/map.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "sys/objtype.h"

#define DIG_DEPTH_MAX 3000
#define OPACITY_MAX 0xFF

static void WallAnimator_calculate_vertex_weights(Object* self, WallAnimator_Data* objData);

// offset: 0x0 | ctor
void WallAnimator_ctor(void *dll) { }

// offset: 0xC | dtor
void WallAnimator_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WallAnimator_setup(Object* self, WallAnimator_Setup* objSetup, s32 reset) {
    s32 pad[2];
    s32 animatedVtxCount;
    WallAnimator_Data* objData;

    objData = self->data;
    
    self->srt.yaw = objSetup->yaw;
    
    objData->animatorID = objSetup->animatorID;
    objData->sidekickCost = objSetup->sidekickCost;
    objData->digDepth = 0;
    objData->previousDigDepth = -1;
    objData->falloffRadius = objSetup->falloffRadius;
    objData->gamebitDug = objSetup->gamebitDug;
    objData->unk28 = objSetup->unk26;
    
    //Check if the local Block has any vertices with the given animatorID
    animatedVtxCount = blockGetAnimatorVertexCount(self, objData->animatorID);
    if (animatedVtxCount == 0) {
        objData->animatorID = 0;
    }
    if (objData->animatorID == 0){
        return;
    }

    //Calculate the dig vector
    objData->digVector.x = mathSinfInterp(self->srt.yaw);
    objData->digVector.y = 0.0f;
    objData->digVector.z = mathCosfInterp(self->srt.yaw);
    
    objData->zero = 0.0f;
    
    //Set up vertex weights
    if (reset == FALSE) {
        objData->vtxWeights = mmAlloc(animatedVtxCount * sizeof(f32), ALLOC_TAG_TRACK_COL, NULL);
        WallAnimator_calculate_vertex_weights(self, objData);
    }
    
    //Check if the wall's already been dug through
    if (mainGetBits(objSetup->gamebitDug)) {
        objData->digFinished = TRUE;
        objData->digDepth = DIG_DEPTH_MAX;
    }
    
    objAddObjectType(self, OBJTYPE_TrickyTarget);
    objAddObjectType(self, OBJTYPE_WallAnimator);
}

// offset: 0x1E8 | func: 1 | export: 1
void WallAnimator_control(Object* self) {
    WallAnimator_Data* objData;
    Block* block;
    WallAnimator_Setup* objSetup;
    BlockVertex *vertices;
    BlockShape* shapes;
    s32 shapeIdx;
    s32 animVtxIdx;
    Object* pushblock;
    f32 displacement;
    f32 distance;
    u8 fadeOut;
    Object* player;
    Object* sidekick;
    s32 vertexIdx;

    objData = self->data;
    
    //Return early if the dig spot doesn't animate anything
    if (objData->animatorID == 0) {
        return;
    }

    objSetup = (WallAnimator_Setup*)self->setup;
    
    //Show the Find command when the player is nearby (provided there isn't a nearby Pushblock obstructing the dig spot)
    if (objData->digDepth != DIG_DEPTH_MAX) {
        player = objGetPlayer();
        sidekick = objGetSidekick();
        distance = M_INFINITY_F;
        pushblock = objGetNearestTypeTo(OBJTYPE_PushBlock, self, &distance);

        if (sidekick && ((pushblock == NULL) || (pushblock && (distance > 35.0f)))) {
            distance = vec3DistanceSquared(&self->globalPosition, &player->globalPosition);
            if (distance <= SQ(objSetup->findCommandRadius)) {
                ((DLL_ISidekick*)sidekick->dll)->vtbl->enable_command(sidekick, Sidekick_Command_INDEX_1_Find);
            }
        }
    }
    
    //Return if the digging hasn't started yet
    if (objData->digDepth == 0) {
        return;
    }
    
    //Get local Blocks model and make sure its vertices are animatable
    block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(
        self->srt.transl.x, self->srt.transl.y, self->srt.transl.z)
    );
    if ((block == NULL) || !(block->vtxFlags & 8)) {
        return;
    }
    
     //Tick animation for 2 frames (including the current one) whenever the dig depth changes
    if (objData->digDepth != objData->previousDigDepth) {
        objData->animUpdates = 2;
        objData->previousDigDepth = objData->digDepth;
    }
    if (objData->animUpdates == 0) {
        return;
    }
    objData->animUpdates--;
    
    //Handle when digging is finished
    if ((objData->digDepth >= DIG_DEPTH_MAX) && (objData->digFinished == FALSE)) {
        objData->digFinished = TRUE;
        objData->digDepth = DIG_DEPTH_MAX;

        mainSetBits(objData->gamebitDug, TRUE);

        dll_amSfx->Play(self, SOUND_B01_Success_Chime, MAX_VOLUME, NULL, NULL, 0, NULL);
    }

    //Animate vertices
    vertices = (BlockVertex*)block->vertices2[block->vtxFlags & 1]; 
    shapeIdx = 0;
    animVtxIdx = 0;
    shapes = block->shapes;
    for (; shapeIdx < block->shapeCount; shapeIdx++){
        if (objData->animatorID == shapes[shapeIdx].animatorID) {

            for (vertexIdx = shapes[shapeIdx].vtxBase; vertexIdx < shapes[shapeIdx + 1].vtxBase; vertexIdx++, animVtxIdx++) {
                
                if (objData->vtxWeights[animVtxIdx] > 0.0f) {
                    displacement = (objData->digDepth / 100.0f); //NOTE: the vtxWeight isn't multiplied in here, unlike GroundAnimators
                    
                    //Extrude vertices
                    if ((shapes[shapeIdx].flags & RENDER_UNK20000000)) {
                        vertices[vertexIdx].ob[0] = block->vertices[vertexIdx].ob[0] + displacement*objData->digVector.x;
                        vertices[vertexIdx].ob[2] = block->vertices[vertexIdx].ob[2] + displacement*objData->digVector.z;
                    } else {
                        vertices[vertexIdx].ob[0] = block->vertices[vertexIdx].ob[0] + displacement*objData->digVector.x;
                        vertices[vertexIdx].ob[2] = block->vertices[vertexIdx].ob[2] + displacement*objData->digVector.z;
                    }
                    
                    //Animate vertex colours
                    displacement = displacement / 30;
                    fadeOut = displacement * OPACITY_MAX;
                    vertices[vertexIdx].cn[0] = (block->vertices[vertexIdx].cn[0] * (OPACITY_MAX - fadeOut)) >> 8;
                    vertices[vertexIdx].cn[1] = (block->vertices[vertexIdx].cn[1] * (OPACITY_MAX - fadeOut)) >> 8;
                    vertices[vertexIdx].cn[2] = (block->vertices[vertexIdx].cn[2] * (OPACITY_MAX - fadeOut)) >> 8;
                    vertices[vertexIdx].cn[3] = OPACITY_MAX - fadeOut;
                }
            }
        }
    }
}

// offset: 0x6E8 | func: 2 | export: 2
void WallAnimator_update(Object *self) { }

// offset: 0x6F4 | func: 3 | export: 3
void WallAnimator_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x748 | func: 4 | export: 4
void WallAnimator_free(Object* self, s32 onlySelf) {
    WallAnimator_Data* objData = self->data;

    if (objData->vtxWeights != NULL) {
        mmFree(objData->vtxWeights);
    }
    
    objFreeObjectType(self, OBJTYPE_TrickyTarget);
    objFreeObjectType(self, OBJTYPE_WallAnimator);
}

// offset: 0x7D0 | func: 5 | export: 5
u32 WallAnimator_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x7E0 | func: 6 | export: 6
u32 WallAnimator_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(WallAnimator_Data);
}

// offset: 0x7F4 | func: 7
void WallAnimator_calculate_vertex_weights(Object* self, WallAnimator_Data* objData) {
    BlockShape* shapes;
    f32 distanceSquared;
    f32 dy;
    f32 objBlockX;
    f32 objBlockZ;
    f32 dx;
    f32 dz;
    f32 tValue;
    f32 minDistance;
    s32 closestVtxID;
    s32 shapeIdx;
    s32 blockWorldGridX;
    s32 blockWorldGridZ;
    s32 vtxID;
    Block* block;
    s32 animVtxIdx;
    f32 radiusSq;

    //Get local Block and make sure its vertices are animatable
    block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(
        self->srt.transl.x, self->srt.transl.y, self->srt.transl.z)
    );
    if ((block == NULL) || !(block->vtxFlags & 8)) {
        return;
    }
    
    //Get the WallAnimator's position relative to the Blocks model's local origin
    blockWorldGridX = floorf((self->srt.transl.x - gWorldX) / BLOCKS_GRID_UNIT_F);
    blockWorldGridZ = floorf((self->srt.transl.z - gWorldZ) / BLOCKS_GRID_UNIT_F);
    
    objBlockX = self->srt.transl.x - (blockWorldGridX * BLOCKS_GRID_UNIT_F + gWorldX);
    objBlockZ = self->srt.transl.z - (blockWorldGridZ * BLOCKS_GRID_UNIT_F + gWorldZ);
    
    //Find the vertexID closest to the WallAnimator
    minDistance = 10000.0f;
    shapeIdx = 0;
    closestVtxID = -1;
    shapes = block->shapes;
    for (; shapeIdx < block->shapeCount; shapeIdx++) {
        if (objData->animatorID == shapes[shapeIdx].animatorID) {
            for (vtxID = shapes[shapeIdx].vtxBase; vtxID < shapes[shapeIdx + 1].vtxBase; vtxID++) {
                dx = block->vertices[vtxID].ob[0];
                dz = block->vertices[vtxID].ob[2];
                dx -= objBlockX;
                dz -= objBlockZ;
                
                distanceSquared = SQ(dx) + SQ(dz);
                if (distanceSquared < minDistance) {
                    minDistance = distanceSquared;
                    closestVtxID = vtxID;
                }
            }
        }
    }

    //Store the closest vertex's coords
    objData->closestVtxPosition.x = block->vertices[closestVtxID].ob[0];
    objData->closestVtxPosition.y = block->vertices[closestVtxID].ob[1];
    objData->closestVtxPosition.z = block->vertices[closestVtxID].ob[2];

    //Search through the Block's shapes, looking for any tagged with the animatorID
    tValue = 1.0f;
    shapes = block->shapes;
    for (shapeIdx = 0, animVtxIdx = 0; shapeIdx < block->shapeCount; shapeIdx++) {
        //Only animate shapes that have a matching animatorID
        if (objData->animatorID == shapes[shapeIdx].animatorID) {

            //Calculate vertices' influence weights, based on falloff and distance from central vertex
            for (vtxID = shapes[shapeIdx].vtxBase; vtxID < shapes[shapeIdx + 1].vtxBase; vtxID++, animVtxIdx++) {
                dx = block->vertices[vtxID].ob[0];
                dy = block->vertices[vtxID].ob[1];
                dz = block->vertices[vtxID].ob[2];
                
                dx -= objData->closestVtxPosition.x;
                dy -= objData->closestVtxPosition.y;
                dz -= objData->closestVtxPosition.z;

                tValue = (SQ(dx) + SQ(dy) + SQ(dz));
                radiusSq = SQ(objData->falloffRadius);
                tValue /= radiusSq;
                if (tValue > 1.0f) {
                    tValue = 1.0f;
                }

                tValue = SQ(tValue);
                tValue = 1.0f - tValue;
                objData->vtxWeights[animVtxIdx] = tValue;
            }
        }
    }
}

// offset: 0xB54 | func: 8 | export: 7
f32 WallAnimator_tick_dig(Object* self, Object* sidekick) {
    WallAnimator_Data* objData;
    f32 dy;
    f32 dx;
    f32 dz;

    objData = self->data;
    
    //Return 0 if sidekick's too far above/below the dig spot
    dy = sidekick->srt.transl.y - self->srt.transl.y;
    if ((dy < -20.0f) || (dy > 20.0f)) {
        return 0;
    }
    
    //Return 0 if the sidekick's not within lateral range
    dx = sidekick->srt.transl.x - self->srt.transl.x;
    dz = sidekick->srt.transl.z - self->srt.transl.z;
    if ((SQ(dx) + SQ(dz)) > 2500.0f) {
        return 0;
    }
    
    //Advance digging
    objData->digDepth += 60; //@framerate-dependent?

    //Return a distance for the sidekick, based on the dig progress
    return ((f32)objData->digDepth / DIG_DEPTH_MAX);
}

// offset: 0xC20 | func: 9 | export: 8
u8 WallAnimator_is_dig_finished(Object* self) {
    WallAnimator_Data* objData = self->data;
    return (objData->digDepth >= DIG_DEPTH_MAX);
}

// offset: 0xC3C | func: 10 | export: 9
u8 WallAnimator_get_energy_cost(Object* self) {
    WallAnimator_Data* objData = self->data;
    return objData->sidekickCost;
}
