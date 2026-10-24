#include "common.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/envfx.h"

typedef struct {
    f32 activateRange;
    s32 gamebitActivate;
    s32 gamebitActionIdxOffset;
    s32 prevActivated;
    s32 prevActionIdxOffset;
    s16 unk14;
    s16 actionIdxBase;
} EnvEmitter_Data;

// offset: 0x0 | ctor
void EnvEmitter_ctor(void* dll) { }

// offset: 0xC | dtor
void EnvEmitter_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void EnvEmitter_obj_Setup(Object* self, EnvEmitter_Setup* objSetup, s32 reset) {
    EnvEmitter_Data* objData = self->data;
    
    self->srt.yaw = 0;
    
    objData->activateRange = objSetup->activateRange * 4;
    objData->unk14 = objSetup->unk19;
    objData->actionIdxBase = objSetup->actionIdxBase;
    objData->gamebitActivate = objSetup->gamebitActivate;
    objData->gamebitActionIdxOffset = objSetup->gamebitActionIdxOffset;
    objData->prevActivated = -1;
    objData->prevActionIdxOffset = -1;
    
    self->srt.scale = 0.1f;
    self->unkDC = 0;
}

// offset: 0x8C | func: 1 | export: 1
void EnvEmitter_obj_Control(Object* self) {
    EnvEmitter_Data* objData;
    Vec3f delta;
    f32 distance;
    Object* player;
    s32 activated;
    s32 actionIdxOffset;

    objData = self->data;
    player = objGetPlayer();
    
    if (player == NULL) {
        return;
    }

    activated = mainGetBits(objData->gamebitActivate);
    actionIdxOffset = mainGetBits(objData->gamebitActionIdxOffset);
    
    if (activated == FALSE) {
        if (self->unkDC == 1) {
            envfxAction(self, player, (objData->actionIdxBase + 2), objData->activateRange);
        }
        return;
    }
    
    delta.f[0] = self->srt.transl.x - player->srt.transl.x;
    delta.f[2] = self->srt.transl.z - player->srt.transl.z;
    distance = sqrtf(SQ(delta.f[0]) + SQ(delta.f[2]));
    
    if ((distance <= objData->activateRange) && (self->unkDC == 0)) {
        envfxAction(self, player, (objData->actionIdxBase + (u8)actionIdxOffset), objData->activateRange);
        self->unkDC = 1;
    } else if ((distance > objData->activateRange) && (self->unkDC == 1)) {
        envfxAction(self, player, (objData->actionIdxBase + 2), objData->activateRange);
        self->unkDC = 0;
    } else if ((distance <= objData->activateRange) && (self->unkDC == 1) && (actionIdxOffset != objData->prevActionIdxOffset)) {
        envfxAction(self, player, (objData->actionIdxBase + 2), objData->activateRange);
        envfxAction(self, player, (objData->actionIdxBase + (u8)actionIdxOffset), objData->activateRange);
        self->unkDC = 0;
    }
    
    objData->prevActivated = activated;
    objData->prevActionIdxOffset = actionIdxOffset;
}

// offset: 0x2F4 | func: 2 | export: 2
void EnvEmitter_obj_Update(Object* self) { }

// offset: 0x300 | func: 3 | export: 3
void EnvEmitter_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x318 | func: 4 | export: 4
void EnvEmitter_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x328 | func: 5 | export: 5
u32 EnvEmitter_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x338 | func: 6 | export: 6
u32 EnvEmitter_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(EnvEmitter_Data);
}
