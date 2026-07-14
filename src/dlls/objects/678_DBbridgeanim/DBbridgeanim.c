#include "common.h"
#include "dlls/objects/677_DBlgtbeam.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 gamebitStart;
} DBbridgeanim_Setup;

typedef struct {
    u8 state;
    s32 timer;
    s8 stepNum;
} DBbridgeanim_Data;

typedef enum {
    STATE_0_Bridge_Off,
    STATE_1_Bridge_On
} DBbridgeanim_States;

/*0x0*/ static s32 dStepDurations[] = {
    550, 
    350, 
    350, 
    350, 
    350, 
    350, 
    350, 
    350, 
    350, 
    350, 
    8, 
    8
};

static void DBbridgeanim_create_step(s32 stepNum);
static void DBbridgeanim_remove_step(s32 stepNum);

// offset: 0x0 | ctor
void DBbridgeanim_ctor(void* dll) { }

// offset: 0xC | dtor
void DBbridgeanim_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBbridgeanim_setup(Object* self, DBbridgeanim_Setup* arg1, s32 reset) {
    DBbridgeanim_Data* objData = self->data;
    
    objData->state = STATE_0_Bridge_Off;
    objData->timer = 0;
    objData->stepNum = 0;
}

// offset: 0x38 | func: 1 | export: 1
void DBbridgeanim_control(Object* self) {
    DBbridgeanim_Setup* objSetup;
    DBbridgeanim_Data* objData;
    s32 gamebitValue;

    objSetup = (DBbridgeanim_Setup*)self->setup;
    objData = self->data;
    
    if (joyGetPressed(0) & B_BUTTON) {
        mainSetBits(objSetup->gamebitStart, TRUE);
    }
    
    switch (objData->state) {
    case STATE_0_Bridge_Off:
        if (mainGetBits(objSetup->gamebitStart)) {
            objData->state = STATE_1_Bridge_On;
            DBbridgeanim_create_step(1);
            objData->timer = 0;
            objData->stepNum = 1;
            mainSetBits(objSetup->gamebitStart, FALSE);
            return;
        }
        return;
    case STATE_1_Bridge_On:
        diPrintf(" On ");
        
        gamebitValue = mainGetBits(BIT_82B);
        if (gamebitValue && (gamebitValue >= objData->stepNum)) {
            STUBBED_PRINTF(" Creating Step Number %i ", gamebitValue);
            DBbridgeanim_create_step(++gamebitValue);
            mainSetBits(BIT_82B, 0);

            objData->timer = 0;
            
            if (objData->stepNum > gamebitValue) {
                objData->stepNum = objData->stepNum;
            } else {
                objData->stepNum = gamebitValue;
            }
        }

        objData->timer += gUpdateRate;
        if (objData->timer > dStepDurations[objData->stepNum]) {
            STUBBED_PRINTF(" Removing Step %i ", objData->stepNum);
            DBbridgeanim_remove_step(objData->stepNum--);

            objData->timer = 0;
            if (objData->stepNum == 0) {
                objData->state = STATE_0_Bridge_Off;
            }
        }
        break;
    }
}
// offset: 0x224 | func: 2 | export: 2
void DBbridgeanim_update(Object* self) { }

// offset: 0x230 | func: 3 | export: 3
void DBbridgeanim_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x284 | func: 4 | export: 4
void DBbridgeanim_free(Object* self, s32 onlySelf) { }

// offset: 0x294 | func: 5 | export: 5
u32 DBbridgeanim_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x2A4 | func: 6 | export: 6
u32 DBbridgeanim_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DBbridgeanim_Data);
}

// offset: 0x2B8 | func: 7
void DBbridgeanim_create_step(s32 stepNum) {
    Object** objects;
    s32 i;
    s32 count;

    objects = objGetAllOfType(OBJTYPE_DBlgtbeam, &count);
    for (i = 0; i < count; i++){
        if (((DLL_677_DBlgtbeam*)objects[i]->dll)->vtbl->func9(objects[i], stepNum)) {
            ((DLL_677_DBlgtbeam*)objects[i]->dll)->vtbl->func7(objects[i], 5);
            i = count;
        }
    }
}

// offset: 0x38C | func: 8
void DBbridgeanim_remove_step(s32 stepNum) {
    Object** objects;
    s32 i;
    s32 count;

    objects = objGetAllOfType(OBJTYPE_DBlgtbeam, &count);
    for (i = 0; i < count; i++){
        if (((DLL_677_DBlgtbeam*)objects[i]->dll)->vtbl->func9(objects[i], stepNum)) {
            ((DLL_677_DBlgtbeam*)objects[i]->dll)->vtbl->func7(objects[i], 6);
            i = count;
        }
    }
}
