#include "common.h"
#include "game/objects/object.h"
#include "sys/objtype.h"
#include "dlls/objects/554_DIM2Conveyor.h"

typedef struct {
    f32 velocityX;
    f32 velocityZ;
    u32 soundHandle;
} DIM2Conveyor_Data;

// offset: 0x0 | ctor
void DIM2Conveyor_ctor(void* dll) { }

// offset: 0xC | dtor
void DIM2Conveyor_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIM2Conveyor_obj_Setup(Object* self, DIM2Conveyor_Setup* objSetup, s32 reset) {
    f32 speed;
    DIM2Conveyor_Data* objData;

    speed = objSetup->speed / 5.0f;
    self->srt.yaw = objSetup->yaw << 8;
    
    objData = self->data;
    objData->velocityX = mathSinfInterp(self->srt.yaw) * speed;
    objData->velocityZ = mathCosfInterp(self->srt.yaw) * speed;
    
    objAddObjectType(self, OBJTYPE_WindLiftable); // why this objtype??
    
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0xEC | func: 1 | export: 1
void DIM2Conveyor_obj_Control(Object* self) {
    DIM2Conveyor_Data* objData = self->data;
    
    if (objData->soundHandle == 0) {
        dll_amSfx->Play(self, SOUND_3C4_Conveyor_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
    }
}

// offset: 0x158 | func: 2 | export: 2
void DIM2Conveyor_obj_Update(Object* self) { }

// offset: 0x164 | func: 3 | export: 3
void DIM2Conveyor_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1B8 | func: 4 | export: 4
void DIM2Conveyor_obj_Free(Object* self, s32 onlySelf) {
    DIM2Conveyor_Data* objData;

    objFreeObjectType(self, OBJTYPE_WindLiftable);

    objData = self->data;
    if (objData->soundHandle) {
        dll_amSfx->Stop(objData->soundHandle);
    }
}

// offset: 0x230 | func: 5 | export: 5
u32 DIM2Conveyor_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x240 | func: 6 | export: 6
u32 DIM2Conveyor_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DIM2Conveyor_Data);
}

// offset: 0x254 | func: 7 | export: 7
void DIM2Conveyor_Move(Object* self, Object* objConveyed, f32 updateRate) {
    DIM2Conveyor_Data* objData = self->data;
    
    objMove(objConveyed, objData->velocityX * updateRate, 0.0f, objData->velocityZ * updateRate);
}
