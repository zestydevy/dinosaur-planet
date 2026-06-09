#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "sys/gfx/modgfx.h"
#include "sys/objtype.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/272_collectable.h"

typedef struct {
    ObjSetup base;
    s16 gamebitDug;         //Gamebit to set when Tricky digs up the spot
    s16 unused1A;               //TODO: Not used by the GroundAnimator, but may store experience/energy cost value for Tricky
    s16 unused1C;               //TODO: Not used by the GroundAnimator, but may store experience/energy cost value for Tricky
    s16 magicCaveID;        //Values 3 onwards cause the dig spot to become be a Magic Cave entrance
    u8 digDepthMax;         //Dig distance (stored at 100x scale)
    u8 soundIndex;          //Index of the sound to play when digging has finished (0: small secret, 1: big secret)
    u8 unused22;                //TODO: Not used by the GroundAnimator, but may store experience/energy cost value for Tricky
    u8 findCommandRadius;   //Range for Find command to show up in inventory
    u8 unused24;                //TODO: Not used by the GroundAnimator, but may store experience/energy cost value for Tricky
    u8 animatorID;          //Block shapes with this tag will be animated
    u8 falloffRadius;       //Vertex influence tapers off with this radius, and it also decides how far Tricky scoots backwards while digging
    u8 collectableDepth;    //How far down the collectable is buried beneath the dig spot
} GroundAnimator_Setup;

typedef struct {
    f32* vtxWeights;        //Displacement strengths (from 0 to 1) for each vertex being animated (influence falls off from centre)
    Object* collectable;
    s32 digDepth;           //Current dig progress (starts at 0, increases while digging)
    f32 falloffRadius;      //Vertex influence tapers off with this radius, and it also decides how far Tricky scoots backwards while digging
    f32 collectableDepth;   //Affects how far down the collectable is buried
    s16 animatedShapeIDs[5];
    s16 misconfiguredShapeID;
    s16 animatedVtxCount;   //The total number of vertices being animated
    u8 animatedShapesCount; //Total shapes animated (i.e. number of items in `animatedShapeIDs`)
    s8 previousDigDepth;    //Previous dig progress value (vertex updates are queued when this differs from current dig value)
    u8 magicCaveID;         //Seems intended as an index for a specific Magic Cave instance (can be queried with `get_magic_cave_index` export)
    u8 animUpdates;         //Used to queue vertex animation updates
    u8 flags;               //Various state flags: Block search, dig finished, Magic Cave entrance/glow
} GroundAnimator_Data;

typedef enum {
    GroundAnimator_FLAG_0_None = 0,
    GroundAnimator_FLAG_1_Block_Found = 1,
    GroundAnimator_FLAG_2_Dig_Finished = 2,
    GroundAnimator_FLAG_4_Unused = 4,
    GroundAnimator_FLAG_8_Magic_Cave_Entrance = 8,
    GroundAnimator_FLAG_10_Glow_Created = 0x10,
    GroundAnimator_FLAG_20_Glow_Required = 0x20
} GroundAnimator_Flags;

/* Sounds that can play when Tricky finishes digging */
/*0x0*/ static u16 dDigJingles[2] = {
    SOUND_B01_Success_Chime,    //small secret uncovered
    SOUND_798_Puzzle_Solved     //big secret uncovered
};

#define OPACITY_MAX 0xFF
#define TOTAL_JINGLES 2

static void GroundAnimator_store_shapeIDs_and_vertex_weights(Object* self, GroundAnimator_Data* objData, GroundAnimator_Setup* objSetup);

// offset: 0x0 | ctor
void GroundAnimator_ctor(void *dll) { }

