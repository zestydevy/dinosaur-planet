#include "common.h"
#include "dlls/engine/6_amsfx.h"

typedef enum {
    DBSpike_MODEL_Regular = 0,
    DBSpike_MODEL_Motion_Smear = 1
} DBSpike_Models;

typedef enum {
    DBSpike_STATE_Retracted = 0,
    DBSpike_STATE_Sprung = 1
} DBSpike_States;

typedef enum {
    DBSpike_FLAG_None = 0,
    DBSpike_FLAG_Moving = 1
} DBSpike_Flags;

typedef struct {
    ObjSetup base;
    s8 roll;
    s8 unused19;
    s16 yaw;
    s16 unused1C;
    s16 gamebit;
} DBSpike_Setup;

typedef struct {
    u32 soundHandle;
    s16 rollGoal;
    u8 state;
    u8 flags;
    s8 motionBlurTimer;
} DBSpike_Data;

/*0x0*/ static s16 dRollGoals[2] = {0xe000, 0x2000};
/*0x4*/ static s16 dRollSpeeds[2] = {0xffc0, 0x0800};

// offset: 0x0 | ctor
void DBSpike_ctor(void *dll) { }

// offset: 0xC | dtor
void DBSpike_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBSpike_setup(Object* self, DBSpike_Setup* objSetup, s32 arg2) {
    DBSpike_Data* objData = self->data;
    
    self->srt.roll = objSetup->roll << 8;
    self->srt.yaw = objSetup->yaw;
    
    objData->motionBlurTimer = -56;
    objData->rollGoal = self->srt.roll;
    objData->state = DBSpike_STATE_Retracted;
    objData->flags = DBSpike_FLAG_None;
    
    func_8002674C(self);
}

// offset: 0x84 | func: 1 | export: 1
void DBSpike_control(Object* self) {
    DBSpike_Data* objData;
    DBSpike_Setup* objSetup;

    objData = self->data;
    objSetup = (DBSpike_Setup*)self->setup;
    
    if ((objData->flags & DBSpike_FLAG_Moving) == FALSE) {        
        //STOPPED

        //Rotate to goal position in averaged half-steps
        if (self->srt.roll != objData->rollGoal) {
            self->srt.roll = (objData->rollGoal + self->srt.roll) >> 1;
        }
        
        //Switch from motion blur model to regular model
        if (self->modelInstIdx != DBSpike_MODEL_Regular) {
            objData->motionBlurTimer -= gUpdateRate;
            if (objData->motionBlurTimer < 0) {
                func_80023A18(self, DBSpike_MODEL_Regular);
            }
        }
        
        //Check gamebit for change in state
        if (main_get_bits(objSetup->gamebit) != objData->state) {
            objData->state = main_get_bits(objSetup->gamebit);
            objData->flags |= DBSpike_FLAG_Moving;
            objData->rollGoal = dRollGoals[objData->state] + self->srt.roll;

            if (objData->state != DBSpike_STATE_Retracted) {
                //Sprung
                func_80023A18(self, DBSpike_MODEL_Motion_Smear);
                objData->motionBlurTimer = -56;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_A2A_Heavy_Swing, MAX_VOLUME, 0, 0, 0, 0);
            } else {
                //Retracting
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_A29_Wooden_Ratcheting_Loop, MAX_VOLUME, &objData->soundHandle, 0, 0, 0);
            }
        }
    } else {
        //MOVING

        //Set up collision (model 0 just pushes, model 1 hurts)
        func_80026128(self, 8, 1, 0);
        func_80026128(self, 8, 1, 1);
        
        //Rotate (different speeds when sprung vs. resetting)
        self->srt.roll += dRollSpeeds[objData->state];

        //Finish rotating
        if ((self->srt.roll < objData->rollGoal + 0x500) && 
            (self->srt.roll > objData->rollGoal - 0x500)
        ) {
            if (objData->soundHandle) {
                gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandle);
                objData->soundHandle = 0;
            }
            objData->flags &= ~DBSpike_FLAG_Moving;
        }

        //Keep gamebit in sync with spike's state
        if (main_get_bits(objSetup->gamebit) != objData->state) {
            main_set_bits(objSetup->gamebit, objData->state);
        }
    }
}

// offset: 0x328 | func: 2 | export: 2
void DBSpike_update(Object* self) {
    //Play sound when any kind of collision is detected
    if (func_80025F40(self, 0, 0, 0)) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_A28_Blade_Impact, MAX_VOLUME, 0, 0, 0, 0);
    }
}

// offset: 0x3AC | func: 3 | export: 3
void DBSpike_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x400 | func: 4 | export: 4
void DBSpike_free(Object *self, s32 a1) { }

// offset: 0x410 | func: 5 | export: 5
u32 DBSpike_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x420 | func: 6 | export: 6
u32 DBSpike_get_data_size(Object *self, u32 a1){
    return sizeof(DBSpike_Data);
}
