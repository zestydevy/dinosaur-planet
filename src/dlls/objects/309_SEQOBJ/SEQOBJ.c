#include "common.h"
#include "dlls/objects/214_animobj.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s16 gamebitCheckPlay;
    s16 gamebitSetFinish;
    u8 rotate;
    u8 playbackOptions;
    s8 seqIndex;
    s8 unk1F;
    s16 unk20;
    u16 unk22;
    u8 warpID;
} SeqObj_Setup;

typedef struct {
    u8 flags;
    s8 unk1;
} SeqObj_Data;

static s32 SeqObj_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3);

// offset: 0x0 | ctor
void SeqObj_ctor(void *dll) { }

// offset: 0xC | dtor
void SeqObj_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SeqObj_setup(Object* self, SeqObj_Setup* objSetup, s32 arg2) {
    SeqObj_Data* objData;
    s16 temp_a0;

    self->srt.yaw = objSetup->rotate << 8;
    self->unk0xbc = (void*)&SeqObj_anim_callback;
    
    objData = self->data;
    self->modelInstIdx = objSetup->unk1F;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
    
    obj_add_object_type(self, 0x11);
    
    objData->flags = 0;
    if (objSetup->gamebitCheckPlay != -1) {

        if (main_get_bits(objSetup->gamebitCheckPlay)) {
            objData->flags |= 1;
            if (objSetup->unk20 != 0) {
                objData->flags |= 2;
            }
        }
    }
    objData->unk1 = 0;
    self->unk0xb0 |= 0x2000;
}

// offset: 0x124 | func: 1 | export: 1
void SeqObj_control(Object* self) {
    SeqObj_Data* objData;
    SeqObj_Setup* objSetup;
    s8 sequencePlayed;

    objData = self->data;
    objSetup = (SeqObj_Setup*)self->setup;
    
    if (objData->flags & 4) {
        if (objSetup->playbackOptions & 1) {
            if (!(objSetup->playbackOptions & 4)) {
                main_set_bits(objSetup->gamebitSetFinish, 0);
            }
        } else {
            if (objSetup->playbackOptions & 8) {
                main_set_bits(objSetup->gamebitCheckPlay, 1);
            }
            objData->flags |= 1;
        }

        objData->flags &= 0xFFFB;
    }
    
    if (!(objData->flags & 1)) {
        if (main_get_bits(objSetup->gamebitCheckPlay)) {
            objData->flags |= 1;
        }
        sequencePlayed = main_get_bits(objSetup->gamebitSetFinish);
        if ((objData->unk1 != ((0, sequencePlayed))) && ((objData->unk1 = sequencePlayed, sequencePlayed))) {
            if (objSetup->seqIndex != -1) {
                gDLL_3_Animation->vtbl->func17(objSetup->seqIndex, self, -1);
            }
            if (!(objSetup->playbackOptions & 1) && !(objSetup->playbackOptions & 0xA)) {
                main_set_bits(objSetup->gamebitCheckPlay, 1);
            }
        }
    } else {
        if (objData->flags & 2) {
            gDLL_3_Animation->vtbl->func20(self, objSetup->unk20);
            if (objSetup->playbackOptions & 0x10) {
                gDLL_3_Animation->vtbl->func17(objSetup->seqIndex, self, objSetup->unk22);
            } else {
                gDLL_3_Animation->vtbl->func17(objSetup->seqIndex, self, 1);
            }
            objData->flags &= 0xFFFD;
            return;
        }
        if (objSetup->playbackOptions & 1) {
            if (main_get_bits(objSetup->gamebitCheckPlay) == 0) {
                objData->flags &= 0xFFFE;
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
                if (!(objSetup->playbackOptions & 1) && (objSetup->playbackOptions & 2)) {
                    main_set_bits(objSetup->gamebitCheckPlay, 1);
                }
                break;
            case 2:
                if (objSetup->warpID) {
                    warpPlayer(objSetup->warpID, 0);
                }
                break;
        }
    }
    
    objData->flags |= 4;
    return 0;
}

/*0x0*/ static const char str_0[] = "SEQOBJ.c: modelno out of range romdefno=%d\n";
/*0x2C*/ static const char str_2C[] = "";
