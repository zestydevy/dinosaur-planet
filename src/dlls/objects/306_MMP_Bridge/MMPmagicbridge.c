#include "game/objects/setups/51_Generic.h"
#include "common.h"

typedef struct {
/*00*/ f32 minZ;                       //The position of the vertex furthest from the model's origin along Z (will be negative, and effectively bridge's length)
/*04*/ f32 vertexZs[15];               //A array of unique Z positions extracted from the model's vertices (with +-10 tolerance), sorted along negative Z
/*40*/ u8 vertexFadeState[15];         //(See `MMP_Bridge_VertexStates`) Causes a unique vertex opacity to fade in when 1, fade out when set to 2, or vanish when 0
/*4f*/ u8 vertexZCount;                //A count of the unique vertex Z values stored in the vertexZ array (calculation gets overridden to 10 later on in setup)
/*50*/ u8 vertexAlphas[15];            //Vertex colour alpha values for each Z position in the vertexZs array
/*5f*/ u8 visible;                     //The bridge is drawn when this is set
/*60*/ u16* vtxFadeInTimes;            //Fade in time offsets for each unique vertex Z position, used to create a staggered or simultaneous fade-in animation
/*64*/ u16* vtxFadeOutTimes;           //Fade out time offsets for each unique vertex Z position, used to create a staggered or simultaneous fade-in animation
/*68*/ u16 phaseAngleA;                //Angle value for the vertices' sinusoidal waving animation
/*6A*/ u16 phaseAngleB;                //Advances, but not used for anything (maybe they once had separate phases for the inner/outer bridge faces)
/*6C*/ f32 timer;                      //Counts down until the bridge disappears (if `objSetup->stayVisible` is FALSE), and manages vertex fade in/out times
/*70*/ u8 flags;                       //See `MMP_Bridge_Flags`
} MMP_Bridge_Data;

typedef enum {
    MMP_Bridge_FLAG_Visible = 1,       //The bridge is currently visible
    MMP_Bridge_FLAG_Tangible = 2       //The bridge is currently solid
} MMP_Bridge_Flags;

typedef enum {
    Vtx_ZERO_OPACITY = 0,
    Vtx_FADE_IN = 1,
    Vtx_FADE_OUT = 2
} MMP_Bridge_VertexStates;

//Time offsets for staggered vertex fade in/out animation
/*0x0*/ static u16 dInStaggered[] = { 0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280 };
/*0x20*/ static u16 dOutStaggered[] = { 0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280 };

