#include "common.h"
#include "game/objects/object.h"
#include "dlls/objects/532_DIMsnowball.h"

typedef struct {
    ObjSetup base;
    s16 snowballInterval;   //How many frames to wait between creating snowballs
    u8 unk1A;
    u8 unk1B;
    u8 yaw;
} DIMSnowBallGenerator_Setup;

typedef struct {
    s16 timer;              //Countdown until a snowball is created
    s16 interval;           //How many frames to wait between creating snowballs
} DIMSnowBallGenerator_Data;

// offset: 0x0 | ctor
void DIMSnowBallGenerator_ctor(void* dll) { }

// offset: 0xC | dtor
void DIMSnowBallGenerator_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIMSnowBallGenerator_obj_Setup(Object* self, DIMSnowBallGenerator_Setup* objSetup, s32 reset) {
    DIMSnowBallGenerator_Data* objData = self->data;
    
    self->srt.yaw = objSetup->yaw << 8;
    
    objData->interval = objSetup->snowballInterval;
    objData->timer = objSetup->snowballInterval;
    
    self->stateFlags |= OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x50 | func: 1 | export: 1
void DIMSnowBallGenerator_obj_Control(Object* self) {
    DIMSnowBall_Setup* snowballSetup;
    DIMSnowBallGenerator_Setup* creatorSetup;
    DIMSnowBallGenerator_Data* objData;

    objData = self->data;

    //Wait out the interval between snowballs
    objData->timer -= gUpdateRate;
    if (objData->timer > 0) {
        return;
    }    

    //Create a snowball
    creatorSetup = (DIMSnowBallGenerator_Setup*)self->setup;
    snowballSetup = (DIMSnowBall_Setup*)objAllocSetup(sizeof(DIMSnowBall_Setup), OBJ_DIMSnowBall);
    snowballSetup->base.loadFlags = creatorSetup->base.loadFlags;
    snowballSetup->base.loadDistance = creatorSetup->base.loadDistance;
    snowballSetup->base.fadeFlags = creatorSetup->base.fadeFlags;
    snowballSetup->base.fadeDistance = creatorSetup->base.fadeDistance;
    snowballSetup->base.x = self->srt.transl.x;
    snowballSetup->base.y = self->srt.transl.y;
    snowballSetup->base.z = self->srt.transl.z;
    snowballSetup->base.uID = creatorSetup->base.uID;
    snowballSetup->unk18 = creatorSetup->yaw;
    snowballSetup->unk1A = creatorSetup->unk1A;
    snowballSetup->unk1C = mathRnd(0, 100) / 100.0f + creatorSetup->unk1B;
    objSetupObject(&snowballSetup->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, NULL);
    
    objData->timer = objData->interval;
}

// offset: 0x1B4 | func: 2 | export: 2
void DIMSnowBallGenerator_obj_Update(Object* self) { }

// offset: 0x1C0 | func: 3 | export: 3
void DIMSnowBallGenerator_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x214 | func: 4 | export: 4
void DIMSnowBallGenerator_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x224 | func: 5 | export: 5
u32 DIMSnowBallGenerator_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x234 | func: 6 | export: 6
u32 DIMSnowBallGenerator_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DIMSnowBallGenerator_Data);
}
