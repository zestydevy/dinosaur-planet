#include "common.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/setups/88_UseObj.h"
#include "sys/math.h"

typedef struct {
    u8 state;
    u8 actID;
} WCUseObj_Data;

typedef enum {
    WCUseObj_STATE_Unused = 0, //The button/item deposit point hasn't been used yet
    WCUseObj_STATE_Used = 1    //The button/item deposit point has been used
} WCUseObj_States;

typedef enum {
    WCUseObj_FLAG_1_Hide_On_Revisit_When_Used = 1,
    WCUseObj_FLAG_2 = 2, //Not used?
    WCUseObj_FLAG_4_ObjSeq_Sets_Gamebit = 4,
    WCUseObj_FLAG_8_Lock_After_Use = 8,
    WCUseObj_FLAG_10_No_Targetting_When_Locked = 0x10,
    WCUseObj_FLAG_20_PreemptActors1and2 = 0x20,
    WCUseObj_FLAG_40_PreemptActors1and2 = 0x40,
    WCUseObj_FLAG_80_PreemptActors1and3 = 0x80
} WCUseObj_Flags;


typedef enum {
    WCUseObj_MODELIDX_Sun = 0,
    WCUseObj_MODELIDX_Moon = 1
} WCUseObj_Models;

static int WCUseObj_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void WCUseObj_ctor(void* dll) { }

// offset: 0xC | dtor
void WCUseObj_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCUseObj_obj_Setup(Object* self, UseObj_Setup* objSetup, s32 reset) {
    WCUseObj_Data* objData;
    TextureAnimator* texAnim;

    self->srt.yaw = objSetup->yaw << 8;
    self->srt.pitch = objSetup->pitch << 8;
    self->srt.roll = objSetup->roll << 8;
    
    self->animCallback = WCUseObj_animCallback;
    
    self->modelInstIdx = objSetup->modelno;
    if (self->modelInstIdx >= self->def->numModels) {
        STUBBED_PRINTF("USEOBJ.c: modelno out of range romdefno=%d\n", objSetup->modelno);
        self->modelInstIdx = 0;
    }
    
    objData = self->data;
    objData->state = mainGetBits(objSetup->gamebitUsed);
    objData->actID = gDLL_29_Gplay->vtbl->get_act(self->mapID);
    
    if ((objSetup->flags & WCUseObj_FLAG_1_Hide_On_Revisit_When_Used) && (objData->state != WCUseObj_STATE_Unused)) {
        self->opacity = 0;
    }
    
    if (objData->state != WCUseObj_STATE_Unused) {
        texAnim = objExprGetTexAnimator(self, 0, 0);
        if (texAnim != NULL) {
            texAnim->frame = 0x100;
        }
    }
}

