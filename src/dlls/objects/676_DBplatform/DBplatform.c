#include "common.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 yOffset;
    s16 quarterVisRadius;
    s16 transitionDuration;
    s16 gamebitStandingOn;
    s16 gamebitStateTransition;
} DBPlatform_Setup;

typedef struct {
    s32 timer;
    u32 unused4;
    u8 state;
} DBPlatform_Data;

typedef enum {
    DBPlatform_STATE_0,
    DBPlatform_STATE_1,
    DBPlatform_STATE_2,
    DBPlatform_STATE_3
} DBPlatform_States;

// offset: 0x0 | ctor
void DBPlatform_ctor(void* dll) { }

// offset: 0xC | dtor
void DBPlatform_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBPlatform_setup(Object* self, DBPlatform_Setup* objSetup, s32 reset) {
    DBPlatform_Data* objData;
    ObjectHitInfo* objHitInfo;

    objData = self->data;
    
    if (mainGetBits(objSetup->gamebitStandingOn)) {
        objData->state = DBPlatform_STATE_0;
    } else {
        objData->state = DBPlatform_STATE_3;
    }
    
    self->srt.transl.y = objSetup->base.y;
    self->srt.transl.x = objSetup->base.x;
    self->srt.transl.z = objSetup->base.z;
    
    objHitInfo = self->objhitInfo;
    objHitInfo->unk52 = objSetup->quarterVisRadius;
    self->visRadius = objSetup->quarterVisRadius * 4.0f;
    
    objHitInfo->unk56 = 100;
    objHitInfo->unk54 = -100;
    objHitInfo->unk5F = 8;
    objHitInfo->unk60 = 1;
    objHitInfo->unk58 |= 1;
    objHitInfo->unk44 = 0x10;
    objHitInfo->unk40 = 0x10;
}

// offset: 0x104 | func: 1 | export: 1
void DBPlatform_control(Object* self) {
    DBPlatform_Setup* objSetup;
    DBPlatform_Data* objData;
    Object* player;
    f32 playerY;
    f32 selfYOffset;

    objData = self->data;
    objSetup = (DBPlatform_Setup*)self->setup;

    switch (objData->state) {
    case DBPlatform_STATE_3:
        self->objhitInfo->unk5A |= 2;

        if (mainGetBits(objSetup->gamebitStateTransition)) {
            objData->state = DBPlatform_STATE_2;
            objData->timer = objSetup->transitionDuration;
            return;
        }

        self->objhitInfo->unk58 |= 1;
        self->objhitInfo->unk5F = 8;
        self->objhitInfo->unk44 = 0x10;
        self->objhitInfo->unk40 = 0x10;
        self->objhitInfo->unk60 = 1;
        self->objhitInfo->unk58 |= 4;
        self->objhitInfo->unk58 |= 0x40;
        break;
    case DBPlatform_STATE_2:
        self->objhitInfo->unk5A &= ~2;

        objData->timer -= gUpdateRate;
        if (objData->timer < 0) {
            objData->state = DBPlatform_STATE_3;
            return;
        }

        player = objGetPlayer();
        if (vec3DistanceXZ(&player->globalPosition, &self->globalPosition) < objSetup->quarterVisRadius) {
            playerY = player->srt.transl.y;
            selfYOffset = self->srt.transl.y + objSetup->yOffset;
            if ((playerY < selfYOffset) && ((selfYOffset - 10.0f) < playerY)) {
                mainSetBits(objSetup->gamebitStandingOn, 1);
                objData->state = DBPlatform_STATE_1;
            }
        }
        break;
    case DBPlatform_STATE_1:
        self->srt.transl.y -= gUpdateRate * 0.5f;
        if ((objSetup->base.y - self->srt.transl.y) > 10.0f) {
            objData->state = DBPlatform_STATE_0;
        }
        break;
    case DBPlatform_STATE_0:
        self->srt.transl.y -= gUpdateRate * -0.005f;
        if (objSetup->base.y <= self->srt.transl.y) {
            self->srt.transl.y = objSetup->base.y;
            mainSetBits(objSetup->gamebitStandingOn, 0);
            objData->state = DBPlatform_STATE_3;
            self->objhitInfo->unk5A |= 2;
        }
        break;
    }
}

// offset: 0x408 | func: 2 | export: 2
void DBPlatform_update(Object* self) { }

// offset: 0x414 | func: 3 | export: 3
void DBPlatform_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x468 | func: 4 | export: 4
void DBPlatform_free(Object* self, s32 onlySelf) { }

// offset: 0x478 | func: 5 | export: 5
u32 DBPlatform_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x488 | func: 6 | export: 6
u32 DBPlatform_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DBPlatform_Data);
}

/*0x0*/ static const char str_0[] = " Removing Cylinder ";
