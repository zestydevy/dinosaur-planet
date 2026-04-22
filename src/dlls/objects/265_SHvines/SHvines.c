#include "common.h"
#include "macros.h"
#include "sys/objtype.h"
#include "dlls/engine/53.h"
#include "dlls/objects/common/sidekick.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s16 unused1A;
    s16 flameDistance;
    s16 gamebitBurnt;
} SHvines_Setup;

typedef struct {
    s32 vertexCount;
} SHvines_Data;

// offset: 0x0 | ctor
void SHvines_ctor(void *dll) { }

// offset: 0xC | dtor
void SHvines_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHvines_setup(Object* self, SHvines_Setup* objSetup, s32 arg2) {
    SHvines_Data* objData;
    ModelInstance* modelInstance;
    Model* model;

    objData = self->data;
    obj_add_object_type(self, OBJTYPE_51);

    if (main_get_bits(objSetup->gamebitBurnt)) {
        self->srt.flags |= OBJFLAG_INVISIBLE;
        obj_free_tick(self);
        func_800267A4(self);
    }
    
    self->srt.yaw = objSetup->yaw << 8;
    
    create_temp_dll(DLL_ID_53);

    modelInstance = self->modelInsts[self->modelInstIdx];
    model = modelInstance->model;
    objData->vertexCount = model->vertexCount;
}

// offset: 0x104 | func: 1 | export: 1
void SHvines_control(Object* self) {
    SHvines_Setup* objSetup;
    Object* sidekick;

    objSetup = (SHvines_Setup*)self->setup;
    sidekick = get_sidekick();
    
    //Allow the Flame command when nearby
    if (sidekick != NULL) {
        if (vec3_distance_squared(&self->globalPosition, &get_player()->globalPosition) <= SQ(objSetup->flameDistance)) {
            ((DLL_ISidekick*)sidekick->dll)->vtbl->func14(sidekick, Sidekick_Command_INDEX_4_Flame);
        }
    }

    //Fade out
    if (self->opacity < OBJECT_OPACITY_MAX) {
        if (self->opacity < gUpdateRate) {
            obj_free_tick(self);
            self->srt.flags |= OBJFLAG_INVISIBLE;
            func_800267A4(self);
            return;
        }
        
        self->opacity -= gUpdateRate;
    }
}

// offset: 0x22C | func: 2 | export: 2
void SHvines_update(Object* self) {
    s32 pad;
    s32 sp28;
    SHvines_Setup* objSetup;

    objSetup = (SHvines_Setup*)self->setup;

    //Check for Flame command collisions
    if (func_80025F40(self, NULL, NULL, &sp28) == Damage_Type_Flame_Command) {
        STUBBED_PRINTF("\n HIT HIT HIT HIT Hit by Booldy Side Kick \n\n\n");
        main_set_bits(objSetup->gamebitBurnt, 1);
        self->opacity = 0xFD; //start fade out
    }
}

// offset: 0x2A4 | func: 3 | export: 3
void SHvines_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    SHvines_Data* objData;
    SRT srt;
    Vec3f *transl;
    s32 i;

    objData = self->data;
    transl = &srt.transl;
    
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        
        if (self->opacity < OBJECT_OPACITY_MAX) {
            srt.scale = self->unkA8 * 0.03f;
            for (i = 0; i < gUpdateRate; i++) {
                ((DLL_53*)gTempDLLInsts[1])->vtbl->func12(self, rand_next(0, objData->vertexCount - 1), transl);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_672, &srt, 0x200001, -1, NULL);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_673, &srt, 0x200001, -1, NULL);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_674, &srt, 0x200001, -1, NULL);
            }
        }
    }
}

// offset: 0x470 | func: 4 | export: 4
void SHvines_free(Object* self, s32 arg1) {
    obj_free_object_type(self, OBJTYPE_51);
    remove_temp_dll(DLL_ID_53);
}

// offset: 0x4C4 | func: 5 | export: 5
u32 SHvines_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x4D4 | func: 6 | export: 6
u32 SHvines_get_data_size(Object *self, u32 a1) {
    return sizeof(SHvines_Data);
}
