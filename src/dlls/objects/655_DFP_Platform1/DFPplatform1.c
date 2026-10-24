#include "common.h"
#include "game/objects/object.h"
#include "game/objects/setups/51_Generic.h"

typedef struct {
    s16 gamebitMoveZ;
    s16 gamebitMoveY;
    s8 unk4;
    s16 unk6;
} DFP_Platform1_Data;

// offset: 0x0 | ctor
void DFP_Platform1_ctor(void* dll) { }

// offset: 0xC | dtor
void DFP_Platform1_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFP_Platform1_obj_Setup(Object* self, DFP_Platform1_Setup* objSetup, s32 reset) {
    DFP_Platform1_Data* objData = self->data;
    
    self->srt.yaw = objSetup->yaw << 8;
    self->srt.roll = objSetup->roll;
    
    objData->gamebitMoveZ = objSetup->gamebitMoveZ;
    objData->gamebitMoveY = objSetup->gamebitMoveY;
    
    objData->unk4 = objSetup->unk19;

    if (mainGetBits(objData->gamebitMoveZ) == FALSE) {
        self->srt.transl.z += 150.0f;
    }
    
    if (mainGetBits(objData->gamebitMoveY) == FALSE) {
        self->srt.transl.y -= 200.0f;
    }
    
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0xF4 | func: 1 | export: 1
void DFP_Platform1_obj_Control(Object* self) {
    DFP_Platform1_Setup* objSetup;
    DFP_Platform1_Data* objData;

    objData = self->data;
    objSetup = (DFP_Platform1_Setup*)self->setup;

    if (mainGetBits(objData->gamebitMoveZ)) {
        if (self->srt.transl.z != objSetup->base.z) {
            self->srt.transl.z -= 2.0f * gUpdateRateF;
            if (self->srt.transl.z <= objSetup->base.z) {
                self->srt.transl.z = objSetup->base.z;
            }
        }
    }

    if (mainGetBits(objData->gamebitMoveY)) {
        if (self->srt.transl.y != objSetup->base.y) {
            self->srt.transl.y += 2.0f * gUpdateRateF;
            if (self->srt.transl.y >= objSetup->base.y) {
                self->srt.transl.y = objSetup->base.y;
            }
        }
    }
}

// offset: 0x208 | func: 2 | export: 2
void DFP_Platform1_obj_Update(Object* self) { }

// offset: 0x214 | func: 3 | export: 3
void DFP_Platform1_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x268 | func: 4 | export: 4
void DFP_Platform1_obj_Free(Object* self, s32 onlySelf) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x2B0 | func: 5 | export: 5
u32 DFP_Platform1_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x2C0 | func: 6 | export: 6
u32 DFP_Platform1_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DFP_Platform1_Data);
}
