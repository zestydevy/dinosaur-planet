#include "common.h"
#include "dlls/objects/210_player.h"
#include "game/objects/setups/51_Generic.h"

typedef struct {
/*00*/ f32 minZ;                       //The position of the vertex furthest from the model's origin along Z (will be negative, and effectively bridge's length)
/*04*/ f32 vertexZs[15];               //A array of unique Z positions extracted from the model's vertices (with +-10 tolerance), sorted along negative Z
/*40*/ u8 vertexFadeIn[15];            //Causes a unique vertex opacity to fade in when true (forced on)
/*4f*/ u8 vertexZCount;                //A count of the unique vertex Z values stored in the vertexZ array (calculation gets overridden to 10 later on in setup)
/*50*/ u8 vertexAlphas[15];            //Vertex colour alpha values for each Z position in the vertexZs array
/*5f*/ u8 visible;                     //The bridge is drawn when this is set
/*60*/ u16 phaseAngleA;                //Angle value for the vertices' sinusoidal waving animation
/*62*/ u16 phaseAngleB;                //Advances, but not used for anything (maybe they once had separate phases for the inner/outer bridge faces)
/*65*/ s16 fadeInWaveTimer;            //Used to progressively fade in the bridge's vertices in a wave along the bridge (DBMagicBridge, DIMMagicBridge)
/*66*/ u8 flags;                       //Tracks whether the gamebit has been set
} DIMMagicBridge_Data;

typedef enum {
    WCTempleBridge_FLAG_Visibility_Gamebit_Set = 1
} WCTempleBridge_Flags;

#define MAX_OPACITY 0xFF
#define DIM_MAGIC_BRIDGE_HITS_ANIMATOR 0x11

static void DIMMagicBridge_advanceAnimation(Object* self, DIMMagicBridge_Data* objData);
static void DIMMagicBridge_updateVertices(Object* self, DIMMagicBridge_Data* objData);
static int DIMMagicBridge_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void DIMMagicBridge_ctor(void* dll) { }

// offset: 0xC | dtor
void DIMMagicBridge_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIMMagicBridge_obj_Setup(Object* self, DIMMagicBridge_Setup* objSetup, s32 reset) {
    s32 i;
    int stop;
    s32 minZ;
    s32 vertZ;
    s32 j;
    int nearbyVertFound;
    DIMMagicBridge_Data* objData;
    ModelInstance* modelInst;
    Model* model;
    
    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = DIMMagicBridge_animCallback;
    
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
    
    for (i = 0; i < 15; i++) { objData->vertexZs[i] = 10000.0f; } 
    
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
    if (mainGetBits(BIT_DIM_Magic_Bridge_Visible)) {
        objData->visible = TRUE;
    }

    if (objData->visible) {
        for (i = 0; i < objData->vertexZCount; i++) {
            objData->vertexAlphas[i] = MAX_OPACITY;
            objData->vertexFadeIn[i] = TRUE;

            //@bug: shouldn't this be outside the loop?
            trackToggleHitLine(DIM_MAGIC_BRIDGE_HITS_ANIMATOR, NULL, FALSE);
        }
    }
    
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED;
}

// offset: 0x2CC | func: 1 | export: 1
void DIMMagicBridge_obj_Control(Object* self) {
    DIMMagicBridge_Data* objData;
    Object* player;
    s32 sp24;
    f32 sp20;

    player = objGetPlayer();
    objData = self->data;
    
    DIMMagicBridge_advanceAnimation(self, objData);
    DIMMagicBridge_updateVertices(self, objData);
    
    if (objData->visible == FALSE) {
        sp24 = dll_player(player)->func36(player, &sp20);

        if (mainGetBits(BIT_1EF) && (sp24 != 1) && dll_player(player)->func42(player)) {
            dll_player(player)->func37(player, 1);
        }

        if ((sp24 == 1) && (sp20 < 0.1f)) {
            mainSetBits(BIT_1E8, TRUE);
        }
    } else {
        trackToggleHitLine(DIM_MAGIC_BRIDGE_HITS_ANIMATOR, NULL, FALSE);
    }
}

// offset: 0x448 | func: 2 | export: 2
void DIMMagicBridge_obj_Update(Object* self) { }

// offset: 0x454 | func: 3 | export: 3
void DIMMagicBridge_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4A8 | func: 4 | export: 4
void DIMMagicBridge_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x4B8 | func: 5 | export: 5
u32 DIMMagicBridge_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x4C8 | func: 6 | export: 6
u32 DIMMagicBridge_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DIMMagicBridge_Data);
}

// offset: 0x4DC | func: 7
int DIMMagicBridge_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    DIMMagicBridge_Data* objData;
    s32 i;
    s32 opacity;

    objData = self->data;

    animData->unk62 = 0;

    DIMMagicBridge_advanceAnimation(self, objData);

    //Become visible via an objSeq message
    if (animData->lastMessage == 1) {
        animData->lastMessage = 0;
        objData->visible = TRUE;
    }
    
    if (objData->visible) {
        //Gradually add vertices to the fade effect by depth-sorted index, 
        //so the fade ripples down the bridge in a wave
        objData->fadeInWaveTimer -= gUpdateRate;
        if (objData->fadeInWaveTimer <= 0) {
            objData->fadeInWaveTimer = 16;

            for (i = 1; objData->vertexFadeIn[i] && i < objData->vertexZCount; i++);

            objData->vertexFadeIn[i] = TRUE;
        }
        
        //Fade in vertices
        for (i = 1; i < objData->vertexZCount; i++) {
            if (objData->vertexFadeIn[i]) {
                opacity = objData->vertexAlphas[i] + gUpdateRate;
                if (opacity > MAX_OPACITY) {
                    opacity = MAX_OPACITY;
                }
                objData->vertexAlphas[i] = opacity;
            }
        }
    }
    
    DIMMagicBridge_updateVertices(self, objData);
    
    return 0;
}

// offset: 0x648 | func: 8
void DIMMagicBridge_advanceAnimation(Object* self, DIMMagicBridge_Data* objData) {
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

// offset: 0x7A8 | func: 9
void DIMMagicBridge_updateVertices(Object* self, DIMMagicBridge_Data* objData) {
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
                vertices[vertexIdx].v.ob[0] = model->vertices[vertexIdx].v.ob[0] + (mathSinfInterp(phase) * 15.0f);
            } else {
                vertices[vertexIdx].v.ob[0] = model->vertices[vertexIdx].v.ob[0] - (mathSinfInterp(phase) * 15.0f);
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
                } else if (objData->vertexFadeIn[idx + 1]) {
                    vertices[vertexIdx].v.cn[3] = objData->vertexAlphas[idx + 1];
                } else {
                    vertices[vertexIdx].v.cn[3] = 0;
                }
            }
        }
    }
}
