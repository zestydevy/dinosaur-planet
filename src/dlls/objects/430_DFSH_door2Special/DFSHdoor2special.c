#include "common.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebitOpened;
/*1A*/ s16 gamebitB;
/*1C*/ s16 seqPreemptTime;
/*1E*/ s8 seqIndex;
/*1F*/ u8 yaw;
/*20*/ u8 enabledActors;
/*21*/ u8 scale;
/*22*/ s16 gamebitLit;
} DFSH_DoorSpecial_Setup;

typedef struct {
    u16 phase;
    u8 state;
    u8 glowState;
    u8 runControl;
} DFSH_DoorSpecial_Data;

typedef enum {
    DFSH_DoorSpecial_GLOW_0_Unlit,
    DFSH_DoorSpecial_GLOW_1_Fade_In,
    DFSH_DoorSpecial_GLOW_2_Pulse
} DFSH_DoorSpecial_GlowStates;

static int DFSH_Door2Special_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3);

// offset: 0x0 | ctor
void DFSH_Door2Special_ctor(void *dll) { }

// offset: 0xC | dtor
void DFSH_Door2Special_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFSH_Door2Special_setup(Object* self, DFSH_DoorSpecial_Setup* objSetup, s32 reset) {
    DFSH_DoorSpecial_Data* objData;
    TextureAnimator* texAnim;

    objData = self->data;
    self->animCallback = DFSH_Door2Special_anim_callback;

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
void DFSH_Door2Special_control(Object *self) { }

// offset: 0xE0 | func: 2 | export: 2
void DFSH_Door2Special_update(Object *self) { }

// offset: 0xEC | func: 3 | export: 3
void DFSH_Door2Special_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x140 | func: 4 | export: 4
void DFSH_Door2Special_free(Object *self, s32 onlySelf) { }

// offset: 0x150 | func: 5 | export: 5
u32 DFSH_Door2Special_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x160 | func: 6 | export: 6
u32 DFSH_Door2Special_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DFSH_DoorSpecial_Data);
}

// offset: 0x174 | func: 7
int DFSH_Door2Special_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
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
            texAnim->frame = 0x100 - ((1.0f - fcos16_precise(objData->phase)) * 50.0f);
        }
        break;
    }

    return 0;
}
