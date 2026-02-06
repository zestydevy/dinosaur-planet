#include "common.h"

typedef struct {
/*0x00*/ u8 _unk0[0xA - 0x0];
    u8 unkA;
}DIM2Icicle_Data;
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
/*0x19*/ s8 _unk19 [0x1E - 0x19];
/*0x1E*/ s16 unk1E;  
} DIM2Icicle_Setup;

// offset: 0x0 | ctor
void dll_562_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_562_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_562_setup(Object* self, DIM2Icicle_Setup* setup, s32 arg2) {

    DIM2Icicle_Data* Data730 = self->data ;
    s32 pad;
    u16 temp;
    s16 temp2;

    temp2 = setup->unk1E;
    
    if (main_get_bits((s32) temp2) != 0) {
        Data730->unkA = 2;
        self->opacity = 0;
    } else {
        Data730->unkA = 0;
        self->opacity = 0xFF;
    }

    temp = setup->unk18;
    
    self->srt.yaw =  temp << 8;
    self->unkB0 |= 0x2000;
    self->speed.f[1] = 0.0f;
}


// offset: 0xBC | func: 1 | export: 1
void dll_562_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/562/dll_562_control.s")

// offset: 0x538 | func: 2 | export: 2
void dll_562_update(Object *self) { }

// offset: 0x544 | func: 3 | export: 3
void dll_562_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x598 | func: 4 | export: 4
void dll_562_free(Object *self, s32 a1) { }

// offset: 0x5A8 | func: 5 | export: 5
u32 dll_562_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x5B8 | func: 6 | export: 6
u32 dll_562_get_data_size(Object* self, u32 a1) {
    return 0x10U;
}

