#include "common.h"

typedef struct {
ObjSetup base;
/*0x18*/ u8  unk18;
/*0x19*/ u8  unk19;
/*0x1A*/ u8  unk1A;
/*0x1C*/ f32 unk1C;
} Flowers_Setup;

// offset: 0x0 | ctor
void dll_513_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_513_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_513_setup(Object* self, Flowers_Setup* setup, s32 arg2) {
    self->srt.yaw = setup->unk1A << 8;
    self->srt.pitch = setup->unk19 << 8;
    self->srt.roll = setup->unk18 << 8;
    self->srt.scale = self->def->scale + (setup->unk1C * 0.15f);
}

// offset: 0x74 | func: 1 | export: 1
void dll_513_control(Object *self) { }

// offset: 0x80 | func: 2 | export: 2
void dll_513_update(Object *self) { }

// offset: 0x8C | func: 3 | export: 3
void dll_513_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    SRT sp30;
   
    if (visibility != 0) {
        if (vec3_distance(&self->positionMirror, &get_player()->positionMirror) <= 100.0f) {
            sp30.scale = self->srt.scale * 10.0f;
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3EA, &sp30, 2, -1, NULL);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x18C | func: 4 | export: 4
void dll_513_free(Object *self, s32 a1) { }

// offset: 0x19C | func: 5 | export: 5
u32 dll_513_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1AC | func: 6 | export: 6
u32 dll_513_get_data_size(Object *self, u32 a1) {
    return 0;
}
