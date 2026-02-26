#include "common.h"
#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 unk18[0x1A - 0x18];
/*1A*/ s16 partidx; // 0 or 1
/*1C*/ u8 unk1C[0x1E - 0x1C];
/*1E*/ s16 gamebit;
/*20*/ s16 unk20;
/*22*/ s8 roll;
/*23*/ s8 pitch;
/*24*/ s8 yaw;
} DIMbosscrackparticles_Setup;

static int DIMbosscrackparticles_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);

// offset: 0x0 | ctor
void DIMbosscrackparticles_ctor(void *dll) { }

// offset: 0xC | dtor
void DIMbosscrackparticles_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIMbosscrackparticles_setup(Object *self, DIMbosscrackparticles_Setup *setup, s32 arg2) {
    self->srt.yaw = 0;
    self->srt.scale = 0.1f;
    self->animCallback = DIMbosscrackparticles_anim_callback;
    self->srt.yaw = setup->yaw << 8;
    self->srt.pitch = setup->pitch << 8;
    self->srt.roll = setup->roll << 8;
}

// offset: 0x70 | func: 1 | export: 1
void DIMbosscrackparticles_control(Object *self) {
    DIMbosscrackparticles_Setup *setup;

    setup = (DIMbosscrackparticles_Setup*)self->setup;
    if (main_get_bits(setup->gamebit)) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_4C6 + setup->partidx, NULL, PARTFXFLAG_2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_4C8, NULL, PARTFXFLAG_2, -1, NULL);
    }
}

// offset: 0x134 | func: 2 | export: 2
void DIMbosscrackparticles_update(Object *self) { }

// offset: 0x140 | func: 3 | export: 3
void DIMbosscrackparticles_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x158 | func: 4 | export: 4
void DIMbosscrackparticles_free(Object *self,  s32 a1) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x1A0 | func: 5 | export: 5
u32 DIMbosscrackparticles_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1B0 | func: 6 | export: 6
u32 DIMbosscrackparticles_get_data_size(Object *self, u32 a1) {
    return 0;
}

// offset: 0x1C4 | func: 7
int DIMbosscrackparticles_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    DIMbosscrackparticles_Setup *setup;

    setup = (DIMbosscrackparticles_Setup*)self->setup;
    if (!main_get_bits(setup->gamebit)) {
        return 0;
    } else {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_4C6 + setup->partidx, NULL, PARTFXFLAG_2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_4C8, NULL, PARTFXFLAG_2, -1, NULL);
    }
    return 0;
}
