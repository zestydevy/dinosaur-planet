#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "functions.h"
#include "dlls/objects/214_animobj.h"

typedef struct {
/*00*/ u8 unk0;
/*01*/ u8 unk1;
/*02*/ u8 _unk2[0x8-0x2];
} WGmutant_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 _unk18[0x1F-0x18];
/*1F*/ u8 yaw;
} WGmutant_Setup;

static int WGmutant_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, void *arg3);

// offset: 0x0 | ctor
void WGmutant_ctor(void *dll) { }

// offset: 0xC | dtor
void WGmutant_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WGmutant_setup(Object *self, WGmutant_Setup *setup, s32 arg2) {
    WGmutant_Data *objdata = self->data;
    objdata->unk0 = 0;
    objdata->unk1 = 1;
    self->srt.yaw = setup->yaw << 8;
    self->unkBC = (ObjectCallback)WGmutant_anim_callback;
}

// offset: 0x58 | func: 1 | export: 1
void WGmutant_control(Object *self) { }

// offset: 0x64 | func: 2 | export: 2
void WGmutant_update(Object *self) { }

// offset: 0x70 | func: 3 | export: 3
void WGmutant_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xC4 | func: 4 | export: 4
void WGmutant_free(Object *self, s32 arg1) { }

// offset: 0xD4 | func: 5 | export: 5
u32 WGmutant_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xE4 | func: 6 | export: 6
u32 WGmutant_get_data_size(Object *self, u32 arg1) {
    return sizeof(WGmutant_Data);
}

// offset: 0xF8 | func: 7
int WGmutant_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, void *arg3) {
    return 0;
}
