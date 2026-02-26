#include "sys/main.h"
#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"

typedef struct {
/*00*/ s32 _unk0;
/*04*/ s32 opacityCounter;
} CFScalesGalleon_Data;

static int CFScalesGalleon_func_16C(Object *self, Object *animObj, AnimObj_Data *animObjData);

// offset: 0x0 | ctor
void CFScalesGalleon_ctor(void *dll) { }

// offset: 0xC | dtor
void CFScalesGalleon_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFScalesGalleon_setup(Object *self, ObjSetup *setup, s32 arg2) {
    u8 sp8[6] = { 0x03, 0x3d, 0x03, 0x37, 0x00, 0x96 };
    CFScalesGalleon_Data *objdata;

    objdata = self->data;
    objdata->opacityCounter = 30;
    self->opacityWithFade = OBJECT_OPACITY_MAX;
    self->animCallback = (AnimationCallback)CFScalesGalleon_func_16C;
    self->unkB0 |= 0x4000;
}

// offset: 0x80 | func: 1 | export: 1
void CFScalesGalleon_control(Object *self) {
    CFScalesGalleon_Data *objdata;

    objdata = self->data;
    if (objdata->opacityCounter != 0) {
        objdata->opacityCounter -= gUpdateRate;
        if (objdata->opacityCounter <= 0) {
            objdata->opacityCounter = 0;
        }
    }
}

// offset: 0xBC | func: 2 | export: 2
void CFScalesGalleon_update(Object *self) {
    CFScalesGalleon_Data *objdata;

    objdata = self->data;
    if (objdata->opacityCounter != 0) {
        self->opacity = ((30 - objdata->opacityCounter) * OBJECT_OPACITY_MAX) / 30;
    } else {
        self->opacity = OBJECT_OPACITY_MAX;
    }
}

// offset: 0x120 | func: 3 | export: 3
void CFScalesGalleon_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x138 | func: 4 | export: 4
void CFScalesGalleon_free(Object *self, s32 a1) { }

// offset: 0x148 | func: 5 | export: 5
u32 CFScalesGalleon_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x158 | func: 6 | export: 6
u32 CFScalesGalleon_get_data_size(Object *self, u32 a1) {
    return sizeof(CFScalesGalleon_Data);
}

// offset: 0x16C | func: 7
int CFScalesGalleon_func_16C(Object *self, Object *animObj, AnimObj_Data *animObjData) {
    CFScalesGalleon_Data *objdata;

    objdata = self->data;
    if (animObjData->unk98 != 0) {
        self->opacity = OBJECT_OPACITY_MAX;
    }
    if (objdata->opacityCounter != 0) {
        objdata->opacityCounter -= gUpdateRate;
        if (objdata->opacityCounter <= 0) {
            objdata->opacityCounter = 0;
        }
    }
    return 0;
}
