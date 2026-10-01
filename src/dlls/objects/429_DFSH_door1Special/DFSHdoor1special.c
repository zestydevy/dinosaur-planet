#include "common.h"
#include "dlls/objects/307_SeqDoor.h"
#include "dlls/objects/429_DFSH_Door1Special.h"
#include "game/gamebits.h"

static int DFSH_Door1Special_animCallback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackResult);

// offset: 0x0 | ctor
void DFSH_Door1Special_ctor(void* dll) { }

// offset: 0xC | dtor
void DFSH_Door1Special_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFSH_Door1Special_obj_Setup(Object* self, DFSH_DoorSpecial_Setup* objSetup, s32 reset) {
    DFSH_DoorSpecial_Data* objData;
    TextureAnimator* texAnim;

    objData = self->data;
    
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
    
    objData->runControl = TRUE;
    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = DFSH_Door1Special_animCallback;
    
    //Set scale
    {
        if (objSetup->scale == 0) {
            objSetup->scale = 64;
        }
        self->srt.scale = objSetup->scale * (1.0f / 64.0f);
        if (self->srt.scale == 0.0f) {
            self->srt.scale = 1.0f;
        }
        self->srt.scale *= self->def->scale;
    }

    //Restore state by gamebit
    if (objSetup->gamebitDoorState != NO_GAMEBIT) {
        objData->state = mainGetBits(objSetup->gamebitDoorState);
    } else {
        objData->state = DFSH_Door1Special_STATE_0_Closed;
    }
    
    objData->phase = 0;
}

// offset: 0x1B0 | func: 1 | export: 1
void DFSH_Door1Special_obj_Control(Object* self) {
    DFSH_DoorSpecial_Data* objData;
    DFSH_DoorSpecial_Setup* objSetup;
    s32 enabledActors;

    objData = self->data;
    objSetup = (DFSH_DoorSpecial_Setup*)self->setup;
    
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
void DFSH_Door1Special_obj_Update(Object* self) { }

// offset: 0x29C | func: 3 | export: 3
void DFSH_Door1Special_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x2F0 | func: 4 | export: 4
void DFSH_Door1Special_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x300 | func: 5 | export: 5
u32 DFSH_Door1Special_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x310 | func: 6 | export: 6
u32 DFSH_Door1Special_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DFSH_DoorSpecial_Data);
}

// offset: 0x324 | func: 7
int DFSH_Door1Special_animCallback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackResult) {
    DFSH_DoorSpecial_Data* objData;
    DFSH_DoorSpecial_Setup* objSetup;
    TextureAnimator* texAnim;
    s32 i;
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
        texAnim = objExprGetTexAnimator(self, 0, 0);
        if (texAnim != NULL) {
            objData->phase += gUpdateRate * 800;
            texAnim->frame = 0x100 - ((1.0f - mathCosfInterp(objData->phase)) * 50.0f);
        }
        break;
    }
    
    //Door opening State Machine (very similar to DLL 307 "SeqDoor")
    if (objData->state == DFSH_Door1Special_STATE_0_Closed) {
        if (mainGetBits(objSetup->gamebitOpened)) {
            objData->state = DFSH_Door1Special_STATE_2_Opening;
        }
    } else if ((objData->state == DFSH_Door1Special_STATE_1_Open) && (mainGetBits(objSetup->gamebitOpened) == FALSE)) {
        objData->state = DFSH_Door1Special_STATE_3_Closing;
    }
    
    if (objData->state == DFSH_Door1Special_STATE_2_Opening) {
        for (i = 0; i < animData->messageCount; i++) {
            if (animData->messages[i] == SeqDoor_SEQCMD_2_Finished_Opening) {
                objData->state = DFSH_Door1Special_STATE_1_Open;
                if (objSetup->gamebitDoorState != NO_GAMEBIT) {
                    mainSetBits(objSetup->gamebitDoorState, DFSH_Door1Special_STATE_1_Open);
                }
            }
        }
    } else if (objData->state == DFSH_Door1Special_STATE_3_Closing) {
        for (i = 0; i < animData->messageCount; i++) {
            if (animData->messages[i] == SeqDoor_SEQCMD_1_Finished_Closing) {
                objData->state = DFSH_Door1Special_STATE_0_Closed;
                if (objSetup->gamebitDoorState != 1) { //@bug?: should this be -1 (NO_GAMEBIT)?
                    mainSetBits(objSetup->gamebitDoorState, DFSH_Door1Special_STATE_0_Closed);
                }
            }
        }
    }
    
    return !(objData->state == DFSH_Door1Special_STATE_2_Opening) && !(objData->state == DFSH_Door1Special_STATE_3_Closing);
}
