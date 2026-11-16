#include "game/gamebits.h"
#include "game/objects/setups/81_XYZAnimator.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/memory.h"
#include "sys/objprint.h"

typedef struct {
    s32 animShapeCount;     //The number of animated shapes
    s32 animVtxCount;       //The number of animated vertices
    f32 radius;             //For `XYZAnimator_MODE_3_Radial_Falloff_Set_Gamebit`
    f32 innerRadius;        //For `XYZAnimator_MODE_3_Radial_Falloff_Set_Gamebit`
    f32 goalRadius;         //For `XYZAnimator_MODE_3_Radial_Falloff_Set_Gamebit`
    f32* distances;         //Lateral distance from the XYZAnimator to each vertex it animates (used for `XYZAnimator_MODE_3_Radial_Falloff_Set_Gamebit`)
    s16* vtxCoords;         //Block-local position of each animated vertex
    s16* bbYmin;            //Updated bounds for each animated shape
    s16* bbYmax;            //Updated bounds for each animated shape
    s16* bbXmin;            //Updated bounds for each animated shape
    s16* bbXmax;            //Updated bounds for each animated shape
    s16* bbZmin;            //Updated bounds for each animated shape
    s16* bbZmax;            //Updated bounds for each animated shape
    Vec3f displacement;     //Position delta
    s8 animatorID;          //The animator tag of the shapes to animate
    s8 goal;                //`XYZAnimator_Goals`
    s8 goalTimer;           //How many ticks the animator has spent at its goal
    s8 prevGoal;            //`XYZAnimator_Goals`
} XYZAnimator_Data;

typedef enum {
    XYZAnimator_MODE_0_Oneshot_and_Set_Gamebit,     //Interpolate to the goal, and set gamebitFinished
    XYZAnimator_MODE_1_Oneshot,                     //Interpolate to the goal
    XYZAnimator_MODE_2_Reversible_Set_Gamebit,      //Interpolate to/from the goal, and set gamebitFinished
    XYZAnimator_MODE_3_Radial_Falloff_Set_Gamebit,  //Animate vertices within an expanding radius, and set gamebitFinished
    XYZAnimator_MODE_4_Oneshot_and_Set_Gamebit_No_Initial_Update //Interpolate to the goal, and set gamebitFinished (without updating the vertices during initialisation)
} XYZAnimator_Modes;

typedef enum {
    XYZAnimator_POSITION_Initial,
    XYZAnimator_POSITION_Goal
} XYZAnimator_Goals;

static void XYZAnimator_updateShapesAndVertices(XYZAnimator_Data* objData, XYZAnimator_Setup* objSetup, Block* block);
static void XYZAnimator_calculateAnimatedVertexDistances(Object* self, XYZAnimator_Data* objData);
static void XYZAnimator_getInitialValues(Object* self, XYZAnimator_Data* objData);

// offset: 0x0 | ctor
void XYZAnimator_ctor(void* dll) { }

// offset: 0xC | dtor
void XYZAnimator_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void XYZAnimator_obj_Setup(Object* self, XYZAnimator_Setup* objSetup, s32 reset) {
    XYZAnimator_Data* objData = self->data;
    
    objData->prevGoal = -1;
}

