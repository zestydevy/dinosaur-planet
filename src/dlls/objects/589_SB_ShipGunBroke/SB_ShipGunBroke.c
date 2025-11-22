#include "dll.h"
#include "functions.h"
#include "sys/main.h"

typedef struct {
/*00*/ u32 soundHandle;
/*04*/ s32 _unk4;
} SB_ShipGunBroke_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 unk18[0x1E-0x18];
/*1E*/ s16 gunDestroyedGamebit;
} SB_ShipGunBroke_Setup;

// offset: 0x0 | ctor
void SB_ShipGunBroke_ctor(void *dll) { }

// offset: 0xC | dtor
void SB_ShipGunBroke_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SB_ShipGunBroke_setup(Object *self, SB_ShipGunBroke_Setup *setup, s32 arg2) { }

// offset: 0x2C | func: 1 | export: 1
void SB_ShipGunBroke_control(Object *self) {
    SB_ShipGunBroke_Setup *setup;
    SB_ShipGunBroke_Data *objdata;

    objdata = self->data;
    setup = (SB_ShipGunBroke_Setup*)self->setup;
    if (main_get_bits(setup->gunDestroyedGamebit) && !objdata->soundHandle) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_BA3, MAX_VOLUME, &objdata->soundHandle, NULL, 0, NULL);
    }
}

// offset: 0xC0 | func: 2 | export: 2
void SB_ShipGunBroke_update(Object *self) { }

// offset: 0xCC | func: 3 | export: 3
void SB_ShipGunBroke_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    SB_ShipGunBroke_Setup *setup;

    setup = (SB_ShipGunBroke_Setup*)self->setup;
    if (main_get_bits(setup->gunDestroyedGamebit)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x150 | func: 4 | export: 4
void SB_ShipGunBroke_free(Object *self, s32 a1) {
    SB_ShipGunBroke_Data *objdata;

    objdata = self->data;
    if (objdata->soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
        objdata->soundHandle = 0;
    }
}

// offset: 0x1B8 | func: 5 | export: 5
u32 SB_ShipGunBroke_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1C8 | func: 6 | export: 6
u32 SB_ShipGunBroke_get_data_size(Object *self, u32 a1) {
    return sizeof(SB_ShipGunBroke_Data);
}
