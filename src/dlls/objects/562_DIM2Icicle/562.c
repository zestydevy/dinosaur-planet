#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "functions.h"

typedef struct {
/*00*/ u8 _unk0[0xA - 0x0];
/*0A*/ u8 unkA;
/*0B*/ u8 _unkB[0x10 - 0xB];
} DIM2Icicle_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 unk18;
/*19*/ s8 _unk19[0x1E - 0x19];
/*1E*/ s16 unk1E;  
} DIM2Icicle_Setup;

// offset: 0x0 | ctor
void dll_562_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_562_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_562_setup(Object* self, DIM2Icicle_Setup* setup, s32 arg2) {
    DIM2Icicle_Data* objdata = self->data;
    
    if (main_get_bits(setup->unk1E) != 0) {
        objdata->unkA = 2;
        self->opacity = 0;
    } else {
        objdata->unkA = 0;
        self->opacity = 0xFF;
    }
    
    self->srt.yaw = setup->unk18 << 8;
    self->unkB0 |= 0x2000;
    self->speed.y = 0.0f;
}

// offset: 0xBC | func: 1 | export: 1
void dll_562_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/562_DIM2Icicle/dll_562_control.s")

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
    return sizeof(DIM2Icicle_Data);
}
