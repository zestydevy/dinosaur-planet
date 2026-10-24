#include "common.h"
#include "game/gamebits.h"
#include "game/objects/setups/77_AlphaAnimator.h"

typedef struct {
    s32 animatedVertexCount;        //How many vertices are affected by the AlphaAnimator
    f32 fadeRadiusOuter;            //(For radial mode) Current radius of the radial fade (+50)
    f32 fadeRadiusInner;            //(For radial mode) Current radius of the radial fade
    f32 fadeRadiusGoal;             //(For radial mode) End radius of the fade animation
    f32* vtxDistances;              //(For radial mode) Distance between the AlphaAnimator and each vertex it animates
    s16 vtxOpacity;                 //Current vertex opacity (for non-radial modes)
    s8 animatorID;                  //Vertices will only be affected if they have this shape animatorID
    s8 fadeActivated;               //Fade animation started
    s8 fadeCompletedTicks;          //Ticks since a fade animation has completed
    s8 prevFadeActivated;           //Whether the fade was activated on the previous tick (used to tell when fade has just been activated)
} AlphaAnimator_Data;

typedef enum {
    AlphaAnimator_MODE_0_Basic_Fade_and_Set_Gamebit, //Fade using a gamebit (and optionally set a secondary gamebit when the fade is complete)
    AlphaAnimator_MODE_1_Basic_Fade,                 //Fade using a gamebit
    AlphaAnimator_MODE_2_Toggleable_Fade,            //Activate/deactivate a fade using a gamebit (and optionally set/unset a secondary gamebit when the fade is complete)
    AlphaAnimator_MODE_3_Radial_Falloff_Fade         //Using a gamebit, fade vertices inside an expanding radius (or outside, based on fadeSpeed's sign!)
} AlphaAnimator_Modes;

typedef enum {
    AlphaAnimator_FLAG_Play_Sound = 4
} AlphaAnimator_Flags;

#define GET_MODE(flags) (flags & 3)
#define SHOULD_SOUND_PLAY(flags) (flags >> 2)

#define MAX_OPACITY 255

static void AlphaAnimator_animateVertices(AlphaAnimator_Data* objData, AlphaAnimator_Setup* objSetup, Block* block);
static void AlphaAnimator_calculateAnimatedVertexDistances(Object* self, AlphaAnimator_Data* objData);

// offset: 0x0 | ctor
void AlphaAnimator_ctor(void* dll) { }

// offset: 0xC | dtor
void AlphaAnimator_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void AlphaAnimator_obj_Setup(Object* self, AlphaAnimator_Setup* objSetup, s32 reset) {
    AlphaAnimator_Data* objData = self->data;
    objData->prevFadeActivated = -1;
}

