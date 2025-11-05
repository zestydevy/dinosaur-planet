#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "dll.h"
#include "functions.h"
#include "sys/objects.h"
#include "sys/main.h"
#include "dlls/objects/338_LFXEmitter.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    Object *lfxEmitter;
} DIM_BossSpit_Data;

static void DIM_BossSpit_func_2AC(Object *self);
static void DIM_BossSpit_func_360(Object *self);
static void DIM_BossSpit_func_53C(Object *self);

// offset: 0x0 | ctor
void DIM_BossSpit_ctor(void *dll) { }

// offset: 0xC | dtor
void DIM_BossSpit_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIM_BossSpit_setup(Object *self, ObjSetup *setup, s32 arg2) {
    DIM_BossSpit_Data *objdata;

    objdata = self->data;
    DIM_BossSpit_func_2AC(self);
    self->unkDC = 0xB4;
    func_80026128(self, 0, 0, 0);
    func_80026940(self, 0);
    objdata->unk0 = 0;
    objdata->unk2 = 0;
    func_8002674C(self);
}

// offset: 0xCC | func: 1 | export: 1
void DIM_BossSpit_control(Object *self) {
    DIM_BossSpit_Data *objdata;

    objdata = self->data;
    if (objdata->unk0 == 0) {
        self->unkDC -= gUpdateRate;
        if (self->unkDC < 0) {
            obj_destroy_object(self);
        } else {
            DIM_BossSpit_func_360(self);
            if (self->objhitInfo->unk9D != 0) {
                self->srt.transl.x = self->objhitInfo->unk34;
                self->srt.transl.y = (self->objhitInfo->unk38 - 10.0f);
                self->srt.transl.z = self->objhitInfo->unk3C;
                objdata->unk0 = 1;
            }
        }
    } else {
        DIM_BossSpit_func_53C(self);
    }
}

// offset: 0x1A8 | func: 2 | export: 2
void DIM_BossSpit_update(Object *self) { }

// offset: 0x1B4 | func: 3 | export: 3
void DIM_BossSpit_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x208 | func: 4 | export: 4
void DIM_BossSpit_free(Object *self, s32 a1) {
    DIM_BossSpit_Data *objdata;
    Object *lfxEmitter;

    objdata = self->data;
    lfxEmitter = objdata->lfxEmitter;
    if (lfxEmitter && a1 == 0) {
        obj_destroy_object(lfxEmitter);
    }
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x288 | func: 5 | export: 5
u32 DIM_BossSpit_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x298 | func: 6 | export: 6
u32 DIM_BossSpit_get_data_size(Object *self, u32 a1) {
    return sizeof(DIM_BossSpit_Data);
}

// offset: 0x2AC | func: 7
void DIM_BossSpit_func_2AC(Object *self) {
    DIM_BossSpit_Data *objdata;
    LFXEmitter_Setup *lfxemitterSetup;

    objdata = self->data;
    lfxemitterSetup = obj_alloc_create_info(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
    lfxemitterSetup->base.x = self->srt.transl.x;
    lfxemitterSetup->base.y = self->srt.transl.y;
    lfxemitterSetup->base.z = self->srt.transl.z;
    lfxemitterSetup->unk1E = 0x145;
    lfxemitterSetup->unk22 = -1;
    lfxemitterSetup->base.loadParamA = 1;
    lfxemitterSetup->base.loadParamB = 1;
    lfxemitterSetup->base.loadDistance = 0xFF;
    lfxemitterSetup->base.fadeDistance = 0xFF;
    objdata->lfxEmitter = obj_create((ObjSetup*)lfxemitterSetup, 5, -1, -1, NULL);
}

// offset: 0x360 | func: 8
void DIM_BossSpit_func_360(Object *self) {
    s32 i;
    DIM_BossSpit_Data *objdata;

    objdata = self->data;
    func_80026128(self, 5, 4, 0);
    func_80026940(self, 0xA);
    self->positionMirror2.x = self->srt.transl.x;
    self->positionMirror2.y = self->srt.transl.y;
    self->positionMirror2.z = self->srt.transl.z;
    self->speed.y -= 0.07f * gUpdateRateF;
    self->speed.y *= 0.97f;
    self->srt.yaw += 0xAAA;
    self->srt.roll += 0x38E;
    self->srt.pitch += 0x38E;
    obj_integrate_speed(self, self->speed.x * gUpdateRateF, self->speed.y * gUpdateRateF, self->speed.z * gUpdateRateF);
    if (objdata->lfxEmitter != NULL) {
        objdata->lfxEmitter->srt.transl.x = self->srt.transl.x;
        objdata->lfxEmitter->srt.transl.y = self->srt.transl.y;
        objdata->lfxEmitter->srt.transl.z = self->srt.transl.z;
    }
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_4B5, NULL, PARTFXFLAG_1, -1, NULL);
    for (i = 0; i != 4; i++) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_4BA, NULL, PARTFXFLAG_1, -1, NULL);
    }
}

// offset: 0x53C | func: 9
void DIM_BossSpit_func_53C(Object *self) {
    /*0x0*/ static s32 _bss_0;
    s32 i;
    Object *lfxEmitter;
    LFXEmitter_Data *lfxEmitterData;
    DIM_BossSpit_Data *objdata;

    objdata = self->data;
    self->srt.scale += 0.1f;
    self->srt.yaw += 0xAAA;
    self->srt.roll += 0x38E;
    self->srt.pitch += 0x38E;
    if (objdata->unk0 == 1) {
        for (i = 0; i != 18; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_340, NULL, PARTFXFLAG_1, -1, NULL);
        }
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_4BB, NULL, PARTFXFLAG_1, -1, NULL);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_380, MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_386, MAX_VOLUME, NULL, NULL, 0, NULL);
        func_80003B70(3.0f);
    }
    objdata->unk0 += gUpdateRate;
    if (objdata->unk0 > 512) {
        obj_destroy_object(self);
    }
    i = 0xFF - (s32) (((f32)objdata->unk0 / 64) * 255.0f);
    _bss_0 = 0x94 - (objdata->unk0 >> 2);
    if (i >= 0) {
        func_80026128(self, 5, 2, 0);
        func_80026940(self, self->objhitInfo->unk52 + 2.1f);
        self->opacity = i;
        lfxEmitter = objdata->lfxEmitter;
        if (lfxEmitter) {
            lfxEmitterData = lfxEmitter->data;
            gDLL_11_Newlfx->vtbl->func6(lfxEmitterData->unk108->unk10, 0, i, lfxEmitter);
        }
    } else {
        lfxEmitter = objdata->lfxEmitter;
        if (lfxEmitter) {
            obj_destroy_object(lfxEmitter);
            objdata->lfxEmitter = NULL;
        }
        self->opacity = 0;
        if (((_bss_0 - 0x40) >> 1) > 10.0f) {
            func_80026128(self, 9, 1, 0);
            func_80026940(self, (_bss_0 - 0x40) >> 1);
        }
    }
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_4BC, NULL, PARTFXFLAG_1, -1, &_bss_0);
}
