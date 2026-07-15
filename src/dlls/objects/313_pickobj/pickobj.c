#include "common.h"
#include "game/objects/object.h"
#include "macros.h"

typedef struct {
    ObjSetup base;
    u8 yaw;
    u8 modelIdx;
    s16 pickupParam;
} PickObj_Setup;

typedef struct {
    Pickup pickupData;
} PickObj_Data;

// offset: 0x0 | ctor
void PickObj_ctor(void* dll) { }

// offset: 0xC | dtor
void PickObj_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void PickObj_setup(Object* self, PickObj_Setup* objSetup, s32 reset) {
    Pickup* objData = self->data;
    
    self->srt.yaw = objSetup->yaw << 8;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
    
    self->modelInstIdx = objSetup->modelIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        //@typo (SEQOBJ.c), likely copied from DLL 309 without updating filename
        STUBBED_PRINTF("SEQOBJ.c: modelno out of range romdefno=%d\n", self->def->numModels);
        self->modelInstIdx = 0;
    }
    gDLL_54_pickup->vtbl->setup(self, objData, objSetup->pickupParam);
}

// offset: 0xB0 | func: 1 | export: 1
void PickObj_control(Object* self) {
    gDLL_54_pickup->vtbl->control(self, self->data);
}

// offset: 0xF8 | func: 2 | export: 2
void PickObj_update(Object* self) { }

// offset: 0x104 | func: 3 | export: 3
void PickObj_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (gDLL_54_pickup->vtbl->should_print(self, visibility)) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x194 | func: 4 | export: 4
void PickObj_free(Object* self, s32 onlySelf) {
    gDLL_54_pickup->vtbl->free(self);
}

// offset: 0x1DC | func: 5 | export: 5
u32 PickObj_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1EC | func: 6 | export: 6
u32 PickObj_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(PickObj_Data);
}
