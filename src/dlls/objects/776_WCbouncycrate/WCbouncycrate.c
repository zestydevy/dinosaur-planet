#include "common.h"
#include "sys/objtype.h"

typedef struct {
    f32 unk0;
    s32 _unk4;
    s16 unk8;
    u8 unkA;
    u8 unkB;
} DLL776_Data;

static f32 dll_776_func_224(Object* arg0);

// offset: 0x0 | ctor
void dll_776_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_776_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_776_setup(Object* self, ObjSetup* setup, s32 arg2) {
    DLL776_Data* DLL776Data;

    DLL776Data = self->data;
    DLL776Data->unk0 = (f32) setup->y;
    DLL776Data->unk8 = 0x28;
}

// offset: 0x38 | func: 1 | export: 1
void dll_776_control(Object* self) {
    DLL776_Data* DLL776Data;

    DLL776Data = self->data;
    if (!(DLL776Data->unkA & 1)) {
        DLL776Data->unk8 -= gUpdateRateF;
        if (DLL776Data->unk8 <= 0) {
            self->speed.y = dll_776_func_224(self);
            DLL776Data->unkB = 0U;
            DLL776Data->unkA = (u8) (DLL776Data->unkA | 1);
        }
        return; 
    }

    self->speed.y += -0.14f * gUpdateRateF;
    self->srt.transl.y += self->speed.y * gUpdateRateF;

    if (self->srt.transl.y <= DLL776Data->unk0) {
        self->srt.transl.y = self->srt.transl.y + (DLL776Data->unk0 - self->srt.transl.y);
        self->speed.y = -self->speed.y * 0.8f;
        DLL776Data->unkB++;
        if (DLL776Data->unkB >= 0xB) {
            DLL776Data->unkA &= ~1;
            DLL776Data->unk8 = 0x28;
            self->srt.transl.y = DLL776Data->unk0;
            self->speed.y = 0.0f;
        }
    }
}


// offset: 0x190 | func: 2 | export: 2
void dll_776_update(Object *self) { }

// offset: 0x19C | func: 3 | export: 3
void dll_776_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1F0 | func: 4 | export: 4
void dll_776_free(Object *self, s32 a1) { }

// offset: 0x200 | func: 5 | export: 5
u32 dll_776_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x210 | func: 6 | export: 6
u32 dll_776_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL776_Data);
}

// offset: 0x224 | func: 7
static f32 dll_776_func_224(Object* arg0) {
    f32 sp24;

    sp24 = 10000.0f;
    if (obj_get_nearest_type_to(4, arg0, &sp24) == NULL) {
        return 0.0f;
    }
    if (sp24 < 200.0f) {
        return 2.0f;
    }
    if (sp24 > 500.0f) {
        return 0.0f;
    }
    return 2.0f * (1.0f - ((sp24 - 200.0f) / 300.0f));
}

