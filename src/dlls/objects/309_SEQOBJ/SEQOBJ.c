#include "common.h"
#include "game/gamebits.h"
#include "macros.h"
#include "game/objects/setups/89_SeqObj.h"
#include "sys/gfx/animseq.h"
#include "sys/objtype.h"

typedef struct {
    u8 flags;
    s8 lastPlayBitValue;
} SeqObj_Data;

typedef enum {
    SEQOBJ_FLAG_None = 0,
    SEQOBJ_FLAG_PlayedBefore = 1,
    SEQOBJ_FLAG_Replay = 2,
    SEQOBJ_FLAG_Anim_Callback_Ran = 4
} SeqObj_Flags;

static int SeqObj_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void SeqObj_ctor(void* dll) { }

// offset: 0xC | dtor
void SeqObj_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void SeqObj_obj_Setup(Object* self, SeqObj_Setup* objSetup, s32 reset) {
    SeqObj_Data* objData;

    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = SeqObj_animCallback;
    
    objData = self->data;

    self->modelInstIdx = objSetup->type;
    if (self->modelInstIdx >= self->def->numModels) {
        STUBBED_PRINTF("SEQOBJ.c: modelno out of range romdefno=%d\n", self->def->numModels);
        self->modelInstIdx = 0;
    }
    
    objAddObjectType(self, OBJTYPE_UseObj);
    
    objData->flags = 0;

    if (objSetup->gamebitHasPlayed != NO_GAMEBIT) {
        if (mainGetBits(objSetup->gamebitHasPlayed)) {
            objData->flags |= SEQOBJ_FLAG_PlayedBefore;
            if (objSetup->replayStartTime != 0) {
                // Seqobj has already played before and should replay on load. This is used to restore
                // game state that is controlled by sequences. For example, if a door is only opened
                // when a seq plays and that door should already be open if the seqobj played before,
                // the replay system can be used to start or instantly complete the seq to open the
                // door automatically.
                objData->flags |= SEQOBJ_FLAG_Replay;
            }
        }
    }
    
    objData->lastPlayBitValue = FALSE;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x124 | func: 1 | export: 1
void SeqObj_obj_Control(Object* self) {
    SeqObj_Data* objData;
    SeqObj_Setup* objSetup;
    s8 playBitValue;

    objData = self->data;
    objSetup = (SeqObj_Setup*)self->setup;
    
    if (objData->flags & SEQOBJ_FLAG_Anim_Callback_Ran) {
        if (objSetup->playbackOptions & SEQOBJ_OPTIONS_ManualHasPlayedBit) {
            if ((objSetup->playbackOptions & SEQOBJ_OPTIONS_ManualHasPlayed_Dont_Unset_Play_Gamebit) == FALSE) {
                mainSetBits(objSetup->gamebitPlay, FALSE);
            }
        } else {
            if (objSetup->playbackOptions & SEQOBJ_OPTIONS_AutoHasPlayed_Set_After_Sequence) {
                mainSetBits(objSetup->gamebitHasPlayed, TRUE);
            }
            objData->flags |= SEQOBJ_FLAG_PlayedBefore;
        }

        objData->flags &= ~SEQOBJ_FLAG_Anim_Callback_Ran;
    }
    
    if ((objData->flags & SEQOBJ_FLAG_PlayedBefore) == FALSE) {
        if (mainGetBits(objSetup->gamebitHasPlayed)) {
            // Played bit changed at runtime, swap state
            objData->flags |= SEQOBJ_FLAG_PlayedBefore;
        }

        // Wait for play gamebit to trigger seqobj
        playBitValue = mainGetBits(objSetup->gamebitPlay);
        if (playBitValue != objData->lastPlayBitValue) {
            objData->lastPlayBitValue = playBitValue;

            if (playBitValue) {
                // Start seq
                if (objSetup->seqIndex != -1) {
                    gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->seqIndex, self, -1);
                }
                // Mark seqobj as having been played at least once
                if ((objSetup->playbackOptions & SEQOBJ_OPTIONS_ManualHasPlayedBit) == FALSE && 
                    (objSetup->playbackOptions & (SEQOBJ_OPTIONS_ManualHasPlayed_Set_By_SeqCmd | SEQOBJ_OPTIONS_AutoHasPlayed_Set_After_Sequence)) == FALSE) {
                    mainSetBits(objSetup->gamebitHasPlayed, TRUE);
                }
            }
        }
    } else {
        // This seqobj has already played before. If it should replay, start the seq.
        if (objData->flags & SEQOBJ_FLAG_Replay) {
            gDLL_3_Animation->vtbl->preempt_sequence_time(self, objSetup->replayStartTime);
            if (objSetup->playbackOptions & SEQOBJ_OPTIONS_HasReplayActorMask) {
                gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->seqIndex, self, objSetup->replayActorMask);
            } else {
                gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->seqIndex, self, 1);
            }
            objData->flags &= ~SEQOBJ_FLAG_Replay;
            return;
        }

        if (objSetup->playbackOptions & SEQOBJ_OPTIONS_ManualHasPlayedBit) {
            if (mainGetBits(objSetup->gamebitHasPlayed) == FALSE) {
                // Played bit changed at runtime, swap state
                objData->flags &= ~SEQOBJ_FLAG_PlayedBefore;
            }
        }
    }
}

// offset: 0x3B8 | func: 2 | export: 2
void SeqObj_obj_Update(Object* self) { }

// offset: 0x3C4 | func: 3 | export: 3
void SeqObj_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x418 | func: 4 | export: 4
void SeqObj_obj_Free(Object* self, s32 onlySelf) {
    objFreeObjectType(self, OBJTYPE_UseObj);
}

// offset: 0x458 | func: 5 | export: 5
u32 SeqObj_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x468 | func: 6 | export: 6
u32 SeqObj_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(SeqObj_Data);
}

// offset: 0x47C | func: 7
static int SeqObj_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 prevCallbackValue) {
    SeqObj_Setup* objSetup;
    SeqObj_Data* objData;
    s32 index;

    if (self->seqSlot == SEQSLOT_NONE) {
        return 0;
    }
    
    objSetup = (SeqObj_Setup*)self->setup;
    objData = self->data;

    animObjData->unk62 = 0;
    for (index = 0; index < animObjData->messageCount; index++){
        switch (animObjData->messages[index]) {
            case 1:
                if (((objSetup->playbackOptions & SEQOBJ_OPTIONS_ManualHasPlayedBit) == FALSE) && 
                     (objSetup->playbackOptions & SEQOBJ_OPTIONS_ManualHasPlayed_Set_By_SeqCmd)) {
                    mainSetBits(objSetup->gamebitHasPlayed, TRUE);
                }
                break;
            case 2:
                if (objSetup->warpID) {
                    mapWarpPlayer(objSetup->warpID, 0);
                }
                break;
        }
    }
    
    objData->flags |= SEQOBJ_FLAG_Anim_Callback_Ran;
    return 0;
}

/*0x2C*/ static const char str_2C[] = "";
