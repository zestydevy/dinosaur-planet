#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "game/gamebits.h"

typedef struct {
/*00*/ u8 unk0;
/*01*/ u8 unk1;
/*02*/ u8 unk2;
/*03*/ u8 _unk3;
/*04*/ u8 unk4;
} CCkrazoaBits_Data;

// offset: 0x0 | ctor
void CCkrazoaBits_ctor(void *dll) { }

// offset: 0xC | dtor
void CCkrazoaBits_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCkrazoaBits_setup(Object *self, ObjSetup *setup, s32 arg2) {
    CCkrazoaBits_Data *objdata = self->data;
    objdata->unk0 = main_get_bits(BIT_CC_Activate_Krazoa_Tablet_Slots);
    objdata->unk1 = main_get_bits(BIT_236);
    objdata->unk2 = main_get_bits(BIT_237);
    self->unkB0 |= 0x2000;
}

// offset: 0xB4 | func: 1 | export: 1
void CCkrazoaBits_control(Object *self) {
    CCkrazoaBits_Data *objdata = self->data;

    if (objdata->unk0 == 0 && main_get_bits(BIT_CC_Activate_Krazoa_Tablet_Slots)) {
        objdata->unk0 = 1;
        main_increment_bits(BIT_CC_Krazoa_Tablets);
        main_set_bits(BIT_23C, 1);
    }
    if (objdata->unk1 == 0 && main_get_bits(BIT_236)) {
        objdata->unk1 = 1;
        objdata->unk4 = 1;
        main_set_bits(BIT_23C, 1);
        main_increment_bits(BIT_CC_Krazoa_Tablets);
        main_set_bits(BIT_23C, 1);
    }
    if (objdata->unk2 == 0 && main_get_bits(BIT_237)) {
        objdata->unk2 = 1;
        main_increment_bits(BIT_CC_Krazoa_Tablets);
        main_set_bits(BIT_23C, 1);
    }
    if ((objdata->unk4 != 0) && main_get_bits(BIT_13F) && main_get_bits(BIT_140)) {
        objdata->unk4 = 0;
        main_set_bits(BIT_23C, 0);
    }
}

// offset: 0x260 | func: 2 | export: 2
void CCkrazoaBits_update(Object *self) { }

// offset: 0x26C | func: 3 | export: 3
void CCkrazoaBits_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x284 | func: 4 | export: 4
void CCkrazoaBits_free(Object *self, s32 arg1) { }

// offset: 0x294 | func: 5 | export: 5
u32 CCkrazoaBits_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x2A4 | func: 6 | export: 6
u32 CCkrazoaBits_get_data_size(Object *self, u32 arg1) {
    return sizeof(CCkrazoaBits_Data);
}
