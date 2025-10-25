#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll.h"
#include "dlls/modgfx/149.h"
#include "dlls/modgfx/155.h"
#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "sys/dll.h"
#include "sys/main.h"
#include "game/gamebits.h"
#include "sys/rand.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "types.h"
#include "functions.h"
#include "sys/gfx/model.h"

/*0x0*/ static DLL_155 *_data_0 = NULL;
/*0x4*/ static DLL_149 *_data_4 = NULL;

typedef struct {
/*00*/ s32 unk0;
/*04*/ s16 effectTimer;
/*06*/ s16 unk6;
/*08*/ s16 bit2;
/*0A*/ s16 bit1;
/*0C*/ s16 mode;
/*10*/ f32 unk10;
} WL_spiritplace_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw;
/*19*/ s8 mode;
/*1A*/ s16 pitch;
/*1C*/ s16 unk1C;
/*1E*/ s16 bit1;
/*20*/ s16 bit2;
} WL_spiritplace_Setup;

static s32 WL_spiritplace_func_5C4(Object *self, Object *arg1, AnimObj_Data *arg2, void *arg3);

// offset: 0x0 | ctor
void WL_spiritplace_ctor(void *dll) { }

// offset: 0xC | dtor
void WL_spiritplace_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_spiritplace_setup(Object *self, WL_spiritplace_Setup *objsetup, s32 arg2) {
    WL_spiritplace_Data *objdata = self->data;

    self->unkBC = (ObjectCallback)WL_spiritplace_func_5C4;
    self->srt.yaw = objsetup->yaw << 8;
    self->srt.pitch = objsetup->pitch << 8;
    objdata->unk10 = (objsetup->unk1C / 32767.0f) / 100.0f;
    objdata->unk0 = 0;
    objdata->effectTimer = 0;
    objdata->unk6 = 0;
    objdata->bit1 = objsetup->bit1;
    objdata->bit2 = objsetup->bit2;
    objdata->mode = objsetup->mode;
    if (objdata->mode == 0) {
        _data_0 = dll_load_deferred(DLL_ID_155, 1);
        _data_4 = dll_load_deferred(DLL_ID_149, 1);
    } else if (objdata->mode == 2) {
        _data_0 = dll_load_deferred(DLL_ID_155, 1);
    }
}

// offset: 0x130 | func: 1 | export: 1
void WL_spiritplace_control(Object *self) {
    WL_spiritplace_Data *objdata = self->data;

    if (main_get_bits(BIT_WM_Spirit_Release_Effect) && objdata->mode == 0) {
        if (objdata->effectTimer <= 0) {
            if (objdata->unk6 == 0) {
                _data_0->vtbl->base.func0(self, 4, NULL, 4, -1, &objdata->unk10);
            } else if (objdata->unk6 == 4) {
                _data_0->vtbl->base.func0(self, 0, NULL, 4, -1, &objdata->unk10);
                _data_0->vtbl->base.func0(self, 1, NULL, 4, -1, &objdata->unk10);
                _data_0->vtbl->base.func0(self, 2, NULL, 4, -1, &objdata->unk10);
                _data_0->vtbl->base.func0(self, 3, NULL, 4, -1, &objdata->unk10);
            }
            objdata->unk6++;
            if (objdata->unk6 >= 9) {
                objdata->unk6 = 0;
            }
            objdata->effectTimer = 15;
        }
        objdata->effectTimer -= delayByte;
    } else if (main_get_bits(BIT_WM_Spirit_Release_Effect) && objdata->mode == 2) {
        if (objdata->effectTimer <= 0) {
            _data_0->vtbl->base.func0(self, 4, NULL, 4, -1, &objdata->unk10);
            objdata->effectTimer = 195 + rand_next(0, 35);
        }
        objdata->effectTimer -= delayByte;
    } else if (
        main_get_bits(objdata->bit1) &&
        objdata->mode == 0 &&
        rand_next(1, 2) == 2 &&
        vec3_distance(&get_player()->positionMirror, &self->positionMirror) < 90.0f) {
            _data_4->vtbl->base.func0(self, 4, NULL, 1, -1, NULL);
    }
    if (main_get_bits(objdata->bit2) && objdata->unk0 == 0) {
        self->srt.roll += 200;
    }
}

// offset: 0x4BC | func: 2 | export: 2
void WL_spiritplace_update(Object *self) { }

// offset: 0x4C8 | func: 3 | export: 3
void WL_spiritplace_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x51C | func: 4 | export: 4
void WL_spiritplace_free(Object *self, s32 arg1) {
    WL_spiritplace_Data *objdata = self->data;
    if (objdata->mode == 0) {
        dll_unload(_data_0);
        dll_unload(_data_4);
    } else if (objdata->mode == 0) { // @bug: should be 2
        dll_unload(_data_0);
    }
}

// offset: 0x5A0 | func: 5 | export: 5
s32 WL_spiritplace_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x5B0 | func: 6 | export: 6
u32 WL_spiritplace_get_data_size(Object *self, u32 arg1) {
    return sizeof(WL_spiritplace_Data);
}

// offset: 0x5C4 | func: 7
s32 WL_spiritplace_func_5C4(Object *self, Object *arg1, AnimObj_Data *arg2, void *arg3) {
    arg2->unk7A = -1;
    arg2->unk62 = 0;
    return 0;
}
