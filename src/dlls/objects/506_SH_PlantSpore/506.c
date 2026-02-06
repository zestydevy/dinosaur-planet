#include "common.h"

typedef struct {
/*0x00*/ s16 objId;
/*0x02*/ u8 quarterSize; // Bits 0-7: Exclude from map setups 1-8
/*0x03*/ u8 setupExclusions1;
/*0x04*/ u8 loadFlags;
/*0x05*/ union {
u8 byte5; // Bits 7-4 (note the reversal): Exclude from map setups 9-12
u8 setupExclusions2; // Bits 0-3: Fade flags
u8 fadeFlags; };
/*0x06*/ union {
u8 byte6; // If loadFlags & 0x10 IS set, the map object group this object is a part of.
u8 mapObjGroup; // If loadFlags & 0x10 is NOT set, maximum distance object is loaded at (divided by 8).
u8 loadDistance; }; // Max distance object is visible at (divided by 8).
/*0x07*/ u8 fadeDistance;
/*0x08*/ f32 x;
/*0x0c*/ f32 y;
/*0x10*/ f32 z;
/*0x14*/ s32 uID;
/*0x18*/ s8 unk18;
/*0x19*/ s8 unk19;
         s16 unk1A;
         s16 unk1C;
         s16 unk1E;
         s16 unk20;
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
    if ((temp_a1 != -1) && (main_get_bits((s32) temp_a1) == 0)) {
        self->unkAF |= 8;
        return;
    }
    if (main_get_bits(0x66C) == 0) {
        self->unkAF |= 0x10;
    } else {
        self->unkAF &= 0xFFEF;
    }
    if ((self->unkAF & 1) && (gDLL_1_UI->vtbl->func_DF4(0x66C) != 0)) {
        main_decrement_bits(0x66C);
        main_set_bits((s32) sp24->unk1E, 1U);
        self->unkAF |= 8;
    }
    if (main_get_bits((s32) sp24->unk1E) == 0) {
        self->unkAF &= 0xFFF7;
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
    return 0U;
}