// offset: 0x34 | func: 1 | export: 1
void XYZAnimator_obj_Control(Object* self) {
    XYZAnimator_Setup* objSetup;
    XYZAnimator_Data* objData;
    Block* block;
    s32 delta;
    s32 numAtGoal;
    
    objSetup = (XYZAnimator_Setup*)self->setup;
    objData = self->data;
    
    block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if (block == NULL) {
        objData->goalTimer = 0;
        return;
    }
    
    if ((block->vtxFlags & 8) == FALSE) {
        return;
    }
    
    //Initialise XYZAnimator
    if (objData->animVtxCount == 0) {
        objData->animatorID = objSetup->animatorID;
        objData->animVtxCount = blockGetAnimatorVertexCount(self, objData->animatorID);
        objData->animShapeCount = blockGetAnimatorShapeCount(self, objData->animatorID);
        if (objData->animVtxCount == 0) {
            objData->animatorID = 0;
        }
        
        if (objData->animatorID == 0) {
            return;
        }
        
        objData->radius = 0.0f;
        objData->innerRadius = 0.0f;
        objData->goalRadius = objSetup->goalRadius;
        
        //Check if the animation should start
        if (objSetup->gamebitActivate == NO_GAMEBIT) {
            objData->goal = XYZAnimator_POSITION_Goal;
        } else {
            objData->goal = mainGetBits(objSetup->gamebitActivate);
        }
        
        objData->displacement.x = objSetup->initDisplacementX;
        objData->displacement.y = objSetup->initDisplacementY;
        objData->displacement.z = objSetup->initDisplacementZ;
        
        //Check if the animation should already be finished
        if (objSetup->gamebitFinished != NO_GAMEBIT && mainGetBits(objSetup->gamebitFinished)) {
            objData->displacement.x = objSetup->endDisplacementX;
            objData->displacement.y = objSetup->endDisplacementY;
            objData->displacement.z = objSetup->endDisplacementZ;
            objData->goal = XYZAnimator_POSITION_Goal;
            objData->radius = objData->goalRadius + 1;
        }
        
        //Allocate and set up the initial values
        {
            if (objSetup->mode == XYZAnimator_MODE_3_Radial_Falloff_Set_Gamebit) {
                objData->distances = mmAlloc(objData->animVtxCount * sizeof(f32), ALLOC_TAG_TRACK_COL, NULL);
                XYZAnimator_calculateAnimatedVertexDistances(self, objData);
            }
            
            objData->vtxCoords = mmAlloc((objData->animVtxCount * (sizeof(s16))) * 3, ALLOC_TAG_TRACK_COL, NULL);
            objData->bbYmin = mmAlloc(objData->animShapeCount * (sizeof(s16)), ALLOC_TAG_TRACK_COL, NULL);
            objData->bbYmax = mmAlloc(objData->animShapeCount * (sizeof(s16)), ALLOC_TAG_TRACK_COL, NULL);
            objData->bbXmin = mmAlloc(objData->animShapeCount * (sizeof(s16)), ALLOC_TAG_TRACK_COL, NULL);
            objData->bbXmax = mmAlloc(objData->animShapeCount * (sizeof(s16)), ALLOC_TAG_TRACK_COL, NULL);
            objData->bbZmin = mmAlloc(objData->animShapeCount * (sizeof(s16)), ALLOC_TAG_TRACK_COL, NULL);
            objData->bbZmax = mmAlloc(objData->animShapeCount * (sizeof(s16)), ALLOC_TAG_TRACK_COL, NULL);
            XYZAnimator_getInitialValues(self, objData);
        }

        //Update the vertices and vertex buffers
        if (objSetup->mode != XYZAnimator_MODE_4_Oneshot_and_Set_Gamebit_No_Initial_Update) {
            XYZAnimator_updateShapesAndVertices(objData, objSetup, block);
            block->vtxFlags ^= 1;
            XYZAnimator_updateShapesAndVertices(objData, objSetup, block);
            block->vtxFlags ^= 1;
        }
    }
    
    if (objData->animatorID == 0) {
        return;
    }
    
    //Check if the animation should start/reverse
    if (objSetup->mode == XYZAnimator_MODE_2_Reversible_Set_Gamebit) {
        objData->goal = mainGetBits(objSetup->gamebitActivate);
        if (objData->goalTimer >= 3) {
            if (objData->goal != objData->prevGoal) {
                objData->goalTimer = 0;
                objData->prevGoal = objData->goal;
            }
        }
        
        if (objData->goalTimer >= 3) {
            return;
        }
    } else if (objData->goalTimer >= 3) {
        return;
    } else if (objData->goal == XYZAnimator_POSITION_Initial) {
        objData->goal = mainGetBits(objSetup->gamebitActivate);
        if (objData->goal == XYZAnimator_POSITION_Initial) {
            return;
        }
    }
    
    switch (objSetup->mode) {
    case XYZAnimator_MODE_0_Oneshot_and_Set_Gamebit:
    case XYZAnimator_MODE_4_Oneshot_and_Set_Gamebit_No_Initial_Update:
        //Animate towards the goal position
        numAtGoal = 0;
        if (objSetup->endDisplacementX < objSetup->initDisplacementX) {
            objData->displacement.x -= (objSetup->speedX * gUpdateRate) * 0.1f;
            if (objData->displacement.x <= objSetup->endDisplacementX) {
                objData->displacement.x = objSetup->endDisplacementX;
                numAtGoal++;
            }
        } else {
            objData->displacement.x += (objSetup->speedX * gUpdateRate) * 0.1f;
            if (objData->displacement.x >= objSetup->endDisplacementX) {
                objData->displacement.x = objSetup->endDisplacementX;
                numAtGoal++;
            }
        }
        
        if (objSetup->endDisplacementY < objSetup->initDisplacementY) {
            objData->displacement.y -= (objSetup->speedY * gUpdateRate) * 0.1f;
            if (objData->displacement.y <= objSetup->endDisplacementY) {
                objData->displacement.y = objSetup->endDisplacementY;
                numAtGoal++;
            }
        } else {
            objData->displacement.y += (objSetup->speedY * gUpdateRate) * 0.1f;
            if (objData->displacement.y >= objSetup->endDisplacementY) {
                objData->displacement.y = objSetup->endDisplacementY;
                numAtGoal++;
            }
        }
        
        if (objSetup->endDisplacementZ < objSetup->initDisplacementZ) {
            objData->displacement.z -= (objSetup->speedZ * gUpdateRate) * 0.1f;
            if (objData->displacement.z <= objSetup->endDisplacementZ) {
                objData->displacement.z = objSetup->endDisplacementZ;
                numAtGoal++;
            }
        } else {
            objData->displacement.z += (objSetup->speedZ * gUpdateRate) * 0.1f;
            if (objData->displacement.z >= objSetup->endDisplacementZ) {
                objData->displacement.z = objSetup->endDisplacementZ;
                numAtGoal++;
            }
        }
        
        //Check if all position components have reached their goal
        if (numAtGoal == 3) {
            if (objSetup->gamebitFinished != NO_GAMEBIT) {
                mainSetBits(objSetup->gamebitFinished, TRUE);
            }
            objData->goalTimer++;
        }
        break;
    case XYZAnimator_MODE_1_Oneshot:
        //Animate towards the goal position
        if (objSetup->endDisplacementX < objSetup->initDisplacementX) {
            objData->displacement.x -= (objSetup->speedX * gUpdateRate) * 0.1f;
            if (objData->displacement.x < objSetup->endDisplacementX) {
                delta = objSetup->endDisplacementX - objData->displacement.x;
                objData->displacement.x = objSetup->initDisplacementX - delta;
            }
        } else {
            objData->displacement.x += (objSetup->speedX * gUpdateRate) * 0.1f;
            if (objSetup->initDisplacementX < objData->displacement.x) {
                delta = objData->displacement.x - objSetup->endDisplacementX;
                objData->displacement.x = objSetup->endDisplacementX + delta;
            }
        }
        
        if (objSetup->endDisplacementY < objSetup->initDisplacementY) {
            objData->displacement.y -= (objSetup->speedY * gUpdateRate) * 0.1f;
            if (objData->displacement.y < objSetup->endDisplacementY) {
                delta = objSetup->endDisplacementY - objData->displacement.y;
                objData->displacement.y = objSetup->initDisplacementY - (delta * 0.1f);
            }
        } else {
            objData->displacement.y += (objSetup->speedY * gUpdateRate) * 0.1f;
            if (objSetup->initDisplacementY < objData->displacement.y) {
                delta = objData->displacement.y - objSetup->endDisplacementY;
                objData->displacement.y = objSetup->endDisplacementY + delta;
            }
        }
        
        if (objSetup->endDisplacementZ < objSetup->initDisplacementZ) {
            objData->displacement.z -= (objSetup->speedZ * gUpdateRate) * 0.1f;
            if (objData->displacement.z < objSetup->endDisplacementZ) {
                delta = objSetup->endDisplacementZ - objData->displacement.z;
                objData->displacement.z = objSetup->initDisplacementZ - delta;
            }
        } else {
            objData->displacement.z += (objSetup->speedZ * gUpdateRate) * 0.1f;
            if (objSetup->initDisplacementZ < objData->displacement.z) {
                delta = objData->displacement.z - objSetup->endDisplacementZ;
                if (((!objSetup->endDisplacementZ) && (!objSetup->endDisplacementZ)) && (!objSetup->endDisplacementZ)) { } //fake
                objData->displacement.z = objSetup->endDisplacementZ + delta;
            }
        }
        break;
    case XYZAnimator_MODE_2_Reversible_Set_Gamebit:
        numAtGoal = 0;

        if (objData->goal != XYZAnimator_POSITION_Initial) {
            //Animate towards the goal position
            if (objSetup->endDisplacementX < objSetup->initDisplacementX) {
                objData->displacement.x -= (objSetup->speedX * gUpdateRate) * 0.1f;
                if (objData->displacement.x <= objSetup->endDisplacementX) {
                    objData->displacement.x = objSetup->endDisplacementX;
                    numAtGoal++;
                }
            } else {
                objData->displacement.x += (objSetup->speedX * gUpdateRate) * 0.1f;
                if (objData->displacement.x >= objSetup->endDisplacementX) {
                    objData->displacement.x = objSetup->endDisplacementX;
                    numAtGoal++;
                }
            }
            
            if (objSetup->endDisplacementY < objSetup->initDisplacementY) {
                objData->displacement.y -= (objSetup->speedY * gUpdateRate) * 0.1f;
                if (objData->displacement.y <= objSetup->endDisplacementY) {
                    objData->displacement.y = objSetup->endDisplacementY;
                    numAtGoal++;
                }
            } else {
                objData->displacement.y += (objSetup->speedY * gUpdateRate) * 0.1f;
                if (objData->displacement.y >= objSetup->endDisplacementY) {
                    objData->displacement.y = objSetup->endDisplacementY;
                    numAtGoal++;
                }
            }
            
            if (objSetup->endDisplacementZ < objSetup->initDisplacementZ) {
                objData->displacement.z -= (objSetup->speedZ * gUpdateRate) * 0.1f;
                if (objData->displacement.z <= objSetup->endDisplacementZ) {
                    objData->displacement.z = objSetup->endDisplacementZ;
                    numAtGoal++;
                }
            } else {
                objData->displacement.z += (objSetup->speedZ * gUpdateRate) * 0.1f;
                if (objData->displacement.z >= objSetup->endDisplacementZ) {
                    objData->displacement.z = objSetup->endDisplacementZ;
                    numAtGoal++;
                }
            }
            
            //Check if all position components have reached their goal
            if (numAtGoal == 3) {
                if (objSetup->gamebitFinished != NO_GAMEBIT) {
                    mainSetBits(objSetup->gamebitFinished, TRUE);
                }
                objData->goalTimer++; 
            } 
        } else {
            //Animate towards the initial position
            if (objSetup->endDisplacementX < objSetup->initDisplacementX) { 
                objData->displacement.x += (objSetup->speedX * gUpdateRate) * 0.1f; 
                if (objData->displacement.x >= objSetup->initDisplacementX) { 
                    objData->displacement.x = objSetup->initDisplacementX; 
                    numAtGoal++; 
                } 
            } else {
                objData->displacement.x -= (objSetup->speedX * gUpdateRate) * 0.1f;
                if (objData->displacement.x <= objSetup->initDisplacementX) {
                    objData->displacement.x = objSetup->initDisplacementX;
                    numAtGoal++;
                }
            }
            
            if (objSetup->endDisplacementY < objSetup->initDisplacementY) {
                objData->displacement.y += (objSetup->speedY * gUpdateRate) * 0.1f;
                if (objData->displacement.y >= objSetup->initDisplacementY) {
                    objData->displacement.y = objSetup->initDisplacementY;
                    numAtGoal++;
                }
            } else {
                objData->displacement.y -= (objSetup->speedY * gUpdateRate) * 0.1f;
                if (objData->displacement.y <= objSetup->initDisplacementY) {
                    objData->displacement.y = objSetup->initDisplacementY;
                    numAtGoal++;
                }
            }
            
            if (objSetup->endDisplacementZ < objSetup->initDisplacementZ) {
                objData->displacement.z += (objSetup->speedZ * gUpdateRate) * 0.1f;
                if (objData->displacement.z >= objSetup->initDisplacementZ) {
                    objData->displacement.z = objSetup->initDisplacementZ;
                    numAtGoal++;
                }
            } else {
                objData->displacement.z -= (objSetup->speedZ * gUpdateRate) * 0.1f;
                if (objData->displacement.z <= objSetup->initDisplacementZ) {
                    objData->displacement.z = objSetup->initDisplacementZ;
                    numAtGoal++;
                }
            }
            
            //Check if all position components have reached their goal
            if (numAtGoal == 3) {
                if (objSetup->gamebitFinished != NO_GAMEBIT) {
                    mainSetBits(objSetup->gamebitFinished, TRUE);
                }
                objData->goalTimer++;
            }
        }
        break;
    case XYZAnimator_MODE_3_Radial_Falloff_Set_Gamebit:
        //Expand the radius towards its goal
        objData->radius += (objSetup->speedX * gUpdateRateF) * 0.1f;
        if (objData->radius > objData->goalRadius) {
            objData->radius = objData->goalRadius;
            mainSetBits(objSetup->gamebitFinished, TRUE);
            objData->goalTimer++;
        }
        objData->innerRadius = objData->radius - 50.0f;
        break;
    }
    
    XYZAnimator_updateShapesAndVertices(objData, objSetup, block);
}

