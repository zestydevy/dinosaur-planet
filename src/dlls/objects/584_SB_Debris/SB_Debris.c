#include "dlls/engine/17_partfx.h"
#include "functions.h"
#include "dll.h"
#include "sys/rand.h"
#include "sys/main.h"
#include "sys/objects.h"

typedef struct {
/*00*/ f32 unk0;
} SB_Debris_Data;

/*0x0*/ static Vec3f sSpeed = { 1.8f, 0.0f, 0.0f };

// offset: 0x0 | ctor
void SB_Debris_ctor(void *dll) { }

// offset: 0xC | dtor
void SB_Debris_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SB_Debris_setup(Object *self, ObjSetup *setup, s32 arg2) {
    SRT transform;
    Vec3f speed;
    SB_Debris_Data *objdata;

    speed = sSpeed;
    objdata = self->data;
    speed.x *= rand_next(3, 5);
    self->unkDC = 75;
    objdata->unk0 = 75.0f;
    transform.transl.x = 0.0f;
    transform.transl.y = 0.0f;
    transform.transl.z = 0.0f;
    transform.scale = 1.0f;
    transform.roll = 0;
    transform.pitch = rand_next(-0x2EE0, 0x2EE0);
    transform.yaw = rand_next(0, 0xFFFE);
    rotate_vec3(&transform, (f32*)&speed);
    self->speed.x = speed.x;
    self->speed.y = speed.y;
    self->speed.z = speed.z;
    self->modelInstIdx = rand_next(0, 1);
    self->srt.scale *= 5.0f;
}

// offset: 0x170 | func: 1 | export: 1
void SB_Debris_control(Object *self) {
    f32 dx;
    f32 dy;
    f32 dz;
    s32 _pad;
    SRT transform;
    s32 _pad2;

    transform.transl.x = 0.0f;
    transform.transl.y = 0.0f;
    transform.transl.z = 0.0f;
    transform.roll = 0;
    transform.pitch = 0;
    transform.yaw = 0;
    transform.scale = 1.0f;
    self->srt.transl.x += self->speed.x * gUpdateRateF;
    self->srt.transl.y += self->speed.y * gUpdateRateF;
    self->srt.transl.z += self->speed.z * gUpdateRateF;
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_9F, NULL, PARTFXFLAG_1, -1, NULL);
    dx = (self->srt.transl.x - self->positionMirror2.x);
    dy = (self->srt.transl.y - self->positionMirror2.y);
    dz = (self->srt.transl.z - self->positionMirror2.z);
    transform.transl.x = dx / 3.0f;
    transform.transl.y = dy / 3.0f;
    transform.transl.z = dz / 3.0f;
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_9F, &transform, PARTFXFLAG_1, -1, NULL);
    VECTOR_SCALE(transform.transl, 2.0f);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_9F, &transform, PARTFXFLAG_1, -1, NULL);
    self->srt.yaw += gUpdateRate * 0x374;
    self->srt.pitch += gUpdateRate * 0x12C;
    self->speed.y -= 0.05f;
    self->unkDC -= gUpdateRate;
    if (self->unkDC < 0) {
        obj_destroy_object(self);
    }
}

// offset: 0x3C0 | func: 2 | export: 2
void SB_Debris_update(Object *self) { }

// offset: 0x3CC | func: 3 | export: 3
void SB_Debris_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    SB_Debris_Data *objdata;
    s16 opacity;

    objdata = self->data;
    opacity = ((self->unkDC * 255.0f) / objdata->unk0);
    if (opacity < 0) {
        opacity = 0;
    } else if (opacity > OBJECT_OPACITY_MAX) {
        opacity = OBJECT_OPACITY_MAX;
    }
    self->opacityWithFade = opacity;
    self->opacity = opacity;
    if (visibility) {
        tex_disable_modes(RENDER_FOG_ACTIVE);
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        tex_enable_modes(RENDER_FOG_ACTIVE);
    }
}

// offset: 0x4D0 | func: 4 | export: 4
void SB_Debris_free(Object *self, s32 a1) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x518 | func: 5 | export: 5
u32 SB_Debris_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x528 | func: 6 | export: 6
u32 SB_Debris_get_data_size(Object *self, u32 a1) {
    return sizeof(SB_Debris_Data);
}
