#include "dlls/objects/210_player.h"
#include "dlls/objects/214_animobj.h"
#include "sys/gfx/gx.h"
#include "sys/gfx/model.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "sys/print.h"
#include "dll.h"
#include "functions.h"

typedef struct {
/*0*/ s16 targetOpacity;
/*2*/ u8 state;
/*3*/ u8 unk3;
} WCApertureSymbol_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 unk18;
/*19*/ s8 unk19;
/*1A*/ s16 unk1A;
/*1C*/ u8 _unk1C[0x1E - 0x1C];
/*1E*/ s16 unk1E;
/*20*/ s16 unk20;
} WCApertureSymbol_Setup;

static int dll_787_func_43C(Object *self, Object *a1, AnimObj_Data *a2, s8 a3);
static s16 dll_787_func_490(Object *self, WCApertureSymbol_Data *objdata, f32 arg2, f32 arg3, f32 arg4);

// offset: 0x0 | ctor
void dll_787_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_787_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_787_setup(Object *self, WCApertureSymbol_Setup *setup, s32 arg2) {
    WCApertureSymbol_Data *objdata = self->data;

    self->srt.yaw = setup->unk18 << 8;
    self->animCallback = dll_787_func_43C;
    self->modelInstIdx = setup->unk19;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
    if (main_get_bits(setup->unk20) != 0) {
        if (main_get_bits(setup->unk1E) != 0) {
            objdata->state = 2;
        } else {
            objdata->state = 1;
        }
    }
    self->opacity = 1;
    objdata->targetOpacity = 0;
}

// offset: 0xFC | func: 1 | export: 1
void dll_787_control(Object *self) {
    WCApertureSymbol_Setup *setup;
    f32 sp48;
    WCApertureSymbol_Data *objdata;
    s32 var_v1;
    Object *player;

    setup = (WCApertureSymbol_Setup*)self->setup;
    objdata = self->data;
    player = get_player();
    objdata->targetOpacity = 0;
    switch (objdata->state) {
    case 2:
        objdata->targetOpacity = 0xFF;
        break;
    case 0:
        break;
    case 1:
        if (gDLL_2_Camera->vtbl->func3() == 0x56) {
            if ((((DLL_210_Player*)player->dll)->vtbl->func70(player) == 0x21) && (vec3_distance(&player->positionMirror, &self->positionMirror) < 200.0f)) {
                gDLL_7_Newday->vtbl->func4(&sp48);
                if (setup->unk19 == 0) {
                    objdata->targetOpacity = dll_787_func_490(self, objdata, 66000.0f, 74000.0f, sp48);
                } else {
                    objdata->targetOpacity = dll_787_func_490(self, objdata, 77000.0f, 81500.0f, sp48);
                }
                if (setup->unk1A < (s32) self->opacity) {
                    main_set_bits(setup->unk1E, 1);
                    objdata->state = 2;
                    objdata->targetOpacity = 0xFF;
                }
            }
        }
        break;
    }
    if (self->opacity < objdata->targetOpacity) {
        var_v1 = self->opacity + (gUpdateRate * 4);
        if (objdata->targetOpacity < var_v1) {
            var_v1 = objdata->targetOpacity;
        }
        self->opacity = (u8) var_v1;
    } else if (objdata->targetOpacity < self->opacity) {
        var_v1 = self->opacity - (gUpdateRate * 4);
        if (var_v1 < objdata->targetOpacity) {
            var_v1 = objdata->targetOpacity;
        }
        self->opacity = (u8) var_v1;
    }
}

// offset: 0x358 | func: 2 | export: 2
void dll_787_update(Object *self) { }

// offset: 0x364 | func: 3 | export: 3
void dll_787_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    WCApertureSymbol_Data *objdata = self->data;

    if (visibility != 0) {
        objdata->unk3 |= 0x1;
    } else {
        objdata->unk3 &= ~0x1;
    }
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x3E4 | func: 4 | export: 4
void dll_787_free(Object *self, s32 a1) { }

