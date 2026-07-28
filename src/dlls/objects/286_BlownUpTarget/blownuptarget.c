#include "common.h"
#include "game/objects/object.h"
#include "sys/objhits.h"

typedef struct {
    ObjSetup base;
    s16 gamebit;            //Gamebit to affect when damaged by an explosion
    s16 timerDuration;      //For Mode 2: how long (in 10ths of a second) until the gamebit unsets after an explosion
    s8 unk1C;
    u8 scale;               //Target scale
    u8 mode;                //Which gamebit setting mode to use (see `BlownUpTarget_Modes`)
} BlownUpTarget_Setup;

typedef struct {
    u8 gamebitValue;        //Current value of the target's gamebit
    u16 timer;              //For Mode 2: how long (in frames) until the gamebit unsets after an explosion
} BlownUpTarget_Data;

typedef enum {
    BlownUpTarget_MODE_0_Damage_Sets_Gamebit,               //Damaging the target sets the gamebit
    BlownUpTarget_MODE_1_Damage_Toggles_Gamebit,            //Damaging the target toggles the gamebit on/off
    BlownUpTarget_MODE_2_Damage_Sets_Gamebit_Temporarily    //Damaging the target sets the gamebit, and starts a countdown to unsetting the gamebit
} BlownUpTarget_Modes;

// offset: 0x0 | ctor
void BlownUpTarget_ctor(void* dll) { }

// offset: 0xC | dtor
void BlownUpTarget_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void BlownUpTarget_obj_Setup(Object* self, BlownUpTarget_Setup* objSetup, s32 reset) {
    BlownUpTarget_Data* objData = self->data;
    
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
    
    if (objSetup->scale == 0) {
        objSetup->scale = 64;
    }
    self->srt.scale = objSetup->scale * self->def->scale / 64.0f;
    
    func_80026940(self, (objSetup->scale * self->def->hitbox_flagsB6) / 64);

    //Get the gamebit's state
    objData->gamebitValue = mainGetBits(objSetup->gamebit);
}

// offset: 0x108 | func: 1 | export: 1
void BlownUpTarget_obj_Control(Object* self) {
    BlownUpTarget_Setup* objSetup;
    BlownUpTarget_Data* objData;

    objData = self->data;
    objSetup = (BlownUpTarget_Setup*)self->setup;
    
    //Check if the target's gamebit is unset, and sync with it
    if ((objData->gamebitValue != FALSE) && (mainGetBits(objSetup->gamebit) == FALSE)) {
        objData->gamebitValue = FALSE;
    }

    //Handle countdown to unsetting the gamebit (mode 2 only)
    if (objData->timer) {
        if (objData->timer <= gUpdateRate) {
            objData->timer = 0;
            mainSetBits(objSetup->gamebit, FALSE);
        } else {
            objData->timer -= gUpdateRate;
        }
    }

    //Check for explosive damage
    if (func_80025F40(self, NULL, NULL, NULL) == Damage_Type_Explosion) {
        if (objData->gamebitValue) {
            //If Mode 1 is in use, unset the gamebit when damaged
            if (objSetup->mode == BlownUpTarget_MODE_1_Damage_Toggles_Gamebit) {
                objData->gamebitValue = FALSE;
                mainSetBits(objSetup->gamebit, FALSE);
            }
        } else {
            //Set the gamebit when damaged
            objData->gamebitValue = TRUE;
            mainSetBits(objSetup->gamebit, TRUE);

            //If Mode 2 is in use, start a countdown to unsetting the gamebit
            if (objSetup->mode == BlownUpTarget_MODE_2_Damage_Sets_Gamebit_Temporarily) {
                objData->timer = objSetup->timerDuration * 0.1f * 60.0f;
            }
        }
    }
}

// offset: 0x300 | func: 2 | export: 2
void BlownUpTarget_obj_Update(Object* self) { }

// offset: 0x30C | func: 3 | export: 3
void BlownUpTarget_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x324 | func: 4 | export: 4
void BlownUpTarget_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x334 | func: 5 | export: 5
u32 BlownUpTarget_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x344 | func: 6 | export: 6
u32 BlownUpTarget_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(BlownUpTarget_Data);
}
