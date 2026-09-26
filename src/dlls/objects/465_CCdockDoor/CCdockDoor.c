#include "common.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 _unk19;
    s16 _unk1A;
    s16 _unk1C;
    s16 gamebitDestroy; 
} CCDockDoor_Setup;

typedef struct {
    u8 destroyed;    
} CCDockDoor_Data;

// offset: 0x0 | ctor
void CCDockDoor_ctor(void* dll) { }

// offset: 0xC | dtor
void CCDockDoor_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCDockDoor_obj_Setup(Object* self, CCDockDoor_Setup* setup, s32 reset) {
    self->srt.yaw = setup->yaw << 8;
    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);
}

// offset: 0x3C | func: 1 | export: 1
void CCDockDoor_obj_Control(Object* self) {
    CCDockDoor_Data* objData;
    CCDockDoor_Setup* objSetup;

    objData = self->data;
    objSetup = (CCDockDoor_Setup*)self->setup;
    
    objData->destroyed = mainGetBits(objSetup->gamebitDestroy);

    if (objData->destroyed == TRUE) {
        self->srt.flags |= OBJFLAG_INVISIBLE;
        self->stateFlags |= OBJSTATE_CONTROL_DISABLED;
        func_800267A4(self);
    }
}

// offset: 0xCC | func: 2 | export: 2
void CCDockDoor_obj_Update(Object* self) { }

// offset: 0xD8 | func: 3 | export: 3
void CCDockDoor_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0xF0 | func: 4 | export: 4
void CCDockDoor_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x100 | func: 5 | export: 5
u32 CCDockDoor_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x110 | func: 6 | export: 6
u32 CCDockDoor_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CCDockDoor_Data);
}