// offset: 0x34 | func: 1 | export: 1
void AlphaAnimator_obj_Control(Object* self) {
    AlphaAnimator_Data* objData;
    AlphaAnimator_Setup* objSetup;
    Block* block;
    s32 fadeSpeed;
    s32 pad;
    s32 opacityDiff;
    s32 mode;

    objSetup = (AlphaAnimator_Setup*)self->setup;
    objData = self->data;

    mode = GET_MODE(objSetup->alphaAnim.flags);

    //Get the object's local BLOCKS model
    block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.f[1], self->srt.transl.f[2]));
    if (block == NULL) {
        objData->fadeCompletedTicks = 0;
        return;
    }

    //Bail if the BLOCKS model isn't animatable
    if ((block->vtxFlags & 8) == FALSE) {
        return;
    }

    //Set up animated vertices
    if (objData->animatedVertexCount == 0) {
        objData->animatorID = objSetup->alphaAnim.animatorID;
        objData->animatedVertexCount = blockGetAnimatorVertexCount(self, objData->animatorID);

        //If no animatable vertices were found, zero out the animatorID
        if (objData->animatedVertexCount == 0) {
            objData->animatorID = 0;
        }

        if (objData->animatorID != 0) {
            objData->fadeRadiusOuter = 0.0f;
            objData->fadeRadiusInner = 0.0f;
            objData->fadeRadiusGoal = objSetup->alphaAnim.fadeRadiusGoal;

            if (objSetup->alphaAnim.gamebitActivate == NO_GAMEBIT) {
                objData->fadeActivated = TRUE;
            } else {
                objData->fadeActivated = mainGetBits(objSetup->alphaAnim.gamebitActivate);
            }

            objData->vtxOpacity = objSetup->alphaAnim.initialOpacity;

            if ((objSetup->alphaAnim.gamebitActivated != NO_GAMEBIT) && mainGetBits(objSetup->alphaAnim.gamebitActivated)) {
                objData->vtxOpacity = objSetup->alphaAnim.goalOpacity;
                objData->fadeActivated = TRUE;
                objData->fadeRadiusOuter = objData->fadeRadiusGoal + 1.0f;
            }

            if (mode == AlphaAnimator_MODE_3_Radial_Falloff_Fade) {
                objData->vtxDistances = mmAlloc(objData->animatedVertexCount * sizeof(f32), ALLOC_TAG_TRACK_COL, NULL);
                AlphaAnimator_calculateAnimatedVertexDistances(self, objData);
            }

            AlphaAnimator_animateVertices(objData, objSetup, block);
            block->vtxFlags ^= 1;
            AlphaAnimator_animateVertices(objData, objSetup, block);
            block->vtxFlags ^= 1;
        } else {
            return;
        }
    }

    //Bail if no shape animation tag is specified
    if (objData->animatorID == 0) {
        return;
    }

    if (mode == AlphaAnimator_MODE_2_Toggleable_Fade) {
        objData->fadeActivated = mainGetBits(objSetup->alphaAnim.gamebitActivate);
        if ((objData->fadeCompletedTicks >= 3) && (objData->fadeActivated != objData->prevFadeActivated)) {
            //Optionally play a sound soon after the fade is completed
            if (SHOULD_SOUND_PLAY(objSetup->alphaAnim.flags)) {
                gDLL_6_AMSFX->vtbl->Play(self, objSetup->alphaAnim.soundID, MAX_VOLUME, NULL, NULL, 0, NULL);
            }

            objData->fadeCompletedTicks = 0;
            objData->prevFadeActivated = objData->fadeActivated;
        }

        if (objData->fadeCompletedTicks >= 3) {
            return;
        }
    } else {
        if (objData->fadeCompletedTicks >= 3) {
            return;
        }

        //Don't continue to the fade State Machine until gamebitActivate is set
        if (objData->fadeActivated == FALSE) {
            objData->fadeActivated = mainGetBits(objSetup->alphaAnim.gamebitActivate);

            //Optionally play a sound when fading in
            if (objData->fadeActivated) {
                if (SHOULD_SOUND_PLAY(objSetup->alphaAnim.flags)) {
                    gDLL_6_AMSFX->vtbl->Play(self, objSetup->alphaAnim.soundID, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
            } else {
                return;
            }

            if (objSetup->alphaAnim.gamebitActivate) { }
        }
    }

    switch (mode) {
    case AlphaAnimator_MODE_0_Basic_Fade_and_Set_Gamebit:
        if (objSetup->alphaAnim.goalOpacity < objSetup->alphaAnim.initialOpacity) {
            //Fade out
            objData->vtxOpacity -= objSetup->alphaAnim.fadeSpeed * gUpdateRate;
            if (objData->vtxOpacity <= objSetup->alphaAnim.goalOpacity) {
                objData->vtxOpacity = objSetup->alphaAnim.goalOpacity;

                //Optionally set a gamebit when the goal opacity has been reached
                if (objSetup->alphaAnim.gamebitActivated != NO_GAMEBIT) {
                    mainSetBits(objSetup->alphaAnim.gamebitActivated, TRUE);
                }

                objData->fadeCompletedTicks++;
            }
        } else {
            //Fade in
            objData->vtxOpacity += objSetup->alphaAnim.fadeSpeed * gUpdateRate;
            if (objData->vtxOpacity >= objSetup->alphaAnim.goalOpacity) {
                objData->vtxOpacity = objSetup->alphaAnim.goalOpacity;

                //Optionally set a gamebit when the goal opacity has been reached
                if (objSetup->alphaAnim.gamebitActivated != NO_GAMEBIT) {
                    mainSetBits(objSetup->alphaAnim.gamebitActivated, TRUE);
                }

                objData->fadeCompletedTicks++;
            }
        }
        break;
    case AlphaAnimator_MODE_1_Basic_Fade:
        if (objSetup->alphaAnim.goalOpacity < objSetup->alphaAnim.initialOpacity) {
            //Fade out
            objData->vtxOpacity -= objSetup->alphaAnim.fadeSpeed * gUpdateRate;
            if (objData->vtxOpacity < objSetup->alphaAnim.goalOpacity) {
                opacityDiff = objSetup->alphaAnim.goalOpacity - objData->vtxOpacity;
                objData->vtxOpacity = objSetup->alphaAnim.initialOpacity - opacityDiff;
            }
        } else {
            //Fade in
            objData->vtxOpacity += objSetup->alphaAnim.fadeSpeed * gUpdateRate;
            if (objData->vtxOpacity > objSetup->alphaAnim.initialOpacity) {
                opacityDiff = objData->vtxOpacity - objSetup->alphaAnim.goalOpacity;
                objData->vtxOpacity = objSetup->alphaAnim.goalOpacity + opacityDiff;
            }
        }
        break;
    case AlphaAnimator_MODE_2_Toggleable_Fade:
        if (objData->fadeActivated) {
            if (objSetup->alphaAnim.goalOpacity < objSetup->alphaAnim.initialOpacity) {
                //Fade out
                objData->vtxOpacity -= objSetup->alphaAnim.fadeSpeed * gUpdateRate;
                if (objData->vtxOpacity <= objSetup->alphaAnim.goalOpacity) {
                    objData->vtxOpacity = objSetup->alphaAnim.goalOpacity;

                    //Optionally set a gamebit when the goal opacity has been reached
                    if (objSetup->alphaAnim.gamebitActivated != NO_GAMEBIT) {
                        mainSetBits(objSetup->alphaAnim.gamebitActivated, TRUE);
                    }

                    objData->fadeCompletedTicks++;
                }
            } else {
                //Fade in
                objData->vtxOpacity += objSetup->alphaAnim.fadeSpeed * gUpdateRate;
                if (objData->vtxOpacity >= objSetup->alphaAnim.goalOpacity) {
                    objData->vtxOpacity = objSetup->alphaAnim.goalOpacity;

                    //Optionally set a gamebit when the goal opacity has been reached
                    if (objSetup->alphaAnim.gamebitActivated != NO_GAMEBIT) {
                        mainSetBits(objSetup->alphaAnim.gamebitActivated, TRUE);
                    }

                    objData->fadeCompletedTicks++;
                }
            }
        } else if (objSetup->alphaAnim.goalOpacity < objSetup->alphaAnim.initialOpacity) {
            //Fade back in
            objData->vtxOpacity += objSetup->alphaAnim.fadeSpeed * gUpdateRate;
            if (objData->vtxOpacity >= objSetup->alphaAnim.initialOpacity) {
                objData->vtxOpacity = objSetup->alphaAnim.initialOpacity;

                //Optionally unset a gamebit when the fade has returned to its initial opacity
                if (objSetup->alphaAnim.gamebitActivated != NO_GAMEBIT) {
                    mainSetBits(objSetup->alphaAnim.gamebitActivated, FALSE);
                }

                objData->fadeCompletedTicks++;
            }
        } else {
            //Fade back out
            objData->vtxOpacity -= objSetup->alphaAnim.fadeSpeed * gUpdateRate;
            if (objData->vtxOpacity <= objSetup->alphaAnim.initialOpacity) {
                objData->vtxOpacity = objSetup->alphaAnim.initialOpacity;

                //Optionally unset a gamebit when the fade has returned to its initial opacity
                if (objSetup->alphaAnim.gamebitActivated != NO_GAMEBIT) {
                    mainSetBits(objSetup->alphaAnim.gamebitActivated, FALSE);
                }

                objData->fadeCompletedTicks++;
            }
        }
        break;
    case AlphaAnimator_MODE_3_Radial_Falloff_Fade:
        fadeSpeed = (objSetup->alphaAnim.fadeSpeed < 0) ? -objSetup->alphaAnim.fadeSpeed : objSetup->alphaAnim.fadeSpeed;

        objData->fadeRadiusOuter += (fadeSpeed / 10.0f) * gUpdateRateF;
        if (objData->fadeRadiusOuter > objData->fadeRadiusGoal) {
            objData->fadeRadiusOuter = objData->fadeRadiusGoal;
            mainSetBits(objSetup->alphaAnim.gamebitActivated, TRUE);
            objData->fadeCompletedTicks++;
        }

        objData->fadeRadiusInner = objData->fadeRadiusOuter - 50.0f;
        break;
    }

    AlphaAnimator_animateVertices(objData, objSetup, block);
}

// offset: 0x808 | func: 2
void AlphaAnimator_animateVertices(AlphaAnimator_Data* objData, AlphaAnimator_Setup* objSetup, Block* block) {
    f32 tOpacity;
    s32 vtxIdx;
    BlockShape* shapes;
    Vtx_t* vertices;
    Vtx_t* vtx;
    s32 animVtxIdx;
    s32 shapeIdx;

    vertices = block->vertices2[block->vtxFlags & 1];

    //Loop over the block's shapes
    for (shapeIdx = 0, animVtxIdx = 0, shapes = block->shapes; shapeIdx < block->shapeCount; shapeIdx++) {
        //Check if the shape has a matching animatorID tag
        if (objData->animatorID == shapes[shapeIdx].animatorID) {
            //Loop over the shape's vertices
            for (vtxIdx = shapes[shapeIdx].vtxBase; vtxIdx < shapes[shapeIdx + 1].vtxBase; vtxIdx++) {
                if (GET_MODE(objSetup->alphaAnim.flags) != AlphaAnimator_MODE_3_Radial_Falloff_Fade) {
                    vertices[vtxIdx].cn[3] = objData->vtxOpacity;
                } else {
                    //Mode 3: Radial Falloff Fade
                    if (objData->fadeActivated) {
                        if (block->vertices[vtxIdx].cn[3] != 0) { //Ignore vertices if they have 0 opacity painted
                            //Calculate the vertex's opacity tValue, based on its distance and the current fade radius
                            tOpacity = (objData->vtxDistances[animVtxIdx] - objData->fadeRadiusInner) * 0.02f;
                            if (tOpacity > 1.0f) {
                                tOpacity = 1.0f;
                            } else if (tOpacity < 0.0f) {
                                tOpacity = 0.0f;
                            }

                            //Optionally invert behaviour
                            if (objSetup->alphaAnim.fadeSpeed < 0) {
                                //Fade out vertices inside the radius
                                vertices[vtxIdx].cn[3] = tOpacity * MAX_OPACITY;
                            } else {
                                //Or fade out vertices outside the radius
                                vertices[vtxIdx].cn[3] = (1.0f - tOpacity) * MAX_OPACITY;
                            }
                        }
                        animVtxIdx++;
                    } else {
                        vertices[vtxIdx].cn[3] = objSetup->alphaAnim.initialOpacity;
                    }
                }
            }

            //Update shape's render flags
            if (GET_MODE(objSetup->alphaAnim.flags) != AlphaAnimator_MODE_3_Radial_Falloff_Fade) {
                if (objData->vtxOpacity == 0) {
                    shapes[shapeIdx].flags |= RENDER_SHAPE_HIDE;
                    if (objSetup->alphaAnim.removeCollisionWhenHidden) {
                        shapes[shapeIdx].flags |= RENDER_UNK800;
                    }
                } else {
                    shapes[shapeIdx].flags &= ~RENDER_SHAPE_HIDE;
                    if (objSetup->alphaAnim.removeCollisionWhenHidden) {
                        shapes[shapeIdx].flags &= ~RENDER_UNK800;
                    }
                }
            } else {
                //Mode 3: Radial Falloff Fade
                if (objData->fadeActivated) {
                    shapes[shapeIdx].flags &= ~RENDER_SHAPE_HIDE;
                    if ((objData->fadeCompletedTicks != 0) && (objSetup->alphaAnim.goalOpacity == 0)) {
                        shapes[shapeIdx].flags |= RENDER_SHAPE_HIDE;
                    }
                } else if (objSetup->alphaAnim.initialOpacity == 0) {
                    shapes[shapeIdx].flags |= RENDER_SHAPE_HIDE;
                }
            }
        }
    }
}

// offset: 0xBB8 | func: 3 | export: 2
void AlphaAnimator_obj_Update(Object* self) { }

// offset: 0xBC4 | func: 4 | export: 3
void AlphaAnimator_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}


