#include "common.h"

typedef struct {
    UnkCurvesStruct curves;
    u8 state;
    s16 prevYaw; //Stored but not used for anything
} WGBoulder_Data;

typedef enum {
    WGBoulder_STATE_0_Stopped,
    WGBoulder_STATE_1_Moving
} WGBoulder_States;

/*0x0*/ static s32 dCurveTypes[] = {
    2
};

static void WGboulder_handle_motion(Object* self);

// offset: 0x0 | ctor
void WGboulder_ctor(void *dll) { }

// offset: 0xC | dtor
void WGboulder_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WGboulder_setup(Object *self, ObjSetup *setup, s32 reset) { }

// offset: 0x2C | func: 1 | export: 1
void WGboulder_control(Object* self) {
    WGBoulder_Data* objData;
    CurveSetup* curveSetup;
    s32 curveUID;
    s32 curveType;

    objData = self->data;
    
    diPrintf("state %d\n", objData->state);

    switch (objData->state) {
    case WGBoulder_STATE_0_Stopped:
        curveType = 2;
        curveUID = gDLL_26_Curves->vtbl->func_1E4(self->srt.transl.x, self->srt.transl.f[1], self->srt.transl.f[2], &curveType, 1, 0);
        if (curveUID != -1) {
            curveSetup = gDLL_26_Curves->vtbl->func_39C(curveUID);
            self->srt.transl.x = curveSetup->pos.x;
            self->srt.transl.z = curveSetup->pos.z;
        }

        if (gDLL_26_Curves->vtbl->func_4288(&objData->curves, self, 100.0f, dCurveTypes, -1) == 0) {
            objData->state = WGBoulder_STATE_1_Moving;
        }
        break;
    case WGBoulder_STATE_1_Moving:
        if ((curves_func_800053B0(&objData->curves.unk0, 2.0f) || objData->curves.unk0.unk10) && gDLL_26_Curves->vtbl->func_4704(&objData->curves)) {
            objData->state = WGBoulder_STATE_0_Stopped;
        }
        
        WGboulder_handle_motion(self);
        break;
    }
}

// offset: 0x1C4 | func: 2 | export: 2
void WGboulder_update(Object *self) { }

// offset: 0x1D0 | func: 3 | export: 3
void WGboulder_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x224 | func: 4 | export: 4
void WGboulder_free(Object *self, s32 onlySelf) { }

// offset: 0x234 | func: 5 | export: 5
u32 WGboulder_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x244 | func: 6 | export: 6
u32 WGboulder_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(WGBoulder_Data);
}

// offset: 0x258 | func: 7
void WGboulder_handle_motion(Object* self) {
    WGBoulder_Data* objData;
    Func_80057F1C_Struct** samples;
    s32 count;
    s32 sampleIdx;
    s32 i;
    f32 minDistance = 10000.0f;
    f32 magnitude;
    f32 dy;
    f32 dx;
    f32 dz;
    
    objData = self->data;
    
    //Roll along curve
    self->velocity.y -= 0.07f * gUpdateRateF;
    self->velocity.y *= 0.97f;
    dx = self->srt.transl.x - objData->curves.unk0.unk68.x;
    dz = self->srt.transl.z - objData->curves.unk0.unk68.z;
    self->srt.yaw = arctan2_f(objData->curves.unk0.unk74, objData->curves.unk0.unk7C) + M_180_DEGREES;
    
    magnitude = sqrtf(SQ(dx) + SQ(dz));
    magnitude -= 60.0f;
    if (!(magnitude > 0)) {
        magnitude = 0;
    }
    
    objData->prevYaw = self->srt.yaw;
    self->srt.pitch -= magnitude * 80.0f;
    magnitude = magnitude * 0.001f;
    self->velocity.x = (objData->curves.unk0.unk68.x - self->srt.transl.x) * magnitude;
    self->velocity.z = (objData->curves.unk0.unk68.z - self->srt.transl.z) * magnitude;
    self->srt.yaw += self->velocity.x * 1200.0f;
    obj_move(self, self->velocity.x * gUpdateRateF, self->velocity.y * gUpdateRateF, self->velocity.z * gUpdateRateF);
    
    //Look for closest ground
    count = func_80057F1C(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &samples, 0, 0);
    for (i = 0, sampleIdx = 0; i < count; i++) {
        dy = (self->srt.transl.y - 35.0f) - samples[i]->unk0[0];
        if (dy < 0.0f) {
            dy *= -1.0f;
        }
        if (dy < minDistance) {
            sampleIdx = i;
            minDistance = dy;
        }
    }    

    //Bounce off/snap to the ground
    if ((self->srt.transl.y - 35.0f) < samples[sampleIdx]->unk0[0]) {
        self->srt.transl.y = samples[sampleIdx]->unk0[0] + 35.0f;
        if (self->velocity.y < 0.0f) {
            self->velocity.y *= -0.9f;
        }
    }
}
