#include "common.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 onDuration;
    s16 unk1C;
    s16 gamebitA;
    s16 gamebitB;
} WL_LaserTarget_Setup;

typedef struct {
    s16 timer;
    u8 switchedOn;
} WL_LaserTarget_Data;

// offset: 0x0 | ctor
void WL_LaserTarget_ctor(void* dll) { }

// offset: 0xC | dtor
void WL_LaserTarget_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_LaserTarget_setup(Object* self, WL_LaserTarget_Setup* objSetup, s32 reset) {
    WL_LaserTarget_Data* objData = self->data;
    
    self->modelInstIdx = main_get_bits(objSetup->gamebitA);
    objData->timer = objSetup->onDuration;
    objData->switchedOn = FALSE;
}

// offset: 0x84 | func: 1 | export: 1
void WL_LaserTarget_control(Object* self) {
    WL_LaserTarget_Setup* objSetup;
    WL_LaserTarget_Data* objData;

    objSetup = (WL_LaserTarget_Setup*)self->setup;
    objData = self->data;
    
    //React to any source of damage
    if (func_80025F40(self, NULL, NULL, NULL)) {
        objData->switchedOn = TRUE;
        objData->timer = objSetup->onDuration;
    }
    
    //Reset after timer runs out
    if ((objData->timer <= 0) && objData->switchedOn) {
        if (main_get_bits(objSetup->gamebitA)) {
            obj_set_model(self, 0);
            main_set_bits(objSetup->gamebitA, FALSE);
            main_set_bits(objSetup->gamebitB, FALSE);
        } else {
            obj_set_model(self, 1);
            main_set_bits(objSetup->gamebitA, TRUE);
            main_set_bits(objSetup->gamebitB, TRUE);
        }
        objData->switchedOn = FALSE;
        objData->timer = objSetup->onDuration;
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
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
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