// offset: 0xC18 | func: 5 | export: 4
void AlphaAnimator_obj_Free(Object* self, s32 onlySelf) {
    AlphaAnimator_Data* objData = self->data;

    if (objData->vtxDistances != NULL) {
        mmFree(objData->vtxDistances);
    }
}

// offset: 0xC68 | func: 6 | export: 5
u32 AlphaAnimator_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0xC78 | func: 7 | export: 6
u32 AlphaAnimator_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(AlphaAnimator_Data);
}

// offset: 0xC8C | func: 8
void AlphaAnimator_calculateAnimatedVertexDistances(Object* self, AlphaAnimator_Data* objData) {
    Block* block;
    BlockShape* shapes;
    f32 localX;
    f32 localZ;
    f32 dx;
    f32 dz;
    s32 vtxID;
    s32 blockWorldGridX;
    s32 blockWorldGridZ;
    s32 animVtxIdx;
    s32 shapeIdx;

    block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if (block == NULL || !(block->vtxFlags & 8)) {
        return;
    }

    blockWorldGridX = floorf((self->srt.transl.x - gWorldX) / BLOCKS_GRID_UNIT_F);
    blockWorldGridZ = floorf((self->srt.transl.z - gWorldZ) / BLOCKS_GRID_UNIT_F);

    localX = self->srt.transl.x - (blockWorldGridX * BLOCKS_GRID_UNIT_F + gWorldX);
    localZ = self->srt.transl.z - (blockWorldGridZ * BLOCKS_GRID_UNIT_F + gWorldZ);

    shapes = block->shapes;

    for (shapeIdx = 0, animVtxIdx = 0; shapeIdx < block->shapeCount; shapeIdx++) {
        if (objData->animatorID == shapes[shapeIdx].animatorID) {

            for (vtxID = shapes[shapeIdx].vtxBase; vtxID < shapes[shapeIdx + 1].vtxBase; vtxID++) {
                dx = block->vertices[vtxID].ob[0];
                dx -= localX;
                dz = block->vertices[vtxID].ob[2];
                dz -= localZ;

                objData->vtxDistances[animVtxIdx++] = sqrtf(SQ(dx) + SQ(dz));
            }
        }
    }
}

/*0x0*/ static const char str_0[] = "%d:%d,%d %d:%d\n";
