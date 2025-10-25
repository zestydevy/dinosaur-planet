#include "game/objects/object.h"
#include "sys/rand.h"
#include "functions.h"

#define SOME_COUNT 4

typedef struct {
    u8 _unk0[0x10];
    f32 x;
    f32 y;
    f32 z;
    f32 unk1C;
    f32 scale;
    u8 _unk24[0x34-0x24];
    s16 unk34[SOME_COUNT];
    u8 _unk3C[0x40-0x3C];
} checkpoint4_Data;

typedef struct {
    ObjSetup base;
    u8 unk18[0x28-0x18];
    s8 unk28;
    u8 yaw;
    u8 scale;
} checkpoint4_Setup;

// offset: 0x0 | ctor
void checkpoint4_ctor(void *dll) { }

// offset: 0xC | dtor
void checkpoint4_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void checkpoint4_setup(Object *self, checkpoint4_Setup *setup, s32 arg2) {
    s32 i;
    f32 scale;
    checkpoint4_Data *objdata;
    SRT transform;
    MtxF mtx;

    objdata = self->data;
    scale = setup->scale & 0xFF;
    if (scale < 5.0f) {
        scale = 5.0f;
    }
    scale *= 0.0078125f;
    self->srt.scale = scale;
    self->srt.yaw = setup->yaw << 8;
    transform.yaw = self->srt.yaw;
    transform.pitch = self->srt.pitch;
    transform.roll = self->srt.roll;
    transform.transl.x = 0.0f;
    transform.transl.y = 0.0f;
    transform.transl.z = 0.0f;
    transform.scale = 1.0f;

    matrix_from_srt(&mtx, &transform);
    vec3_transform(&mtx, 0.0f, 0.0f, 1.0f, &objdata->x, &objdata->y, &objdata->z);

    objdata->unk1C = -((self->srt.transl.x * objdata->x) + (self->srt.transl.y * objdata->y) + (self->srt.transl.z * objdata->z));
    objdata->scale = self->srt.scale * 2;

    for (i = 0; i != SOME_COUNT; i++) objdata->unk34[i] = rand_next(0, 240);

    self->unkDC = setup->unk28;
    self->unkB0 |= 0xA000;
}

// offset: 0x1D8 | func: 1 | export: 1
void checkpoint4_control(Object *self) { }

// offset: 0x1E4 | func: 2 | export: 2
void checkpoint4_update(Object *self) { }

// offset: 0x1F0 | func: 3 | export: 3
void checkpoint4_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x244 | func: 4 | export: 4
void checkpoint4_free(Object *self, s32 a1) { }

// offset: 0x254 | func: 5 | export: 5
u32 checkpoint4_get_model_flags(Object *self) {
    return MODFLAGS_10;
}

// offset: 0x264 | func: 6 | export: 6
u32 checkpoint4_get_data_size(Object *self, u32 a1) {
    return sizeof(checkpoint4_Data);
}

// offset: 0x278 | func: 7 | export: 7
void checkpoint4_func_278(Object *self) { }
