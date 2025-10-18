#include "sys/rand.h"
#include "sys/main.h"
#include "game/gamebits.h"
#include "dll.h"
#include "functions.h"

typedef struct {
    u8 unk0;
} DIM_LevelControl_Data;

// offset: 0x0 | ctor
void DIM_LevelControl_ctor(void *dll) { }

// offset: 0xC | dtor
void DIM_LevelControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIM_LevelControl_setup(Object *self, ObjSetup *setup, s32 arg2) {
    DIM_LevelControl_Data *objdata;
    u8 rand;

    rand = rand_next(0, 11);
    objdata = self->data;
    objdata->unk0 = 0;
    main_set_bits(BIT_DIM_Gear_3_Random_Tent, rand);
    if (main_get_bits(BIT_42C)) {
        gDLL_29_Gplay->vtbl->set_map_setup(self->mapID, 2);
    } else {
        gDLL_29_Gplay->vtbl->set_map_setup(self->mapID, 1);
    }
    self->unk0xb0 |= 0x6000;
}

// offset: 0xF8 | func: 1 | export: 1
void DIM_LevelControl_control(Object *self) {
    u8 temp_a1;
    DIM_LevelControl_Data *objdata;
    s32 sp34;

    objdata = self->data;
    sp34 = !main_get_bits(BIT_3E3);
    temp_a1 = main_get_bits(BIT_3E2) & sp34;
    if (temp_a1 != objdata->unk0) {
        main_set_bits(BIT_3E8, temp_a1);
        objdata->unk0 = temp_a1;
    }
    if (!((u8)main_get_bits(BIT_DIM_Lit_All_Igloo_Fires)) &&
        (main_get_bits(BIT_DIM_Lit_Igloo_Fire_One)) &&
        (main_get_bits(BIT_DIM_Lit_Igloo_Fire_Two)) &&
        (main_get_bits(BIT_DIM_Lit_Igloo_Fire_Three))) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_798_Puzzle_Solved, MAX_VOLUME, NULL, NULL, 0, NULL);
        main_set_bits(BIT_DIM_Lit_All_Igloo_Fires, 1);
    }
}

// offset: 0x250 | func: 2 | export: 2
void DIM_LevelControl_update(Object *self) { }

// offset: 0x25C | func: 3 | export: 3
void DIM_LevelControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x2B0 | func: 4 | export: 4
void DIM_LevelControl_free(Object *self, s32 a1) { }

// offset: 0x2C0 | func: 5 | export: 5
u32 DIM_LevelControl_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x2D0 | func: 6 | export: 6
u32 DIM_LevelControl_get_data_size(Object *self, u32 a1) {
    return sizeof(DIM_LevelControl_Data);
}
