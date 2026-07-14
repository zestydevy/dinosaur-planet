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

typedef enum {
    STATE_0_Unpowered,          //Stopped
    STATE_1_Powered,            //Pulley: plays sound when nearby, Turbine: starts sound and advances state
    STATE_2_Spin_Turbine        //Turbine only: spins
} DFPulley_States;

#define SOUND_RANGE 120.0f

// offset: 0x0 | ctor
void DFPulley_ctor(void* dll) { }

// offset: 0xC | dtor
void DFPulley_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFPulley_setup(Object* self, DFPulley_Setup* objSetup, s32 reset) {
    DFPulley_Data* objData = self->data;
    
    if (self->id == OBJ_DFpulley) {
        objAddObjectType(self, OBJTYPE_Pulley);
    }
    
    self->srt.roll = objSetup->roll << 8;
    self->srt.pitch = objSetup->pitch << 8;
    self->srt.yaw = objSetup->yaw << 8;
    self->srt.scale *= objSetup->scale;
    
    self->stateFlags = objSetup->stateFlags;
    
    objData->state = mainGetBits(BIT_DF_Cradle_Powered);

    if (objData->state != STATE_0_Unpowered) {
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
    f32 pitch;
    f32 audioValue;

    objData = self->data;
    
    switch (objData->state) {
    case STATE_0_Unpowered:
        objData->state = mainGetBits(BIT_DF_Cradle_Powered);
        break;
    case STATE_1_Powered:
        //Turbine behaviour
        if (self->id == OBJ_DFturbine) {
            objData->state = STATE_2_Spin_Turbine;
            objData->soundHandle = gDLL_6_AMSFX->vtbl->play(self, SOUND_76F, MAX_VOLUME, NULL, NULL, 0, NULL);
            break;
        }

        //Pulley behaviour
        if (!(self->stateFlags & 2)) {
            cradle = objGetObjectByUID(0x2CA7);
            cradleData = cradle->data;
            distance = vec3_distance(&self->globalPosition, &cradle->globalPosition);

            //Play sound loop when nearby
            if (distance < SOUND_RANGE) {
                if (objData->soundHandle == 0) {
                    objData->soundHandle = gDLL_6_AMSFX->vtbl->play(self, SOUND_782, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
                
                //Adjust pitch and volume
                if (objData->soundHandle) {
                    pitch = (cradleData->unk9C / 2.0f * 0.5f) + 0.5f;

                    if (pitch > 1.0f) {
                        audioValue = 1.0f;
                    } else {
                        audioValue = pitch;
                    }
                    gDLL_6_AMSFX->vtbl->set_pitch(objData->soundHandle, audioValue);
                    
                    if (cradleData->unkB6 != 0) {
                        audioValue = 0.0f;
                    }
                    gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandle, (((SOUND_RANGE - distance) / SOUND_RANGE) * MAX_VOLUME * audioValue));
                }

            //Otherwise, stop sound
            } else if (objData->soundHandle) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
                objData->soundHandle = 0;
            }
        }
        break;
    case STATE_2_Spin_Turbine:
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
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x510 | func: 4 | export: 4
void DFPulley_free(Object* self, s32 onlySelf) {
    DFPulley_Data* objData = self->data;

    if (self->id == OBJ_DFpulley) {
        objFreeObjectType(self, OBJTYPE_Pulley);
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
