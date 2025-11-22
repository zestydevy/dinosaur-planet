#include "dll.h"
#include "dlls/engine/17_partfx.h"
#include "sys/dll.h"
#include "sys/main.h"
#include "functions.h"
#include "sys/gfx/modgfx.h"
#include "dlls/modgfx/133.h"
#include "dlls/objects/573_SB_Cloudrunner.h"

typedef struct {
/*00*/ Object *cloudrunner;
/*04*/ s16 counter;
/*06*/ s16 unk6;
/*08*/ Vec3f speed;
/*14*/ s8 speedSaved;
} SB_FireBall_Data;

static void SB_FireBall_func_408(Object *self);

// offset: 0x0 | ctor
void SB_FireBall_ctor(void *dll) { }

// offset: 0xC | dtor
void SB_FireBall_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SB_FireBall_setup(Object *self, ObjSetup *setup, s32 arg2) {
    void *dll;
    SB_FireBall_Data *objdata;

    objdata = self->data;
    dll = dll_load_deferred(DLL_ID_133, 1);
    ((DLL_133*)dll)->vtbl->base.func0(self, 4, NULL, 0x10000 | 0x2, -1, NULL);
    dll_unload(dll);
    self->unkDC = 1200;
    objdata->unk6 = 0;
    objdata->speedSaved = FALSE;
}

// offset: 0xCC | func: 1 | export: 1
void SB_FireBall_control(Object *self) {
    SB_FireBall_Data *objdata;
    f32 a1;
    f32 a2;

    objdata = self->data;
    if (!objdata->cloudrunner) {
        objdata->cloudrunner = (Object*)self->unkE0;
    }
    if (objdata->cloudrunner) {
        self->srt.yaw = 0;
        self->srt.roll += gUpdateRate * -0x320;
        ((DLL_573_SB_Cloudrunner*)objdata->cloudrunner->dll)->vtbl->func10(objdata->cloudrunner, &a1, &a2);
        self->unkDC -= gUpdateRate;
        if (self->unkDC < 0) {
            obj_destroy_object(self);
        }
        if (!objdata->speedSaved) {
            objdata->speed.x = self->speed.x;
            objdata->speed.y = self->speed.y;
            objdata->speed.z = self->speed.z;
            objdata->speedSaved = TRUE;
        }
        self->srt.transl.z -= a1 * 0.25f * gUpdateRateF;
        self->srt.transl.x += objdata->speed.x * gUpdateRateF;
        self->srt.transl.y += objdata->speed.y * gUpdateRateF;
        self->srt.transl.z += objdata->speed.z * gUpdateRateF;
        SB_FireBall_func_408(self);
        if (objdata->counter >= 16) {
            self->objhitInfo->unk5F = 5;
            self->objhitInfo->unk60 = 1;
            self->objhitInfo->unk40 = 0x10;
            self->objhitInfo->unk44 = 0x10;
            self->objhitInfo->unk58 |= 1;
        } else {
            self->objhitInfo->unk58 &= ~1;
        }
        objdata->counter += gUpdateRate;
    }
}

// offset: 0x2CC | func: 2 | export: 2
void SB_FireBall_update(Object *self) { }

// offset: 0x2D8 | func: 3 | export: 3
void SB_FireBall_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        func_8003E9B4(8);
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_8003E9D0(8);
    }
}

// offset: 0x374 | func: 4 | export: 4
void SB_FireBall_free(Object *self, s32 a1) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x3E4 | func: 5 | export: 5
u32 SB_FireBall_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x3F4 | func: 6 | export: 6
u32 SB_FireBall_get_data_size(Object *self, u32 a1) {
    return sizeof(SB_FireBall_Data);
}

// offset: 0x408 | func: 7
void SB_FireBall_func_408(Object *self) {
    s32 _pad[3];
    SRT transform;
    transform.transl.x = 0.0f;
    transform.transl.y = 0.0f;
    transform.transl.z = 0.0f;
    transform.roll = 0;
    transform.pitch = 0;
    transform.yaw = 0;
    do {
        transform.scale = 1.0f;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_9D, NULL, PARTFXFLAG_1, -1, NULL); \
        transform.transl.x = 0.0f; \
        transform.transl.y = 0.0f; \
        transform.transl.z = 0.0f; \
        do {
            transform.transl.x /= 35.0f;
            transform.transl.y /= 35.0f;
            transform.transl.z /= 35.0f;
        } while (0);
    } while (0);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_A1, &transform, PARTFXFLAG_1, -1, NULL);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_A1, &transform, PARTFXFLAG_1, -1, NULL);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_A1, &transform, PARTFXFLAG_1, -1, NULL);
}
