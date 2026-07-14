#include "common.h"
#include "game/objects/interaction_arrow.h"
#include "macros.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s16 gamebitB;
    s16 gamebitRestoreState;
    s16 preemptTime;
    s8 objSeqIdx;
    u8 yaw;
    u8 unk20;
    u8 scale;
    s16 gamebitC;
} DBBigNest_Setup;

typedef struct {
    u8 state;
    u8 sequenceNotStarted;
    u8 eggsPlaced;
} DBBigNest_Data;

typedef enum {
    DBBigNest_STATE_0,
    DBBigNest_STATE_1,
    DBBigNest_STATE_2,
    DBBigNest_STATE_3
} DBBigNest_States;

static int DBBigNest_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);
static void DBBigNest_handle_eggs(Object* self, u8* eggsPlaced, u8 maxEggs);

// offset: 0x0 | ctor
void DBBigNest_ctor(void* dll) { }

// offset: 0xC | dtor
void DBBigNest_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBBigNest_setup(Object* self, DBBigNest_Setup* objSetup, s32 reset) {
    DBBigNest_Data* objData = self->data;
    
    objData->sequenceNotStarted = TRUE;
    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = DBBigNest_anim_callback;
    
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

    //Restore state
    if (objSetup->gamebitRestoreState != NO_GAMEBIT) {
        objData->state = mainGetBits(objSetup->gamebitRestoreState);
    } else {
        objData->state = DBBigNest_STATE_0;
        self->opacity = 0;
    }
    
    objData->eggsPlaced = 0;
}

// offset: 0x128 | func: 1 | export: 1
void DBBigNest_control(Object* self) {
    DBBigNest_Data* objData;
    DBBigNest_Setup* objSetup;
    s32 actorsEnabled;

    objData = self->data;
    objSetup = (DBBigNest_Setup*)self->setup;
    
    actorsEnabled = -1;

    if (objData->sequenceNotStarted) {
        objData->sequenceNotStarted = FALSE;

        if (objSetup->preemptTime && (objData->state != DBBigNest_STATE_0)) {
            gDLL_3_Animation->vtbl->preempt_sequence_time(self, objSetup->preemptTime);
            actorsEnabled = 1;
        }

        if (objSetup->objSeqIdx != -1) {
            gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->objSeqIdx, self, actorsEnabled);
        }
    }

    if (mainGetBits(BIT_DB_Nest_Added_All_Eggs) == FALSE) {
        DBBigNest_handle_eggs(self, &objData->eggsPlaced, 2);
    }
}

// offset: 0x238 | func: 2 | export: 2
void DBBigNest_update(Object* self) { }

// offset: 0x244 | func: 3 | export: 3
void DBBigNest_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x298 | func: 4 | export: 4
void DBBigNest_free(Object* self, s32 onlySelf) { }

// offset: 0x2A8 | func: 5 | export: 5
u32 DBBigNest_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x2B8 | func: 6 | export: 6
u32 DBBigNest_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DBBigNest_Data);
}

// offset: 0x2CC | func: 7
int DBBigNest_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    DBBigNest_Data* objData;
    DBBigNest_Setup* objSetup;

    objData = self->data;
    objSetup = (DBBigNest_Setup*)self->setup;
    
    if (objData->state == DBBigNest_STATE_0) {
        if (mainGetBits(objSetup->gamebitB) && 
            ((objSetup->gamebitC == NO_GAMEBIT) || mainGetBits(objSetup->gamebitC))
        ) {
            objData->state = DBBigNest_STATE_2;
        }
    } else if (objData->state == DBBigNest_STATE_1) {
        if (mainGetBits(objSetup->gamebitB) == 0) {
            objData->state = DBBigNest_STATE_3;
            objData->eggsPlaced = 0;
        }
    }
    
    if (objData->state == DBBigNest_STATE_2) {
        if (animData->lastMessage == 2) {
            objData->state = DBBigNest_STATE_1;
            if (objSetup->gamebitRestoreState != NO_GAMEBIT) {
                mainSetBits(objSetup->gamebitRestoreState, DBBigNest_STATE_1);
            } 
        }
    } else if (objData->state == DBBigNest_STATE_3) {
        if (animData->lastMessage == 1) {
            objData->state = DBBigNest_STATE_0;
            if (objSetup->gamebitRestoreState != 1) { //@bug: should this be -1 (NO_GAMEBIT)?
                mainSetBits(objSetup->gamebitRestoreState, DBBigNest_STATE_0);
            } 
        }  
    } 
    
    if (mainGetBits(BIT_DB_Nest_Added_All_Eggs) == FALSE) {
        DBBigNest_handle_eggs(self, &objData->eggsPlaced, 2);
    }

    return (!(objData->state == DBBigNest_STATE_2) && !(objData->state == DBBigNest_STATE_3));
}

// offset: 0x49C | func: 8
void DBBigNest_handle_eggs(Object* self, u8* eggsPlaced, u8 maxEggs) {
    Object** objects;
    Object* obj;
    f32 distance;
    s32 count;
    s32 i;
    s32 matches;

    matches = 0;
    objects = objGetAllOfType(OBJTYPE_38, &count);
    
    for (i = 0; i < count; i++) {
        distance = vec3_distance_xz(&self->globalPosition, &objects[i]->globalPosition);
        if (distance < 45.0f) {
            if (objects[i]->srt.transl.y < self->srt.transl.y) {
                matches++;
                objects[i]->unkAF |= ARROW_FLAG_8_No_Targetting;
            }
        } else if (distance < 68.0f) {
            obj = objects[i];
            obj->srt.transl.x += (self->srt.transl.x - obj->srt.transl.x) * 0.05f;
            obj->srt.transl.z += (self->srt.transl.z - obj->srt.transl.z) * 0.05f;
        }
    }
    
    if ((u8)matches > *eggsPlaced) {
        STUBBED_PRINTF(" Egg Added ");
        mainSetBits(BIT_DB_Nest_Egg_Added, TRUE);
        *eggsPlaced += 1;
    } 
    
    STUBBED_PRINTF(" Egg Count %i ", (u8)matches);
    
    if (*eggsPlaced > (u8)matches) {
        STUBBED_PRINTF(" Egg taken Away ");
    }

    if (maxEggs == matches) {
        STUBBED_PRINTF(" Have Dropped off All the eggs ");
        mainSetBits(BIT_DB_Nest_Added_All_Eggs, TRUE);
    }
    
    *eggsPlaced = matches;
}
