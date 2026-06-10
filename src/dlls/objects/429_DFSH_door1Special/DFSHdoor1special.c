#include "common.h"
#include "game/gamebits.h"

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
} DFSH_Door1Special_Setup;

typedef struct {
    u16 phase;
    u8 state;
    u8 glowState;
    u8 runControl;
} DFSH_DoorSpecial_Data;

typedef enum {
    DFSH_Door1Special_STATE_0,
    DFSH_Door1Special_STATE_1,
    DFSH_Door1Special_STATE_2,
    DFSH_Door1Special_STATE_3
} DFSH_DoorSpecial_States;

typedef enum {
    DFSH_DoorSpecial_GLOW_0_Unlit,
    DFSH_DoorSpecial_GLOW_1_Fade_In,
    DFSH_DoorSpecial_GLOW_2_Pulse
} DFSH_DoorSpecial_GlowStates;

static int DFSH_Door1Special_anim_callback(Object* self, Object* arg1, AnimObj_Data* animData, s8 prevCallbackResult);

// offset: 0x0 | ctor
void DFSH_Door1Special_ctor(void *dll) { }

// offset: 0xC | dtor
void DFSH_Door1Special_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFSH_Door1Special_setup(Object* self, DFSH_Door1Special_Setup* objSetup, s32 reset) {
    DFSH_DoorSpecial_Data* objData;
    TextureAnimator* texAnim;

    objData = self->data;
    
    //Restore texture glow state
    {
        if (main_get_bits(objSetup->gamebitLit)) {
            objData->glowState = DFSH_DoorSpecial_GLOW_2_Pulse;
        } else {
            objData->glowState = DFSH_DoorSpecial_GLOW_0_Unlit;
        }
    
        texAnim = func_800348A0(self, 0, 0);
        if (texAnim != NULL) {
            if (objData->glowState == DFSH_DoorSpecial_GLOW_2_Pulse) {
                texAnim->frame = 1;
            } else {
                texAnim->frame = 0;
            }
        }
    }
    
    objData->runControl = TRUE;
    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = DFSH_Door1Special_anim_callback;
    
    //Set scale
    {
        if (objSetup->scale == 0) {
            objSetup->scale = 64;
        }
        self->srt.scale = objSetup->scale * ONE_SIXTY_FOURTH_F;
        if (self->srt.scale == 0.0f) {
            self->srt.scale = 1.0f;
        }
        self->srt.scale *= self->def->scale;
    }

    //Restore state by gamebit
    if (objSetup->gamebitB != NO_GAMEBIT) {
        objData->state = main_get_bits(objSetup->gamebitB);
    } else {
        objData->state = DFSH_Door1Special_STATE_0;
    }
    
    objData->phase = 0;
}

// offset: 0x1B0 | func: 1 | export: 1
void DFSH_Door1Special_control(Object* self) {
    DFSH_DoorSpecial_Data* objData;
    DFSH_Door1Special_Setup* objSetup;
    s32 enabledActors;

    objData = self->data;
    objSetup = (DFSH_Door1Special_Setup*)self->setup;
    
    if (objData->runControl == FALSE) {
        return;
    }
    
    //Skip to end of door-opening sequence if needed
    if (objSetup->seqPreemptTime && objData->state) {
        enabledActors = objSetup->enabledActors;
        gDLL_3_Animation->vtbl->preempt_sequence_time(self, objSetup->seqPreemptTime);
    } else {
        enabledActors = -1;    
    }

    //Play door-opening sequence
    if (objSetup->seqIndex != -1) {
        gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->seqIndex, self, enabledActors);
    }
    
    objData->runControl = FALSE;
}

// offset: 0x290 | func: 2 | export: 2
void DFSH_Door1Special_update(Object *self) { }

// offset: 0x29C | func: 3 | export: 3
void DFSH_Door1Special_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x2F0 | func: 4 | export: 4
void DFSH_Door1Special_free(Object *self, s32 onlySelf) { }

// offset: 0x300 | func: 5 | export: 5
u32 DFSH_Door1Special_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x310 | func: 6 | export: 6
u32 DFSH_Door1Special_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DFSH_DoorSpecial_Data);
}

// offset: 0x324 | func: 7
int DFSH_Door1Special_anim_callback(Object* self, Object* arg1, AnimObj_Data* animData, s8 prevCallbackResult) {
    DFSH_DoorSpecial_Data* objData;
    DFSH_Door1Special_Setup* objSetup;
    TextureAnimator* texAnim;
    s32 i;
    s32 frame;

    objData = self->data;
    objSetup = (DFSH_Door1Special_Setup*)self->setup;
    
    //Texture glow State Machine
    switch (objData->glowState) {
    case DFSH_DoorSpecial_GLOW_0_Unlit:
        if (main_get_bits(objSetup->gamebitLit)) {
            objData->glowState = DFSH_DoorSpecial_GLOW_1_Fade_In;
        }
        break;
    case DFSH_DoorSpecial_GLOW_1_Fade_In:
        //Texture blends into glowing state
        texAnim = func_800348A0(self, 0, 0);
        if (texAnim != NULL) {
            frame = texAnim->frame + (gUpdateRate * 8);
            if (frame > 0x100) {
                frame = 0x100;
                objData->glowState = DFSH_DoorSpecial_GLOW_2_Pulse;
            }
            texAnim->frame = frame;
        }
        break;
    case DFSH_DoorSpecial_GLOW_2_Pulse:
    default:
        //Glow pulses slowly, via oscillating texture frame blending 
        texAnim = func_800348A0(self, 0, 0);
        if (texAnim != NULL) {
            objData->phase += gUpdateRate * 800;
            texAnim->frame = 0x100 - ((1.0f - fcos16_precise(objData->phase)) * 50.0f);
        }
        break;
    }
    
    if (objData->state == DFSH_Door1Special_STATE_0) {
        if (main_get_bits(objSetup->gamebitOpened)) {
            objData->state = DFSH_Door1Special_STATE_2;
        }
    } else if ((objData->state == DFSH_Door1Special_STATE_1) && (main_get_bits(objSetup->gamebitOpened) == FALSE)) {
        objData->state = DFSH_Door1Special_STATE_3;
    }
    
    if (objData->state == DFSH_Door1Special_STATE_2) {
        for (i = 0; i < animData->messageCount; i++) {
            if (animData->messages[i] == 2) {
                objData->state = DFSH_Door1Special_STATE_1;
                if (objSetup->gamebitB != NO_GAMEBIT) {
                    main_set_bits(objSetup->gamebitB, TRUE);
                }
            }
        }
    } else if (objData->state == DFSH_Door1Special_STATE_3) {
        for (i = 0; i < animData->messageCount; i++) {
            if (animData->messages[i] == 1) {
                objData->state = DFSH_Door1Special_STATE_0;
                if (objSetup->gamebitB != 1) { //@bug?: should this be -1 (NO_GAMEBIT)?
                    main_set_bits(objSetup->gamebitB, FALSE);
                }
            }
        }
    }
    
    return !(objData->state == DFSH_Door1Special_STATE_2) && !(objData->state == DFSH_Door1Special_STATE_3);
}