//Zeroed time offsets, vertices fade in/out in sync
/*0x40*/ static u16 dInSimultaneous[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
/*0x60*/ static u16 dOutSimultaneous[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#define MAX_OPACITY 0xFF

#define FADE_IN_SPEED 1
#define FADE_OUT_SPEED 1

//Animation duration: fade in to MAX_OPACITY opacity, hold (for `objSetup->duration` seconds), fade out to 0 opacity 
#define ANIMATION_DURATION_FULL ((objSetup->duration * 60.0f) + ((FADE_IN_SPEED + FADE_OUT_SPEED) * MAX_OPACITY + 2.0f))

static int MMP_Bridge_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

static void MMP_Bridge_advanceAnimation(Object* self, MMP_Bridge_Data* objData);
static void MMP_Bridge_updateVertices(Object* self, MMP_Bridge_Data* objData);

// offset: 0x0 | ctor
void MMP_Bridge_ctor(void* dll) { }

// offset: 0xC | dtor
void MMP_Bridge_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void MMP_Bridge_obj_Setup(Object* self, MMP_Bridge_Setup* objSetup, s32 reset) {
    s32 i;
    int stop;
    s32 minZ;
    s32 vertZ;
    s32 j;
    int nearbyVertFound;
    MMP_Bridge_Data* objData;
    ModelInstance* modelInst;
    Model* model;
    
    self->srt.yaw = objSetup->yaw << 8;
    
    self->modelInstIdx = objSetup->modelIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
    
    self->animCallback = MMP_Bridge_animCallback;

    objData = self->data;
    modelInst = self->modelInsts[self->modelInstIdx];
    model = modelInst->model;
    
    //Find the position of the vertex furthest from the model origin along negative Z (bridge's length, effectively!)
    minZ = 0;
    for (i = 0; i < model->vertexCount; i++) {
        if (minZ > model->vertices[i].v.ob[2]) {
            minZ = model->vertices[i].v.ob[2];
        }
    }
    
    for (i = 0; i < 15; i++) { 
        objData->vertexZs[i] = 10000.0f; 
    } 
    
    //Store an array of vertices' unique Z values (with +- 10 tolerance), as well as the array's count
    for (i = 0; i < model->vertexCount; i++) {
        nearbyVertFound = FALSE;
        vertZ = model->vertices[i].v.ob[2]; 
        for (j = 0; j < objData->vertexZCount; j++) {
            if ((vertZ == minZ) || 
                (((((s16)objData->vertexZs[j]) - 10) < vertZ) && (vertZ < (((s16) objData->vertexZs[j]) + 10)))
            ) {
                nearbyVertFound = TRUE;
                j = objData->vertexZCount; //index break
            }
        }
        
        if (nearbyVertFound == FALSE) {
            objData->vertexZs[objData->vertexZCount] = vertZ;
            objData->vertexZCount++;

            if (1) {}
        }
    }
    
    //Sort the uniqueVertexZ array from largest to smallest (progressing along negative Z)
    stop = FALSE;
    while (stop == FALSE) {
        stop = TRUE;
        for (i = 0; i < objData->vertexZCount - 1; i++) {
            if (objData->vertexZs[i] < objData->vertexZs[i + 1]) {
                stop = FALSE;
                vertZ = objData->vertexZs[i];
                objData->vertexZs[i] = objData->vertexZs[i + 1];
                objData->vertexZs[i + 1] = vertZ;
            }
        }
    }

    //Use a hard-coded count instead of the earlier calculated one
    objData->vertexZCount = 10;

    objData->minZ = minZ;

    //Restore visibility state
    if (mainGetBits(objSetup->gamebitVisible)) {
        objData->flags |= MMP_Bridge_FLAG_Visible | MMP_Bridge_FLAG_Tangible;
    }

    //Set initial opacity and collision
    if (objData->flags & MMP_Bridge_FLAG_Visible) {
        for (i = 0; i < objData->vertexZCount; i++) {
            objData->vertexAlphas[i] = MAX_OPACITY;
            objData->vertexFadeState[i] = Vtx_FADE_IN;
        }
        func_8002674C(self);
    } else {
        func_800267A4(self);
    }
    
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED;

    objData->timer = 0.0f;
    
    //Choose between staggered or simultaneous vertex fade animations
    if (objSetup->useTravellingFade == TRUE) {
        objData->vtxFadeInTimes = dInStaggered;
        objData->vtxFadeOutTimes = dOutStaggered;
    } else {
        //Zero time offsets, vertices fade in sync
        objData->vtxFadeInTimes = dInSimultaneous;
        objData->vtxFadeOutTimes = dOutSimultaneous;
    }
}

// offset: 0x35C | func: 1 | export: 1
void MMP_Bridge_obj_Control(Object* self) {

    MMP_Bridge_Data* objData;
    MMP_Bridge_Setup* objSetup;
    f32 animationLength;
    s32 i;
    s32 opacity;

    objData = self->data;
    objSetup = (MMP_Bridge_Setup*)self->setup;

    MMP_Bridge_advanceAnimation(self, objData);

    if (objData->flags & MMP_Bridge_FLAG_Visible) {
        if ((objData->flags & MMP_Bridge_FLAG_Tangible) == FALSE) {
            objData->flags |= MMP_Bridge_FLAG_Tangible;
            mainSetBits(objSetup->gamebitVisible, TRUE);
            func_8002674C(self);
        }
        
        if (objData->timer > 0.0f) {
            objData->timer -= gUpdateRateF;
            if (objData->timer <= 0.0f) {
                objData->timer = 0.0f;
                if (objSetup->stayVisible == FALSE) {
                    objData->flags &= ~(MMP_Bridge_FLAG_Visible | MMP_Bridge_FLAG_Tangible);
                    mainSetBits(objSetup->gamebitVisible, FALSE);
                    func_800267A4(self);
                }
            }
        }
        
        //Vertex opacity animation (fade vertices in/out in a staggered/overlapping way that progresses along the bridge)
        animationLength = ANIMATION_DURATION_FULL;
        
        for (i = 0; i < objData->vertexZCount; i++) {
            if (objData->vertexFadeState[i] == Vtx_FADE_IN) {
                opacity = objData->vertexAlphas[i] + gUpdateRate;
                if (opacity > MAX_OPACITY) {
                    opacity = MAX_OPACITY;
                }
                objData->vertexAlphas[i] = (u8)opacity;
                
                if (objData->timer <= (objData->vtxFadeOutTimes[i] + FADE_OUT_SPEED * (MAX_OPACITY + 1.0f))) {
                    objData->vertexFadeState[i] = Vtx_FADE_OUT;
                }
            } else if (objData->vertexFadeState[i] == Vtx_FADE_OUT) {
                if (objSetup->stayVisible == FALSE) {
                    opacity = objData->vertexAlphas[i] - gUpdateRate;
                    if (opacity < 0) {
                        opacity = 0;
                    }
                    objData->vertexAlphas[i] = (u8)opacity;
                }
            } else if (objData->timer <= animationLength - objData->vtxFadeInTimes[i]) {
                objData->vertexFadeState[i] = Vtx_FADE_IN;
            }
        }        
    }
    
    MMP_Bridge_updateVertices(self, objData);
}

// offset: 0x610 | func: 2 | export: 2
void MMP_Bridge_obj_Update(Object* self) { }

// offset: 0x61C | func: 3 | export: 3
void MMP_Bridge_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    MMP_Bridge_Data* objData = self->data;
    if (visibility && (objData->flags & MMP_Bridge_FLAG_Visible)) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x688 | func: 4 | export: 4
void MMP_Bridge_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x698 | func: 5 | export: 5
u32 MMP_Bridge_obj_GetModelFlags(Object* self) {
    MMP_Bridge_Setup* objSetup;
    s8 modelIdx;

    objSetup = (MMP_Bridge_Setup*)self->setup;
    
    modelIdx = objSetup->modelIdx;
    if (modelIdx >= self->def->numModels) {
        modelIdx = 0;
    }
    
    return MODFLAGS_MODEL_INDEX(modelIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x6CC | func: 6 | export: 6
u32 MMP_Bridge_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(MMP_Bridge_Data);
}

// offset: 0x6E0 | func: 7
int MMP_Bridge_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    MMP_Bridge_Data* objData;
    MMP_Bridge_Setup* objSetup;
    s32 i;
    s32 j;
    
    objSetup = (MMP_Bridge_Setup*)self->setup;
    objData = self->data;
    animData->unk62 = 0;
    
    //Become visible via an objSeq message
    for (i = 0; i < animData->messageCount; i++) {
        if ((animData->messages[i] == 1) && (objData->timer <= 0.0f)) {
            for (j = 0; j < objData->vertexZCount; j++) {
                objData->vertexFadeState[j] = Vtx_ZERO_OPACITY;
                objData->vertexAlphas[j] = 0;
            }

            objData->timer = ANIMATION_DURATION_FULL;
            objData->flags |= MMP_Bridge_FLAG_Visible;
        }
    }
    
    MMP_Bridge_obj_Control(self);
    
    return 0;
}

// offset: 0x7FC | func: 8
void MMP_Bridge_advanceAnimation(Object* self, MMP_Bridge_Data* objData) {
    TextureAnimator* texAnim;
    s32 angle;
    s32 i;

    //Scroll texture UVs
    {
        for (i = 0; i < 10; i++) {
            texAnim = objExprGetTexAnimator(self, i, 0);
            
            texAnim->positionV += 8; //@framerate-dependent
            if (texAnim->positionV > 0x800) {
                texAnim->positionV -= 0x800;
            }
            
            texAnim->positionU += 4; //@framerate-dependent
            if (texAnim->positionU > 0x800) {
                texAnim->positionU -= 0x800;
            }
        }
        
        for (i = 10; i < 20; i++) {
            texAnim = objExprGetTexAnimator(self, i, 0);
            
            texAnim->positionV += 10; //@framerate-dependent
            if (texAnim->positionV > 0x800) {
                texAnim->positionV -= 0x800;
            }
        }
    }
    
    //Advance phaseAngleA
    angle = objData->phaseAngleA + (gUpdateRate << 8);
    if (angle >= M_360_DEGREES) {
        angle += 0xFFFF0001;
    }
    objData->phaseAngleA = angle;
    
    //Advance phaseAngleB (not used for anything?)
    angle = objData->phaseAngleB + (gUpdateRate << 7);
    if (angle >= M_360_DEGREES) {
        angle += 0xFFFF0001;
    }
    objData->phaseAngleB = angle;
}

// offset: 0x95C | func: 9
void MMP_Bridge_updateVertices(Object* self, MMP_Bridge_Data* objData) {
    ModelInstance* modelInst;
    Model* model;
    Vtx* vertices;
    s32 vertexIdx;
    s32 shapeIdx;
    f32 tValue;
    s32 phase;
    s32 idx;

    modelInst = self->modelInsts[self->modelInstIdx];
    model = modelInst->model;
    vertices = modelInst->vertices[(modelInst->unk34 >> 1) & 1];
    
    for (shapeIdx = 0; shapeIdx < model->unk70; shapeIdx++) {
        for (vertexIdx = model->faces[shapeIdx].baseVertexID; vertexIdx < model->faces[shapeIdx + 1].baseVertexID; vertexIdx++) {
            //Get the vertex's animation phase angle, based on its tValue position in Z along the bridge
            tValue = vertices[vertexIdx].v.ob[2] / objData->minZ;
            phase = (((u32) (tValue * (M_360_DEGREES - 1))) & 0xFFFF) + objData->phaseAngleA;
            
            //Oscillate the vertex along X axis (mirrored across X)
            if (model->vertices[vertexIdx].v.ob[0] > 0) {
                vertices[vertexIdx].v.ob[0] = model->vertices[vertexIdx].v.ob[0] + (mathSinfInterp(phase) * 20.0f);
            } else {
                vertices[vertexIdx].v.ob[0] = model->vertices[vertexIdx].v.ob[0] - (mathSinfInterp(phase) * 20.0f);
            }
            
            //Animate vertex opacity (only on vertices with nonzero initial alpha)
            if (model->vertices[vertexIdx].v.cn[3] != 0) {
                idx = model->faces[shapeIdx].tagB;
                if (idx > 10) {
                    idx -= 10;
                }
                idx--;
                
                //Use the vertex's model tags and Z position to determine which vertexAlpha index to use
                if (vertices[vertexIdx].v.ob[2] >= (s16)objData->vertexZs[idx] - 8) {
                    vertices[vertexIdx].v.cn[3] = objData->vertexAlphas[idx];
                } else if (objData->vertexFadeState[idx + 1] != Vtx_ZERO_OPACITY) {
                    vertices[vertexIdx].v.cn[3] = objData->vertexAlphas[idx + 1];
                } else {
                    vertices[vertexIdx].v.cn[3] = 0;
                }
            }
        }
    }
}
