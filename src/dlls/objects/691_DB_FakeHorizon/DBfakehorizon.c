#include "common.h"
#include "macros.h"
#include "sys/objprint.h"

typedef struct {
    ObjSetup base;
    s16 unused18;
    s16 divisor;
} DBFakeHorizon_Setup;

typedef struct {
    s32 vtxIDs[10];     //Stores the vertexIDs of all the model vertices that have zero opacity
    s32 vtxCount;       //The number of vertexIDs stored in the vtxIDs array
} DBFakeHorizon_Data;

// offset: 0x0 | ctor
void DBFakeHorizon_ctor(void *dll) { }

// offset: 0xC | dtor
void DBFakeHorizon_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBFakeHorizon_setup(Object* self, DBFakeHorizon_Setup* objSetup, s32 arg2) {
    DBFakeHorizon_Data* objData;
    ModelInstance* modelInstance;
    Model* model;
    Vtx_t* vertices;
    s32 i;

    objData = self->data;

    modelInstance = self->modelInsts[self->modelInstIdx];
    model = modelInstance->model;
    
    vertices = (Vtx_t*)model->vertices;

    for (i = 0; i < model->vertexCount; i++, vertices++){
        if (vertices->cn[3] == 0) {
            STUBBED_PRINTF(" Found Vertex With No Alpha %i ");
            objData->vtxIDs[objData->vtxCount] = i;
            objData->vtxCount++;
        }
    }
    
    self->unkB0 |= 0xA000;
}

// offset: 0xA0 | func: 1 | export: 1
void DBFakeHorizon_control(Object *self) { }

// offset: 0xAC | func: 2 | export: 2
void DBFakeHorizon_update(Object *self) { }

// offset: 0xB8 | func: 3 | export: 3
void DBFakeHorizon_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    MtxF mtx;
    f32 dy;
    f32 scaleY;
    DBFakeHorizon_Setup *objSetup;
    
    if (!visibility){
        return;
    }
    
    objSetup = (DBFakeHorizon_Setup*)self->setup;

    dy = get_main_camera()->srt.transl.y - self->srt.transl.y;
    if (dy < 0.0f){
        return;
    }
    
    if (objSetup->divisor == 0){
        return;
    }

    scaleY = dy / objSetup->divisor;
    matrix_scaling(&mtx, self->srt.scale, self->srt.scale * scaleY, self->srt.scale);
    mtx.m[3][0] = self->srt.transl.x - gWorldX;
    mtx.m[3][1] = self->srt.transl.y;
    mtx.m[3][2] = self->srt.transl.z - gWorldZ;
    func_80034FF0(&mtx);
    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    func_80034FF0(NULL);
}

// offset: 0x200 | func: 4 | export: 4
void DBFakeHorizon_free(Object *self, s32 a1) { }

// offset: 0x210 | func: 5 | export: 5
u32 DBFakeHorizon_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x220 | func: 6 | export: 6
u32 DBFakeHorizon_get_data_size(Object *self, u32 a1){
    return sizeof(DBFakeHorizon_Data);
}