// offset: 0x1174 | func: 2
static void XYZAnimator_updateShapesAndVertices(XYZAnimator_Data* objData, XYZAnimator_Setup* objSetup, Block* block) {
    f32 tValue;
    s32 vtxIdx;
    s16 bounds;
    s32 animVtxIdx;
    s32 animShapeIdx;
    s32 shapeIdx;
    BlockShape* shapes;
    BlockVertex* vertices;

    vertices = (BlockVertex*)block->vertices2[(block->vtxFlags & 1)];
    
    shapeIdx = 0;
    animVtxIdx = 0;
    animShapeIdx = 0;
    shapes = block->shapes;
    for (; shapeIdx < block->shapeCount; shapeIdx++) {
        if (objData->animatorID == shapes[shapeIdx].animatorID) {
            //Update shape bounds
            {
                shapes[shapeIdx].Ymin = objData->bbYmin[animShapeIdx] + objData->displacement.y;
                
                bounds = objData->bbXmin[animShapeIdx] + objData->displacement.x;
                shapes[shapeIdx].Xmin = bounds >> 2;
                shapes[shapeIdx].boundsRemainder = bounds & 3;
                
                bounds = objData->bbZmin[animShapeIdx] + objData->displacement.z;
                shapes[shapeIdx].Zmin = bounds >> 2;
                shapes[shapeIdx].boundsRemainder |= (bounds & 3) << 2;
                
                shapes[shapeIdx].Ymax = objData->bbYmax[animShapeIdx] + objData->displacement.y;
                
                bounds = objData->bbXmax[animShapeIdx] + objData->displacement.x;
                shapes[shapeIdx].Xmax = bounds >> 2;
                shapes[shapeIdx].boundsRemainder |= (bounds & 3) << 4;
                
                bounds = objData->bbZmax[animShapeIdx] + objData->displacement.z;
                shapes[shapeIdx].Zmax = bounds >> 2;
                shapes[shapeIdx].boundsRemainder |= (bounds & 3) << 6;
            }

            animShapeIdx++;

            //Update vertices' coords
            for (vtxIdx = shapes[shapeIdx].vtxBase; vtxIdx < shapes[shapeIdx + 1].vtxBase; vtxIdx++, animVtxIdx++) {
                if (objSetup->mode != XYZAnimator_MODE_3_Radial_Falloff_Set_Gamebit) {
                    vertices[vtxIdx].ob[0] = objData->vtxCoords[(animVtxIdx * 3) + 0] + objData->displacement.x;
                    vertices[vtxIdx].ob[1] = objData->vtxCoords[(animVtxIdx * 3) + 1] + objData->displacement.y;
                    vertices[vtxIdx].ob[2] = objData->vtxCoords[(animVtxIdx * 3) + 2] + objData->displacement.z;
                    block->vertices[vtxIdx].ob[0] = vertices[vtxIdx].ob[0];
                    block->vertices[vtxIdx].ob[1] = vertices[vtxIdx].ob[1];
                    block->vertices[vtxIdx].ob[2] = vertices[vtxIdx].ob[2];
                } else {
                    //Radial Falloff mode
                    tValue = (objData->distances[animVtxIdx] - objData->innerRadius) * 0.02f;
                    if (tValue > 1.0f) {
                        tValue = 1.0f;
                    } else if (tValue < 0.0f) {
                        tValue = 0.0f;
                    }

                    vertices[vtxIdx].ob[0] = objData->vtxCoords[(animVtxIdx * 3) + 0] + objData->displacement.x + (tValue * (objSetup->endDisplacementX - objSetup->initDisplacementX));
                    vertices[vtxIdx].ob[1] = objData->vtxCoords[(animVtxIdx * 3) + 1] + objData->displacement.y + (tValue * (objSetup->endDisplacementY - objSetup->initDisplacementY));
                    vertices[vtxIdx].ob[2] = objData->vtxCoords[(animVtxIdx * 3) + 2] + objData->displacement.z + (tValue * (objSetup->endDisplacementZ - objSetup->initDisplacementZ));
                    block->vertices[vtxIdx].ob[0] = vertices[vtxIdx].ob[0];
                    block->vertices[vtxIdx].ob[1] = vertices[vtxIdx].ob[1];
                    block->vertices[vtxIdx].ob[2] = vertices[vtxIdx].ob[2];
                }
            }
        }
    }
}