// offset: 0xC | dtor
void GroundAnimator_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void GroundAnimator_setup(Object* self, GroundAnimator_Setup* objSetup, s32 reset) {
    GroundAnimator_Data* objData = self->data;
    
    //Check if the dig spot is a Magic Cave entrance
    objData->magicCaveID = objSetup->magicCaveID;
    if (objData->magicCaveID >= 3) { //Considered a cave entrance from 3 onwards
        objData->flags |= GroundAnimator_FLAG_8_Magic_Cave_Entrance;
        objData->magicCaveID -= 3; //Subtracting the condition threshold to get a zero-indexed instance ID for the Magic Cave, maybe?
    }
    
    objData->collectableDepth = objSetup->collectableDepth;
    objData->previousDigDepth = -1;
    objData->falloffRadius = objSetup->falloffRadius;
    
    if (objSetup->animatorID) {
        //Check if spot's already been dug up
        if (main_get_bits(objSetup->gamebitDug)) {
            objData->digDepth = objSetup->digDepthMax * 100;
            objData->flags |= GroundAnimator_FLAG_2_Dig_Finished;
            if (objData->flags & GroundAnimator_FLAG_8_Magic_Cave_Entrance) {
                objData->flags |= GroundAnimator_FLAG_20_Glow_Required;
            }
        }

        obj_add_object_type(self, OBJTYPE_TrickyTarget);

        //Ensure the sound index is in bounds
        if (objSetup->soundIndex >= TOTAL_JINGLES) {
            objSetup->soundIndex = 0;
        }
    }
}

