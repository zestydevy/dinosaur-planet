#include "PR/ultratypes.h"
#include "functions.h"
#include "game/objects/object.h"
#include "sys/objtype.h"
#include "sys/main.h"
#include "dll.h"
#include "dlls/objects/214_animobj.h"
#include "game/gamebits.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 roll;
/*19*/ u8 pitch;
/*1A*/ u8 yaw;
} CCspellpage_Setup;

static int CCspellpage_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);

// offset: 0x0 | ctor
void CCspellpage_ctor(void *dll) { }

// offset: 0xC | dtor
void CCspellpage_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCspellpage_setup(Object *self, CCspellpage_Setup *setup, s32 arg2) {
    self->srt.yaw = setup->yaw << 8;
    self->srt.pitch = setup->pitch << 8;
    self->srt.roll = setup->roll << 8;
    self->animCallback = CCspellpage_anim_callback;
    obj_add_object_type(self, 5);
}

// offset: 0x90 | func: 1 | export: 1
void CCspellpage_control(Object *self) { }

// offset: 0x9C | func: 2 | export: 2
void CCspellpage_update(Object *self) { }

// offset: 0xA8 | func: 3 | export: 3
void CCspellpage_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xFC | func: 4 | export: 4
void CCspellpage_free(Object *self, s32 arg1) {
    obj_free_object_type(self, 5);
}

// offset: 0x13C | func: 5 | export: 5
u32 CCspellpage_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x14C | func: 6 | export: 6
u32 CCspellpage_get_data_size(Object *self, u32 arg1) {
    return 0;
}

// offset: 0x160 | func: 7
int CCspellpage_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    /*0x0*/ static f32 _data_0 = 0.0f;
    s32 i;

    for (i = 0; i < animObjData->unk98; i++) {
        if (animObjData->unk8E[i] == 1) {
            _data_0 = 100.0f;
            main_set_bits(BIT_Spell_Forcefield, 1);
        }
    }
    _data_0 -= gUpdateRateF;
    if (_data_0 > 0.0f) {
        gDLL_1_UI->vtbl->ui_func_130c(0x1D, 0x50, 0x3C);
    }

    return 0;
}
