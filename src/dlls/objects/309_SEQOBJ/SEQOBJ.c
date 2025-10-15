#include "common.h"
#include "dlls/objects/214_animobj.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s16 gamebitPlay;        //The sequence will play when this gamebit is set
    s16 gamebitFinished;    //This gamebit will be set when the sequence has played
    u8 rotate;
    u8 playbackOptions;
    s8 seqIndex;            //The index of the sequence in the Object.bin entry's sequence list
    s8 modelInstIdx;        //Choose between 3D models, visible when debugging (usually a clapperboard)
    s16 unk20;
    u16 unk22;
    u8 warpID;              //Optionally warp the player
} SeqObj_Setup;

typedef struct {
    u8 flags;
    s8 finished;
} SeqObj_Data;

typedef enum {
    SEQOBJ_FLAG_None = 0,
    SEQOBJ_FLAG_Playing = 1,
    SEQOBJ_FLAG_Unk_2 = 2, //finished?
    SEQOBJ_FLAG_Anim_Callback_Ran = 4
} SeqObj_Flags;

typedef enum {
    SEQOBJ_OPTIONS_None = 0,
    SEQOBJ_OPTIONS_Stoppable = 1,
    SEQOBJ_OPTIONS_2 = 2,
    SEQOBJ_OPTIONS_4 = 4,
    SEQOBJ_OPTIONS_8 = 8,
    SEQOBJ_OPTIONS_A = 16
} SeqObj_PlaybackOptions; //TO-DO: figure out what these do! Looping and auto-play may be options?

static s32 SeqObj_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3);

// offset: 0x0 | ctor
void SeqObj_ctor(void *dll) { }

// offset: 0xC | dtor
void SeqObj_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SeqObj_setup(Object* self, SeqObj_Setup* objSetup, s32 arg2) {
    SeqObj_Data* objData;

    self->srt.yaw = objSetup->rotate << 8;
    self->unk0xbc = (void*)&SeqObj_anim_callback;
    
    objData = self->data;
    self->modelInstIdx = objSetup->modelInstIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
    
    obj_add_object_type(self, 0x11);
    
    objData->flags = SEQOBJ_FLAG_None;
    if (objSetup->gamebitPlay != -1) {
        if (main_get_bits(objSetup->gamebitPlay)) {
            objData->flags |= SEQOBJ_FLAG_Playing;
            if (objSetup->unk20 != 0) {
                objData->flags |= SEQOBJ_FLAG_Unk_2;
            }
        }
    }
    objData->finished = FALSE;
    self->unk0xb0 |= 0x2000;
}

// offset: 0x124 | func: 1 | export: 1
void SeqObj_control(Object* self) {
    SeqObj_Data* objData;
    SeqObj_Setup* objSetup;
    s8 sequencePlayed;

    objData = self->data;
    objSetup = (SeqObj_Setup*)self->setup;
    
    //Check if the anim callback function successfully ran
    if (objData->flags & SEQOBJ_FLAG_Anim_Callback_Ran) {
        if (objSetup->playbackOptions & SEQOBJ_OPTIONS_Stoppable) {
            if ((objSetup->playbackOptions & SEQOBJ_OPTIONS_4) == FALSE) {
                main_set_bits(objSetup->gamebitFinished, FALSE);
            }
        } else {
            if (objSetup->playbackOptions & SEQOBJ_OPTIONS_8) {
                main_set_bits(objSetup->gamebitPlay, TRUE);
            }
            objData->flags |= SEQOBJ_FLAG_Playing;
        }

        objData->flags &= ~SEQOBJ_FLAG_Anim_Callback_Ran;
    }
    
    //If sequence isn't playing
    if ((objData->flags & SEQOBJ_FLAG_Playing) == FALSE) {
        //Start playing if "play" gamebit was set
        if (main_get_bits(objSetup->gamebitPlay)) {
            objData->flags |= SEQOBJ_FLAG_Playing;
        }

        //If "finished" gamebit is set, and objData->finished isn't set?
        if (objData->finished != (sequencePlayed = main_get_bits(objSetup->gamebitFinished)) && 
                (objData->finished = sequencePlayed, sequencePlayed)) {
            if (objSetup->seqIndex != -1) {
                gDLL_3_Animation->vtbl->func17(objSetup->seqIndex, self, -1);
            }
            if ((objSetup->playbackOptions & SEQOBJ_OPTIONS_Stoppable) == FALSE && 
                (objSetup->playbackOptions & (SEQOBJ_OPTIONS_2 | SEQOBJ_OPTIONS_8)) == FALSE) {
                main_set_bits(objSetup->gamebitPlay, TRUE);
            }
        }
    } else {
    //If sequence is playing
        if (objData->flags & SEQOBJ_FLAG_Unk_2) {
            gDLL_3_Animation->vtbl->func20(self, objSetup->unk20);
            if (objSetup->playbackOptions & SEQOBJ_OPTIONS_A) {
                gDLL_3_Animation->vtbl->func17(objSetup->seqIndex, self, objSetup->unk22);
            } else {
                gDLL_3_Animation->vtbl->func17(objSetup->seqIndex, self, 1);
            }
            objData->flags &= ~SEQOBJ_FLAG_Unk_2;
            return;
        }

        //If the sequence is stoppable and the "play" gamebit unsets, stop playing
        if (objSetup->playbackOptions & SEQOBJ_OPTIONS_Stoppable) {
            if (main_get_bits(objSetup->gamebitPlay) == FALSE) {
                objData->flags &= ~SEQOBJ_FLAG_Playing;
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
    obj_free_object_type(self, 0x11);
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
s32 SeqObj_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3) {
    SeqObj_Setup* objSetup;
    SeqObj_Data* objData;
    s32 index;

    if (self->unk0xb4 == -1) {
        return 0;
    }
    
    objSetup = (SeqObj_Setup*)self->setup;
    objData = self->data;

    animObjData->unk62 = 0;
    for (index = 0; index < animObjData->unk98; index++){
        switch (animObjData->unk8E[index]) {
            case 1:
                if (!(objSetup->playbackOptions & SEQOBJ_OPTIONS_Stoppable) && 
                     (objSetup->playbackOptions & SEQOBJ_OPTIONS_2)) {
                    main_set_bits(objSetup->gamebitPlay, 1);
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

/*0x0*/ static const char str_0[] = "SEQOBJ.c: modelno out of range romdefno=%d\n";
/*0x2C*/ static const char str_2C[] = "";
