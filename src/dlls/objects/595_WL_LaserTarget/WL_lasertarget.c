#include "game/objects/setups/51_Generic.h"
#include "common.h"

typedef struct {
    s16 timer;
    u8 switchedOn;
} WL_LaserTarget_Data;

// offset: 0x0 | ctor
void WL_LaserTarget_ctor(void* dll) { }

// offset: 0xC | dtor
void WL_LaserTarget_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_LaserTarget_setup(Object* self, Generic_Setup* objSetup, s32 reset) {
    WL_LaserTarget_Data* objData = self->data;
    
    self->modelInstIdx = mainGetBits(objSetup->bitA);
    objData->timer = objSetup->genericA;
    objData->switchedOn = FALSE;
}

// offset: 0x84 | func: 1 | export: 1
void WL_LaserTarget_control(Object* self) {
    Generic_Setup* objSetup;
    WL_LaserTarget_Data* objData;

    objSetup = (Generic_Setup*)self->setup;
    objData = self->data;
    
    //React to any source of damage
    if (func_80025F40(self, NULL, NULL, NULL)) {
        objData->switchedOn = TRUE;
        objData->timer = objSetup->genericA;
    }
    
    //Reset after timer runs out
    if ((objData->timer <= 0) && objData->switchedOn) {
        if (mainGetBits(objSetup->bitA)) {
            objSetModel(self, 0);
            mainSetBits(objSetup->bitA, FALSE);
            mainSetBits(objSetup->bitB, FALSE);
        } else {
            objSetModel(self, 1);
            mainSetBits(objSetup->bitA, TRUE);
            mainSetBits(objSetup->bitB, TRUE);
        }
        objData->switchedOn = FALSE;
        objData->timer = objSetup->genericA;
        return;
    }
    
    //Run down timer until reset
    if (objData->timer > 0) {
        objData->timer -= gUpdateRate;
    }
}

// offset: 0x1DC | func: 2 | export: 2
void WL_LaserTarget_update(Object* self) { }

// offset: 0x1E8 | func: 3 | export: 3
void WL_LaserTarget_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x23C | func: 4 | export: 4
void WL_LaserTarget_free(Object* self, s32 onlySelf) { }

// offset: 0x24C | func: 5 | export: 5
u32 WL_LaserTarget_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x25C | func: 6 | export: 6
u32 WL_LaserTarget_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(WL_LaserTarget_Data);
}
