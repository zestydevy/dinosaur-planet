//Used by DFpulley and DFturbine

#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "game/gamebits.h"
#include "sys/objtype.h"
#include "dlls/objects/421_DFcradle.h"

typedef struct {
    ObjSetup base;
    u8 roll;
    u8 pitch;
    u8 yaw;
    u8 stateFlags;
    f32 scale;
} DFPulley_Setup;

typedef struct {
    u32 soundHandle;
    u16 rotateSpeed;
    u8 state;
} DFPulley_Data;

#define SOUND_RANGE 120.0f

// offset: 0x0 | ctor
void DFPulley_ctor(void* dll) { }

// offset: 0xC | dtor
void DFPulley_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFPulley_setup(Object* self, DFPulley_Setup* objSetup, s32 reset) {
    DFPulley_Data* objData = self->data;
    
    if (self->id == OBJ_DFpulley) {
        obj_add_object_type(self, OBJTYPE_Pulley);
    }
    
    self->srt.roll = objSetup->roll << 8;
    self->srt.pitch = objSetup->pitch << 8;
    self->srt.yaw = objSetup->yaw << 8;
    self->srt.scale *= objSetup->scale;
    
    self->stateFlags = objSetup->stateFlags;
    
    objData->state = main_get_bits(BIT_DF_Cradle_Powered);
    if (objData->state) {
        objData->rotateSpeed = 350;
    } else {
        objData->rotateSpeed = 0;
    }
}

// offset: 0x100 | func: 1 | export: 1
void DFPulley_control(Object* self) {
    DFCradle_Data* cradleData;
    DFPulley_Data* objData;
    Object* cradle;
    f32 distance;
    f32 temp_fv1;
    f32 pitch;

    objData = self->data;
    
    switch (objData->state) {
    case 0:
        objData->state = main_get_bits(BIT_DF_Cradle_Powered);
        return;
    case 1:
        //Turbine behaviour
        if (self->id == OBJ_DFturbine) {
            objData->state = 2;
            objData->soundHandle = gDLL_6_AMSFX->vtbl->play(self, 0x76F, MAX_VOLUME, NULL, NULL, 0, NULL);
            return;
        }

        //Pulley behaviour
        if (!(self->stateFlags & 2)) {
            cradle = func_800211B4(0x2CA7);
            cradleData = cradle->data;
            distance = vec3_distance(&self->globalPosition, &cradle->globalPosition);
            if (distance < SOUND_RANGE) {
                if (objData->soundHandle == 0) {
                    objData->soundHandle = gDLL_6_AMSFX->vtbl->play(self, 0x782, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
                
                if (objData->soundHandle) {
                    temp_fv1 = (cradleData->unk9C / 2.0f * 0.5f) + 0.5f;

                    if (temp_fv1 > 1.0f) {
                        pitch = 1.0f;
                    } else {
                        pitch = temp_fv1;
                    }
                    gDLL_6_AMSFX->vtbl->set_pitch(objData->soundHandle, pitch);
                    
                    if (cradleData->unkB6 != 0) {
                        pitch = 0.0f;
                    }
                    gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandle, (((SOUND_RANGE - distance) / SOUND_RANGE) * MAX_VOLUME * pitch));
                    return;
                }
            } else {
                if (objData->soundHandle) {
                    gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
                    objData->soundHandle = 0;
                    return;
                }
            }
        } else {
            return;
        }
        break;
    case 2:
        //Turbine behaviour
        if (objData->rotateSpeed < 350) {
            objData->rotateSpeed += 30;
        }
        self->srt.pitch -= objData->rotateSpeed * gUpdateRateF;
        break;
    }
}

// offset: 0x4B0 | func: 2 | export: 2
void DFPulley_update(Object* self) { }

// offset: 0x4BC | func: 3 | export: 3
void DFPulley_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x510 | func: 4 | export: 4
void DFPulley_free(Object* self, s32 onlySelf) {
    DFPulley_Data* objData = self->data;

    if (self->id == OBJ_DFpulley) {
        obj_free_object_type(self, OBJTYPE_Pulley);
    }
    
    if (objData->soundHandle != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
        objData->soundHandle = 0;
    }
}

// offset: 0x5A0 | func: 5 | export: 5
u32 DFPulley_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x5B0 | func: 6 | export: 6
u32 DFPulley_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DFPulley_Data);
}