// offset: 0x15DC | func: 3 | export: 2
void XYZAnimator_obj_Update(Object* self) { }

// offset: 0x15E8 | func: 4 | export: 3
void XYZAnimator_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x163C | func: 5 | export: 4
void XYZAnimator_obj_Free(Object* self, s32 onlySelf) {
    s32 pad[2];
    XYZAnimator_Setup* objSetup;
    XYZAnimator_Data* objData;
    Block* block;

    objData = self->data;
    objSetup = (XYZAnimator_Setup*)self->setup;
    
    objData->displacement.x = 0;
    objData->displacement.y = 0;
    objData->displacement.z = 0;
    
    if (onlySelf == FALSE) {
        block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
        if ((block != NULL) && (objData->animVtxCount != 0)) {
            //@bug: only updates one vertex buffer (should maybe use `block->vtxFlags ^= 1` and then update a second time)
            XYZAnimator_updateShapesAndVertices(objData, objSetup, block);
        }
    }
    
    if (objData->distances != NULL) {
        mmFree(objData->distances);
    }
    if (objData->vtxCoords != NULL) {
        mmFree(objData->vtxCoords);
    }
    if (objData->bbYmin != NULL) {
        mmFree(objData->bbYmin);
    }
    if (objData->bbYmax != NULL) {
        mmFree(objData->bbYmax);
    }
    if (objData->bbXmin != NULL) {
        mmFree(objData->bbXmin);
    }
    if (objData->bbXmax != NULL) {
        mmFree(objData->bbXmax);
    }
    if (objData->bbZmin != NULL) {
        mmFree(objData->bbZmin);
    }
    if (objData->bbZmax != NULL) {
        mmFree(objData->bbZmax);
    }
}

