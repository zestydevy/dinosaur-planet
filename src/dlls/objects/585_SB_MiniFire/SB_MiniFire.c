#include "sys/main.h"
#include "functions.h"
#include "dll.h"
#include "sys/dll.h"
#include "sys/rand.h"
#include "dlls/modgfx/133.h"

typedef struct {
/*00*/ u8 unk0[2];
} SB_MiniFire_Data;

/*0x0*/ static s32 _data_0[1] = { 1 };

// offset: 0x0 | ctor
void SB_MiniFire_ctor(void *dll) { }

// offset: 0xC | dtor
void SB_MiniFire_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SB_MiniFire_setup(Object *self, ObjSetup *setup, s32 arg2) {
    s32 _pad[5];
    f32 rand;
    void **dll133;

    self->unkDC = 180;
    rand = rand_next(20, 40);
    self->speed.x = -((rand * 0.01f) + 0.8f);
    self->speed.y = 0.0f;
    self->speed.z = -0.3f;
    self->srt.scale *= 0.8f;
    dll133 = dll_load_deferred(DLL_ID_133, 1);
    ((DLL_133*)dll133)->vtbl->base.func0(self, _data_0[0], NULL, 0x10000 | 0x2, -1, NULL);
    _data_0[0]++;
    if (_data_0[0] > 3) {
        _data_0[0] = 1;
    }
    dll_unload(dll133);
}

// offset: 0x148 | func: 1 | export: 1
void SB_MiniFire_control(Object *self) {
    f32 dx;
    f32 dy;
    f32 dz;
    s32 _pad[3];
    SRT transform;

    self->srt.transl.x += self->speed.x * gUpdateRateF;
    self->srt.transl.y += self->speed.y * gUpdateRateF;
    self->srt.transl.z += self->speed.z * gUpdateRateF;
    transform.transl.x = 0.0f;
    transform.transl.y = 0.0f;
    transform.transl.z = 0.0f;
    transform.scale = 1.0f;
    if (self->unkDC <= 60) {
        transform.scale = self->unkDC / 60.0f;
        self->opacity = ((u8)((s32)((self->unkDC / 60.0f) * 255.0f))) & 0xFF;
    }
    transform.roll = 0;
    transform.pitch = 0;
    transform.yaw = 0;
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_A0, &transform, PARTFXFLAG_1, -1, NULL);
    dx = (self->srt.transl.x - self->positionMirror2.x);
    dy = (self->srt.transl.y - self->positionMirror2.y);
    dz = (self->srt.transl.z - self->positionMirror2.z);
    transform.transl.x = dx / 3.0f;
    transform.transl.y = dy / 3.0f;
    transform.transl.z = dz / 3.0f;
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_A0, &transform, PARTFXFLAG_1, -1, NULL);
    VECTOR_SCALE(transform.transl, 2.0f);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_A0, &transform, PARTFXFLAG_1, -1, NULL);
    self->srt.yaw += gUpdateRate * 0x374;
    self->srt.pitch += gUpdateRate * 0x12C;
    self->unkDC -= gUpdateRate;
    if (self->unkDC < 0) {
        obj_destroy_object(self);
    }
}

// offset: 0x3DC | func: 2 | export: 2
void SB_MiniFire_update(Object *self) { }

// offset: 0x3E8 | func: 3 | export: 3
void SB_MiniFire_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        tex_disable_modes(RENDER_FOG_ACTIVE);
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        tex_enable_modes(RENDER_FOG_ACTIVE);
    }
}

// offset: 0x484 | func: 4 | export: 4
void SB_MiniFire_free(Object *self, s32 a1) {
    gDLL_13_Expgfx->vtbl->func5(self);
    gDLL_14_Modgfx->vtbl->func5(self);
}

// offset: 0x4F4 | func: 5 | export: 5
u32 SB_MiniFire_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x504 | func: 6 | export: 6
u32 SB_MiniFire_get_data_size(Object *self, u32 a1) {
    return sizeof(SB_MiniFire_Data);
}
