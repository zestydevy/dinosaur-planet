//"EXPLODER.c" appears to be Rare's official filename, sourced from "default.dol"

#include "common.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s16 xMin;            //Random position variance for explosion particles
    s16 yMin;            //Random position variance for explosion particles
    s16 zMin;            //Random position variance for explosion particles
    s16 xMax;            //Random position variance for explosion particles
    s16 yMax;            //Random position variance for explosion particles
    s16 zMax;            //Random position variance for explosion particles
    s16 particleID;      //Type of particle to create for the explosion
    s16 unused26;
    s16 param0Max;       //Random variance for particle param 0
    s16 param1Max;       //Random variance for particle param 1
    u8 particleCount;    //Number of particles to create
    u8 pad2D;
    s16 param0Min;       //Random variance for particle param 0
    s16 param1Min;       //Random variance for particle param 1
    s16 gamebitExploded; //gamebit set when explosion has played out
    s16 gamebitExplodeTrigger;  //gamebit checked to trigger the explosion
} ExplodeAnimator_Setup;

typedef struct {
    s8 unused0;
    s8 unused1;
    u8 flags;
    s8 unused3;
} ExplodeAnimator_Data;

typedef enum {
    ExplodeAnimator_FLAG_Waiting_to_Explode = 0,
    ExplodeAnimator_FLAG_Finished = 1
} ExplodeAnimator_Flags;

static void ExplodeAnimator_create_partfx(Object* self, ExplodeAnimator_Setup* objSetup);

// offset: 0x0 | ctor
void ExplodeAnimator_ctor(void *dll) { }

// offset: 0xC | dtor
void ExplodeAnimator_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void ExplodeAnimator_setup(Object* self, ExplodeAnimator_Setup* objSetup, s32 arg2) {
    ExplodeAnimator_Data* objData;

    objData = self->data;
    
    if (main_get_bits(objSetup->gamebitExploded)) {
        objData->flags = ExplodeAnimator_FLAG_Finished;
    } else {
        objData->flags = ExplodeAnimator_FLAG_Waiting_to_Explode;
    }

    obj_add_object_type(self, OBJTYPE_28);
}

// offset: 0x94 | func: 1 | export: 1
void ExplodeAnimator_control(Object* self) {
    ExplodeAnimator_Data* objData;
    ExplodeAnimator_Setup* objSetup;

    objData = self->data;
    if (objData->flags & ExplodeAnimator_FLAG_Finished) {
        return;
    }
    
    objSetup = (ExplodeAnimator_Setup*)self->setup;
    if (main_get_bits(objSetup->gamebitExplodeTrigger)) {
        main_set_bits(objSetup->gamebitExploded, 1);
        objData->flags |= ExplodeAnimator_FLAG_Finished;
        ExplodeAnimator_create_partfx(self, objSetup);
    }
}

// offset: 0x140 | func: 2 | export: 2
void ExplodeAnimator_update(Object *self) { }

// offset: 0x14C | func: 3 | export: 3
void ExplodeAnimator_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x164 | func: 4 | export: 4
void ExplodeAnimator_free(Object *self, s32 arg1) {
    obj_free_object_type(self, OBJTYPE_28);
}

// offset: 0x1A4 | func: 5 | export: 5
u32 ExplodeAnimator_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1B4 | func: 6 | export: 6
u32 ExplodeAnimator_get_data_size(Object *self, u32 a1) {
    return sizeof(ExplodeAnimator_Data);
}

// offset: 0x1C8 | func: 7
void ExplodeAnimator_create_partfx(Object* self, ExplodeAnimator_Setup* objSetup) {
    s32 i;
    SRT transform;
    f32 params[2];

    for (i = 0; i < objSetup->particleCount; i++){
        params[0] = rand_next(objSetup->param0Min, objSetup->param0Max) * 0.01f;
        params[1] = rand_next(objSetup->param1Min, objSetup->param1Max) * 0.01f;
        transform.transl.x = rand_next(objSetup->xMin, objSetup->xMax);
        transform.transl.y = rand_next(objSetup->yMin, objSetup->yMax);
        transform.transl.z = rand_next(objSetup->zMin, objSetup->zMax);
        gDLL_17_partfx->vtbl->spawn(self, objSetup->particleID, &transform, 2, -1, &params);
    }
}
