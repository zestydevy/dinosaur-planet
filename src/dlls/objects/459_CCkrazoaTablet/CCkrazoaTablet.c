#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/objtype.h"
#include "sys/main.h"
#include "dll.h"
#include "dlls/objects/214_animobj.h"
#include "segment_334F0.h"
#include "functions.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw;
/*19*/ s8 objectSeqIndex;
/*1A*/ s8 _unk1A[0x1E - 0x1A];
/*1E*/ s16 gamebit;
} CCkrazoaTablet_Setup;

typedef struct {
/*00*/ u8 unk0;
/*04*/ f32 unk4;
} CCkrazoaTablet_Data;

static int CCkrazoaTablet_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);
static s32 CCkrazoaTablet_func_3F8(void);
static void CCkrazoaTablet_func_48C(Object *self, CCkrazoaTablet_Data *objdata);

// offset: 0x0 | ctor
void CCkrazoaTablet_ctor(void *dll) { }

// offset: 0xC | dtor
void CCkrazoaTablet_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCkrazoaTablet_setup(Object *self, CCkrazoaTablet_Setup *setup, s32 arg2) {
    CCkrazoaTablet_Data *objdata = self->data;

    self->animCallback = CCkrazoaTablet_anim_callback;
    self->unkB0 |= 0x6000;
    self->srt.yaw = setup->yaw << 8;
    if (main_get_bits(setup->gamebit)) {
        objdata->unk0 |= 1;
    } else {
        self->unkAF |= 8;
    }
}

// offset: 0xB4 | func: 1 | export: 1
void CCkrazoaTablet_control(Object *self) {
    CCkrazoaTablet_Setup *setup;
    CCkrazoaTablet_Data *objdata;

    setup = (CCkrazoaTablet_Setup*)self->setup;
    objdata = self->data;

    CCkrazoaTablet_func_48C(self, objdata);

    if (!(objdata->unk0 & 2)) {
        if (main_get_bits(BIT_3A9)) {
            objdata->unk0 |= 2;
            self->unkAF |= 8;
        } else {
            if (func_80032538(self) != 0) {
                gDLL_3_Animation->vtbl->func17(setup->objectSeqIndex + 3, obj_get_nearest_type_to(0x11, self, NULL), -1);
            }
        }
    }
}

// offset: 0x1BC | func: 2 | export: 2
void CCkrazoaTablet_update(Object *self) { }

// offset: 0x1C8 | func: 3 | export: 3
void CCkrazoaTablet_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x1E0 | func: 4 | export: 4
void CCkrazoaTablet_free(Object *self, s32 arg1) { }

// offset: 0x1F0 | func: 5 | export: 5
s32 CCkrazoaTablet_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x200 | func: 6 | export: 6
u32 CCkrazoaTablet_get_data_size(Object *self, u32 arg1) {
    return sizeof(CCkrazoaTablet_Data);
}

// offset: 0x214 | func: 7
int CCkrazoaTablet_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    s32 i;
    CCkrazoaTablet_Data *objdata;
    CCkrazoaTablet_Setup *setup;

    objdata = self->data;
    setup = (CCkrazoaTablet_Setup*)self->setup;

    CCkrazoaTablet_func_48C(self, objdata);

    for (i = 0; i < animObjData->unk98; i++) {
        switch (animObjData->unk8E[i]) {
            case 1:
                main_set_bits(setup->gamebit, 1);
                main_decrement_bits(BIT_CC_Krazoa_Tablets);
                self->unkAF &= ~8;
                objdata->unk4 = 0.0f;
                objdata->unk0 |= 1;
                if (CCkrazoaTablet_func_3F8()) {
                    animObjData->unk9D |= 4;
                    main_set_bits(BIT_3A9, 1);
                }
                break;
            case 2:
                main_set_bits(setup->gamebit, 0);
                main_increment_bits(BIT_CC_Krazoa_Tablets);
                self->unkAF |= 8;
                objdata->unk4 = 256.0f;
                objdata->unk0 &= ~1;
                break;
        }
    }

    return 0;
}

// offset: 0x3F8 | func: 8
s32 CCkrazoaTablet_func_3F8(void) {
    if ((!main_get_bits(BIT_3A9)) &&
        (main_get_bits(BIT_CC_Placed_Krazoa_Tablet_One)) &&
        (main_get_bits(BIT_CC_Placed_Krazoa_Tablet_Two)) &&
        (main_get_bits(BIT_CC_Placed_Krazoa_Tablet_Three))) {
        return 1;
    }
    return 0;
}

// offset: 0x48C | func: 9
void CCkrazoaTablet_func_48C(Object *self, CCkrazoaTablet_Data *objdata) {
    s32 *temp_v0;

    if (objdata->unk0 & 1) {
        objdata->unk4 += gUpdateRateF * 5.0f;
        if (objdata->unk4 > 512.0f) {
            objdata->unk4 -= 512.0f;
        }
    } else {
        objdata->unk4 -= gUpdateRateF * 5.0f;
        if (objdata->unk4 < 0.0f) {
            objdata->unk4 = 0.0f;
        }
    }

    temp_v0 = func_800348A0(self, 0, 0);

    if (objdata->unk4 >= 256.5f) {
        *temp_v0 = (512.0f - objdata->unk4);
    } else {
        *temp_v0 = objdata->unk4;
    }
}
