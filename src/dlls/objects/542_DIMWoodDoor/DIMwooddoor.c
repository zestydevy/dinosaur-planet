#include "common.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s16 unk1A;
    s16 unk1C;
    s16 gamebitDestroyed;
} DIMWoodDoor_Setup;

typedef struct {
    s8 health;
} DIMWoodDoor_Data;

// offset: 0x0 | ctor
void DIMWoodDoor_ctor(void* dll) { }

// offset: 0xC | dtor
void DIMWoodDoor_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIMWoodDoor_obj_Setup(Object* self, DIMWoodDoor_Setup* objSetup, s32 reset) {
    DIMWoodDoor_Data* objData;

    self->srt.yaw = objSetup->yaw << 8;
    self->stateFlags |= OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED;
    
    objData = self->data;
    objData->health = 3;
    
    //Check if already destroyed
    if (mainGetBits(objSetup->gamebitDestroyed)) {
        objData->health = 0;
        self->objhitInfo->unk58 &= ~1;
        self->opacity = 0;
    }
}

// offset: 0xAC | func: 1 | export: 1
void DIMWoodDoor_obj_Control(Object* self) {
    s32 modelIdx;
    s32 opacity;
    DIMWoodDoor_Data* objData;
    DIMWoodDoor_Setup* objSetup;
    ObjectPolyhits* polyhits;
    ObjectHitInfo* objhits;
    Object* obj;
    s32 hit;
    s32 i;

    objData = self->data;
    objSetup = (DIMWoodDoor_Setup*)self->setup;

    //Fade out when destroyed
    if (objData->health <= 0) {
        opacity = self->opacity - (gUpdateRate * 0x10);
        if (opacity < 0) {
            opacity = 0;
        }
        objhits = self->objhitInfo;
        objhits->unk58 &= ~1;
        self->opacity = (u8) opacity;
        return;
    }

    //Check for damage (SnowHorn tusk attacks or DIMCannonBall impacts)
    hit = FALSE;
    polyhits = self->polyhits;
    for (i = 0; i < polyhits->unk10F; i++) {
        obj = polyhits->unk100[i];
        if (obj->id == OBJ_DIMSnowHorn1 || obj->id == OBJ_DIMCannonBall) {
            hit = TRUE;
            break;
        }
    }

    //Take damage
    if (hit) {
        objData->health--;
        if (objData->health <= 0) {
            mainSetBits(objSetup->gamebitDestroyed, TRUE);
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_388_Wood_Smash, MAX_VOLUME, NULL, NULL, 0, NULL);
        } else {
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_387_Wood_Impact, MAX_VOLUME, NULL, NULL, 0, NULL);
        }

        //Change to progressively more damaged-looking models
        {
            modelIdx = 3 - objData->health;
            if (modelIdx > 2) {
                modelIdx = 2;
            }
        
            if (modelIdx != self->modelInstIdx) {
                objSetModel(self, modelIdx);
            }
        }
    }
}

// offset: 0x268 | func: 2 | export: 2
void DIMWoodDoor_obj_Update(Object* self) { }

// offset: 0x274 | func: 3 | export: 3
void DIMWoodDoor_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x2C8 | func: 4 | export: 4
void DIMWoodDoor_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x2D8 | func: 5 | export: 5
u32 DIMWoodDoor_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x2E8 | func: 6 | export: 6
u32 DIMWoodDoor_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DIMWoodDoor_Data);
}
