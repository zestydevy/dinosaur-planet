#include "common.h"

typedef struct {
    ObjSetup base;
} DFwhirlpool_Setup;

typedef struct {
    u8 state;
    s16 yawSpeed;
    s32 soundHandle;
} DFwhirlpool_Data;

typedef enum {
    DFwhirlpool_STATE_Initial = 0,
    DFwhirlpool_STATE_Draining = 1,
    DFwhirlpool_STATE_Water_Lowered = 2
} DFwhirlpool_States;

#define WHIRLPOOL_LOWERED_HEIGHT 57.0f

#define WHIRLPOOL_SPEED_START 30
#define WHIRLPOOL_SPEED_MIN 10
#define WHIRLPOOL_SPEED_MAX 320
#define WHIRLPOOL_SPEED_STATE_THRESHOLD (WHIRLPOOL_SPEED_MAX - 20)

// offset: 0x0 | ctor
void DFwhirlpool_ctor(void *dll) { }

// offset: 0xC | dtor
void DFwhirlpool_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFwhirlpool_setup(Object* self, DFwhirlpool_Setup* objSetup, s32 arg2) {
    DFwhirlpool_Data* objData = self->data;
    
    //Check if already lowered
    if (main_get_bits(BIT_106)) {
        objData->state = DFwhirlpool_STATE_Water_Lowered;
        self->srt.transl.y -= WHIRLPOOL_LOWERED_HEIGHT;
        return;
    }
    
    objData->soundHandle = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_779_Water_Rushing_Loop, MAX_VOLUME, 0, 0, 0, 0);
}

// offset: 0xD4 | func: 1 | export: 1
void DFwhirlpool_control(Object* self) {
    DFwhirlpool_Data* objData;

    objData = self->data;

    switch (objData->state) {
    case DFwhirlpool_STATE_Initial:
        //Rotate at fixed speed
        self->srt.yaw -= gUpdateRate * WHIRLPOOL_SPEED_START;

        //Wait for cave wall to be demolished
        if (main_get_bits(BIT_DF_Whirlpool_Cave_Wall_Demolished)) {
            objData->state = DFwhirlpool_STATE_Draining;
            gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandle);
            objData->soundHandle = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_77A_Water_Draining_Loop, MAX_VOLUME, 0, 0, 0, 0);
            return;
        }
        break;
    case DFwhirlpool_STATE_Draining:
        //Accelerate rotation, and lower water level
        self->srt.yaw -= (objData->yawSpeed + WHIRLPOOL_SPEED_START) * gUpdateRate;
        objData->yawSpeed += gUpdateRate;
        self->srt.transl.y -= 0.2f * gUpdateRate;

        //Advance state based on yawSpeed threshold
        if (objData->yawSpeed > WHIRLPOOL_SPEED_STATE_THRESHOLD) {
            gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandle);
            objData->state = DFwhirlpool_STATE_Water_Lowered;
            objData->yawSpeed = WHIRLPOOL_SPEED_MAX;
            return;
        }
        break;
    case DFwhirlpool_STATE_Water_Lowered:
        //Decelerate rotation
        if (objData->yawSpeed > 0) {
            objData->yawSpeed -= gUpdateRate * 2;
        } else {
            objData->yawSpeed = 0;
        }
        
        //Continue rotating at low speed
        self->srt.yaw -= (objData->yawSpeed + WHIRLPOOL_SPEED_MIN) * gUpdateRate;
        break;
    }
}

// offset: 0x2D4 | func: 2 | export: 2
void DFwhirlpool_update(Object *self) { }

// offset: 0x2E0 | func: 3 | export: 3
void DFwhirlpool_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x334 | func: 4 | export: 4
void DFwhirlpool_free(Object *self, s32 a1) { }

// offset: 0x344 | func: 5 | export: 5
u32 DFwhirlpool_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x354 | func: 6 | export: 6
u32 DFwhirlpool_get_data_size(Object *self, u32 a1){
    return sizeof(DFwhirlpool_Data);
}