// offset: 0x17C8 | func: 6 | export: 5
u32 XYZAnimator_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x17D8 | func: 7 | export: 6
u32 XYZAnimator_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(XYZAnimator_Data);
}

// offset: 0x17EC | func: 8
void XYZAnimator_calculateAnimatedVertexDistances(Object* self, XYZAnimator_Data* objData) {
    Block* block;
    BlockShape* shapes;
    f32 localX;
    f32 localZ;
    f32 dx;
    f32 dz;
    s32 vtxIdx;
    s32 blockWorldGridX;
    s32 blockWorldGridZ;
    s32 animVtxIdx;
    s32 shapeIdx;

    block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if ((block == NULL) || (block->vtxFlags & 8) == FALSE) {
        return;
    }
    
    blockWorldGridX = floorf((self->srt.transl.x - gWorldX) / BLOCKS_GRID_UNIT_F);
    blockWorldGridZ = floorf((self->srt.transl.z - gWorldZ) / BLOCKS_GRID_UNIT_F);

    localX = self->srt.transl.x - ((blockWorldGridX * BLOCKS_GRID_UNIT_F) + gWorldX);
    localZ = self->srt.transl.z - ((blockWorldGridZ * BLOCKS_GRID_UNIT_F) + gWorldZ);
    
    shapes = block->shapes;

    for (shapeIdx = 0, animVtxIdx = 0; shapeIdx < block->shapeCount; shapeIdx++) {
        if (objData->animatorID == shapes[shapeIdx].animatorID) {
            for (vtxIdx = shapes[shapeIdx].vtxBase; vtxIdx < shapes[shapeIdx + 1].vtxBase; vtxIdx++) {
                dx = block->vertices[vtxIdx].ob[0];
                dx -= localX;
                dz = block->vertices[vtxIdx].ob[2];
                dz -= localZ;
                objData->distances[animVtxIdx] = sqrtf(SQ(dx) + SQ(dz));
                
                animVtxIdx++;
            }
        }
    }
}