// offset: 0x3F4 | func: 5 | export: 5
u32 dll_787_get_model_flags(Object *self) {
    WCApertureSymbol_Setup *setup = (WCApertureSymbol_Setup*)self->setup;
    s8 modelIdx = setup->unk19;

    if (modelIdx >= self->def->numModels) {
        modelIdx = 0;
    }
    return MODFLAGS_MODEL_INDEX(modelIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}


// offset: 0x428 | func: 6 | export: 6
u32 dll_787_get_data_size(Object *self, u32 a1) {
    return sizeof(WCApertureSymbol_Data);
}

// offset: 0x43C | func: 7
static int dll_787_func_43C(Object *self, Object *a1, AnimObj_Data *a2, s8 a3) {
    WCApertureSymbol_Data *objdata;
    s32 i;

    objdata = self->data;

    for (i = 0; i < a2->unk98; i++) {
        if (a2->unk8E[i] == 1) {
            objdata->state = 1;
        }
    }

    return 0;
}

// offset: 0x490 | func: 8
static s16 dll_787_func_490(Object *self, WCApertureSymbol_Data *objdata, f32 arg2, f32 arg3, f32 arg4) {
    s32 sp74;
    s32 sp70;
    f32 sp6C;
    f32 sp68;
    f32 sp64;
    f32 temp_fa0;
    f32 temp_fa0_3;
    f32 temp_ft4;
    f32 temp_fv0;
    f32 temp_ft5;
    f32 temp_fv1;
    u32 temp_a0;
    f32 temp_fv1_2;
    u32 temp_v0;
    u32 temp_v1;
    f32 var_fv0_2;

    var_fv0_2 = self->srt.transl.x - gWorldX;
    temp_fa0_3 = self->srt.transl.z - gWorldZ;

    func_80002D14(var_fv0_2, self->srt.transl.y, temp_fa0_3, &sp6C, &sp68, &sp64);
    func_80002E94(sp6C, sp68, sp64, &sp74, &sp70, NULL);
    if (vi_contains_point(sp74, sp70) == 0) {
        return 0;
    }
    if ((arg4 < arg2) || (arg3 < arg4)) {
        return 0;
    }
    temp_v0 = vi_get_current_size();
    temp_v1 = GET_VIDEO_HEIGHT(temp_v0);
    temp_a0 = GET_VIDEO_WIDTH(temp_v0);
    if ((temp_a0 < (u32) sp74) || (sp74 < 0)) {
        return 0;
    }
    if ((temp_v1 < (u32) sp70) || (sp70 < 0)) {
        return 0;
    }
    temp_ft4 = (f32) (temp_a0 >> 1);
    temp_fa0 = (f32) (temp_v1 >> 1);
    temp_fv0 = (f32) sp74 - temp_ft4;
    temp_ft5 = (f32) sp70 - temp_fa0;
    if (temp_fv0 < 0.0f) {
        temp_fv0 = -temp_fv0;
    }
    temp_fv1 = temp_ft5;
    if ((f32) sp70 < temp_fa0) {
        temp_fv1 = -temp_ft5;
    }
    temp_fv0 = temp_ft4 - temp_fv0;
    temp_fv1 = temp_fa0 - temp_fv1;
    temp_fv0 *= temp_fv0;
    temp_fv1 *= temp_fv1;
    temp_fa0 *= temp_fa0;
    temp_ft4 *= temp_ft4;
    temp_fv0 /= temp_ft4;
    temp_fv1 /= temp_fa0;
    temp_fv0 += temp_fv1;
    temp_fv0 *= 0.5f;
    if (temp_fv0 > 1.0f) {
        temp_fv0 = 1.0f;
    } else if (temp_fv0 < 0) {
        temp_fv0 = 0;
    }
    temp_fa0_3 = (arg3 + arg2) * 0.5f;
    temp_fv1_2 = arg4 - temp_fa0_3;
    if (temp_fv1_2 < 0.0f) {
        temp_fv1_2 = -temp_fv1_2;
    }
    var_fv0_2 = temp_fa0_3 - arg2;
    temp_fv1_2 = (var_fv0_2 - temp_fv1_2);
    temp_fv1_2 /= var_fv0_2;
    if (temp_fv1_2 > 1.0f) {
        temp_fv1_2 = 1.0f;
    } else if (temp_fv1_2 < 0) {
        temp_fv1_2 = 0;
    }
    diPrintf("dpos=%f dt=%f\n", &temp_fv0, &temp_fv1_2);
    return (s16) (s32) (temp_fv0 * temp_fv1_2 * 255.0f);
}
