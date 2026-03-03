#include "common.h"
#include "dlls/engine/17_partfx.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 sequenceIndex;
    s16 unused1A;
    s16 unused1C;
    s16 gamebit;
} NWtreebridge_Setup;

typedef struct {
    s16 gamebit;            //Tree falls when set
    s16 sequenceIndex;
    u16 seqValue;
    u8 felled;              //Boolean tracking whether tree should fall
    u8 searchAttempts;      //When nonzero, the control function searches for a nearby objType and decrements this
    Object* unkObj;         //Nearby object searched for by objType (unsure what it is: Chuka?)
} NWtreebridge_Data;

static s32 NWtreebridge_anim_callback(Object* self, Object* arg1, AnimObj_Data* arg2);

// offset: 0x0 | ctor
void NWtreebridge_ctor(void *dll) { }

// offset: 0xC | dtor
void NWtreebridge_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void NWtreebridge_setup(Object* self, NWtreebridge_Setup* objSetup, s32 arg2) {
    NWtreebridge_Data* objdata;

    objdata = self->data;
    obj_set_update_priority(self, 90);
    self->animCallback = (void*)&NWtreebridge_anim_callback;
    self->srt.yaw = objSetup->yaw << 8;
    objdata->sequenceIndex = objSetup->sequenceIndex;
    objdata->gamebit = objSetup->gamebit;
    if (main_get_bits(objdata->gamebit)) {
        objdata->seqValue = 380;
    }
    objdata->searchAttempts = 4;
}

// offset: 0xD4 | func: 1 | export: 1
void NWtreebridge_control(Object* self) {
    NWtreebridge_Data* objdata;
    f32 distance;
    s16 seqValue;

    objdata = self->data;
    distance = 100.0f;
    if (objdata->searchAttempts) {
        objdata->unkObj = obj_get_nearest_type_to(5, self, &distance);
        if (objdata->unkObj) {
            objdata->searchAttempts = 0;
            return;
        }
        objdata->searchAttempts--;
        return;
    }

    if (objdata->felled == FALSE) {
        seqValue = objdata->seqValue;
        if (seqValue) {
            gDLL_3_Animation->vtbl->func20(self, seqValue);
        }
        gDLL_3_Animation->vtbl->func17(objdata->sequenceIndex, self, -1);
    }
}

// offset: 0x1CC | func: 2 | export: 2
void NWtreebridge_update(Object *self) { }

// offset: 0x1D8 | func: 3 | export: 3
void NWtreebridge_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    NWtreebridge_Data* objdata;
    f32 x;
    f32 y;
    f32 z;

    objdata = self->data;
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
    if (objdata->unkObj) {
        func_80031F6C(self, 0, &x, &y, &z, 0);
        objdata->unkObj->srt.transl.x = x;
        objdata->unkObj->srt.transl.y = y;
        objdata->unkObj->srt.transl.z = z;
    }
}

// offset: 0x298 | func: 4 | export: 4
void NWtreebridge_free(Object *self, s32 a1) { }

// offset: 0x2A8 | func: 5 | export: 5
u32 NWtreebridge_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x2B8 | func: 6 | export: 6
u32 NWtreebridge_get_data_size(Object *self, u32 a1) {
    return sizeof(NWtreebridge_Data);
}

// offset: 0x2CC | func: 7
s32 NWtreebridge_anim_callback(Object* self, Object* arg1, AnimObj_Data* arg2) { //no arg3?
    s32 fxCount;
    s32 i;
    NWtreebridge_Data* objdata;
    u32 flags;
    u32 partFXArg4;

    objdata = self->data;
    arg2->unk62 = 0;
    if (objdata->felled == FALSE) {
        if (main_get_bits(objdata->gamebit)) {
            objdata->felled = TRUE;
        } else {
            return 1;
        }
    }

    for (i = 0; i < arg2->unk98; i++){
        flags = PARTFXFLAG_1;
        partFXArg4 = -1;
        switch (arg2->unk8E[i]) {
        case 1:
            fxCount = 200;
            do {
                gDLL_17_partfx->vtbl->spawn(self, 0xCC, 0, flags, partFXArg4, 0); 
            } while (--fxCount);
            break;
        case 2:
            fxCount = 100;
            if (self->id == OBJ_NWtreebridge) {
                do {
                    gDLL_17_partfx->vtbl->spawn(self, 0xD3, 0, flags, partFXArg4, 0); 
                } while (--fxCount);
            } else {
                if (objdata->sequenceIndex == 0){
                    do {
                        gDLL_17_partfx->vtbl->spawn(self, 0xCD, 0, flags, partFXArg4, 0); 
                    } while (--fxCount);
                } else if (objdata->sequenceIndex == 1){
                    do {
                        gDLL_17_partfx->vtbl->spawn(self, 0xCF, 0, flags, partFXArg4, 0); 
                    } while (--fxCount);
                }
            }
            break;
        case 3:
            fxCount = 5;
            if (self->id == OBJ_NWtreebridge) {
                do {
                    gDLL_17_partfx->vtbl->spawn(self, 0xD4, 0, flags, partFXArg4, 0); 
                } while (--fxCount);
            } else {
                if (objdata->sequenceIndex == 0) {
                    do {
                        gDLL_17_partfx->vtbl->spawn(self, 0xCE, 0, flags, partFXArg4, 0); 
                    } while (--fxCount);
                } else if (objdata->sequenceIndex == 1) {
                    do {
                        gDLL_17_partfx->vtbl->spawn(self, 0xD0, 0, flags, partFXArg4, 0); 
                    } while (--fxCount);
                }
            }
            break;
        }
    }
        
    return 0;
}
