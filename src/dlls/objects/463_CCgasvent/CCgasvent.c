#include "PR/ultratypes.h"
#include "dlls/engine/6_amsfx.h"
#include "game/objects/object.h"
#include "sys/objtype.h"
#include "dll.h"
#include "constants.h"

typedef struct {
/*00*/ s32 soundHandle;
} CCgasvent_Data;

// offset: 0x0 | ctor
void CCgasvent_ctor(void *dll) { }

// offset: 0xC | dtor
void CCgasvent_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCgasvent_setup(Object *self, ObjSetup *objsetup, s32 arg2) {
    obj_add_object_type(self, OBJTYPE_64);
    self->unkB0 |= 0x2000;
}

// offset: 0x6C | func: 1 | export: 1
void CCgasvent_control(Object *self) {
    CCgasvent_Data *objdata = self->data;
    f32 distance = F32_MAX;

    obj_get_nearest_type_to(OBJTYPE_6, self, &distance);
    if (distance < 10.0f) {
        if (objdata->soundHandle) {
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
            objdata->soundHandle = 0;
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B96_Gasvent_Covered, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    } else {
        if (objdata->soundHandle == 0) {
            objdata->soundHandle = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B95_Gasvent_Uncovered, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_Gasvent_Gas, NULL, PARTFXFLAG_NONE, -1, NULL);
    }
}

// offset: 0x1E8 | func: 2 | export: 2
void CCgasvent_update(Object *self) { }

// offset: 0x1F4 | func: 3 | export: 3
void CCgasvent_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x20C | func: 4 | export: 4
void CCgasvent_free(Object *self, s32 arg1) {
    CCgasvent_Data *objdata = self->data;

    obj_free_object_type(self, OBJTYPE_64);
    if (objdata->soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
    }
}

// offset: 0x280 | func: 5 | export: 5
u32 CCgasvent_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x290 | func: 6 | export: 6
u32 CCgasvent_get_data_size(Object *self, u32 arg1) {
    return sizeof(CCgasvent_Data);
}
