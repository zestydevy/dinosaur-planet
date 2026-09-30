#include "common.h"
#include "dlls/objects/429_DFSH_Door1Special.h"

static int DFSH_Door2Special_animCallback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void DFSH_Door2Special_ctor(void* dll) { }

// offset: 0xC | dtor
void DFSH_Door2Special_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFSH_Door2Special_obj_Setup(Object* self, DFSH_DoorSpecial_Setup* objSetup, s32 reset) {
    DFSH_DoorSpecial_Data* objData;
    TextureAnimator* texAnim;

    objData = self->data;
    self->animCallback = DFSH_Door2Special_animCallback;

    //Restore texture glow state
    {
        if (mainGetBits(objSetup->gamebitLit)) {
            objData->glowState = DFSH_DoorSpecial_GLOW_2_Pulse;
        } else {
            objData->glowState = DFSH_DoorSpecial_GLOW_0_Unlit;
        }
        
        texAnim = objExprGetTexAnimator(self, 0, 0);
        if (texAnim != NULL) {
            if (objData->glowState == DFSH_DoorSpecial_GLOW_2_Pulse) {
                texAnim->frame = 1;
            } else {
                texAnim->frame = 0;
            }
        }
    }
    
    objData->phase = 0;
}

// offset: 0xD4 | func: 1 | export: 1
void DFSH_Door2Special_obj_Control(Object* self) { }

// offset: 0xE0 | func: 2 | export: 2
void DFSH_Door2Special_obj_Update(Object* self) { }

// offset: 0xEC | func: 3 | export: 3
void DFSH_Door2Special_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x140 | func: 4 | export: 4
void DFSH_Door2Special_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x150 | func: 5 | export: 5
u32 DFSH_Door2Special_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x160 | func: 6 | export: 6
u32 DFSH_Door2Special_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DFSH_DoorSpecial_Data);
}

// offset: 0x174 | func: 7
int DFSH_Door2Special_animCallback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    TextureAnimator* texAnim;
    DFSH_DoorSpecial_Setup* objSetup;
    DFSH_DoorSpecial_Data* objData;
    s32 frame;

    objData = self->data;
    objSetup = (DFSH_DoorSpecial_Setup*)self->setup;
    
    //Texture glow State Machine
    switch (objData->glowState) {
    case DFSH_DoorSpecial_GLOW_0_Unlit:
        if (mainGetBits(objSetup->gamebitLit)) {
            objData->glowState = DFSH_DoorSpecial_GLOW_1_Fade_In;
        }
        break;
    case DFSH_DoorSpecial_GLOW_1_Fade_In:
        //Texture blends into glowing state
        texAnim = objExprGetTexAnimator(self, 0, 0);
        if (texAnim != NULL) {
            frame = texAnim->frame + (gUpdateRate * 0x10);
            if (frame > 0x100) {
                frame = 0x100;
                objData->glowState = DFSH_DoorSpecial_GLOW_2_Pulse;
            }
            texAnim->frame = frame;
        }
        break;
    default:
    case DFSH_DoorSpecial_GLOW_2_Pulse:
        //Glow pulses slowly, via oscillating texture frame blending 
        texAnim = objExprGetTexAnimator(self, 0, 0);
        if (texAnim != NULL) {
            objData->phase += gUpdateRate * 800;
            texAnim->frame = 0x100 - ((1.0f - mathCosfInterp(objData->phase)) * 50.0f);
        }
        break;
    }

    return 0;
}
