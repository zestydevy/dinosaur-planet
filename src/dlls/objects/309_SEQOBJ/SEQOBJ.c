#include "common.h"
#include "macros.h"
#include "sys/gfx/animseq.h"
#include "sys/objtype.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebitFinished;
/*1A*/ s16 gamebitPlay;
/*1C*/ u8 yaw;
/*1D*/ u8 playbackOptions;
/*1E*/ s8 seqIndex;            //The index of the sequence in the Object.bin entry's sequence list
/*1F*/ s8 modelInstIdx;        //Choose between 3D models, visible when debugging (usually a clapperboard)
/*20*/ s16 unk20;
/*22*/ u16 unk22;
/*24*/ u8 warpID;              //Optionally warp the player
} SeqObj_Setup;

typedef struct {
    u8 flags;
    s8 prevGamebitPlayValue;
} SeqObj_Data;

typedef enum {
    SEQOBJ_FLAG_None = 0,
    SEQOBJ_FLAG_Has_Played = 1,
    SEQOBJ_FLAG_Unk_2 = 2, //finished?
    SEQOBJ_FLAG_Anim_Callback_Ran = 4
} SeqObj_Flags;

typedef enum {
    SEQOBJ_OPTIONS_None = 0,
    SEQOBJ_OPTIONS_1 = 1,
    SEQOBJ_OPTIONS_2_Set_GamebitFinished_via_SeqCommand = 2,        //gamebitFinished intended to be set via a seqCommand?
    SEQOBJ_OPTIONS_4_Retain_GamebitPlay_Value = 4,                  //Avoids setting gamebitPlay back to 0 after sequence finishes, under some circumstances?
    SEQOBJ_OPTIONS_8_Set_GamebitFinished_After_Sequence_Ends = 8,   //Set gamebitFinished after sequences finishes, instead of immediately?
    SEQOBJ_OPTIONS_10 = 0x10                                        //Related to `preempt_sequence_time` block, decides arg2 for `start_obj_sequence`
} SeqObj_PlaybackOptions; //TO-DO: figure out what these do! Looping and auto-play may be options?

static int SeqObj_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3);

// offset: 0x0 | ctor
void SeqObj_ctor(void *dll) { }

// offset: 0xC | dtor
void SeqObj_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SeqObj_setup(Object* self, SeqObj_Setup* objSetup, s32 arg2) {
    SeqObj_Data* objData;

    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = SeqObj_anim_callback;
    
    objData = self->data;

    self->modelInstIdx = objSetup->modelInstIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        STUBBED_PRINTF("SEQOBJ.c: modelno out of range romdefno=%d\n", self->def->numModels);
        self->modelInstIdx = 0;
    }
    
    obj_add_object_type(self, OBJTYPE_17);
    
    objData->flags = SEQOBJ_FLAG_None;
    if ((objSetup->gamebitFinished != NO_GAMEBIT) && main_get_bits(objSetup->gamebitFinished)) {
        objData->flags |= SEQOBJ_FLAG_Has_Played;
        if (objSetup->unk20) {
            objData->flags |= SEQOBJ_FLAG_Unk_2;
        }
    }

    objData->prevGamebitPlayValue = FALSE;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x124 | func: 1 | export: 1
void SeqObj_control(Object* self) {
    SeqObj_Data* objData;
    SeqObj_Setup* objSetup;
    s8 gamebitPlayValue;

    objData = self->data;
    objSetup = (SeqObj_Setup*)self->setup;
    
    //Check if the anim callback function ran
    if (objData->flags & SEQOBJ_FLAG_Anim_Callback_Ran) {
        if (objSetup->playbackOptions & SEQOBJ_OPTIONS_1) {
            if ((objSetup->playbackOptions & SEQOBJ_OPTIONS_4_Retain_GamebitPlay_Value) == FALSE) {
                main_set_bits(objSetup->gamebitPlay, FALSE);
            }
        } else {
            if (objSetup->playbackOptions & SEQOBJ_OPTIONS_8_Set_GamebitFinished_After_Sequence_Ends) {
                main_set_bits(objSetup->gamebitFinished, TRUE);
            }
            objData->flags |= SEQOBJ_FLAG_Has_Played;
        }

        objData->flags &= ~SEQOBJ_FLAG_Anim_Callback_Ran;
    }
    
    //If sequence hasn't played yet
    if ((objData->flags & SEQOBJ_FLAG_Has_Played) == FALSE) {

        if (main_get_bits(objSetup->gamebitFinished)) {
            objData->flags |= SEQOBJ_FLAG_Has_Played;
        }

        //Check if the "play" gamebit's value has changed
        gamebitPlayValue = main_get_bits(objSetup->gamebitPlay);
        if (gamebitPlayValue != objData->prevGamebitPlayValue) {
            //Store the new "play" value, and return if the sequence shouldn't play
            objData->prevGamebitPlayValue = gamebitPlayValue;
            if (gamebitPlayValue == FALSE) {
                return;
            }
            
            //Start playing sequence
            if (objSetup->seqIndex != -1) {
                gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->seqIndex, self, -1);
            }

            //Set gamebitFinished immediately (conditionally)
            if ((objSetup->playbackOptions & SEQOBJ_OPTIONS_1) == FALSE && 
                (objSetup->playbackOptions & (SEQOBJ_OPTIONS_2_Set_GamebitFinished_via_SeqCommand | SEQOBJ_OPTIONS_8_Set_GamebitFinished_After_Sequence_Ends)) == FALSE) {
                main_set_bits(objSetup->gamebitFinished, TRUE);
            }
        }

    //Sequence has played
    } else {

        if (objData->flags & SEQOBJ_FLAG_Unk_2) {
            gDLL_3_Animation->vtbl->preempt_sequence_time(self, objSetup->unk20);
            if (objSetup->playbackOptions & SEQOBJ_OPTIONS_10) {
                gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->seqIndex, self, objSetup->unk22);
            } else {
                gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->seqIndex, self, 1);
            }
            objData->flags &= ~SEQOBJ_FLAG_Unk_2;
            return;
        }

        if (objSetup->playbackOptions & SEQOBJ_OPTIONS_1) {
            if (main_get_bits(objSetup->gamebitFinished) == FALSE) {
                objData->flags &= ~SEQOBJ_FLAG_Has_Played;
            }
        }
    }
}

// offset: 0x3B8 | func: 2 | export: 2
void SeqObj_update(Object *self) { }

// offset: 0x3C4 | func: 3 | export: 3
void SeqObj_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x418 | func: 4 | export: 4
void SeqObj_free(Object *self, s32 arg1) {
    obj_free_object_type(self, OBJTYPE_17);
}

// offset: 0x458 | func: 5 | export: 5
u32 SeqObj_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x468 | func: 6 | export: 6
u32 SeqObj_get_data_size(Object *self, u32 a1) {
    return sizeof(SeqObj_Data);
}

// offset: 0x47C | func: 7
static int SeqObj_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
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
                if (((objSetup->playbackOptions & SEQOBJ_OPTIONS_1) == FALSE) && 
                     (objSetup->playbackOptions & SEQOBJ_OPTIONS_2_Set_GamebitFinished_via_SeqCommand)) {
                    main_set_bits(objSetup->gamebitFinished, TRUE);
                }
                break;
            case 2:
                if (objSetup->warpID) {
                    warpPlayer(objSetup->warpID, 0);
                }
                break;
        }
    }
    
    objData->flags |= SEQOBJ_FLAG_Anim_Callback_Ran;
    return 0;
}

/*0x2C*/ static const char str_2C[] = "";
