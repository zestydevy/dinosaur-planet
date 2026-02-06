#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "functions.h"
#include "dll.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 unk18;
/*19*/ s8 unk19;
/*1A*/ s16 unk1A;
/*1C*/ s16 unk1C;
/*1E*/ s16 unk1E;
/*20*/ s16 unk20;
} DLL506_Setup;

// offset: 0x0 | ctor
void dll_506_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_506_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_506_setup(Object* self, ObjSetup* setup, s32 arg2) {
    self->unkB0 |= 0x2000;
}

// offset: 0x34 | func: 1 | export: 1
void dll_506_control(Object* self) {
    DLL506_Setup* sp24 = (DLL506_Setup*) self->setup;
    s16 temp_a1;

    temp_a1 = sp24->unk20;
    if ((temp_a1 != -1) && (main_get_bits(temp_a1) == 0)) {
        self->unkAF |= 8;
        return;
    }
    if (main_get_bits(BIT_Inventory_Purple_Mushrooms) == 0) {
        self->unkAF |= 0x10;
    } else {
        self->unkAF &= ~0x10;
    }
    if ((self->unkAF & 1) && (gDLL_1_UI->vtbl->func_DF4(BIT_Inventory_Purple_Mushrooms) != 0)) {
        main_decrement_bits(BIT_Inventory_Purple_Mushrooms);
        main_set_bits(sp24->unk1E, 1);
        self->unkAF |= 0x8;
    }
    if (main_get_bits(sp24->unk1E) == 0) {
        self->unkAF &= ~0x8;
    }
    func_80036438(self);
}

// offset: 0x184 | func: 2 | export: 2
void dll_506_update(Object *self) { }

// offset: 0x190 | func: 3 | export: 3
void dll_506_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x1A8 | func: 4 | export: 4
void dll_506_free(Object *self, s32 a1) { }

// offset: 0x1B8 | func: 5 | export: 5
u32 dll_506_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1C8 | func: 6 | export: 6
u32 dll_506_get_data_size(Object* self, u32 a1) {
    return 0;
}