// offset: 0x148 | func: 1 | export: 1
void WCUseObj_obj_Control(Object* self) {
    WCUseObj_Data* objData;
    UseObj_Setup* objSetup;
    TextureAnimator* texAnim;
    s32 actorMask;

    objData = self->data;
    objSetup = (UseObj_Setup*)self->setup;
    
    objData->state = mainGetBits(objSetup->gamebitUsed);
    
    if (objData->state == WCUseObj_STATE_Unused) {
        texAnim = objExprGetTexAnimator(self, 0, 0);
        if (texAnim != NULL) {
            texAnim->frame = 0;
        }
        
        self->srt.transl.x = objSetup->base.x;
        self->srt.transl.y = objSetup->base.y;
        self->srt.transl.z = objSetup->base.z;

        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        if (objSetup->gamebitEnabled != NO_GAMEBIT) {
            if (mainGetBits(objSetup->gamebitEnabled)) {
                self->unkAF &= ~ARROW_FLAG_10_Greyed_Out;
            } else {
                self->unkAF |= ARROW_FLAG_10_Greyed_Out;
                if (objSetup->flags & WCUseObj_FLAG_10_No_Targetting_When_Locked) {
                    self->unkAF |= ARROW_FLAG_8_No_Targetting;
                }
            }
        } else {
            self->unkAF &= ~ARROW_FLAG_10_Greyed_Out;
        }
        
        //Check if the button was pressed / an item was used
        if ((self->unkAF & ARROW_FLAG_1_Interacted) && 
            (objSetup->gamebitRequiredItem == NO_GAMEBIT || gDLL_1_cmdmenu->vtbl->was_this_item_used(objSetup->gamebitRequiredItem))
        ) {
            if (objSetup->objectSeqIndex != -1) {
                if (self->id == OBJ_WCInvUseObj) {
                    if ((objData->actID == 1) && (mainGetBits(BIT_WC_Placed_Gold_RedEye_Tooth) || mainGetBits(BIT_WC_Placed_Silver_RedEye_Tooth))) {
                        gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->objectSeqIndex + 2, self, -1);
                    } else if ((objData->actID == 2) && (mainGetBits(BIT_WC_Used_Sun_Stone) || mainGetBits(BIT_WC_Used_Moon_Stone))) {
                        gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->objectSeqIndex + 2, self, -1);
                    } else {
                        gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->objectSeqIndex, self, -1);
                    }
                } else {
                    gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->objectSeqIndex, self, -1);
                }
            }
            
            
            if ((objSetup->flags & WCUseObj_FLAG_4_ObjSeq_Sets_Gamebit) == FALSE) {
                mainSetBits(objSetup->gamebitUsed, TRUE);

                texAnim = objExprGetTexAnimator(self, 0, 0);
                if (texAnim != NULL) {
                    texAnim->frame = 0x100;
                }
            }
            
            if (objSetup->flags & WCUseObj_FLAG_8_Lock_After_Use) {
                mainSetBits(objSetup->gamebitEnabled, FALSE);
            } else {
                objData->state = WCUseObj_STATE_Used;
                self->unkDC = 1;
            }
            
            joyDisableButtons(0, A_BUTTON);
        }
    } else {
        //Restore "used" state with an ObjSeq preempt
        if ((self->unkDC == 0) && (objSetup->objectSeqIndex != -1)) {
            if (objSetup->replayStartTime != 0) {
                gDLL_3_Animation->vtbl->preempt_sequence_time(self, objSetup->replayStartTime);
                actorMask = 1;
                if (objSetup->flags & WCUseObj_FLAG_20_PreemptActors1and2) {
                    actorMask = 1 | 2;
                }
                if (objSetup->flags & WCUseObj_FLAG_40_PreemptActors1and2) {
                    actorMask |= 1 | 2;
                }
                if (objSetup->flags & WCUseObj_FLAG_80_PreemptActors1and3) {
                    actorMask |= 4;
                }
                gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->objectSeqIndex, self, actorMask);
            }
        }
        
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    }
    
    self->unkDC = 1;
}

// offset: 0x554 | func: 2 | export: 2
void WCUseObj_obj_Update(Object* self) {
    if ((self->def->flags & OBJDEF_INVISIBLE) && (self->unk74 != NULL)) {
        objprintUpdateLockIconCoords(self);
    }
}

// offset: 0x5AC | func: 3 | export: 3
void WCUseObj_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x600 | func: 4 | export: 4
void WCUseObj_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x610 | func: 5 | export: 5
u32 WCUseObj_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x620 | func: 6 | export: 6
u32 WCUseObj_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WCUseObj_Data);
}

// offset: 0x634 | func: 7
int WCUseObj_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    UseObj_Setup* objSetup = (UseObj_Setup*)self->setup;
    TextureAnimator* texAnim;
    s32 i;
    Vec3f dRestartPoint = VEC3F(-6318.1, -1232, -5884);

    self->unkAF |= ARROW_FLAG_8_No_Targetting;

    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
        default:
        case 1:
            if (objSetup->flags & WCUseObj_FLAG_4_ObjSeq_Sets_Gamebit) {
                mainSetBits(objSetup->gamebitUsed, TRUE);

                texAnim = objExprGetTexAnimator(self, 0, 0);
                if (texAnim != NULL) {
                    texAnim->frame = 0x100;
                }
            }
            break;
        case 2:
            if (objSetup->replayStartTime != 0) {
                gDLL_3_Animation->vtbl->func21(animData, objSetup->replayStartTime);
            }
            break;
        case 3:
            if (self->modelInstIdx == WCUseObj_MODELIDX_Moon) {
                //Set a restart point before the Moon Temple's falling tiles
                gDLL_29_Gplay->vtbl->restart_set(&dRestartPoint, -M_90_DEGREES, mapGetLayer());
            }
            break;
        }
    }
    
    return 0;
}