// offset: 0x170 | func: 1 | export: 1
void GroundAnimator_control(Object* self) {
    GroundAnimator_Data* objData;
    Block* block;
    GroundAnimator_Setup* objSetup;
    s32 shapeID;
    Object* collectable;
    BlockShape* shapes;
    Object* player;
    Object* sidekick;
    f32 displacement;
    s16 endVtxID;
    s32 vertexIdx;
    f32 distance;
    s32 shapeIndex;
    s32 animVtxIdx;
    u16 previousState;
    u8 fadeOut;
    s8 blockIndex;
    DLL_IModgfx* modGfxDLL;
    BlockVertex* vertices;

    distance = 100.0f;
    objData = self->data;
    objSetup = (GroundAnimator_Setup*)self->setup;
    
    //Handle Magic Cave entrances
    if (objData->flags & GroundAnimator_FLAG_8_Magic_Cave_Entrance) {
        //Create a modGfx glow around the dig spot
        if ((objData->flags & GroundAnimator_FLAG_20_Glow_Required) && !(objData->flags & GroundAnimator_FLAG_10_Glow_Created)) {
            modGfxDLL = dll_load_deferred(DLL_ID_184, 1);
            modGfxDLL->vtbl->func0(self, 0, 0, 0, -1, 0);
            modGfxDLL->vtbl->func0(self, 0, 0, 0, -1, &shapeID);
            dll_unload(modGfxDLL);
            
            objData->flags |= GroundAnimator_FLAG_10_Glow_Created;
        }
        
        //Warp the player to the Magic Cave when they approach
        if ((objData->flags & GroundAnimator_FLAG_2_Dig_Finished) && 
            (vec3_distance_xz(&self->globalPosition, &get_player()->globalPosition) < 10.0f)
        ) {
            gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        }
    }
    
    //Return early if the dig spot doesn't animate any shapes
    if (objSetup->animatorID == 0) {
        return;
    }
        
    //Get the GroundAnimator's local Block index, and queue vertex animation updates when Block is found
    {
        blockIndex = map_world_coords_to_block_index(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);

        previousState = objData->flags & GroundAnimator_FLAG_1_Block_Found;    
        if (blockIndex >= 0) {
            objData->flags |= GroundAnimator_FLAG_1_Block_Found;
        } else {
            objData->flags &= ~GroundAnimator_FLAG_1_Block_Found;
        }

        if ((objData->flags & GroundAnimator_FLAG_1_Block_Found) != previousState) {
            objData->animUpdates = 2;
        }
    }
    
    //Return if a Block couldn't be found
    if (!(objData->flags & GroundAnimator_FLAG_1_Block_Found)) {
        return;
    }
        
    //Set up animation: get shapeIDs and calculate vertex influence weights
    if ((objData->flags & GroundAnimator_FLAG_1_Block_Found) && (objData->vtxWeights == NULL)) {
        objData->animatedVtxCount = block_get_animator_vertex_count(self, objSetup->animatorID);
        if (objData->animatedVtxCount > 0) {
            objData->vtxWeights = mmAlloc(objData->animatedVtxCount * sizeof(f32), ALLOC_TAG_TRACK_COL, NULL);
            GroundAnimator_store_shapeIDs_and_vertex_weights(self, objData, objSetup);
        }
    }
    
    //Return if no animatable vertices were found
    if (!objData->animatedVtxCount) {
        return;
    }
        
    //Update the dig spot's nearby collectable, if it has one
    if (objData->collectable == NULL) {
        objData->collectable = obj_get_nearest_type_to(OBJTYPE_Collectable, self, &distance);
        collectable = objData->collectable;
        if (collectable != NULL) {
            //Pause the collectable if digging hasn't finished yet
            if ((objData->flags & GroundAnimator_FLAG_2_Dig_Finished) == FALSE) {
                ((DLL_272_Collectable*)collectable->dll)->vtbl->set_pause_state(collectable, 1);
            }
            ((DLL_272_Collectable*)collectable->dll)->vtbl->save_position(collectable, 
                self->srt.transl.x, self->srt.transl.y - objData->collectableDepth, self->srt.transl.z
            );
        }
    } else if (objData->collectable->stateFlags & OBJSTATE_DESTROYED) {
        objData->collectable = NULL;
    }
    
    //Get Block by index
    block = map_get_block_by_index(blockIndex);
    if ((block == NULL) || !(block->vtxFlags & 8)) {
        return;
    }
        
    //Update the vertices' animation as the digging progresses
    if (objData->digDepth) {
        //Tick animation for 2 frames (including the current one) whenever the dig depth changes
        if (objData->digDepth != objData->previousDigDepth) {
            objData->animUpdates = 2;
            objData->previousDigDepth = objData->digDepth;
        }

        //Update vertices
        if (objData->animUpdates) {
            objData->animUpdates--;
            
            //Finish digging
            if (objData->digDepth > (objSetup->digDepthMax * 100)) {
                objData->digDepth = objSetup->digDepthMax * 100;
                
                //Unpause the collectable
                if ((objData->collectable != NULL) && (objData->collectable->data != NULL)) {
                    ((DLL_272_Collectable*)objData->collectable->dll)->vtbl->set_pause_state(objData->collectable, 0);
                }
                
                main_set_bits(objSetup->gamebitDug, TRUE);
                objData->flags |= (GroundAnimator_FLAG_20_Glow_Required | GroundAnimator_FLAG_2_Dig_Finished);

                //Play success sound
                gDLL_6_AMSFX->vtbl->play(self, dDigJingles[objSetup->soundIndex], MAX_VOLUME, NULL, NULL, 0, NULL);
            }

            vertices = (BlockVertex*)block->vertices2[block->vtxFlags & 1];            
            
            for (shapeIndex = 0, animVtxIdx = 0, shapes = block->shapes; shapeIndex < objData->animatedShapesCount; shapeIndex++) {
                shapeID = objData->animatedShapeIDs[shapeIndex];
                
                for (vertexIdx = shapes[shapeID].vtxBase; vertexIdx < shapes[shapeID + 1].vtxBase; vertexIdx++, animVtxIdx++) {
                    if (objData->vtxWeights[animVtxIdx] > 0.0f) {
                        displacement = objData->vtxWeights[animVtxIdx] * (objData->digDepth / 100);

                        //Move vertices down
                        if (!(shapes[shapeID].flags & RENDER_UNK20000000)) {
                            vertices[vertexIdx].ob[1] = block->vertices[vertexIdx].ob[1] - displacement;
                        } else {
                            vertices[vertexIdx].ob[1] = ((block->vertices[vertexIdx].ob[1] - block->minY) - displacement) * 20.0f;
                        }

                        //Fade out displaced vertices (reveals ground beneath soil/snow)
                        if (displacement < 0.0f) {
                            fadeOut = 0;
                        } else if (displacement > 5.0f) {
                            fadeOut = OPACITY_MAX;
                        } else {
                            displacement /= 5.0f;
                            if (displacement < 0.0f) {
                                displacement = 0.0f;
                            } else if (displacement > 1.0f) {
                                displacement = 1.0f;
                            }
                            fadeOut = (displacement * OPACITY_MAX);
                        }
                        vertices[vertexIdx].cn[3] = OPACITY_MAX - fadeOut;                        
                    }
                }
            }
        }        
    }
    
    //Show the Find command when the player is nearby
    if (!(objData->flags & GroundAnimator_FLAG_2_Dig_Finished)) {
        player = get_player();
        sidekick = get_sidekick();
        if (sidekick != NULL) {
            distance = vec3_distance_squared(&self->globalPosition, &player->globalPosition);
            if (distance <= SQ(objSetup->findCommandRadius)) {
                ((DLL_ISidekick*)sidekick->dll)->vtbl->func14(sidekick, Sidekick_Command_INDEX_1_Find);
            }
        }
    }
}

