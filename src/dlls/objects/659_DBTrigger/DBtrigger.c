#include "common.h"

typedef struct {
    ObjSetup base;
    s16 unused18;
    s16 unused1A;
    s16 unused1C;
    s16 gamebit;
} DBTrigger_Setup;

typedef struct {
    Vec3f boundsMin;
    Vec3f boundsMax;
    u8 pressed;     //set to 1 when the switch is pressed down by the player or the sidekick
    s8 unk19;       //set to 0 during setup, but otherwise unused?
} DBTrigger_Data;

static s32 DBTrigger_get_model_bounds(Object* self, Vec3f* min, Vec3f* max);
static int DBTrigger_anim_callback(Object* self, Object* arg1, AnimObj_Data* arg2, s8 arg3);

// offset: 0x0 | ctor
void DBTrigger_ctor(void *dll) { }

// offset: 0xC | dtor
void DBTrigger_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBTrigger_setup(Object* self, DBTrigger_Setup* objSetup, s32 arg2) {
    DBTrigger_Data* objData = self->data;

    objData->pressed = 0;
    objData->unk19 = 0;
    self->animCallback = DBTrigger_anim_callback;
    self->objhitInfo->unk58 |= 1;
    DBTrigger_get_model_bounds(self, &objData->boundsMin, &objData->boundsMax);
}


// offset: 0x84 | func: 1 | export: 1
void DBTrigger_control(Object* self) {
    s16 sequenceIndices[2] = {1, 0};
    DBTrigger_Data* objData;
    DBTrigger_Setup* objSetup;
    Object* player;
    Object* sidekick;
    s32 seqIndex;
    f32 dx;
    f32 dz;
    f32 delta;

    objData = self->data;
    seqIndex = 0;
    
    //Check if the sidekick is inside the object's bounds
    sidekick = get_sidekick();
    if (sidekick != NULL) {
        if (self->srt.transl.x <= sidekick->srt.transl.x) {
            delta = sidekick->srt.transl.x - self->srt.transl.x;
        } else {
            delta = -(sidekick->srt.transl.x - self->srt.transl.x);
        }
        dx = fsin16_precise(self->srt.yaw) * delta;
        
        if (self->srt.transl.z <= sidekick->srt.transl.z) {
            delta = sidekick->srt.transl.z - self->srt.transl.z;
        } else {
            delta = -(sidekick->srt.transl.z - self->srt.transl.z);
        }
        dz = fcos16_precise(self->srt.yaw) * delta;
        
        if (dx < objData->boundsMin.x && dz < objData->boundsMin.z) {
            seqIndex = 1;
        }
    }
    
    //Check if the player is inside the object's bounds
    player = get_player();
    if (player != NULL) {
        if (self->srt.transl.x <= player->srt.transl.x) {
            delta = player->srt.transl.x - self->srt.transl.x;
        } else {
            delta = -(player->srt.transl.x - self->srt.transl.x);
        }
        dx = fsin16_precise(self->srt.yaw) * delta;
        
        if (self->srt.transl.z <= player->srt.transl.z) {
            delta = player->srt.transl.z - self->srt.transl.z;
        } else {
            delta = -(player->srt.transl.z - self->srt.transl.z);
        }
        dz = fcos16_precise(self->srt.yaw) * delta;
        
        if (dx < objData->boundsMin.x && dz < objData->boundsMin.z) {
            seqIndex = 1;
        }
    }
    
    //Play a sequence to move the switch up/down
    if (seqIndex != objData->pressed) {
        objData->pressed = seqIndex;
        objSetup = (DBTrigger_Setup*)self->setup;
        if (seqIndex != 0) {
            main_set_bits(objSetup->gamebit, main_get_bits(objSetup->gamebit) ^ 1);
        }
        gDLL_3_Animation->vtbl->func17(sequenceIndices[objData->pressed], self, -1);
    }
}

// offset: 0x330 | func: 2 | export: 2
void DBTrigger_update(Object *self) { }

// offset: 0x33C | func: 3 | export: 3
void DBTrigger_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x390 | func: 4 | export: 4
void DBTrigger_free(Object *self, s32 a1) { }

// offset: 0x3A0 | func: 5 | export: 5
u32 DBTrigger_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x3B0 | func: 6 | export: 6
u32 DBTrigger_get_data_size(Object *self, u32 a1) {
    return sizeof(DBTrigger_Data);
}

// offset: 0x3C4 | func: 7
int DBTrigger_anim_callback(Object* self, Object* arg1, AnimObj_Data* arg2, s8 arg3) {
    DBTrigger_Data* objData;
    SRT fxTrans;
    f32 absZ;
    f32 absX;

    objData = self->data;
    if (objData->pressed) {
        if (rand_next(0, 2)) {
            fxTrans.transl.x = rand_next(-objData->boundsMin.x, objData->boundsMin.x);
            fxTrans.transl.z = rand_next(-objData->boundsMin.z, objData->boundsMin.z);
            if (fxTrans.transl.x >= 0.0f) {
                absX = fxTrans.transl.x;
            } else {
                absX = -fxTrans.transl.x;
            }
            
            if (fxTrans.transl.z >= 0.0f) {
                absZ = fxTrans.transl.z;
            } else {
                absZ = -fxTrans.transl.z;
            }
            
            if (absZ < absX) {
                if (fxTrans.transl.x > 0.0f) {
                    fxTrans.transl.x = objData->boundsMin.x;
                } else {
                    fxTrans.transl.x = -objData->boundsMin.x;
                }
            } else {
                if (fxTrans.transl.z > 0.0f) {
                    fxTrans.transl.z = objData->boundsMin.z;
                } else {
                    fxTrans.transl.z = -objData->boundsMin.z;
                }
            }
            
            gDLL_17_partfx->vtbl->spawn(self, 0x3B6, &fxTrans, 0, -1, 0);
        }
    }

    return 0;
}

// offset: 0x57C | func: 8
/** Queries the max/min XYZ values in the model's verts, and transforms the values into worldSpace */
s32 DBTrigger_get_model_bounds(Object* self, Vec3f* min, Vec3f* max) {
    s16 *componentPtr;
    s32 vertexIndex;
    s32 compIdx;
    Model *model;
    ModelInstance *modelInstance;
    
    modelInstance = self->modelInsts[0];
    model = modelInstance->model;
    
    componentPtr = (s16*)model->vertices;
    
    //Initialise values using the first vertex
    for (compIdx = 0; compIdx < 3; compIdx++) {
        min->f[compIdx] = componentPtr[compIdx];
        max->f[compIdx] = componentPtr[compIdx];
    }

    //Compare min/max component values in the remaining vertices
    for (vertexIndex = 1; vertexIndex < model->vertexCount; vertexIndex++) {
        componentPtr = (s16*)&model->vertices[vertexIndex];
        for (compIdx = 0; compIdx < 3; compIdx++){
            if (min->f[compIdx] < componentPtr[compIdx]) {
                min->f[compIdx] = componentPtr[compIdx];
            }
            if (max->f[compIdx] > componentPtr[compIdx]) {
                max->f[compIdx] = componentPtr[compIdx];
            }
        }
    }

    STUBBED_PRINTF(" Max X %f Y %f Z %f ");
    
    //Scale values wrt. overall object scale
    for (vertexIndex = 0; vertexIndex < 3; vertexIndex++) {
        min->f[vertexIndex] *= self->srt.scale;
        max->f[vertexIndex] *= self->srt.scale;
    }
    
    STUBBED_PRINTF("o");
    
    return 0;
}