// offset: 0x1A04 | func: 9
void XYZAnimator_getInitialValues(Object* self, XYZAnimator_Data* objData) {
    Block* block;
    BlockShape* shapes;
    s32 shapeIdx;
    s32 vtxIdx;
    s32 animVtxIdx;
    s32 animShapeIdx;

    block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if (block == NULL) {
        return;
    }
    
    if ((block->vtxFlags & 8) == FALSE) {
        return;
    }
    
    shapeIdx = 0;
    animVtxIdx = 0;
    animShapeIdx = 0;
    shapes = block->shapes;
    for (; shapeIdx < block->shapeCount; shapeIdx++) {
        if (objData->animatorID == shapes[shapeIdx].animatorID) {
            objData->bbYmin[animShapeIdx] = shapes[shapeIdx].Ymin;
            objData->bbYmax[animShapeIdx] = shapes[shapeIdx].Ymax;
            objData->bbXmin[animShapeIdx] = (shapes[shapeIdx].Xmin * 4) | (shapes[shapeIdx].boundsRemainder & 3);
            objData->bbXmax[animShapeIdx] = (shapes[shapeIdx].Xmax * 4) | ((shapes[shapeIdx].boundsRemainder >> 4) & 3);
            objData->bbZmin[animShapeIdx] = (shapes[shapeIdx].Zmin * 4) | ((shapes[shapeIdx].boundsRemainder >> 2) & 3);
            objData->bbZmax[animShapeIdx] = (shapes[shapeIdx].Zmax * 4) | ((shapes[shapeIdx].boundsRemainder >> 6) & 3);
            
            for (vtxIdx = shapes[shapeIdx].vtxBase; (vtxIdx < shapes[shapeIdx + 1].vtxBase); vtxIdx++) {
                objData->vtxCoords[(animVtxIdx * 3) + 0] = block->vertices[vtxIdx].ob[0];
                objData->vtxCoords[(animVtxIdx * 3) + 1] = block->vertices[vtxIdx].ob[1];
                objData->vtxCoords[(animVtxIdx * 3) + 2] = block->vertices[vtxIdx].ob[2];
                animVtxIdx++;
            }
            animShapeIdx++;
        }
    }
}