// offset: 0x950 | func: 2 | export: 2
void GroundAnimator_update(Object *self) { }

// offset: 0x95C | func: 3 | export: 3
void GroundAnimator_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x974 | func: 4 | export: 4
void GroundAnimator_free(Object* self, s32 onlySelf) {
    GroundAnimator_Data* objData = self->data;
    
    if (objData->vtxWeights != NULL) {
        mmFree(objData->vtxWeights);
    }
    
    obj_free_object_type(self, OBJTYPE_TrickyTarget);
    gDLL_14_Modgfx->vtbl->func4(self);
}

// offset: 0xA04 | func: 5 | export: 5
u32 GroundAnimator_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xA14 | func: 6 | export: 6
u32 GroundAnimator_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(GroundAnimator_Data);
}

// offset: 0xA28 | func: 7
void GroundAnimator_store_shapeIDs_and_vertex_weights(Object* self, GroundAnimator_Data* objData, GroundAnimator_Setup* objSetup) {
    BlockShape* shapes;
    s32 pad[6];
    f32 radiusSq;
    f32 tValue;
    s32 blockWorldGridX;
    s32 blockWorldGridZ;
    Block* block;
    f32 digBlockX;
    f32 digBlockZ;
    s32 shapeIdx;
    s32 animVtxIdx;
    f32 dx;
    f32 dz;
    s32 vtxID;
    
    block = map_get_block_by_index(map_world_coords_to_block_index(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    
    if ((block == NULL) || !(block->vtxFlags & 8)) {
        return;
    }
        
    //Get the GroundAnimator's position relative to the Blocks model's local origin
    blockWorldGridX = floor_f((self->srt.transl.x - gWorldX) / BLOCKS_GRID_UNIT_F);
    blockWorldGridZ = floor_f((self->srt.transl.z - gWorldZ) / BLOCKS_GRID_UNIT_F);

    digBlockX = self->srt.transl.x - (blockWorldGridX * BLOCKS_GRID_UNIT_F + gWorldX);
    digBlockZ = self->srt.transl.z - (blockWorldGridZ * BLOCKS_GRID_UNIT_F + gWorldZ);
    
    //Search through the Block's shapes, looking for any tagged with the animatorID
    objData->animatedShapesCount = 0;
    shapes = block->shapes;
    for (shapeIdx = 0, animVtxIdx = 0; shapeIdx < block->shapeCount; shapeIdx++) {
        //Only animate shapes that have a matching animatorID
        if (objSetup->animatorID == shapes[shapeIdx].animatorID) {

            //Calculate vertices' influence weights, based on GroundAnimator's distance and falloff
            for (vtxID = shapes[shapeIdx].vtxBase; vtxID < shapes[shapeIdx + 1].vtxBase; vtxID++, animVtxIdx++) {
                dx = block->vertices[vtxID].ob[0];
                dx -= digBlockX;
                dz = block->vertices[vtxID].ob[2];
                dz -= digBlockZ;
                
                radiusSq = SQ(objData->falloffRadius);
                tValue = (SQ(dx) + SQ(dz)) / radiusSq;
                if (tValue > 1.0f) {
                    tValue = 1.0f;
                }

                tValue = SQ(tValue);
                tValue = 1.0f - tValue;
                objData->vtxWeights[animVtxIdx] = tValue;
            }
            
            //Store last tagged shapeID that doesn't have RENDER_SUBSURFACE enabled (doesn't do anything with this)
            if ((shapes[shapeIdx].flags & RENDER_SUBSURFACE) == FALSE) {
                objData->misconfiguredShapeID = shapeIdx;
            }
            
            //Store ID of animated shape
            objData->animatedShapeIDs[objData->animatedShapesCount++] = shapeIdx;

            //Warn if too many shapes are being animated
            if (objData->animatedShapesCount >= ARRAYCOUNT(objData->animatedShapeIDs)) {
                STUBBED_PRINTF("groundanim group overflow\n");
            }
        }
    }
}

// offset: 0xC90 | func: 8 | export: 7
f32 GroundAnimator_tick_dig(Object* self, Object* sidekick) {
    GroundAnimator_Setup* objSetup;
    GroundAnimator_Data* objData;
    Object* collectable;
    f32 range;
    f32 dx;
    f32 dy;
    f32 dz;

    objData = self->data;
    objSetup = (GroundAnimator_Setup*)self->setup;

    //Return 0 if sidekick's too far above/below the dig spot
    dy = sidekick->srt.transl.y - self->srt.transl.y;
    if ((dy < -20.0f) || (dy > 20.0f)) {
        return 0;
    }
    
    //Return -1 if the sidekick's not within lateral range
    dx = sidekick->srt.transl.x - self->srt.transl.x; 
    dz = sidekick->srt.transl.z - self->srt.transl.z;
    range = objData->falloffRadius + 10.0f;
    range = SQ(range);
    if (range < SQ(dx) + SQ(dz)) {
        return -1;
    }
    
    //Unpause the collectable when at zero dig depth
    if (objData->digDepth == 0) {
        collectable = objData->collectable;
        if (collectable != NULL) {
            ((DLL_272_Collectable*)collectable->dll)->vtbl->set_pause_state(collectable, 0);
        }
    }
    
    //Advance digging
    objData->digDepth += 35; //@framerate-dependent?

    //Return a distance for the sidekick, based on the dig progress factor and dig falloff radius
    return (objData->digDepth / (objSetup->digDepthMax * 100.0f)) * objData->falloffRadius;
}

// offset: 0xDFC | func: 9 | export: 8
u8 GroundAnimator_is_dig_finished(Object* self) {
    GroundAnimator_Data* objData = self->data;
    GroundAnimator_Setup* objSetup = (GroundAnimator_Setup*)self->setup;
    
    return (objData->digDepth >= (objSetup->digDepthMax * 100));
}

// offset: 0xE38 | func: 10 | export: 9
/* 
 * Seems intended to return the index of the particular Magic Cave the dig spot should warp the player to!
 *
 * Presumably the warp cutscene would've warped you to different setups of the Magic Cave based on this (like in SFA).
 */
u8 GroundAnimator_get_magic_cave_index(Object* self) {
    GroundAnimator_Data* objData = self->data;
    return objData->magicCaveID;
}

/*0x1C*/ static const char str_1C[] = "";
