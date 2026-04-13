#include "common.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
} SkeetlaWall_Data;

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
} SkeetlaWall_Setup;

// offset: 0x0 | ctor
void dll_241_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_241_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_241_setup(Object* self, SkeetlaWall_Setup* setup, s32 arg2) {
    SkeetlaWall_Data* temp_v0;

    temp_v0 = self->data;
    temp_v0->unk0 = setup->unk18;
    temp_v0->unk1 = setup->unk19;
    temp_v0->unk2 = setup->unk1A;
    temp_v0->unk3 = setup->unk1B;
    temp_v0->unk4 = setup->unk1C;
    temp_v0->unk5 = setup->unk1D;
    temp_v0->unk6 = setup->unk1E;
}

// offset: 0x60 | func: 1 | export: 1
void dll_241_control(Object *self) { }

// offset: 0x6C | func: 2 | export: 2
void dll_241_update(Object *self) { }

// offset: 0x78 | func: 3 | export: 3
void dll_241_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if ((visibility != 0) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xD8 | func: 4 | export: 4
void dll_241_free(Object *self, s32 a1) { }

// offset: 0xE8 | func: 5 | export: 5
u32 dll_241_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xF8 | func: 6 | export: 6
u32 dll_241_get_data_size(Object *self, u32 a1) {
    return sizeof(SkeetlaWall_Data);
}

// offset: 0x10C | func: 7 | export: 7
void dll_241_func_10C(Object* self, f32* arg1, s8* arg2) {
    SkeetlaWall_Data* objdata;

    objdata = self->data;
    arg1[0] = self->globalPosition.x - objdata->unk0;
    arg1[1] = self->globalPosition.x + objdata->unk1;
    arg1[2] = self->globalPosition.z + objdata->unk2;
    arg1[3] = self->globalPosition.z - objdata->unk3;
    arg1[4] = self->globalPosition.y + objdata->unk4;
    arg1[5] = self->globalPosition.y - objdata->unk5;
    arg2[0] = objdata->unk6;
}
