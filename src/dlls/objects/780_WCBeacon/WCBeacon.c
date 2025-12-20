#include "game/objects/object.h"
#include "segment_334F0.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "dll.h"
#include "functions.h"

typedef struct {
/*0*/ f32 unk0;
/*4*/ u8 state;
} WCBeacon_Data;

typedef struct {
/*00*/ ObjSetup base;
/*19*/ s8 unk18;
/*19*/ s8 modelIndex;
/*1A*/ u8 _unk1A[0x1E - 0x1A];
/*1E*/ s16 unk1E;
/*20*/ s16 unk20;
} WCBeacon_Setup;

enum WCBeaconState {
    WCBEACON_STATE_0 = 0,
    WCBEACON_STATE_1 = 1,
    WCBEACON_STATE_2 = 2,
    WCBEACON_STATE_3 = 3
};

// offset: 0x0 | ctor
void dll_780_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_780_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_780_setup(Object *self, WCBeacon_Setup *setup, s32 arg2) {
    WCBeacon_Data *objdata = self->data;
    TextureAnimator *animatedTexture;
    
    self->srt.yaw = setup->unk18 << 8;
    self->modelInstIdx = setup->modelIndex;

    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }

    if (main_get_bits(setup->unk20) != 0) {
        if (main_get_bits(setup->unk1E) != 0) {
            objdata->state = WCBEACON_STATE_3;
        } else {
            objdata->state = WCBEACON_STATE_1;
        }
    }

    animatedTexture = func_800348A0(self, 0, 0);
    if ((animatedTexture != NULL) && (objdata->state == WCBEACON_STATE_3)) {
        animatedTexture->frame = 0x100;
    }
}

// offset: 0x11C | func: 1 | export: 1
void dll_780_control(Object *self) {
    WCBeacon_Setup *setup;
    WCBeacon_Data *objdata;
    s32 *temp_v0_2;
    s16 var_v1;

    objdata = (WCBeacon_Data*)self->data;
    setup = (WCBeacon_Setup*)self->setup;
    
    if (objdata->state == WCBEACON_STATE_1) {
        if (main_get_bits(setup->unk20) == 0) {
            gDLL_3_Animation->vtbl->func17(1, self, -1);
            objdata->state = WCBEACON_STATE_0;
        }
        if (func_80025F40(self, NULL, NULL, NULL) != 0) {
            // Hit
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_50a_Fire_Burning_Low_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_50b_Fire_Burning_High_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
            objdata->state = WCBEACON_STATE_2;
            objdata->unk0 = 0.0f;
        }
    } else if (objdata->state == WCBEACON_STATE_0) {
        if (main_get_bits(setup->unk20) != 0) {
            gDLL_3_Animation->vtbl->func17(0, self, -1);
            objdata->state = WCBEACON_STATE_1;
        }
    } else if (objdata->state == WCBEACON_STATE_2) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_73A, NULL, PARTFXFLAG_2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_73B, NULL, PARTFXFLAG_2, -1, NULL);
        objdata->unk0 += gUpdateRateF;
        if (objdata->unk0 >= 90.0f) {
            objdata->state = WCBEACON_STATE_3;
        }
        var_v1 = (s16) ((objdata->unk0 / 45.0f) * 256.0f);
        if (var_v1 >= 0x101) {
            var_v1 = 0x100;
        }
        temp_v0_2 = func_800348A0(self, 0, 0);
        if (temp_v0_2 != NULL) {
            *temp_v0_2 = (s32) var_v1;
        }
    } else if (objdata->state == WCBEACON_STATE_3) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_73A, NULL, PARTFXFLAG_2, -1, NULL);
        main_set_bits(setup->unk1E, 1);
        temp_v0_2 = func_800348A0(self, 0, 0);
        if (temp_v0_2 != NULL) {
            *temp_v0_2 = 0x100;
        }
        if (self->unkDC == 0) {
            gDLL_3_Animation->vtbl->func20(self, 0x69);
            gDLL_3_Animation->vtbl->func17(0, self, 1);
        }
    }
    self->unkDC = 1;
}

// offset: 0x4E8 | func: 2 | export: 2
void dll_780_update(Object *self) { }

// offset: 0x4F4 | func: 3 | export: 3
void dll_780_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x548 | func: 4 | export: 4
void dll_780_free(Object *self, s32 a1) { }

// offset: 0x558 | func: 5 | export: 5
u32 dll_780_get_model_flags(Object *self) {
    WCBeacon_Setup *setup = (WCBeacon_Setup*)self->setup;
    s8 modelIdx;

    modelIdx = setup->modelIndex;
    if (modelIdx >= self->def->numModels) {
        modelIdx = 0;
    }
    return MODFLAGS_MODEL_INDEX(modelIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x58C | func: 6 | export: 6
u32 dll_780_get_data_size(Object *self, u32 a1) {
    return sizeof(WCBeacon_Data);
}
